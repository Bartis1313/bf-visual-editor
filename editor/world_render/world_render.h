#pragma once

#include "../../SDK/sdk.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace editor::world_render
{
    void init();
    void shutdown();
    void clear();

    bool hasCaptured();
    bool isOverrideEnabled();
    void setOverrideEnabled(bool enabled);

    fb::WorldRenderSettings* getOriginal();
    fb::WorldRenderSettings* getEdit();

    void capture();
    void apply();
    void reset();

    void renderTab();

    json serialize();
    void deserialize(const json& j);
}