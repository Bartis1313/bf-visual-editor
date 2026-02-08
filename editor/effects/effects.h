#pragma once

#include "../types.h"
#include <vector>
#include <string>

namespace editor::effects
{
    void init();
    void shutdown();
    void clear();

    void scanAssets();

    [[nodiscard]] uint32_t spawnAtTransform(fb::Asset* effect, const fb::LinearTransform& transform);
    void stop(uint32_t handle);
    void stopAll();

    void renderTab();
    void renderOverlay();

    inline std::vector<std::pair<std::string, fb::Asset*>> assets;
    inline std::vector<SpawnedEffect> spawned;
    inline fb::Asset* selected = nullptr;
    inline fb::LinearTransform transform = { };
    inline bool showSpawnedEffects = true;
    inline bool showLocalSphere = true;
}