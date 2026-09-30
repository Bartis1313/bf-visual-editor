#pragma once

#include <cstdint>

namespace editor::enlighten::look
{
    struct Settings
    {
        bool keepShippedColor = false;
        float saturation = 1.0f;
        float tint[3] = { 1.0f, 1.0f, 1.0f };
        float skyStrength = 1.0f; // relight: sky-lit part
        float sunStrength = 1.0f; // relight: sun/bounce part
    };
    inline Settings settings;
    inline uint32_t generation = 0;

    inline bool isDefault()
    {
        const Settings& s = settings;
        return !s.keepShippedColor && s.saturation == 1.0f && s.tint[0] == 1.0f && s.tint[1] == 1.0f && s.tint[2] == 1.0f &&
            s.skyStrength == 1.0f && s.sunStrength == 1.0f;
    }

    // shipped: null = unknown
    inline void apply(float* rgb, const float* shipped)
    {
        const Settings& s = settings;
        const float sum = rgb[0] + rgb[1] + rgb[2];
        if (s.keepShippedColor && shipped)
        {
            const float ss = shipped[0] + shipped[1] + shipped[2];
            if (ss > 0.0f)
                for (int c = 0; c < 3; ++c) rgb[c] = shipped[c] * (sum / ss);
        }
        if (s.saturation != 1.0f)
        {
            const float mean = sum / 3.0f;
            for (int c = 0; c < 3; ++c) rgb[c] = mean + (rgb[c] - mean) * s.saturation;
        }
        for (int c = 0; c < 3; ++c) rgb[c] = rgb[c] > 0.0f ? rgb[c] * s.tint[c] : 0.0f;
    }

    bool renderUI(bool relight);
}
