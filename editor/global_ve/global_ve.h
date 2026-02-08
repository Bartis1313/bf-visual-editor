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

    void renderTab();

    json serialize();
    void deserialize(const json& j);
}