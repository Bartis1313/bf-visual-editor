#pragma once

#include "../types.h"
#include <nlohmann/json.hpp>
#include <unordered_map>

using json = nlohmann::json;

namespace editor::lights
{
    void init();
    void shutdown();
    void clear();

    void scanAll();
    void scanExistingEntities();
    void scanAndApplyOverrides();

    void onEntityCreated(fb::LocalLightEntity* entity, fb::LocalLightEntityData* data);
    void onEntityDestroyed(fb::LocalLightEntity* entity);

    void applyOverride(LightDataEntry& entry);
    void applyToData(fb::LocalLightEntityData* data, const LightDataEntry& entry);
    void captureOriginal(LightDataEntry& entry);
    void resetAll();

    std::unordered_map<fb::LocalLightEntityData*, LightDataEntry>& getEntries();

    void renderTab();
    void renderEditor(LightDataEntry& entry);

    json serialize(const LightDataEntry& entry);
    void deserialize(const json& j, LightDataEntry& entry);
}