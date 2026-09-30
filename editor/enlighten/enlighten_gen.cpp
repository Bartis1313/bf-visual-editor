#include "enlighten_gen.h"
#include "enlighten_scene.h"
#include "enlighten_db.h"
#include "enlighten_internal.h"
#include "enlighten_dirtable_bf3.h"
#include "../editor_context.h"
#include "../../SDK/fb.h"
#include "../../SDK/offsets.h"
#include "../../utils/log.h"

#include <imgui.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace editor::enlighten::gen
{
    using fb::Vec3;
    using fb::vec3;

    namespace
    {
        Params g_params;
        std::atomic<int> g_state = 0; // 0 idle, 1 running, 2 finished, join pending
        std::atomic<float> g_progress = 0.0f;
        std::atomic<bool> g_cancel = false;
        bool g_waitScene = false;
        std::string g_written;
        std::atomic<uint64_t> g_backRays = 0, g_allRays = 0, g_buried = 0, g_refilled = 0;
        std::mutex g_statusMutex;
        std::string g_status;
        std::thread g_thread;
        std::vector<TexelInfo> g_diag;
        std::atomic<bool> g_diagReady = false;

        void setStatus(const std::string& s)
        {
            std::lock_guard<std::mutex> lock(g_statusMutex);
            g_status = s;
        }

        constexpr float WEIGHT_SCALE = 5.9839302e-8f; // 8-bit solver: weight * coef * scale
        constexpr float WEIGHT_SCALE_BF3 = 1.2014978e-7f; // BF3 SolveIrradiance: weight * (coef - 127) * scale
        constexpr uint32_t BF3_ID_BASE = 0x40000000u;
        constexpr uint32_t BF3_MAGIC = 0xAD105174u; // EnlightenSystem block header
        constexpr uint32_t ENV_RES = 2; // cube texels per face edge
        constexpr uint32_t ENV_COUNT = 24; // resolution 2: 6 faces x 2 x 2
        constexpr int32_t ENV_TREE_NODES = 31; // 24 leaves + 6 face averages + root
        constexpr uint32_t ENV_SYSTEM_ID = 0xFFFFFFFEu; // ENVIRONMENT_SYSTEM_ID
        constexpr uint16_t TERRAIN_MATERIAL = 12345; // updateMaterials gives it the VE terrain color

        constexpr uint32_t MAGIC_TRANSPORT = 0x47534547u; // 'GESG'
        constexpr uint32_t TRANSPORT_VERSION = 30;
        constexpr uint32_t TRANSPORT_VERSION_BF3 = 22; // RadiositySystem v22
        constexpr uint32_t MAGIC_WORKSPACE = 0x57494547u; // 'GEIW' InputWorkspace
        constexpr uint32_t MAGIC_TREE = 0x47445447u; // 'GTDG' InputTree
        constexpr uint32_t TREE_VERSION = 7;
        constexpr uint32_t MAGIC_VISIBILITY = 0x53564547u; // 'GEVS'
        constexpr int32_t VISIBILITY_VERSION = 4;
        constexpr int32_t VISIBILITY_HEADER = 320; // header bytes, block offsets follow
        constexpr int MAX_VIS_SLICES = 64; // header holds 64 u16 start + 64 u16 length
        constexpr uint32_t MAGIC_PROBE_TEXELS = 0x42525045u; // 'EPRB' <map>_probes.bin
        constexpr uint32_t MAGIC_BF3_FILE = 0x53334245u; // 'EB3S' <map>_generated.bf3edb

        // cosine rays from each texel: origin lifted off the surface, hits facing the ray are back faces
        constexpr float RAY_BIAS = 0.05f; // meters along the texel normal
        constexpr float BACK_FACE_DOT = 0.05f; // dot(ray, hit normal) above = back face
        constexpr float TRANSPORT_RANGE = 3000.0f; // meters, a miss = environment
        constexpr int MIN_RAYS = 8;
        constexpr float SUN_RANGE = 4000.0f; // GEVS rays
        constexpr int SUN_PASS_HITS = 4; // back faces a sun ray may pass through
        constexpr float SUN_PASS_STEP = 0.02f; // meters past each passed hit
        constexpr int PROBE_RAYS = 64; // fibonacci sphere
        constexpr float GOLDEN_ANGLE = 2.399963f; // pi * (3 - sqrt 5)
        constexpr float PROBE_RANGE = 300.0f; // meters

        // coverage (Raster::cov)
        constexpr uint8_t COV_SURFACE = 1, COV_GUTTER = 2, COV_FILL = 3;
        constexpr uint8_t KIND_TERRAIN = 1; // scene::kinds(), 0 mesh
        constexpr int GUTTER_RINGS = 2; // dilated around charts, solved as pixels so bilinear never reads 0
        constexpr uint32_t LOD_TRIANGLE = 0x80000000u; // scene triangle id flag: LOD 1+ chart

        constexpr uint32_t BUCKET = 4; // 4x4 pixels per transport bucket, one bounce pixel per 2x2 quad
        constexpr uint32_t GROUP_CLUSTERS = 32; // clusters per workspace group bounds
        constexpr int MIN_BASIS = 16; // solver reads basis in blocks of 16

        // EnlightenCubeMapTable directions
        const float kEnvDirs[ENV_COUNT][3] = {
            { 0.816f, 0.408f, 0.408f }, { 0.816f, 0.408f, -0.408f }, { 0.816f, -0.408f, 0.408f }, { 0.816f, -0.408f, -0.408f },
            { -0.816f, 0.408f, -0.408f }, { -0.816f, 0.408f, 0.408f }, { -0.816f, -0.408f, -0.408f }, { -0.816f, -0.408f, 0.408f },
            { -0.408f, 0.816f, -0.408f }, { 0.408f, 0.816f, -0.408f }, { -0.408f, 0.816f, 0.408f }, { 0.408f, 0.816f, 0.408f },
            { -0.408f, -0.816f, 0.408f }, { 0.408f, -0.816f, 0.408f }, { -0.408f, -0.816f, -0.408f }, { 0.408f, -0.816f, -0.408f },
            { -0.408f, 0.408f, 0.816f }, { 0.408f, 0.408f, 0.816f }, { -0.408f, -0.408f, 0.816f }, { 0.408f, -0.408f, 0.816f },
            { 0.408f, 0.408f, -0.816f }, { -0.408f, 0.408f, -0.816f }, { 0.408f, -0.408f, -0.816f }, { -0.408f, -0.408f, -0.816f } };

        struct Writer
        {
            std::vector<uint8_t> b;
            size_t pos() const { return b.size(); }
            void align(size_t a) { while (b.size() % a) b.push_back(0); }
            size_t zeros(size_t n) { const size_t p = b.size(); b.resize(p + n, 0); return p; }
            size_t raw(const void* d, size_t n) { const size_t p = b.size(); b.resize(p + n); std::memcpy(&b[p], d, n); return p; }
            template <class T> size_t put(const T& v) { return raw(&v, sizeof(T)); }
            template <class T> void at(size_t p, const T& v) { std::memcpy(&b[p], &v, sizeof(T)); }
        };

        struct Cluster
        {
            std::vector<uint32_t> tex;
            Vec3 mn, mx, nrm, center;
            uint8_t nb[3] = {};
            bool terrain = false;
            uint32_t quadStart = 0;
        };

        struct Sys
        {
            uint32_t index = 0, x0 = 0, y0 = 0, w = 0, h = 0;
            std::vector<uint32_t> pixels;
            std::vector<Cluster> clusters;
            Vec3 mn, mx;
        };

        uint8_t normalByte(float n) { return uint8_t((std::clamp)(int(std::lround((1.0f - n) * 127.0f)), 0, 254)); }
        uint8_t unitByte(float n) { return uint8_t((std::clamp)(int(std::lround((n + 1.0f) * 127.5f)), 0, 255)); }

        void parallelFor(size_t count, const std::function<void(size_t, size_t)>& fn)
        {
            const size_t workers = (std::max)(1u, std::thread::hardware_concurrency() - 1);
            const size_t chunk = (count + workers - 1) / workers;
            std::vector<std::thread> pool;
            for (size_t w = 0; w < workers; ++w)
            {
                const size_t b = w * chunk, e = (std::min)(count, b + chunk);
                if (b >= e) break;
                pool.emplace_back([=, &fn] { fn(b, e); });
            }
            for (auto& t : pool) t.join();
        }

        Vec3 cosineDir(const Vec3& n, float u1, float u2) { return math::cosineDir(n, u1, u2); }

        // lowbias32 (Wellons)
        uint32_t hash32(uint32_t x) { x ^= x >> 16; x *= 0x7FEB352Du; x ^= x >> 15; x *= 0x846CA68Bu; x ^= x >> 16; return x; }

        // per-texel ray sequence, shared by the transport and traceTexel so both cast the same rays
        // u1 stratified by ray index, u2 golden-ratio sequence with a random rotation
        struct RaySeq
        {
            uint32_t state;
            float rot;
            explicit RaySeq(uint32_t texel) : state(hash32(texel * 0x9E3779B1u + 17)) { rot = next(); } // 2^32 / golden ratio
            float next() { state = state * 1664525u + 1013904223u; return (state >> 8) * (1.0f / 16777216.0f); } // LCG, top 24 bits
            Vec3 dir(const Vec3& n, int r, int R)
            {
                const float u1 = (float(r) + next()) / float(R);
                float u2 = float(r) * 0.6180339887f + rot; u2 -= std::floor(u2);
                return cosineDir(n, u1, u2);
            }
        };

        int rayCount() { return (std::max)(MIN_RAYS, g_params.pixelRays); }

        void probeDirs(Vec3 (&dirs)[PROBE_RAYS])
        {
            for (int i = 0; i < PROBE_RAYS; ++i)
            {
                const float y = 1.0f - 2.0f * (i + 0.5f) / PROBE_RAYS;
                const float rr = std::sqrt((std::max)(0.0f, 1.0f - y * y));
                dirs[i] = vec3(rr * std::cos(GOLDEN_ANGLE * i), y, rr * std::sin(GOLDEN_ANGLE * i));
            }
        }

        // 10 bits per axis
        uint32_t morton(uint32_t x, uint32_t y, uint32_t z)
        {
            const auto spread = [](uint32_t v) { v &= 0x3FF; v = (v | v << 16) & 0x030000FF; v = (v | v << 8) & 0x0300F00F; v = (v | v << 4) & 0x030C30C3; v = (v | v << 2) & 0x09249249; return v; };
            return spread(x) | spread(y) << 1 | spread(z) << 2;
        }

        // cov: 0 empty, 1 surface, 2 gutter, 3 fill with sky-only transport
        struct Raster
        {
            std::vector<Vec3> pos, nrm;
            std::vector<uint8_t> cov, kind;
        } g_raster;

        // shipped direction atlas: RGBA8, rgb = dir * 0.5 + 0.5
        std::vector<uint8_t> g_dirAtlas;
        uint32_t g_dirW = 0, g_dirH = 0;

        uint32_t orientByShippedDirection()
        {
            namespace sc = scene;
            const uint32_t W = sc::width(), H = sc::height();
            if (g_dirAtlas.empty() || g_dirW != W || g_dirH != H) return 0;
            const uint32_t* tri = sc::triangles();
            constexpr float MIN_DIR_LENGTH = 0.3f; // shorter = no dominant incoming direction, no vote
            std::unordered_map<uint32_t, float> vote;
            for (size_t t = 0; t < g_raster.cov.size(); ++t)
            {
                if (g_raster.cov[t] != COV_SURFACE || g_raster.kind[t] != 0 || tri[t] == ~0u) continue;
                const uint8_t* d = &g_dirAtlas[t * 4];
                const Vec3 dir = vec3(d[0] / 127.5f - 1.0f, d[1] / 127.5f - 1.0f, d[2] / 127.5f - 1.0f);
                const float len = fb::length(dir);
                if (len < MIN_DIR_LENGTH) continue;
                vote[tri[t]] += fb::dot(g_raster.nrm[t], dir) / len;
            }
            uint32_t flipped = 0;
            for (size_t t = 0; t < g_raster.cov.size(); ++t)
            {
                if (g_raster.cov[t] != COV_SURFACE || g_raster.kind[t] != 0 || tri[t] == ~0u) continue;
                const auto it = vote.find(tri[t]);
                if (it == vote.end() || it->second >= 0.0f) continue;
                g_raster.nrm[t] = -g_raster.nrm[t];
                ++flipped;
            }
            return flipped;
        }

        void buildRaster(int rings)
        {
            namespace sc = scene;
            const uint32_t W = sc::width(), H = sc::height();
            const size_t n = size_t(W) * H;
            g_raster.pos.assign(sc::positions(), sc::positions() + n);
            g_raster.nrm.assign(sc::normals(), sc::normals() + n);
            g_raster.cov.assign(sc::coverage(), sc::coverage() + n);
            g_raster.kind.assign(sc::kinds(), sc::kinds() + n);
            for (uint8_t& c : g_raster.cov) c = c ? COV_SURFACE : 0;
            if (const uint32_t flipped = orientByShippedDirection())
                logger::info("[enlighten] generator: {} texel normals flipped to face the shipped direction atlas", flipped);
            static constexpr int ORDER[8][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 } };
            for (int r = 0; r < rings; ++r)
            {
                std::vector<uint32_t> from(n, ~0u);
                for (uint32_t y = 0; y < H; ++y)
                    for (uint32_t x = 0; x < W; ++x)
                    {
                        const size_t t = size_t(y) * W + x;
                        if (g_raster.cov[t]) continue;
                        for (const auto& o : ORDER)
                        {
                            const int nx = int(x) + o[0], ny = int(y) + o[1];
                            if (nx < 0 || ny < 0 || nx >= int(W) || ny >= int(H)) continue;
                            const size_t s = size_t(ny) * W + nx;
                            if (g_raster.cov[s]) { from[t] = uint32_t(s); break; }
                        }
                    }
                for (size_t t = 0; t < n; ++t)
                {
                    if (from[t] == ~0u) continue;
                    g_raster.pos[t] = g_raster.pos[from[t]];
                    g_raster.nrm[t] = g_raster.nrm[from[t]];
                    g_raster.kind[t] = g_raster.kind[from[t]];
                    g_raster.cov[t] = COV_GUTTER;
                }
            }
            const detail::Original& luma = detail::g_orig[0];
            if (luma.width == W && luma.height == H && luma.bpp == 2)
            {
                const uint16_t* L = reinterpret_cast<const uint16_t*>(luma.pixels.data());
                size_t filled = 0;
                for (size_t t = 0; t < n; ++t)
                    if (!g_raster.cov[t] && L[t])
                    {
                        g_raster.cov[t] = COV_FILL;
                        g_raster.nrm[t] = vec3(0.0f, 1.0f, 0.0f);
                        g_raster.kind[t] = 0;
                        ++filled;
                    }
                logger::info("[enlighten] generator: {} texels the shipped atlas lights have no rasterized mesh (sky-only fill)", filled);
            }
            // <map>_coverage.bin: u32 w, u32 h, u8[w*h] class
            enum : uint8_t { kClassEmpty, kClassLod0, kClassLod1, kClassTerrain, kClassGutter, kClassFill };
            std::vector<uint8_t> cls(n, kClassEmpty);
            const uint32_t* tri = sc::triangles();
            for (size_t t = 0; t < n; ++t)
            {
                const uint8_t c = g_raster.cov[t];
                if (c == COV_GUTTER) cls[t] = kClassGutter;
                else if (c == COV_FILL) cls[t] = kClassFill;
                else if (c == COV_SURFACE) cls[t] = g_raster.kind[t] == KIND_TERRAIN ? kClassTerrain : tri[t] != ~0u && (tri[t] & LOD_TRIANGLE) ? kClassLod1 : kClassLod0;
            }
            std::ofstream f(db::directory() + "/" + db::mapFileName() + "_coverage.bin", std::ios::binary);
            f.write(reinterpret_cast<const char*>(&W), 4);
            f.write(reinterpret_cast<const char*>(&H), 4);
            f.write(reinterpret_cast<const char*>(cls.data()), std::streamsize(n));
        }

        std::string g_calibNote;

        void buildSystems(std::vector<Sys>& out, std::vector<int32_t>& texSys, std::vector<int32_t>& texCluster)
        {
            namespace sc = scene;
            constexpr int MIN_TILE = 8; // texels
            constexpr float MIN_CLUSTER_SIZE = 0.25f; // meters
            constexpr size_t MIN_SURFACE = 4; // one quad, else the tile is skipped
            constexpr float FILL_BOX = 16384.0f; // fill-only system box, the list builder re-solves it off-screen
            const uint32_t W = sc::width(), H = sc::height(), T = uint32_t((std::max)(MIN_TILE, g_params.tile));
            const Vec3* pos = g_raster.pos.data();
            const Vec3* nrm = g_raster.nrm.data();
            const uint8_t* cov = g_raster.cov.data();
            const uint8_t* kind = g_raster.kind.data();
            texSys.assign(size_t(W) * H, -1);
            texCluster.assign(size_t(W) * H, -1);
            const float cs = (std::max)(MIN_CLUSTER_SIZE, g_params.clusterSize);
            for (uint32_t ty = 0; ty < H; ty += T)
                for (uint32_t tx = 0; tx < W; tx += T)
                {
                    Sys s;
                    s.x0 = tx; s.y0 = ty; s.w = (std::min)(T, W - tx); s.h = (std::min)(T, H - ty);
                    for (uint32_t y = ty; y < ty + s.h; ++y)
                        for (uint32_t x = tx; x < tx + s.w; ++x)
                            if (cov[size_t(y) * W + x]) s.pixels.push_back(y * W + x);
                    size_t surface = 0, fill = 0;
                    for (uint32_t t : s.pixels) { surface += cov[t] == COV_SURFACE; fill += cov[t] == COV_FILL; }
                    if (surface < MIN_SURFACE && !fill) continue;
                    s.index = uint32_t(out.size());

                    // cluster = cs-meter cell x dominant normal axis x terrain
                    // key: cell x, y, z 20 bits each at 44 / 24 / 4, axis 0..5 in bits 0-2, terrain bit 3
                    std::unordered_map<uint64_t, uint32_t> keyed;
                    for (uint32_t t : s.pixels)
                    {
                        if (cov[t] != COV_SURFACE)
                        {
                            texSys[t] = int32_t(s.index);
                            continue;
                        }
                        const Vec3& p = pos[t];
                        const Vec3& n = nrm[t];
                        const float ax = std::fabs(n.m_x), ay = std::fabs(n.m_y), az = std::fabs(n.m_z);
                        const int axis = ax >= ay && ax >= az ? (n.m_x >= 0 ? 0 : 1) : ay >= az ? (n.m_y >= 0 ? 2 : 3) : (n.m_z >= 0 ? 4 : 5);
                        const uint64_t k = (uint64_t(uint32_t(int(std::floor(p.m_x / cs))) & 0xFFFFF) << 44) | (uint64_t(uint32_t(int(std::floor(p.m_y / cs))) & 0xFFFFF) << 24) |
                            (uint64_t(uint32_t(int(std::floor(p.m_z / cs))) & 0xFFFFF) << 4) | uint64_t(axis) | (kind[t] ? 8 : 0);
                        auto [it, ins] = keyed.try_emplace(k, uint32_t(s.clusters.size()));
                        if (ins) { s.clusters.emplace_back(); s.clusters.back().terrain = kind[t] != 0; }
                        s.clusters[it->second].tex.push_back(t);
                    }
                    if (s.clusters.empty())
                    {
                        for (uint32_t t : s.pixels) if (cov[t] == COV_FILL) { s.clusters.emplace_back(); s.clusters.back().tex.push_back(t); break; } // unused placeholder
                    }
                    s.mn = vec3(1e30f, 1e30f, 1e30f); s.mx = vec3(-1e30f, -1e30f, -1e30f);
                    for (Cluster& c : s.clusters)
                    {
                        c.mn = vec3(1e30f, 1e30f, 1e30f); c.mx = vec3(-1e30f, -1e30f, -1e30f);
                        Vec3 ns{};
                        for (uint32_t t : c.tex) { c.mn = fb::vmin(c.mn, pos[t]); c.mx = fb::vmax(c.mx, pos[t]); ns += nrm[t]; }
                        c.nrm = fb::normalized(ns);
                        if (fb::lengthSq(c.nrm) < 0.5f) c.nrm = nrm[c.tex[0]]; // normals cancelled out
                        c.center = (c.mn + c.mx) * 0.5f;
                        c.nb[0] = normalByte(c.nrm.m_x); c.nb[1] = normalByte(c.nrm.m_y); c.nb[2] = normalByte(c.nrm.m_z);
                        std::sort(c.tex.begin(), c.tex.end());
                        while (c.tex.size() % 4) c.tex.push_back(c.tex.back()); // samples are packed as quads
                        s.mn = fb::vmin(s.mn, c.mn); s.mx = fb::vmax(s.mx, c.mx);
                    }
                    if (!surface)
                    {
                        s.mn = vec3(-FILL_BOX, -FILL_BOX, -FILL_BOX);
                        s.mx = vec3(FILL_BOX, FILL_BOX, FILL_BOX);
                    }
                    // clusters in morton order of their centres, 10 bits per axis
                    const Vec3 ext = s.mx - s.mn;
                    const auto q = [&](float v, float lo, float e) { return uint32_t((std::clamp)((v - lo) / (std::max)(e, 1e-3f), 0.0f, 1.0f) * 1023.0f); };
                    std::sort(s.clusters.begin(), s.clusters.end(), [&](const Cluster& a, const Cluster& b)
                    {
                        return morton(q(a.center.m_x, s.mn.m_x, ext.m_x), q(a.center.m_y, s.mn.m_y, ext.m_y), q(a.center.m_z, s.mn.m_z, ext.m_z)) <
                               morton(q(b.center.m_x, s.mn.m_x, ext.m_x), q(b.center.m_y, s.mn.m_y, ext.m_y), q(b.center.m_z, s.mn.m_z, ext.m_z));
                    });
                    uint32_t quad = 0;
                    for (uint32_t ci = 0; ci < s.clusters.size(); ++ci)
                    {
                        Cluster& c = s.clusters[ci];
                        c.quadStart = quad;
                        quad += uint32_t(c.tex.size() / 4);
                        for (uint32_t t : c.tex) { texSys[t] = int32_t(s.index); texCluster[t] = int32_t(ci); }
                    }
                    out.push_back(std::move(s));
                }
        }

        size_t writeWorkspace(Writer& w, const Sys& s, uint32_t& length)
        {
            namespace sc = scene;
            const uint32_t W = sc::width();
            const Vec3* pos = g_raster.pos.data();
            const uint32_t nc = uint32_t(s.clusters.size());
            const uint32_t ng = (nc + GROUP_CLUSTERS - 1) / GROUP_CLUSTERS;
            uint32_t quads = 0;
            for (const Cluster& c : s.clusters) quads += uint32_t(c.tex.size() / 4);

            w.align(128);
#if defined(BFVE_GAME_BF3)
            const size_t base = w.zeros(0x90); // + SystemId, InputWorkspaceSize
#else
            const size_t base = w.zeros(0x80);
#endif
            const auto rel = [&](size_t p) { return int32_t(p - base); };

            // packed quads 16 B: 4 x u8 xyz in the cluster box, bytes 3/7/11 cluster normal, byte 15 0xFF
            const size_t packed = w.pos();
            for (const Cluster& c : s.clusters)
            {
                const Vec3 e = c.mx - c.mn;
                const auto qz = [](float v, float lo, float ext) { return uint8_t(ext > 1e-6f ? (std::clamp)(int(std::lround((v - lo) / ext * 255.0f)), 0, 255) : 0); };
                for (size_t k = 0; k < c.tex.size(); k += 4)
                {
                    uint8_t qd[16];
                    for (int d = 0; d < 4; ++d)
                    {
                        const Vec3& p = pos[c.tex[k + d]];
                        qd[4 * d + 0] = qz(p.m_x, c.mn.m_x, e.m_x);
                        qd[4 * d + 1] = qz(p.m_y, c.mn.m_y, e.m_y);
                        qd[4 * d + 2] = qz(p.m_z, c.mn.m_z, e.m_z);
                    }
                    qd[3] = c.nb[0]; qd[7] = c.nb[1]; qd[11] = c.nb[2]; qd[15] = 0xFF;
                    w.raw(qd, 16);
                }
            }
            // u16 per quad: its pixel in the half-res bounce buffer
            const size_t bounce = w.pos();
#if defined(BFVE_GAME_BF3)
            const uint32_t bounceStride = ((s.w >> 1) + 1) & ~1u; // SolveIrradiance bounce row
#else
            const uint32_t bounceStride = s.w >> 1;
#endif
            for (const Cluster& c : s.clusters)
                for (size_t k = 0; k < c.tex.size(); k += 4)
                {
                    const uint32_t t = c.tex[k];
                    const uint32_t lx = t % W - s.x0, ly = t / W - s.y0;
                    w.put(uint16_t((lx >> 1) + bounceStride * (ly >> 1)));
                }
            w.align(16);
            // bounds 32 B: min.xyz + normal-cone low bytes in .w, max.xyz + high bytes in .w
            const auto packBounds = [&](const Vec3& mn, const Vec3& mx, const uint8_t (&lo)[3], const uint8_t (&hi)[3])
            {
                float a[4] = { mn.m_x, mn.m_y, mn.m_z, 0.0f }, b[4] = { mx.m_x, mx.m_y, mx.m_z, 0.0f };
                uint8_t wa[4] = { lo[0], lo[1], lo[2], 0 }, wb[4] = { hi[0], hi[1], hi[2], 0 };
                std::memcpy(&a[3], wa, 4); std::memcpy(&b[3], wb, 4);
                w.raw(a, 16); w.raw(b, 16);
            };
            const size_t dusterBounds = w.pos();
            for (const Cluster& c : s.clusters) packBounds(c.mn, c.mx, c.nb, c.nb);
            const size_t groupBounds = w.pos();
            for (uint32_t g = 0; g < ng; ++g)
            {
                Vec3 mn = vec3(1e30f, 1e30f, 1e30f), mx = vec3(-1e30f, -1e30f, -1e30f);
                uint8_t lo[3] = { 255, 255, 255 }, hi[3] = { 0, 0, 0 };
                for (uint32_t ci = GROUP_CLUSTERS * g; ci < (std::min)(nc, GROUP_CLUSTERS * g + GROUP_CLUSTERS); ++ci)
                {
                    const Cluster& c = s.clusters[ci];
                    mn = fb::vmin(mn, c.mn); mx = fb::vmax(mx, c.mx);
                    for (int k = 0; k < 3; ++k) { lo[k] = (std::min)(lo[k], c.nb[k]); hi[k] = (std::max)(hi[k], c.nb[k]); }
                }
                packBounds(mn, mx, lo, hi);
            }
            std::vector<uint32_t> start(nc + 1, 0);
            for (uint32_t ci = 0; ci < nc; ++ci) start[ci + 1] = start[ci] + uint32_t(s.clusters[ci].tex.size() / 4);
            const size_t groupQuads = w.pos();
            uint32_t maxGroup = 0;
            for (uint32_t g = 0; g < ng; ++g)
            {
                const uint32_t a = start[GROUP_CLUSTERS * g], b = start[(std::min)(nc, GROUP_CLUSTERS * g + GROUP_CLUSTERS)];
                maxGroup = (std::max)(maxGroup, b - a);
                w.put(a); w.put(b); w.put(uint32_t(0)); w.put(uint32_t(0));
            }
            const size_t nodeIdx = w.pos();
            for (uint32_t v : start) w.put(v);
            w.align(16);
            // flat tree: every cluster a leaf, no nodes
            const size_t treeRange = w.pos();
            w.put(int32_t(0)); w.put(int32_t(nc - 1)); w.put(int32_t(nc)); w.put(int32_t(nc));
            const size_t tree = w.pos();
            w.put(MAGIC_TREE); w.put(TREE_VERSION); w.put(uint32_t(nc)); w.put(uint32_t(0));
            for (const Cluster& c : s.clusters) w.put(uint8_t((std::min)(size_t(255), c.tex.size() / 4)));
            w.align(16);

            float bmin[4] = { s.mn.m_x, s.mn.m_y, s.mn.m_z, 0.0f }, bmax[4] = { s.mx.m_x, s.mx.m_y, s.mx.m_z, 0.0f };
            std::memcpy(&w.b[base], bmin, 16);
            std::memcpy(&w.b[base + 16], bmax, 16);
            // samples, clusters, clusters in tree, section offsets, max quads per group, bounce cache -1 / 0
            const int32_t fields[] = {
                int32_t(quads * 4), int32_t(nc), int32_t(nc), rel(packed), rel(bounce), rel(dusterBounds), rel(groupBounds), rel(groupQuads),
                rel(nodeIdx), rel(tree), int32_t(maxGroup), -1, 0, 0, 0, 0, 0, 0, 1, rel(treeRange), int32_t(nc - 1) };
            length = uint32_t(w.pos() - base);
#if defined(BFVE_GAME_BF3)
            w.at(base + 0x28, uint32_t(BF3_ID_BASE + s.index)); // SystemId {0, id}
            w.at(base + 0x30, MAGIC_WORKSPACE);
            w.at(base + 0x34, length);
            std::memcpy(&w.b[base + 0x38], fields, sizeof(fields));
#else
            w.at(base + 0x20, MAGIC_WORKSPACE);
            std::memcpy(&w.b[base + 0x24], fields, sizeof(fields));
#endif
            return base;
        }

        size_t writeVisibility(Writer& w, const Sys& s)
        {
            namespace sc = scene;
            const Vec3* pos = g_raster.pos.data();
            const Vec3* nrm = g_raster.nrm.data();
            constexpr int MIN_VIS_SLICES = 4;
            constexpr float LEVELS = 3.0f; // 2-bit visibility 0..3, 4 clusters per byte
            constexpr double RING_EPS = 1e-9; // sin(pi/6) lands on a whole ring length, keep it whole
            // n polar slices from +Y, slice i has 1 + 2(n-1) sin(theta) directions toward the light
            const int n = (std::clamp)(g_params.visSlices, MIN_VIS_SLICES, MAX_VIS_SLICES);
            std::vector<uint16_t> len(n), idx(n);
            uint32_t count = 0;
            for (int i = 0; i < n; ++i)
            {
                idx[i] = uint16_t(count);
                len[i] = uint16_t(1 + int(2.0 * (n - 1) * std::sin(math::PI_D * i / (n - 1)) + RING_EPS));
                count += len[i];
            }
            std::vector<Vec3> dirs;
            for (int i = 0; i < n; ++i)
            {
                const float th = math::PI * float(i) / float(n - 1);
                for (int j = 0; j < len[i]; ++j)
                {
                    const float ph = math::PI_2 * float(j) / float(len[i]) - math::PI;
                    dirs.push_back(vec3(std::cos(ph) * std::sin(th), -std::cos(th), -std::sin(ph) * std::sin(th)));
                }
            }
            const uint32_t nc = uint32_t(s.clusters.size());
            const uint32_t per = (nc + 3) / 4;
            std::vector<uint8_t> vis(size_t(count) * per, 0);
            const int S = (std::max)(1, g_params.visSamples);
            parallelFor(size_t(count) * nc, [&](size_t b, size_t e)
            {
                for (size_t k = b; k < e && !g_cancel.load(); ++k)
                {
                    const uint32_t d = uint32_t(k / nc), ci = uint32_t(k % nc);
                    const Cluster& c = s.clusters[ci];
                    int seen = 0, tried = 0;
                    for (int j = 0; j < S; ++j)
                    {
                        const uint32_t t = c.tex[(size_t(j) * c.tex.size()) / S];
                        ++tried;
                        if (fb::dot(nrm[t], dirs[d]) <= 0.0f) continue;
                        Vec3 o = pos[t] + nrm[t] * RAY_BIAS;
                        if (!sc::blocked(o, dirs[d], SUN_RANGE)) { ++seen; continue; }
                        // back faces do not block the sun, step through them
                        bool open = false;
                        for (int step = 0; step < SUN_PASS_HITS; ++step)
                        {
                            scene::HitInfo h;
                            if (!sc::hitInfo(o, dirs[d], SUN_RANGE, h)) { open = true; break; }
                            if (h.texel == ~size_t(0) || fb::dot(dirs[d], nrm[h.texel]) <= BACK_FACE_DOT) break;
                            o = o + dirs[d] * (h.t + SUN_PASS_STEP);
                        }
                        seen += open;
                    }
                    const uint8_t v = uint8_t(std::lround(LEVELS * float(seen) / float((std::max)(tried, 1))));
                    vis[size_t(d) * per + ci / 4] |= uint8_t(v << (2 * (ci % 4)));
                }
            });
            // RLE: c <= 127 = c literal bytes, c >= 128 = next byte repeated c - 128 times (runs of 3+)
            const auto rle = [](const uint8_t* data, size_t size, std::vector<uint8_t>& out)
            {
                std::vector<uint8_t> lit;
                const auto flush = [&] { while (!lit.empty()) { const size_t c = (std::min)(size_t(127), lit.size()); out.push_back(uint8_t(c)); out.insert(out.end(), lit.begin(), lit.begin() + c); lit.erase(lit.begin(), lit.begin() + c); } };
                size_t i = 0;
                while (i < size)
                {
                    size_t j = i;
                    while (j < size && data[j] == data[i] && j - i < 127) ++j;
                    if (j - i >= 3) { flush(); out.push_back(uint8_t(128 + (j - i))); out.push_back(data[i]); i = j; }
                    else lit.push_back(data[i++]);
                }
                flush();
            };
            const uint32_t perBlock = 4; // directions per RLE block
            const uint32_t blocks = (count + perBlock - 1) / perBlock;
            std::vector<uint32_t> info(blocks);
            std::vector<uint8_t> data;
            for (uint32_t k = 0; k < blocks; ++k)
            {
                info[k] = uint32_t(data.size());
                const uint32_t d0 = k * perBlock, d1 = (std::min)(count, d0 + perBlock);
                rle(&vis[size_t(d0) * per], size_t(d1 - d0) * per, data);
            }
            w.align(16);
            const size_t base = w.pos();
            // slices, values per byte, block table bytes + offset, data bytes + offset, blocks, dirs per block, slices, dirs
            w.put(MAGIC_VISIBILITY); w.put(VISIBILITY_VERSION); w.zeros(16);
            w.put(int32_t(n)); w.put(int32_t(4)); w.put(int32_t(4 * blocks)); w.put(VISIBILITY_HEADER);
            w.put(int32_t(data.size())); w.put(int32_t(VISIBILITY_HEADER + 4 * blocks)); w.put(int32_t(blocks)); w.put(int32_t(perBlock));
            w.put(int32_t(n)); w.put(int32_t(count));
            for (int i = 0; i < MAX_VIS_SLICES; ++i) w.put(uint16_t(i < n ? idx[i] : 0));
            for (int i = 0; i < MAX_VIS_SLICES; ++i) w.put(uint16_t(i < n ? len[i] : 0));
            for (uint32_t v : info) w.put(v);
            w.raw(data.data(), data.size());
            return base;
        }

        size_t writeAlbedo(Writer& w, const Sys& s)
        {
            const uint32_t nc = uint32_t(s.clusters.size());
            w.align(16);
            const size_t base = w.pos();
            // ClusterAlbedoWorkspaceMaterialData: size, materials, clusters, then offsets of
            // ids 0x30, colors 0x40, emissive 0x60, per-cluster material count 0x80, (weight, material) pairs
            const uint32_t idxOff = (0x80 + 2 * nc + 15) & ~15u;
            const uint32_t size = idxOff + 4 * nc;
            const uint32_t hdr[8] = { size, 2, nc, 0x30, 0x40, 0x60, 0x80, idxOff };
            w.raw(hdr, sizeof(hdr));
            w.zeros(16);
            w.put(uint64_t(0)); w.put(uint64_t(TERRAIN_MATERIAL));
            const float col[8] = { 0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f }; // placeholder, the engine fills colors by id
            w.raw(col, sizeof(col));
            w.zeros(32);
            for (uint32_t ci = 0; ci < nc; ++ci) w.put(uint16_t(1));
            while (w.pos() - base < idxOff) w.put(uint8_t(0));
            for (const Cluster& c : s.clusters) { w.put(uint16_t(c.tex.size())); w.put(uint16_t(0)); } // weight = sample count, material 0
            return base;
        }

        struct Ref { uint64_t key; uint32_t count; Vec3 dir; };
        struct Bucket { std::vector<uint32_t> px; uint32_t nBounce = 0; };
        constexpr uint64_t ENV_KEY = 0xFFFFFFFF00000000ull; // ref key: env | direction, else system << 32 | cluster

        // nearest of the 256 solver directions, entry 0 = none
        uint8_t paletteIndex(const Vec3& d, const Vec3* dirTable)
        {
            uint32_t best = 0; float bd = -2.0f;
            for (uint32_t i = 1; i < 256; ++i)
            {
                const float dd = fb::dot(d, dirTable[i]);
                if (dd > bd) { bd = dd; best = i; }
            }
            return uint8_t(best);
        }

        // u8 per 2x2 quad: which of the 8 neighbours carry directional data (bits L R U D LU LD RU RD)
        std::vector<uint8_t> quadMaskOf(const Sys& s, const std::vector<Bucket>& buckets, uint32_t W)
        {
            const uint32_t qw = s.w >> 1, qh = s.h >> 1;
            std::vector<uint8_t> hasDir(size_t(qw) * qh, 0);
            for (const Bucket& b : buckets)
                for (uint32_t p = 0; p < b.nBounce; ++p)
                {
                    const uint32_t qx = (b.px[p] % W - s.x0) >> 1, qy = (b.px[p] / W - s.y0) >> 1;
                    if (qx < qw && qy < qh) hasDir[qy * qw + qx] = 1;
                }
            std::vector<uint8_t> mask(hasDir.size(), 0);
            static constexpr int NEIGHBOUR[8][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
            for (uint32_t qy = 0; qy < qh; ++qy)
                for (uint32_t qx = 0; qx < qw; ++qx)
                {
                    if (!hasDir[qy * qw + qx]) continue;
                    uint8_t m = 0;
                    for (int k = 0; k < 8; ++k)
                    {
                        const int nx = int(qx) + NEIGHBOUR[k][0], ny = int(qy) + NEIGHBOUR[k][1];
                        if (nx >= 0 && ny >= 0 && nx < int(qw) && ny < int(qh) && hasDir[ny * qw + nx]) m |= uint8_t(1 << k);
                    }
                    mask[qy * qw + qx] = m;
                }
            return mask;
        }

        // BF3 RadiositySystem v22, SolveIrradiance 0x18968a0, SolveDirectionalIrradiance 0x18978f0:
        // header, 32-byte buckets, guids, u32 per pixel (valid | chart << 8), u8 mask per quad; per bucket (S = 0)
        // coef[B*n] (u8 - 127), palette index[B*quads], weight u16[B], cluster u16[B], normal u8x4[n], x[n], y[n], count u16[nsys]
        size_t writeTransportBf3(Writer& w, const Sys& s, const std::vector<Bucket>& buckets, const std::vector<std::vector<Ref>>& refs,
                                 const std::vector<uint16_t>& valid, const std::vector<int32_t>& guidOrder,
                                 const std::function<uint32_t(uint64_t)>& slotOf, const Vec3* dirTable,
                                 uint32_t& length, uint32_t& maxBudget, uint32_t& numPixels)
        {
            namespace sc = scene;
            const uint32_t W = sc::width();
            const Vec3* nrm = g_raster.nrm.data();
            const uint32_t nsys = uint32_t(guidOrder.size() + 2);
            const auto norm = [&](size_t k) { return float((std::max)(uint16_t(1), valid[k])); };
            size_t pixels = 0;
            uint32_t quads = 0;
            for (const Bucket& b : buckets) { pixels += b.px.size(); quads += b.nBounce; }

            w.align(16);
            const size_t base = w.pos();
            w.put(MAGIC_TRANSPORT); w.put(TRANSPORT_VERSION_BF3); w.put(uint32_t(buckets.size())); w.put(nsys);
            w.put(int32_t(s.w)); w.put(int32_t(s.h)); w.put(int32_t(pixels)); w.put(int32_t(quads));
            const size_t table = w.zeros(32 * buckets.size());
            for (int32_t v : guidOrder) { w.put(uint64_t(0)); w.put(uint32_t(BF3_ID_BASE + uint32_t(v))); w.put(uint32_t(0)); }
            w.put(uint64_t(0)); w.put(ENV_SYSTEM_ID); w.put(uint32_t(0));
            w.put(uint64_t(~0ull)); w.put(uint64_t(~0ull)); // sentinel
            std::vector<uint32_t> pixelInfo(size_t(s.w) * s.h, 0);
            for (const Bucket& b : buckets)
                for (uint32_t t : b.px) pixelInfo[size_t(t / W - s.y0) * s.w + (t % W - s.x0)] = 0x101; // valid, chart 1
            w.raw(pixelInfo.data(), pixelInfo.size() * 4);
            const std::vector<uint8_t> mask = quadMaskOf(s, buckets, W);
            w.raw(mask.data(), mask.size());

            uint32_t maxB = 0;
            size_t cursor = 0;
            for (size_t bi = 0; bi < buckets.size(); ++bi)
            {
                const Bucket& b = buckets[bi];
                const size_t n = b.px.size();
                struct Basis { uint64_t key; uint32_t slot; uint32_t cluster; float maxT; };
                std::vector<Basis> basis;
                std::unordered_map<uint64_t, uint32_t> where;
                for (size_t p = 0; p < n; ++p)
                    for (const Ref& r : refs[cursor + p])
                    {
                        const float T = float(r.count) / norm(cursor + p);
                        auto [it, ins] = where.try_emplace(r.key, uint32_t(basis.size()));
                        if (ins) basis.push_back({ r.key, slotOf(r.key), uint32_t(r.key & 0xFFFFFFFFu), T });
                        else basis[it->second].maxT = (std::max)(basis[it->second].maxT, T);
                    }
                constexpr int COEF_ZERO = 127, COEF_MAX = 128; // coef byte = 127 + value, value 0..128
                const uint32_t cap = uint32_t((std::max)(MIN_BASIS, g_params.maxBasis)) & ~15u;
                if (basis.size() > cap)
                {
                    std::sort(basis.begin(), basis.end(), [](const Basis& a, const Basis& c) { return a.maxT > c.maxT; });
                    basis.resize(cap);
                }
                // padded to a multiple of 16 with unused basis on the sentinel slot
                const uint32_t used = uint32_t(basis.size());
                const uint32_t B = (std::max)(uint32_t(MIN_BASIS), (used + 15) & ~15u);
                while (basis.size() < B) basis.push_back({ ~0ull, nsys - 1, 0, 0.0f });
                std::sort(basis.begin(), basis.end(), [](const Basis& a, const Basis& c) { return a.slot != c.slot ? a.slot < c.slot : a.cluster < c.cluster; });
                where.clear();
                for (uint32_t k = 0; k < B; ++k) if (basis[k].key != ~0ull) where[basis[k].key] = k;
                // per-basis weight so the largest transport maps to COEF_MAX
                std::vector<uint16_t> weight(B, 0);
                for (uint32_t k = 0; k < B; ++k)
                    if (basis[k].key != ~0ull)
                        weight[k] = uint16_t((std::clamp)(int(std::ceil(basis[k].maxT / (float(COEF_MAX) * WEIGHT_SCALE_BF3))), 1, 65535));

                w.align(16);
                const size_t bucketAddr = table + 32 * bi;
                const size_t dataStart = w.pos();
                for (size_t p = 0; p < n; ++p)
                {
                    std::vector<uint8_t> cf(B, COEF_ZERO);
                    for (const Ref& r : refs[cursor + p])
                    {
                        auto it = where.find(r.key);
                        if (it == where.end()) continue;
                        const float T = float(r.count) / norm(cursor + p);
                        cf[it->second] = uint8_t(COEF_ZERO + (std::clamp)(int(std::lround(T / (float(weight[it->second]) * WEIGHT_SCALE_BF3))), 0, COEF_MAX));
                    }
                    w.raw(cf.data(), B);
                }
                for (uint32_t p = 0; p < b.nBounce; ++p)
                {
                    std::vector<uint8_t> di(B, 0);
                    for (const Ref& r : refs[cursor + p])
                    {
                        auto it = where.find(r.key);
                        if (it != where.end()) di[it->second] = paletteIndex(fb::normalized(r.dir), dirTable);
                    }
                    w.raw(di.data(), B);
                }
                for (uint16_t v : weight) w.put(v);
                for (const Basis& x : basis) w.put(uint16_t(x.cluster));
                for (size_t p = 0; p < n; ++p)
                {
                    const Vec3& nn = nrm[b.px[p]];
                    w.put(unitByte(nn.m_x)); w.put(unitByte(nn.m_y)); w.put(unitByte(nn.m_z)); w.put(uint8_t(255));
                }
                for (size_t p = 0; p < n; ++p) w.put(uint16_t(b.px[p] % W - s.x0));
                for (size_t p = 0; p < n; ++p) w.put(uint16_t(b.px[p] / W - s.y0));
                std::vector<uint16_t> counts(nsys, 0);
                for (const Basis& x : basis) ++counts[x.slot];
                for (uint16_t v : counts) w.put(v);

                // bucket 32 B: pixels, bounce pixels, basis, shift 0, data offset, first pixel x / y, +0x1C 1
                w.at(bucketAddr + 0x00, uint16_t(n));
                w.at(bucketAddr + 0x02, uint16_t(b.nBounce));
                w.at(bucketAddr + 0x04, int32_t(B));
                w.at(bucketAddr + 0x08, int32_t(0));
                w.at(bucketAddr + 0x0C, int32_t(dataStart - bucketAddr));
                w.at(bucketAddr + 0x10, uint16_t(b.px[0] % W - s.x0));
                w.at(bucketAddr + 0x12, uint16_t(b.px[0] / W - s.y0));
                w.at(bucketAddr + 0x1C, int32_t(1));
                maxB = (std::max)(maxB, B);
                cursor += n;
            }
            w.align(16);
            length = uint32_t(w.pos() - base);
            maxBudget = maxB;
            numPixels = uint32_t(pixels);
            return base;
        }

        size_t writeTransport(Writer& w, const Sys& s, const std::vector<Sys>& all, const std::vector<int32_t>& texSys,
                              const std::vector<int32_t>& texCluster, const Vec3* dirTable, std::vector<int32_t>& inputsOut,
                              uint32_t& length, uint32_t& maxBudget, uint32_t& numPixels)
        {
            namespace sc = scene;
            const uint32_t W = sc::width();
            const Vec3* pos = g_raster.pos.data();
            const Vec3* nrm = g_raster.nrm.data();
            constexpr float SURFACE_FIRST = 100.0f; // bounce pixel: any surface texel beats gutter / fill
            const int R = rayCount();

            // buckets: 4x4 blocks, per 2x2 block one bounce pixel first
            // bounce pixel = the texel closest to the quad's mean normal
            std::vector<Bucket> buckets;
            for (uint32_t by = 0; by < s.h; by += BUCKET)
                for (uint32_t bx = 0; bx < s.w; bx += BUCKET)
                {
                    Bucket b;
                    std::vector<uint32_t> rest;
                    for (uint32_t sy = by; sy < (std::min)(by + BUCKET, s.h); sy += 2)
                        for (uint32_t sx = bx; sx < (std::min)(bx + BUCKET, s.w); sx += 2)
                        {
                            uint32_t quad[4];
                            int n = 0;
                            Vec3 mean{};
                            for (uint32_t y = sy; y < (std::min)(sy + 2, s.h); ++y)
                                for (uint32_t x = sx; x < (std::min)(sx + 2, s.w); ++x)
                                {
                                    const uint32_t t = (s.y0 + y) * W + (s.x0 + x);
                                    if (texSys[t] != int32_t(s.index)) continue;
                                    quad[n++] = t;
                                    mean += nrm[t];
                                }
                            if (!n) continue;
                            int best = 0;
                            float bestScore = -1e30f;
                            for (int k = 0; k < n; ++k)
                            {
                                const float score = fb::dot(nrm[quad[k]], mean) + (g_raster.cov[quad[k]] == COV_SURFACE ? SURFACE_FIRST : 0.0f);
                                if (score > bestScore) { bestScore = score; best = k; }
                            }
                            b.px.push_back(quad[best]);
                            for (int k = 0; k < n; ++k)
                                if (k != best) rest.push_back(quad[k]);
                        }
                    b.nBounce = uint32_t(b.px.size());
                    b.px.insert(b.px.end(), rest.begin(), rest.end());
                    if (!b.px.empty()) buckets.push_back(std::move(b));
                }

            std::vector<uint32_t> order;
            for (const Bucket& b : buckets) order.insert(order.end(), b.px.begin(), b.px.end());
            std::vector<std::vector<Ref>> refs(order.size());
            std::vector<uint16_t> valid(order.size(), 0);
            parallelFor(order.size(), [&](size_t b, size_t e)
            {
                for (size_t k = b; k < e && !g_cancel.load(); ++k)
                {
                    const uint32_t t = order[k];
                    // fill: no geometry, R rays split over the upper env directions by their up component
                    if (g_raster.cov[t] == COV_FILL)
                    {
                        float sum = 0.0f;
                        for (uint32_t i = 0; i < ENV_COUNT; ++i) sum += (std::max)(0.0f, kEnvDirs[i][1]);
                        for (uint32_t i = 0; i < ENV_COUNT; ++i)
                        {
                            const uint32_t c = uint32_t(std::lround(float(R) * (std::max)(0.0f, kEnvDirs[i][1]) / sum));
                            if (c) refs[k].push_back({ ENV_KEY | i, c, vec3(kEnvDirs[i][0], kEnvDirs[i][1], kEnvDirs[i][2]) * float(c) });
                        }
                        valid[k] = uint16_t(R);
                        continue;
                    }
                    // transport = share of the R cosine rays reaching each cluster / env direction
                    // back-face hits are dropped from the count (texel buried inside geometry)
                    const Vec3 n = nrm[t];
                    const Vec3 o = pos[t] + n * RAY_BIAS;
                    RaySeq seq(t);
                    std::vector<Ref>& out = refs[k];
                    int good = 0;
                    for (int r = 0; r < R; ++r)
                    {
                        const Vec3 d = seq.dir(n, r, R);
                        size_t hit; float dist;
                        uint64_t key;
                        if (sc::hitTexel(o, d, TRANSPORT_RANGE, hit, dist))
                        {
                            if (hit != ~size_t(0) && fb::dot(d, nrm[hit]) > BACK_FACE_DOT) continue;
                            ++good;
                            if (hit == ~size_t(0) || texSys[hit] < 0 || texCluster[hit] < 0) continue;
                            key = (uint64_t(uint32_t(texSys[hit])) << 32) | uint32_t(texCluster[hit]);
                        }
                        else
                        {
                            ++good;
                            if (!g_params.environment) continue;
                            uint32_t best = 0; float bd = -2.0f;
                            for (uint32_t i = 0; i < ENV_COUNT; ++i)
                            {
                                const float dd = d.m_x * kEnvDirs[i][0] + d.m_y * kEnvDirs[i][1] + d.m_z * kEnvDirs[i][2];
                                if (dd > bd) { bd = dd; best = i; }
                            }
                            key = ENV_KEY | best;
                        }
                        auto it = std::find_if(out.begin(), out.end(), [&](const Ref& x) { return x.key == key; });
                        if (it == out.end()) out.push_back({ key, 1, d });
                        else { ++it->count; it->dir += d; }
                    }
                    valid[k] = uint16_t(good);
                    g_backRays += uint64_t(R - good);
                    g_allRays += uint64_t(R);
                }
            });
            for (size_t k = 0; k < order.size(); ++k)
            {
                TexelInfo& d = g_diag[order[k]];
                d.system = texSys[order[k]];
                d.cluster = texCluster[order[k]];
                d.rays = uint16_t(R);
                d.rawValid = valid[k];
                d.pooled = 1;
                d.flags = uint8_t((g_raster.cov[order[k]] == COV_FILL ? kFill : 0) | (g_raster.cov[order[k]] == COV_GUTTER ? kGutter : 0));
            }
            for (const Bucket& b : buckets)
                for (uint32_t p = 0; p < b.nBounce; ++p) g_diag[b.px[p]].flags |= kBounce;
            {
                // buried pixel (under R / 4 valid rays) copies a coplanar neighbour within 2 rings that has R / 2
                constexpr int BURIED_DIV = 4, DONOR_DIV = 2, DONOR_RINGS = 2;
                constexpr float DONOR_DOT = 0.8f; // normal agreement
                constexpr float DONOR_DIST = 1.5f; // meters
                // smoothing: pool refs of coplanar neighbours, weight 2^(radius - ring)
                constexpr int MAX_SMOOTH = 3;
                constexpr float POOL_DOT = 0.95f; // normal agreement
                constexpr float POOL_PLANE = 0.3f; // meters off the texel's plane
                constexpr float POOL_DIST = 8.0f; // meters
                constexpr uint32_t POOL_FLOOR_DIV = 4; // pooled valid >= R / 4, buried neighbourhoods stay dark, not bright
                std::unordered_map<uint32_t, uint32_t> at;
                for (uint32_t k = 0; k < order.size(); ++k) at[order[k]] = k;
                const int minValid = (std::max)(1, R / BURIED_DIV);
                std::vector<uint32_t> donor(order.size(), ~0u);
                for (uint32_t k = 0; k < order.size(); ++k)
                {
                    if (valid[k] >= minValid) continue;
                    ++g_buried;
                    g_diag[order[k]].flags |= kBuried;
                    const int x = int(order[k] % W), y = int(order[k] / W);
                    int best = -1;
                    for (int r = 1; r <= DONOR_RINGS && best < 0; ++r)
                        for (int dy = -r; dy <= r && best < 0; ++dy)
                            for (int dx = -r; dx <= r; ++dx)
                            {
                                const auto it = at.find(uint32_t((y + dy) * int(W) + (x + dx)));
                                if (it == at.end() || valid[it->second] < R / DONOR_DIV) continue;
                                const uint32_t a = order[k], c = order[it->second];
                                if (fb::dot(nrm[a], nrm[c]) < DONOR_DOT || fb::distanceSq(pos[a], pos[c]) > DONOR_DIST * DONOR_DIST) continue;
                                best = int(it->second);
                                break;
                            }
                    if (best >= 0) donor[k] = uint32_t(best);
                }
                for (uint32_t k = 0; k < order.size(); ++k)
                    if (donor[k] != ~0u) { refs[k] = refs[donor[k]]; valid[k] = valid[donor[k]]; ++g_refilled; g_diag[order[k]].flags |= kRefilled; }

                const int sr = (std::clamp)(g_params.smoothRadius, 0, MAX_SMOOTH);
                if (sr > 0)
                {
                    std::vector<std::vector<Ref>> pooled(order.size());
                    std::vector<uint16_t> pooledValid(order.size(), 0);
                    parallelFor(order.size(), [&](size_t b, size_t e)
                    {
                        std::unordered_map<uint64_t, uint32_t> slotOfKey;
                        for (size_t k = b; k < e; ++k)
                        {
                            const uint32_t a = order[k];
                            const int x = int(a % W), y = int(a / W);
                            std::vector<Ref>& out = pooled[k];
                            slotOfKey.clear();
                            uint32_t v = 0, wsum = 0;
                            for (int dy = -sr; dy <= sr; ++dy)
                                for (int dx = -sr; dx <= sr; ++dx)
                                {
                                    const auto it = at.find(uint32_t((y + dy) * int(W) + (x + dx)));
                                    if (it == at.end() || !valid[it->second]) continue;
                                    const uint32_t c = order[it->second];
                                    if (c != a && (fb::dot(nrm[a], nrm[c]) < POOL_DOT || std::fabs(fb::dot(nrm[a], pos[c] - pos[a])) > POOL_PLANE ||
                                        fb::distanceSq(pos[a], pos[c]) > POOL_DIST * POOL_DIST)) continue;
                                    const uint32_t wgt = 1u << (sr - (std::max)(std::abs(dx), std::abs(dy)));
                                    v += valid[it->second] * wgt;
                                    wsum += wgt;
                                    ++g_diag[a].pooled;
                                    for (const Ref& r : refs[it->second])
                                    {
                                        const auto [f, fresh] = slotOfKey.try_emplace(r.key, uint32_t(out.size()));
                                        if (fresh) out.push_back({ r.key, r.count * wgt, r.dir * float(wgt) });
                                        else { out[f->second].count += r.count * wgt; out[f->second].dir += r.dir * float(wgt); }
                                    }
                                }
                            v = (std::max)(v, wsum * uint32_t(R) / POOL_FLOOR_DIV);
                            const uint32_t scale = (v + 65534) / 65535; // valid is u16: scale counts down on overflow
                            if (scale > 1)
                                for (Ref& r : out) r.count = (std::max)(1u, r.count / scale);
                            pooledValid[k] = uint16_t((std::max)(1u, v / (std::max)(scale, 1u)));
                        }
                    });
                    for (size_t k = 0; k < order.size(); ++k)
                        if (valid[k]) { refs[k] = std::move(pooled[k]); valid[k] = pooledValid[k]; }
                }
            }
            const auto norm = [&](size_t k) { return float((std::max)(uint16_t(1), valid[k])); };
            for (size_t k = 0; k < order.size(); ++k)
            {
                TexelInfo& d = g_diag[order[k]];
                if (d.pooled > 1) --d.pooled;
                double env = 0.0, lit = 0.0;
                for (const Ref& r : refs[k]) ((r.key & ENV_KEY) == ENV_KEY ? env : lit) += r.count;
                const double v = norm(k);
                d.refs = uint16_t((std::min)(refs[k].size(), size_t(65535)));
                d.env = float(env / v);
                d.lit = float(lit / v);
                d.black = (std::max)(0.0f, 1.0f - d.env - d.lit);
            }

            // guid order: system index descending, env, sentinel
            std::vector<int32_t> sysIn;
            for (const auto& v : refs)
                for (const Ref& r : v)
                    if ((r.key & ENV_KEY) != ENV_KEY) sysIn.push_back(int32_t(r.key >> 32));
            sysIn.push_back(int32_t(s.index));
            std::sort(sysIn.begin(), sysIn.end());
            sysIn.erase(std::unique(sysIn.begin(), sysIn.end()), sysIn.end());
            std::vector<int32_t> guidOrder(sysIn.rbegin(), sysIn.rend());
            const uint32_t nsys = uint32_t(guidOrder.size() + 2);
            std::unordered_map<int32_t, uint32_t> slot;
            for (uint32_t i = 0; i < guidOrder.size(); ++i) slot[guidOrder[i]] = i;
            const auto slotOf = [&](uint64_t key) { return (key & ENV_KEY) == ENV_KEY ? uint32_t(guidOrder.size()) : slot[int32_t(key >> 32)]; };
            inputsOut.clear();
            inputsOut.push_back(int32_t(s.index));
            for (int32_t v : sysIn) if (v != int32_t(s.index)) inputsOut.push_back(v);

#if defined(BFVE_GAME_BF3)
            return writeTransportBf3(w, s, buckets, refs, valid, guidOrder, slotOf, dirTable, length, maxBudget, numPixels);
#endif
            // header, bucket table, guids, counts
            w.align(16);
            const size_t base = w.pos();
            w.put(MAGIC_TRANSPORT); w.put(TRANSPORT_VERSION); w.put(uint32_t(buckets.size())); w.put(nsys);
            w.put(uint16_t(s.w)); w.put(uint16_t(s.h));
            const size_t maxAt = w.zeros(4); // u16 max pixels, u16 max bounce pixels per bucket
            w.put(uint32_t(order.size()));
            uint32_t quadsTotal = 0;
            for (const Bucket& b : buckets) quadsTotal += b.nBounce;
            w.put(quadsTotal);
            const size_t table = w.zeros(16 * buckets.size());
            for (int32_t v : guidOrder) { w.put(uint64_t(0)); w.put(uint64_t(uint32_t(v))); }
            w.put(uint64_t(0)); w.put(uint64_t(ENV_SYSTEM_ID));
            w.put(uint64_t(~0ull)); w.put(uint64_t(~0ull)); // sentinel
            for (int32_t v : guidOrder) w.put(int32_t(all[v].clusters.size())); // input tree size per guid
            w.put(ENV_TREE_NODES); w.put(int32_t(0));
            for (uint32_t i = nsys; i % 4; ++i) w.put(int32_t(0));

            const std::vector<uint8_t> quadMask = quadMaskOf(s, buckets, W);
            w.raw(quadMask.data(), quadMask.size());

            uint32_t maxPix = 0, maxBounce = 0, maxBasis = 0;
            size_t cursor = 0;
            for (size_t bi = 0; bi < buckets.size(); ++bi)
            {
                const Bucket& b = buckets[bi];
                const size_t n = b.px.size();
                struct Basis { uint64_t key; uint32_t slot; uint32_t cluster; float maxT; };
                std::vector<Basis> basis;
                std::unordered_map<uint64_t, uint32_t> where;
                for (size_t p = 0; p < n; ++p)
                    for (const Ref& r : refs[cursor + p])
                    {
                        const float T = float(r.count) / norm(cursor + p);
                        auto [it, ins] = where.try_emplace(r.key, uint32_t(basis.size()));
                        if (ins) basis.push_back({ r.key, slotOf(r.key), uint32_t(r.key & 0xFFFFFFFFu), T });
                        else basis[it->second].maxT = (std::max)(basis[it->second].maxT, T);
                    }
                const uint32_t cap = uint32_t((std::max)(MIN_BASIS, g_params.maxBasis));
                if (basis.size() > cap)
                {
                    std::sort(basis.begin(), basis.end(), [](const Basis& a, const Basis& b) { return a.maxT > b.maxT; });
                    basis.resize(cap);
                }
                std::sort(basis.begin(), basis.end(), [](const Basis& a, const Basis& b) { return a.slot != b.slot ? a.slot < b.slot : a.cluster < b.cluster; });
                where.clear();
                for (uint32_t k = 0; k < basis.size(); ++k) where[basis[k].key] = k;
                const uint32_t nb = uint32_t(basis.size());
                // per-basis weight so the largest transport maps to coef 255
                std::vector<uint16_t> weight(nb);
                for (uint32_t k = 0; k < nb; ++k)
                    weight[k] = uint16_t((std::clamp)(int(std::ceil(basis[k].maxT / (255.0f * WEIGHT_SCALE))), 1, 65535));

                w.align(16);
                const size_t bucketAddr = table + 16 * bi;
                const size_t dataStart = w.pos();
                for (uint16_t v : weight) w.put(v);
                for (const Basis& x : basis) w.put(uint16_t(x.cluster));
                std::vector<uint16_t> counts(nsys, 0);
                for (const Basis& x : basis) ++counts[x.slot];
                for (uint16_t v : counts) w.put(v);
                const size_t coef = (dataStart + 2 * (2 * nb + nsys) + 15) & ~size_t(15);
                while (w.pos() < coef) w.put(uint8_t(0));
                for (uint32_t p = 0; p < b.nBounce; ++p)
                {
                    std::vector<uint8_t> di(nb, 0);
                    for (const Ref& r : refs[cursor + p])
                    {
                        auto it = where.find(r.key);
                        if (it != where.end()) di[it->second] = paletteIndex(fb::normalized(r.dir), dirTable);
                    }
                    w.raw(di.data(), nb);
                }
                for (size_t p = 0; p < n; ++p)
                {
                    std::vector<uint8_t> cf(nb, 0);
                    for (const Ref& r : refs[cursor + p])
                    {
                        auto it = where.find(r.key);
                        if (it == where.end()) continue;
                        const float T = float(r.count) / norm(cursor + p);
                        cf[it->second] = uint8_t((std::clamp)(int(std::lround(T / (float(weight[it->second]) * WEIGHT_SCALE))), 0, 255));
                    }
                    w.raw(cf.data(), nb);
                }
                w.align(4);
                for (size_t p = 0; p < n; ++p) w.put(uint16_t(b.px[p] % W - s.x0));
                for (size_t p = 0; p < n; ++p) w.put(uint16_t(b.px[p] / W - s.y0));
                for (size_t p = 0; p < n; ++p)
                {
                    const Vec3& nn = nrm[b.px[p]];
                    w.put(unitByte(nn.m_x)); w.put(unitByte(nn.m_y)); w.put(unitByte(nn.m_z)); w.put(uint8_t(255));
                }
                // bucket 16 B: pixels, bounce pixels, basis, u8 shift 0 (8-bit coefs), u8 1, systems, extra 0, data offset
                const int32_t off = int32_t(dataStart - bucketAddr);
                w.at(bucketAddr + 0, uint16_t(n));
                w.at(bucketAddr + 2, uint16_t(b.nBounce));
                w.at(bucketAddr + 4, uint16_t(nb));
                w.at(bucketAddr + 6, uint8_t(0));
                w.at(bucketAddr + 7, uint8_t(1));
                w.at(bucketAddr + 8, uint16_t(nsys));
                w.at(bucketAddr + 10, uint16_t(0));
                w.at(bucketAddr + 12, off);
                maxPix = (std::max)(maxPix, uint32_t(n));
                maxBounce = (std::max)(maxBounce, b.nBounce);
                maxBasis = (std::max)(maxBasis, nb);
                cursor += n;
            }
            w.at(maxAt, uint16_t(maxPix));
            w.at(maxAt + 2, uint16_t(maxBounce));
            length = uint32_t(w.pos() - base);
            maxBudget = (maxBasis + 15) & ~15u; // rounded to MIN_BASIS
            numPixels = uint32_t(order.size());
            return base;
        }

        // <map>_probes.bin: 'EPRB', u32 version 1, u32 sets, per set u32 count, per probe u16 hits + u32 texel[hits]
#if defined(BFVE_GAME_BF4)
        bool writeProbeTexels(const db::Captured& base, const std::string& path)
        {
            namespace sc = scene;
            const uint8_t* blob = base.blob.data();
            const auto* header = reinterpret_cast<const fb::EnlightenDatabaseHeader*>(blob);
            const uint32_t nSets = header->m_probeSetCount;
            const uint64_t setsOff = reinterpret_cast<uint64_t>(header->m_probeSets); // unrelocated blob offset
            if (!nSets || setsOff + sizeof(fb::EnlightenProbeSetEntry) * nSets > base.blob.size()) return false;
            Vec3 dirs[PROBE_RAYS];
            probeDirs(dirs);
            std::vector<uint8_t> out;
            const auto put32 = [&](uint32_t v) { out.insert(out.end(), reinterpret_cast<uint8_t*>(&v), reinterpret_cast<uint8_t*>(&v) + 4); };
            put32(MAGIC_PROBE_TEXELS); put32(1); put32(nSets);
            size_t probes = 0, withHits = 0;
            for (uint32_t si = 0; si < nSets; ++si)
            {
                const auto& e = reinterpret_cast<const fb::EnlightenProbeSetEntry*>(blob + setsOff)[si];
                const fb::LinearTransform xf = e.m_transform;
                const Vec3 mn = e.m_boxMin, mx = e.m_boxMax;
                const int dim[3] = { e.m_dims[0], e.m_dims[1], e.m_dims[2] };
                const uint32_t count = e.m_probeCount;
                const uint64_t cellsOff = reinterpret_cast<uint64_t>(e.m_cells);
                const size_t cells = size_t((std::max)(dim[0], 0)) * (std::max)(dim[1], 0) * (std::max)(dim[2], 0);
                const bool ok = count && cells && cellsOff + 4 * cells <= base.blob.size();
                put32(ok ? count : 0);
                if (!ok) continue;
                // probe position = mean of the grid cell centres that index it
                std::vector<Vec3> pos(count, Vec3{});
                std::vector<uint32_t> n(count, 0);
                const uint32_t* cell = reinterpret_cast<const uint32_t*>(blob + cellsOff);
                const Vec3 step = (mx - mn) * vec3(1.0f / dim[0], 1.0f / dim[1], 1.0f / dim[2]);
                for (int z = 0; z < dim[2]; ++z)
                    for (int y = 0; y < dim[1]; ++y)
                        for (int x = 0; x < dim[0]; ++x)
                        {
                            const uint32_t pi = cell[size_t(dim[0]) * (size_t(z) * dim[1] + y) + x];
                            if (pi >= count) continue;
                            pos[pi] += fb::transformPoint(xf, mn + step * vec3(x + 0.5f, y + 0.5f, z + 0.5f));
                            ++n[pi];
                        }
                std::vector<std::vector<uint32_t>> hits(count);
                parallelFor(count, [&](size_t b, size_t en)
                {
                    for (size_t pi = b; pi < en; ++pi)
                    {
                        if (!n[pi]) continue;
                        const Vec3 p = pos[pi] * (1.0f / float(n[pi]));
                        for (const Vec3& d : dirs)
                        {
                            size_t t; float dist;
                            if (sc::hitTexel(p, d, PROBE_RANGE, t, dist) && t != ~size_t(0) && g_raster.cov[t] == COV_SURFACE)
                                hits[pi].push_back(uint32_t(t));
                        }
                    }
                });
                for (uint32_t pi = 0; pi < count; ++pi)
                {
                    const uint16_t h = uint16_t(hits[pi].size());
                    out.push_back(uint8_t(h)); out.push_back(uint8_t(h >> 8));
                    for (uint32_t t : hits[pi]) put32(t);
                    ++probes;
                    withHits += h > 0;
                }
            }
            std::ofstream f(path, std::ios::binary);
            if (!f.write(reinterpret_cast<const char*>(out.data()), std::streamsize(out.size()))) return false;
            logger::info("[enlighten] generator: {} probes in {} sets, {} see lightmap texels -> {}", probes, nSets, withHits, path);
            return true;
        }
#endif

#if defined(BFVE_GAME_BF3)
        // positions from EnlightenProbeSetData +0xC8
        bool writeProbeTexelsBf3(const std::string& path)
        {
            namespace sc = scene;
            fb::EnlightenRuntimeDatabase* ldb = db::liveDatabase();
            if (fb::EnlightenRenderer* r = fb::EnlightenRenderer::GetInstance(); !ldb && r)
                for (auto** it = r->m_entities.begin(); it && it < r->m_entities.end(); ++it)
                    if (*it && (*it)->m_staticEntity && (*it)->m_staticEntity->m_luma == detail::g_orig[0].tex) ldb = (*it)->m_database;
            if (!ldb) return false;
            constexpr float BOX_MARGIN = 1.0f; // meters, positions outside the set box are local
            Vec3 dirs[PROBE_RAYS];
            probeDirs(dirs);
            std::vector<uint8_t> out;
            const auto put32 = [&](uint32_t v) { out.insert(out.end(), reinterpret_cast<uint8_t*>(&v), reinterpret_cast<uint8_t*>(&v) + 4); };
            const uint32_t nSets = uint32_t(ldb->m_probeSets.size());
            put32(MAGIC_PROBE_TEXELS); put32(1); put32(nSets);
            size_t probes = 0, withHits = 0, transformed = 0;
            for (uint32_t si = 0; si < nSets; ++si)
            {
                const fb::EnlightenProbeSetRuntime* set = ldb->m_probeSets[si].m_set;
                const fb::EnlightenProbeSetData* d = set ? set->m_data : nullptr;
                const uint32_t count = d && d->m_positions ? d->m_probeCount : 0;
                put32(count);
                if (!count) continue;
                std::vector<Vec3> pos(count);
                for (uint32_t pi = 0; pi < count; ++pi)
                {
                    const Vec3 p = d->m_positions[pi];
                    const bool inside = p.m_x >= d->m_boxMin.m_x - BOX_MARGIN && p.m_y >= d->m_boxMin.m_y - BOX_MARGIN && p.m_z >= d->m_boxMin.m_z - BOX_MARGIN &&
                        p.m_x <= d->m_boxMax.m_x + BOX_MARGIN && p.m_y <= d->m_boxMax.m_y + BOX_MARGIN && p.m_z <= d->m_boxMax.m_z + BOX_MARGIN;
                    pos[pi] = inside ? p : fb::transformPoint(d->m_transform, p);
                    transformed += !inside;
                }
                std::vector<std::vector<uint32_t>> hits(count);
                parallelFor(count, [&](size_t b, size_t en)
                {
                    for (size_t pi = b; pi < en; ++pi)
                        for (const Vec3& dir : dirs)
                        {
                            size_t t; float dist;
                            if (sc::hitTexel(pos[pi], dir, PROBE_RANGE, t, dist) && t != ~size_t(0) && g_raster.cov[t] == COV_SURFACE)
                                hits[pi].push_back(uint32_t(t));
                        }
                });
                for (uint32_t pi = 0; pi < count; ++pi)
                {
                    const uint16_t h = uint16_t((std::min)(hits[pi].size(), size_t(65535)));
                    out.push_back(uint8_t(h)); out.push_back(uint8_t(h >> 8));
                    for (uint16_t k = 0; k < h; ++k) put32(hits[pi][k]);
                    ++probes;
                    withHits += h > 0;
                }
            }
            std::ofstream f(path, std::ios::binary);
            if (!f.write(reinterpret_cast<const char*>(out.data()), std::streamsize(out.size()))) return false;
            logger::info("[enlighten] generator: {} probes in {} sets ({} placed by the set transform), {} see lightmap texels -> {}",
                probes, nSets, transformed, withHits, path);
            return true;
        }

        // EnlightenSystem::load stream, retail sub_17B5A10
        std::vector<uint8_t> systemStreamBf3(const Sys& s, const std::vector<Sys>& all, const std::vector<int32_t>& texSys,
                                             const std::vector<int32_t>& texCluster, const Vec3* dirTable)
        {
            // blocks: {magic, version, data type, length}
            constexpr uint32_t CORE_VERSION = 22, TRANSPORT_TYPE = 1;
            constexpr uint32_t WORKSPACE_VERSION = 8, WORKSPACE_TYPE = 10;
            constexpr uint32_t ALBEDO_VERSION = 4, ALBEDO_TYPE = 11;
            Writer out;
            const auto block = [&](uint32_t version, uint32_t type, const Writer& body)
            {
                out.put(BF3_MAGIC); out.put(version); out.put(type); out.put(uint32_t(body.b.size()));
                out.raw(body.b.data(), body.b.size());
            };
            std::vector<int32_t> inputs;
            uint32_t tlen = 0, budget = 0, pixels = 0, wlen = 0;
            Writer tw;
            writeTransport(tw, s, all, texSys, texCluster, dirTable, inputs, tlen, budget, pixels);
            // RadSystemMetaData: guid, w, h, irradiance budget, sph budget 0, pixels, directional, env resolution
            out.put(BF3_MAGIC); out.put(CORE_VERSION);
            uint8_t meta[48] = {};
            const uint32_t id = BF3_ID_BASE + s.index;
            const int32_t m[5] = { int32_t(s.w), int32_t(s.h), int32_t(budget), 0, int32_t(pixels) };
            std::memcpy(meta + 8, &id, 4);
            std::memcpy(meta + 16, m, sizeof(m));
            meta[0x24] = 1; // directional
            const int32_t envRes = int32_t(ENV_RES);
            std::memcpy(meta + 0x28, &envRes, 4);
            out.raw(meta, sizeof(meta));
            out.put(TRANSPORT_TYPE); out.put(uint32_t(tw.b.size())); out.raw(tw.b.data(), tw.b.size());
            Writer iw;
            writeWorkspace(iw, s, wlen);
            block(WORKSPACE_VERSION, WORKSPACE_TYPE, iw);
            Writer vw;
            writeVisibility(vw, s);
            out.put(int32_t(vw.b.size() + 4)); out.put(int32_t(vw.b.size())); out.raw(vw.b.data(), vw.b.size());
            Writer aw;
            writeAlbedo(aw, s);
            block(ALBEDO_VERSION, ALBEDO_TYPE, aw);
            out.put(uint32_t(2)); out.put(uint64_t(0)); out.put(uint64_t(TERRAIN_MATERIAL)); // material ids
            const float box[6] = { s.mn.m_x, s.mn.m_y, s.mn.m_z, s.mx.m_x, s.mx.m_y, s.mx.m_z };
            out.raw(box, sizeof(box));
            out.put(uint32_t(s.x0)); out.put(uint32_t(s.y0));
            out.put(uint32_t(inputs.size()));
            for (int32_t v : inputs) out.put(uint32_t(BF3_ID_BASE + uint32_t(v)));
            out.put(uint8_t(0));
            return std::move(out.b);
        }
#endif

#if defined(BFVE_GAME_BF3)
        // <map>_generated.bf3edb: 'EB3S', u32 version 1, u32 systems, u32 atlas w, h, per system u32 size + stream
        void runBf3()
        {
            namespace sc = scene;
            setStatus("building systems");
            g_backRays = 0; g_allRays = 0; g_buried = 0; g_refilled = 0;
            g_diagReady = false;
            buildRaster(GUTTER_RINGS);
            g_diag.assign(size_t(sc::width()) * sc::height(), TexelInfo{});
            std::vector<Sys> systems;
            std::vector<int32_t> texSys, texCluster;
            buildSystems(systems, texSys, texCluster);
            if (systems.empty()) { setStatus("failed: no covered texels"); return; }
            size_t clusters = 0;
            for (const Sys& s : systems) clusters += s.clusters.size();
            logger::info("[enlighten] generator: {} systems, {} clusters, {} texels", systems.size(), clusters, sc::covered().size());
            Vec3 dirTable[256];
            for (int i = 0; i < 256; ++i) dirTable[i] = vec3(BF3_DIR_TABLE[i][0], BF3_DIR_TABLE[i][1], BF3_DIR_TABLE[i][2]);
            Writer file;
            file.put(MAGIC_BF3_FILE); file.put(uint32_t(1)); file.put(uint32_t(systems.size()));
            file.put(sc::width()); file.put(sc::height());
            for (size_t i = 0; i < systems.size() && !g_cancel.load(); ++i)
            {
                setStatus("system " + std::to_string(i + 1) + " / " + std::to_string(systems.size()));
                const std::vector<uint8_t> st = systemStreamBf3(systems[i], systems, texSys, texCluster, dirTable);
                file.put(uint32_t(st.size()));
                file.raw(st.data(), st.size());
                g_progress = float(i + 1) / float(systems.size());
            }
            if (g_cancel.load()) { setStatus("cancelled"); return; }
            const std::string path = db::directory() + "/" + db::mapFileName() + "_generated.bf3edb";
            std::error_code ec;
            std::filesystem::create_directories(db::directory(), ec);
            std::ofstream f(path, std::ios::binary);
            if (!f.write(reinterpret_cast<const char*>(file.b.data()), std::streamsize(file.b.size()))) { setStatus("failed: cannot write " + path); return; }
            f.close();
            logger::info("[enlighten] generator wrote {}: {} systems, {} clusters, {} bytes", path, systems.size(), clusters, file.b.size());
            setStatus("probe texels");
            writeProbeTexelsBf3(db::directory() + "/" + db::mapFileName() + "_probes.bin");
            logger::info("[enlighten] generator: {:.1f}% of rays hit a surface from behind; {} buried pixels, {} refilled from a neighbour",
                g_allRays ? 100.0 * double(g_backRays) / double(g_allRays) : 0.0, uint64_t(g_buried), uint64_t(g_refilled));
            g_written = path;
            g_diagReady = true;
            setStatus("wrote " + db::mapFileName() + "_generated.bf3edb (" + std::to_string(systems.size()) + " systems) - applying live");
        }
#endif

        void run()
        {
            try
            {
                std::string why;
                namespace sc = scene;
                if (!sc::prepare(why)) { setStatus("failed: " + why); return; }
#if defined(BFVE_GAME_BF3)
                runBf3();
#else
                db::Captured base;
                if (!db::capturedFor(sc::width(), sc::height(), base)) { setStatus("failed: this level's database was not captured - load it again with the editor injected"); return; }
                if (reinterpret_cast<const fb::EnlightenDatabaseHeader*>(base.blob.data())->m_systemCount)
                {
                    setStatus("failed: the level already ships systems (use its own database)");
                    return;
                }

                setStatus("building systems");
                g_backRays = 0; g_allRays = 0; g_buried = 0; g_refilled = 0;
                g_diagReady = false;
                buildRaster(GUTTER_RINGS);
                g_diag.assign(size_t(sc::width()) * sc::height(), TexelInfo{});

                std::vector<Sys> systems;
                std::vector<int32_t> texSys, texCluster;
                buildSystems(systems, texSys, texCluster);
                if (systems.empty()) { setStatus("failed: no covered texels"); return; }
                size_t clusters = 0;
                for (const Sys& s : systems) clusters += s.clusters.size();
                size_t gutter = 0;
                for (uint8_t c : g_raster.cov) gutter += c == 2;
                logger::info("[enlighten] generator: {} systems, {} clusters, {} texels + {} gutter", systems.size(), clusters, sc::covered().size(), gutter);

                Vec3 dirTable[256];
                std::memcpy(dirTable, reinterpret_cast<const void*>(OFF_Enlighten_directionTable), sizeof(dirTable));

                Writer w;
                w.b = base.blob;
                std::vector<uint32_t> relocs = base.relocs;
                w.align(128);
                const auto offsetPtr = [](size_t off) { return reinterpret_cast<void*>(off); };
                const size_t entries = w.zeros(sizeof(fb::EnlightenSystemEntry) * systems.size());
                for (size_t i = 0; i < systems.size() && !g_cancel.load(); ++i)
                {
                    const Sys& s = systems[i];
                    setStatus("system " + std::to_string(i + 1) + " / " + std::to_string(systems.size()));
                    std::vector<int32_t> inputs;
                    uint32_t tlen = 0, budget = 0, pixels = 0, wlen = 0;
                    const size_t transport = writeTransport(w, s, systems, texSys, texCluster, dirTable, inputs, tlen, budget, pixels);
                    const size_t ws = writeWorkspace(w, s, wlen);
                    const size_t gevs = writeVisibility(w, s);
                    const size_t albedo = writeAlbedo(w, s);
                    w.align(16);
                    const size_t inputsAt = w.pos();
                    for (int32_t v : inputs) w.put(v);
                    w.align(8);
                    const size_t shadersAt = w.pos();
                    w.put(uint64_t(0)); w.put(uint64_t(TERRAIN_MATERIAL));
                    w.align(16);
                    // system GUID also on the workspace ref
                    // CacheInputLighting sub_1411968A0 skips mismatched buffer GUIDs
                    fb::EnlightenWorkspaceRef ref{};
                    ref.m_guid[1] = s.index;
                    ref.m_workspace = offsetPtr(ws);
                    ref.m_size = wlen;
                    ref.m_version = 4; // version / flags as the shipped refs
                    ref.m_flags = 1;
                    const size_t wsRef = w.put(ref);
                    relocs.push_back(uint32_t(wsRef + offsetof(fb::EnlightenWorkspaceRef, m_workspace)));
                    w.align(16);
                    fb::EnlightenSystemCore core{};
                    core.m_guid[1] = s.index;
                    core.m_width = s.w;
                    core.m_height = s.h;
                    core.m_basisBudget = budget;
                    // solve alloc bytes: 16 per basis vector (+64), 16 per half-res bounce texel with a 1 ring border,
                    // 64 per input incl. env + sentinel, 8 KiB slack
                    core.m_workspaceSize = uint32_t(16 * (budget + 64) + 16 * (s.w / 2 + 2) * (s.h / 2 + 2) + 64 * uint32_t(inputs.size() + 2) + 8192);
                    core.m_pixelCount = pixels;
                    core.m_environmentRes = ENV_RES;
                    core.m_hasDirectional = 1;
                    core.m_transport = offsetPtr(transport);
                    core.m_transportSize = tlen;
                    core.m_type = 1;
                    core.m_fourBit = 0;
                    const size_t coreAt = w.put(core);
                    relocs.push_back(uint32_t(coreAt + offsetof(fb::EnlightenSystemCore, m_transport)));

                    fb::EnlightenSystemEntry entry{};
                    entry.m_boxMin = vec3(s.mn.m_x, s.mn.m_y, s.mn.m_z);
                    entry.m_boxMax = vec3(s.mx.m_x, s.mx.m_y, s.mx.m_z);
                    entry.m_atlasX = s.x0;
                    entry.m_atlasY = s.y0;
                    entry.m_inputCount = uint32_t(inputs.size());
                    entry.m_shaderCount = 2; // material ids 0 and TERRAIN_MATERIAL
                    entry.m_inputs = static_cast<int32_t*>(offsetPtr(inputsAt));
                    entry.m_shaders = static_cast<uint64_t*>(offsetPtr(shadersAt));
                    entry.m_core = static_cast<fb::EnlightenSystemCore*>(offsetPtr(coreAt));
                    entry.m_workspace = static_cast<fb::EnlightenWorkspaceRef*>(offsetPtr(wsRef));
                    entry.m_visibility = offsetPtr(gevs);
                    entry.m_albedo = offsetPtr(albedo);
                    const size_t e = entries + sizeof(fb::EnlightenSystemEntry) * i;
                    w.at(e, entry);
                    for (size_t field : { offsetof(fb::EnlightenSystemEntry, m_inputs), offsetof(fb::EnlightenSystemEntry, m_shaders),
                                          offsetof(fb::EnlightenSystemEntry, m_core), offsetof(fb::EnlightenSystemEntry, m_workspace),
                                          offsetof(fb::EnlightenSystemEntry, m_visibility), offsetof(fb::EnlightenSystemEntry, m_albedo) })
                        relocs.push_back(uint32_t(e + field));
                    g_progress = float(i + 1) / float(systems.size());
                }
                if (g_cancel.load()) { setStatus("cancelled"); return; }
                w.at(offsetof(fb::EnlightenDatabaseHeader, m_systemCount), uint32_t(systems.size()));
                w.at(offsetof(fb::EnlightenDatabaseHeader, m_systems), uint64_t(entries));
                relocs.push_back(uint32_t(offsetof(fb::EnlightenDatabaseHeader, m_systems)));
                std::sort(relocs.begin(), relocs.end());
                relocs.erase(std::unique(relocs.begin(), relocs.end()), relocs.end());

                db::Captured out;
                out.flags = base.flags | 3; // bit0 enabled, bit1 dynamic
                out.blob = std::move(w.b);
                out.relocs = std::move(relocs);
                std::string map = getCurrentMapName();
                const size_t slash = map.find_last_of("/\\");
                if (slash != std::string::npos) map = map.substr(slash + 1);
                if (map.empty()) map = "level";
                const std::string path = db::directory() + "/" + map + "_generated.edb";
                if (!db::writeEdb(path, out)) { setStatus("failed: cannot write " + path); return; }
                setStatus("probe texels");
                writeProbeTexels(base, db::directory() + "/" + map + "_probes.bin");
                logger::info("[enlighten] generator wrote {}: {} systems, {} clusters, {} bytes, {} relocations", path, systems.size(), clusters, out.blob.size(), out.relocs.size());
                logger::info("[enlighten] generator: {:.1f}% of rays hit a surface from behind; {} buried pixels, {} refilled from a neighbour",
                    g_allRays ? 100.0 * double(g_backRays) / double(g_allRays) : 0.0, uint64_t(g_buried), uint64_t(g_refilled));
                g_written = path;
                g_diagReady = true;
                setStatus("wrote " + map + "_generated.edb (" + std::to_string(systems.size()) + " systems) - applying live; calibrate once it has run 15 s");
#endif
            }
            catch (const std::exception& ex)
            {
                setStatus(std::string("failed: ") + ex.what());
            }
        }
    }

    Params& params() { return g_params; }
    bool running() { return g_state.load() == 1 || g_waitScene; }
    float progress() { return g_progress.load(); }
    const std::string& status()
    {
        static std::string copy;
        std::lock_guard<std::mutex> lock(g_statusMutex);
        copy = g_status;
        return copy;
    }

    namespace
    {
        void launch()
        {
        {
            std::string err;
            detail::Original& o = detail::g_orig[2];
            if (detail::readback(o, err) && o.bpp == 4)
            {
                g_dirAtlas = o.pixels;
                g_dirW = o.width; g_dirH = o.height;
            }
            else
                logger::info("[enlighten] generator: no shipped direction atlas ({}), normals keep the bake's side", err);
        }
        g_cancel = false;
        g_progress = 0.0f;
        setStatus("starting");
        g_state = 1;
        g_thread = std::thread([] { run(); g_state = 2; });
        }
    }

    void start()
    {
        if (g_state.load() != 0 || g_waitScene) return;
        if (scene::phase() == scene::Phase::Ready) { launch(); return; }
        scene::start();
        if (scene::phase() == scene::Phase::Failed) { setStatus("failed: " + scene::error()); return; }
        g_waitScene = true;
        setStatus("rasterizing the level");
    }

    void cancel() { g_cancel = true; g_waitScene = false; }

    void tick()
    {
        if (g_waitScene)
        {
            if (scene::phase() == scene::Phase::Ready) { g_waitScene = false; launch(); }
            else if (scene::phase() == scene::Phase::Failed) { g_waitScene = false; setStatus("failed: " + scene::error()); }
            else g_progress = scene::progress() * 0.3f; // rasterizing = first 30% of the bar
        }
        if (g_state.load() == 2)
        {
            if (g_thread.joinable()) g_thread.join();
            g_state = 0;
            if (!g_written.empty())
            {
                std::error_code ec;
                std::filesystem::remove(db::gainPath(), ec);
                db::applyLive(g_written);
                db::requestCalibration();
            }
            g_written.clear();
        }
    }


    void renderUI()
    {
        Params& p = g_params;
        const bool busy = running();
        ImGui::BeginDisabled(busy);
        ImGui::SliderInt("system tile", &p.tile, 16, 256);
        ImGui::SliderFloat("cluster size", &p.clusterSize, 0.5f, 8.0f, "%.1f m");
        ImGui::SliderInt("rays per texel", &p.pixelRays, 16, 512);
        ImGui::SliderInt("max clusters per bucket", &p.maxBasis, 32, 512);
        ImGui::SliderInt("transport smoothing", &p.smoothRadius, 0, 3);
        ImGui::SliderInt("sun visibility slices", &p.visSlices, 8, 32);
        ImGui::SliderInt("sun visibility samples", &p.visSamples, 1, 16);
        ImGui::Checkbox("environment input", &p.environment);
        ImGui::EndDisabled();
        if (!busy)
        {
            if (ImGui::Button("Generate Enlighten systems (.edb)"))
                start();
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("rasterizes the level if needed, then writes the systems and applies them");
        }
        else
        {
            ImGui::ProgressBar(progress(), ImVec2(-1, 0));
            if (ImGui::SmallButton("cancel##gen")) cancel();
        }
        const std::string st = status();
        if (!st.empty()) ImGui::TextWrapped("%s", st.c_str());
        if (!busy)
        {
            if (ImGui::Button(db::calibrationPending() ? "Measure now" : "Calibrate to shipped atlas"))
                db::calibrationPending() ? db::measureNow() : db::requestCalibration();
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("runs by itself after generating; your VE edits are suspended until the systems re-solved");
            ImGui::SameLine();
            if (ImGui::SmallButton("reset calibration"))
            {
                std::error_code ec;
                std::filesystem::remove(db::gainPath(), ec);
                db::reloadGains();
                g_calibNote = "calibration removed";
            }
            if (!db::calibrationNote().empty()) ImGui::TextWrapped("%s", db::calibrationNote().c_str());
            else if (!g_calibNote.empty()) ImGui::TextWrapped("%s", g_calibNote.c_str());
        }
    }

    bool texelInfo(uint32_t texel, TexelInfo& out)
    {
        if (!g_diagReady.load() || texel >= g_diag.size()) return false;
        out = g_diag[texel];
        return true;
    }

    bool texelSurface(uint32_t texel, Vec3& pos, Vec3& nrm, uint8_t& cov)
    {
        if (texel >= g_raster.cov.size()) return false;
        pos = g_raster.pos[texel];
        nrm = g_raster.nrm[texel];
        cov = g_raster.cov[texel];
        return true;
    }

    // replays the transport rays of one texel (same RaySeq, bias, range and back-face test)
    void traceTexel(uint32_t t, std::vector<RayViz>& out)
    {
        constexpr float MISS_LENGTH = 6.0f; // meters drawn for an escaped ray
        out.clear();
        if (!g_diagReady.load() || running() || t >= g_raster.cov.size() || !g_raster.cov[t] || g_raster.cov[t] == COV_FILL) return;
        const Vec3 n = g_raster.nrm[t];
        const Vec3 o = g_raster.pos[t] + n * RAY_BIAS;
        const int R = rayCount();
        RaySeq seq(t);
        for (int r = 0; r < R; ++r)
        {
            const Vec3 d = seq.dir(n, r, R);
            scene::HitInfo h;
            if (!scene::hitInfo(o, d, TRANSPORT_RANGE, h)) { out.push_back({ o, o + d * MISS_LENGTH, kRayEscaped, false, nullptr }); continue; }
            const size_t hit = h.texel;
            uint8_t kind = kRayUnsolved;
            if (hit != ~size_t(0) && fb::dot(d, g_raster.nrm[hit]) > BACK_FACE_DOT) kind = kRayBackFace;
            else if (hit != ~size_t(0) && hit < g_diag.size() && g_diag[hit].system >= 0 && g_diag[hit].cluster >= 0) kind = kRayCluster;
            out.push_back({ o, o + d * h.t, kind, h.terrain, h.mesh });
        }
    }

    void clearDiagnostics()
    {
        g_diagReady = false;
        g_diag.clear();
    }
}

