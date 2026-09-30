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
        uint32_t frame = 0;
        uint32_t draws = 0;
        bool valid = false;
    };

    inline bool enabled = false;
    inline uint32_t interval = 15;
    inline uint32_t slices = 10;
    inline bool onlyKnown = true;

    // an empty filter probes every scene draw
    void setShaderFilter(const void* const* shaders, size_t count);

    void init(ID3D11Device* device, ID3D11DeviceContext* context);
    void onPresent(int crosshairX, int crosshairY, int width, int height);
    bool current(Surface& out);
}
