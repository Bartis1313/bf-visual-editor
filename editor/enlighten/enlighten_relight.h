#pragma once

#include <cstdint>

namespace fb { class VisualEnvironment; }

namespace editor::enlighten::relight
{
    inline bool enabled = false;
    inline float scale = 1.0f;

    void onUpdated(fb::VisualEnvironment* ve); // VE update thread
    void tick(); // render thread
    void flushRestores(); // render thread
    void renderUI();
    void clear(); // level unload
    bool relit(uint32_t texel, float* rgb); // render thread
}
