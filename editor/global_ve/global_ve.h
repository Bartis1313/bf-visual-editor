#pragma once

#include "../types.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// rewrite, use copying with memcpy + ignoring shaders
namespace editor::global_ve
{
    void init();
    void shutdown();
    void clear();

    void onUpdated(fb::VisualEnvironment* ve);

    bool hasCaptured();
    bool isEnabled();
    int enabledCount();
    GlobalVEData& getData();

    void capture(fb::VisualEnvironment* ve);
    void apply(fb::VisualEnvironment* ve);
    void resetToOriginal();
    void resetComponent(int index);
    void applyNightPreset(); // Zavod night colors, sky/envmap scaled down

    void renderTab();

    json serialize();
    void deserialize(const json& j);

    namespace sun
    {
        struct Settings
        {
            bool flaresFollow = false;
            bool discColorEnabled = false;
            float discColor[3] = { 1.0f, 0.85f, 0.6f };
            float discIntensity = 4.0f;
            bool discSizeEnabled = false;
            float discSize = 0.004f;
        };
        Settings& settings();
        bool disc(float* rgb, float& size); // false = engine disc, size < 0 = VE's
        fb::Vec3 direction(float rotationX, float rotationY); // toward the sun
        void onUpdated(fb::VisualEnvironment* ve); // VE update thread
        void clear();
        void renderUI();
        json serialize();
        void deserialize(const json& j);
    }
}