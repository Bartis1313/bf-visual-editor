#include "enlighten_db.h"
#include "enlighten_internal.h"
#include "enlighten_look.h"
#include "enlighten.h"
#include "../editor_context.h"
#include "../states/states.h"
#include "../../hooks/functions.h"
#include "../../utils/log.h"
#include "../../SDK/offsets.h"

#include <imgui.h>
#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <mutex>

namespace fs = std::filesystem;

namespace editor::enlighten::db
{
    namespace
    {
        // .edb: "EDB1", u32 flags, u32 blobSize, u32 relocCount, blob, u32 reloc[relocCount]
        // reloc = blob offset of a u64 holding a blob offset
        struct FileHeader
        {
            char magic[4];
            uint32_t flags, blobSize, relocCount;
        };

        struct Key
        {
            uint32_t w = 0, h = 0, instances = 0, firstHash = 0;
            bool operator==(const Key& o) const { return w == o.w && h == o.h && instances == o.instances && firstHash == o.firstHash; }
        };

        constexpr int DB_ENABLE = 1; // resource flags bit0
        constexpr int DB_DYNAMIC = 2; // resource flags bit1, systems solve live
        constexpr int DB_FLAGS_MASK = 3;
        constexpr int LOADER_MAX_BUFFERS = 8;
        constexpr int LOADER_BUFFER_STRIDE = 80;
        constexpr int LOADER_META_STRIDE = 16; // {flags, blobSize, relocSize, ..}
        constexpr int CAPTURED_KEEP = 4; // oldest capture dropped
        constexpr int RETIRE_TICKS = 120; // game ticks before a swapped-out db is released (frames, solve jobs)
        constexpr int SWAP_WAIT_TICKS = 3; // game ticks between reattach and swap

        constexpr int PROBE_FLOATS = 16; // SH probe: 4 rows of 4 floats
        constexpr int PROBE_ROW_FLOATS = 4;
        constexpr int PROBE_RGB_FLOATS = 12; // rows r, g, b; row 3 occlusion
        constexpr float LAYOUT_NEG_EPS = 1e-4f; // negative row 0 = layout 1
        constexpr int LAYOUT_NEG_RATIO = 100; // layout 1 when over 1 in 100 probes has one
        constexpr int PROBE_FORCE_UPDATES = 10; // VE updates of m_LightProbeForceUpdate

        constexpr uint32_t PROBES_MAGIC = 0x42525045; // 'EPRB', <map>_probes.bin
        constexpr uint32_t PROBES_VERSION = 1;
        constexpr int LIVE_PROBE_INTERVAL = 60; // render ticks between live-probe readbacks
        constexpr float LIVE_PROBE_FLOOR = 0.05f; // ratio denominator floor, share of the mean shipped texel
        constexpr float RATIO_EPS = 1e-9f;
        constexpr float MAX_PROBE_RATIO = 64.0f;
        constexpr int MIN_PROBE_TEXELS = 4; // fewer seen texels take the level ratio

        constexpr int MAX_ATLAS_SIZE = 8192; // gain.bin header bound
        constexpr int GAIN_RADIUS = 1; // 3x3 neighbourhood sums
        constexpr double GAIN_EPS = 1e-4;
        constexpr float TINT_MIN_RAW = 1e-7f; // texel raw sum
        constexpr double TINT_MIN_SHIPPED = 1e-6; // neighbourhood shipped sum
        constexpr double TINT_MIN_RAW_N = 1e-7; // neighbourhood raw sum
        constexpr double TINT_EPS = 1e-3; // on both channel shares
        constexpr float TINT_NORM_EPS = 1e-9f;
        constexpr float FLOOR_BELOW = 0.9f; // floored when raw * gain < 90% of shipped
        constexpr float RAISED_GAIN = 1.25f; // stats only
        constexpr float LOWERED_GAIN = 0.8f; // stats only
        constexpr int FLOOR_SCALE_INTERVAL = 30; // render ticks
        constexpr double MAX_FLOOR_SCALE = 64.0; // current / calibration raw sum
        constexpr int GAIN_REFRESH_UPDATES = 180; // VE updates at TemporalCoherenceThreshold 0, converged systems re-convert
        constexpr int MEASURE_REFRESH_UPDATES = 60; // re-armed every tick while measuring
        constexpr int MEASURE_SETTLE_UPDATES = 4; // VE updates for the restored states to blend
        constexpr int MEASURE_SOLVES = 3; // solves per system after settling
        constexpr int MEASURE_DONE_NUM = 49, MEASURE_DONE_DEN = 50; // 98% of systems

        constexpr uint32_t DIRECTION_UNSET = 0xFFFFFFFFu; // direction output texel never written

        std::mutex g_mutex;
        std::vector<Loaded> g_loaded;
        std::vector<Captured> g_captured;

#if defined(BFVE_GAME_BF4)
        Key keyOf(const fb::EnlightenDatabaseHeader* h)
        {
            Key k{ h->m_atlasWidth, h->m_atlasHeight, h->m_instanceCount, 0 };
            if (h->m_instanceCount && h->m_instances) k.firstHash = h->m_instances[0].m_hash;
            return k;
        }

        Key keyOfFile(const std::vector<uint8_t>& blob)
        {
            Key k;
            if (blob.size() < sizeof(fb::EnlightenDatabaseHeader)) return k;
            fb::EnlightenDatabaseHeader h;
            std::memcpy(&h, blob.data(), sizeof(h));
            k.w = h.m_atlasWidth; k.h = h.m_atlasHeight; k.instances = h.m_instanceCount;
            const uint64_t inst = reinterpret_cast<uint64_t>(h.m_instances); // unrelocated blob offset
            if (k.instances && inst + sizeof(fb::EnlightenInstanceEntry) <= blob.size())
                k.firstHash = reinterpret_cast<const fb::EnlightenInstanceEntry*>(blob.data() + inst)->m_hash;
            return k;
        }
#endif

        bool readFile(const fs::path& p, FileHeader& hdr, std::vector<uint8_t>& blob, std::vector<uint32_t>& relocs)
        {
            std::ifstream f(p, std::ios::binary);
            if (!f || !f.read(reinterpret_cast<char*>(&hdr), sizeof(hdr)) || std::memcmp(hdr.magic, "EDB1", 4) != 0)
                return false;
            blob.resize(hdr.blobSize);
            relocs.resize(hdr.relocCount);
            f.read(reinterpret_cast<char*>(blob.data()), blob.size());
            f.read(reinterpret_cast<char*>(relocs.data()), relocs.size() * 4);
            return bool(f);
        }

#if defined(BFVE_GAME_BF4)
        using AllocFn = void* (__fastcall*)(void* arena, uint64_t size, uint64_t align);
        using FreeFn = void(__fastcall*)(void* arena, void* p);
        void* arenaAlloc(void* arena, size_t size, size_t align) { return reinterpret_cast<AllocFn>(OFF_MemoryArena_alloc)(arena, size, align); }
        void arenaFree(void* arena, void* p) { reinterpret_cast<FreeFn>(OFF_MemoryArena_free)(arena, p); }

        uint8_t* relocatedBlob(void* arena, const std::vector<uint8_t>& data, const std::vector<uint32_t>& relocs)
        {
            auto* mem = static_cast<uint8_t*>(arenaAlloc(arena, data.size(), 16));
            if (!mem) return nullptr;
            std::memcpy(mem, data.data(), data.size());
            for (uint32_t off : relocs)
            {
                if (off + 8 > data.size()) { arenaFree(arena, mem); return nullptr; }
                uint64_t v = 0;
                std::memcpy(&v, mem + off, 8);
                v += reinterpret_cast<uint64_t>(mem);
                std::memcpy(mem + off, &v, 8);
            }
            return mem;
        }
#endif
    }

    std::string directory() { return getEditorRoot() + "/Enlighten"; }

    std::string mapFileName()
    {
        std::string map = getCurrentMapName();
        const size_t slash = map.find_last_of("/\\");
        if (slash != std::string::npos) map = map.substr(slash + 1);
        return map.empty() ? "level" : map;
    }

    const std::vector<Loaded>& loaded() { return g_loaded; }

    // loader slot 3 (request +0x1C count, +0x28 meta[16 B: flags, blobSize, relocSize, ..]; buffers 80 B:
    // +0 blob, +0x10 size, +0x28 relocs, +0x38 reloc bytes)
#if defined(BFVE_GAME_BF4)
    void onLoaderLoad(void* request, void* buffers)
    {
        if (!request || !buffers)
            return;
        auto* req = static_cast<uint8_t*>(request);
        const uint32_t count = *reinterpret_cast<uint32_t*>(req + 0x1C);
        const uint8_t* meta = *reinterpret_cast<uint8_t**>(req + 0x28);
        std::lock_guard<std::mutex> lock(g_mutex);
        for (uint32_t i = 0; i < count && i < LOADER_MAX_BUFFERS; ++i)
        {
            const uint8_t* e = static_cast<uint8_t*>(buffers) + LOADER_BUFFER_STRIDE * i;
            const uint8_t* blob = *reinterpret_cast<uint8_t* const*>(e);
            const uint32_t size = *reinterpret_cast<const uint32_t*>(e + 0x10);
            const uint8_t* rel = *reinterpret_cast<uint8_t* const*>(e + 0x28);
            const uint32_t relBytes = *reinterpret_cast<const uint32_t*>(e + 0x38);
            if (!blob || size < sizeof(fb::EnlightenDatabaseHeader))
                continue;
            Captured c;
            c.flags = meta ? *reinterpret_cast<const uint32_t*>(meta + LOADER_META_STRIDE * i) : DB_ENABLE;
            c.blob.assign(blob, blob + size);
            if (rel && relBytes)
                c.relocs.assign(reinterpret_cast<const uint32_t*>(rel), reinterpret_cast<const uint32_t*>(rel) + relBytes / 4);
            const auto* h = reinterpret_cast<const fb::EnlightenDatabaseHeader*>(blob);
            logger::info("[enlighten] captured database {}x{}: {} bytes, {} relocations, flags {}", h->m_atlasWidth, h->m_atlasHeight, size, c.relocs.size(), c.flags);
            g_captured.push_back(std::move(c));
            if (g_captured.size() > CAPTURED_KEEP)
                g_captured.erase(g_captured.begin());
        }
    }

    namespace
    {
        bool captureFromLive(uint32_t width, uint32_t height, Captured& out, std::string& why);
        const fb::EnlightenDatabaseHeader* levelBlob();
    }

    bool capturedFor(uint32_t width, uint32_t height, Captured& out)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (const fb::EnlightenDatabaseHeader* lb = levelBlob(); lb && lb->m_atlasWidth == width && lb->m_atlasHeight == height)
        {
            const Key want = keyOf(lb);
            for (auto it = g_captured.rbegin(); it != g_captured.rend(); ++it)
                if (keyOfFile(it->blob) == want) { out = *it; return true; }
        }
        for (auto it = g_captured.rbegin(); it != g_captured.rend(); ++it)
        {
            const auto* h = reinterpret_cast<const fb::EnlightenDatabaseHeader*>(it->blob.data());
            if (h->m_atlasWidth == width && h->m_atlasHeight == height) { out = *it; return true; }
        }
        std::string why;
        if (!captureFromLive(width, height, out, why))
        {
            logger::info("[enlighten] database {}x{} not captured at load and not rebuildable: {}", width, height, why);
            return false;
        }
        g_captured.push_back(out);
        return true;
    }
#else
    void onLoaderLoad(void*, void*) {}
    bool capturedFor(uint32_t, uint32_t, Captured&) { return false; }
#endif

    bool writeEdb(const std::string& path, const Captured& c)
    {
        std::error_code ec;
        fs::create_directories(fs::path(path).parent_path(), ec);
        std::ofstream f(path, std::ios::binary);
        if (!f) return false;
        const FileHeader hdr{ { 'E', 'D', 'B', '1' }, c.flags, uint32_t(c.blob.size()), uint32_t(c.relocs.size()) };
        f.write(reinterpret_cast<const char*>(&hdr), sizeof(hdr));
        f.write(reinterpret_cast<const char*>(c.blob.data()), c.blob.size());
        f.write(reinterpret_cast<const char*>(c.relocs.data()), c.relocs.size() * 4);
        return bool(f);
    }

    void* onConstruct(void* arena, int* resourceFlags, void* blob)
    {
#if defined(BFVE_GAME_BF4)
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!blob || !resourceFlags || !arena)
            return blob;
        const auto* header = static_cast<const fb::EnlightenDatabaseHeader*>(blob);
        const Key key = keyOf(header);
        Loaded rec;
        rec.width = key.w; rec.height = key.h; rec.instances = key.instances;
        rec.systems = header->m_systemCount;
        rec.flags = uint32_t(*resourceFlags);
        void* result = blob;

        std::error_code ec;
        if (edbOverrides && fs::is_directory(directory(), ec))
        {
            for (const auto& entry : fs::directory_iterator(directory(), ec))
            {
                if (entry.path().extension() != ".edb")
                    continue;
                FileHeader hdr{};
                std::vector<uint8_t> data;
                std::vector<uint32_t> relocs;
                if (!readFile(entry.path(), hdr, data, relocs) || !(keyOfFile(data) == key))
                    continue;
                uint8_t* mem = relocatedBlob(arena, data, relocs);
                if (!mem)
                {
                    logger::warning("[enlighten] {}: relocation outside the blob, ignored", entry.path().filename().string());
                    continue;
                }
                *resourceFlags = int(hdr.flags);
                rec.systems = reinterpret_cast<const fb::EnlightenDatabaseHeader*>(mem)->m_systemCount;
                rec.file = entry.path().filename().string();
                rec.replaced = true;
                rec.flags = hdr.flags;
                result = mem;
                logger::info("[enlighten] database {}x{} ({} instances) replaced by {}: {} systems, flags {}, {} bytes",
                    key.w, key.h, key.instances, rec.file, rec.systems, hdr.flags, data.size());
                break;
            }
        }
        if (result != blob)
            arenaFree(arena, blob);
        if (!rec.replaced && forceDynamic && rec.systems && !(*resourceFlags & DB_DYNAMIC))
        {
            *resourceFlags |= DB_DYNAMIC;
            rec.forcedDynamic = true;
            rec.flags = uint32_t(*resourceFlags);
        }
        if (!rec.replaced)
            logger::info("[enlighten] database {}x{} ({} instances): {} systems, flags {}", key.w, key.h, key.instances, rec.systems, rec.flags);
        g_loaded.push_back(rec);
        return result;
#else
        (void)arena; (void)resourceFlags;
        return blob;
#endif
    }

#if defined(BFVE_GAME_BF4)
    namespace
    {
        // as 0x140CFF100, 0x140D050F0 re-pairs on m_repairStatic
        struct Detached
        {
            fb::EnlightenRendererEntity* entity;
            fb::EnlightenStaticEntity* staticEntity;
            fb::DxTexture* textures[3];
            fb::DxTexture* direction;
        };
        std::vector<Detached> g_detached;

        void refreshRenderer(fb::EnlightenRenderer* r)
        {
            reinterpret_cast<void(__fastcall*)(fb::EnlightenRenderer*)>(OFF_EnlightenRenderer_selectTextures)(r);
            reinterpret_cast<void(__fastcall*)(fb::EnlightenRenderer*)>(OFF_EnlightenRenderer_updateState)(r);
            r->m_rebind = true;
        }

        void removeStatic(fb::EnlightenRenderer* r, fb::EnlightenStaticEntity* st)
        {
            auto& v = r->m_staticEntities;
            for (auto** p = v.begin(); p && p < v.end(); ++p)
                if (*p == st) { v.erase_unsorted(p); return; }
        }

        void addStatic(fb::EnlightenRenderer* r, fb::EnlightenStaticEntity* st)
        {
            r->m_staticEntities.push_back_in_capacity(st);
        }

        void directionOrderXyz(fb::EnlightenRuntimeDatabase* db)
        {
            for (uint32_t s = 0; db->m_systems && s < db->m_systemCount; ++s)
                if (fb::EnlightenSolveTask* task = db->m_systems[s].m_solveTask)
                    task->m_rgbaDirection = 1;
        }

        Detached detach(fb::EnlightenRenderer* r, fb::EnlightenRendererEntity* e)
        {
            directionOrderXyz(e->m_database);
            Detached d{ e, e->m_staticEntity, { e->m_textures[0], e->m_textures[1], e->m_textures[2] }, e->m_direction };
            removeStatic(r, d.staticEntity);
            const fb::EnlightenRuntimeDatabase* db = e->m_database;
            e->m_textures[0] = db->m_chromaTexture;
            e->m_textures[1] = db->m_lumaTexture;
            e->m_textures[2] = db->m_directionTexture;
            e->m_direction = db->m_directionTexture;
            e->m_staticEntity = nullptr;
            return d;
        }

        void reattach(fb::EnlightenRenderer* r, const Detached& d)
        {
            addStatic(r, d.staticEntity);
            for (int k = 0; k < 3; ++k) d.entity->m_textures[k] = d.textures[k];
            d.entity->m_direction = d.direction;
            d.entity->m_staticEntity = d.staticEntity;
        }

        void reattachAll(fb::EnlightenRenderer* r)
        {
            if (g_detached.empty()) return;
            for (const Detached& d : g_detached) reattach(r, d);
            r->m_repairStatic = true;
            refreshRenderer(r);
            logger::info("[enlighten] {} static entities reattached", g_detached.size());
            g_detached.clear();
        }

        struct Clamp
        {
            bool active = false, saved = false;
            uint32_t probes = 0, cubes = 0;
            fb::EnlightenRuntimeSettings* solverSettings = nullptr;
        } g_clamp;

        bool hasStaticParts(const fb::EnlightenDatabaseHeader* h)
        {
            for (uint32_t i = 0; i < h->m_probeSetCount; ++i)
                if (!h->m_probeSets[i].m_dynamic) return true;
            for (uint32_t i = 0; i < h->m_cubeCount; ++i)
                if (!h->m_cubes[i].m_dynamic) return true;
            return false;
        }

        void restoreClamp()
        {
            if (g_clamp.saved && g_clamp.solverSettings)
            {
                g_clamp.solverSettings->m_LightProbeMaxSourceSolveCount = g_clamp.probes;
                g_clamp.solverSettings->m_CubeMapMaxUpdateCount = g_clamp.cubes;
            }
            g_clamp = Clamp{};
        }

        void activateClamp(fb::EnlightenRuntimeSettings* rs)
        {
            if (g_clamp.active || !rs) return;
            if (!g_clamp.saved)
            {
                g_clamp.probes = rs->m_LightProbeMaxSourceSolveCount;
                g_clamp.cubes = rs->m_CubeMapMaxUpdateCount;
                g_clamp.saved = true;
                g_clamp.solverSettings = rs;
            }
            g_clamp.active = true;
            clampRuntime(rs);
        }

        struct Swapped
        {
            fb::EnlightenRendererEntity* entity = nullptr;
            fb::EnlightenRuntimeDatabase* original = nullptr;
            fb::EnlightenRuntimeDatabase* current = nullptr;
            std::string file;
        } g_swap;
        std::string g_pendingSwap;
        int g_swapWait = 0;

        fb::EnlightenRendererEntity* levelEntity(fb::EnlightenRenderer* r)
        {
            if (g_swap.entity) return g_swap.entity;
            const fb::DxTexture* luma = detail::g_orig[0].tex;
            for (const Detached& d : g_detached)
                if (luma && d.textures[1] == luma) return d.entity;
            fb::EnlightenRendererEntity* first = nullptr;
            for (auto** it = r->m_entities.begin(); it && it < r->m_entities.end(); ++it)
            {
                fb::EnlightenRendererEntity* e = *it;
                if (!e || !e->m_database) continue;
                if (!first) first = e;
                if (luma && (e->m_textures[1] == luma || (e->m_staticEntity && e->m_staticEntity->m_textures[1] == luma))) return e;
            }
            return first;
        }

        const fb::EnlightenDatabaseHeader* levelBlob()
        {
            if (g_swap.original) return g_swap.original->m_blob;
            fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
            fb::EnlightenRendererEntity* e = r ? levelEntity(r) : nullptr;
            return e && e->m_database ? e->m_database->m_blob : nullptr;
        }

        // blob pointers besides the header: +0xD0 probeCount*4, +0xD8 probeCount*16
        bool captureFromLive(uint32_t width, uint32_t height, Captured& out, std::string& why)
        {
            fb::EnlightenRuntimeDatabase* db = g_swap.original;
            if (!db)
                if (fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance())
                {
                    if (fb::EnlightenRendererEntity* e = levelEntity(r); e && e->m_database && e->m_database->m_blob
                        && e->m_database->m_blob->m_atlasWidth == width && e->m_database->m_blob->m_atlasHeight == height)
                        db = e->m_database;
                    for (fb::EnlightenRendererEntity** it = r->m_entities.begin(); !db && it && it < r->m_entities.end(); ++it)
                    {
                        fb::EnlightenRuntimeDatabase* d = *it ? (*it)->m_database : nullptr;
                        if (d && d->m_blob && d->m_blob->m_atlasWidth == width && d->m_blob->m_atlasHeight == height) { db = d; break; }
                    }
                }
            if (!db || !db->m_blob) { why = "no database of this atlas size on the renderer"; return false; }
            const fb::EnlightenDatabaseHeader* h = db->m_blob;
            if (h->m_systemCount) { why = "the live database already carries systems"; return false; }
            const uintptr_t base = reinterpret_cast<uintptr_t>(h);
            const auto off = [&](const void* p) { return reinterpret_cast<uintptr_t>(p) - base; };
            size_t end = sizeof(fb::EnlightenDatabaseHeader);
            const auto reach = [&](const void* p, size_t bytes) { if (p) end = (std::max)(end, size_t(off(p)) + bytes); };
            reach(h->m_instances, size_t(h->m_instanceCount) * sizeof(fb::EnlightenInstanceEntry));
            reach(h->m_terrain, size_t(h->m_terrainCount) * sizeof(fb::EnlightenTerrainEntry));
            reach(h->m_systems, 0);
            reach(h->m_probeSets, size_t(h->m_probeSetCount) * sizeof(fb::EnlightenProbeSetEntry));
            reach(h->m_cubes, size_t(h->m_cubeCount) * sizeof(fb::EnlightenCubeEntry));
            for (uint32_t i = 0; i < h->m_probeSetCount; ++i)
            {
                const fb::EnlightenProbeSetEntry& s = h->m_probeSets[i];
                if (s.m_systemIndices || s.m_core) { why = "probe set " + std::to_string(i) + " is dynamic (layout unknown)"; return false; }
                reach(s.m_cells, size_t(s.m_probeCount) * 4);
                reach(s.m_positions, size_t(s.m_probeCount) * 16);
            }
            for (uint32_t i = 0; i < h->m_cubeCount; ++i)
                if (h->m_cubes[i].m_core) { why = "cube " + std::to_string(i) + " has solver data (layout unknown)"; return false; }
            end = (end + 15) & ~size_t(15);

            out = Captured{};
            out.flags = db->m_flags & DB_FLAGS_MASK;
            out.blob.assign(reinterpret_cast<const uint8_t*>(h), reinterpret_cast<const uint8_t*>(h) + end);
            const auto unrelocate = [&](size_t at)
            {
                uint64_t v;
                std::memcpy(&v, &out.blob[at], 8);
                if (!v) return true;
                const uint64_t o = v - base;
                if (o > end) return false;
                std::memcpy(&out.blob[at], &o, 8);
                out.relocs.push_back(uint32_t(at));
                return true;
            };
            bool ok = true;
            for (size_t at : { offsetof(fb::EnlightenDatabaseHeader, m_instances), offsetof(fb::EnlightenDatabaseHeader, m_terrain),
                offsetof(fb::EnlightenDatabaseHeader, m_systems), offsetof(fb::EnlightenDatabaseHeader, m_probeSets), offsetof(fb::EnlightenDatabaseHeader, m_cubes) })
                ok &= unrelocate(at);
            const size_t sets = h->m_probeSets ? size_t(off(h->m_probeSets)) : 0;
            for (uint32_t i = 0; i < h->m_probeSetCount; ++i)
                for (size_t field : { offsetof(fb::EnlightenProbeSetEntry, m_cells), offsetof(fb::EnlightenProbeSetEntry, m_positions) })
                    ok &= unrelocate(sets + i * sizeof(fb::EnlightenProbeSetEntry) + field);
            if (!ok) { why = "a pointer leaves the blob"; return false; }
            logger::info("[enlighten] database {}x{} rebuilt from the live resource: {} bytes, {} relocations, flags {}",
                width, height, out.blob.size(), out.relocs.size(), out.flags);
            return true;
        }

        std::string g_swapNote;

        void releaseDatabase(fb::EnlightenRuntimeDatabase* db)
        {
            using ReleaseFn = unsigned int(__fastcall*)(fb::EnlightenRuntimeDatabase*);
            reinterpret_cast<ReleaseFn*>(db->m_vtable)[2](db);
        }

        struct Retired { fb::EnlightenRuntimeDatabase* db; int ticks; };
        std::vector<Retired> g_retired;

        void tickRetired(bool all)
        {
            for (auto it = g_retired.begin(); it != g_retired.end();)
            {
                if (all || --it->ticks <= 0)
                {
                    releaseDatabase(it->db);
                    it = g_retired.erase(it);
                }
                else ++it;
            }
        }

        struct PristineProbes
        {
            fb::EnlightenRuntimeDatabase* db = nullptr;
            std::vector<std::vector<float>> sh; // set -> 16 floats per probe
            int layout = 0; // 1: rows R,G,B,O of (dx,dy,dz,c); 2: rows c,dx,dy,dz of (r,g,b,o)
        } g_pristine;
        bool g_probesStale = false;

        const PristineProbes& pristineProbes(fb::EnlightenRuntimeDatabase* db)
        {
            if (g_pristine.db == db) return g_pristine;
            g_pristine = PristineProbes{};
            g_pristine.db = db;
            g_pristine.sh.assign(db->m_probeSetCount, {});
            size_t total = 0, negRow0 = 0;
            for (uint32_t si = 0; si < db->m_probeSetCount; ++si)
            {
                const fb::EnlightenProbeSetRuntime& set = db->m_probeSets[si];
                const uint32_t count = set.m_entry ? set.m_entry->m_probeCount : 0;
                if (!set.m_sh || !count) continue;
                g_pristine.sh[si].assign(set.m_sh, set.m_sh + size_t(count) * PROBE_FLOATS);
                for (uint32_t pi = 0; pi < count; ++pi, ++total)
                    if (set.m_sh[pi * PROBE_FLOATS] < -LAYOUT_NEG_EPS || set.m_sh[pi * PROBE_FLOATS + 1] < -LAYOUT_NEG_EPS || set.m_sh[pi * PROBE_FLOATS + 2] < -LAYOUT_NEG_EPS) ++negRow0;
            }
            g_pristine.layout = total && negRow0 * LAYOUT_NEG_RATIO > total ? 1 : 2;
            return g_pristine;
        }

        void writeProbes(fb::EnlightenRuntimeDatabase* db, const PristineProbes& p, uint32_t si, uint32_t pi, const float* k)
        {
            const fb::EnlightenProbeSetRuntime& set = db->m_probeSets[si];
            const float* o = &p.sh[si][size_t(pi) * PROBE_FLOATS];
            float* dst = set.m_sh + size_t(pi) * PROBE_FLOATS;
            for (int i = 0; i < PROBE_FLOATS; ++i)
            {
                const int ch = p.layout == 1 ? i / PROBE_ROW_FLOATS : i % PROBE_ROW_FLOATS; // 3 = occlusion
                dst[i] = k && ch < 3 ? o[i] * k[ch] : o[i];
            }
        }

        bool probeSetMatches(fb::EnlightenRuntimeDatabase* db, const PristineProbes& p, uint32_t si)
        {
            const fb::EnlightenProbeSetRuntime& set = db->m_probeSets[si];
            return si < p.sh.size() && set.m_sh && set.m_entry && p.sh[si].size() == size_t(set.m_entry->m_probeCount) * PROBE_FLOATS;
        }

        void restorePristine(fb::EnlightenRuntimeDatabase* db)
        {
            if (!db || g_pristine.db != db) return;
            for (uint32_t si = 0; si < db->m_probeSetCount; ++si)
                if (probeSetMatches(db, g_pristine, si))
                    for (uint32_t pi = 0; pi < db->m_probeSets[si].m_entry->m_probeCount; ++pi)
                        writeProbes(db, g_pristine, si, pi, nullptr);
        }

        void restoreOriginal(fb::EnlightenRenderer* r);

        bool swapIn(fb::EnlightenRenderer* r, const std::string& path)
        {
            FileHeader hdr{};
            std::vector<uint8_t> data;
            std::vector<uint32_t> relocs;
            if (!readFile(path, hdr, data, relocs)) { g_swapNote = "cannot read " + path; return false; }
            const Key want = keyOfFile(data);
            fb::EnlightenRendererEntity* e = nullptr;
            bool any = false;
            for (auto** it = r->m_entities.begin(); it && it < r->m_entities.end() && !e; ++it)
            {
                fb::EnlightenRuntimeDatabase* d = *it && *it == g_swap.entity ? g_swap.original : (*it ? (*it)->m_database : nullptr);
                any |= d != nullptr;
                if (d && d->m_blob && keyOf(d->m_blob) == want) e = *it;
            }
            if (!any) { g_swapNote = "no Enlighten entity on this level"; return false; }
            if (!e) { g_swapNote = fs::path(path).filename().string() + " belongs to another level"; return false; }
            if (g_swap.entity && g_swap.entity != e) restoreOriginal(r);
            fb::EnlightenRuntimeDatabase* old = e->m_database;
            void* arena = old->m_arena;
            uint8_t* blob = relocatedBlob(arena, data, relocs);
            if (!blob) { g_swapNote = "bad relocation table"; return false; }
            auto* db = static_cast<fb::EnlightenRuntimeDatabase*>(arenaAlloc(arena, sizeof(fb::EnlightenRuntimeDatabase), 16));
            if (!db) { arenaFree(arena, blob); g_swapNote = "allocation failed"; return false; }
            int flags = int(hdr.flags);
            oBf4_EnlightenDatabase_ctor(db, arena, &flags, blob);
            using AddRefFn = unsigned int(__fastcall*)(fb::EnlightenRuntimeDatabase*);
            reinterpret_cast<AddRefFn*>(db->m_vtable)[1](db);

            fb::EnlightenRuntimeDatabase* level = g_swap.original ? g_swap.original : old;
            const PristineProbes& pristine = pristineProbes(level);
            if (db->m_probeSetCount == level->m_probeSetCount)
                for (uint32_t i = 0; i < db->m_probeSetCount; ++i)
                {
                    const fb::EnlightenProbeSetRuntime& from = level->m_probeSets[i];
                    fb::EnlightenProbeSetRuntime& to = db->m_probeSets[i];
                    if (to.m_sh && to.m_entry && probeSetMatches(level, pristine, i) && from.m_entry->m_probeCount == to.m_entry->m_probeCount)
                        std::memcpy(to.m_sh, pristine.sh[i].data(), pristine.sh[i].size() * 4);
                    to.m_hasStaticData = from.m_hasStaticData;
                    to.m_ready = from.m_ready;
                }

            if (!g_swap.original) { g_swap.original = old; g_swap.entity = e; }
            e->m_database = db;
            refreshRenderer(r);
            if (g_swap.current && g_swap.current != g_swap.original) g_retired.push_back({ g_swap.current, RETIRE_TICKS });
            g_swap.current = db;
            g_swap.file = fs::path(path).filename().string();
            g_probesStale = true;
            g_swapNote = "applied " + g_swap.file + " live (" + std::to_string(db->m_systemCount) + " systems)";
            logger::info("[enlighten] {}", g_swapNote);
            return true;
        }

        void restoreOriginal(fb::EnlightenRenderer* r)
        {
            if (!g_swap.original || !g_swap.entity) return;
            g_swap.entity->m_database = g_swap.original;
            if (r) refreshRenderer(r);
            if (g_swap.current && g_swap.current != g_swap.original) releaseDatabase(g_swap.current);
            g_swap = Swapped{};
            tickRetired(true);
        }
    }

    void applyLive(const std::string& path)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_pendingSwap = path;
        g_swapWait = 0;
        g_swapNote = "applying...";
    }

    fb::EnlightenRuntimeDatabase* liveDatabase() { return g_detached.empty() ? nullptr : g_detached.front().entity->m_database; }

    void clampRuntime(fb::EnlightenRuntimeSettings* rs)
    {
        if (!g_clamp.active || !rs) return;
        rs->m_LightProbeMaxSourceSolveCount = 0;
        rs->m_CubeMapMaxUpdateCount = 0;
    }

    void tickGameThread()
    {
        fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
        if (!r) return;

        std::string pending;
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            pending = g_pendingSwap;
        }
        tickRetired(false);
        if (!pending.empty())
        {
            reattachAll(r);
            if (++g_swapWait >= SWAP_WAIT_TICKS)
            {
                {
                    std::lock_guard<std::mutex> lock(g_mutex);
                    g_pendingSwap.clear();
                }
                if (swapIn(r, pending)) engineSolver = true;
            }
            return;
        }
        if (!engineSolver) reattachAll(r);

        fb::EnlightenRuntimeSettings* rs = r->m_settings;
        bool needClamp = false;
        for (auto** it = r->m_entities.begin(); it && it < r->m_entities.end(); ++it)
        {
            fb::EnlightenRendererEntity* e = *it;
            fb::EnlightenRuntimeDatabase* db = e ? e->m_database : nullptr;
            if (!db || !(db->m_flags & DB_DYNAMIC) || !db->m_systemCount) continue;
            const bool staticParts = hasStaticParts(db->m_blob);
            if (engineSolver && e->m_staticEntity)
            {
                if (staticParts)
                {
                    if (!rs) continue;
                    activateClamp(rs);
                }
                g_detached.push_back(detach(r, e));
                refreshRenderer(r);
                logger::info("[enlighten] entity {}: static data detached, {} systems solve live{}", static_cast<void*>(e), db->m_systemCount,
                    staticParts ? " (static probe sets/cubemaps: their solve budgets held at 0)" : "");
            }
            needClamp |= staticParts && !e->m_staticEntity;
        }
        if (needClamp) activateClamp(rs);
        else if (g_clamp.active) restoreClamp();
        clampRuntime(rs);
        if (g_clamp.active)
            if (fb::EnlightenRuntimeSettings* global = detail::runtimeSettings(); global && global != rs)
                clampRuntime(global);
    }

    namespace
    {
        struct GainRange
        {
            uintptr_t begin, end;
            uint32_t ax, ay, w, h;
            std::vector<float> gain;
            std::vector<float> add;
            std::vector<float> raw;
            double raw0Sum = 0.0;
            float floorScale = 1.0f;
            std::vector<float> tint;
            std::vector<float> shipped;
        };
        constexpr float MAX_TINT = 2.0f; // per-channel tint clamp 1/x..x
        constexpr float MAX_GAIN = 16.0f, MIN_GAIN = 1.0f / 256.0f; // per-texel gain clamp
        std::atomic<const std::vector<GainRange>*> g_gainRanges = nullptr;
        std::vector<std::unique_ptr<std::vector<GainRange>>> g_gainTables;
        fb::EnlightenRuntimeDatabase* g_gainsFor = nullptr;
    }

    std::string gainPath() { return directory() + "/" + mapFileName() + "_gain.bin"; }

    void scaleOutput(float* irradiance, const unsigned short* luma)
    {
        const std::vector<GainRange>* ranges = g_gainRanges.load(std::memory_order_acquire);
        if (!ranges || !irradiance) return;
        const uintptr_t p = reinterpret_cast<uintptr_t>(luma);
        auto it = std::upper_bound(ranges->begin(), ranges->end(), p, [](uintptr_t v, const GainRange& r) { return v < r.begin; });
        if (it == ranges->begin()) return;
        --it;
        if (p >= it->end) return;
        const size_t i = (p - it->begin) / 2;
        float* raw = const_cast<float*>(&it->raw[i * 3]);
        raw[0] = irradiance[0]; raw[1] = irradiance[1]; raw[2] = irradiance[2];
        const float s = calibrationStrength, o = outputScale;
        const float g = 1.0f + (it->gain[i] - 1.0f) * s, f = it->floorScale * s;
        const float* add = &it->add[i * 3];
        const float* tint = &it->tint[i * 3];
        for (int c = 0; c < 3; ++c)
            irradiance[c] = (irradiance[c] * g * (1.0f + (tint[c] - 1.0f) * s) + add[c] * f) * o;
        if (!look::isDefault()) look::apply(irradiance, it->shipped.empty() ? nullptr : &it->shipped[i * 3]);
    }

    void reloadGains()
    {
        g_gainRanges.store(nullptr, std::memory_order_release);
        fb::EnlightenRuntimeDatabase* db = liveDatabase();
        if (!db) return;
        // <map>_gain.bin: u32 w, u32 h, float gain[w*h], float add[3*w*h], float raw0[w*h] (raw luma at calibration),
        // float tint[3*w*h] (optional)
        std::vector<float> atlas, atlasAdd, atlasRaw0, atlasTint;
        uint32_t W = 0, H = 0;
        {
            std::ifstream f(gainPath(), std::ios::binary);
            if (f && f.read(reinterpret_cast<char*>(&W), 4) && f.read(reinterpret_cast<char*>(&H), 4) && W && H && W <= MAX_ATLAS_SIZE && H <= MAX_ATLAS_SIZE)
            {
                atlas.resize(size_t(W) * H);
                if (!f.read(reinterpret_cast<char*>(atlas.data()), std::streamsize(atlas.size() * 4))) atlas.clear();
                atlasAdd.resize(size_t(W) * H * 3);
                if (atlas.empty() || !f.read(reinterpret_cast<char*>(atlasAdd.data()), std::streamsize(atlasAdd.size() * 4))) atlasAdd.clear();
                atlasRaw0.resize(size_t(W) * H);
                if (atlasAdd.empty() || !f.read(reinterpret_cast<char*>(atlasRaw0.data()), std::streamsize(atlasRaw0.size() * 4))) atlasRaw0.clear();
                atlasTint.resize(size_t(W) * H * 3);
                if (atlasRaw0.empty() || !f.read(reinterpret_cast<char*>(atlasTint.data()), std::streamsize(atlasTint.size() * 4))) atlasTint.clear();
            }
        }
        std::string readErr;
        detail::Original& sl = detail::g_orig[0];
        detail::Original& sc = detail::g_orig[1];
        const bool shippedOk = detail::readback(sl, readErr) && detail::readback(sc, readErr) && sl.bpp == 2 &&
            sc.width == sl.width && sc.height == sl.height;
        int sbx = 0, sby = 1;
        if (shippedOk) detail::chromaBytes(sc.dxgi, sbx, sby);
        auto table = std::make_unique<std::vector<GainRange>>();
        for (uint32_t s = 0; db->m_systems && s < db->m_systemCount; ++s)
        {
            const fb::EnlightenRuntimeSystem& sys = db->m_systems[s];
            const fb::EnlightenSystemEntry* entry = sys.m_entry;
            const uint32_t w = entry->m_core->m_width, h = entry->m_core->m_height;
            if (!sys.m_lumaOut || !w || !h) continue;
            const size_t px = size_t(w) * h;
            const uintptr_t lumaOut = reinterpret_cast<uintptr_t>(sys.m_lumaOut);
            GainRange r{ lumaOut, lumaOut + 2 * px, entry->m_atlasX, entry->m_atlasY, w, h,
                std::vector<float>(px, 1.0f), std::vector<float>(px * 3, 0.0f), std::vector<float>(px * 3, 0.0f) };
            r.tint.assign(px * 3, 1.0f);
            if (shippedOk)
            {
                r.shipped.assign(px * 3, 0.0f);
                for (uint32_t y = 0; y < h; ++y)
                    for (uint32_t x = 0; x < w; ++x)
                    {
                        if (r.ax + x >= sl.width || r.ay + y >= sl.height) continue;
                        const size_t a = size_t(r.ay + y) * sl.width + (r.ax + x), l = size_t(y) * w + x;
                        const float lum = reinterpret_cast<const uint16_t*>(sl.pixels.data())[a] / 65535.0f;
                        const uint8_t* c = sc.pixels.data() + a * sc.bpp;
                        const float red = c[sby] / 255.0f, blue = c[sbx] / 255.0f;
                        r.shipped[l * 3] = lum * red;
                        r.shipped[l * 3 + 1] = lum * (std::max)(0.0f, 1.0f - red - blue);
                        r.shipped[l * 3 + 2] = lum * blue;
                    }
            }
            for (uint32_t y = 0; y < h && !atlas.empty(); ++y)
                for (uint32_t x = 0; x < w; ++x)
                {
                    if (r.ax + x >= W || r.ay + y >= H) continue;
                    const size_t a = size_t(r.ay + y) * W + (r.ax + x), l = size_t(y) * w + x;
                    r.gain[l] = atlas[a];
                    if (!atlasAdd.empty()) for (int c = 0; c < 3; ++c) r.add[l * 3 + c] = atlasAdd[a * 3 + c];
                    if (!atlasRaw0.empty()) r.raw0Sum += atlasRaw0[a];
                    if (!atlasTint.empty()) for (int c = 0; c < 3; ++c) r.tint[l * 3 + c] = atlasTint[a * 3 + c];
                }
            table->push_back(std::move(r));
        }
        std::sort(table->begin(), table->end(), [](const GainRange& a, const GainRange& b) { return a.begin < b.begin; });
        if (!atlas.empty())
            logger::info("[enlighten] calibration gains on {} systems", table->size());
        g_gainRanges.store(table.get(), std::memory_order_release);
        g_gainTables.push_back(std::move(table));
        detail::runtimeRefresh(GAIN_REFRESH_UPDATES);
    }

    bool gainAt(uint32_t x, uint32_t y, TexelGain& out)
    {
        const std::vector<GainRange>* ranges = g_gainRanges.load(std::memory_order_acquire);
        if (!ranges) return false;
        for (const GainRange& r : *ranges)
        {
            if (x < r.ax || y < r.ay || x >= r.ax + r.w || y >= r.ay + r.h) continue;
            const size_t l = size_t(y - r.ay) * r.w + (x - r.ax);
            if (l >= r.gain.size()) return false;
            out.gain = r.gain[l];
            out.floorScale = r.floorScale;
            for (int c = 0; c < 3 && l * 3 + c < r.tint.size(); ++c) out.tint[c] = r.tint[l * 3 + c];
            for (int c = 0; c < 3; ++c)
            {
                out.add[c] = l * 3 + c < r.add.size() ? r.add[l * 3 + c] : 0.0f;
                out.raw[c] = l * 3 + c < r.raw.size() ? r.raw[l * 3 + c] : 0.0f;
            }
            return true;
        }
        return false;
    }

    namespace
    {
        struct Measure
        {
            int phase = 0; // 0 idle, 1 waiting for the solver, 2 blending the restored states, 3 re-solving
            bool overrides = true, force = false;
            uint32_t since = 0;
            std::vector<uint32_t> counts;
            std::string note;
        } g_measure;

        void resumeOverrides()
        {
            detail::g_measuringReference = false;
            editor::overridesEnabled = g_measure.overrides;
            g_measure.phase = 0;
            g_measure.force = false;
        }

        void solveCounts(fb::EnlightenRuntimeDatabase* db, std::vector<uint32_t>& out)
        {
            out.clear();
            for (uint32_t s = 0; db->m_systems && s < db->m_systemCount; ++s) out.push_back(db->m_systems[s].m_selectCount);
        }

        void tickMeasure()
        {
            if (!g_measure.phase) return;
            fb::EnlightenRuntimeDatabase* db = liveDatabase();
            if (!db) { g_measure.note = "calibration waits for the engine solver"; return; }
            if (g_measure.phase == 1)
            {
                g_measure.overrides = editor::overridesEnabled;
                editor::overridesEnabled = false;
                states::restoreAll();
                detail::g_measuringReference = true;
                g_measure.since = detail::g_veUpdates;
                g_measure.phase = 2;
                g_measure.note = "measuring the level's own lighting";
                logger::info("[enlighten] calibration: overrides suspended, measuring the level's own lighting");
                return;
            }
            detail::runtimeRefresh(MEASURE_REFRESH_UPDATES);
            if (g_measure.phase == 2)
            {
                if (detail::g_veUpdates - g_measure.since < MEASURE_SETTLE_UPDATES && !g_measure.force) return;
                solveCounts(db, g_measure.counts);
                g_measure.phase = 3;
            }
            std::vector<uint32_t> now;
            solveCounts(db, now);
            size_t done = 0;
            for (size_t i = 0; i < now.size() && i < g_measure.counts.size(); ++i) done += now[i] >= g_measure.counts[i] + MEASURE_SOLVES;
            if (!g_measure.force && done * MEASURE_DONE_DEN < now.size() * MEASURE_DONE_NUM)
            {
                char b[96];
                std::snprintf(b, sizeof(b), "measuring the level's own lighting: %zu of %zu systems re-solved", done, now.size());
                g_measure.note = b;
                return;
            }
            std::string note;
            calibrate(note);
            g_measure.note = note;
            resumeOverrides();
        }
    }

    void requestCalibration()
    {
        if (g_measure.phase) return;
        g_measure.phase = 1;
        g_measure.note = "calibration queued";
    }
    void measureNow() { if (g_measure.phase) g_measure.force = true; }
    bool calibrationPending() { return g_measure.phase != 0; }
    const std::string& calibrationNote() { return g_measure.note; }

    bool calibrate(std::string& note)
    {
        const std::vector<GainRange>* ranges = g_gainRanges.load(std::memory_order_acquire);
        if (!liveDatabase() || !ranges) { note = "turn the engine solver on first"; return false; }
        std::string err;
        detail::Original& sl = detail::g_orig[0];
        detail::Original& sc = detail::g_orig[1];
        if (!detail::readback(sl, err) || !detail::readback(sc, err) || sl.bpp != 2) { note = "shipped atlas readback failed: " + err; return false; }
        int bx, by;
        detail::chromaBytes(sc.dxgi, bx, by);
        const uint32_t W = sl.width, H = sl.height;
        std::vector<float> gain(size_t(W) * H, 1.0f), add(size_t(W) * H * 3, 0.0f), raw0(size_t(W) * H, 0.0f), tint(size_t(W) * H * 3, 1.0f);
        const uint16_t* S = reinterpret_cast<const uint16_t*>(sl.pixels.data());
        size_t measured = 0, floored = 0, raised = 0, lowered = 0;
        double logSum = 0.0;
        // luma = r+g+b, chroma = blue share, red share
        const auto shippedRgb = [&](size_t a, float rgb[3])
        {
            const float ship = S[a] / 65535.0f;
            const uint8_t* c = sc.pixels.data() + a * sc.bpp;
            const float red = c[by] / 255.0f, blue = c[bx] / 255.0f;
            rgb[0] = ship * red; rgb[1] = ship * (std::max)(0.0f, 1.0f - red - blue); rgb[2] = ship * blue;
        };
        for (const GainRange& r : *ranges)
            for (uint32_t y = 0; y < r.h; ++y)
                for (uint32_t x = 0; x < r.w; ++x)
                {
                    if (r.ax + x >= W || r.ay + y >= H) continue;
                    const size_t a = size_t(r.ay + y) * W + (r.ax + x), l = size_t(y) * r.w + x;
                    const float* raw = &r.raw[l * 3];
                    const float rawSum = raw[0] + raw[1] + raw[2];
                    raw0[a] = rawSum;
                    if (!S[a]) continue;
                    const float ship = S[a] / 65535.0f;
                    double sN = 0.0, rN = 0.0, sC[3] = {}, rC[3] = {};
                    for (int dy = -GAIN_RADIUS; dy <= GAIN_RADIUS; ++dy)
                        for (int dx = -GAIN_RADIUS; dx <= GAIN_RADIUS; ++dx)
                        {
                            const int nx = int(x) + dx, ny = int(y) + dy;
                            if (nx < 0 || ny < 0 || nx >= int(r.w) || ny >= int(r.h)) continue;
                            const size_t na = size_t(r.ay + ny) * W + (r.ax + nx), nl = size_t(ny) * r.w + nx;
                            if (r.ax + nx >= W || r.ay + ny >= H || !S[na]) continue;
                            sN += S[na] / 65535.0;
                            rN += double(r.raw[nl * 3]) + r.raw[nl * 3 + 1] + r.raw[nl * 3 + 2];
                            float srgb[3];
                            shippedRgb(na, srgb);
                            for (int c = 0; c < 3; ++c) { sC[c] += srgb[c]; rC[c] += r.raw[nl * 3 + c]; }
                        }
                    const float g = (std::clamp)(float((sN + GAIN_EPS) / (rN + GAIN_EPS)), MIN_GAIN, MAX_GAIN);
                    gain[a] = g;
                    if (rawSum > TINT_MIN_RAW && sN > TINT_MIN_SHIPPED && rN > TINT_MIN_RAW_N)
                    {
                        float t[3], lumaAfter = 0.0f;
                        for (int c = 0; c < 3; ++c)
                        {
                            t[c] = (std::clamp)(float((sC[c] / sN + TINT_EPS) / (rC[c] / rN + TINT_EPS)), 1.0f / MAX_TINT, MAX_TINT);
                            lumaAfter += raw[c] * t[c];
                        }
                        const float norm = lumaAfter > TINT_NORM_EPS ? rawSum / lumaAfter : 1.0f;
                        for (int c = 0; c < 3; ++c) tint[a * 3 + c] = t[c] * norm;
                    }
                    ++measured;
                    raised += g > RAISED_GAIN;
                    lowered += g < LOWERED_GAIN;
                    logSum += std::log(g);
                    if (rawSum * g < ship * FLOOR_BELOW)
                    {
                        float rgb[3];
                        shippedRgb(a, rgb);
                        const float k = 1.0f - rawSum * g / ship;
                        for (int c = 0; c < 3; ++c) add[a * 3 + c] = rgb[c] * k;
                        ++floored;
                    }
                }
        {
            std::ofstream f(gainPath(), std::ios::binary);
            f.write(reinterpret_cast<const char*>(&W), 4);
            f.write(reinterpret_cast<const char*>(&H), 4);
            f.write(reinterpret_cast<const char*>(gain.data()), std::streamsize(gain.size() * 4));
            f.write(reinterpret_cast<const char*>(add.data()), std::streamsize(add.size() * 4));
            f.write(reinterpret_cast<const char*>(raw0.data()), std::streamsize(raw0.size() * 4));
            f.write(reinterpret_cast<const char*>(tint.data()), std::streamsize(tint.size() * 4));
        }
        reloadGains();
        const double geo = measured ? std::exp(logSum / double(measured)) : 1.0;
        char buf[256];
        std::snprintf(buf, sizeof(buf), "%zu texels: %zu raised, %zu lowered, mean x%.2f, %zu floored with the shipped color", measured, raised, lowered, geo, floored);
        note = buf;
        logger::info("[enlighten] calibration: {} -> {}", note, gainPath());
        const detail::VeEnlighten& ve = detail::g_veEnl;
        logger::info("[enlighten] calibrated with VE terrain color {:.3f} {:.3f} {:.3f}, bounce {:.2f}, sun {:.2f}, default albedo {:.3f} {:.3f} {:.3f}",
            ve.terrain.m_x, ve.terrain.m_y, ve.terrain.m_z, ve.bounce, ve.sun, ve.albedo.m_x, ve.albedo.m_y, ve.albedo.m_z);
        return true;
    }

    namespace
    {
        // <map>_probes.bin
        struct LiveProbes
        {
            bool loaded = false, failed = false;
            fb::EnlightenRuntimeDatabase* db = nullptr;
            std::vector<std::vector<std::vector<uint32_t>>> texels; // set -> probe -> texels
            int frame = 0;
            bool applied = false;
            fb::Vec3 global{ 1.0f, 1.0f, 1.0f };
        } g_lp;

        void restoreProbes()
        {
            if (g_lp.applied && g_lp.db)
            {
                restorePristine(g_lp.db);
                forceProbeUpdate(PROBE_FORCE_UPDATES);
            }
            g_lp = LiveProbes{};
        }

        bool loadProbeTexels(fb::EnlightenRuntimeDatabase* db)
        {
            std::ifstream f(directory() + "/" + mapFileName() + "_probes.bin", std::ios::binary);
            if (!f) return false;
            std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            size_t at = 0;
            const auto u32 = [&](uint32_t& v) { if (at + 4 > d.size()) return false; std::memcpy(&v, &d[at], 4); at += 4; return true; };
            uint32_t magic = 0, version = 0, sets = 0;
            if (!u32(magic) || magic != PROBES_MAGIC || !u32(version) || version != PROBES_VERSION || !u32(sets)) return false;
            if (sets != db->m_probeSetCount) return false;
            g_lp.texels.assign(sets, {});
            pristineProbes(db);
            for (uint32_t si = 0; si < sets; ++si)
            {
                uint32_t count = 0;
                if (!u32(count)) return false;
                const fb::EnlightenProbeSetRuntime& set = db->m_probeSets[si];
                if (count && (count != set.m_entry->m_probeCount || !set.m_sh)) return false;
                g_lp.texels[si].resize(count);
                for (uint32_t pi = 0; pi < count; ++pi)
                {
                    if (at + 2 > d.size()) return false;
                    const uint16_t h = uint16_t(d[at] | (d[at + 1] << 8));
                    at += 2;
                    auto& t = g_lp.texels[si][pi];
                    t.resize(h);
                    for (uint16_t k = 0; k < h; ++k) if (!u32(t[k])) return false;
                }
            }
            g_lp.db = db;
            return true;
        }

        // luma R16 = r+g+b, chroma bx blue share, by red share
        void decodeAtlas(const detail::Original& luma, const detail::Original& chroma, int bx, int by, std::vector<fb::Vec3>& out)
        {
            const size_t n = size_t(luma.width) * luma.height;
            out.assign(n, fb::Vec3{});
            const uint16_t* L = reinterpret_cast<const uint16_t*>(luma.pixels.data());
            for (size_t i = 0; i < n; ++i)
            {
                const float l = L[i] / 65535.0f;
                const uint8_t* c = chroma.pixels.data() + i * chroma.bpp;
                const float red = c[by] / 255.0f, blue = c[bx] / 255.0f;
                out[i] = fb::vec3(l * red, l * (std::max)(0.0f, 1.0f - red - blue), l * blue);
            }
        }

        void updateProbes(fb::EnlightenRuntimeDatabase* live, fb::EnlightenRuntimeDatabase* probeDb)
        {
            if ((g_lp.db && g_lp.db != probeDb) || g_probesStale) { restoreProbes(); g_probesStale = false; }
            if (!g_lp.loaded && !g_lp.failed)
            {
                g_lp.loaded = loadProbeTexels(probeDb);
                g_lp.failed = !g_lp.loaded;
                logger::info("[enlighten] live probes: {}", g_lp.loaded ? "texel lists loaded" : "no matching <map>_probes.bin (regenerate)");
            }
            if (!g_lp.loaded || (g_lp.frame++ % LIVE_PROBE_INTERVAL) != 0) return;

            std::string err;
            detail::Original& sl = detail::g_orig[0];
            detail::Original& sc = detail::g_orig[1];
            detail::Original ll, lc;
            ll.tex = live->m_lumaTexture;
            lc.tex = live->m_chromaTexture;
            if (!detail::readback(sl, err) || !detail::readback(sc, err) || !ll.tex || !lc.tex || !detail::readback(ll, err) || !detail::readback(lc, err))
                return;
            if (ll.width != sl.width || ll.height != sl.height || ll.bpp != 2 || sl.bpp != 2 || lc.bpp < 2) return;
            int sbx, sby;
            detail::chromaBytes(sc.dxgi, sbx, sby);
            static std::vector<fb::Vec3> liveRgb, shippedRgb;
            decodeAtlas(ll, lc, 0, 1, liveRgb); // ConvertToOutputFormat: low byte blue, high byte red
            decodeAtlas(sl, sc, sbx, sby, shippedRgb);

            fb::Vec3 gl{}, gs{};
            size_t lit = 0;
            for (size_t i = 0; i < liveRgb.size(); ++i)
            {
                gl += liveRgb[i];
                gs += shippedRgb[i];
                lit += shippedRgb[i].m_x + shippedRgb[i].m_y + shippedRgb[i].m_z > 0.0f;
            }
            const fb::Vec3 floorPerTexel = gs * (LIVE_PROBE_FLOOR / float((std::max)(lit, size_t(1))));
            const auto ratio = [&](const fb::Vec3& a, const fb::Vec3& b, float texels)
            {
                const auto r = [&](float x, float y, float f)
                {
                    const float d = (std::max)(y, f * texels);
                    return d > RATIO_EPS ? (std::clamp)(x / d, 0.0f, MAX_PROBE_RATIO) : 1.0f;
                };
                return fb::vec3(r(a.m_x, b.m_x, floorPerTexel.m_x), r(a.m_y, b.m_y, floorPerTexel.m_y), r(a.m_z, b.m_z, floorPerTexel.m_z));
            };
            const fb::Vec3 global = ratio(gl, gs, 0.0f);
            g_lp.global = global;
            const PristineProbes& pristine = pristineProbes(probeDb);
            for (uint32_t si = 0; si < probeDb->m_probeSetCount && si < g_lp.texels.size(); ++si)
            {
                if (!probeSetMatches(probeDb, pristine, si)) continue;
                const uint32_t count = probeDb->m_probeSets[si].m_entry->m_probeCount;
                for (uint32_t pi = 0; pi < count && pi < g_lp.texels[si].size(); ++pi)
                {
                    const auto& t = g_lp.texels[si][pi];
                    fb::Vec3 a{}, b{};
                    for (uint32_t x : t) { a += liveRgb[x]; b += shippedRgb[x]; }
                    const fb::Vec3 k = t.size() >= MIN_PROBE_TEXELS ? ratio(a, b, float(t.size())) : global;
                    const float kk[3] = { k.m_x, k.m_y, k.m_z };
                    writeProbes(probeDb, pristine, si, pi, kk);
                }
            }
            if (!g_lp.applied)
                logger::info("[enlighten] live probes: {} sets follow the lightmap (layout {}, level ratio {:.2f} {:.2f} {:.2f})",
                    probeDb->m_probeSetCount, pristine.layout, global.m_x, global.m_y, global.m_z);
            g_lp.applied = true;
            forceProbeUpdate(PROBE_FORCE_UPDATES);
        }
    }

    namespace
    {
        struct LevelProbes
        {
            fb::EnlightenRuntimeDatabase* db = nullptr;
        } g_rp;

        fb::EnlightenRuntimeDatabase* levelDatabase()
        {
            if (g_swap.original) return g_swap.original;
            fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
            fb::EnlightenRendererEntity* e = r ? levelEntity(r) : nullptr;
            return e ? e->m_database : nullptr;
        }
    }

    void restoreLevelProbes()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (g_rp.db)
        {
            restorePristine(g_rp.db);
            forceProbeUpdate(PROBE_FORCE_UPDATES);
        }
        g_rp = LevelProbes{};
    }

    void scaleLevelProbes(float r, float g, float b)
    {
        fb::EnlightenRuntimeDatabase* db;
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            db = levelDatabase();
        }
        if (!db || !db->m_probeSetCount) return;
        if (g_rp.db != db)
        {
            restoreLevelProbes();
            g_rp.db = db;
        }
        const PristineProbes& pristine = pristineProbes(db);
        const float k[3] = { r, g, b };
        for (uint32_t si = 0; si < db->m_probeSetCount; ++si)
            if (probeSetMatches(db, pristine, si))
                for (uint32_t pi = 0; pi < db->m_probeSets[si].m_entry->m_probeCount; ++pi)
                    writeProbes(db, pristine, si, pi, k);
        forceProbeUpdate(PROBE_FORCE_UPDATES);
    }

    void tickRender()
    {
        fb::EnlightenRuntimeDatabase* live = liveDatabase();
        if (!live)
        {
            if (g_measure.phase >= 2) resumeOverrides();
            if (g_lp.applied || g_lp.loaded) restoreProbes();
            return;
        }
        if (g_gainsFor != live) { g_gainsFor = live; reloadGains(); }
        tickMeasure();
        static int floorTick = 0;
        if ((floorTick++ % FLOOR_SCALE_INTERVAL) == 0)
            if (const std::vector<GainRange>* ranges = g_gainRanges.load(std::memory_order_acquire))
                for (const GainRange& r : *ranges)
                {
                    if (r.raw0Sum <= 0.0) continue;
                    double sum = 0.0;
                    for (size_t i = 0; i < r.raw.size(); ++i) sum += r.raw[i];
                    if (sum > 0.0) const_cast<GainRange&>(r).floorScale = float((std::clamp)(sum / r.raw0Sum, 0.0, MAX_FLOOR_SCALE)); // 0 = not output since the reload
                }
        fb::EnlightenRuntimeDatabase* probeDb = g_swap.original ? g_swap.original : live;
        if (liveProbes && probeDb->m_probeSetCount) updateProbes(live, probeDb);
        else if (g_lp.applied || g_lp.loaded) restoreProbes();
    }

    void onLevelUnload()
    {
        if (g_measure.phase >= 2) resumeOverrides();
        g_measure = Measure{};
        restoreLevelProbes();
        std::lock_guard<std::mutex> lock(g_mutex);
        fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
        if (r)
            for (const Detached& d : g_detached)
                reattach(r, d);
        g_detached.clear();
        restoreProbes();
        restoreOriginal(r);
        g_pristine = PristineProbes{};
        g_pendingSwap.clear();
        g_swapNote.clear();
        g_gainRanges.store(nullptr, std::memory_order_release);
        g_gainTables.clear();
        g_gainsFor = nullptr;
        restoreClamp();
        engineSolver = false;
        g_loaded.clear();
    }

    namespace
    {
        void logSystems(fb::EnlightenRuntimeDatabase* db, uint32_t rendererState)
        {
            logger::info("[enlighten] database {}: renderer state {}, {} systems", static_cast<void*>(db), rendererState, db->m_systemCount);
            for (uint32_t s = 0; db->m_systems && s < db->m_systemCount; ++s)
            {
                const fb::EnlightenRuntimeSystem& sys = db->m_systems[s];
                const uint32_t px = sys.m_entry->m_core->m_width * sys.m_entry->m_core->m_height;
                const auto stats16 = [&](const uint16_t* p, uint32_t& nz, uint32_t& mx, double& mean)
                {
                    nz = mx = 0; mean = 0.0;
                    if (!p) return;
                    for (uint32_t i = 0; i < px; ++i) { nz += p[i] != 0; mx = (std::max)(mx, uint32_t(p[i])); mean += p[i]; }
                    mean /= (std::max)(px, 1u);
                };
                uint32_t nzC, mxC, nzL, mxL, dirSet = 0;
                double meanC, meanL;
                stats16(sys.m_chromaOut, nzC, mxC, meanC);
                stats16(sys.m_lumaOut, nzL, mxL, meanL);
                if (sys.m_directionOut)
                    for (uint32_t i = 0; i < px; ++i) dirSet += sys.m_directionOut[i] != DIRECTION_UNSET;
                constexpr int LIGHT_TYPES = 3, LIGHT_SUN = 2; // 0 spot, 1 point, 2 sun
                uint32_t lights[LIGHT_TYPES] = {};
                fb::Vec3 sun{};
                for (const fb::EnlightenLightInput* l = sys.m_lightsBegin; l && l < sys.m_lightsEnd; ++l)
                    if (l->m_type < LIGHT_TYPES)
                    {
                        ++lights[l->m_type];
                        if (l->m_type == LIGHT_SUN) sun = l->m_color;
                    }
                logger::info("[enlighten]  sys {} {}x{}: selected {}, lights point {} spot {} sun {} (color {:.3f} {:.3f} {:.3f}), chroma nonzero {} mean {:.1f}, luma nonzero {} max {} mean {:.1f}, direction written {}",
                    s, sys.m_entry->m_core->m_width, sys.m_entry->m_core->m_height, sys.m_selectCount, lights[1], lights[0], lights[2],
                    sun.m_x, sun.m_y, sun.m_z, nzC, meanC, nzL, mxL, meanL, dirSet);
            }
        }

        // render thread
        void dumpAtlases(fb::EnlightenRuntimeDatabase* db)
        {
            struct Item { const char* name; fb::DxTexture* tex; detail::Original* shipped; };
            detail::Original live[3];
            const Item items[] = {
                { "live_luma", db->m_lumaTexture, nullptr },
                { "live_chroma", db->m_chromaTexture, nullptr },
                { "live_direction", db->m_directionTexture, nullptr },
                { "shipped_luma", nullptr, &detail::g_orig[0] },
                { "shipped_chroma", nullptr, &detail::g_orig[1] },
                { "shipped_direction", nullptr, &detail::g_orig[2] },
            };
            int k = 0;
            for (const Item& it : items)
            {
                detail::Original& o = it.shipped ? *it.shipped : live[k++];
                if (!it.shipped) o.tex = it.tex;
                std::string err;
                if (!o.tex || !detail::readback(o, err))
                {
                    logger::info("[enlighten] dump {}: {}", it.name, o.tex ? err : "no texture");
                    continue;
                }
                const std::string path = directory() + "/" + it.name + ".dds";
                if (detail::writeAtlasDds(path, o.width, o.height, o.dxgi, o.bpp, o.pixels, err))
                    logger::info("[enlighten] dump {}: {}x{} dxgi {} -> {}", it.name, o.width, o.height, o.dxgi, path);
                else
                    logger::info("[enlighten] dump {}: {}", it.name, err);
            }
        }
    }

    void renderUI()
    {
        fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
        if (!r)
        {
            ImGui::TextDisabled("no Enlighten renderer");
            return;
        }
        const uint32_t state = r->m_state;
        ImGui::Text("renderer: %s", state == 0 ? "solving live" : state == 1 ? "static atlas" : "disabled");
        ImGui::Checkbox("engine solver", &engineSolver);
        ImGui::SameLine();
        ImGui::Checkbox("live probes", &liveProbes);
        bool rescale = ImGui::SliderFloat("calibration strength", &calibrationStrength, 0.0f, 1.0f, "%.2f");
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("0 = the raw solve, 1 = matched to the shipped atlas at the level's own lighting");
        rescale |= ImGui::SliderFloat("output scale", &outputScale, 0.1f, 4.0f, "x%.2f", ImGuiSliderFlags_Logarithmic);
        if (rescale) detail::runtimeRefresh(GAIN_REFRESH_UPDATES);
        if (liveProbes && g_lp.applied)
        {
            ImGui::SameLine();
            ImGui::TextDisabled("level x%.2f %.2f %.2f", g_lp.global.m_x, g_lp.global.m_y, g_lp.global.m_z);
        }
        const std::string generated = directory() + "/" + mapFileName() + "_generated.edb";
        std::error_code ec;
        if (fs::exists(generated, ec))
        {
            ImGui::SameLine();
            if (ImGui::SmallButton("apply generated live"))
                applyLive(generated);
        }
        if (!g_swapNote.empty()) ImGui::TextDisabled("%s", g_swapNote.c_str());
        int i = 0;
        for (auto** it = r->m_entities.begin(); it && it < r->m_entities.end() && i < 16; ++it, ++i)
        {
            fb::EnlightenRuntimeDatabase* db = *it ? (*it)->m_database : nullptr;
            if (!db) continue;
            ImGui::BulletText("database: %u systems, flags %u%s%s", db->m_systemCount, db->m_flags,
                (*it)->m_staticEntity ? ", static data attached" : "", g_swap.current == db ? " (swapped in)" : "");
            ImGui::SameLine();
            ImGui::PushID(i);
            if (ImGui::SmallButton("log systems")) logSystems(db, state);
            ImGui::SameLine();
            if (ImGui::SmallButton("dump atlases")) dumpAtlases(db);
            ImGui::PopID();
        }
        if (ImGui::TreeNode("level load##db"))
        {
            ImGui::Checkbox("use .edb overrides", &edbOverrides);
            ImGui::SameLine();
            ImGui::Checkbox("force dynamic on shipped systems", &forceDynamic);
            ImGui::TextDisabled("%s", directory().c_str());
            std::lock_guard<std::mutex> lock(g_mutex);
            for (const Loaded& l : g_loaded)
                ImGui::BulletText("%ux%u, %u instances, %u systems, flags %u%s%s", l.width, l.height, l.instances, l.systems, l.flags,
                    l.replaced ? (" <- " + l.file).c_str() : "", l.forcedDynamic ? " (forced dynamic)" : "");
            ImGui::TreePop();
        }
    }
#else
    namespace
    {
        // fb::Buffer over the generated stream: vtable {dtor, readEx, getAvailableBytes, skip, writeEx, setPosition,
        // getPosition, flush, getIdentifier, setIdentifier, getNativeHandle}; m_caps +8, m_bufferSize +0x10
        struct StreamBuffer
        {
            void** vtable;
            uint32_t _0x04 = 0;
            int32_t caps = 1;
            uint32_t _0x0C = 0;
            int64_t size = 0;
            void* handle = nullptr;
            uint32_t _0x1C = 0;
            const uint8_t* data = nullptr;
            size_t pos = 0;
        };
        void __fastcall sbDtor(StreamBuffer*, void*) {}
        int __fastcall sbRead(StreamBuffer* b, void*, void* dst, int64_t n)
        {
            if (n < 0 || b->pos + size_t(n) > size_t(b->size)) return 1;
            std::memcpy(dst, b->data + b->pos, size_t(n));
            b->pos += size_t(n);
            return 0;
        }
        int64_t __fastcall sbAvailable(StreamBuffer* b, void*) { return b->size - int64_t(b->pos); }
        void __fastcall sbSkip(StreamBuffer* b, void*, int64_t n) { b->pos = size_t((std::min)(b->size, int64_t(b->pos) + n)); }
        int __fastcall sbWrite(StreamBuffer*, void*, const void*, int64_t) { return 1; }
        void __fastcall sbSetPos(StreamBuffer* b, void*, int64_t p) { b->pos = size_t((std::clamp)(p, int64_t(0), b->size)); }
        int64_t __fastcall sbGetPos(StreamBuffer* b, void*) { return int64_t(b->pos); }
        void __fastcall sbFlush(StreamBuffer*, void*) {}
        const char* __fastcall sbId(StreamBuffer*, void*) { return "generated Enlighten system"; }
        void __fastcall sbSetId(StreamBuffer*, void*, const char*) {}
        void* __fastcall sbHandle(StreamBuffer*, void*) { return nullptr; }
        void* g_sbVtable[11] = { (void*)&sbDtor, (void*)&sbRead, (void*)&sbAvailable, (void*)&sbSkip, (void*)&sbWrite, (void*)&sbSetPos,
                                 (void*)&sbGetPos, (void*)&sbFlush, (void*)&sbId, (void*)&sbSetId, (void*)&sbHandle };

        struct Live
        {
            fb::EnlightenRendererEntity* entity = nullptr;
            fb::EnlightenStaticEntity* staticEntity = nullptr;
            fb::DxTexture* textures[3] = {};
            size_t systems = 0;
            std::string file;
        } g_live;
        std::string g_pendingApply;
        bool g_pendingRestore = false;
        // MP probe sets have no RadProbeSetCore +0x1C, SolveProbeTaskL1 0x1927380 reads it
        // held at m_LightProbeMaxUpdateSolveCount 0 while live
        uint32_t g_probeBudget = 0;
        bool g_probeClamped = false;

        void clampProbes(fb::EnlightenRuntimeSettings* rs, bool on)
        {
            if (!rs) return;
            if (on)
            {
                if (!g_probeClamped) { g_probeBudget = rs->m_LightProbeMaxUpdateSolveCount; g_probeClamped = true; }
                rs->m_LightProbeMaxUpdateSolveCount = 0;
            }
            else if (g_probeClamped)
            {
                rs->m_LightProbeMaxUpdateSolveCount = g_probeBudget;
                g_probeClamped = false;
            }
        }
        std::string g_liveNote;

        constexpr int ARENA_MAP_SHIFT = 16; // one arena map entry per 64 KiB
        constexpr int BF3_FILE_HEADER = 20; // 'EB3S', version, systems, atlas w, h
        constexpr uint32_t BF3_FORMAT_RG8 = 21; // TextureFormat
        constexpr uint32_t BF3_FORMAT_L16 = 11;
        constexpr uint32_t BF3_FORMAT_ARGB8 = 9;
        constexpr int NEUTRAL_CHROMA = 0x5555; // blue and red share 1/3 each
        constexpr int BF3_SYSTEM_SIZE = 0xF0; // EnlightenSystem ctor size
        constexpr int INJECT_KICK_UPDATES = 60;

        void* arenaOf(const void* p)
        {
            void* a = reinterpret_cast<void**>(OFF_g_arenaMap)[reinterpret_cast<uintptr_t>(p) >> ARENA_MAP_SHIFT];
            const intptr_t v = reinterpret_cast<intptr_t>(a);
            return v == 0 || v == -1 || v == -2 ? nullptr : a;
        }

        fb::EnlightenRendererEntity* levelEntity(fb::EnlightenRenderer* r)
        {
            if (g_live.entity) return g_live.entity;
            for (auto** it = r->m_entities.begin(); it && it < r->m_entities.end(); ++it)
                if (*it && (*it)->m_database && (*it)->m_staticEntity && (*it)->m_staticEntity->m_luma == detail::g_orig[0].tex) return *it;
            return nullptr;
        }

        // retail TextureCreateDesc (PDB layout minus the 220-byte name): format +0x10, width +0x14, height +0x18,
        // depth/mips/samples 1, bindFlags 8 (EnlightenDatabase::load sub_17B2E70)
        fb::DxTexture* createTexture(void* arena, uint32_t format, uint32_t w, uint32_t h)
        {
            uint32_t desc[18] = {};
            desc[1] = 1;
            desc[4] = format;
            desc[5] = w; desc[6] = h;
            desc[7] = 1; desc[8] = 1; desc[9] = 1;
            desc[11] = 8;
            void* renderer = *reinterpret_cast<void**>(OFF_g_renderer);
            using Fn = fb::DxTexture*(__thiscall*)(void*, void*, uint32_t*);
            fb::DxTexture* t = (*reinterpret_cast<Fn**>(renderer))[OFF_Renderer_createTexture_slot](renderer, arena, desc);
            return t;
        }

        void addRefTex(fb::DxTexture* t)
        {
            if (t) reinterpret_cast<void(__thiscall*)(void*)>((*reinterpret_cast<void***>(t))[1])(t);
        }

        void updateState(fb::EnlightenRenderer* r)
        {
            reinterpret_cast<void(__thiscall*)(fb::EnlightenRenderer*)>(OFF_EnlightenRenderer_updateState)(r);
        }

        void logSystems(fb::EnlightenRenderer* r)
        {
            fb::EnlightenRuntimeDatabase* db = g_live.entity ? g_live.entity->m_database : nullptr;
            if (!db) { logger::info("[enlighten] no generated systems live"); return; }
            const fb::EnlightenRuntimeSettings* rs = r->m_runtimeSettings;
            logger::info("[enlighten] renderer state {}, jobs {}, systems {}, enable {}, lightmaps {}", int(r->state), rs ? rs->m_JobCount : 0,
                db->m_systems.size(), rs ? rs->m_Enable : false, rs ? rs->m_LightMapsEnable : false);
            uint32_t never = 0, i = 0;
            double lumaAll = 0.0, inAll = 0.0;
            for (auto* p = db->m_systems.begin(); p && p < db->m_systems.end(); ++p, ++i)
            {
                const auto* sys = reinterpret_cast<const fb::EnlightenRuntimeSystem*>(p->m_set);
                if (!sys) continue;
                const uint32_t px = sys->m_width * sys->m_height;
                double luma = 0.0;
                uint32_t lit = 0, dirSet = 0;
                for (uint32_t k = 0; sys->m_lumaOut && k < px; ++k) { luma += sys->m_lumaOut[k]; lit += sys->m_lumaOut[k] != 0; }
                for (uint32_t k = 0; sys->m_directionOut && k < px; ++k) dirSet += sys->m_directionOut[k] != DIRECTION_UNSET;
                double in[3] = {};
                int32_t clusters = 0;
                if (sys->m_currentInput)
                {
                    clusters = *reinterpret_cast<const int32_t*>(reinterpret_cast<const uint8_t*>(sys->m_currentInput) + 0x10);
                    const float* v = sys->m_currentInput + 8;
                    for (int32_t c = 0; c < clusters && c < 1 << 20; ++c) for (int k = 0; k < 3; ++k) in[k] += v[4 * c + k];
                    for (double& x : in) x /= (std::max)(clusters, 1);
                }
                never += sys->m_updateCount == 0;
                lumaAll += luma / (std::max)(px, 1u);
                inAll += (in[0] + in[1] + in[2]) / 3.0;
                if (i < 24 || sys->m_updateCount == 0)
                    logger::info("[enlighten]  sys {} {}x{}: solved {} times, {:.2f} ms, {:.0f}% pixels, luma lit {} mean {:.1f}, direction set {}, input {} clusters mean {:.4f} {:.4f} {:.4f}",
                        i, sys->m_width, sys->m_height, sys->m_updateCount, sys->m_solveTime, sys->m_solvePerc * 100.0f, lit, luma / (std::max)(px, 1u),
                        dirSet, clusters, in[0], in[1], in[2]);
            }
            logger::info("[enlighten] {} systems never solved; mean output luma {:.1f}, mean cluster input {:.4f}", never, lumaAll / (std::max)(i, 1u), inAll / (std::max)(i, 1u));
        }

        bool inject(fb::EnlightenRenderer* r, const std::string& path)
        {
            std::ifstream f(path, std::ios::binary);
            std::vector<uint8_t> file((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            if (file.size() < BF3_FILE_HEADER || std::memcmp(file.data(), "EB3S", 4) != 0) { g_liveNote = "not a BF3 generated file: " + path; return false; }
            uint32_t hdr[5];
            std::memcpy(hdr, file.data(), sizeof(hdr));
            const uint32_t count = hdr[2], w = hdr[3], h = hdr[4];
            fb::EnlightenRendererEntity* e = levelEntity(r);
            if (!e) { g_liveNote = "no Enlighten entity holds this level's baked atlas"; return false; }
            fb::EnlightenRuntimeDatabase* db = e->m_database;
            if (db->m_outputAtlasWidth != w || db->m_outputAtlasHeight != h) { g_liveNote = fs::path(path).filename().string() + " belongs to another level"; return false; }
            if (db->m_dynamicDataEnable || !db->m_systems.empty()) { g_liveNote = "this database already carries systems (restore first, or load the level again)"; return false; }
            void* arena = arenaOf(db);
            if (!arena) { g_liveNote = "no engine arena for the database"; return false; }

            db->m_dynamicChroma = createTexture(arena, BF3_FORMAT_RG8, w, h);
            db->m_dynamicLuma = createTexture(arena, BF3_FORMAT_L16, w, h);
            db->m_dynamicDirection = createTexture(arena, BF3_FORMAT_ARGB8, w, h);
            if (!db->m_dynamicChroma || !db->m_dynamicLuma || !db->m_dynamicDirection) { g_liveNote = "output texture creation failed"; return false; }
            addRefTex(db->m_dynamicChroma); addRefTex(db->m_dynamicLuma); addRefTex(db->m_dynamicDirection);
            {
                const std::vector<uint16_t> zero(size_t(w) * h, 0), neutral(size_t(w) * h, NEUTRAL_CHROMA);
                const uint32_t box[6] = { 0, 0, 0, w, h, 1 };
                void* renderer = *reinterpret_cast<void**>(OFF_g_renderer);
                using UpdateFn = void(__thiscall*)(void*, void*, uint32_t, const uint32_t*, const void*, uint32_t, uint32_t);
                const UpdateFn update = (*reinterpret_cast<UpdateFn**>(renderer))[10];
                update(renderer, db->m_dynamicChroma, 0, box, neutral.data(), 2 * w, 2 * w * h);
                update(renderer, db->m_dynamicLuma, 0, box, zero.data(), 2 * w, 2 * w * h);
            }

            reinterpret_cast<void(__thiscall*)(void*, uint32_t)>(OFF_EnlightenDatabase_resizeSystems)(&db->m_systems, count);
            void* none = nullptr;
            reinterpret_cast<void(__thiscall*)(void*, uint32_t, void**)>(OFF_EnlightenDatabase_appendInputBuffers)(&db->m_inputLightingBuffers, count, &none);
            if (db->m_systems.size() != count || db->m_inputLightingBuffers.size() != count)
            {
                g_liveNote = "database vectors did not grow";
                return false;
            }
            using AllocFn = void*(__thiscall*)(void*, uint32_t, uint32_t);
            using CtorFn = void*(__thiscall*)(void*, void*);
            using LoadFn = void(__thiscall*)(void*, void*, void*, void*);
            size_t at = BF3_FILE_HEADER;
            for (uint32_t i = 0; i < count; ++i)
            {
                uint32_t len = 0;
                std::memcpy(&len, file.data() + at, 4);
                at += 4;
                auto* sys = static_cast<uint8_t*>(reinterpret_cast<AllocFn>(OFF_MemoryArena_alloc)(arena, BF3_SYSTEM_SIZE, 16));
                reinterpret_cast<CtorFn>(OFF_EnlightenSystem_ctor)(sys, arena);
                StreamBuffer buf;
                buf.vtable = g_sbVtable;
                buf.size = int64_t(len);
                buf.data = file.data() + at;
                reinterpret_cast<LoadFn>(OFF_EnlightenSystem_load)(sys, arena, &buf, nullptr);
                at += len;
                if (buf.pos != len) logger::info("[enlighten] BF3 system {}: load read {} of {} bytes", i, buf.pos, len);
                db->m_systems[i].m_set = reinterpret_cast<fb::EnlightenProbeSetRuntime*>(sys); // ResourceProxy {ptr, handle}
                db->m_systems[i]._0x04 = nullptr;
                auto* f32 = reinterpret_cast<uint32_t*>(sys);
                f32[54] = reinterpret_cast<uint32_t>(db->m_inputLightingBuffers.begin());
                f32[55] = count;
                f32[56] = reinterpret_cast<uint32_t>(db->m_dynamicChroma);
                f32[57] = reinterpret_cast<uint32_t>(db->m_dynamicLuma);
                f32[58] = reinterpret_cast<uint32_t>(db->m_dynamicDirection);
                f32[14] = i;
                db->m_inputLightingBuffers[i] = reinterpret_cast<void*>(f32[34]); // m_currentInputLightingBuffer
            }
            for (auto* p = db->m_probeSets.begin(); p && p < db->m_probeSets.end(); ++p)
                if (p->m_set)
                {
                    auto* ps = reinterpret_cast<uint32_t*>(p->m_set);
                    ps[12] = reinterpret_cast<uint32_t>(db->m_inputLightingBuffers.begin());
                    ps[13] = count;
                }
            clampProbes(r->m_runtimeSettings, true);
            db->m_dynamicDataEnable = true;

            // DynamicEnlightenEntity::removeStatic: the entity binds the database's outputs
            g_live.entity = e;
            g_live.staticEntity = e->m_staticEntity;
            for (int k = 0; k < 3; ++k) g_live.textures[k] = e->m_textures[k];
            e->m_staticEntity = nullptr;
            e->m_textures[0] = db->m_dynamicChroma;
            e->m_textures[1] = db->m_dynamicLuma;
            e->m_textures[2] = db->m_dynamicDirection;
            updateState(r);
            g_live.systems = count;
            g_live.file = fs::path(path).filename().string();
            g_liveNote = "applied " + g_live.file + " live (" + std::to_string(count) + " systems), renderer state " + std::to_string(int(r->state));
            logger::info("[enlighten] {}", g_liveNote);
            detail::runtimeKick(INJECT_KICK_UPDATES);
            return true;
        }

        void restore(fb::EnlightenRenderer* r)
        {
            if (!g_live.entity) return;
            fb::EnlightenRendererEntity* e = g_live.entity;
            e->m_database->m_dynamicDataEnable = false;
            e->m_staticEntity = g_live.staticEntity;
            for (int k = 0; k < 3; ++k) e->m_textures[k] = g_live.textures[k];
            updateState(r);
            clampProbes(r->m_runtimeSettings, false);
            g_liveNote = "shipped atlas back (the generated systems stay loaded until the level unloads)";
            logger::info("[enlighten] {}", g_liveNote);
            g_live = Live{};
        }
    }

    void applyLive(const std::string& path) { g_pendingApply = path; g_liveNote = "applying..."; }
    fb::EnlightenRuntimeDatabase* liveDatabase() { return g_live.entity ? g_live.entity->m_database : nullptr; }
    void clampRuntime(fb::EnlightenRuntimeSettings* rs)
    {
        if (g_live.entity && rs) rs->m_LightProbeMaxUpdateSolveCount = 0;
    }

    void tickGameThread()
    {
        fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
        if (!r) return;
        if (g_live.entity) clampProbes(r->m_runtimeSettings, true);
        if (g_pendingRestore) { g_pendingRestore = false; restore(r); }
        if (!g_pendingApply.empty())
        {
            const std::string path = g_pendingApply;
            g_pendingApply.clear();
            if (g_live.entity) { g_liveNote = "already live: restore first"; return; }
            if (inject(r, path)) engineSolver = true;
            else logger::info("[enlighten] BF3 apply failed: {}", g_liveNote);
        }
    }
    namespace
    {
        bool g_bf3ProbesScaled = false;

        // pairing copies [first, first + count) via sub_179A830
        // sets +0x38, k null = shipped
        void writeLevelProbes(const float* k)
        {
            fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
            for (auto** it = r ? r->m_entities.begin() : nullptr; it && it < r->m_entities.end(); ++it)
            {
                fb::EnlightenRendererEntity* e = *it;
                if (!e || !e->m_database || !e->m_staticEntity || !e->m_staticEntity->m_database) continue;
                const fb::EnlightenStaticDatabase* sdb = e->m_staticEntity->m_database;
                const size_t total = size_t(sdb->m_probesEnd - sdb->m_probesBegin) / PROBE_FLOATS;
                auto& sets = e->m_database->m_probeSets;
                for (auto* p = sets.begin(); p && p < sets.end(); ++p)
                {
                    fb::EnlightenProbeSetRuntime* set = p->m_set;
                    if (!set || !set->m_data || !set->m_probes) continue;
                    const uint32_t first = set->m_data->m_firstStaticProbe, count = set->m_data->m_probeCount;
                    if (size_t(first) + count > total) continue;
                    for (uint32_t i = 0; i < count; ++i)
                    {
                        const float* src = sdb->m_probesBegin + size_t(first + i) * PROBE_FLOATS;
                        float* dst = set->m_probes + size_t(i) * PROBE_FLOATS;
                        for (int c = 0; c < PROBE_FLOATS; ++c) dst[c] = k && c < PROBE_RGB_FLOATS ? src[c] * k[c / PROBE_ROW_FLOATS] : src[c]; // rows R, G, B, occlusion
                    }
                    set->m_probesUpdated = true;
                }
            }
            forceProbeUpdate(PROBE_FORCE_UPDATES);
        }
    }

    void scaleLevelProbes(float r, float g, float b)
    {
        const float k[3] = { r, g, b };
        writeLevelProbes(k);
        g_bf3ProbesScaled = true;
    }

    void restoreLevelProbes()
    {
        if (!g_bf3ProbesScaled) return;
        writeLevelProbes(nullptr);
        g_bf3ProbesScaled = false;
    }

    // calibration: copyIrradianceTexturesToGpu hook, <map>_gain.bin as BF4
    namespace
    {
        struct GainRange
        {
            const void* sys;
            uint32_t ax, ay, w, h;
            std::vector<float> gain, add, tint, shipped;
            double raw0Sum = 0.0;
            std::atomic<float> floorScale = 1.0f;
            std::atomic<double> rawSum = 0.0;
            GainRange() = default;
            GainRange(GainRange&& o) noexcept : sys(o.sys), ax(o.ax), ay(o.ay), w(o.w), h(o.h), gain(std::move(o.gain)), add(std::move(o.add)),
                tint(std::move(o.tint)), shipped(std::move(o.shipped)), raw0Sum(o.raw0Sum), floorScale(o.floorScale.load()), rawSum(o.rawSum.load()) {}
            GainRange& operator=(GainRange&& o) noexcept
            {
                sys = o.sys; ax = o.ax; ay = o.ay; w = o.w; h = o.h; gain = std::move(o.gain); add = std::move(o.add); tint = std::move(o.tint);
                shipped = std::move(o.shipped); raw0Sum = o.raw0Sum; floorScale = o.floorScale.load(); rawSum = o.rawSum.load();
                return *this;
            }
        };
        constexpr float MAX_TINT = 2.0f; // per-channel tint clamp 1/x..x
        constexpr float MAX_GAIN = 16.0f, MIN_GAIN = 1.0f / 256.0f; // per-texel gain clamp
        std::atomic<const std::vector<GainRange>*> g_gainRanges = nullptr;
        std::vector<std::unique_ptr<std::vector<GainRange>>> g_gainTables;
        const void* g_gainsFor = nullptr;

        const GainRange* rangeOf(const std::vector<GainRange>& t, const void* sys)
        {
            auto it = std::lower_bound(t.begin(), t.end(), sys, [](const GainRange& r, const void* s) { return r.sys < s; });
            return it != t.end() && it->sys == sys ? &*it : nullptr;
        }

        // WriteIrradiance format 3 (0x1896690): luma = (r+g+b) * 65535.5, chroma = blue share * 255.5 | red share * 255.5 << 8
        constexpr float LUMA_SCALE = 65535.5f, SHARE_SCALE = 255.5f;
        constexpr float ENCODE_MIN_SUM = 1e-6f;
        void decode(uint16_t L, uint16_t C, float rgb[3])
        {
            const float sum = L / LUMA_SCALE, red = float(C >> 8) / SHARE_SCALE, blue = float(C & 0xFF) / SHARE_SCALE;
            rgb[0] = sum * red; rgb[2] = sum * blue; rgb[1] = (std::max)(0.0f, sum - rgb[0] - rgb[2]);
        }
        void encode(const float rgb[3], uint16_t& L, uint16_t& C)
        {
            const float sum = (std::max)(0.0f, rgb[0]) + (std::max)(0.0f, rgb[1]) + (std::max)(0.0f, rgb[2]);
            L = uint16_t((std::min)(65535.0f, sum * LUMA_SCALE));
            if (sum <= ENCODE_MIN_SUM) { C = 0; return; }
            const int red = (std::clamp)(int((std::max)(0.0f, rgb[0]) / sum * SHARE_SCALE), 0, 255), blue = (std::clamp)(int((std::max)(0.0f, rgb[2]) / sum * SHARE_SCALE), 0, 255);
            C = uint16_t(blue | red << 8);
        }

        void shippedRgbAt(const detail::Original& sl, const detail::Original& sc, int bx, int by, size_t a, float rgb[3])
        {
            const float ship = reinterpret_cast<const uint16_t*>(sl.pixels.data())[a] / 65535.0f;
            const uint8_t* c = sc.pixels.data() + a * sc.bpp;
            const float red = c[by] / 255.0f, blue = c[bx] / 255.0f;
            rgb[0] = ship * red; rgb[1] = ship * (std::max)(0.0f, 1.0f - red - blue); rgb[2] = ship * blue;
        }

        template <class Fn> void forEachLiveSystem(Fn&& fn)
        {
            fb::EnlightenRuntimeDatabase* db = g_live.entity ? g_live.entity->m_database : nullptr;
            for (auto* p = db ? db->m_systems.begin() : nullptr; p && p < db->m_systems.end(); ++p)
                if (const auto* sys = reinterpret_cast<const fb::EnlightenRuntimeSystem*>(p->m_set))
                    if (sys->m_lumaOut && sys->m_chromaOut && sys->m_width && sys->m_height) fn(sys);
        }
    }

    std::string gainPath() { return directory() + "/" + mapFileName() + "_gain.bin"; }

    void reloadGains()
    {
        g_gainRanges.store(nullptr, std::memory_order_release);
        if (!g_live.entity) return;
        std::vector<float> atlas, atlasAdd, atlasRaw0, atlasTint;
        uint32_t W = 0, H = 0;
        {
            std::ifstream f(gainPath(), std::ios::binary);
            if (f && f.read(reinterpret_cast<char*>(&W), 4) && f.read(reinterpret_cast<char*>(&H), 4) && W && H && W <= MAX_ATLAS_SIZE && H <= MAX_ATLAS_SIZE)
            {
                atlas.resize(size_t(W) * H);
                if (!f.read(reinterpret_cast<char*>(atlas.data()), std::streamsize(atlas.size() * 4))) atlas.clear();
                atlasAdd.resize(size_t(W) * H * 3);
                if (atlas.empty() || !f.read(reinterpret_cast<char*>(atlasAdd.data()), std::streamsize(atlasAdd.size() * 4))) atlasAdd.clear();
                atlasRaw0.resize(size_t(W) * H);
                if (atlasAdd.empty() || !f.read(reinterpret_cast<char*>(atlasRaw0.data()), std::streamsize(atlasRaw0.size() * 4))) atlasRaw0.clear();
                atlasTint.resize(size_t(W) * H * 3);
                if (atlasRaw0.empty() || !f.read(reinterpret_cast<char*>(atlasTint.data()), std::streamsize(atlasTint.size() * 4))) atlasTint.clear();
            }
        }
        std::string err;
        detail::Original& sl = detail::g_orig[0];
        detail::Original& sc = detail::g_orig[1];
        const bool shippedOk = detail::readback(sl, err) && detail::readback(sc, err) && sl.bpp == 2 && sc.width == sl.width && sc.height == sl.height;
        int sbx = 0, sby = 1;
        if (shippedOk) detail::chromaBytes(sc.dxgi, sbx, sby);
        auto table = std::make_unique<std::vector<GainRange>>();
        forEachLiveSystem([&](const fb::EnlightenRuntimeSystem* sys)
        {
            const uint32_t w = sys->m_width, h = sys->m_height;
            const size_t px = size_t(w) * h;
            GainRange r;
            r.sys = sys; r.ax = sys->m_atlasX; r.ay = sys->m_atlasY; r.w = w; r.h = h;
            r.gain.assign(px, 1.0f); r.add.assign(px * 3, 0.0f); r.tint.assign(px * 3, 1.0f);
            if (shippedOk)
            {
                r.shipped.assign(px * 3, 0.0f);
                for (uint32_t y = 0; y < h; ++y)
                    for (uint32_t x = 0; x < w; ++x)
                        if (r.ax + x < sl.width && r.ay + y < sl.height)
                            shippedRgbAt(sl, sc, sbx, sby, size_t(r.ay + y) * sl.width + (r.ax + x), &r.shipped[(size_t(y) * w + x) * 3]);
            }
            for (uint32_t y = 0; y < h && !atlas.empty(); ++y)
                for (uint32_t x = 0; x < w; ++x)
                {
                    if (r.ax + x >= W || r.ay + y >= H) continue;
                    const size_t a = size_t(r.ay + y) * W + (r.ax + x), l = size_t(y) * w + x;
                    r.gain[l] = atlas[a];
                    if (!atlasAdd.empty()) for (int c = 0; c < 3; ++c) r.add[l * 3 + c] = atlasAdd[a * 3 + c];
                    if (!atlasRaw0.empty()) r.raw0Sum += atlasRaw0[a];
                    if (!atlasTint.empty()) for (int c = 0; c < 3; ++c) r.tint[l * 3 + c] = atlasTint[a * 3 + c];
                }
            table->push_back(std::move(r));
        });
        std::sort(table->begin(), table->end(), [](const GainRange& a, const GainRange& b) { return a.sys < b.sys; });
        if (!atlas.empty()) logger::info("[enlighten] calibration gains on {} systems", table->size());
        g_gainRanges.store(table.get(), std::memory_order_release);
        g_gainTables.push_back(std::move(table));
        detail::runtimeRefresh(GAIN_REFRESH_UPDATES);
    }

    bool uploadSystemOutput(void* system)
    {
        if (!g_live.entity) return false;
        const std::vector<GainRange>* ranges = g_gainRanges.load(std::memory_order_acquire);
        const GainRange* r = ranges ? rangeOf(*ranges, system) : nullptr;
        const auto* sys = static_cast<const fb::EnlightenRuntimeSystem*>(system);
        const uint32_t w = sys->m_width, h = sys->m_height;
        if (!sys->m_lumaOut || !sys->m_chromaOut || !w || !h) return false;
        const size_t px = size_t(w) * h;
        thread_local std::vector<uint16_t> luma, chroma;
        luma.resize(px); chroma.resize(px);
        const float s = calibrationStrength, o = outputScale, f = r ? r->floorScale.load() * s : 0.0f;
        const bool lookOn = !look::isDefault();
        double rawSum = 0.0;
        for (size_t i = 0; i < px; ++i)
        {
            float rgb[3];
            decode(sys->m_lumaOut[i], sys->m_chromaOut[i], rgb);
            rawSum += rgb[0] + rgb[1] + rgb[2];
            if (r)
            {
                const float g = 1.0f + (r->gain[i] - 1.0f) * s;
                for (int c = 0; c < 3; ++c)
                    rgb[c] = rgb[c] * g * (1.0f + (r->tint[i * 3 + c] - 1.0f) * s) + r->add[i * 3 + c] * f;
            }
            for (float& c : rgb) c *= o;
            if (lookOn) look::apply(rgb, r && !r->shipped.empty() ? &r->shipped[i * 3] : nullptr);
            encode(rgb, luma[i], chroma[i]);
        }
        for (uint32_t y = 0; y < h; ++y)
            for (uint32_t x = 0; x < w; ++x)
            {
                const size_t i = size_t(y) * w + x;
                if (luma[i]) continue;
                uint16_t c = NEUTRAL_CHROMA;
                if (x > 0 && luma[i - 1]) c = chroma[i - 1];
                else if (x + 1 < w && luma[i + 1]) c = chroma[i + 1];
                else if (y > 0 && luma[i - w]) c = chroma[i - w];
                else if (y + 1 < h && luma[i + w]) c = chroma[i + w];
                chroma[i] = c;
            }
        if (r) const_cast<GainRange*>(r)->rawSum = rawSum;
        // g_renderer->updateSubresource (vtable slot 10), box {left, top, front, right, bottom, back} as sub_179B130
        const uint32_t box[6] = { sys->m_atlasX, sys->m_atlasY, 0, sys->m_atlasX + w, sys->m_atlasY + h, 1 };
        void* renderer = *reinterpret_cast<void**>(OFF_g_renderer);
        using UpdateFn = void(__thiscall*)(void*, void*, uint32_t, const uint32_t*, const void*, uint32_t, uint32_t);
        const UpdateFn update = (*reinterpret_cast<UpdateFn**>(renderer))[10];
        const auto* tex = reinterpret_cast<void* const*>(reinterpret_cast<const uint8_t*>(sys) + 0xE0); // chroma, luma, direction
        update(renderer, tex[0], 0, box, chroma.data(), 2 * w, uint32_t(2 * px));
        update(renderer, tex[1], 0, box, luma.data(), 2 * w, uint32_t(2 * px));
        update(renderer, tex[2], 0, box, sys->m_directionOut, 4 * w, uint32_t(4 * px));
        return true;
    }

    bool gainAt(uint32_t x, uint32_t y, TexelGain& out)
    {
        const std::vector<GainRange>* ranges = g_gainRanges.load(std::memory_order_acquire);
        if (!ranges) return false;
        for (const GainRange& r : *ranges)
        {
            if (x < r.ax || y < r.ay || x >= r.ax + r.w || y >= r.ay + r.h) continue;
            const size_t l = size_t(y - r.ay) * r.w + (x - r.ax);
            out.gain = r.gain[l];
            out.floorScale = r.floorScale;
            for (int c = 0; c < 3; ++c) { out.tint[c] = r.tint[l * 3 + c]; out.add[c] = r.add[l * 3 + c]; }
            const auto* sys = static_cast<const fb::EnlightenRuntimeSystem*>(r.sys);
            decode(sys->m_lumaOut[l], sys->m_chromaOut[l], out.raw);
            return true;
        }
        return false;
    }

    void scaleOutput(float*, const unsigned short*) {}

    namespace
    {
        struct Measure
        {
            int phase = 0; // 0 idle, 1 waiting for the solver, 2 blending the restored states, 3 re-solving
            bool overrides = true, force = false;
            uint32_t since = 0;
            std::vector<uint32_t> counts;
            std::string note;
        } g_measure;

        void resumeOverrides()
        {
            detail::g_measuringReference = false;
            editor::overridesEnabled = g_measure.overrides;
            g_measure.phase = 0;
            g_measure.force = false;
        }

        void solveCounts(std::vector<uint32_t>& out)
        {
            out.clear();
            forEachLiveSystem([&](const fb::EnlightenRuntimeSystem* sys) { out.push_back(sys->m_updateCount); });
        }
    }

    namespace
    {
        struct LiveProbes
        {
            bool loaded = false, failed = false, applied = false;
            std::vector<std::vector<std::vector<uint32_t>>> texels; // set -> probe -> atlas texels
            int frame = 0;
        } g_lp;

        // pristine = the static database's probes [first, first + count) of each set (sub_179A830)
        void writeLiveProbe(const fb::EnlightenStaticDatabase* sdb, fb::EnlightenProbeSetRuntime* set, uint32_t pi, const float* k)
        {
            const float* src = sdb->m_probesBegin + size_t(set->m_data->m_firstStaticProbe + pi) * PROBE_FLOATS;
            float* dst = set->m_probes + size_t(pi) * PROBE_FLOATS;
            for (int c = 0; c < PROBE_FLOATS; ++c) dst[c] = k && c < PROBE_RGB_FLOATS ? src[c] * k[c / PROBE_ROW_FLOATS] : src[c]; // rows R, G, B, occlusion
        }

        bool probeSetOk(const fb::EnlightenStaticDatabase* sdb, const fb::EnlightenProbeSetRuntime* set)
        {
            if (!set || !set->m_data || !set->m_probes || !sdb) return false;
            const size_t total = size_t(sdb->m_probesEnd - sdb->m_probesBegin) / PROBE_FLOATS;
            return size_t(set->m_data->m_firstStaticProbe) + set->m_data->m_probeCount <= total;
        }

        void restoreLiveProbes()
        {
            if (g_lp.applied && g_live.entity && g_live.staticEntity)
            {
                const fb::EnlightenStaticDatabase* sdb = g_live.staticEntity->m_database;
                auto& sets = g_live.entity->m_database->m_probeSets;
                for (auto* p = sets.begin(); p && p < sets.end(); ++p)
                    if (probeSetOk(sdb, p->m_set))
                    {
                        for (uint32_t pi = 0; pi < p->m_set->m_data->m_probeCount; ++pi) writeLiveProbe(sdb, p->m_set, pi, nullptr);
                        p->m_set->m_probesUpdated = true;
                    }
                forceProbeUpdate(PROBE_FORCE_UPDATES);
            }
            g_lp = LiveProbes{};
        }

        bool loadProbeTexels(fb::EnlightenRuntimeDatabase* ldb)
        {
            std::ifstream f(directory() + "/" + mapFileName() + "_probes.bin", std::ios::binary);
            if (!f) return false;
            std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            size_t at = 0;
            const auto u32 = [&](uint32_t& v) { if (at + 4 > d.size()) return false; std::memcpy(&v, &d[at], 4); at += 4; return true; };
            uint32_t magic = 0, version = 0, sets = 0;
            if (!u32(magic) || magic != PROBES_MAGIC || !u32(version) || version != PROBES_VERSION || !u32(sets)) return false;
            if (sets != uint32_t(ldb->m_probeSets.size())) return false;
            g_lp.texels.assign(sets, {});
            for (uint32_t si = 0; si < sets; ++si)
            {
                uint32_t count = 0;
                if (!u32(count)) return false;
                const fb::EnlightenProbeSetRuntime* set = ldb->m_probeSets[si].m_set;
                if (count && (!set || !set->m_data || set->m_data->m_probeCount != count)) return false;
                g_lp.texels[si].resize(count);
                for (uint32_t pi = 0; pi < count; ++pi)
                {
                    if (at + 2 > d.size()) return false;
                    const uint16_t hc = uint16_t(d[at] | (d[at + 1] << 8));
                    at += 2;
                    auto& t = g_lp.texels[si][pi];
                    t.resize(hc);
                    for (uint16_t k = 0; k < hc; ++k) if (!u32(t[k])) return false;
                }
            }
            return true;
        }

        void decodeAtlas(const detail::Original& luma, const detail::Original& chroma, int bx, int by, std::vector<fb::Vec3>& out)
        {
            const size_t n = size_t(luma.width) * luma.height;
            out.assign(n, fb::Vec3{});
            const uint16_t* L = reinterpret_cast<const uint16_t*>(luma.pixels.data());
            for (size_t i = 0; i < n; ++i)
            {
                const float l = L[i] / 65535.0f;
                const uint8_t* c = chroma.pixels.data() + i * chroma.bpp;
                const float red = c[by] / 255.0f, blue = c[bx] / 255.0f;
                out[i] = fb::vec3(l * red, l * (std::max)(0.0f, 1.0f - red - blue), l * blue);
            }
        }

        void updateLiveProbes()
        {
            fb::EnlightenRuntimeDatabase* ldb = g_live.entity->m_database;
            if (!g_lp.loaded && !g_lp.failed)
            {
                g_lp.loaded = loadProbeTexels(ldb);
                g_lp.failed = !g_lp.loaded;
                logger::info("[enlighten] live probes: {}", g_lp.loaded ? "texel lists loaded" : "no matching <map>_probes.bin (regenerate)");
            }
            if (!g_lp.loaded || !g_live.staticEntity || (g_lp.frame++ % LIVE_PROBE_INTERVAL) != 0) return;
            std::string err;
            detail::Original& sl = detail::g_orig[0];
            detail::Original& sc = detail::g_orig[1];
            detail::Original ll, lc;
            ll.tex = ldb->m_dynamicLuma;
            lc.tex = ldb->m_dynamicChroma;
            static std::string lastFail;
            const auto fail = [&](const std::string& why) { if (why != lastFail) logger::info("[enlighten] live probes: {}", why); lastFail = why; };
            if (!detail::readback(sl, err) || !detail::readback(sc, err)) return fail("shipped atlas readback: " + err);
            if (!detail::readback(ll, err) || !detail::readback(lc, err)) return fail("live output readback: " + err);
            if (ll.width != sl.width || ll.height != sl.height || ll.bpp != 2 || sl.bpp != 2 || lc.bpp < 2)
                return fail(std::format("live {}x{} bpp {}/{} vs shipped {}x{} bpp {}", ll.width, ll.height, ll.bpp, lc.bpp, sl.width, sl.height, sl.bpp));
            lastFail.clear();
            int sbx, sby;
            detail::chromaBytes(sc.dxgi, sbx, sby);
            static std::vector<fb::Vec3> liveRgb, shippedRgb;
            decodeAtlas(ll, lc, 0, 1, liveRgb); // low byte blue, high byte red
            decodeAtlas(sl, sc, sbx, sby, shippedRgb);
            fb::Vec3 gl{}, gs{};
            size_t lit = 0;
            for (size_t i = 0; i < liveRgb.size(); ++i)
            {
                gl += liveRgb[i];
                gs += shippedRgb[i];
                lit += shippedRgb[i].m_x + shippedRgb[i].m_y + shippedRgb[i].m_z > 0.0f;
            }
            const fb::Vec3 floorPerTexel = gs * (LIVE_PROBE_FLOOR / float((std::max)(lit, size_t(1))));
            const auto ratio = [&](const fb::Vec3& a, const fb::Vec3& b, float texels)
            {
                const auto r = [&](float x, float y, float fl)
                {
                    const float dd = (std::max)(y, fl * texels);
                    return dd > RATIO_EPS ? (std::clamp)(x / dd, 0.0f, MAX_PROBE_RATIO) : 1.0f;
                };
                return fb::vec3(r(a.m_x, b.m_x, floorPerTexel.m_x), r(a.m_y, b.m_y, floorPerTexel.m_y), r(a.m_z, b.m_z, floorPerTexel.m_z));
            };
            const fb::Vec3 global = ratio(gl, gs, 0.0f);
            const fb::EnlightenStaticDatabase* sdb = g_live.staticEntity->m_database;
            uint32_t si = 0;
            for (auto* p = ldb->m_probeSets.begin(); p && p < ldb->m_probeSets.end() && si < g_lp.texels.size(); ++p, ++si)
            {
                if (!probeSetOk(sdb, p->m_set)) continue;
                const uint32_t count = p->m_set->m_data->m_probeCount;
                for (uint32_t pi = 0; pi < count && pi < g_lp.texels[si].size(); ++pi)
                {
                    const auto& t = g_lp.texels[si][pi];
                    fb::Vec3 a{}, b{};
                    for (uint32_t x : t) if (x < liveRgb.size()) { a += liveRgb[x]; b += shippedRgb[x]; }
                    const fb::Vec3 k = t.size() >= MIN_PROBE_TEXELS ? ratio(a, b, float(t.size())) : global;
                    const float kk[3] = { k.m_x, k.m_y, k.m_z };
                    writeLiveProbe(sdb, p->m_set, pi, kk);
                }
                p->m_set->m_probesUpdated = true;
            }
            if (!g_lp.applied)
                logger::info("[enlighten] live probes: {} sets follow the lightmap (level ratio {:.2f} {:.2f} {:.2f})", si, global.m_x, global.m_y, global.m_z);
            g_lp.applied = true;
            forceProbeUpdate(PROBE_FORCE_UPDATES);
        }
    }

    bool calibrate(std::string& note)
    {
        if (!g_live.entity) { note = "apply the generated systems first"; return false; }
        std::string err;
        detail::Original& sl = detail::g_orig[0];
        detail::Original& sc = detail::g_orig[1];
        if (!detail::readback(sl, err) || !detail::readback(sc, err) || sl.bpp != 2) { note = "shipped atlas readback failed: " + err; return false; }
        int bx, by;
        detail::chromaBytes(sc.dxgi, bx, by);
        const uint32_t W = sl.width, H = sl.height;
        std::vector<float> gain(size_t(W) * H, 1.0f), add(size_t(W) * H * 3, 0.0f), raw0(size_t(W) * H, 0.0f), tint(size_t(W) * H * 3, 1.0f);
        const uint16_t* S = reinterpret_cast<const uint16_t*>(sl.pixels.data());
        size_t measured = 0, floored = 0, raised = 0, lowered = 0;
        double logSum = 0.0;
        forEachLiveSystem([&](const fb::EnlightenRuntimeSystem* sys)
        {
            const uint32_t w = sys->m_width, h = sys->m_height, ax = sys->m_atlasX, ay = sys->m_atlasY;
            std::vector<float> raw(size_t(w) * h * 3);
            for (size_t i = 0; i < size_t(w) * h; ++i) decode(sys->m_lumaOut[i], sys->m_chromaOut[i], &raw[i * 3]);
            for (uint32_t y = 0; y < h; ++y)
                for (uint32_t x = 0; x < w; ++x)
                {
                    if (ax + x >= W || ay + y >= H) continue;
                    const size_t a = size_t(ay + y) * W + (ax + x), l = size_t(y) * w + x;
                    const float* rp = &raw[l * 3];
                    const float rawSum = rp[0] + rp[1] + rp[2];
                    raw0[a] = rawSum;
                    if (!S[a]) continue;
                    const float ship = S[a] / 65535.0f;
                    double sN = 0.0, rN = 0.0, sC[3] = {}, rC[3] = {};
                    for (int dy = -GAIN_RADIUS; dy <= GAIN_RADIUS; ++dy)
                        for (int dx = -GAIN_RADIUS; dx <= GAIN_RADIUS; ++dx)
                        {
                            const int nx = int(x) + dx, ny = int(y) + dy;
                            if (nx < 0 || ny < 0 || nx >= int(w) || ny >= int(h) || ax + nx >= W || ay + ny >= H) continue;
                            const size_t na = size_t(ay + ny) * W + (ax + nx), nl = size_t(ny) * w + nx;
                            if (!S[na]) continue;
                            sN += S[na] / 65535.0;
                            rN += double(raw[nl * 3]) + raw[nl * 3 + 1] + raw[nl * 3 + 2];
                            float srgb[3];
                            shippedRgbAt(sl, sc, bx, by, na, srgb);
                            for (int c = 0; c < 3; ++c) { sC[c] += srgb[c]; rC[c] += raw[nl * 3 + c]; }
                        }
                    const float g = (std::clamp)(float((sN + GAIN_EPS) / (rN + GAIN_EPS)), MIN_GAIN, MAX_GAIN);
                    gain[a] = g;
                    if (rawSum > TINT_MIN_RAW && sN > TINT_MIN_SHIPPED && rN > TINT_MIN_RAW_N)
                    {
                        float t[3], lumaAfter = 0.0f;
                        for (int c = 0; c < 3; ++c)
                        {
                            t[c] = (std::clamp)(float((sC[c] / sN + TINT_EPS) / (rC[c] / rN + TINT_EPS)), 1.0f / MAX_TINT, MAX_TINT);
                            lumaAfter += rp[c] * t[c];
                        }
                        const float norm = lumaAfter > TINT_NORM_EPS ? rawSum / lumaAfter : 1.0f;
                        for (int c = 0; c < 3; ++c) tint[a * 3 + c] = t[c] * norm;
                    }
                    ++measured;
                    raised += g > RAISED_GAIN;
                    lowered += g < LOWERED_GAIN;
                    logSum += std::log(g);
                    if (rawSum * g < ship * FLOOR_BELOW)
                    {
                        float rgb[3];
                        shippedRgbAt(sl, sc, bx, by, a, rgb);
                        const float k = 1.0f - rawSum * g / ship;
                        for (int c = 0; c < 3; ++c) add[a * 3 + c] = rgb[c] * k;
                        ++floored;
                    }
                }
        });
        {
            std::ofstream f(gainPath(), std::ios::binary);
            f.write(reinterpret_cast<const char*>(&W), 4);
            f.write(reinterpret_cast<const char*>(&H), 4);
            f.write(reinterpret_cast<const char*>(gain.data()), std::streamsize(gain.size() * 4));
            f.write(reinterpret_cast<const char*>(add.data()), std::streamsize(add.size() * 4));
            f.write(reinterpret_cast<const char*>(raw0.data()), std::streamsize(raw0.size() * 4));
            f.write(reinterpret_cast<const char*>(tint.data()), std::streamsize(tint.size() * 4));
        }
        reloadGains();
        const double geo = measured ? std::exp(logSum / double(measured)) : 1.0;
        char buf[256];
        std::snprintf(buf, sizeof(buf), "%zu texels: %zu raised, %zu lowered, mean x%.2f, %zu floored with the shipped color", measured, raised, lowered, geo, floored);
        note = buf;
        logger::info("[enlighten] calibration: {} -> {}", note, gainPath());
        return true;
    }

    void requestCalibration()
    {
        if (g_measure.phase) return;
        g_measure.phase = 1;
        g_measure.note = "calibration queued";
    }
    void measureNow() { if (g_measure.phase) g_measure.force = true; }
    bool calibrationPending() { return g_measure.phase != 0; }
    const std::string& calibrationNote() { return g_measure.note; }

    void tickRender()
    {
        if (!g_live.entity)
        {
            if (g_measure.phase >= 2) resumeOverrides();
            if (g_measure.phase == 1) g_measure.note = "calibration waits for the generated systems";
            return;
        }
        if (g_gainsFor != g_live.entity) { g_gainsFor = g_live.entity; reloadGains(); }
        if (g_measure.phase == 1)
        {
            g_measure.overrides = editor::overridesEnabled;
            editor::overridesEnabled = false;
            states::restoreAll();
            detail::g_measuringReference = true;
            g_measure.since = detail::g_veUpdates;
            g_measure.phase = 2;
            g_measure.note = "measuring the level's own lighting";
            logger::info("[enlighten] calibration: overrides suspended, measuring the level's own lighting");
        }
        else if (g_measure.phase >= 2)
        {
            detail::runtimeRefresh(MEASURE_REFRESH_UPDATES);
            if (g_measure.phase == 2 && (detail::g_veUpdates - g_measure.since >= MEASURE_SETTLE_UPDATES || g_measure.force))
            {
                solveCounts(g_measure.counts);
                g_measure.phase = 3;
            }
            if (g_measure.phase == 3)
            {
                std::vector<uint32_t> now;
                solveCounts(now);
                size_t done = 0;
                for (size_t i = 0; i < now.size() && i < g_measure.counts.size(); ++i) done += now[i] >= g_measure.counts[i] + MEASURE_SOLVES;
                if (g_measure.force || done * MEASURE_DONE_DEN >= now.size() * MEASURE_DONE_NUM)
                {
                    std::string note;
                    calibrate(note);
                    g_measure.note = note;
                    resumeOverrides();
                }
                else
                {
                    char b[96];
                    std::snprintf(b, sizeof(b), "measuring the level's own lighting: %zu of %zu systems re-solved", done, now.size());
                    g_measure.note = b;
                }
            }
        }
        static int floorTick = 0;
        if ((floorTick++ % FLOOR_SCALE_INTERVAL) == 0)
            if (const std::vector<GainRange>* ranges = g_gainRanges.load(std::memory_order_acquire))
                for (const GainRange& r : *ranges)
                    if (r.raw0Sum > 0.0 && r.rawSum.load() > 0.0)
                        const_cast<GainRange&>(r).floorScale = float((std::clamp)(r.rawSum.load() / r.raw0Sum, 0.0, MAX_FLOOR_SCALE));
        if (liveProbes) updateLiveProbes();
        else if (g_lp.applied || g_lp.loaded) restoreLiveProbes();
    }
    void onLevelUnload()
    {
        g_loaded.clear();
        g_bf3ProbesScaled = false;
        if (fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance()) clampProbes(r->m_runtimeSettings, false);
        g_live = Live{};
        g_pendingApply.clear();
        g_pendingRestore = false;
        g_liveNote.clear();
        g_probeClamped = false;
        if (g_measure.phase >= 2) resumeOverrides();
        g_measure = Measure{};
        g_lp = LiveProbes{};
        g_gainRanges.store(nullptr, std::memory_order_release);
        g_gainTables.clear();
        g_gainsFor = nullptr;
    }

    void renderUI()
    {
        fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance();
        if (!r) { ImGui::TextDisabled("no Enlighten renderer"); return; }
        ImGui::Text("renderer: %s", r->state == 0 ? "solving live" : r->state == 1 ? "static atlas" : "disabled");
        const std::string generated = directory() + "/" + mapFileName() + "_generated.bf3edb";
        std::error_code ec;
        if (!g_live.entity && fs::exists(generated, ec))
        {
            ImGui::SameLine();
            if (ImGui::SmallButton("apply generated live")) applyLive(generated);
        }
        if (g_live.entity)
        {
            ImGui::SameLine();
            if (ImGui::SmallButton("restore shipped atlas")) { restoreLiveProbes(); g_pendingRestore = true; }
            ImGui::SameLine();
            if (ImGui::SmallButton("log systems")) logSystems(r);
            ImGui::TextDisabled("%zu generated systems live", g_live.systems);
            if (ImGui::Checkbox("live probes", &liveProbes) && !liveProbes) restoreLiveProbes();
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("the shipped probe SH scaled by how the lightmap each probe sees changed");
            ImGui::SliderFloat("calibration strength", &calibrationStrength, 0.0f, 1.0f, "%.2f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("0 = the raw solve, 1 = matched to the shipped atlas at the level's own lighting");
            ImGui::SliderFloat("output scale", &outputScale, 0.1f, 4.0f, "x%.2f", ImGuiSliderFlags_Logarithmic);
        }
        if (!g_liveNote.empty()) ImGui::TextWrapped("%s", g_liveNote.c_str());
    }
#endif
}
