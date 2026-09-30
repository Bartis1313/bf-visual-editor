#include "enlighten_relight.h"
#include "enlighten_internal.h"
#include "enlighten_db.h"
#include "enlighten_look.h"
#include "enlighten_scene.h"
#include "../editor_context.h"
#include "../states/states.h"
#include "../render/render.h"
#include "../textures/viewsubst.h"
#include "../../hooks/functions.h"
#include "../../utils/log.h"

#include <d3d11.h>
#include <imgui.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <mutex>
#include <string>
#include <vector>

namespace editor::enlighten::relight
{
    using fb::Vec3;
    using fb::vec3;

    namespace
    {
        struct Light
        {
            Vec3 sky{}, ground{}, sunDisc{}, sun{};
            float sunDiscSize = 0.0f, sunScale = 0.0f, bounce = 0.0f;
            bool operator==(const Light& o) const { return std::memcmp(this, &o, sizeof(Light)) == 0; }
        };

        std::mutex g_mutex;
        Light g_now, g_ref;
        bool g_haveNow = false, g_haveRef = false;

        constexpr float NO_VISIBILITY = 0.5f; // no sky visibility atlas: half sky, half sun
        constexpr float NO_DIRECTION_UP = 0.75f; // up weight when a texel has no direction (dir.y 0.5)
        constexpr float MIN_DIR_LENGTH = 0.05f; // shorter shipped direction = no dominant one
        constexpr size_t EXPOSURE_PERCENTILE = 99; // atlas view exposure = 99th percentile channel peak
        constexpr float MIN_EXPOSURE = 1e-4f;
        constexpr float RATIO_EPS = 1e-6f; // reference color channel treated as 0
        constexpr float BOUNCE_EPS = 1e-4f;
        constexpr float BOUNCE_SHARE = 0.5f; // share of the non-sky light that BounceScale scales
        constexpr int REF_UPDATES = 4; // VE updates for the restored states to blend before the reference is read
        constexpr int RETIRE_FRAMES = 3; // copies may still be bound by a deferred context

        // 0 none, 1 suspend requested, 2 counting VE updates, 3 resume
        int g_refPhase = 0, g_refUpdates = 0;
        bool g_refOverrides = true;

        uint32_t W = 0, H = 0;
        std::vector<Vec3> g_rgb;
        std::vector<float> g_vis, g_up;
        std::vector<uint8_t> g_valid;
        std::vector<Vec3> g_out;
        float g_exposure = 1.0f;
        bool g_loaded = false, g_loadFailed = false;
        std::string g_note;

        bool g_installed = false;

        // the level's texture when DEFAULT, else a copy (level textures are IMMUTABLE) swapped in at bind
        struct Target
        {
            ID3D11Texture2D* res = nullptr;
            bool copy = false;
            ID3D11Resource* level = nullptr; // compared only: re-created by streaming -> reinstall
            ID3D11ShaderResourceView* levelViews[2] = {};
            ID3D11ShaderResourceView* views[2] = {}; // copy only
            uint32_t width = 0, bpp = 0, mips = 0;
            int bx = 0, by = 1;
            std::vector<std::vector<uint8_t>> original; // per mip, tightly packed
        };
        Target g_luma, g_chroma;

        struct Retired
        {
            Target t;
            int frames;
        };
        std::mutex g_restoreMutex;
        std::vector<Retired> g_restores;

        Light g_applied{};
        float g_appliedScale = 1.0f;
        uint32_t g_appliedLook = 0;
        bool g_hasApplied = false;
        Vec3 g_levelRatio{ 1.0f, 1.0f, 1.0f };

        bool g_viewOpen = false, g_viewRelit = true, g_viewDirty = true;
        ID3D11Texture2D* g_viewTex = nullptr;
        ID3D11ShaderResourceView* g_viewSrv = nullptr;

        Light read(fb::VisualEnvironment* ve)
        {
            Light l;
            l.sky = ve->enlighten.m_SkyBoxSkyColor;
            l.ground = ve->enlighten.m_SkyBoxGroundColor;
            l.sunDisc = ve->enlighten.m_SkyBoxSunLightColor;
            l.sunDiscSize = ve->enlighten.m_SkyBoxSunLightColorSize;
            l.sunScale = ve->enlighten.m_SunScale;
            l.bounce = ve->enlighten.m_BounceScale;
            l.sun = ve->outdoorLight.m_SunColor;
            return l;
        }

        bool load()
        {
            std::string err;
            detail::Original& luma = detail::g_orig[0];
            detail::Original& chroma = detail::g_orig[1];
            detail::Original& dir = detail::g_orig[2];
            if (!detail::g_staticAsset) { g_note = "this level has no baked Enlighten atlas"; return false; }
            if (!detail::readback(luma, err) || !detail::readback(chroma, err) || luma.bpp != 2) { g_note = "atlas readback failed: " + err; return false; }
            W = luma.width;
            H = luma.height;
            if (chroma.width != W || chroma.height != H) { g_note = "chroma atlas size differs"; return false; }
            const bool haveDir = detail::readback(dir, err) && dir.width == W && dir.height == H && dir.bpp == 4;
            detail::Original vis;
            vis.tex = detail::g_skyVisTex;
            const bool haveVis = vis.tex && detail::readback(vis, err) && vis.bpp == 2 && vis.width && vis.height;
            int bx, by;
            detail::chromaBytes(chroma.dxgi, bx, by);
            const size_t n = size_t(W) * H;
            g_rgb.assign(n, Vec3{});
            g_vis.assign(n, NO_VISIBILITY);
            g_up.assign(n, NO_DIRECTION_UP);
            g_valid.assign(n, 0);
            std::vector<float> peaks;
            const uint16_t* L = reinterpret_cast<const uint16_t*>(luma.pixels.data());
            for (size_t t = 0; t < n; ++t)
            {
                if (!L[t]) continue;
                const float l = L[t] / 65535.0f;
                const uint8_t* c = chroma.pixels.data() + t * chroma.bpp;
                // luma = r + g + b, chroma = blue and red share, green the rest
                const float blue = c[bx] / 255.0f, red = c[by] / 255.0f;
                g_rgb[t] = vec3(l * red, l * (std::max)(0.0f, 1.0f - red - blue), l * blue);
                g_valid[t] = 1;
                peaks.push_back((std::max)({ g_rgb[t].m_x, g_rgb[t].m_y, g_rgb[t].m_z }));
                if (haveDir)
                {
                    const uint8_t* d = dir.pixels.data() + t * 4;
                    const Vec3 v = vec3(d[0] / 127.5f - 1.0f, d[1] / 127.5f - 1.0f, d[2] / 127.5f - 1.0f);
                    const float len = fb::length(v);
                    if (len > MIN_DIR_LENGTH) g_up[t] = (std::clamp)(0.5f + 0.5f * v.m_y / len, 0.0f, 1.0f);
                }
                if (haveVis)
                {
                    const uint16_t* V = reinterpret_cast<const uint16_t*>(vis.pixels.data());
                    const float fx = (float(t % W) + 0.5f) * vis.width / W - 0.5f, fy = (float(t / W) + 0.5f) * vis.height / H - 0.5f;
                    const int x0 = (std::clamp)(int(std::floor(fx)), 0, int(vis.width) - 1), y0 = (std::clamp)(int(std::floor(fy)), 0, int(vis.height) - 1);
                    const int x1 = (std::min)(x0 + 1, int(vis.width) - 1), y1 = (std::min)(y0 + 1, int(vis.height) - 1);
                    const float ax = (std::clamp)(fx - float(x0), 0.0f, 1.0f), ay = (std::clamp)(fy - float(y0), 0.0f, 1.0f);
                    const auto at = [&](int x, int y) { return V[size_t(y) * vis.width + x] / 65535.0f; };
                    g_vis[t] = (at(x0, y0) * (1 - ax) + at(x1, y0) * ax) * (1 - ay) + (at(x0, y1) * (1 - ax) + at(x1, y1) * ax) * ay;
                }
            }
            g_exposure = MIN_EXPOSURE;
            if (!peaks.empty())
            {
                const size_t k = peaks.size() * EXPOSURE_PERCENTILE / 100;
                std::nth_element(peaks.begin(), peaks.begin() + k, peaks.end());
                g_exposure = (std::max)(peaks[k], MIN_EXPOSURE);
            }
            g_note =haveVis ? "" : "no sky visibility atlas: sky and bounce split evenly";
            g_viewDirty = true;
            logger::info("[enlighten] relight: atlas {}x{} loaded (direction {}, sky visibility {})", W, H, haveDir, haveVis);
            return true;
        }

        void downsample(const std::vector<uint8_t>& src, uint32_t w, uint32_t h, uint32_t bpp, bool u16, std::vector<uint8_t>& dst)
        {
            const uint32_t dw = (std::max)(1u, w / 2), dh = (std::max)(1u, h / 2);
            dst.assign(size_t(dw) * dh * bpp, 0);
            for (uint32_t y = 0; y < dh; ++y)
                for (uint32_t x = 0; x < dw; ++x)
                {
                    const uint32_t x0 = (std::min)(2 * x, w - 1), x1 = (std::min)(2 * x + 1, w - 1);
                    const uint32_t y0 = (std::min)(2 * y, h - 1), y1 = (std::min)(2 * y + 1, h - 1);
                    const size_t s[4] = { size_t(y0) * w + x0, size_t(y0) * w + x1, size_t(y1) * w + x0, size_t(y1) * w + x1 };
                    const size_t d = size_t(y) * dw + x;
                    if (u16)
                    {
                        const uint16_t* S = reinterpret_cast<const uint16_t*>(src.data());
                        reinterpret_cast<uint16_t*>(dst.data())[d] = uint16_t((uint32_t(S[s[0]]) + S[s[1]] + S[s[2]] + S[s[3]] + 2) / 4);
                    }
                    else
                        for (uint32_t b = 0; b < bpp; ++b)
                            dst[d * bpp + b] = uint8_t((uint32_t(src[s[0] * bpp + b]) + src[s[1] * bpp + b] + src[s[2] * bpp + b] + src[s[3] * bpp + b] + 2) / 4);
                }
        }

        bool open(const detail::Original& o, bool luma, Target& out)
        {
            out = Target{};
            if (!o.tex || !o.tex->m_resource || !g_pDevice || !g_pContext) return false;
            ID3D11Texture2D* t = nullptr;
            if (FAILED(o.tex->m_resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&t))) || !t) return false;
            D3D11_TEXTURE2D_DESC d{};
            t->GetDesc(&d);
            if (d.ArraySize != 1 || d.SampleDesc.Count != 1 || d.Width != W || d.Height != H || !d.MipLevels) { t->Release(); return false; }
            const uint32_t f = detail::typedFormat(d.Format), bpp = detail::bytesPerPixel(f);
            // otherwise the copy is R16 luma + R8G8 chroma (blue share, red share)
            const bool native = bpp && bpp == o.bpp && (!luma || f == DXGI_FORMAT_R16_UNORM);
            out.level = o.tex->m_resource;
            out.levelViews[0] = o.tex->m_shaderViews[0];
            out.levelViews[1] = o.tex->m_shaderViews[1];
            out.width = d.Width;
            out.mips = d.MipLevels;
            out.bpp = native ? bpp : 2;
            if (native && !luma) detail::chromaBytes(o.dxgi, out.bx, out.by);
            out.original.resize(d.MipLevels);
            if (native)
            {
                D3D11_TEXTURE2D_DESC sd = d;
                sd.Usage = D3D11_USAGE_STAGING;
                sd.BindFlags = 0;
                sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                sd.MiscFlags = 0;
                ID3D11Texture2D* staging = nullptr;
                if (FAILED(g_pDevice->CreateTexture2D(&sd, nullptr, &staging)) || !staging) { t->Release(); out = Target{}; return false; }
                g_pContext->CopyResource(staging, t);
                for (uint32_t m = 0; m < d.MipLevels; ++m)
                {
                    const uint32_t mw = (std::max)(1u, d.Width >> m), mh = (std::max)(1u, d.Height >> m);
                    D3D11_MAPPED_SUBRESOURCE map{};
                    if (FAILED(g_pContext->Map(staging, m, D3D11_MAP_READ, 0, &map))) { staging->Release(); t->Release(); out = Target{}; return false; }
                    out.original[m].resize(size_t(mw) * mh * bpp);
                    for (uint32_t y = 0; y < mh; ++y)
                        std::memcpy(out.original[m].data() + size_t(y) * mw * bpp, static_cast<const uint8_t*>(map.pData) + size_t(y) * map.RowPitch, size_t(mw) * bpp);
                    g_pContext->Unmap(staging, m);
                }
                staging->Release();
            }
            else
                for (uint32_t m = 0; m < d.MipLevels; ++m)
                    out.original[m].assign(size_t((std::max)(1u, d.Width >> m)) * (std::max)(1u, d.Height >> m) * out.bpp, 0);
            if (native && d.Usage == D3D11_USAGE_DEFAULT)
            {
                out.res = t;
                return true;
            }
            t->Release();

            D3D11_TEXTURE2D_DESC cd = d;
            cd.Usage = D3D11_USAGE_DEFAULT;
            cd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            cd.CPUAccessFlags = 0;
            cd.MiscFlags = 0;
            if (!native) cd.Format = luma ? DXGI_FORMAT_R16_UNORM : DXGI_FORMAT_R8G8_UNORM;
            std::vector<D3D11_SUBRESOURCE_DATA> init(d.MipLevels);
            for (uint32_t m = 0; m < d.MipLevels; ++m)
                init[m] = { out.original[m].data(), (std::max)(1u, d.Width >> m) * out.bpp, 0 };
            if (FAILED(g_pDevice->CreateTexture2D(&cd, init.data(), &out.res)) || !out.res) { out = Target{}; return false; }
            out.copy = true;
            for (int i = 0; i < 2; ++i)
            {
                ID3D11ShaderResourceView* v = out.levelViews[i];
                if (!v || (i == 1 && v == out.levelViews[0])) continue;
                D3D11_SHADER_RESOURCE_VIEW_DESC vd{};
                v->GetDesc(&vd);
                if (!native) vd.Format = cd.Format;
                if (FAILED(g_pDevice->CreateShaderResourceView(out.res, &vd, &out.views[i])))
                {
                    for (auto*& w : out.views) if (w) w->Release();
                    out.res->Release();
                    out = Target{};
                    return false;
                }
            }
            return out.views[0] || out.views[1];
        }

        bool stale(const Target& p, const detail::Original& o)
        {
            return !o.tex || o.tex->m_resource != p.level || o.tex->m_shaderViews[0] != p.levelViews[0] || o.tex->m_shaderViews[1] != p.levelViews[1];
        }

        void writeMips(const Target& p, std::vector<uint8_t> level, bool u16)
        {
            uint32_t w = W, h = H;
            for (uint32_t m = 0; m < p.mips; ++m)
            {
                g_pContext->UpdateSubresource(p.res, m, nullptr, level.data(), w * p.bpp, 0);
                if (m + 1 == p.mips) break;
                std::vector<uint8_t> next;
                downsample(level, w, h, p.bpp, u16, next);
                level.swap(next);
                w = (std::max)(1u, w / 2);
                h = (std::max)(1u, h / 2);
            }
        }

        void release(Target& p)
        {
            if (!p.res) return;
            if (!p.copy)
                for (uint32_t m = 0; m < p.mips && m < p.original.size(); ++m)
                    g_pContext->UpdateSubresource(p.res, m, nullptr, p.original[m].data(), (std::max)(1u, p.width >> m) * p.bpp, 0);
            for (auto*& v : p.views) if (v) v->Release();
            p.res->Release();
            p = Target{};
        }

        // a copy may still be bound by a deferred context this frame
        void retire(Target& p, int frames)
        {
            if (!p.res) return;
            std::lock_guard<std::mutex> lock(g_restoreMutex);
            g_restores.push_back({ std::move(p), frames });
            p = Target{};
        }

        bool install()
        {
            if (!g_pDevice) return false;
            if (!open(detail::g_orig[0], true, g_luma) || !open(detail::g_orig[1], false, g_chroma))
            {
                release(g_luma);
                release(g_chroma);
                g_note = "lightmap textures cannot be opened for writing";
                return false;
            }
            textures::subst::Pair pairs[4];
            size_t n = 0;
            for (const Target* p : { &g_luma, &g_chroma })
                if (p->copy)
                    for (int i = 0; i < 2; ++i)
                        if (p->views[i]) pairs[n++] = { p->levelViews[i], p->views[i] };
            textures::subst::set(pairs, n);
            g_installed = true;
            logger::info("[enlighten] relight: luma {}, chroma {} ({} / {} mips)", g_luma.copy ? "copy swapped at bind" : "in place",
                g_chroma.copy ? "copy swapped at bind" : "in place", g_luma.mips, g_chroma.mips);
            return true;
        }

        void uninstall()
        {
            if (g_installed)
            {
                textures::subst::set(nullptr, 0);
                if (g_luma.copy) retire(g_luma, RETIRE_FRAMES); else release(g_luma);
                if (g_chroma.copy) retire(g_chroma, RETIRE_FRAMES); else release(g_chroma);
                db::restoreLevelProbes();
            }
            g_installed = false;
            g_hasApplied = false;
            g_viewDirty = true;
        }

        float ratio(float now, float ref) { return ref > RATIO_EPS ? now / ref : 1.0f; }

        // out = in * (vis * sky ratio + (1 - vis) * sun ratio)
        // sky radiance: hemispheres blended by texel up weight
        void apply(const Light& now, const Light& ref)
        {
            constexpr int UP_STEPS = 64; // sky ratio table over the texel up weight
            Vec3 skyRatio[UP_STEPS + 1];
            for (int i = 0; i <= UP_STEPS; ++i)
            {
                const float u = float(i) / UP_STEPS;
                const Vec3 a = (now.sky + now.sunDisc * now.sunDiscSize) * u + now.ground * (1.0f - u);
                const Vec3 b = (ref.sky + ref.sunDisc * ref.sunDiscSize) * u + ref.ground * (1.0f - u);
                skyRatio[i] = vec3(ratio(a.m_x, b.m_x), ratio(a.m_y, b.m_y), ratio(a.m_z, b.m_z));
            }
            const Vec3 sunNow = now.sun * now.sunScale, sunRef = ref.sun * ref.sunScale;
            const float bounce = ref.bounce > BOUNCE_EPS ? (1.0f - BOUNCE_SHARE) + BOUNCE_SHARE * now.bounce / ref.bounce : 1.0f;
            const Vec3 restRatio = vec3(ratio(sunNow.m_x, sunRef.m_x), ratio(sunNow.m_y, sunRef.m_y), ratio(sunNow.m_z, sunRef.m_z)) * bounce;

            const size_t n = size_t(W) * H;
            std::vector<uint8_t> luma(n * 2, 0);
            uint16_t* L = reinterpret_cast<uint16_t*>(luma.data());
            g_out.assign(n, Vec3{});
            const uint32_t cbpp = g_chroma.bpp;
            const int bx = g_chroma.bx, by = g_chroma.by;
            std::vector<uint8_t> chroma = g_chroma.original[0];
            double sumIn[3] = {}, sumOut[3] = {};
            for (size_t t = 0; t < n; ++t)
            {
                if (!g_valid[t]) continue;
                const float v = g_vis[t];
                const Vec3& s = skyRatio[int(g_up[t] * UP_STEPS + 0.5f)];
                const Vec3 k = s * (v * look::settings.skyStrength) + restRatio * ((1.0f - v) * look::settings.sunStrength);
                const Vec3& in = g_rgb[t];
                Vec3 out = in * k * scale;
                look::apply(&out.m_x, &in.m_x);
                g_out[t] = out;
                const float l = out.m_x + out.m_y + out.m_z;
                L[t] = uint16_t((std::clamp)(l, 0.0f, 1.0f) * 65535.0f + 0.5f);
                if (l > 0.0f)
                {
                    chroma[t * cbpp + bx] = uint8_t((std::clamp)(out.m_z / l, 0.0f, 1.0f) * 255.0f + 0.5f);
                    chroma[t * cbpp + by] = uint8_t((std::clamp)(out.m_x / l, 0.0f, 1.0f) * 255.0f + 0.5f);
                }
                sumIn[0] += in.m_x; sumIn[1] += in.m_y; sumIn[2] += in.m_z;
                sumOut[0] += out.m_x; sumOut[1] += out.m_y; sumOut[2] += out.m_z;
            }
            writeMips(g_luma, std::move(luma), true);
            writeMips(g_chroma, std::move(chroma), false);
            for (int c = 0; c < 3; ++c) fb::at(g_levelRatio, c) = sumIn[c] > 0.0 ? float(sumOut[c] / sumIn[c]) : 1.0f;
            db::scaleLevelProbes(g_levelRatio.m_x, g_levelRatio.m_y, g_levelRatio.m_z);
            g_viewDirty = true;
        }

        void suspendForReference()
        {
            g_refOverrides = editor::overridesEnabled;
            editor::overridesEnabled = false;
            states::restoreAll();
            std::lock_guard<std::mutex> lock(g_mutex);
            g_refUpdates = 0;
            g_refPhase = 2;
        }

        void releaseView()
        {
            if (g_viewSrv) g_viewSrv->Release();
            if (g_viewTex) g_viewTex->Release();
            g_viewSrv = nullptr;
            g_viewTex = nullptr;
        }

        const Vec3& shown(size_t t) { return g_viewRelit && g_installed && t < g_out.size() ? g_out[t] : g_rgb[t]; }

        void updateView()
        {
            if (!g_viewOpen || !g_viewDirty || !g_loaded || !g_pDevice || !g_pContext) return;
            g_viewDirty = false;
            if (!g_viewTex)
            {
                D3D11_TEXTURE2D_DESC d{};
                d.Width = W; d.Height = H; d.MipLevels = 1; d.ArraySize = 1; d.SampleDesc.Count = 1;
                d.Format = DXGI_FORMAT_R8G8B8A8_UNORM; d.Usage = D3D11_USAGE_DEFAULT; d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                if (FAILED(g_pDevice->CreateTexture2D(&d, nullptr, &g_viewTex)) || FAILED(g_pDevice->CreateShaderResourceView(g_viewTex, nullptr, &g_viewSrv)))
                {
                    releaseView();
                    return;
                }
            }
            std::vector<uint32_t> px(size_t(W) * H, 0xFF000000u);
            const auto ch = [](float v) { return uint32_t(255.0f * std::pow((std::clamp)(v, 0.0f, 1.0f), 1.0f / 2.2f)); };
            for (size_t t = 0; t < px.size(); ++t)
            {
                if (!g_valid[t]) continue;
                const Vec3& c = shown(t);
                px[t] = 0xFF000000u | ch(c.m_z / g_exposure) << 16 | ch(c.m_y / g_exposure) << 8 | ch(c.m_x / g_exposure);
            }
            g_pContext->UpdateSubresource(g_viewTex, 0, nullptr, px.data(), W * 4, 0);
        }

        void renderView()
        {
            if (!ImGui::TreeNode("atlas view##relight")) { g_viewOpen = false; return; }
            g_viewOpen = true;
            if (!g_loaded) { ImGui::TextDisabled("loads with relight on"); ImGui::TreePop(); return; }
            if (ImGui::Checkbox("relit", &g_viewRelit)) g_viewDirty = true;
            if (g_viewSrv)
            {
                const float disp = (std::min)(768.0f, ImGui::GetContentRegionAvail().x);
                const ImVec2 size(disp, disp * float(H) / float(W));
                ImGui::InvisibleButton("##atlasview", size);
                const ImVec2 mn = ImGui::GetItemRectMin(), mx = ImGui::GetItemRectMax();
                ImDrawList* dl = ImGui::GetWindowDrawList();
                dl->AddImage(reinterpret_cast<ImTextureID>(g_viewSrv), mn, mx);
                Vec3 cam, fwd;
                size_t texel = 0;
                float t = 0.0f;
                if (scene::phase() == scene::Phase::Ready && render::cameraPosition(cam) && render::cameraForward(fwd) &&
                    scene::hitTexel(cam + fwd * 0.3f, fwd, 2000.0f, texel, t) && texel != ~size_t(0) && scene::width() == W) // as the inspector pick
                {
                    const float px = mn.x + (float(texel % W) + 0.5f) * size.x / W, py = mn.y + (float(texel / W) + 0.5f) * size.y / H;
                    dl->AddCircle(ImVec2(px, py), 6.0f, IM_COL32(255, 230, 0, 255), 12, 2.0f);
                }
                if (ImGui::IsItemHovered())
                {
                    const ImVec2 m = ImGui::GetMousePos();
                    const uint32_t x = (std::min)(uint32_t((m.x - mn.x) / size.x * W), W - 1), y = (std::min)(uint32_t((m.y - mn.y) / size.y * H), H - 1);
                    const size_t i = size_t(y) * W + x;
                    const Vec3& o = g_rgb[i];
                    const Vec3& c = shown(i);
                    ImGui::SetTooltip("texel %u, %u\nshipped %.4f %.4f %.4f\nshown %.4f %.4f %.4f", x, y, o.m_x, o.m_y, o.m_z, c.m_x, c.m_y, c.m_z);
                }
                if (scene::phase() == scene::Phase::Ready)
                    ImGui::TextDisabled("circle = texel under the crosshair");
            }
            ImGui::TreePop();
        }
    }

    void onUpdated(fb::VisualEnvironment* ve)
    {
        if (!ve) return;
        const Light l = read(ve);
        std::lock_guard<std::mutex> lock(g_mutex);
        g_now = l;
        g_haveNow = true;
        if (g_refPhase == 2 && ++g_refUpdates >= REF_UPDATES)
        {
            g_ref = l;
            g_haveRef = true;
            g_refPhase = 3;
        }
    }

    void flushRestores()
    {
        std::vector<Retired> due;
        {
            std::lock_guard<std::mutex> lock(g_restoreMutex);
            for (size_t i = 0; i < g_restores.size();)
                if (g_restores[i].frames-- <= 0)
                {
                    due.push_back(std::move(g_restores[i]));
                    g_restores.erase(g_restores.begin() + i);
                }
                else
                    ++i;
        }
        for (Retired& r : due)
            release(r.t);
    }

    void tick()
    {
        if (!detail::g_scanned) return;
        int phase;
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            phase = g_refPhase;
        }
        if (phase == 1) { suspendForReference(); return; }
        if (phase == 3)
        {
            editor::overridesEnabled = g_refOverrides;
            std::lock_guard<std::mutex> lock(g_mutex);
            g_refPhase = 0;
            logger::info("[enlighten] relight: reference taken from the level's own VE");
        }
        const bool active = enabled && !db::liveDatabase();
        if (!active)
        {
            if (g_installed) uninstall();
            if (g_viewOpen && !g_loaded && !g_loadFailed && detail::g_staticAsset)
            {
                g_loaded = load();
                g_loadFailed = !g_loaded;
            }
            updateView();
            return;
        }
        if (!g_loaded)
        {
            g_loaded = load();
            if (!g_loaded) { enabled = false; return; }
        }
        Light now, ref;
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            if (!g_haveRef) { if (!g_refPhase) g_refPhase = 1; return; }
            if (!g_haveNow) return;
            now = g_now;
            ref = g_ref;
        }
        if (g_installed && (stale(g_luma, detail::g_orig[0]) || stale(g_chroma, detail::g_orig[1])))
        {
            logger::info("[enlighten] relight: lightmap texture re-created, reinstalling");
            uninstall();
        }
        if (!g_installed && !install()) { uninstall(); enabled = false; return; }
        if (!g_hasApplied || !(now == g_applied) || scale != g_appliedScale || look::generation != g_appliedLook)
        {
            apply(now, ref);
            g_applied = now;
            g_appliedScale = scale;
            g_appliedLook = look::generation;
            g_hasApplied = true;
        }
        updateView();
    }

    void renderUI()
    {
        const bool solver = db::liveDatabase() != nullptr;
        ImGui::BeginDisabled(solver);
        ImGui::Checkbox("relight baked lightmap", &enabled);
        ImGui::EndDisabled();
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            ImGui::SetTooltip("sky colors scale the sky-lit part of each texel, sun color x sun scale the rest");
        if (solver) { ImGui::SameLine(); ImGui::TextDisabled("(engine solver on)"); }
        if (!g_note.empty()) ImGui::TextDisabled("%s", g_note.c_str());
        if (enabled)
        {
            ImGui::SameLine();
            ImGui::BeginDisabled(g_refPhase != 0);
            if (ImGui::SmallButton("retake reference"))
            {
                std::lock_guard<std::mutex> lock(g_mutex);
                g_haveRef = false;
                g_refPhase = 1;
                g_hasApplied = false;
            }
            ImGui::EndDisabled();
            ImGui::SliderFloat("relight brightness", &scale, 0.1f, 8.0f, "x%.2f", ImGuiSliderFlags_Logarithmic);
            if (g_installed)
                ImGui::TextDisabled("level light x%.3f %.3f %.3f of the baked atlas", g_levelRatio.m_x, g_levelRatio.m_y, g_levelRatio.m_z);
        }
        renderView();
    }

    bool relit(uint32_t texel, float* rgb)
    {
        if (!g_installed || texel >= g_out.size()) return false;
        rgb[0] = g_out[texel].m_x; rgb[1] = g_out[texel].m_y; rgb[2] = g_out[texel].m_z;
        return true;
    }

    void clear()
    {
        // update thread
        if (g_installed)
        {
            // before teardown frees the level's views (a reused address must not be swapped)
            textures::subst::set(nullptr, 0);
            retire(g_luma, 0);
            retire(g_chroma, 0);
            g_installed = false;
            db::restoreLevelProbes();
        }
        uninstall();
        releaseView();
        if (g_refPhase == 2 || g_refPhase == 3) editor::overridesEnabled = g_refOverrides;
        std::lock_guard<std::mutex> lock(g_mutex);
        g_refPhase = 0;
        g_haveRef = g_haveNow = false;
        g_loaded = g_loadFailed = false;
        g_rgb.clear(); g_vis.clear(); g_up.clear(); g_valid.clear(); g_out.clear();
        g_note.clear();
    }
}
