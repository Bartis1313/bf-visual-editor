#pragma once

#include <cstdint>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace editor::textures::pick
{
    struct Hit
    {
        void* pixelShader = nullptr;
        void* srvs[16] = { };
        float cb[3][64] = { };
        bool cbValid[3] = { };
    };
    struct Surface
    {
        Hit hits[32];
        uint32_t hitCount = 0;
        uint32_t frame = 0; // probe frame number, changes with every new result
        uint32_t draws = 0; // draws probed that cycle
        bool valid = false;
    };

    inline bool enabled = false; // draw-call hooks + occlusion queries; off until asked for
    inline uint32_t interval = 15; // frames between probe cycles
    inline uint32_t slices = 10; // a cycle spreads the draws over this many consecutive frames
    inline bool onlyKnown = true; // probe only draws whose pixel shader is in the filter

    // Pixel shaders the material catalogue knows; an empty filter probes every scene draw.
    void setShaderFilter(const void* const* shaders, size_t count);

    void init(ID3D11Device* device, ID3D11DeviceContext* context); // hooks the context's draws
    void onPresent(int crosshairX, int crosshairY, int width, int height);
    bool current(Surface& out);
}
