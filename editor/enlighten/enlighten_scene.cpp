#include "enlighten_scene.h"
#include "enlighten_internal.h"
#include "../lights/lights.h"
#include "../editor_context.h"
#include "../../hooks/functions.h"
#include "../../utils/log.h"

#include <d3d11.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstring>
#include <fstream>
#include <limits>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace editor::enlighten::scene
{
    using fb::Vec3;
    using fb::vec3;

    namespace
    {
        constexpr int RASTER_PER_TICK = 200; // meshes
        constexpr int TERRAIN_RAYS_PER_TICK = 3000;
        constexpr float GRID_SPACING = 2.0f; // meters
        constexpr float GRID_MARGIN = 40.0f; // meters
        constexpr double MAX_GRID_CELLS = 4.0e6; // terrain grid, step grows until it fits
        constexpr float GRID_GROW = 1.5f; // step multiplier
        constexpr float GRID_HEADROOM = 200.0f; // meters above/below the scene box
        constexpr int TERRAIN_PASSES = 12; // casts per column, one per static mesh passed
        constexpr float PASS_BELOW = 0.05f; // meters under a passed mesh hit
        constexpr float TERRAIN_TOP = 1500.0f; // terrain texel ray start, meters
        constexpr float TERRAIN_BOTTOM = -1500.0f;
        constexpr size_t TERRAIN_VOTE_SAMPLES = 400; // texels cast to pick the terrain body
        constexpr int GEOMETRY_TRIES = 2000; // ticks a mesh may wait for resident geometry
        constexpr int COPIES_PER_TICK = 6; // gpu geometry copies drained per tick
        constexpr float RASTER_SHARE = 0.8f; // progress: raster phase, rest is terrain
        constexpr float TERRAIN_SHARE = 0.2f;
        constexpr uint32_t BVH_LEAF_TRIS = 6;
        constexpr int BVH_MAX_DEPTH = 40;
        constexpr int TRACE_STACK = 64; // bvh nodes, a push adds 2
        constexpr float RAY_TRI_EPS = 1e-9f; // ray parallel to the triangle
        constexpr float ZERO_DIR = 1e-12f; // stands in for a zero ray component
        constexpr uint32_t MAX_INSTANCE_BUCKETS = 1u << 22; // sanity caps on engine tables
        constexpr uint32_t MAX_INSTANCES = 400000;
        constexpr uint32_t MAX_TERRAIN_TILES = 4096;
        constexpr size_t MAX_REGISTRATIONS = 400000;
        constexpr float DEGENERATE_DET = 1e-18f; // terrain uv transform not invertible
        constexpr float MIN_UV_AREA = 1e-6f; // triangle area in texels^2
        constexpr float MIN_EDGE = 1e-6f; // edge length in texels
        constexpr float EDGE_EXPAND = 0.7f; // texels outside an edge still rastered (conservative, feeds the gutter)
        constexpr float RASTER_PAD = 1.0f; // texel bbox pad
        constexpr float SNAP_MIN_CELL = 0.25f; // lod snap grid cell, meters
        constexpr float SNAP_CELLS = 32.0f; // cells along the largest instance extent
        constexpr int SNAP_RINGS = 4; // cell rings searched for a LOD0 texel
        constexpr float SNAP_NORMAL_PENALTY = 4.0f; // distance scale per unit of normal mismatch
        constexpr float OPEN_SKY = 0.85f; // sky visibility of texels that must face up
        constexpr size_t ORIENT_MIN_TEXELS = 200;

        // VertexElementFormat
        constexpr uint32_t VEF_FLOAT2 = 2;
        constexpr uint32_t VEF_HALF2 = 6;
        constexpr uint32_t VEF_SHORT2 = 0xF;
        constexpr uint32_t VEF_SHORT2N = 0x13;
        constexpr uint32_t VEF_USHORT2N = 0x18;

        Phase g_phase = Phase::Idle;
        std::atomic<float> g_progress = 0.0f;
        std::string g_error;
        Stats g_stats;

        uint32_t W = 0, H = 0;
        std::vector<Vec3> g_pos, g_nrm;
        std::vector<uint8_t> g_cov; // 0 empty, 1 edge sample, 2 interior sample
        std::vector<uint8_t> g_texKind; // 0 mesh, 1 terrain
        std::vector<uint32_t> g_texTri;
        std::vector<uint32_t> g_covList;
        bool g_oriented = false;

        // uv from world x, z, BF3 setEnlightenShaderParameters 0x94ed80 normalises to the tile box
        struct TerrainTile
        {
            float m[6], x0, z0, x1, z1;
            float u0, v0, u1, v1;
            bool normalized = false;
            void toUV(float x, float z, float& u, float& v) const
            {
                if (normalized) { x = (x - x0) / (x1 - x0); z = (z - z0) / (z1 - z0); }
                u = x * m[0] + z * m[2] + m[4]; v = x * m[1] + z * m[3] + m[5];
            }
            void toWorld(float u, float v, float& x, float& z) const
            {
                const float det = m[0] * m[3] - m[1] * m[2];
                const float du = u - m[4], dv = v - m[5];
                x = (du * m[3] - dv * m[2]) / det;
                z = (dv * m[0] - du * m[1]) / det;
                if (normalized) { x = x0 + x * (x1 - x0); z = z0 + z * (z1 - z0); }
            }
        };
        std::vector<TerrainTile> g_terrain;

        const TerrainTile* terrainAtUV(float u, float v)
        {
            for (const TerrainTile& t : g_terrain)
                if (u >= t.u0 && u < t.u1 && v >= t.v0 && v < t.v1) return &t;
            return nullptr;
        }

        const TerrainTile* terrainAtWorld(float x, float z)
        {
            const TerrainTile* best = nullptr;
            for (const TerrainTile& t : g_terrain)
                if (x >= t.x0 && x < t.x1 && z >= t.z0 && z < t.z1 && (!best || (t.x1 - t.x0) < (best->x1 - best->x0))) best = &t;
            return best;
        }

        struct InstanceMap { uint32_t hash; float t[4]; float tr[2]; };
        std::vector<InstanceMap> g_table;

        struct Work
        {
            fb::MeshAsset* mesh;
            fb::LinearTransform frame;
            InstanceMap map;
            int tries;
            bool chart;
            Vec3 mn, mx;
            bool hasBox;
            int geoState = 0;
            uint32_t rastered = 0, subsets = 0, noUv = 0, otherStream = 0, noStride = 0;
        };
        std::vector<Work> g_work;
        size_t g_next = 0;

        struct Tri
        {
            Vec3 v[3];
            float uv[3][2];
            bool charted;
            uint32_t work = ~0u; // ~0 terrain
        };
        std::vector<Tri> g_tris;
        constexpr uint32_t LOD_TRI = 0x80000000u;
        uint32_t g_lodTris = 0;
        std::vector<uint32_t>* g_record = nullptr;
        std::vector<std::pair<uint32_t, uint32_t>> g_lodSnap;

        float g_gridX0 = 0.0f, g_gridZ0 = 0.0f, g_gridStep = GRID_SPACING, g_gridTop = 0.0f, g_gridBottom = 0.0f;
        uint32_t g_gridNx = 0, g_gridNz = 0;
        std::vector<float> g_gridH; // NaN = no hit
        size_t g_gridNext = 0;
        std::vector<uint32_t> g_terrainTexels;
        size_t g_terrainNext = 0;
        std::vector<void*> g_meshBodies;
        void* g_terrainBody = nullptr;
        bool g_terrainVoted = false;
        uint32_t g_passedMeshes = 0;

        bool castTerrain(float x, float z, float top, float bottom, fb::RayCastHit& out, int& casts)
        {
            float from = top;
            for (int i = 0; i < TERRAIN_PASSES && from > bottom; ++i)
            {
                fb::RayCastHit hit{};
                ++casts;
                if (!fb::physicsRayQuery(vec3(x, from, z), vec3(x, bottom, z), hit, fb::RAY_CAST_WORLD_ONLY, nullptr)) return false;
                const bool mesh = std::binary_search(g_meshBodies.begin(), g_meshBodies.end(), hit.m_rigidBody) ||
                    (g_terrainBody && hit.m_rigidBody != g_terrainBody);
                if (!mesh) { out = hit; return true; }
                ++g_passedMeshes;
                from = hit.m_position.m_y - PASS_BELOW;
            }
            return false;
        }
        Vec3 g_sceneMin, g_sceneMax;
        std::vector<float> g_skyVis;

        struct Registration { uint16_t handle; fb::MeshAsset* mesh; fb::LinearTransform world; };
        std::mutex g_regMutex;
        std::vector<Registration> g_regs;

        void fail(const std::string& why)
        {
            g_error = why;
            g_phase = Phase::Failed;
            logger::warning("[enlighten] scene: {}", why);
        }

        bool texelAt(const Tri& tr, float u, float v, size_t& out)
        {
            const float hu = (1.0f - u - v) * tr.uv[0][0] + u * tr.uv[1][0] + v * tr.uv[2][0];
            const float hv = (1.0f - u - v) * tr.uv[0][1] + u * tr.uv[1][1] + v * tr.uv[2][1];
            const int hx = (std::clamp)(int(hu * float(W)), 0, int(W) - 1);
            const int hy = (std::clamp)(int(hv * float(H)), 0, int(H) - 1);
            out = size_t(hy) * W + hx;
            return g_cov[out] != 0;
        }

        struct Node { Vec3 mn, mx; uint32_t left, count; }; // count 0 = inner, left = first triangle when leaf
        std::vector<Node> g_nodes;
        std::vector<uint32_t> g_right;
        std::vector<uint32_t> g_triOrder;

        uint32_t buildTree(uint32_t first, uint32_t count, int depth)
        {
            Node n{};
            n.mn = vec3(1e30f, 1e30f, 1e30f);
            n.mx = vec3(-1e30f, -1e30f, -1e30f);
            for (uint32_t i = 0; i < count; ++i)
                for (const Vec3& v : g_tris[g_triOrder[first + i]].v) { n.mn = fb::vmin(n.mn, v); n.mx = fb::vmax(n.mx, v); }
            const uint32_t idx = uint32_t(g_nodes.size());
            g_nodes.push_back(n);
            g_right.push_back(0);
            if (count <= BVH_LEAF_TRIS || depth > BVH_MAX_DEPTH)
            {
                g_nodes[idx].left = first;
                g_nodes[idx].count = count;
                return idx;
            }
            const Vec3 ext = n.mx - n.mn;
            int axis = 0;
            if (ext.m_y > fb::at(ext, axis)) axis = 1;
            if (ext.m_z > fb::at(ext, axis)) axis = 2;
            const uint32_t half = count / 2;
            std::nth_element(g_triOrder.begin() + first, g_triOrder.begin() + first + half, g_triOrder.begin() + first + count,
                [axis](uint32_t a, uint32_t b)
                {
                    const Tri& ta = g_tris[a]; const Tri& tb = g_tris[b];
                    return fb::at(ta.v[0], axis) + fb::at(ta.v[1], axis) + fb::at(ta.v[2], axis) <
                           fb::at(tb.v[0], axis) + fb::at(tb.v[1], axis) + fb::at(tb.v[2], axis);
                });
            const uint32_t l = buildTree(first, half, depth + 1);
            const uint32_t r = buildTree(first + half, count - half, depth + 1);
            g_nodes[idx].left = l;
            g_nodes[idx].count = 0;
            g_right[idx] = r;
            return idx;
        }

        void buildBvh()
        {
            g_nodes.clear();
            g_right.clear();
            g_triOrder.resize(g_tris.size());
            for (uint32_t i = 0; i < g_triOrder.size(); ++i) g_triOrder[i] = i;
            if (g_tris.empty()) return;
            g_nodes.reserve(g_tris.size() / 3 + 16);
            g_right.reserve(g_tris.size() / 3 + 16);
            buildTree(0, uint32_t(g_tris.size()), 0);
        }

        bool rayBox(const Vec3& o, const Vec3& inv, const Node& n, float tmax)
        {
            float t0 = 0.0f, t1 = tmax;
            for (int k = 0; k < 3; ++k)
            {
                float a = (fb::at(n.mn, k) - fb::at(o, k)) * fb::at(inv, k);
                float b = (fb::at(n.mx, k) - fb::at(o, k)) * fb::at(inv, k);
                if (a > b) std::swap(a, b);
                t0 = (std::max)(t0, a);
                t1 = (std::min)(t1, b);
                if (t0 > t1) return false;
            }
            return true;
        }

        float rayTri(const Vec3& o, const Vec3& d, const Tri& t, float& u, float& v)
        {
            const Vec3 e1 = t.v[1] - t.v[0], e2 = t.v[2] - t.v[0];
            const Vec3 p = fb::cross(d, e2);
            const float det = fb::dot(e1, p);
            if (std::fabs(det) < RAY_TRI_EPS) return -1.0f;
            const float inv = 1.0f / det;
            const Vec3 s = o - t.v[0];
            u = fb::dot(s, p) * inv;
            if (u < 0.0f || u > 1.0f) return -1.0f;
            const Vec3 q = fb::cross(s, e1);
            v = fb::dot(d, q) * inv;
            if (v < 0.0f || u + v > 1.0f) return -1.0f;
            return fb::dot(e2, q) * inv;
        }

        struct Hit { uint32_t tri = 0; float t = 0.0f, u = 0.0f, v = 0.0f; };

        bool trace(const Vec3& o, const Vec3& d, float tmax, bool anyHit, Hit& hit)
        {
            if (g_nodes.empty()) return false;
            const auto safeInv = [](float x) { return 1.0f / (x != 0.0f ? x : ZERO_DIR); };
            const Vec3 inv = vec3(safeInv(d.m_x), safeInv(d.m_y), safeInv(d.m_z));
            uint32_t stack[TRACE_STACK];
            int sp = 0;
            stack[sp++] = 0;
            bool any = false;
            float best = tmax;
            while (sp)
            {
                const uint32_t ni = stack[--sp];
                const Node& n = g_nodes[ni];
                if (!rayBox(o, inv, n, best)) continue;
                if (n.count)
                {
                    for (uint32_t i = 0; i < n.count; ++i)
                    {
                        const uint32_t ti = g_triOrder[n.left + i];
                        const Tri& tr = g_tris[ti];
                        if (tr.work == ~0u)
                        {
                            const Vec3 ng = fb::cross(tr.v[1] - tr.v[0], tr.v[2] - tr.v[0]);
                            if (fb::dot(d, ng) * (ng.m_y < 0.0f ? -1.0f : 1.0f) > 0.0f) continue;
                        }
                        float u, v;
                        const float t = rayTri(o, d, tr, u, v);
                        if (t > 0.0f && t < best)
                        {
                            best = t; any = true; hit.tri = ti; hit.u = u; hit.v = v;
                            if (anyHit) { hit.t = t; return true; }
                        }
                    }
                }
                else if (sp < TRACE_STACK - 2)
                {
                    stack[sp++] = n.left;
                    stack[sp++] = g_right[ni];
                }
            }
            hit.t = best;
            return any;
        }

        bool readInstanceTable()
        {
            g_table.clear();
            fb::EnlightenRenderer* renderer = fb::EnlightenRenderer::GetInstance();
            if (!renderer) return false;
#if defined(BFVE_GAME_BF3)
            // EnlightenDatabase::m_instanceHashMap (retail load sub_17B2E70): node {key, LightMapInstance +16, next +64}
            for (fb::EnlightenRendererEntity** it = renderer->m_entities.begin(); it && it < renderer->m_entities.end(); ++it)
            {
                const fb::EnlightenRuntimeDatabase* db = *it ? (*it)->m_database : nullptr;
                if (!db || !db->m_instanceBuckets || db->m_instanceBucketCount > MAX_INSTANCE_BUCKETS) continue;
                for (uint32_t b = 0; b < db->m_instanceBucketCount; ++b)
                    for (const fb::EnlightenInstanceNode* n = db->m_instanceBuckets[b]; n; n = n->m_next)
                    {
                        InstanceMap m{};
                        std::memcpy(m.t, n->m_uvTransform, 16);
                        std::memcpy(m.tr, n->m_uvTranslation, 8);
                        m.hash = n->m_key;
                        g_table.push_back(m);
                    }
            }
#else
            for (fb::EnlightenRendererEntity** it = renderer->m_entities.begin(); it && it < renderer->m_entities.end(); ++it)
            {
                const fb::EnlightenRuntimeDatabase* db = *it ? (*it)->m_database : nullptr;
                const fb::EnlightenDatabaseHeader* blob = db ? db->m_blob : nullptr;
                if (!blob || !blob->m_instances || blob->m_instanceCount > MAX_INSTANCES) continue;
                for (uint32_t i = 0; i < blob->m_instanceCount; ++i)
                {
                    const fb::EnlightenInstanceEntry& e = blob->m_instances[i];
                    InstanceMap m{};
                    std::memcpy(m.t, e.m_scale, 16);
                    std::memcpy(m.tr, e.m_offset, 8);
                    m.hash = e.m_hash;
                    g_table.push_back(m);
                }
            }
#endif
            std::sort(g_table.begin(), g_table.end(), [](const InstanceMap& a, const InstanceMap& b) { return a.hash < b.hash; });
            return !g_table.empty();
        }

        const InstanceMap* findInstance(uint32_t hash)
        {
            auto it = std::lower_bound(g_table.begin(), g_table.end(), hash, [](const InstanceMap& a, uint32_t h) { return a.hash < h; });
            return it != g_table.end() && it->hash == hash ? &*it : nullptr;
        }

        uint32_t instanceHash(fb::MeshAsset* mesh, const fb::LinearTransform& frame)
        {
            fb::Guid* guid = static_cast<fb::DataContainer*>(mesh)->getInstanceGuid();
            if (!guid) return 0;
            alignas(16) fb::LinearTransform t = frame;
#if defined(BFVE_GAME_BF3)
            using Fn = uint32_t(__cdecl*)(const fb::Guid*, const fb::LinearTransform*);
#else
            using Fn = uint32_t(__fastcall*)(const fb::Guid*, const fb::LinearTransform*);
#endif
            return reinterpret_cast<Fn>(OFF_Enlighten_lightMapInstanceHash)(guid, &t);
        }

        void readTerrain()
        {
            g_terrain.clear();
            fb::EnlightenRenderer* renderer = fb::EnlightenRenderer::GetInstance();
#if defined(BFVE_GAME_BF3)
            // EnlightenDatabase::m_terrainLightMaps: {AABB, uvTransform, uvTranslation}, 64 bytes
            for (auto** it = renderer ? renderer->m_entities.begin() : nullptr; it && it < renderer->m_entities.end(); ++it)
            {
                const fb::EnlightenRuntimeDatabase* db = *it ? (*it)->m_database : nullptr;
                if (!db) continue;
                for (const fb::EnlightenTerrainLightMap* e = db->m_terrainLightMaps.begin(); e && e < db->m_terrainLightMaps.end(); ++e)
                {
                    TerrainTile t{};
                    std::memcpy(t.m, e->m_uvTransform, 16);
                    std::memcpy(t.m + 4, e->m_uvTranslation, 8);
                    t.x0 = e->m_min.m_x; t.z0 = e->m_min.m_z; t.x1 = e->m_max.m_x; t.z1 = e->m_max.m_z;
                    t.normalized = true;
                    if (g_terrain.empty())
                        logger::info("[enlighten] scene: terrain tile box {:.0f},{:.0f} - {:.0f},{:.0f}, uv transform {:.5f} {:.5f} {:.5f} {:.5f} + {:.5f} {:.5f}",
                            t.x0, t.z0, t.x1, t.z1, t.m[0], t.m[1], t.m[2], t.m[3], t.m[4], t.m[5]);
                    if (std::fabs(t.m[0] * t.m[3] - t.m[1] * t.m[2]) < DEGENERATE_DET || t.x1 <= t.x0 || t.z1 <= t.z0) continue;
                    float ua, va, ub, vb;
                    t.toUV(t.x0, t.z0, ua, va);
                    t.toUV(t.x1, t.z1, ub, vb);
                    t.u0 = (std::min)(ua, ub); t.u1 = (std::max)(ua, ub);
                    t.v0 = (std::min)(va, vb); t.v1 = (std::max)(va, vb);
                    g_terrain.push_back(t);
                }
            }
            g_stats.terrainTiles = uint32_t(g_terrain.size());
#else
            if (!renderer || !renderer->m_terrain || renderer->m_terrainCount > MAX_TERRAIN_TILES) return;
            for (uint32_t i = 0; i < renderer->m_terrainCount; ++i)
            {
                const fb::EnlightenTerrainEntry& e = renderer->m_terrain[i];
                TerrainTile t{};
                std::memcpy(t.m, e.m_uv, sizeof(t.m));
                t.x0 = e.m_x0; t.z0 = e.m_z0; t.x1 = e.m_x1; t.z1 = e.m_z1;
                if (std::fabs(t.m[0] * t.m[3] - t.m[1] * t.m[2]) < DEGENERATE_DET || t.x1 <= t.x0 || t.z1 <= t.z0) continue;
                float ua, va, ub, vb;
                t.toUV(t.x0, t.z0, ua, va);
                t.toUV(t.x1, t.z1, ub, vb);
                t.u0 = (std::min)(ua, ub); t.u1 = (std::max)(ua, ub);
                t.v0 = (std::min)(va, vb); t.v1 = (std::max)(va, vb);
                g_terrain.push_back(t);
            }
            g_stats.terrainTiles = uint32_t(g_terrain.size());
#endif
        }

        void readSkyVisibility()
        {
            g_skyVis.clear();
            detail::Original o;
            o.tex = detail::g_skyVisTex;
            std::string err;
            if (!o.tex || !detail::readback(o, err) || o.bpp != 2 || !o.width || !o.height) return;
            const uint16_t* v = reinterpret_cast<const uint16_t*>(o.pixels.data());
            g_skyVis.resize(size_t(W) * H);
            for (uint32_t y = 0; y < H; ++y)
                for (uint32_t x = 0; x < W; ++x)
                    g_skyVis[size_t(y) * W + x] = v[size_t(y * o.height / H) * o.width + (x * o.width / W)] / 65535.0f;
        }

        float decodeElem(const uint8_t* p, uint32_t format, int comp)
        {
            switch (format)
            {
            case VEF_FLOAT2: return reinterpret_cast<const float*>(p)[comp];
            case VEF_HALF2:
            {
                // ieee half, inf/nan read as 0
                const uint16_t h = reinterpret_cast<const uint16_t*>(p)[comp];
                const uint32_t s = (h >> 15) & 1, e = (h >> 10) & 0x1F, m = h & 0x3FF;
                float v;
                if (e == 0) v = std::ldexp(float(m), -24);
                else if (e == 31) v = 0.0f;
                else v = std::ldexp(float(m + 1024), int(e) - 25);
                return s ? -v : v;
            }
            case VEF_SHORT2N: return reinterpret_cast<const int16_t*>(p)[comp] / 32767.0f;
            case VEF_USHORT2N: return reinterpret_cast<const uint16_t*>(p)[comp] / 65535.0f;
            case VEF_SHORT2: return float(reinterpret_cast<const int16_t*>(p)[comp]);
            default: return 0.0f;
            }
        }

        void rasterTriangle(const float (&uv)[3][2], const Vec3 (&wp)[3], uint32_t triIndex, float windingSign)
        {
            const bool lodFill = (triIndex & LOD_TRI) != 0;
            const Vec3 n = fb::normalized(fb::cross(wp[1] - wp[0], wp[2] - wp[0])) * windingSign;
            float px[3], py[3];
            for (int i = 0; i < 3; ++i) { px[i] = uv[i][0] * float(W); py[i] = uv[i][1] * float(H); }
            const float area = (px[1] - px[0]) * (py[2] - py[0]) - (px[2] - px[0]) * (py[1] - py[0]);
            if (std::fabs(area) < MIN_UV_AREA) return;
            const int x0 = (std::max)(0, int(std::floor((std::min)({ px[0], px[1], px[2] }) - RASTER_PAD)));
            const int x1 = (std::min)(int(W) - 1, int(std::ceil((std::max)({ px[0], px[1], px[2] }) + RASTER_PAD)));
            const int y0 = (std::max)(0, int(std::floor((std::min)({ py[0], py[1], py[2] }) - RASTER_PAD)));
            const int y1 = (std::min)(int(H) - 1, int(std::ceil((std::max)({ py[0], py[1], py[2] }) + RASTER_PAD)));
            if (x1 - x0 > int(W) * 6 / 10 || y1 - y0 > int(H) * 6 / 5) return; // broken uvs: wider than 60% of the atlas
            const float inv = 1.0f / area;
            const float len[3] = {
                std::hypot(px[2] - px[1], py[2] - py[1]),
                std::hypot(px[0] - px[2], py[0] - py[2]),
                std::hypot(px[1] - px[0], py[1] - py[0]) };
            for (int y = y0; y <= y1; ++y)
            {
                const float cy = y + 0.5f;
                for (int x = x0; x <= x1; ++x)
                {
                    const float cx = x + 0.5f;
                    float w0 = ((px[1] - cx) * (py[2] - cy) - (px[2] - cx) * (py[1] - cy)) * inv;
                    float w1 = ((px[2] - cx) * (py[0] - cy) - (px[0] - cx) * (py[2] - cy)) * inv;
                    float w2 = 1.0f - w0 - w1;
                    const float d0 = w0 * std::fabs(area) / (std::max)(len[0], MIN_EDGE);
                    const float d1 = w1 * std::fabs(area) / (std::max)(len[1], MIN_EDGE);
                    const float d2 = w2 * std::fabs(area) / (std::max)(len[2], MIN_EDGE);
                    if (d0 < -EDGE_EXPAND || d1 < -EDGE_EXPAND || d2 < -EDGE_EXPAND) continue;
                    const bool inside = w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f;
                    const size_t i = size_t(y) * W + x;
                    const bool lodOwned = g_cov[i] && (g_texTri[i] & LOD_TRI);
                    if (lodFill ? g_cov[i] && !lodOwned : g_cov[i] == 2 && !inside && !lodOwned) continue;
                    w0 = (std::max)(w0, 0.0f); w1 = (std::max)(w1, 0.0f); w2 = (std::max)(w2, 0.0f);
                    const float s = w0 + w1 + w2;
                    if (s <= 0.0f) continue;
                    g_pos[i] = (wp[0] * w0 + wp[1] * w1 + wp[2] * w2) / s;
                    g_nrm[i] = n;
                    g_texTri[i] = triIndex;
                    g_cov[i] = inside ? 2 : 1;
                    if (g_record) g_record->push_back(uint32_t(i));
                }
            }
        }

        void rasterGeometry(const Work& w, const lights::GeoView& geo, bool lodFill)
        {
            const float windingSign = fb::determinant(w.frame) < 0.0f ? -1.0f : 1.0f;
            Work& wd = const_cast<Work&>(w);
            for (const lights::GeoSubset& s : geo.subsets)
            {
                if (!lodFill)
                {
                    ++wd.subsets;
                    wd.noStride += !s.stride;
                    wd.noUv += s.stride && w.chart && !s.uvFormat;
                    wd.otherStream += s.stride && w.chart && s.uvFormat && s.uvStream != 0;
                }
                if (!s.stride || (w.chart && (!s.uvFormat || s.uvStream != 0))) continue;
                const uint8_t* vb = geo.vertices + s.vertexOffset;
                if (s.vertexOffset + size_t(s.vertexCount) * s.stride > geo.vertexBytes) continue;
                for (uint32_t t = 0; t < s.primitiveCount; ++t)
                {
                    const size_t base = (size_t(s.startIndex) + size_t(t) * 3) * geo.bytesPerIndex;
                    if (base + 3 * geo.bytesPerIndex > geo.indexBytes) break;
                    uint32_t idx[3];
                    if (geo.bytesPerIndex == 2)
                    {
                        uint16_t s16[3];
                        std::memcpy(s16, geo.indices + base, 6);
                        idx[0] = s16[0]; idx[1] = s16[1]; idx[2] = s16[2];
                    }
                    else std::memcpy(idx, geo.indices + base, 12);
                    if (idx[0] >= s.vertexCount || idx[1] >= s.vertexCount || idx[2] >= s.vertexCount) continue;
                    Tri tri{};
                    Vec3 wp[3];
                    for (int k = 0; k < 3; ++k)
                    {
                        const uint8_t* v = vb + size_t(idx[k]) * s.stride;
                        Vec3 local{};
                        if (s.posHalves)
                            local = vec3(decodeElem(v + s.posOffset, VEF_HALF2, 0), decodeElem(v + s.posOffset, VEF_HALF2, 1), decodeElem(v + s.posOffset, VEF_HALF2, 2));
                        else
                            std::memcpy(&local, v + s.posOffset, 12);
                        wp[k] = fb::transformPoint(w.frame, local);
                        tri.v[k] = wp[k];
                        if (!w.chart) continue;
                        const float u = decodeElem(v + s.uvOffset, s.uvFormat, 0);
                        const float vv = decodeElem(v + s.uvOffset, s.uvFormat, 1);
                        tri.uv[k][0] = w.map.t[0] * u + w.map.t[1] * vv + w.map.tr[0];
                        tri.uv[k][1] = w.map.t[2] * u + w.map.t[3] * vv + w.map.tr[1];
                    }
                    tri.charted = w.chart;
                    tri.work = uint32_t(&w - g_work.data());
                    if (lodFill)
                    {
                        rasterTriangle(tri.uv, wp, LOD_TRI | g_lodTris++, windingSign);
                        continue;
                    }
                    const uint32_t ti = uint32_t(g_tris.size());
                    g_tris.push_back(tri);
                    if (w.chart) rasterTriangle(tri.uv, wp, ti, windingSign);
                }
            }
        }

        void snapLodTexels(std::vector<uint32_t>& base, std::vector<uint32_t>& lod)
        {
            std::sort(base.begin(), base.end());
            base.erase(std::unique(base.begin(), base.end()), base.end());
            std::sort(lod.begin(), lod.end());
            lod.erase(std::unique(lod.begin(), lod.end()), lod.end());
            Vec3 mn = g_pos[base[0]], mx = mn;
            for (uint32_t t : base) { mn = fb::vmin(mn, g_pos[t]); mx = fb::vmax(mx, g_pos[t]); }
            const Vec3 ext = mx - mn;
            const float cell = (std::max)(SNAP_MIN_CELL, (std::max)({ ext.m_x, ext.m_y, ext.m_z }) / SNAP_CELLS);
            const auto key = [&](int x, int y, int z) { return (uint64_t(uint16_t(x)) << 32) | (uint64_t(uint16_t(y)) << 16) | uint16_t(z); };
            const auto cellOf = [&](const Vec3& p, int& x, int& y, int& z)
            {
                x = int(std::floor((p.m_x - mn.m_x) / cell)); y = int(std::floor((p.m_y - mn.m_y) / cell)); z = int(std::floor((p.m_z - mn.m_z) / cell));
            };
            std::unordered_map<uint64_t, std::vector<uint32_t>> grid;
            for (uint32_t t : base)
            {
                if (g_texTri[t] & LOD_TRI) continue;
                int x, y, z;
                cellOf(g_pos[t], x, y, z);
                grid[key(x, y, z)].push_back(t);
            }
            for (uint32_t t : lod)
            {
                if (!(g_texTri[t] & LOD_TRI)) continue;
                int cx, cy, cz;
                cellOf(g_pos[t], cx, cy, cz);
                uint32_t best = ~0u;
                float bestScore = 1e30f;
                for (int r = 0; r <= SNAP_RINGS && best == ~0u; ++r)
                    for (int dz = -r; dz <= r; ++dz)
                        for (int dy = -r; dy <= r; ++dy)
                            for (int dx = -r; dx <= r; ++dx)
                            {
                                if ((std::max)({ std::abs(dx), std::abs(dy), std::abs(dz) }) != r) continue;
                                const auto it = grid.find(key(cx + dx, cy + dy, cz + dz));
                                if (it == grid.end()) continue;
                                for (uint32_t b : it->second)
                                {
                                    const float align = std::fabs(fb::dot(g_nrm[t], g_nrm[b]));
                                    const float score = fb::distanceSq(g_pos[t], g_pos[b]) * (1.0f + SNAP_NORMAL_PENALTY * (1.0f - align));
                                    if (score < bestScore) { bestScore = score; best = b; }
                                }
                            }
                if (best != ~0u) g_lodSnap.push_back({ t, best });
            }
        }

        // 1 done, 0 geometry not resident yet, <0 no geometry
        int rasterWork(Work& w)
        {
            lights::GeoView geo;
            const int state = lights::meshGeometry(w.mesh, geo);
            w.geoState = state;
            if (state != 1) return state;
            std::vector<uint32_t> base, lod;
            g_record = &base;
            rasterGeometry(w, geo, false);
            g_record = &lod;
            if (w.chart)
                for (uint32_t l = 1; l < lights::meshLodCount(w.mesh); ++l)
                    if (lights::meshGeometry(w.mesh, geo, l) == 1)
                        rasterGeometry(w, geo, true);
            g_record = nullptr;
            w.rastered = uint32_t(base.size() + lod.size());
            if (!lod.empty() && !base.empty()) snapLodTexels(base, lod);
            return 1;
        }

        void orientNormals()
        {
            if (g_oriented) return;
            g_oriented = true;
            if (g_skyVis.empty()) return;
            double up = 0.0; size_t n = 0;
            for (uint32_t i : g_covList)
                if (g_skyVis[i] > OPEN_SKY) { up += g_nrm[i].m_y; ++n; }
            if (n > ORIENT_MIN_TEXELS && up < 0.0)
                for (size_t i = 0; i < g_cov.size(); ++i)
                    if (g_texKind[i] == 0) g_nrm[i] = -g_nrm[i];
        }

        void setupTerrainGrid()
        {
            g_gridH.clear();
            g_gridNext = 0;
            if (g_sceneMax.m_x <= g_sceneMin.m_x) return;
            float step = GRID_SPACING;
            const float sx = g_sceneMax.m_x - g_sceneMin.m_x + 2.0f * GRID_MARGIN, sz = g_sceneMax.m_z - g_sceneMin.m_z + 2.0f * GRID_MARGIN;
            while (double(sx / step + 2) * double(sz / step + 2) > MAX_GRID_CELLS) step *= GRID_GROW;
            g_gridStep = step;
            g_gridX0 = g_sceneMin.m_x - GRID_MARGIN;
            g_gridZ0 = g_sceneMin.m_z - GRID_MARGIN;
            g_gridNx = uint32_t(sx / step) + 2;
            g_gridNz = uint32_t(sz / step) + 2;
            g_gridTop = g_sceneMax.m_y + GRID_HEADROOM;
            g_gridBottom = g_sceneMin.m_y - GRID_HEADROOM;
            g_gridH.assign(size_t(g_gridNx) * g_gridNz, std::numeric_limits<float>::quiet_NaN());
        }

        void buildTerrainGrid()
        {
            const auto at = [&](uint32_t x, uint32_t z) { return g_gridH[size_t(z) * g_gridNx + x]; };
            const auto emit = [&](const Vec3& a, const Vec3& b, const Vec3& c)
            {
                Tri t{};
                t.v[0] = a; t.v[1] = b; t.v[2] = c;
                const TerrainTile* te = terrainAtWorld((a.m_x + b.m_x + c.m_x) / 3.0f, (a.m_z + b.m_z + c.m_z) / 3.0f);
                t.charted = te != nullptr;
                for (int k = 0; k < 3 && te; ++k)
                {
                    te->toUV(t.v[k].m_x, t.v[k].m_z, t.uv[k][0], t.uv[k][1]);
                    if (t.uv[k][0] < te->u0 || t.uv[k][0] > te->u1 || t.uv[k][1] < te->v0 || t.uv[k][1] > te->v1) t.charted = false;
                }
                g_tris.push_back(t);
            };
            for (uint32_t z = 0; z + 1 < g_gridNz; ++z)
                for (uint32_t x = 0; x + 1 < g_gridNx; ++x)
                {
                    const float h00 = at(x, z), h10 = at(x + 1, z), h01 = at(x, z + 1), h11 = at(x + 1, z + 1);
                    if (std::isnan(h00) || std::isnan(h10) || std::isnan(h01) || std::isnan(h11)) continue;
                    const float x0 = g_gridX0 + x * g_gridStep, x1 = x0 + g_gridStep;
                    const float z0 = g_gridZ0 + z * g_gridStep, z1 = z0 + g_gridStep;
                    const Vec3 p00 = vec3(x0, h00, z0), p10 = vec3(x1, h10, z0), p01 = vec3(x0, h01, z1), p11 = vec3(x1, h11, z1);
                    emit(p00, p01, p11);
                    emit(p00, p11, p10);
                }
        }

        bool gridSurface(float x, float z, float& h, Vec3& n)
        {
            const float fx = (x - g_gridX0) / g_gridStep, fz = (z - g_gridZ0) / g_gridStep;
            if (fx < 0.0f || fz < 0.0f) return false;
            const uint32_t ix = uint32_t(fx), iz = uint32_t(fz);
            if (ix + 1 >= g_gridNx || iz + 1 >= g_gridNz) return false;
            const float h00 = g_gridH[size_t(iz) * g_gridNx + ix], h10 = g_gridH[size_t(iz) * g_gridNx + ix + 1];
            const float h01 = g_gridH[size_t(iz + 1) * g_gridNx + ix], h11 = g_gridH[size_t(iz + 1) * g_gridNx + ix + 1];
            if (std::isnan(h00) || std::isnan(h10) || std::isnan(h01) || std::isnan(h11)) return false;
            const float tx = fx - ix, tz = fz - iz;
            float dhdx, dhdz;
            if (tz >= tx) { dhdx = h11 - h01; dhdz = h01 - h00; h = h00 + dhdz * tz + dhdx * tx; }
            else { dhdx = h10 - h00; dhdz = h11 - h10; h = h00 + dhdx * tx + dhdz * tz; }
            n = fb::normalized(vec3(-dhdx / g_gridStep, 1.0f, -dhdz / g_gridStep));
            return true;
        }

        void finish()
        {
            if (!g_gridH.empty())
            {
                buildTerrainGrid();
                for (size_t i = 0; i < g_cov.size(); ++i)
                {
                    if (!g_cov[i] || g_texKind[i] != 1) continue;
                    float h; Vec3 n;
                    if (!gridSurface(g_pos[i].m_x, g_pos[i].m_z, h, n)) continue;
                    g_pos[i].m_y = h;
                    g_nrm[i] = n;
                }
            }
            uint32_t snapped = 0;
            for (const auto& [t, b] : g_lodSnap)
                if ((g_texTri[t] & LOD_TRI) && g_texKind[t] == 0 && g_texKind[b] == 0)
                {
                    g_pos[t] = g_pos[b];
                    g_nrm[t] = g_nrm[b];
                    ++snapped;
                }
            g_stats.lodSnapped = snapped;
            g_covList.clear();
            for (size_t i = 0; i < g_cov.size(); ++i) if (g_cov[i]) g_covList.push_back(uint32_t(i));
            g_stats.texels = uint32_t(g_covList.size());
            g_stats.triangles = uint32_t(g_tris.size());
            for (uint32_t i : g_covList) g_stats.lodTexels += g_texKind[i] == 0 && g_texTri[i] != ~0u && (g_texTri[i] & LOD_TRI);
            {
                const detail::Original& luma = detail::g_orig[0];
                const uint16_t* L = luma.pixels.empty() ? nullptr : reinterpret_cast<const uint16_t*>(luma.pixels.data());
                std::vector<uint32_t> missing(g_table.size(), 0);
                uint32_t outside = 0, total = 0;
                for (size_t t = 0; L && t < g_cov.size(); ++t)
                {
                    if (g_cov[t] || !L[t]) continue;
                    ++total;
                    const float u = (float(t % W) + 0.5f) / float(W), v = (float(t / W) + 0.5f) / float(H);
                    bool found = false;
                    for (size_t k = 0; k < g_table.size() && !found; ++k)
                    {
                        const InstanceMap& m = g_table[k];
                        const float u0 = m.tr[0], v0 = m.tr[1], u1 = m.tr[0] + m.t[0] + m.t[1], v1 = m.tr[1] + m.t[2] + m.t[3];
                        if (u >= (std::min)(u0, u1) && u < (std::max)(u0, u1) && v >= (std::min)(v0, v1) && v < (std::max)(v0, v1)) { ++missing[k]; found = true; }
                    }
                    outside += !found;
                }
                std::vector<size_t> order(g_table.size());
                for (size_t k = 0; k < order.size(); ++k) order[k] = k;
                std::sort(order.begin(), order.end(), [&](size_t a, size_t b) { return missing[a] > missing[b]; });
                logger::info("[enlighten] scene: {} shipped-lit texels have no mesh ({} outside every instance rect, e.g. terrain)", total, outside);
                // <map>_scene_classes.bmp: black unlit, grey raster mesh, blue raster terrain, red missing in an instance rect,
                // yellow missing in a terrain rect, magenta missing outside every rect, outlines: green instance, cyan terrain
                {
                    std::vector<uint8_t> img(size_t(W) * H * 3, 0);
                    const auto put = [&](uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
                    {
                        if (x >= W || y >= H) return;
                        uint8_t* p = &img[(size_t(H - 1 - y) * W + x) * 3];
                        p[0] = b; p[1] = g; p[2] = r;
                    };
                    const auto inInstance = [&](float u, float v)
                    {
                        for (const InstanceMap& m : g_table)
                        {
                            const float u0 = m.tr[0], v0 = m.tr[1], u1 = m.tr[0] + m.t[0] + m.t[1], v1 = m.tr[1] + m.t[2] + m.t[3];
                            if (u >= (std::min)(u0, u1) && u < (std::max)(u0, u1) && v >= (std::min)(v0, v1) && v < (std::max)(v0, v1)) return true;
                        }
                        return false;
                    };
                    for (size_t t = 0; t < g_cov.size(); ++t)
                    {
                        const uint32_t x = uint32_t(t % W), y = uint32_t(t / W);
                        if (g_cov[t]) { if (g_texKind[t] == 1) put(x, y, 40, 90, 255); else put(x, y, 150, 150, 150); continue; }
                        if (!L || !L[t]) continue;
                        const float u = (float(x) + 0.5f) / float(W), v = (float(y) + 0.5f) / float(H);
                        if (inInstance(u, v)) put(x, y, 255, 40, 40);
                        else if (terrainAtUV(u, v)) put(x, y, 255, 230, 0);
                        else put(x, y, 255, 0, 255);
                    }
                    const auto rect = [&](float u0, float v0, float u1, float v1, uint8_t r, uint8_t g, uint8_t b)
                    {
                        const int x0 = int((std::min)(u0, u1) * W), x1 = int((std::max)(u0, u1) * W), y0 = int((std::min)(v0, v1) * H), y1 = int((std::max)(v0, v1) * H);
                        for (int x = x0; x <= x1; ++x) { put(uint32_t(x), uint32_t(y0), r, g, b); put(uint32_t(x), uint32_t(y1), r, g, b); }
                        for (int y = y0; y <= y1; ++y) { put(uint32_t(x0), uint32_t(y), r, g, b); put(uint32_t(x1), uint32_t(y), r, g, b); }
                    };
                    for (const InstanceMap& m : g_table) rect(m.tr[0], m.tr[1], m.tr[0] + m.t[0] + m.t[1], m.tr[1] + m.t[2] + m.t[3], 0, 200, 0);
                    for (const TerrainTile& tt : g_terrain) rect(tt.u0, tt.v0, tt.u1, tt.v1, 0, 230, 230);
                    std::string map = getCurrentMapName();
                    if (const size_t sl = map.find_last_of("/\\"); sl != std::string::npos) map = map.substr(sl + 1);
                    const std::string path = getEditorRoot() + "/Enlighten/" + map + "_scene_classes.bmp";
                    std::ofstream f(path, std::ios::binary);
                    const uint32_t rowBytes = W * 3, fileSize = 54 + rowBytes * H;
                    uint8_t hdr[54] = { 'B', 'M' };
                    const auto w32 = [&](int at, uint32_t v) { std::memcpy(hdr + at, &v, 4); };
                    w32(2, fileSize); w32(10, 54); w32(14, 40); w32(18, W); w32(22, H);
                    hdr[26] = 1; hdr[28] = 24;
                    w32(34, rowBytes * H);
                    f.write(reinterpret_cast<const char*>(hdr), 54);
                    f.write(reinterpret_cast<const char*>(img.data()), std::streamsize(img.size()));
                    logger::info("[enlighten] scene: atlas classes -> {}", path);
                }
                for (size_t n = 0; n < 20 && n < order.size() && missing[order[n]]; ++n)
                {
                    const InstanceMap& m = g_table[order[n]];
                    const Work* wk = nullptr;
                    for (const Work& w : g_work) if (w.chart && w.map.hash == m.hash) { wk = &w; break; }
                    if (wk)
                        logger::info("[enlighten] scene:  {} missing in instance {:08x} ({}): geometry {}, {} texels rastered, {} subsets ({} no radiosity uv, {} uv in another stream, {} no stride)",
                            missing[order[n]], m.hash, wk->mesh && wk->mesh->m_Name ? wk->mesh->m_Name : "?", wk->geoState, wk->rastered, wk->subsets, wk->noUv, wk->otherStream, wk->noStride);
                    else
                        logger::info("[enlighten] scene:  {} missing in instance {:08x}: no placed mesh matched", missing[order[n]], m.hash);
                }
            }
            g_nodes.clear();
            g_oriented = false;
            g_phase = Phase::Ready;
            g_progress = 1.0f;
            logger::info("[enlighten] scene: {} texels ({} terrain, {} from LOD 1+ charts, {} snapped to LOD0), {} triangles, {} of {} instances matched",
                g_stats.texels, g_stats.terrainTexels, g_stats.lodTexels, g_stats.lodSnapped, g_stats.triangles, g_stats.matched, g_stats.instances);
        }
    }

    Phase phase() { return g_phase; }
    float progress() { return g_progress.load(); }
    const std::string& error() { return g_error; }
    const Stats& stats() { return g_stats; }

    void registered(uint16_t handle, fb::MeshAsset* mesh, const fb::LinearTransform& world)
    {
        std::lock_guard<std::mutex> lock(g_regMutex);
        if (g_regs.size() < MAX_REGISTRATIONS)
            g_regs.push_back({ handle, mesh, world });
    }

    void clear()
    {
        {
            std::lock_guard<std::mutex> lock(g_regMutex);
            g_regs.clear();
        }
        g_phase = Phase::Idle;
        g_error.clear();
        g_stats = Stats{};
        g_work.clear(); g_table.clear(); g_tris.clear(); g_nodes.clear(); g_right.clear(); g_triOrder.clear();
        g_pos.clear(); g_nrm.clear(); g_cov.clear(); g_texKind.clear(); g_texTri.clear(); g_covList.clear();
        g_terrain.clear(); g_terrainTexels.clear(); g_gridH.clear(); g_skyVis.clear();
    }

    void start()
    {
        std::string err;
        g_error.clear();
        g_stats = Stats{};
        g_progress = 0.0f;
        detail::Original& luma = detail::g_orig[0];
        if (!detail::g_staticAsset || !detail::readback(luma, err) || !luma.width || !luma.height) return fail("shipped atlas not available: " + err);
        W = luma.width;
        H = luma.height;
        if (!readInstanceTable()) return fail("no lightmap instance table in the Enlighten renderer");
        readTerrain();
        readSkyVisibility();

        const size_t n = size_t(W) * H;
        g_pos.assign(n, Vec3{});
        g_nrm.assign(n, Vec3{});
        g_cov.assign(n, 0);
        g_texKind.assign(n, 0);
        g_texTri.assign(n, 0);
        g_covList.clear();
        g_tris.clear();
        g_tris.reserve(1u << 20);
        g_lodTris = 0;
        g_lodSnap.clear();
        g_nodes.clear();
        g_work.clear();
        g_next = 0;

        // addLightMapHandle registrations: exact transform + instance mapping
        std::unordered_set<uint32_t> queued;
#if defined(BFVE_GAME_BF4)
        {
            std::vector<Registration> regs;
            {
                std::lock_guard<std::mutex> lock(g_regMutex);
                regs = g_regs;
            }
            fb::EnlightenRenderer* renderer = fb::EnlightenRenderer::GetInstance();
            const size_t handles = renderer->m_handlesBegin && renderer->m_handlesEnd >= renderer->m_handlesBegin
                ? size_t(renderer->m_handlesEnd - renderer->m_handlesBegin) : 0;
            std::unordered_map<uint16_t, size_t> latest;
            for (size_t i = 0; i < regs.size(); ++i) latest[regs[i].handle] = i;
            uint32_t moved = 0;
            for (const auto& [handle, index] : latest)
            {
                const Registration& rg = regs[index];
                const char* name = rg.mesh && rg.mesh->m_Name ? rg.mesh->m_Name : "?";
                if (rg.handle >= handles || !rg.mesh) { logger::info("[enlighten] scene: handle {} ({}) out of range", rg.handle, name); continue; }
                const fb::EnlightenRenderer::HandleEntry& he = renderer->m_handlesBegin[rg.handle];
                if (!he.m_instance) { logger::info("[enlighten] scene: handle {} ({}) has no instance entry", rg.handle, name); continue; }
                const uint32_t key = he.m_instance->m_hash;
                if (!findInstance(key)) { logger::info("[enlighten] scene: handle {} ({}) entry hash {:08x} not in the table", rg.handle, name, key); continue; }
                if (queued.count(key)) continue;
                if (uint32_t(he.m_hash) != instanceHash(rg.mesh, rg.world))
                {
                    ++moved;
                    logger::info("[enlighten] scene: handle {} hash differs from its registration ({})", rg.handle, rg.mesh->m_Name ? rg.mesh->m_Name : "?");
                }
                InstanceMap m{};
                std::memcpy(m.t, he.m_instance->m_scale, 16);
                std::memcpy(m.tr, he.m_instance->m_offset, 8);
                m.hash = key;
                Work w{ rg.mesh, rg.world, m, 0, true, {}, {}, false };
                w.hasBox = lights::meshBox(rg.mesh, w.mn, w.mx);
                g_work.push_back(w);
                queued.insert(m.hash);
                ++g_stats.instances; ++g_stats.matched; ++g_stats.fromRegistrations;
            }
            if (moved) logger::info("[enlighten] scene: {} registered placements used despite a hash mismatch", moved);
        }
#endif
        // Static charted, Proxy and TerrainProjected only block
        std::unordered_map<std::string, uint32_t> unmatchedStatic;
        lights::forEachWorldMesh([&](const lights::WorldMeshRef& r)
        {
            ++g_stats.instances;
            const int type = r.mesh ? int(r.mesh->m_EnlightenType) : -1;
            bool chart = type == int(fb::EnlightenType_Static) && r.radiosityOverride == 0;
#if defined(BFVE_GAME_BF4)
            bool occlude = chart || type == int(fb::EnlightenType_Proxy) || r.radiosityOverride == 3;
#else
            bool occlude = chart || r.radiosityOverride == 3; // BF3 has no Proxy type
#endif
            if (r.radiosityOverride == 1 || r.radiosityOverride == 2) { chart = false; occlude = false; }
            if (!chart && !occlude) return;
            const uint32_t h = chart ? instanceHash(r.mesh, *r.frame) : 0;
            const InstanceMap* m = h ? findInstance(h) : nullptr;
            if (m && queued.count(m->hash)) return;
            if (m)
            {
                queued.insert(m->hash);
                ++g_stats.matched; ++g_stats.fromPlacements;
                g_work.push_back({ r.mesh, *r.frame, *m, 0, true, r.boxMin, r.boxMax, r.hasBox });
                return;
            }
            if (chart) ++unmatchedStatic[r.mesh->m_Name ? r.mesh->m_Name : "?"];
            if (occlude)
                g_work.push_back({ r.mesh, *r.frame, InstanceMap{}, 0, false, r.boxMin, r.boxMax, r.hasBox });
        });
        for (const auto& [name, count] : unmatchedStatic)
            logger::info("[enlighten] scene: static placement without a lightmap instance: {} x{}", name, count);
        g_sceneMin = vec3(1e30f, 1e30f, 1e30f);
        g_sceneMax = vec3(-1e30f, -1e30f, -1e30f);
        for (const Work& w : g_work)
        {
            if (!w.hasBox) continue;
            Vec3 c[8];
            fb::boxCorners(w.frame, w.mn, w.mx, c);
            for (const Vec3& p : c) { g_sceneMin = fb::vmin(g_sceneMin, p); g_sceneMax = fb::vmax(g_sceneMax, p); }
        }
        if (g_work.empty()) return fail("no placed mesh matched the lightmap instance table");
        uint32_t unmatched = 0;
        for (const InstanceMap& m : g_table)
        {
            if (queued.count(m.hash)) continue;
            ++unmatched;
            const float u0 = m.tr[0], v0 = m.tr[1], u1 = m.tr[0] + m.t[0] + m.t[1], v1 = m.tr[1] + m.t[2] + m.t[3];
            logger::info("[enlighten] scene: instance {:08x} has no mesh, atlas {:.0f},{:.0f} - {:.0f},{:.0f}", m.hash,
                (std::min)(u0, u1) * W, (std::min)(v0, v1) * H, (std::max)(u0, u1) * W, (std::max)(v0, v1) * H);
        }
        logger::info("[enlighten] scene: {} of {} lightmap instances have no mesh ({} registrations seen)", unmatched, g_table.size(), g_regs.size());
        logger::info("[enlighten] scene: {}x{}, {} instances, {} matched ({} registered, {} from placements), {} terrain tiles",
            W, H, g_stats.instances, g_stats.matched, g_stats.fromRegistrations, g_stats.fromPlacements, g_terrain.size());
        g_phase = Phase::Raster;
    }

    void tick()
    {
        if (g_phase != Phase::Raster) return;
        lights::drainGeometryCopies(COPIES_PER_TICK);
        int done = 0;
        const size_t total = g_work.size();
        size_t i = g_next, looked = 0;
        while (done < RASTER_PER_TICK && looked < total)
        {
            Work& w = g_work[i];
            ++looked;
            if (w.tries >= 0)
            {
                const int r = rasterWork(w);
                if (r == 0) { if (++w.tries > GEOMETRY_TRIES) w.tries = -1; }
                else { w.tries = -1; ++done; }
            }
            i = (i + 1) % total;
        }
        g_next = i;
        size_t remaining = 0;
        for (const Work& w : g_work) if (w.tries >= 0) ++remaining;
        g_progress = total ? RASTER_SHARE * (1.0f - float(remaining) / float(total)) : RASTER_SHARE;
        if (remaining) return;

        g_terrainTexels.clear();
        g_terrainNext = 0;
        g_terrainVoted = false;
        g_terrainBody = nullptr;
        g_passedMeshes = 0;
        const detail::Original& luma = detail::g_orig[0];
        const uint16_t* L = luma.pixels.empty() ? nullptr : reinterpret_cast<const uint16_t*>(luma.pixels.data());
        for (size_t t = 0; t < g_cov.size(); ++t)
            if (!g_cov[t] && (!L || L[t] > 0) && terrainAtUV((float(t % W) + 0.5f) / float(W), (float(t / W) + 0.5f) / float(H)))
                g_terrainTexels.push_back(uint32_t(t));
        if (!g_terrain.empty()) setupTerrainGrid();
        if (g_terrainTexels.empty() && g_gridH.empty()) { finish(); return; }
        g_phase = Phase::Terrain;
    }

    void tickGameThread()
    {
        if (g_phase != Phase::Terrain) return;
        const size_t total = g_terrainTexels.size();
        int cast = 0;
        if (!g_terrainVoted)
        {
            g_terrainVoted = true;
            lights::staticMeshBodies(g_meshBodies);
            std::unordered_map<void*, uint32_t> votes;
            uint32_t n = 0;
            for (size_t i = 0; i < TERRAIN_VOTE_SAMPLES && total; ++i)
            {
                const uint32_t t = g_terrainTexels[i * total / TERRAIN_VOTE_SAMPLES];
                const float u = (float(t % W) + 0.5f) / float(W), v = (float(t / W) + 0.5f) / float(H);
                const TerrainTile* te = terrainAtUV(u, v);
                if (!te) continue;
                float x, z;
                te->toWorld(u, v, x, z);
                fb::RayCastHit hit{};
                if (!castTerrain(x, z, TERRAIN_TOP, TERRAIN_BOTTOM, hit, cast)) continue;
                ++votes[hit.m_rigidBody];
                ++n;
            }
            uint32_t best = 0;
            for (const auto& [body, c] : votes) if (c > best) { best = c; g_terrainBody = body; }
            if (best * 10 < n * 6) g_terrainBody = nullptr; // terrain body needs 60% of the samples
            logger::info("[enlighten] scene: {} static mesh bodies skipped by terrain rays, terrain body {} ({} of {} samples)",
                g_meshBodies.size(), g_terrainBody, best, n);
        }
        while (g_terrainNext < total && cast < TERRAIN_RAYS_PER_TICK)
        {
            const uint32_t t = g_terrainTexels[g_terrainNext++];
            ++cast;
            const float u = (float(t % W) + 0.5f) / float(W), v = (float(t / W) + 0.5f) / float(H);
            const TerrainTile* te = terrainAtUV(u, v);
            if (!te) continue;
            float x, z;
            te->toWorld(u, v, x, z);
            fb::RayCastHit hit{};
            if (!castTerrain(x, z, TERRAIN_TOP, TERRAIN_BOTTOM, hit, cast)) continue;
            g_pos[t] = hit.m_position;
            Vec3 nn = fb::normalized(hit.m_normal);
            if (nn.m_y < 0.0f) nn = -nn;
            g_nrm[t] = nn;
            g_cov[t] = 1;
            g_texKind[t] = 1;
            g_texTri[t] = ~0u;
            ++g_stats.terrainTexels;
        }
        while (g_gridNext < g_gridH.size() && cast < TERRAIN_RAYS_PER_TICK)
        {
            const size_t i = g_gridNext++;
            ++cast;
            const float x = g_gridX0 + float(i % g_gridNx) * g_gridStep;
            const float z = g_gridZ0 + float(i / g_gridNx) * g_gridStep;
            fb::RayCastHit hit{};
            if (castTerrain(x, z, g_gridTop, g_gridBottom, hit, cast))
                g_gridH[i] = hit.m_position.m_y;
        }
        const size_t all = total + g_gridH.size();
        g_progress = RASTER_SHARE + TERRAIN_SHARE * (all ? float(g_terrainNext + g_gridNext) / float(all) : 1.0f);
        if (g_terrainNext >= total && g_gridNext >= g_gridH.size())
        {
            logger::info("[enlighten] scene: terrain rays passed through {} static mesh hits", g_passedMeshes);
            finish();
        }
    }

    bool prepare(std::string& why)
    {
        if (g_phase != Phase::Ready) { why = g_phase == Phase::Failed ? g_error : "the scene is not rasterized"; return false; }
        if (g_nodes.empty()) buildBvh();
        orientNormals();
        return true;
    }
    uint32_t width() { return W; }
    uint32_t height() { return H; }
    const Vec3* positions() { return g_pos.data(); }
    const Vec3* normals() { return g_nrm.data(); }
    const uint8_t* coverage() { return g_cov.data(); }
    const uint8_t* kinds() { return g_texKind.data(); }
    const uint32_t* triangles() { return g_texTri.data(); }
    const std::vector<uint32_t>& covered() { return g_covList; }
    bool hitTexel(const Vec3& o, const Vec3& d, float tmax, size_t& texel, float& t)
    {
        Hit h;
        if (!trace(o, d, tmax, false, h)) return false;
        t = h.t;
        const Tri& tr = g_tris[h.tri];
        if (!tr.charted || !texelAt(tr, h.u, h.v, texel)) texel = ~size_t(0);
        return true;
    }
    bool blocked(const Vec3& o, const Vec3& d, float tmax) { Hit h; return trace(o, d, tmax, true, h); }
    bool hitInfo(const Vec3& o, const Vec3& d, float tmax, HitInfo& out)
    {
        out = HitInfo{};
        Hit h;
        if (!trace(o, d, tmax, false, h)) return false;
        const Tri& tr = g_tris[h.tri];
        out.t = h.t;
        out.charted = tr.charted;
        out.terrain = tr.work == ~0u;
        out.mesh = tr.work < g_work.size() ? g_work[tr.work].mesh : nullptr;
        if (!tr.charted || !texelAt(tr, h.u, h.v, out.texel)) out.texel = ~size_t(0);
        return true;
    }
}

