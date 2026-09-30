#pragma once

#include "../../SDK/fb.h"
#include <cstdint>
#include <string>
#include <vector>

namespace editor::enlighten::scene
{
    enum class Phase { Idle, Raster, Terrain, Ready, Failed };

    struct Stats
    {
        uint32_t instances = 0, matched = 0, fromRegistrations = 0, fromPlacements = 0;
        uint32_t texels = 0, terrainTexels = 0, lodTexels = 0, lodSnapped = 0, triangles = 0, terrainTiles = 0;
    };

    Phase phase();
    float progress();
    const std::string& error();
    const Stats& stats();

    void start(); // render thread
    void tick(); // render thread
    void tickGameThread(); // VE update thread
    void clear(); // level unload
    void registered(uint16_t handle, fb::MeshAsset* mesh, const fb::LinearTransform& world); // addLightMapHandle hook

    bool prepare(std::string& why);
    uint32_t width();
    uint32_t height();
    const fb::Vec3* positions();
    const fb::Vec3* normals();
    const uint8_t* coverage();
    const uint8_t* kinds(); // 0 mesh, 1 terrain
    const uint32_t* triangles(); // ~0 for terrain texels
    const std::vector<uint32_t>& covered();
    // texel = ~0 when uncharted
    bool hitTexel(const fb::Vec3& o, const fb::Vec3& d, float tmax, size_t& texel, float& t);
    bool blocked(const fb::Vec3& o, const fb::Vec3& d, float tmax);
    struct HitInfo
    {
        size_t texel = ~size_t(0);
        float t = 0.0f;
        fb::MeshAsset* mesh = nullptr;
        bool charted = false, terrain = false;
    };
    bool hitInfo(const fb::Vec3& o, const fb::Vec3& d, float tmax, HitInfo& out);
}
