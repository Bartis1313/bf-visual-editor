#include "textures.h"
#include "texgen.h"
#include "surfacepick.h"
#include "../lights/lights.h"
#include "../../hooks/functions.h"
#include "../../utils/log.h"

#include "../editor_context.h"

#include <Windows.h>
#include <d3d11.h>
#include <imgui.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <atomic>
#include <deque>
#include <format>
#include <fstream>
#include <filesystem>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <map>
#include <set>

#include "textures_internal.h"

namespace editor::textures
{
    using namespace detail;
    namespace detail { uint8_t* liveInstance(uint64_t setKey, uint32_t materialIndex); }

    namespace detail
    {
        struct ShaderSlot { std::string name; uint32_t slot; };

        // bf4: resolved program table, bf3: solution pairs
        template <typename F>
        void forEachPixelPermutation(fb::SurfaceShaderInstance* inst, F&& f)
        {
            if (!inst || !inst->m_shader)
                return;
            auto* sh = static_cast<fb::SurfaceShader*>(inst->m_shader);
#if defined(BFVE_GAME_BF4)
            if (!sh->m_resolved || !sh->m_programs)
                return;
            const auto* refs = static_cast<const fb::ShaderProgramRef*>(sh->m_programs);
            for (uint32_t p = 0; p < sh->m_programCount && p < 4096; ++p)
            {
                fb::ShaderProgramEntry* e = refs[p].m_entry;
                if (e && e->m_pixel && e->m_pixel->m_shader)
                    f(e->m_pixel);
            }
#else
            if (!sh->m_solutionPairs || sh->m_solutionPairCount > 1024)
                return;
            for (uint32_t p = 0; p < sh->m_solutionPairCount; ++p)
            {
                fb::DxShaderSolution* sol = sh->m_solutionPairs[p].m_solution;
                if (sol && sol->m_pixelPermutation && sol->m_pixelPermutation->m_shader)
                    f(sol->m_pixelPermutation);
            }
#endif
        }
        struct CbVar { std::string name; uint32_t cbSlot; uint32_t offset; uint32_t size; };
        struct Reflection { std::vector<ShaderSlot> textures; std::vector<CbVar> vars; };

        Reflection parseRdef(const uint8_t* data, uint32_t size)
        {
            Reflection out;
            if (!data || size < 32 || std::memcmp(data, "DXBC", 4) != 0)
                return out;
            const uint32_t chunkCount = *reinterpret_cast<const uint32_t*>(data + 28);
            for (uint32_t c = 0; c < chunkCount && c < 64; ++c)
            {
                const uint32_t off = *reinterpret_cast<const uint32_t*>(data + 32 + 4 * c);
                if (off + 8 > size || std::memcmp(data + off, "RDEF", 4) != 0)
                    continue;
                const uint32_t chunkSize = *reinterpret_cast<const uint32_t*>(data + off + 4);
                const uint8_t* rd = data + off + 8;
                if (off + 8 + chunkSize > size || chunkSize < 28)
                    break;
                const auto str = [&](uint32_t o) -> std::string
                {
                    if (o >= chunkSize) return std::string();
                    return std::string(reinterpret_cast<const char*>(rd + o), strnlen(reinterpret_cast<const char*>(rd + o), chunkSize - o));
                };
                const uint32_t cbCount = *reinterpret_cast<const uint32_t*>(rd);
                const uint32_t cbOff = *reinterpret_cast<const uint32_t*>(rd + 4);
                const uint32_t resCount = *reinterpret_cast<const uint32_t*>(rd + 8);
                const uint32_t resOff = *reinterpret_cast<const uint32_t*>(rd + 12);
                const uint8_t minor = rd[16], major = rd[17];
                const uint32_t entry = (major > 5 || (major == 5 && minor >= 1)) ? 40 : 32;
                std::unordered_map<std::string, uint32_t> cbSlots;
                for (uint32_t i = 0; i < resCount && i < 128; ++i)
                {
                    const uint8_t* e = rd + resOff + i * entry;
                    if (resOff + (i + 1) * entry > chunkSize)
                        break;
                    const uint32_t nameOff = *reinterpret_cast<const uint32_t*>(e);
                    const uint32_t type = *reinterpret_cast<const uint32_t*>(e + 4);
                    const uint32_t bind = *reinterpret_cast<const uint32_t*>(e + 20);
                    if (type == 2 && bind < 128)
                        out.textures.push_back({ str(nameOff), bind });
                    else if (type == 0 && bind < 16)
                        cbSlots[str(nameOff)] = bind;
                }
                const uint32_t varSize = major >= 5 ? 40 : 24;
                for (uint32_t b = 0; b < cbCount && b < 32; ++b)
                {
                    const uint8_t* cb = rd + cbOff + b * 24;
                    if (cbOff + (b + 1) * 24 > chunkSize) break;
                    const std::string cbName = str(*reinterpret_cast<const uint32_t*>(cb));
                    auto slotIt = cbSlots.find(cbName);
                    if (slotIt == cbSlots.end()) continue;
                    const uint32_t varCount = *reinterpret_cast<const uint32_t*>(cb + 4);
                    const uint32_t varOff = *reinterpret_cast<const uint32_t*>(cb + 8);
                    for (uint32_t v = 0; v < varCount && v < 256; ++v)
                    {
                        const uint8_t* vr = rd + varOff + v * varSize;
                        if (varOff + (v + 1) * varSize > chunkSize) break;
                        out.vars.push_back({ str(*reinterpret_cast<const uint32_t*>(vr)), slotIt->second,
                                             *reinterpret_cast<const uint32_t*>(vr + 4), *reinterpret_cast<const uint32_t*>(vr + 8) });
                    }
                }
                break;
            }
            return out;
        }

        const Reflection& reflectionOf(fb::PixelShaderPermutation* perm)
        {
            static std::unordered_map<void*, Reflection> cache;
            static uint32_t generation = 0;
            if (generation != catalogGeneration)
            {
                cache.clear();
                generation = catalogGeneration;
            }
            auto it = cache.find(perm);
            if (it != cache.end())
                return it->second;
            Reflection r = perm && perm->m_data ? parseRdef(perm->m_data, perm->m_dataSize) : Reflection{ };
            if (cache.size() > 2048)
                cache.clear();
            return cache.emplace(perm, std::move(r)).first->second;
        }

        const std::vector<ShaderParam>& shaderParamNames(const MaterialEntry& m)
        {
            static std::unordered_map<void*, std::vector<ShaderParam>> cache;
            static uint32_t generation = 0;
            static const std::vector<ShaderParam> none;
            if (generation != catalogGeneration)
            {
                cache.clear();
                generation = catalogGeneration;
            }

            auto* inst = reinterpret_cast<fb::SurfaceShaderInstance*>(liveInstance(m.setKey, m.index));
            if (!inst || !inst->m_shader)
                return none;
            if (auto it = cache.find(inst->m_shader); it != cache.end())
                return it->second;

            std::vector<ShaderParam> out;
            const auto add = [&](const std::vector<CbVar>& vars, bool vertex)
            {
                for (const CbVar& v : vars)
                {
                    if (v.name.rfind("external_", 0) != 0)
                        continue;
                    const std::string name = v.name.substr(9);
                    auto it = std::find_if(out.begin(), out.end(),
                        [&](const ShaderParam& p) { return _stricmp(p.name.c_str(), name.c_str()) == 0; });
                    if (it == out.end())
                        out.push_back({ name, v.size, vertex, vertex ? ~0u : v.cbSlot, v.offset, shaderParamHandle(name.c_str()) });
                    else if (!vertex && it->vertexOnly)
                    {
                        it->vertexOnly = false;
                        it->cbSlot = v.cbSlot;
                        it->offset = v.offset;
                    }
                }
            };

            auto* sh = static_cast<fb::SurfaceShader*>(inst->m_shader);
#if defined(BFVE_GAME_BF4)
            if (sh->m_resolved && sh->m_programs)
            {
                const auto* refs = static_cast<const fb::ShaderProgramRef*>(sh->m_programs);
                for (uint32_t p = 0; p < sh->m_programCount && p < 4096; ++p)
                {
                    fb::ShaderProgramEntry* e = refs[p].m_entry;
                    if (!e)
                        continue;
                    if (e->m_pixel && e->m_pixel->m_data)
                        add(reflectionOf(e->m_pixel).vars, false);
                    if (e->m_vertex && e->m_vertex->m_data)
                    {
                        static std::unordered_map<const void*, Reflection> vsCache;
                        auto vit = vsCache.find(e->m_vertex->m_data);
                        if (vit == vsCache.end())
                        {
                            if (vsCache.size() > 2048)
                                vsCache.clear();
                            vit = vsCache.emplace(e->m_vertex->m_data, parseRdef(e->m_vertex->m_data, e->m_vertex->m_dataSize)).first;
                        }
                        add(vit->second.vars, true);
                    }
                }
            }
#else
            forEachPixelPermutation(inst, [&](fb::PixelShaderPermutation* perm) { add(reflectionOf(perm).vars, false); });
#endif
            if (out.empty())
            {
                static std::unordered_set<void*> logged;
                if (logged.insert(inst->m_shader).second)
                {
#if defined(BFVE_GAME_BF4)
                    logger::info("[textures] shader reads: nothing for {} [{}] shader {} resolved {} programs {}",
                        m.meshName, m.index, inst->m_shader, sh->m_resolved, sh->m_programCount);
#else
                    logger::info("[textures] shader reads: nothing for {} [{}] shader {}", m.meshName, m.index, inst->m_shader);
#endif
                }
                return none; // programs resolve on first draw
            }
            else
            {
                static std::unordered_set<void*> logged;
                if (logged.insert(inst->m_shader).second)
                {
                    std::string names;
                    for (const ShaderParam& p : out)
                        names += (names.empty() ? "" : ", ") + p.name;
                    logger::info("[textures] shader reads for {} [{}] shader {}: {}", m.meshName, m.index, inst->m_shader, names);
                }
            }
            std::sort(out.begin(), out.end(),
                [](const ShaderParam& a, const ShaderParam& b) { return _stricmp(a.name.c_str(), b.name.c_str()) < 0; });
            if (cache.size() > 1024)
                cache.clear();
            return cache.emplace(inst->m_shader, std::move(out)).first->second;
        }

        bool pickedConstant(const MaterialEntry& m, const ShaderParam& p, float out[4])
        {
            if (p.cbSlot >= 3 || p.offset + 16 > 256)
                return false;
            pick::Surface s;
            if (!pick::current(s) || !s.valid)
                return false;
            auto* inst = reinterpret_cast<fb::SurfaceShaderInstance*>(liveInstance(m.setKey, m.index));
            if (!inst || !inst->m_shader)
                return false;
            for (uint32_t i = 0; i < s.hitCount; ++i)
            {
                const pick::Hit& hit = s.hits[i];
                if (!hit.pixelShader || !hit.cbValid[p.cbSlot])
                    continue;
                bool ours = false;
                forEachPixelPermutation(inst, [&](fb::PixelShaderPermutation* perm)
                {
                    if (perm->m_shader == hit.pixelShader)
                        ours = true;
                });
                if (!ours)
                    continue;
                std::memcpy(out, hit.cb[p.cbSlot] + p.offset / 4, 16);
                return true;
            }
            return false;
        }

        // shader names them external_<Parameter>
        int constantMatches(const MaterialEntry& m, fb::PixelShaderPermutation* perm, const pick::Hit& hit)
        {
            if (!m.block || !perm)
                return 0;
            int matches = 0;
            for (const CbVar& v : reflectionOf(perm).vars)
            {
                if (v.cbSlot >= 3 || !hit.cbValid[v.cbSlot] || v.offset + 12 > 256 || v.name.rfind("external_", 0) != 0)
                    continue;
                const std::string param = v.name.substr(9);
                for (uint32_t i = 0; i < m.vecCount; ++i)
                {
                    uint32_t handle = 0;
                    uint16_t offset = 0;
                    if (!paramInfo(static_cast<const uint8_t*>(m.block), i, handle, offset))
                        continue;
                    const char* pn = nameForParamHandle(handle);
                    if (!pn || _stricmp(pn, param.c_str()) != 0)
                        continue;
                    float val[4];
                    std::memcpy(val, paramValue(m.block, offset), 16);
                    const float* c = hit.cb[v.cbSlot] + v.offset / 4;
                    if (std::fabs(c[0] - val[0]) < 1e-3f && std::fabs(c[1] - val[1]) < 1e-3f && std::fabs(c[2] - val[2]) < 1e-3f)
                        ++matches;
                    else
                        --matches;
                    break;
                }
            }
            return matches;
        }

        std::vector<ShaderSlot> parseRdefTextures(const uint8_t* data, uint32_t size)
        {
            std::vector<ShaderSlot> out;
            if (!data || size < 32 || std::memcmp(data, "DXBC", 4) != 0)
                return out;
            const uint32_t chunkCount = *reinterpret_cast<const uint32_t*>(data + 28);
            for (uint32_t c = 0; c < chunkCount && c < 64; ++c)
            {
                const uint32_t off = *reinterpret_cast<const uint32_t*>(data + 32 + 4 * c);
                if (off + 8 > size || std::memcmp(data + off, "RDEF", 4) != 0)
                    continue;
                const uint32_t chunkSize = *reinterpret_cast<const uint32_t*>(data + off + 4);
                const uint8_t* rd = data + off + 8;
                if (off + 8 + chunkSize > size || chunkSize < 28)
                    break;
                const uint32_t resCount = *reinterpret_cast<const uint32_t*>(rd + 8);
                const uint32_t resOff = *reinterpret_cast<const uint32_t*>(rd + 12);
                const uint8_t minor = rd[16], major = rd[17];
                const uint32_t entry = (major > 5 || (major == 5 && minor >= 1)) ? 40 : 32;
                for (uint32_t i = 0; i < resCount && i < 128; ++i)
                {
                    const uint8_t* e = rd + resOff + i * entry;
                    if (resOff + (i + 1) * entry > chunkSize)
                        break;
                    const uint32_t nameOff = *reinterpret_cast<const uint32_t*>(e);
                    const uint32_t type = *reinterpret_cast<const uint32_t*>(e + 4);
                    const uint32_t bind = *reinterpret_cast<const uint32_t*>(e + 20);
                    if (type != 2 || nameOff >= chunkSize || bind >= 128)
                        continue;
                    const char* name = reinterpret_cast<const char*>(rd + nameOff);
                    const size_t maxLen = chunkSize - nameOff;
                    out.push_back({ std::string(name, strnlen(name, maxLen)), bind });
                }
                break;
            }
            return out;
        }

        const std::vector<ShaderSlot>& textureSlotsOf(fb::PixelShaderPermutation* perm)
        {
            static std::unordered_map<void*, std::vector<ShaderSlot>> cache;
            static uint32_t generation = 0;
            if (generation != catalogGeneration)
            {
                cache.clear();
                generation = catalogGeneration;
            }
            auto it = cache.find(perm);
            if (it != cache.end())
                return it->second;
            std::vector<ShaderSlot> slots = perm && perm->m_data ? parseRdefTextures(perm->m_data, perm->m_dataSize) : std::vector<ShaderSlot>{ };
            if (cache.size() > 2048)
                cache.clear();
            return cache.emplace(perm, std::move(slots)).first->second;
        }
    }

    bool surfaceUnderCrosshair(SurfaceInfo& out)
    {
        pick::Surface s;
        if (!pick::current(s))
            return false;
        static SurfaceInfo cached;
        static bool cachedOk = false;
        static uint32_t cachedFrame = ~0u;
        static uint32_t cachedGeneration = 0;
        if (cachedGeneration != catalogGeneration)
        {
            cachedGeneration = catalogGeneration;
            cachedFrame = ~0u;
            cachedOk = false;
        }
        static bool filterSet = false;
        if (materials.empty())
        {
            if (filterSet)
                pick::setShaderFilter(nullptr, 0);
            filterSet = false;
            return false;
        }
        if (s.frame == cachedFrame)
        {
            out = cached;
            return cachedOk;
        }
        cachedFrame = s.frame;
        cachedOk = false;

        struct PsUse { int material; fb::PixelShaderPermutation* perm; };
        static std::unordered_map<void*, std::vector<PsUse>> psIndex;
        static uint32_t psIndexGeneration = 0, psIndexFrame = 0;
        if (psIndexGeneration != catalogGeneration || s.frame - psIndexFrame >= 20 || psIndex.empty())
        {
            psIndex.clear();
            psIndexGeneration = catalogGeneration;
            psIndexFrame = s.frame;
            for (size_t i = 0; i < materials.size(); ++i)
            {
                auto* inst = reinterpret_cast<fb::SurfaceShaderInstance*>(liveInstance(materials[i].setKey, materials[i].index));
                forEachPixelPermutation(inst, [&](fb::PixelShaderPermutation* perm)
                    { psIndex[perm->m_shader].push_back({ int(i), perm }); });
            }
            std::vector<const void*> known;
            known.reserve(psIndex.size());
            for (const auto& [ps, uses] : psIndex)
                known.push_back(ps);
            pick::setShaderFilter(known.data(), known.size());
            filterSet = true;
        }

        fb::PixelShaderPermutation* matchedPerm = nullptr;
        const auto matchHit = [&matchedPerm](const pick::Hit& hit) -> int
        {
            int best = -1, bestScore = -1;
            fb::PixelShaderPermutation* bestPerm = nullptr;
            auto uses = psIndex.find(hit.pixelShader);
            if (uses == psIndex.end())
                return -1;
            for (const PsUse& use : uses->second)
            {
                const size_t i = size_t(use.material);
                if (i >= materials.size())
                    continue;
                const MaterialEntry& m = materials[i];
                fb::PixelShaderPermutation* perm = use.perm;

                int score = 0;
                if (m.block)
                    for (uint32_t t = 0; t < m.texCount; ++t)
                    {
                        uint32_t handle = 0;
                        uint16_t offset = 0;
                        if (!paramInfo(static_cast<const uint8_t*>(m.block), uint32_t(m.vecCount) + t, handle, offset))
                            continue;
                        void* tex = readTex(m.block, offset);
                        if (!tex)
                            continue;
                        auto* dx = static_cast<fb::DxTexture*>(tex);
                        void* const v0 = dx->m_shaderViews[0];
                        void* const v1 = dx->m_shaderViews[1];
                        void* const ov = gen::installedView(tex);
                        for (void* srv : hit.srvs)
                            if (srv && (srv == v0 || srv == v1 || srv == ov))
                            {
                                ++score;
                                break;
                            }
                    }
                const int weighted = score * 4 + constantMatches(m, perm, hit) * 2 + (m.block ? 1 : 0);
                if (weighted > bestScore)
                {
                    bestScore = weighted;
                    best = int(i);
                    bestPerm = perm;
                }
            }
            matchedPerm = bestPerm;
            return best;
        };

        // last passing draw = nearest, bar lighting/fog
        int best = -1;
        uint32_t used = 0;
        for (uint32_t h = s.hitCount; h > 0 && best < 0; --h)
        {
            best = matchHit(s.hits[h - 1]);
            used = h;
        }
        if (best < 0)
        {
            static uint32_t misses = 0;
            if ((++misses % 120) == 1)
            {
                std::string hits;
                for (uint32_t h = s.hitCount; h > 0 && s.hitCount - h < 4; --h)
                    hits += std::format("{} ", s.hits[h - 1].pixelShader);
                const void* sample = psIndex.empty() ? nullptr : psIndex.begin()->first;
                logger::info("[shaders] pick: {} hit(s), none of their pixel shaders is indexed ({}); index has {} shaders, e.g. {}",
                             s.hitCount, hits, psIndex.size(), sample);
            }
            for (uint32_t h = s.hitCount; h > 0; --h)
            {
                std::vector<std::string> bound = texturesOfViews(s.hits[h - 1].srvs, 16, 8);
                if (bound.empty())
                    continue;
                cached = SurfaceInfo{ };
                cached.frame = s.frame;
                cached.textures = std::move(bound);
                cachedOk = true;
                out = cached;
                return true;
            }
            return false;
        }

        const MaterialEntry& m = materials[best];
        cached = SurfaceInfo{ };
        cached.setKey = m.setKey;
        cached.material = m.index;
        cached.meshName = m.meshName;
        cached.shaderName = m.shaderName;
        cached.variationName = m.variationName;
        cached.frame = s.frame;
        if (m.block)
            for (uint32_t t = 0; t < m.texCount; ++t)
            {
                uint32_t handle = 0;
                uint16_t offset = 0;
                if (!paramInfo(static_cast<const uint8_t*>(m.block), uint32_t(m.vecCount) + t, handle, offset))
                    continue;
                void* tex = readTex(m.block, offset);
                std::string path = tex ? texturePath(tex) : std::string();
                if (!path.empty())
                    cached.textures.push_back(std::move(path));
            }
        if (m.dbTexParams)
        {
            auto* db = static_cast<fb::TextureShaderParameter*>(m.dbTexParams);
            for (uint32_t i = 0; i < m.dbTexCount; ++i)
            {
                const std::string path = db[i].m_Value ? assetName(db[i].m_Value) : std::string();
                if (!path.empty() && std::find(cached.textures.begin(), cached.textures.end(), path) == cached.textures.end())
                    cached.textures.push_back(path);
            }
        }
        cached.own = cached.textures.size();

        if (matchedPerm)
            for (const ShaderSlot& sl : textureSlotsOf(matchedPerm))
            {
                if (sl.slot >= 16)
                    continue;
                std::vector<std::string> one = texturesOfViews(&s.hits[used - 1].srvs[sl.slot], 1, 1);
                cached.slots.emplace_back(sl.name, one.empty() ? std::string() : one[0]);
                if (!one.empty() && std::find(cached.textures.begin(), cached.textures.end(), one[0]) == cached.textures.end())
                {
                    cached.textures.insert(cached.textures.begin() + cached.own, one[0]);
                    ++cached.own;
                }
            }

        for (const std::string& b : texturesOfViews(s.hits[used - 1].srvs, 16, 8))
            if (std::find(cached.textures.begin(), cached.textures.end(), b) == cached.textures.end())
                cached.textures.push_back(b);
        cachedOk = true;
        out = cached;
        return true;
    }

    std::vector<std::string> texturesOfViews(void* const* srvs, size_t count, size_t max)
    {
        std::vector<std::string> out;
        for (const TextureEntry& e : entries)
        {
            if (!e.texture || out.size() >= max)
                continue;
            auto* dx = static_cast<fb::DxTexture*>(e.texture);
            void* const v0 = dx->m_shaderViews[0];
            void* const v1 = dx->m_shaderViews[1];
            void* const ov = gen::installedView(e.texture);
            for (size_t i = 0; i < count; ++i)
                if (srvs[i] && (srvs[i] == v0 || srvs[i] == v1 || srvs[i] == ov))
                {
                    if (std::find(out.begin(), out.end(), e.lowerPath) == out.end())
                        out.push_back(e.lowerPath);
                    break;
                }
        }
        return out;
    }

    std::vector<std::string> materialTextures(uint32_t meshHash, uint32_t material, size_t max)
    {
        std::vector<std::string> out;
        for (const MaterialEntry& m : materials)
        {
            if (m.index != material || uint32_t(m.setKey >> 32) != meshHash)
                continue;
            if (m.block)
                for (uint32_t t = 0; t < m.texCount && out.size() < max; ++t)
                {
                    uint32_t handle = 0;
                    uint16_t offset = 0;
                    if (!paramInfo(static_cast<const uint8_t*>(m.block), uint32_t(m.vecCount) + t, handle, offset))
                        continue;
                    void* tex = readTex(m.block, offset);
                    std::string path = tex ? texturePath(tex) : std::string();
                    if (!path.empty() && std::find(out.begin(), out.end(), path) == out.end())
                        out.push_back(std::move(path));
                }
            if (m.dbTexParams)
            {
                auto* db = static_cast<fb::TextureShaderParameter*>(m.dbTexParams);
                for (uint32_t i = 0; i < m.dbTexCount && out.size() < max; ++i)
                {
                    const std::string path = db[i].m_Value ? assetName(db[i].m_Value) : std::string();
                    if (!path.empty() && std::find(out.begin(), out.end(), path) == out.end())
                        out.push_back(path);
                }
            }
            for (const auto& [handle, path] : m.ebxTextures)
                if (out.size() < max && std::find(out.begin(), out.end(), path) == out.end())
                    out.push_back(path);
            if (!out.empty())
                break;
        }
        return out;
    }

    int renderMeshTextures(const std::vector<MeshVariationRef>& meshes)
    {
        if (meshes.empty())
        {
            ImGui::TextDisabled("no mesh beside this light");
            return 0;
        }

        std::vector<std::pair<uint32_t, std::string>> found;
        bool byName = false;
        collectMeshTextures(meshes, found, byName);

        if (found.empty())
        {
            ImGui::TextDisabled("nothing found for this lamp");
            return 0;
        }

        if (byName)
            ImGui::TextDisabled("named after this lamp - its shader names no texture");

        {
            static gen::Params bulk;

            ImGui::SetNextItemWidth(180.0f);
            ImGui::ColorEdit4("##lampall", bulk.colorA,
                ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaBar);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(90.0f);
            ImGui::DragFloat("##lampallb", &bulk.brightness, 0.01f, 0.0f, 8.0f, "x%.2f");

            ImGui::SameLine();
            if (ImGui::SmallButton("tint all"))
                for (const auto& [h, p] : found)
                {
                    std::string err;
                    if (void* const t = textureByPath(p); t && !gen::tintExisting(t, bulk, err))
                        logger::warning("[textures] tint {}: {}", p, err);
                }

            ImGui::SameLine();
            if (ImGui::SmallButton("revert all"))
                for (const auto& [h, p] : found)
                    if (void* const t = textureByPath(p))
                        gen::revert(t);
        }

        ImGui::Separator();

        for (const auto& [handle, path] : found)
        {
            void* const tex = textureByPath(path);
            TextureEntry* te = tex ? const_cast<TextureEntry*>(findTextureEntry(tex)) : nullptr;

            if (!te)
            {
                ImGui::TextDisabled("%s  (not streamed in)", path.c_str());
                continue;
            }

            renderLampTextureRow(*te, nameForHandle(handle), path);
        }

        return int(found.size());
    }

    namespace detail
    {
        void renderShaderNamedTextures(const MaterialEntry& m)
        {
            if (m.shaderName.empty())
                return;
            const size_t slash = m.shaderName.find_last_of('/');
            std::string key = slash == std::string::npos ? m.shaderName : m.shaderName.substr(slash + 1);
            std::transform(key.begin(), key.end(), key.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
            if (key.rfind("ss_", 0) == 0)
                key.erase(0, 3);
            if (key.size() < 4)
                return;

            std::vector<int> hits;
            for (int i = 0; i < int(entries.size()); ++i)
            {
                const TextureEntry& e = entries[i];
                if (!e.asset || e.lowerPath.empty())
                    continue;
                const size_t ls = e.lowerPath.find_last_of('/');
                const std::string leaf = ls == std::string::npos ? e.lowerPath : e.lowerPath.substr(ls + 1);
                if (leaf.find(key) != std::string::npos)
                    hits.push_back(i);
                if (hits.size() >= 8)
                    break;
            }
            if (hits.empty())
                return;

            ImGui::TextDisabled("textures named like the shader");
            for (int i : hits)
            {
                ImGui::PushID(i);
                renderAssetTextureRow(entries[i].asset, 32.0f);
                ImGui::PopID();
            }
        }
    }

    namespace detail
    {
        const MaterialEntry* materialFor(uint64_t setKey, uint32_t index)
        {
            for (const MaterialEntry& m : materials)
                if (m.setKey == setKey && m.index == index)
                    return &m;
            return nullptr;
        }

        uint8_t* liveInstance(uint64_t setKey, uint32_t materialIndex)
        {
            fb::MeshVariationManager* mgr = fb::MeshVariationManager::Singleton();
            if (!mgr || !mgr->m_buckets || !mgr->m_bucketCount)
                return nullptr;
            const uint32_t bucket = uint32_t(setKey & 0xFFFFFFFFull) % mgr->m_bucketCount;
            uint32_t guard = 0;
            for (fb::MeshVariationNode* node = mgr->m_buckets[bucket]; node && guard < 4096;
                 node = node->m_next, ++guard)
            {
                if (node->m_key != setKey || !node->m_set)
                    continue;
                uint8_t* set = static_cast<uint8_t*>(node->m_set);
                uint8_t* const arr = setMaterials(set);
                if (!arr || materialIndex >= setMaterialCount(set))
                    return nullptr;
                return arr + 1ull * materialIndex * layout.instanceStride;
            }
            return nullptr;
        }
    }

    namespace detail
    {
        struct Literal { size_t offset; float value[4]; };

        // IMMEDIATE32 vec4 operand: 0x00004002 + four dwords
        std::vector<Literal> findLiterals(const uint8_t* data, uint32_t size)
        {
            std::vector<Literal> out;
            if (!data || size < 32 || std::memcmp(data, "DXBC", 4) != 0)
                return out;
            const uint32_t chunkCount = *reinterpret_cast<const uint32_t*>(data + 28);
            for (uint32_t c = 0; c < chunkCount && c < 64; ++c)
            {
                const uint32_t off = *reinterpret_cast<const uint32_t*>(data + 32 + 4 * c);
                if (off + 8 > size)
                    continue;
                const char* tag = reinterpret_cast<const char*>(data + off);
                if (std::memcmp(tag, "SHEX", 4) != 0 && std::memcmp(tag, "SHDR", 4) != 0)
                    continue;
                const uint32_t len = *reinterpret_cast<const uint32_t*>(data + off + 4);
                const uint32_t begin = off + 8, end = std::min<uint32_t>(size, begin + len);
                for (uint32_t p = begin + 8; p + 20 <= end; p += 4)
                {
                    const uint32_t tok = *reinterpret_cast<const uint32_t*>(data + p);
                    if ((tok & 0x7FFFFFFFu) != 0x00004002u)
                        continue;
                    Literal l{ p + 4, { } };
                    std::memcpy(l.value, data + p + 4, 16);
                    bool plausible = true;
                    for (float v : l.value)
                        if (!(v == 0.0f || (std::fabs(v) >= 1e-5f && std::fabs(v) <= 1e5f)))
                            plausible = false;
                    if (plausible)
                        out.push_back(l);
                    p += 16;
                }
            }
            return out;
        }

        struct Md5 { uint32_t a = 0x67452301, b = 0xefcdab89, c = 0x98badcfe, d = 0x10325476; };
        void md5Block(Md5& st, const uint8_t* p)
        {
            static const uint32_t K[64] = {
                0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
                0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
                0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
                0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
                0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
                0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
                0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
                0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 };
            static const uint32_t R[64] = {
                7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22, 5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
                4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23, 6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21 };
            uint32_t w[16];
            std::memcpy(w, p, 64);
            uint32_t a = st.a, b = st.b, c = st.c, d = st.d;
            for (uint32_t i = 0; i < 64; ++i)
            {
                uint32_t f, g;
                if (i < 16) { f = (b & c) | (~b & d); g = i; }
                else if (i < 32) { f = (d & b) | (~d & c); g = (5 * i + 1) & 15; }
                else if (i < 48) { f = b ^ c ^ d; g = (3 * i + 5) & 15; }
                else { f = c ^ (b | ~d); g = (7 * i) & 15; }
                const uint32_t t = d;
                d = c; c = b;
                const uint32_t x = a + f + K[i] + w[g];
                b = b + ((x << R[i]) | (x >> (32 - R[i])));
                a = t;
            }
            st.a += a; st.b += b; st.c += c; st.d += d;
        }

        void dxbcSign(uint8_t* blob, uint32_t size)
        {
            const uint8_t* data = blob + 20;
            const uint32_t len = size - 20;
            Md5 st;
            const uint32_t full = len / 64, rest = len % 64;
            for (uint32_t i = 0; i < full; ++i)
                md5Block(st, data + 64 * i);
            const uint32_t bits = len * 8;
            uint8_t block[64];
            if (rest >= 56)
            {
                std::memset(block, 0, 64);
                std::memcpy(block, data + 64 * full, rest);
                block[rest] = 0x80;
                md5Block(st, block);
                std::memset(block, 0, 64);
                std::memcpy(block, &bits, 4);
                const uint32_t tail = (bits >> 2) | 1;
                std::memcpy(block + 60, &tail, 4);
                md5Block(st, block);
            }
            else
            {
                std::memset(block, 0, 64);
                std::memcpy(block, &bits, 4);
                std::memcpy(block + 4, data + 64 * full, rest);
                block[4 + rest] = 0x80;
                const uint32_t tail = (bits >> 2) | 1;
                std::memcpy(block + 60, &tail, 4);
                md5Block(st, block);
            }
            std::memcpy(blob + 4, &st.a, 4);
            std::memcpy(blob + 8, &st.b, 4);
            std::memcpy(blob + 12, &st.c, 4);
            std::memcpy(blob + 16, &st.d, 4);
        }

        struct ShaderPatch
        {
            fb::PixelShaderPermutation* permutation = nullptr;
            ID3D11PixelShader* original = nullptr;
            ID3D11PixelShader* patched = nullptr;
            std::vector<uint8_t> bytes;
            std::vector<uint8_t> originalBytes;
            std::string graph;
        };
        std::vector<ShaderPatch> g_shaderPatches;

        ShaderPatch* patchFor(fb::PixelShaderPermutation* perm)
        {
            for (ShaderPatch& p : g_shaderPatches)
                if (p.permutation == perm)
                    return &p;
            return nullptr;
        }

        bool installPatch(ShaderPatch& p, std::string& err)
        {
            if (!g_pDevice)
            {
                err = "no device";
                return false;
            }
            dxbcSign(p.bytes.data(), uint32_t(p.bytes.size()));
            ID3D11PixelShader* ps = nullptr;
            const HRESULT hr = g_pDevice->CreatePixelShader(p.bytes.data(), p.bytes.size(), nullptr, &ps);
            if (FAILED(hr) || !ps)
            {
                err = std::format("CreatePixelShader failed {:#x}", uint32_t(hr));
                return false;
            }
            if (!p.original)
            {
                p.original = p.permutation->m_shader;
                if (p.original)
                    p.original->AddRef();
            }
            // engine re-creates m_shader from m_data when empty
            if (p.originalBytes.empty())
                p.originalBytes.assign(p.permutation->m_data, p.permutation->m_data + p.permutation->m_dataSize);
            if (p.bytes.size() == p.permutation->m_dataSize)
                std::memcpy(const_cast<unsigned char*>(p.permutation->m_data), p.bytes.data(), p.bytes.size());
            ID3D11PixelShader* old = p.permutation->m_shader;
            p.permutation->m_shader = ps;
            if (old && old != p.original)
                old->Release();
            if (p.patched)
                p.patched->Release();
            p.patched = ps;
            ps->AddRef();
            return true;
        }

        void revertPatch(ShaderPatch& p)
        {
            if (p.permutation && p.originalBytes.size() == p.permutation->m_dataSize)
                std::memcpy(const_cast<unsigned char*>(p.permutation->m_data), p.originalBytes.data(), p.originalBytes.size());
            p.originalBytes.clear();
            if (p.permutation && p.original)
            {
                ID3D11PixelShader* cur = p.permutation->m_shader;
                p.permutation->m_shader = p.original;
                if (cur && cur != p.original)
                    cur->Release();
            }
            if (p.patched)
                p.patched->Release();
            p.patched = nullptr;
            p.original = nullptr;
        }

        void holdShaderPatches()
        {
            for (ShaderPatch& p : g_shaderPatches)
            {
                if (!p.patched || !p.permutation)
                    continue;
                if (p.permutation->m_shader == p.patched)
                    continue;
                static uint32_t held = 0;
                if (++held <= 8 || (held % 64) == 0)
                    logger::info("[textures] pixel shader slot of {} was rewritten by the engine ({} total)", p.graph, held);
                ID3D11PixelShader* cur = p.permutation->m_shader;
                p.permutation->m_shader = p.patched;
                p.patched->AddRef();
                if (cur && cur != p.original)
                    cur->Release();
            }
        }

        std::vector<ShaderEdit> g_pendingShaderEdits;

        bool applyShaderEdit(const ShaderEdit& e)
        {
            bool any = false;
            for (const MaterialEntry& m : materials)
            {
                if (m.shaderName != e.graph)
                    continue;
                auto* inst = reinterpret_cast<fb::SurfaceShaderInstance*>(liveInstance(m.setKey, m.index));
                forEachPixelPermutation(inst, [&](fb::PixelShaderPermutation* perm)
                {
                    if (!perm->m_data)
                        return;
                    ShaderPatch* patch = patchFor(perm);
                    const uint8_t* reference = patch && !patch->originalBytes.empty() ? patch->originalBytes.data()
                                             : patch ? patch->bytes.data() : perm->m_data;
                    std::vector<size_t> where;
                    for (const Literal& l : findLiterals(reference, perm->m_dataSize))
                        if (std::memcmp(l.value, e.original, 16) == 0)
                            where.push_back(l.offset);
                    if (where.empty())
                        return;
                    if (!patch)
                    {
                        ShaderPatch np;
                        np.permutation = perm;
                        np.bytes.assign(perm->m_data, perm->m_data + perm->m_dataSize);
                        np.graph = m.shaderName;
                        g_shaderPatches.push_back(std::move(np));
                        patch = &g_shaderPatches.back();
                    }
                    for (size_t off : where)
                        std::memcpy(patch->bytes.data() + off, e.value, 16);
                    std::string err;
                    if (installPatch(*patch, err))
                        any = true;
                    else
                        logger::warning("[textures] saved shader edit on {}: {}", e.graph, err);
                });
                if (any)
                    break;
            }
            return any;
        }

        void applyPendingShader()
        {
            for (size_t i = 0; i < g_pendingShaderEdits.size();)
            {
                if (applyShaderEdit(g_pendingShaderEdits[i]))
                {
                    logger::info("[textures] config: shader edit on {} applied", g_pendingShaderEdits[i].graph);
                    g_pendingShaderEdits.erase(g_pendingShaderEdits.begin() + i);
                }
                else
                    ++i;
            }
        }

        void clearShaderPatches()
        {
            for (ShaderPatch& p : g_shaderPatches)
            {
                if (p.patched) p.patched->Release();
                if (p.original) p.original->Release();
            }
            g_shaderPatches.clear();
        }

        const std::vector<Literal>& literalsOf(fb::PixelShaderPermutation* perm)
        {
            static std::unordered_map<void*, std::vector<Literal>> cache;
            static uint32_t generation = ~0u;
            if (generation != catalogGeneration)
            {
                cache.clear();
                generation = catalogGeneration;
            }
            auto it = cache.find(perm);
            if (it != cache.end())
                return it->second;
            const ShaderPatch* patch = patchFor(perm);
            const uint8_t* data = patch && !patch->originalBytes.empty() ? patch->originalBytes.data() : perm->m_data;
            return cache.emplace(perm, findLiterals(data, perm->m_dataSize)).first->second;
        }

        void renderShaderPatches(const MaterialEntry& m)
        {
            auto* inst = reinterpret_cast<fb::SurfaceShaderInstance*>(liveInstance(m.setKey, m.index));
            if (!inst || !inst->m_shader)
            {
                ImGui::TextDisabled("material not realized");
                return;
            }
            struct Program { fb::PixelShaderPermutation* perm; std::string label; };
            std::vector<Program> programs;
#if defined(BFVE_GAME_BF4)
            {
                auto* sh = static_cast<fb::SurfaceShader*>(inst->m_shader);
                if (!sh->m_resolved || !sh->m_programs)
                {
                    ImGui::TextDisabled("graph not resolved yet");
                    return;
                }
                const auto* refs = static_cast<const fb::ShaderProgramRef*>(sh->m_programs);
                for (uint32_t i = 0; i < sh->m_programCount && i < 32; ++i)
                {
                    fb::ShaderProgramEntry* e = refs[i].m_entry;
                    if (e && e->m_pixel && e->m_pixel->m_data)
                        programs.push_back({ e->m_pixel, std::format("program {}  pass {}/{}", i, e->m_pass, e->m_subPass) });
                }
            }
#else
            {
                uint32_t i = 0;
                forEachPixelPermutation(inst, [&](fb::PixelShaderPermutation* perm)
                {
                    if (perm->m_data)
                        programs.push_back({ perm, std::format("solution {}", i) });
                    ++i;
                });
                if (programs.empty())
                {
                    ImGui::TextDisabled("graph has no pixel solutions yet");
                    return;
                }
            }
#endif

            struct Candidate
            {
                float original[4];
                float value[4];
                std::vector<std::pair<fb::PixelShaderPermutation*, size_t>> where;
            };
            static bool allConstants = false;
            ImGui::Checkbox("all constants", &allConstants);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("every float4 literal, not only the bright ones");
            std::vector<Candidate> cands;
            for (const Program& pr : programs)
            {
                fb::PixelShaderPermutation* perm = pr.perm;
                const ShaderPatch* patch = patchFor(perm);
                for (const Literal& l : literalsOf(perm))
                {
                    const bool bright = l.value[0] > 1.0f || l.value[1] > 1.0f || l.value[2] > 1.0f;
                    const bool zero = l.value[0] == 0.0f && l.value[1] == 0.0f && l.value[2] == 0.0f && l.value[3] == 0.0f;
                    if (!bright && !(allConstants && !zero))
                        continue;
                    float cur[4];
                    std::memcpy(cur, patch ? patch->bytes.data() + l.offset : perm->m_data + l.offset, 16);
                    bool merged = false;
                    for (Candidate& c : cands)
                        if (std::memcmp(c.original, l.value, 16) == 0)
                        {
                            c.where.emplace_back(perm, l.offset);
                            merged = true;
                            break;
                        }
                    if (!merged)
                    {
                        Candidate c{ };
                        std::memcpy(c.original, l.value, 16);
                        std::memcpy(c.value, cur, 16);
                        c.where.emplace_back(perm, l.offset);
                        cands.push_back(std::move(c));
                    }
                }
            }

            ImGui::TextDisabled("glow candidates: bright constants of this graph's pixel shaders (%zu)", cands.size());
            if (cands.empty())
                ImGui::TextDisabled("none above 1.0 - open the programs below");
            for (size_t k = 0; k < cands.size(); ++k)
            {
                Candidate& c = cands[k];
                ImGui::PushID(int(k) + 700);
                float mx = 1.0f;
                for (int ch = 0; ch < 3; ++ch)
                    if (c.value[ch] > mx) mx = c.value[ch];
                float rgb[3] = { c.value[0] / mx, c.value[1] / mx, c.value[2] / mx };
                float strength = mx;
                ImGui::SetNextItemWidth(160.0f);
                bool changed = ImGui::ColorEdit3("##cv", rgb, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(90.0f);
                changed |= ImGui::DragFloat("##str", &strength, 0.05f, 0.0f, 50.0f, "x%.2f");
                ImGui::SameLine();
                ImGui::TextDisabled("was %.2f %.2f %.2f  in %zu place(s)", c.original[0], c.original[1], c.original[2], c.where.size());
                float v[4] = { rgb[0] * strength, rgb[1] * strength, rgb[2] * strength, c.value[3] };
                if (changed)
                {
                    for (auto& [perm, offset] : c.where)
                    {
                        ShaderPatch* patch = patchFor(perm);
                        if (!patch)
                        {
                            ShaderPatch np;
                            np.permutation = perm;
                            np.bytes.assign(perm->m_data, perm->m_data + perm->m_dataSize);
                            np.graph = m.shaderName;
                            g_shaderPatches.push_back(std::move(np));
                            patch = &g_shaderPatches.back();
                        }
                        std::memcpy(patch->bytes.data() + offset, v, 16);
                    }
                }
                ImGui::PopID();
            }
            {
                bool anyEdited = false, anyInstalled = false;
                for (const ShaderPatch& p : g_shaderPatches)
                    if (p.graph == m.shaderName) { anyEdited = true; if (p.patched) anyInstalled = true; }
                if (anyEdited)
                {
                    if (ImGui::Button("apply all"))
                        for (ShaderPatch& p : g_shaderPatches)
                            if (p.graph == m.shaderName)
                            {
                                std::string err;
                                if (!installPatch(p, err))
                                    logger::warning("[textures] patch: {}", err);
                            }
                    ImGui::SameLine();
                    if (ImGui::Button("revert all"))
                    {
                        for (size_t i = 0; i < g_shaderPatches.size();)
                        {
                            if (g_shaderPatches[i].graph == m.shaderName)
                            {
                                revertPatch(g_shaderPatches[i]);
                                g_shaderPatches.erase(g_shaderPatches.begin() + i);
                            }
                            else
                                ++i;
                        }
                    }
                    ImGui::SameLine();
                    ImGui::TextDisabled("%s", anyInstalled ? "installed" : "edited, not applied");
                }
            }

            if (!ImGui::TreeNode("all programs and literals"))
                return;
            for (uint32_t i = 0; i < programs.size(); ++i)
            {
                fb::PixelShaderPermutation* perm = programs[i].perm;
                ShaderPatch* patch = patchFor(perm);
                const uint8_t* data = patch ? patch->bytes.data() : perm->m_data;
                const std::vector<Literal> lits = findLiterals(data, perm->m_dataSize);

                char label[160];
                std::snprintf(label, sizeof(label), "%s  %u bytes  %zu literal(s)%s##p%u",
                    programs[i].label.c_str(), perm->m_dataSize, lits.size(), patch ? "  [patched]" : "", i);
                if (!ImGui::TreeNode(label))
                    continue;

                for (size_t k = 0; k < lits.size(); ++k)
                {
                    float v[4];
                    std::memcpy(v, lits[k].value, 16);
                    ImGui::PushID(int(k));
                    float mx = 1.0f;
                    for (int ch = 0; ch < 3; ++ch)
                        if (std::fabs(v[ch]) > mx) mx = std::fabs(v[ch]);
                    float rgb[3] = { v[0] / mx, v[1] / mx, v[2] / mx };
                    float strength = mx;
                    ImGui::SetNextItemWidth(120.0f);
                    bool changed = ImGui::ColorEdit3("##lc", rgb, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(80.0f);
                    changed |= ImGui::DragFloat("##ls", &strength, 0.05f, 0.0f, 50.0f, "x%.2f");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(70.0f);
                    changed |= ImGui::DragFloat("##lw", &v[3], 0.01f, -1000.0f, 1000.0f, "w %.2f");
                    if (changed)
                    {
                        v[0] = rgb[0] * strength;
                        v[1] = rgb[1] * strength;
                        v[2] = rgb[2] * strength;
                    }
                    ImGui::SameLine();
                    ImGui::TextDisabled("%.2f %.2f %.2f @%zx", lits[k].value[0], lits[k].value[1], lits[k].value[2], lits[k].offset);
                    if (changed)
                    {
                        if (!patch)
                        {
                            ShaderPatch np;
                            np.permutation = perm;
                            np.bytes.assign(perm->m_data, perm->m_data + perm->m_dataSize);
                            np.graph = m.shaderName;
                            g_shaderPatches.push_back(std::move(np));
                            patch = &g_shaderPatches.back();
                        }
                        std::memcpy(patch->bytes.data() + lits[k].offset, v, 16);
                    }
                    ImGui::PopID();
                }
                if (patch)
                {
                    if (ImGui::SmallButton("apply"))
                    {
                        std::string err;
                        if (installPatch(*patch, err))
                            logger::info("[textures] pixel shader of {} program {} patched", m.shaderName, i);
                        else
                            logger::warning("[textures] patch: {}", err);
                    }
                    ImGui::SameLine();
                    if (ImGui::SmallButton("revert"))
                    {
                        revertPatch(*patch);
                        g_shaderPatches.erase(g_shaderPatches.begin() + (patch - g_shaderPatches.data()));
                        patch = nullptr;
                    }
                    if (patch)
                    {
                        ImGui::SameLine();
                        ImGui::TextDisabled("%s", patch->patched ? "installed" : "edited, not applied");
                    }
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        void renderShadersTab()
        {
            ImGui::Separator();
            ImGui::TextDisabled("pixel shader constants");
            {
                static bool fromCrosshair = false;
                static bool holdPick = false;
                static uint64_t heldKey = 0;
                static uint32_t heldIndex = 0;
                if (ImGui::Checkbox("surface under the crosshair", &fromCrosshair) && fromCrosshair)
                    pick::enabled = true;
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("material the GPU pick names (turns the pick on)");
                ImGui::SameLine();
                ImGui::Checkbox("hold", &holdPick);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("keep the last picked material");
                ImGui::SameLine();
                ImGui::Checkbox("catalogued only", &pick::onlyKnown);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("only draws of catalogued materials (cheaper); off: every draw, uncatalogued surfaces too");
                editor::lights::PlacedMesh pm;
                MeshVariationRef ref;
                static int pickedIndex = -1;
                if (fromCrosshair)
                {
                    const MaterialEntry* picked = nullptr;
                    SurfaceInfo si;
                    if ((holdPick || ImGui::IsAnyItemActive()) && heldKey)
                        picked = materialFor(heldKey, heldIndex);
                    else if (surfaceUnderCrosshair(si) && !si.meshName.empty())
                    {
                        picked = materialFor(si.setKey, si.material);
                        if (picked)
                        {
                            heldKey = si.setKey;
                            heldIndex = si.material;
                        }
                    }
                    if (!picked && heldKey)
                        picked = materialFor(heldKey, heldIndex);
                    static uint64_t chosenKey = 0;
                    static uint32_t chosenIndex = 0;
                    static uint32_t chosenMesh = 0;
                    if (picked)
                    {
                        const uint32_t mesh = uint32_t(picked->setKey >> 32);
                        std::vector<const MaterialEntry*> mats;
                        for (const MaterialEntry& mm : materials)
                            if (uint32_t(mm.setKey >> 32) == mesh)
                                mats.push_back(&mm);
                        std::sort(mats.begin(), mats.end(), [](const MaterialEntry* a, const MaterialEntry* b)
                            { return a->index != b->index ? a->index < b->index : a->setKey < b->setKey; });
                        if (chosenMesh != mesh)
                            chosenKey = 0;
                        chosenMesh = mesh;
                        const MaterialEntry* chosen = nullptr;
                        for (const MaterialEntry* mm : mats)
                            if (chosenKey && mm->setKey == chosenKey && mm->index == chosenIndex)
                                chosen = mm;
                        if (!chosen && !picked->block) // the plain set often has no block
                            for (const MaterialEntry* mm : mats)
                                if (!chosen && mm->index == picked->index && mm->block)
                                    chosen = mm;
                        if (chosen)
                            picked = chosen;
                        if (mats.size() > 1)
                        {
                            const auto leaf = [](const std::string& n) { const size_t k = n.find_last_of('/'); return k == std::string::npos ? n.c_str() : n.c_str() + k + 1; };
                            ImGui::TextDisabled("%zu materials of %s:", mats.size(), leaf(picked->meshName));
                            for (const MaterialEntry* mm : mats)
                            {
                                char lab[200];
                                std::snprintf(lab, sizeof(lab), "[%u] %s%s%s%s##m%llx_%u", mm->index, leaf(mm->shaderName),
                                              mm->variationName.empty() ? "" : "  ", mm->variationName.empty() ? "" : leaf(mm->variationName),
                                              mm->block ? "" : "  - no parameters", (unsigned long long)mm->setKey, mm->index);
                                if (ImGui::RadioButton(lab, mm == picked))
                                {
                                    chosenKey = mm->setKey;
                                    chosenIndex = mm->index;
                                    heldKey = mm->setKey;
                                    heldIndex = mm->index;
                                    picked = mm;
                                }
                            }
                        }
                    }
                    if (picked)
                    {
                        const size_t slash = picked->shaderName.find_last_of('/');
                        ImGui::Text("%s [%u]  %s", picked->meshName.c_str(), picked->index,
                            slash == std::string::npos ? picked->shaderName.c_str() : picked->shaderName.c_str() + slash + 1);
                        if (!picked->block)
                            ImGui::TextDisabled("no parameter block - the shader reads its defaults; add one below");
                        ImGui::PushID(picked);
                        renderVectorParams(*picked, false, nullptr);
                        ImGui::PopID();
                        renderShaderPatches(*picked);
                    }
                    else
                        ImGui::TextDisabled(holdPick ? "nothing held yet - aim at a surface" : "aim at a surface (GPU pick on)");
                }
                else if (!editor::lights::currentMesh(pm) || !editor::lights::meshRefFor(pm, ref))
                    ImGui::TextDisabled("lock a mesh in the Meshes tab first");
                else
                {
                    const MaterialEntry* picked = nullptr;
                    for (int i = 0; i < int(materials.size()); ++i)
                    {
                        const MaterialEntry& mm = materials[i];
                        const bool same = ref.key != 0 && (mm.setKey == ref.key ||
                            ((ref.key >> 32) != 0 && (mm.setKey >> 32) == (ref.key >> 32)));
                        if (!same)
                            continue;
                        const size_t slash = mm.shaderName.find_last_of('/');
                        const char* leaf = mm.shaderName.empty() ? "?"
                            : (slash == std::string::npos ? mm.shaderName.c_str() : mm.shaderName.c_str() + slash + 1);
                        char lab[200];
                        std::snprintf(lab, sizeof(lab), "[%u] %s##pick%d", mm.index, leaf, i);
                        if (ImGui::RadioButton(lab, pickedIndex == i))
                            pickedIndex = i;
                        if (pickedIndex == i)
                            picked = &mm;
                    }
                    if (picked)
                        renderShaderPatches(*picked);
                    else
                        ImGui::TextDisabled("pick a material");
                }
            }

        }
    }

    std::vector<ShaderEdit> listShaderEdits()
    {
        std::vector<ShaderEdit> out;
        for (const ShaderPatch& p : g_shaderPatches)
        {
            if (!p.permutation || p.bytes.empty())
                continue;
            const uint8_t* reference = !p.originalBytes.empty() ? p.originalBytes.data() : p.permutation->m_data;
            if (!reference)
                continue;
            for (const Literal& l : findLiterals(reference, uint32_t(p.bytes.size())))
            {
                if (l.offset + 16 > p.bytes.size() || std::memcmp(reference + l.offset, p.bytes.data() + l.offset, 16) == 0)
                    continue;
                ShaderEdit e;
                e.graph = p.graph;
                std::memcpy(e.original, reference + l.offset, 16);
                std::memcpy(e.value, p.bytes.data() + l.offset, 16);
                bool dup = false;
                for (const ShaderEdit& o : out)
                    if (o.graph == e.graph && std::memcmp(o.original, e.original, 16) == 0 && std::memcmp(o.value, e.value, 16) == 0)
                        dup = true;
                if (!dup)
                    out.push_back(std::move(e));
            }
        }
        return out;
    }

    void queueShaderEdit(const ShaderEdit& e) { g_pendingShaderEdits.push_back(e); }

    size_t pendingShaderWork() { return g_pendingShaderEdits.size(); }
}
