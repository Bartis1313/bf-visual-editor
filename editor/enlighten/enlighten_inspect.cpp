#include "enlighten_inspect.h"
#include "enlighten_internal.h"
#include "enlighten_db.h"
#include "enlighten_gen.h"
#include "enlighten_scene.h"
#include "../render/render.h"
#include "../lights/lights.h"
#include "enlighten_relight.h"
#include "enlighten_spawn.h"
#include "../../hooks/functions.h"
#include "../../utils/log.h"

#include <d3d11.h>
#include <imgui.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace editor::enlighten::inspect
{
    using fb::Vec3;
    using fb::vec3;
    using Clock = std::chrono::steady_clock;

    namespace
    {
        bool g_enabled = false, g_follow = true, g_drawRays = true, g_drawGrid = true, g_throughWalls = true, g_overview = false;
        int g_radius = 10;
        uint32_t g_texel = ~0u;
        bool g_pickRequest = false, g_dirty = false;
        std::string g_pickNote;
        Clock::time_point g_pickAt{}, g_liveAt{};
        detail::Original g_live[3];
        bool g_liveOk = false;

        struct Seg { Vec3 a, b; ImColor c; };
        std::mutex g_segMutex;
        std::vector<Seg> g_segs;
        Vec3 g_labelAt{};
        std::string g_label;
        std::string g_raySummary;

        struct Tex { ID3D11Texture2D* tex = nullptr; ID3D11ShaderResourceView* srv = nullptr; uint32_t w = 0, h = 0; };
        Tex g_tiles, g_map;
        uint32_t g_cropN = 0;
        int g_cropX0 = 0, g_cropY0 = 0;
        constexpr uint32_t MAP_STEP = 4; // one pixel per 4x4 texels
        constexpr uint32_t LOD_TRI = 0x80000000u; // scene triangle id: texel from a LOD 1+ chart
        constexpr uint8_t KIND_TERRAIN = 1; // scene::kinds()
        constexpr uint8_t COV_GUTTER = 2; // gen coverage
        constexpr uint8_t COV_FILL = 3;
        constexpr float LUMA_EPS = 1e-6f; // live luma in log2(live / shipped)
        constexpr float RATIO_STOPS = 2.0f; // log2 range to full color
        constexpr float MIN_SCALE = 1e-6f; // tone scale floor
        constexpr float POOL_DOT = 0.95f; // gen smoothing: coplanar neighbours
        constexpr float POOL_PLANE = 0.3f; // meters off the plane
        constexpr float POOL_DIST = 8.0f; // meters
        constexpr int GRID_RINGS = 4; // 9x9 texel grid
        constexpr float GRID_LINK_SQ = 16.0f; // neighbour lines only under 4 m
        constexpr float PICK_START = 0.3f; // meters ahead of the camera
        constexpr float PICK_RANGE = 2000.0f;
        constexpr float AREA_FALLBACK = 10.0f; // meters, area centre when nothing is hit
        constexpr float CROSS_HALF = 0.15f; // meters, area cross without a right neighbour
        constexpr float CROSS_MIN = 0.03f;
        constexpr float CROSS_MAX = 1.0f;
        constexpr float CROSS_LIFT = 0.03f; // meters off the surface
        constexpr int LIVE_REFRESH_MS = 1000; // live atlas readback
        constexpr int REBUILD_MS = 250; // crosshair pick, area overlay

        enum Cls { kEmpty, kLod0, kLod1, kTerrain, kGutter, kFillCls, kClsCount };
        const ImU32 kClsCol[kClsCount] = { IM_COL32(0, 0, 0, 255), IM_COL32(128, 128, 128, 255), IM_COL32(0, 200, 0, 255),
            IM_COL32(150, 110, 50, 255), IM_COL32(60, 60, 170, 255), IM_COL32(210, 0, 210, 255) };
        const char* const kClsName[kClsCount] = { "empty", "mesh LOD0", "mesh LOD1+", "terrain", "gutter", "sky-only fill" };
        const char* const kRayName[4] = { "sky", "cluster", "unsolved surface", "back face" };
        const ImColor kRayCol[4] = { ImColor(90, 170, 255, 200), ImColor(60, 230, 60, 200), ImColor(255, 150, 0, 220), ImColor(255, 40, 40, 220) };

        void release(Tex& t)
        {
            if (t.srv) t.srv->Release();
            if (t.tex) t.tex->Release();
            t = Tex{};
        }

        bool upload(Tex& t, uint32_t w, uint32_t h, const std::vector<ImU32>& px)
        {
            if (!g_pDevice || !g_pContext) return false;
            if (t.w != w || t.h != h)
            {
                release(t);
                D3D11_TEXTURE2D_DESC d{};
                d.Width = w; d.Height = h; d.MipLevels = 1; d.ArraySize = 1;
                d.Format = DXGI_FORMAT_R8G8B8A8_UNORM; d.SampleDesc.Count = 1;
                d.Usage = D3D11_USAGE_DEFAULT; d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                if (FAILED(g_pDevice->CreateTexture2D(&d, nullptr, &t.tex))) { t = Tex{}; return false; }
                if (FAILED(g_pDevice->CreateShaderResourceView(t.tex, nullptr, &t.srv))) { release(t); return false; }
                t.w = w; t.h = h;
            }
            g_pContext->UpdateSubresource(t.tex, 0, nullptr, px.data(), w * 4, 0);
            return true;
        }

        struct Atlas
        {
            const uint8_t* p = nullptr;
            uint32_t w = 0, h = 0, bpp = 0, dxgi = 0;
        };
        Atlas atlasOf(const detail::Original& o) { return o.pixels.empty() ? Atlas{} : Atlas{ o.pixels.data(), o.width, o.height, o.bpp, o.dxgi }; }
        Atlas shipped(int i) { return atlasOf(detail::g_orig[i]); }
        Atlas live(int i) { return g_liveOk ? atlasOf(g_live[i]) : Atlas{}; }

        float lumaAt(const Atlas& a, uint32_t t) { return a.p && a.bpp == 2 ? reinterpret_cast<const uint16_t*>(a.p)[t] / 65535.0f : 0.0f; }

        // luma = r + g + b, chroma = blue and red shares
        bool rgbAt(const Atlas& luma, const Atlas& chroma, uint32_t t, float rgb[3])
        {
            rgb[0] = rgb[1] = rgb[2] = 0.0f;
            if (!luma.p || !chroma.p || chroma.w != luma.w) return false;
            const float L = lumaAt(luma, t);
            int bx, by;
            detail::chromaBytes(chroma.dxgi, bx, by);
            const uint8_t* c = chroma.p + size_t(t) * chroma.bpp;
            rgb[2] = L * c[bx] / 255.0f;
            rgb[0] = L * c[by] / 255.0f;
            rgb[1] = (std::max)(0.0f, L - rgb[0] - rgb[2]);
            return true;
        }

        bool dirAt(const Atlas& a, uint32_t t, Vec3& d, float& alpha)
        {
            if (!a.p || a.bpp != 4) return false;
            const uint8_t* c = a.p + size_t(t) * 4;
            d = vec3(c[0] / 127.5f - 1.0f, c[1] / 127.5f - 1.0f, c[2] / 127.5f - 1.0f);
            alpha = c[3] / 255.0f;
            return true;
        }

        int classOf(uint32_t t)
        {
            Vec3 p, n;
            uint8_t cov = 0;
            if (!gen::texelSurface(t, p, n, cov) || !cov) return kEmpty;
            if (cov == COV_GUTTER) return kGutter;
            if (cov == COV_FILL) return kFillCls;
            if (scene::kinds()[t] == KIND_TERRAIN) return kTerrain;
            const uint32_t tri = scene::triangles()[t];
            return tri != ~0u && (tri & LOD_TRI) ? kLod1 : kLod0;
        }

        // log2(live / shipped): blue .. white .. red, +-2 stops
        ImU32 ratioColor(float ship, float lv)
        {
            if (ship <= 0.0f) return IM_COL32(24, 24, 24, 255);
            const float s = (std::clamp)(std::log2((lv + LUMA_EPS) / ship) / RATIO_STOPS, -1.0f, 1.0f);
            const int k = int(255.0f * (1.0f - std::fabs(s)));
            return s > 0.0f ? IM_COL32(255, k, k, 255) : IM_COL32(k, k, 255, 255);
        }

        ImU32 toneColor(const float rgb[3], float scale)
        {
            const auto ch = [&](float v) { return int(255.0f * std::pow((std::clamp)(v / scale, 0.0f, 1.0f), 1.0f / 2.2f)); };
            return IM_COL32(ch(rgb[0]), ch(rgb[1]), ch(rgb[2]), 255);
        }

        bool ready(std::string& why)
        {
            if (gen::running()) { why = "generating..."; return false; }
            gen::TexelInfo ti;
            if (scene::phase() != scene::Phase::Ready || !gen::texelInfo(0, ti))
            {
                why = "generate systems this session first: the inspector reads the generator's scene";
                return false;
            }
            return true;
        }

        bool coplanar(const Vec3& pa, const Vec3& na, const Vec3& pc, const Vec3& nc)
        {
            return fb::dot(na, nc) >= POOL_DOT && std::fabs(fb::dot(na, pc - pa)) <= POOL_PLANE && fb::distanceSq(pa, pc) <= POOL_DIST * POOL_DIST;
        }

        void addCross(std::vector<Seg>& out, const Vec3& p, float r, const ImColor& c)
        {
            out.push_back({ p - vec3(r, 0, 0), p + vec3(r, 0, 0), c });
            out.push_back({ p - vec3(0, r, 0), p + vec3(0, r, 0), c });
            out.push_back({ p - vec3(0, 0, r), p + vec3(0, 0, r), c });
        }

        const char* meshName(const fb::MeshAsset* m) { return m && m->m_Name ? m->m_Name : "?"; }

        void summarizeRays(const std::vector<gen::RayViz>& rays)
        {
            std::string out;
            for (int k = 0; k < 4; ++k)
            {
                uint32_t n = 0, terrain = 0;
                std::unordered_map<const fb::MeshAsset*, uint32_t> meshes;
                for (const gen::RayViz& r : rays)
                {
                    if (r.kind != k) continue;
                    ++n;
                    if (r.terrain) ++terrain;
                    else if (r.mesh) ++meshes[r.mesh];
                }
                if (!n) continue;
                const fb::MeshAsset* top = nullptr;
                uint32_t topN = 0;
                for (const auto& [m, c] : meshes) if (c > topN) { topN = c; top = m; }
                char b[384];
                std::snprintf(b, sizeof(b), "%s%s %u: terrain %u%s%s (%u)", out.empty() ? "" : "\n", kRayName[k], n, terrain,
                    top ? ", mostly " : "", top ? meshName(top) : "", topN);
                out += b;
            }
            g_raySummary = out;
        }

        void rebuildWorld()
        {
            std::vector<Seg> segs;
            Vec3 p{}, n{};
            uint8_t cov = 0;
            const uint32_t W = scene::width(), H = scene::height();
            if (g_texel != ~0u && gen::texelSurface(g_texel, p, n, cov) && cov)
            {
                const ImColor yellow(255, 230, 0, 255);
                segs.push_back({ p, p + n * 1.0f, yellow });
                addCross(segs, p, 0.2f, yellow);
                if (g_drawGrid)
                {
                    const int G = GRID_RINGS, sr = gen::params().smoothRadius;
                    const int tx = int(g_texel % W), ty = int(g_texel / W);
                    for (int dy = -G; dy <= G; ++dy)
                        for (int dx = -G; dx <= G; ++dx)
                        {
                            const int x = tx + dx, y = ty + dy;
                            if (x < 0 || y < 0 || x >= int(W) || y >= int(H)) continue;
                            const uint32_t q = uint32_t(y) * W + uint32_t(x);
                            Vec3 qp, qn;
                            uint8_t qc = 0;
                            if (!gen::texelSurface(q, qp, qn, qc) || !qc) continue;
                            const ImColor col(kClsCol[classOf(q)]);
                            const bool pooled = (std::max)(std::abs(dx), std::abs(dy)) <= sr && coplanar(p, n, qp, qn);
                            if (q != g_texel) addCross(segs, qp, pooled ? 0.12f : 0.06f, pooled ? ImColor(0, 255, 255, 255) : col);
                            const int nx[2][2] = { { 1, 0 }, { 0, 1 } };
                            for (const auto& o : nx)
                            {
                                if (x + o[0] >= int(W) || y + o[1] >= int(H) || std::abs(dx + o[0]) > G || std::abs(dy + o[1]) > G) continue;
                                Vec3 rp, rn;
                                uint8_t rc = 0;
                                if (gen::texelSurface(q + o[0] + o[1] * W, rp, rn, rc) && rc && fb::distanceSq(qp, rp) < GRID_LINK_SQ)
                                    segs.push_back({ qp, rp, col });
                            }
                        }
                }
                std::vector<gen::RayViz> rays;
                gen::traceTexel(g_texel, rays);
                if (g_drawRays)
                    for (const gen::RayViz& r : rays) segs.push_back({ r.from, r.to, kRayCol[r.kind & 3] });
                summarizeRays(rays);
            }
            std::lock_guard<std::mutex> lock(g_segMutex);
            g_segs = std::move(segs);
            g_labelAt = p + n * 1.15f;
            g_label = g_texel != ~0u ? "texel " + std::to_string(g_texel % (W ? W : 1)) + ", " + std::to_string(g_texel / (W ? W : 1)) : "";
        }

        void select(uint32_t t)
        {
            if (t == g_texel) return;
            g_texel = t;
            g_dirty = true;
            rebuildWorld();
        }

        const char* typeName(const fb::MeshAsset* m)
        {
            if (!m) return "?";
            switch (int(m->m_EnlightenType)) // EnlightenType
            {
            case 0: return "Dynamic (probe lit)";
            case 1: return "LightProbe";
            case 2: return "Static";
            case 3: return "Proxy";
            default: return "?";
            }
        }
        void pick()
        {
            Vec3 cam, fwd;
            if (!render::cameraPosition(cam) || !render::cameraForward(fwd)) { g_pickNote = "no camera"; return; }
            char b[768];
            int len = 0;
            scene::HitInfo h;
            const bool hit = scene::hitInfo(cam + fwd * PICK_START, fwd, PICK_RANGE, h);
            if (!hit) len = std::snprintf(b, sizeof(b), "scene: no hit");
            else if (h.terrain) len = std::snprintf(b, sizeof(b), "scene: terrain at %.1f m%s", h.t, h.charted ? "" : " (outside the terrain tiles)");
            else len = std::snprintf(b, sizeof(b), "scene: %s [%s, %s] at %.1f m", meshName(h.mesh), typeName(h.mesh),
                h.charted ? "charted" : "occluder only", h.t);
            lights::AimRay a;
            if (lights::lastAimRay(a) && a.valid)
            {
                const fb::MeshAsset* m = a.exactValid ? static_cast<const fb::MeshAsset*>(a.exact.mesh) : nullptr;
                len += std::snprintf(b + len, sizeof(b) - len, "\nengine: %s [%s] at %.1f m", m ? meshName(m) : (a.engine ? "physics body (no mesh resolved)" : "placement box"),
                    m ? typeName(m) : "-", a.t);
            }
            g_pickNote = b;
            if (hit && h.texel != ~size_t(0)) select(uint32_t(h.texel));
        }

        void refreshLive()
        {
            std::string err;
            for (detail::Original& o : detail::g_orig) detail::readback(o, err);
            g_liveOk = false;
            fb::EnlightenRuntimeDatabase* db = db::liveDatabase();
            if (!db) return;
#if defined(BFVE_GAME_BF3)
            fb::DxTexture* const tex[3] = { db->m_dynamicLuma, db->m_dynamicChroma, db->m_dynamicDirection };
#else
            fb::DxTexture* const tex[3] = { db->m_lumaTexture, db->m_chromaTexture, db->m_directionTexture };
#endif
            for (int i = 0; i < 3; ++i)
            {
                g_live[i] = detail::Original{};
                g_live[i].tex = tex[i];
                if (!tex[i] || !detail::readback(g_live[i], err)) return;
            }
            g_liveOk = true;
        }

        void buildPreviews()
        {
            const uint32_t W = scene::width(), H = scene::height();
            if (g_texel == ~0u || !W) return;
            const uint32_t N = uint32_t(2 * g_radius + 1), cell = (std::max)(2u, 240u / N), S = N * cell;
            g_cropN = N;
            g_cropX0 = int(g_texel % W) - g_radius;
            g_cropY0 = int(g_texel / W) - g_radius;
            const Atlas sl = shipped(0), sc = shipped(1), sd = shipped(2), ll = live(0), lc = live(1), ld = live(2);
            float scale = MIN_SCALE;
            for (uint32_t j = 0; j < N; ++j)
                for (uint32_t i = 0; i < N; ++i)
                {
                    const int x = g_cropX0 + int(i), y = g_cropY0 + int(j);
                    if (x < 0 || y < 0 || x >= int(W) || y >= int(H)) continue;
                    scale = (std::max)(scale, lumaAt(sl, uint32_t(y) * W + x));
                }
            std::vector<ImU32> px(size_t(S) * 3 * S * 2, IM_COL32(0, 0, 0, 255));
            for (uint32_t j = 0; j < N; ++j)
                for (uint32_t i = 0; i < N; ++i)
                {
                    const int x = g_cropX0 + int(i), y = g_cropY0 + int(j);
                    if (x < 0 || y < 0 || x >= int(W) || y >= int(H)) continue;
                    const uint32_t t = uint32_t(y) * W + x;
                    ImU32 c[6] = { IM_COL32(0, 0, 0, 255), IM_COL32(0, 0, 0, 255), IM_COL32(0, 0, 0, 255), kClsCol[classOf(t)], IM_COL32(0, 0, 0, 255), IM_COL32(0, 0, 0, 255) };
                    float rgb[3];
                    if (rgbAt(sl, sc, t, rgb)) c[0] = toneColor(rgb, scale);
                    if (rgbAt(ll, lc, t, rgb)) c[1] = toneColor(rgb, scale);
                    if (sl.p && ll.p) c[2] = ratioColor(lumaAt(sl, t), lumaAt(ll, t));
                    if (sd.p) { const uint8_t* d = sd.p + size_t(t) * 4; c[4] = IM_COL32(d[0], d[1], d[2], 255); }
                    if (ld.p) { const uint8_t* d = ld.p + size_t(t) * 4; c[5] = IM_COL32(d[0], d[1], d[2], 255); }
                    for (int k = 0; k < 6; ++k)
                    {
                        const uint32_t ox = (k % 3) * S + i * cell, oy = (k / 3) * S + j * cell;
                        for (uint32_t yy = 0; yy < cell; ++yy)
                            for (uint32_t xx = 0; xx < cell; ++xx)
                            {
                                const bool edge = cell >= 6 && (xx == 0 || yy == 0);
                                px[size_t(oy + yy) * 3 * S + ox + xx] = edge ? (((c[k] >> 2) & 0x003F3F3Fu) * 3) | 0xFF000000u : c[k];
                            }
                    }
                }
            upload(g_tiles, 3 * S, 2 * S, px);

            if (!g_overview || !sl.p || !ll.p) return;
            const uint32_t MW = W / MAP_STEP, MH = H / MAP_STEP;
            std::vector<ImU32> map(size_t(MW) * MH, IM_COL32(0, 0, 0, 255));
            for (uint32_t my = 0; my < MH; ++my)
                for (uint32_t mx = 0; mx < MW; ++mx)
                {
                    double sum = 0.0;
                    int n = 0;
                    for (uint32_t j = 0; j < MAP_STEP; ++j)
                        for (uint32_t i = 0; i < MAP_STEP; ++i)
                        {
                            const uint32_t t = (my * MAP_STEP + j) * W + mx * MAP_STEP + i;
                            const float s = lumaAt(sl, t);
                            if (s <= 0.0f) continue;
                            sum += std::log2((lumaAt(ll, t) + LUMA_EPS) / s);
                            ++n;
                        }
                    if (n) map[size_t(my) * MW + mx] = ratioColor(1.0f, float(std::exp2(sum / n)));
                }
            upload(g_map, MW, MH, map);
        }

        uint32_t worstInBlock(uint32_t mx, uint32_t my)
        {
            const uint32_t W = scene::width();
            const Atlas sl = shipped(0), ll = live(0);
            uint32_t best = ~0u;
            float bestDev = -1.0f;
            for (uint32_t j = 0; j < MAP_STEP; ++j)
                for (uint32_t i = 0; i < MAP_STEP; ++i)
                {
                    const uint32_t t = (my * MAP_STEP + j) * W + mx * MAP_STEP + i;
                    const float s = lumaAt(sl, t);
                    if (s <= 0.0f || classOf(t) == kEmpty) continue;
                    const float dev = std::fabs(std::log2((lumaAt(ll, t) + LUMA_EPS) / s));
                    if (dev > bestDev) { bestDev = dev; best = t; }
                }
            return best;
        }

        std::string describe(uint32_t t)
        {
            const uint32_t W = scene::width();
            char b[1024];
            gen::TexelInfo ti;
            gen::texelInfo(t, ti);
            Vec3 p{}, n{};
            uint8_t cov = 0;
            gen::texelSurface(t, p, n, cov);
            const float s = lumaAt(shipped(0), t), l = lumaAt(live(0), t);
            float srgb[3], lrgb[3];
            rgbAt(shipped(0), shipped(1), t, srgb);
            rgbAt(live(0), live(1), t, lrgb);
            Vec3 sd{}, ld{};
            float sa = 0.0f, la = 0.0f;
            const bool hs = dirAt(shipped(2), t, sd, sa), hl = dirAt(live(2), t, ld, la);
            const float dd = hs && hl ? fb::dot(fb::normalized(sd), fb::normalized(ld)) : 0.0f;
            db::TexelGain g;
            const bool hg = db::gainAt(t % W, t / W, g);
            std::snprintf(b, sizeof(b),
                "texel %u,%u %s | system %d cluster %d%s%s%s\n"
                "pos %.2f %.2f %.2f  normal %.2f %.2f %.2f\n"
                "rays %u/%u valid (%.0f%% back face), pooled %u texels, %u refs: sky %.0f%%  clusters %.0f%%  unsolved %.0f%%\n"
                "luma shipped %.5f  live %.5f  ratio %.2f\n"
                "rgb shipped %.4f %.4f %.4f  live %.4f %.4f %.4f\n"
                "direction shipped %.2f %.2f %.2f a%.2f  live %.2f %.2f %.2f a%.2f  dot %.2f\n"
                "calibration %s gain %.3f  tint %.2f %.2f %.2f  floor %.5f %.5f %.5f x%.2f  raw %.5f %.5f %.5f",
                t % W, t / W, kClsName[classOf(t)], ti.system, ti.cluster,
                (ti.flags & gen::kBounce) ? " | bounce pixel" : "", (ti.flags & gen::kBuried) ? " | buried" : "", (ti.flags & gen::kRefilled) ? " | refilled" : "",
                p.m_x, p.m_y, p.m_z, n.m_x, n.m_y, n.m_z,
                ti.rawValid, ti.rays, ti.rays ? 100.0f * (1.0f - float(ti.rawValid) / ti.rays) : 0.0f, ti.pooled, ti.refs, ti.env * 100.0f, ti.lit * 100.0f, ti.black * 100.0f,
                s, l, s > 0.0f ? l / s : 0.0f,
                srgb[0], srgb[1], srgb[2], lrgb[0], lrgb[1], lrgb[2],
                sd.m_x, sd.m_y, sd.m_z, sa, ld.m_x, ld.m_y, ld.m_z, la, dd,
                hg ? "" : "(none)", g.gain, g.tint[0], g.tint[1], g.tint[2], g.add[0], g.add[1], g.add[2], g.floorScale, g.raw[0], g.raw[1], g.raw[2]);
            return b;
        }
    }

    namespace
    {
        constexpr float AREA_RADIUS = 15.0f;
        constexpr size_t AREA_MAX = 3000;
        bool g_area = false, g_areaPrepared = false;
        Clock::time_point g_areaAt{};
        std::vector<Seg> g_areaSegs; // under g_segMutex
        std::string g_areaError;

        bool displayedAt(uint32_t t, float* rgb)
        {
            if (g_liveOk) return rgbAt(live(0), live(1), t, rgb);
            if (relight::relit(t, rgb)) return true;
            return rgbAt(shipped(0), shipped(1), t, rgb);
        }

        void rebuildArea()
        {
            const uint32_t W = scene::width();
            Vec3 cam, fwd;
            if (!W || !render::cameraPosition(cam) || !render::cameraForward(fwd)) return;
            scene::HitInfo h;
            const Vec3 center = scene::hitInfo(cam + fwd * PICK_START, fwd, PICK_RANGE, h) ? cam + fwd * (PICK_START + h.t) : cam + fwd * AREA_FALLBACK;
            const Vec3* pos = scene::positions();
            const Vec3* nrm = scene::normals();
            const uint8_t* cov = scene::coverage();
            std::vector<std::pair<float, uint32_t>> picked;
            for (uint32_t t : scene::covered())
            {
                const float d = fb::distanceSq(pos[t], center);
                if (d <= AREA_RADIUS * AREA_RADIUS) picked.push_back({ d, t });
            }
            if (picked.size() > AREA_MAX)
            {
                std::nth_element(picked.begin(), picked.begin() + AREA_MAX, picked.end());
                picked.resize(AREA_MAX);
            }
            float scale = MIN_SCALE;
            for (const auto& [d, t] : picked)
            {
                float c[3];
                if (rgbAt(shipped(0), shipped(1), t, c)) scale = (std::max)({ scale, c[0], c[1], c[2] });
            }
            std::vector<Seg> segs;
            segs.reserve(picked.size() * 2);
            for (const auto& [d, t] : picked)
            {
                float v[3];
                const ImU32 col = displayedAt(t, v) ? toneColor(v, scale) : IM_COL32(40, 40, 40, 255);
                float half = CROSS_HALF;
                if (t % W + 1 < W && cov[t + 1]) half = (std::clamp)(0.5f * std::sqrt(fb::distanceSq(pos[t], pos[t + 1])), CROSS_MIN, CROSS_MAX);
                const Vec3 n = nrm[t];
                const Vec3 ref = std::fabs(n.m_y) < 0.9f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f);
                const Vec3 u = fb::normalized(fb::cross(n, ref)) * half;
                const Vec3 w = fb::normalized(fb::cross(n, u)) * half;
                const Vec3 p = pos[t] + n * CROSS_LIFT;
                segs.push_back({ p - u, p + u, ImColor(col) });
                segs.push_back({ p - w, p + w, ImColor(col) });
            }
            std::lock_guard<std::mutex> lock(g_segMutex);
            g_areaSegs = std::move(segs);
        }

        void tickArea()
        {
            if (scene::phase() != scene::Phase::Ready || gen::running()) return;
            if (!g_areaPrepared)
            {
                g_areaPrepared = scene::prepare(g_areaError);
                if (!g_areaPrepared) return;
            }
            const Clock::time_point now = Clock::now();
            if (now - g_liveAt > std::chrono::milliseconds(LIVE_REFRESH_MS))
            {
                refreshLive();
                g_liveAt = now;
            }
            if (now - g_areaAt > std::chrono::milliseconds(REBUILD_MS))
            {
                g_areaAt = now;
                rebuildArea();
            }
        }
    }

    void renderOverlayUI()
    {
        if (ImGui::Checkbox("lightmap overlay", &g_area) && g_area && (scene::phase() == scene::Phase::Idle || scene::phase() == scene::Phase::Failed))
        {
            g_areaPrepared = false;
            g_areaError.clear();
            scene::start();
        }
        if (!g_area) return;
        const scene::Phase ph = scene::phase();
        if (ph == scene::Phase::Raster || ph == scene::Phase::Terrain)
            ImGui::ProgressBar(scene::progress(), ImVec2(-1, 0), "reading the level");
        else if (ph == scene::Phase::Failed)
            ImGui::TextDisabled("%s", scene::error().c_str());
        else if (!g_areaError.empty())
            ImGui::TextDisabled("%s", g_areaError.c_str());
    }

    void tick()
    {
        if (g_area) tickArea();
        lights::aimRayRequested = g_enabled || spawn::wantsAimRay();
        if (!g_enabled) return;
        std::string why;
        if (!ready(why)) return;
        const Clock::time_point now = Clock::now();
        if (g_pickRequest || (g_follow && now - g_pickAt > std::chrono::milliseconds(REBUILD_MS)))
        {
            g_pickRequest = false;
            g_pickAt = now;
            pick();
        }
        if (g_texel == ~0u) return;
        if (now - g_liveAt > std::chrono::milliseconds(LIVE_REFRESH_MS))
        {
            refreshLive();
            g_liveAt = now;
            g_dirty = true;
        }
        if (g_dirty)
        {
            g_dirty = false;
            buildPreviews();
        }
    }

    void renderOverlay()
    {
        std::lock_guard<std::mutex> lock(g_segMutex);
        const bool depth = render::lineDepthTest;
        if (g_area && !g_areaSegs.empty())
        {
            render::lineDepthTest = true;
            for (const Seg& s : g_areaSegs) render::line3(s.a, s.b, s.c, 2.0f);
        }
        if (g_enabled && !g_segs.empty())
        {
            render::lineDepthTest = !g_throughWalls;
            for (const Seg& s : g_segs) render::line3(s.a, s.b, s.c, 1.5f);
            ImVec2 sp;
            if (!g_label.empty() && render::worldToScreen(g_labelAt, sp)) render::label(sp, g_label.c_str(), ImColor(255, 230, 0, 255));
        }
        render::lineDepthTest = depth;
    }

    void renderUI()
    {
        ImGui::Checkbox("texel inspector", &g_enabled);
        if (!g_enabled) return;
        std::string why;
        if (!ready(why)) { ImGui::TextDisabled("%s", why.c_str()); return; }
        ImGui::Checkbox("follow crosshair", &g_follow);
        ImGui::SameLine();
        if (ImGui::Button("pick now")) g_pickRequest = true;
        ImGui::SameLine();
        if (ImGui::Button("log texel")) logger::info("[enlighten] inspect {}\n{}\n{}", g_pickNote, g_texel != ~0u ? describe(g_texel) : "no texel", g_raySummary);
        bool world = false;
        world |= ImGui::Checkbox("rays", &g_drawRays);
        ImGui::SameLine();
        world |= ImGui::Checkbox("texel grid", &g_drawGrid);
        ImGui::SameLine();
        ImGui::Checkbox("through walls", &g_throughWalls);
        ImGui::SameLine();
        if (ImGui::Checkbox("atlas overview", &g_overview)) g_dirty = true;
        if (world) rebuildWorld();
        if (ImGui::SliderInt("crop radius", &g_radius, 4, 32)) g_dirty = true;
        if (!g_pickNote.empty()) ImGui::TextDisabled("%s", g_pickNote.c_str());
        if (g_texel == ~0u) return;
        if (!g_liveOk) ImGui::TextDisabled("engine solver off: live columns are empty");

        ImGui::TextUnformatted(describe(g_texel).c_str());
        ImGui::TextUnformatted(g_raySummary.c_str());
        const detail::VeEnlighten& ve = detail::g_veEnl;
        ImGui::TextDisabled("VE terrain color %.3f %.3f %.3f  bounce %.2f  sun %.2f  default albedo %.3f %.3f %.3f",
            ve.terrain.m_x, ve.terrain.m_y, ve.terrain.m_z, ve.bounce, ve.sun, ve.albedo.m_x, ve.albedo.m_y, ve.albedo.m_z);
        for (int k = 0; k < 4; ++k)
        {
            if (k) ImGui::SameLine();
            ImGui::TextColored(kRayCol[k].Value, "%s", kRayName[k]);
        }
        for (int k = 1; k < kClsCount; ++k)
        {
            if (k > 1) ImGui::SameLine();
            ImGui::TextColored(ImColor(kClsCol[k]).Value, "%s", kClsName[k]);
        }
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0, 1, 1, 1), "pooled");

        if (g_tiles.srv && g_cropN)
        {
            const uint32_t W = scene::width();
            const float spacing = ImGui::GetStyle().ItemSpacing.x;
            const float disp = (std::min)(220.0f, (ImGui::GetContentRegionAvail().x - 2.0f * spacing) / 3.0f);
            const char* const names[6] = { "shipped", "live", "live / shipped", "texel kind", "shipped direction", "live direction" };
            for (int k = 0; k < 6; ++k)
            {
                if (k % 3) ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::TextDisabled("%s", names[k]);
                const float u0 = float(k % 3) / 3.0f, v0 = float(k / 3) / 2.0f;
                ImGui::PushID(k);
                ImGui::InvisibleButton("##crop", ImVec2(disp, disp));
                ImGui::PopID();
                const ImVec2 mn = ImGui::GetItemRectMin();
                ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<ImTextureID>(g_tiles.srv), mn, ImGui::GetItemRectMax(),
                    ImVec2(u0, v0), ImVec2(u0 + 1.0f / 3.0f, v0 + 0.5f));
                const float c = disp / float(g_cropN);
                ImGui::GetWindowDrawList()->AddRect(ImVec2(mn.x + g_radius * c, mn.y + g_radius * c),
                    ImVec2(mn.x + (g_radius + 1) * c, mn.y + (g_radius + 1) * c), IM_COL32(255, 230, 0, 255));
                if (ImGui::IsItemHovered())
                {
                    const ImVec2 m = ImGui::GetMousePos();
                    const int x = g_cropX0 + int((m.x - mn.x) / c), y = g_cropY0 + int((m.y - mn.y) / c);
                    if (x >= 0 && y >= 0 && x < int(W) && y < int(scene::height()))
                    {
                        const uint32_t t = uint32_t(y) * W + x;
                        ImGui::SetTooltip("%s", describe(t).c_str());
                        if (ImGui::IsItemClicked()) { g_follow = false; select(t); }
                    }
                }
                ImGui::EndGroup();
            }
            ImGui::TextDisabled("ratio: blue darker, red brighter than shipped (full color = 4x); click a texel to select it");
        }

        if (g_overview && g_map.srv)
        {
            const float disp = (std::min)(512.0f, ImGui::GetContentRegionAvail().x);
            ImGui::InvisibleButton("##overview", ImVec2(disp, disp * float(g_map.h) / float(g_map.w)));
            const ImVec2 mn = ImGui::GetItemRectMin(), mx = ImGui::GetItemRectMax();
            ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<ImTextureID>(g_map.srv), mn, mx);
            const uint32_t W = scene::width();
            const float sx = (mx.x - mn.x) / float(W), sy = (mx.y - mn.y) / float(scene::height());
            const float px = mn.x + (g_texel % W + 0.5f) * sx, py = mn.y + (g_texel / W + 0.5f) * sy;
            ImGui::GetWindowDrawList()->AddCircle(ImVec2(px, py), 6.0f, IM_COL32(255, 230, 0, 255), 12, 2.0f);
            if (ImGui::IsItemClicked())
            {
                const ImVec2 m = ImGui::GetMousePos();
                const uint32_t bx = uint32_t((m.x - mn.x) / (mx.x - mn.x) * g_map.w), by = uint32_t((m.y - mn.y) / (mx.y - mn.y) * g_map.h);
                const uint32_t t = worstInBlock((std::min)(bx, g_map.w - 1), (std::min)(by, g_map.h - 1));
                if (t != ~0u) { g_follow = false; select(t); }
            }
            ImGui::TextDisabled("whole atlas, live / shipped per 4x4 block; click picks the worst texel of a block");
        }
    }

    void clear()
    {
        g_areaPrepared = false;
        g_areaError.clear();
        g_texel = ~0u;
        g_liveOk = false;
        for (detail::Original& o : g_live) o = detail::Original{};
        release(g_tiles);
        release(g_map);
        g_cropN = 0;
        std::lock_guard<std::mutex> lock(g_segMutex);
        g_segs.clear();
        g_areaSegs.clear();
        g_label.clear();
    }
}

