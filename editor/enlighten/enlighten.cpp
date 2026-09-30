#include "enlighten.h"
#include "enlighten_internal.h"
#include "enlighten_scene.h"
#include "enlighten_spawn.h"
#include "enlighten_db.h"
#include "enlighten_gen.h"
#include "enlighten_inspect.h"
#include "enlighten_relight.h"
#include "enlighten_look.h"
#include "../editor_context.h"
#include "../../SDK/fb.h"
#include "../../hooks/functions.h"
#include "../../utils/log.h"

#include <d3d11.h>
#include <algorithm>
#include <cmath>
#include <cstring>

namespace editor::enlighten
{
    namespace detail
    {
        namespace
        {
            constexpr uint64_t FNV_BASIS = 0xCBF29CE484222325ull; // fnv-1a 64
            constexpr uint64_t FNV_PRIME = 0x100000001B3ull;
            constexpr int VE_CHANGE_REFRESH = 120; // VE updates re-solving after an Enlighten/sun change
            constexpr int KICK_OFF_UPDATES = 2; // kick: updates with Enable off
            constexpr int KICK_MIN_UPDATES = 3;
            constexpr uint16_t PROBE_DIRTY = 1; // LightProbeInstanceState bit0
            constexpr char STATIC_SUFFIX[] = "_Static"; // StaticEnlightenData name -> EnlightenDataAsset "_Dynamic"
        }

        void* g_staticAsset = nullptr;
        std::string g_staticName;
        bool g_dynamicEnable = false;
        bool g_scanned = false;
        uint64_t g_veSig = 0; // 0 = none yet
        json g_pendingRuntime;

        uint64_t hashBytes(const void* p, size_t n, uint64_t h)
        {
            const auto* b = static_cast<const uint8_t*>(p);
            for (size_t i = 0; i < n; ++i) h = (h ^ b[i]) * FNV_PRIME;
            return h;
        }
        Original g_orig[SLOT_COUNT];
        fb::DxTexture* g_skyVisTex = nullptr;
        RuntimeEdit g_rt;

        bool isDxTexture(const void* p)
        {
            return p && fb::isValidPtr(p) &&
                static_cast<const fb::DxTexture*>(p)->m_vtable == fb::DxTexture::VTable();
        }

#if defined(BFVE_GAME_BF4)
        fb::DxTexture* resolveAsset(const void* textureAsset)
        {
            if (!textureAsset || !fb::isValidPtr(textureAsset))
                return nullptr;
            const uintptr_t ref = static_cast<const fb::TextureAsset*>(textureAsset)->m_Resource;
            if (ref == ~uintptr_t(0))
                return nullptr;
            void* p = reinterpret_cast<void*>(ref & ~uintptr_t(3)); // low 2 bits tag the ref
            return isDxTexture(p) ? static_cast<fb::DxTexture*>(p) : nullptr;
        }

        fb::EnlightenRuntimeSettings* runtimeSettings()
        {
            auto** slot = reinterpret_cast<fb::EnlightenRuntimeSettings**>(OFF_g_enlightenRuntimeSettings);
            if (*slot)
                return *slot;
            void* registry = *reinterpret_cast<void**>(OFF_g_settingsRegistry);
            if (!registry)
                return nullptr;
            using Fn = fb::EnlightenRuntimeSettings* (__fastcall*)(void*, uintptr_t);
            auto* s = reinterpret_cast<Fn>(OFF_SettingsRegistry_getSettings)(registry, fb::EnlightenRuntimeSettings::ClassInfoPtr());
            if (s)
                *slot = s;
            return s;
        }

#else
        fb::EnlightenRuntimeSettings* runtimeSettings()
        {
            fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
            return r ? r->m_runtimeSettings : nullptr;
        }
#endif

        std::string assetName(const void* asset)
        {
            if (!asset || !fb::isValidPtr(asset))
                return {};
            const char* n = static_cast<const fb::Asset*>(asset)->m_Name;
            if (!n || !fb::isValidPtr(n))
                return {};
            char buf[256];
            strncpy_s(buf, sizeof(buf), n, _TRUNCATE);
            return buf;
        }

#if defined(BFVE_GAME_BF3)
        // bf3: textures come from the paired static entity
        // sub_17A5B10 copies them, sky visibility is the fourth
        bool scan()
        {
            fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
            g_skyVisTex = nullptr;
            g_staticAsset = nullptr;
            g_dynamicEnable = false;
            for (auto& o : g_orig)
                o = Original{};
            fb::EnlightenStaticEntity* st = nullptr;
            size_t sets = 0;
            for (auto** it = r ? r->m_entities.begin() : nullptr; it && it < r->m_entities.end(); ++it)
            {
                fb::EnlightenRendererEntity* e = *it;
                if (!e) continue;
                if (e->m_database && e->m_database->m_dynamicDataEnable) g_dynamicEnable = true;
                if (!e->m_staticEntity || !isDxTexture(e->m_staticEntity->m_luma)) continue;
                ++sets;
                if (st) continue;
                st = e->m_staticEntity;
                if (isDxTexture(e->m_textures[3])) g_skyVisTex = e->m_textures[3];
            }
            if (!st)
            {
                logger::info("[enlighten] no static Enlighten textures on this level (dynamic={})", g_dynamicEnable);
                return false;
            }
            void* data = st->m_data ? *reinterpret_cast<void**>(static_cast<uint8_t*>(st->m_data) + 0x18) : nullptr; // StaticEnlightenData
            g_staticAsset = st;
            g_staticName = assetName(data);
            fb::DxTexture* const tex[SLOT_COUNT] = { st->m_luma, st->m_chroma, st->m_direction };
            for (int i = 0; i < SLOT_COUNT; ++i)
            {
                g_orig[i].tex = isDxTexture(tex[i]) ? tex[i] : nullptr;
                if (g_orig[i].tex)
                {
                    g_orig[i].width = g_orig[i].tex->m_width;
                    g_orig[i].height = g_orig[i].tex->m_height;
                }
            }
            if (sets > 1)
                logger::info("[enlighten] {} static sets on this level: using {}", sets, g_staticName);
            logger::info("[enlighten] static data {} ({}x{}), sky visibility {}, dynamic={}",
                g_staticName, g_orig[0].width, g_orig[0].height, g_skyVisTex != nullptr, g_dynamicEnable);
            return true;
        }
#else
        bool scan()
        {
            fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
            if (!rm)
                return false;

            std::vector<const fb::DxTexture*> bound;
            if (fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance())
                for (auto** it = r->m_entities.begin(); it && it < r->m_entities.end(); ++it)
                    if (fb::EnlightenRendererEntity* e = *it)
                    {
                        bound.push_back(e->m_textures[1]);
                        if (e->m_staticEntity) bound.push_back(e->m_staticEntity->m_textures[1]);
                    }
            std::vector<void*> statics;
            std::vector<std::pair<std::string, fb::DxTexture*>> skyVis;

            void* found = nullptr;
            bool dynamic = false;
            g_skyVisTex = nullptr;
            for (const auto& comp : rm->m_compartments)
            {
                if (!comp)
                    continue;
                for (const auto& obj : comp->m_objects)
                {
                    if (!obj)
                        continue;
                    fb::ClassInfo* ci = fb::classOf(obj);
                    const char* cn = ci && ci->m_InfoData ? ci->m_InfoData->m_Name : nullptr;
                    if (!cn)
                        continue;
                    if (strcmp(cn, "StaticEnlightenData") == 0)
                    {
                        auto* sd = static_cast<fb::StaticEnlightenData*>(static_cast<void*>(obj));
                        if (const fb::DxTexture* luma = resolveAsset(sd->m_StaticIrradianceLumaTexture))
                        {
                            statics.push_back(obj);
                            if (!found && std::find(bound.begin(), bound.end(), luma) != bound.end())
                                found = obj;
                        }
                    }
                    else if (strcmp(cn, "EnlightenDataAsset") == 0)
                    {
                        auto* da = static_cast<fb::EnlightenDataAsset*>(static_cast<void*>(obj));
                        dynamic = dynamic || da->m_DynamicEnable;
                        if (fb::DxTexture* vis = resolveAsset(da->m_SkyVisibilityTexture))
                            skyVis.emplace_back(assetName(obj), vis);
                    }
                }
            }
            if (!found && !statics.empty())
                found = statics.front();
            if (found)
            {
                std::string want = assetName(found);
                if (const size_t p = want.rfind(STATIC_SUFFIX); p != std::string::npos)
                    want.replace(p, sizeof(STATIC_SUFFIX) - 1, "_Dynamic");
                for (const auto& [name, vis] : skyVis)
                    if (name == want) g_skyVisTex = vis;
            }
            if (!g_skyVisTex && !skyVis.empty())
                g_skyVisTex = skyVis.front().second;
            if (statics.size() > 1 || skyVis.size() > 1)
                logger::info("[enlighten] {} static sets, {} data assets on this level: using {}", statics.size(), skyVis.size(), assetName(found));

            g_dynamicEnable = dynamic;
            g_staticAsset = found;
            g_staticName = assetName(found);
            for (auto& o : g_orig)
                o = Original{};

            if (found)
            {
                auto* sd = static_cast<fb::StaticEnlightenData*>(found);
                const void* assets[SLOT_COUNT] = {
                    sd->m_StaticIrradianceLumaTexture,
                    sd->m_StaticIrradianceChromaTexture,
                    sd->m_StaticDirectionTexture,
                };
                for (int i = 0; i < SLOT_COUNT; ++i)
                {
                    g_orig[i].tex = resolveAsset(assets[i]);
                    if (g_orig[i].tex)
                    {
                        g_orig[i].width = g_orig[i].tex->m_width;
                        g_orig[i].height = g_orig[i].tex->m_height;
                    }
                }
                logger::info("[enlighten] static data {} ({}x{}, luma fmt {}, chroma fmt {}), dynamic={}",
                    g_staticName, g_orig[0].width, g_orig[0].height,
                    g_orig[0].tex ? g_orig[0].tex->m_format : 0u,
                    g_orig[1].tex ? g_orig[1].tex->m_format : 0u, dynamic);
            }
            else
            {
                logger::info("[enlighten] no static Enlighten textures on this level (dynamic={})", dynamic);
            }
            return found != nullptr;
        }
#endif

        uint32_t typedFormat(uint32_t f)
        {
            switch (f)
            {
            case DXGI_FORMAT_R16_TYPELESS: return DXGI_FORMAT_R16_UNORM;
            case DXGI_FORMAT_R8G8B8A8_TYPELESS: return DXGI_FORMAT_R8G8B8A8_UNORM;
            case DXGI_FORMAT_B8G8R8A8_TYPELESS: return DXGI_FORMAT_B8G8R8A8_UNORM;
            case DXGI_FORMAT_R8_TYPELESS: return DXGI_FORMAT_R8_UNORM;
            case DXGI_FORMAT_R8G8_TYPELESS: return DXGI_FORMAT_R8G8_UNORM; // bf3 live chroma
            default: return f;
            }
        }

        uint32_t bytesPerPixel(uint32_t f)
        {
            switch (f)
            {
            case DXGI_FORMAT_R16_UNORM: case DXGI_FORMAT_R16_FLOAT: case DXGI_FORMAT_R8G8_UNORM: return 2;
            case DXGI_FORMAT_R8G8B8A8_UNORM: case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            case DXGI_FORMAT_B8G8R8A8_UNORM: case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
            case DXGI_FORMAT_R32_FLOAT: return 4;
            case DXGI_FORMAT_R8_UNORM: return 1;
            default: return 0;
            }
        }

        bool readback(Original& o, std::string& err)
        {
            if (o.read)
                return true;
            if (!o.tex || !o.tex->m_resource || !g_pDevice || !g_pContext)
            {
                err = "texture not resident";
                return false;
            }

            ID3D11Texture2D* src = nullptr;
            if (FAILED(o.tex->m_resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&src))) || !src)
            {
                err = "resource is not a 2D texture";
                return false;
            }

            D3D11_TEXTURE2D_DESC d{};
            src->GetDesc(&d);
            uint32_t fmt = typedFormat(d.Format);
            const bool bc1 = fmt == DXGI_FORMAT_BC1_UNORM || fmt == DXGI_FORMAT_BC1_TYPELESS || fmt == DXGI_FORMAT_BC1_UNORM_SRGB;
            const bool bc4 = fmt == DXGI_FORMAT_BC4_UNORM || fmt == DXGI_FORMAT_BC4_TYPELESS;
            const bool bc3 = fmt == DXGI_FORMAT_BC3_UNORM || fmt == DXGI_FORMAT_BC3_TYPELESS || fmt == DXGI_FORMAT_BC3_UNORM_SRGB;
            if (bc1 || bc3) fmt = DXGI_FORMAT_R8G8B8A8_UNORM;
            if (bc4) fmt = DXGI_FORMAT_R16_UNORM;
            const uint32_t bpp = bytesPerPixel(fmt);
            if (!bpp)
            {
                src->Release();
                err = "unsupported atlas format " + std::to_string(d.Format);
                return false;
            }

            D3D11_TEXTURE2D_DESC sd = d;
            sd.MipLevels = 1;
            sd.ArraySize = 1;
            sd.Usage = D3D11_USAGE_STAGING;
            sd.BindFlags = 0;
            sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            sd.MiscFlags = 0;

            ID3D11Texture2D* staging = nullptr;
            if (FAILED(g_pDevice->CreateTexture2D(&sd, nullptr, &staging)) || !staging)
            {
                src->Release();
                err = "staging texture failed";
                return false;
            }

            g_pContext->CopySubresourceRegion(staging, 0, 0, 0, 0, src, 0, nullptr);

            D3D11_MAPPED_SUBRESOURCE m{};
            if (FAILED(g_pContext->Map(staging, 0, D3D11_MAP_READ, 0, &m)))
            {
                staging->Release();
                src->Release();
                err = "map failed";
                return false;
            }

            o.width = d.Width;
            o.height = d.Height;
            o.dxgi = fmt;
            o.bpp = bpp;
            o.pixels.assign(size_t(d.Width) * d.Height * bpp, 0);
            if (bc1 || bc3 || bc4)
            {
                // 4x4 texel blocks: BC1 8 bytes (565 endpoints, 2-bit indices), BC4 8 bytes (u8 endpoints, 3-bit indices), BC3 = BC4 alpha + BC1
                const uint32_t bw = (d.Width + 3) / 4, bh = (d.Height + 3) / 4;
                for (uint32_t by = 0; by < bh; ++by)
                {
                    const uint8_t* row = static_cast<const uint8_t*>(m.pData) + size_t(by) * m.RowPitch;
                    for (uint32_t bx = 0; bx < bw; ++bx)
                    {
                        const uint8_t* b = row + bx * (bc3 ? 16 : 8);
                        if (bc3)
                        {
                            // BC4 alpha block then 4-color BC1 block
                            uint8_t apal[8];
                            apal[0] = b[0]; apal[1] = b[1];
                            if (b[0] > b[1]) for (int k = 1; k <= 6; ++k) apal[k + 1] = uint8_t(((7 - k) * b[0] + k * b[1]) / 7);
                            else { for (int k = 1; k <= 4; ++k) apal[k + 1] = uint8_t(((5 - k) * b[0] + k * b[1]) / 5); apal[6] = 0; apal[7] = 255; }
                            uint64_t abits = 0;
                            for (int k = 0; k < 6; ++k) abits |= uint64_t(b[2 + k]) << (8 * k);
                            const uint8_t* cb = b + 8;
                            const uint16_t c0 = uint16_t(cb[0] | (cb[1] << 8)), c1 = uint16_t(cb[2] | (cb[3] << 8));
                            uint8_t pal[4][3];
                            const auto expand = [](uint16_t c, uint8_t* out)
                            {
                                out[0] = uint8_t(((c >> 11) & 31) * 255 / 31);
                                out[1] = uint8_t(((c >> 5) & 63) * 255 / 63);
                                out[2] = uint8_t((c & 31) * 255 / 31);
                            };
                            expand(c0, pal[0]); expand(c1, pal[1]);
                            for (int k = 0; k < 3; ++k)
                            {
                                pal[2][k] = uint8_t((2 * pal[0][k] + pal[1][k]) / 3);
                                pal[3][k] = uint8_t((pal[0][k] + 2 * pal[1][k]) / 3);
                            }
                            const uint32_t bits = uint32_t(cb[4] | (cb[5] << 8) | (cb[6] << 16) | (uint32_t(cb[7]) << 24));
                            for (int t = 0; t < 16; ++t)
                            {
                                const uint32_t x = bx * 4 + (t % 4), y = by * 4 + t / 4;
                                if (x >= d.Width || y >= d.Height) continue;
                                const uint8_t* c = pal[(bits >> (2 * t)) & 3];
                                uint8_t* dst = o.pixels.data() + (size_t(y) * d.Width + x) * 4;
                                dst[0] = c[0]; dst[1] = c[1]; dst[2] = c[2]; dst[3] = apal[(abits >> (3 * t)) & 7];
                            }
                        }
                        else if (bc1)
                        {
                            const uint16_t c0 = uint16_t(b[0] | (b[1] << 8)), c1 = uint16_t(b[2] | (b[3] << 8));
                            uint8_t pal[4][3];
                            const auto expand = [](uint16_t c, uint8_t* out)
                            {
                                out[0] = uint8_t(((c >> 11) & 31) * 255 / 31);
                                out[1] = uint8_t(((c >> 5) & 63) * 255 / 63);
                                out[2] = uint8_t((c & 31) * 255 / 31);
                            };
                            expand(c0, pal[0]); expand(c1, pal[1]);
                            for (int k = 0; k < 3; ++k)
                            {
                                if (c0 > c1) { pal[2][k] = uint8_t((2 * pal[0][k] + pal[1][k]) / 3); pal[3][k] = uint8_t((pal[0][k] + 2 * pal[1][k]) / 3); }
                                else { pal[2][k] = uint8_t((pal[0][k] + pal[1][k]) / 2); pal[3][k] = 0; }
                            }
                            const uint32_t bits = uint32_t(b[4] | (b[5] << 8) | (b[6] << 16) | (uint32_t(b[7]) << 24));
                            for (int t = 0; t < 16; ++t)
                            {
                                const uint32_t x = bx * 4 + (t % 4), y = by * 4 + t / 4;
                                if (x >= d.Width || y >= d.Height) continue;
                                const uint8_t* c = pal[(bits >> (2 * t)) & 3];
                                uint8_t* dst = o.pixels.data() + (size_t(y) * d.Width + x) * 4;
                                dst[0] = c[0]; dst[1] = c[1]; dst[2] = c[2]; dst[3] = 255;
                            }
                        }
                        else
                        {
                            const float r0 = b[0] / 255.0f, r1 = b[1] / 255.0f;
                            float pal[8];
                            pal[0] = r0; pal[1] = r1;
                            if (b[0] > b[1]) for (int k = 1; k <= 6; ++k) pal[k + 1] = ((7 - k) * r0 + k * r1) / 7.0f;
                            else { for (int k = 1; k <= 4; ++k) pal[k + 1] = ((5 - k) * r0 + k * r1) / 5.0f; pal[6] = 0.0f; pal[7] = 1.0f; }
                            uint64_t bits = 0;
                            for (int k = 0; k < 6; ++k) bits |= uint64_t(b[2 + k]) << (8 * k);
                            for (int t = 0; t < 16; ++t)
                            {
                                const uint32_t x = bx * 4 + (t % 4), y = by * 4 + t / 4;
                                if (x >= d.Width || y >= d.Height) continue;
                                reinterpret_cast<uint16_t*>(o.pixels.data())[size_t(y) * d.Width + x] = uint16_t(pal[(bits >> (3 * t)) & 7] * 65535.0f + 0.5f);
                            }
                        }
                    }
                }
            }
            else
            {
                for (uint32_t y = 0; y < d.Height; ++y)
                    std::memcpy(o.pixels.data() + size_t(y) * d.Width * bpp,
                        static_cast<const uint8_t*>(m.pData) + size_t(y) * m.RowPitch, size_t(d.Width) * bpp);
            }

            g_pContext->Unmap(staging, 0);
            staging->Release();
            src->Release();
            o.read = true;
            return true;
        }

        void chromaBytes(uint32_t dxgi, int& bx, int& by)
        {
#if defined(BFVE_GAME_BF3)
            // bf3 TextureFormat 9: bytes 0/1 blue/red share, 2/3 zero
            (void)dxgi;
            bx = 0;
#else
            const bool bgra = dxgi == DXGI_FORMAT_B8G8R8A8_UNORM || dxgi == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB ||
                dxgi == DXGI_FORMAT_B8G8R8A8_TYPELESS;
            bx = bgra ? 2 : 0;
#endif
            by = 1;
        }

        void captureRuntime(fb::EnlightenRuntimeSettings* rs)
        {
            if (g_rt.captured || !rs)
                return;
            std::memcpy(g_rt.orig, rs, RUNTIME_SIZE);
            std::memcpy(g_rt.edit, rs, RUNTIME_SIZE);
            g_rt.captured = true;
            logger::info("[enlighten] runtime settings {} captured (enable {}, forceDynamic {}, jobs {})",
                static_cast<void*>(rs), rs->m_Enable, rs->m_ForceDynamic, rs->m_JobCount);
            if (g_pendingRuntime.is_object())
            {
                applyRuntimeJson(g_pendingRuntime);
                g_pendingRuntime = json();
            }
        }

        // force alone resamples only instances in view; dirty ones resample when they come into view
        void markProbeInstancesDirty()
        {
            fb::LightProbeInstanceManager* m = fb::LightProbeInstanceManager::GetInstance();
            if (!m || !m->m_states) return;
            const uint32_t n = m->count();
            for (uint32_t i = 0; i < n; ++i)
                m->m_states[i].m_flags |= PROBE_DIRTY;
        }

        void applyRuntime(fb::EnlightenRuntimeSettings* rs)
        {
            if (!rs || !g_rt.captured)
                return;
            if (g_rt.enabled && g_measuringReference)
                std::memcpy(reinterpret_cast<uint8_t*>(rs) + RUNTIME_BEGIN, g_rt.orig + RUNTIME_BEGIN, RUNTIME_SIZE - RUNTIME_BEGIN);
            else if (g_rt.enabled)
            {
                alignas(16) uint8_t next[RUNTIME_SIZE];
                std::memcpy(next, g_rt.edit, RUNTIME_SIZE);
                db::clampRuntime(reinterpret_cast<fb::EnlightenRuntimeSettings*>(next));
                std::memcpy(reinterpret_cast<uint8_t*>(rs) + RUNTIME_BEGIN, next + RUNTIME_BEGIN, RUNTIME_SIZE - RUNTIME_BEGIN);
            }
            if (g_rt.probeForceFrames > 0)
            {
                rs->m_LightProbeForceUpdate = true;
                markProbeInstancesDirty();
                if (--g_rt.probeForceFrames == 0)
                {
                    const auto* e = reinterpret_cast<const fb::EnlightenRuntimeSettings*>(g_rt.enabled ? g_rt.edit : g_rt.orig);
                    rs->m_LightProbeForceUpdate = e->m_LightProbeForceUpdate;
                }
            }
            if (g_rt.refreshFrames > 0)
            {
                rs->m_TemporalCoherenceThreshold = 0.0f;
                if (--g_rt.refreshFrames == 0)
                {
                    const auto* e = reinterpret_cast<const fb::EnlightenRuntimeSettings*>(g_rt.enabled ? g_rt.edit : g_rt.orig);
                    rs->m_TemporalCoherenceThreshold = e->m_TemporalCoherenceThreshold;
                }
            }
            if (g_rt.kickFrames > 0)
            {
                // Enable off first, then force-update flags
                const bool off = g_rt.kickFrames > g_rt.kickTotal - KICK_OFF_UPDATES;
                rs->m_Enable = !off;
                rs->m_LightProbeForceUpdate = true;
                rs->m_AlbedoForceUpdateEnable = true;
#if defined(BFVE_GAME_BF4)
                rs->m_ForceUpdateStaticLightingBuffersEnable = true;
#endif
                rs->m_TemporalCoherenceThreshold = 0.0f;
                if (--g_rt.kickFrames == 0)
                {
                    const auto* e = reinterpret_cast<const fb::EnlightenRuntimeSettings*>(g_rt.enabled ? g_rt.edit : g_rt.orig);
                    rs->m_Enable = e->m_Enable;
                    rs->m_LightProbeForceUpdate = e->m_LightProbeForceUpdate;
                    rs->m_AlbedoForceUpdateEnable = e->m_AlbedoForceUpdateEnable;
#if defined(BFVE_GAME_BF4)
                    rs->m_ForceUpdateStaticLightingBuffersEnable = e->m_ForceUpdateStaticLightingBuffersEnable;
#endif
                    rs->m_TemporalCoherenceThreshold = e->m_TemporalCoherenceThreshold;
                    logger::info("[enlighten] solver kicked ({} updates)", g_rt.kickTotal);
                }
            }
        }

    }

    using namespace detail;

    void init() {}

    void shutdown()
    {
        scene::clear();
    }

    void clear()
    {
        if (g_rt.captured && g_rt.enabled)
            if (fb::EnlightenRuntimeSettings* rs = runtimeSettings())
                std::memcpy(reinterpret_cast<uint8_t*>(rs) + RUNTIME_BEGIN, g_rt.orig + RUNTIME_BEGIN, RUNTIME_SIZE - RUNTIME_BEGIN);
        spawn::destroyAll();
        relight::clear();
        inspect::clear();
        gen::clearDiagnostics();
        scene::clear();
        g_skyVisTex = nullptr;
        g_rt = RuntimeEdit{};
        g_pendingRuntime = json();
        g_scanned = false;
        g_veSig = 0;
        g_veUpdates = 0;
        relight::enabled = false;
        relight::scale = 1.0f;
        look::settings = look::Settings{};
        ++look::generation;
        db::engineSolver = false;
        db::liveProbes = true;
        db::calibrationStrength = 1.0f;
        db::outputScale = 1.0f;
        g_staticAsset = nullptr;
        g_staticName.clear();
        g_dynamicEnable = false;
        for (auto& o : g_orig)
            o = Original{};
    }

    void onUpdated(fb::VisualEnvironment* ve)
    {
        if (!ve)
            return;
        ++g_veUpdates;
        if (!g_scanned && hasCapturedOriginals)
        {
            scan();
            g_scanned = true;
        }
        fb::EnlightenRuntimeSettings* rs = runtimeSettings();
        captureRuntime(rs);
        applyRuntime(rs);
#if defined(BFVE_GAME_BF3)
        // updateFrameQueue: after 3 solves only within CullDistance
        if (db::liveDatabase()) ve->enlighten.m_CullDistance = -1.0f;
#endif
        const auto& ol = ve->outdoorLight;
        const float sun[11] = { ol.m_SunColor.m_x, ol.m_SunColor.m_y, ol.m_SunColor.m_z, ol.m_SkyColor.m_x, ol.m_SkyColor.m_y, ol.m_SkyColor.m_z,
            ol.m_GroundColor.m_x, ol.m_GroundColor.m_y, ol.m_GroundColor.m_z, ol.m_SunRotationX, ol.m_SunRotationY };
        const uint64_t sig = hashBytes(&ve->enlighten, sizeof(ve->enlighten), hashBytes(sun, sizeof(sun), FNV_BASIS));
        if (g_veSig && sig != g_veSig) runtimeRefresh(VE_CHANGE_REFRESH);
        g_veSig = sig;
        g_veEnl.terrain = ve->enlighten.m_TerrainColor;
        g_veEnl.bounce = ve->enlighten.m_BounceScale;
        g_veEnl.sun = ve->enlighten.m_SunScale;
        if (rs) g_veEnl.albedo = rs->m_AlbedoDefaultColor;
        g_veEnl.valid = true;
        relight::onUpdated(ve);
        scene::tickGameThread();
        spawn::tickGameThread();
        db::tickGameThread();
    }

    void tick()
    {
        relight::flushRestores();
        scene::tick();
        gen::tick();
        db::tickRender();
        inspect::tick();
        relight::tick();
    }

    void renderOverlay()
    {
        inspect::renderOverlay();
        spawn::renderOverlay();
    }

    namespace detail
    {
        VeEnlighten g_veEnl;

        void runtimeRefresh(int frames) { g_rt.refreshFrames = (std::max)(g_rt.refreshFrames, frames); }

        void runtimeKick(int frames)
        {
            g_rt.kickTotal = (std::max)(frames, KICK_MIN_UPDATES);
            g_rt.kickFrames = g_rt.kickTotal;
        }

        void runtimeRevert()
        {
            if (!g_rt.captured)
                return;
            std::memcpy(g_rt.edit, g_rt.orig, RUNTIME_SIZE);
            if (fb::EnlightenRuntimeSettings* rs = runtimeSettings())
                std::memcpy(reinterpret_cast<uint8_t*>(rs) + RUNTIME_BEGIN, g_rt.orig + RUNTIME_BEGIN, RUNTIME_SIZE - RUNTIME_BEGIN);
            g_rt.enabled = false;
        }
    }

    bool hasSaveData()
    {
        return (g_rt.captured && g_rt.enabled) || !spawn::list().empty() || relight::enabled || relight::scale != 1.0f ||
            db::engineSolver || !db::liveProbes || db::calibrationStrength != 1.0f || db::outputScale != 1.0f || !look::isDefault();
    }
    void forceProbeUpdate(int updates) { g_rt.probeForceFrames = (std::max)(updates, 1); }
    void onLightMapRegistered(uint16_t handle, fb::MeshAsset* mesh, const fb::LinearTransform& world) { scene::registered(handle, mesh, world); }
}
