#pragma once

#include "../types.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace editor::enlighten
{
    void init();
    void shutdown();
    void clear(); // level unload

    void onUpdated(fb::VisualEnvironment* ve); // VE update thread, after the manager blend
    void tick(); // render thread
    void renderOverlay(); // update or render thread, per backend

    void renderTab();

    bool hasSaveData();
    void forceProbeUpdate(int updates); // EnlightenRuntimeSettings::LightProbeForceUpdate for N VE updates
    void onLightMapRegistered(uint16_t handle, fb::MeshAsset* mesh, const fb::LinearTransform& world); // any thread

    json serialize();
    void deserialize(const json& j);
}
