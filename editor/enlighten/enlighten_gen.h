#pragma once

#include <string>
#include <vector>
#include "../../SDK/fb.h"

namespace editor::enlighten::gen
{
    struct Params
    {
        int tile = 64;
        float clusterSize = 2.0f; // meters
        int pixelRays = 96;
        int maxBasis = 256; // clusters one 4x4 bucket may read
        int smoothRadius = 2;
        int visSlices = 16; // GEVS
        int visSamples = 4; // per cluster and direction
        bool environment = true;
    };

    Params& params();
    bool running();
    float progress();
    const std::string& status();
    void start(); // render thread, needs a 1x bake raster
    void cancel();
    void tick(); // render thread
    void renderUI();

    enum TexelFlags : uint8_t { kBounce = 1, kBuried = 2, kRefilled = 4, kFill = 8, kGutter = 16 };
    struct TexelInfo
    {
        int32_t system = -1, cluster = -1;
        uint16_t rays = 0, rawValid = 0; // cast, and not back-face
        uint16_t pooled = 0; // self included
        uint16_t refs = 0;
        float env = 0.0f, lit = 0.0f, black = 0.0f; // valid-ray shares: escaped, cluster, unsolved surface
        uint8_t flags = 0;
    };
    bool texelInfo(uint32_t texel, TexelInfo& out);
    bool texelSurface(uint32_t texel, fb::Vec3& pos, fb::Vec3& nrm, uint8_t& cov);
    enum RayKind : uint8_t { kRayEscaped, kRayCluster, kRayUnsolved, kRayBackFace };
    struct RayViz { fb::Vec3 from, to; uint8_t kind; bool terrain; fb::MeshAsset* mesh; }; // kind = RayKind
    void traceTexel(uint32_t texel, std::vector<RayViz>& out);
    void clearDiagnostics();
}
