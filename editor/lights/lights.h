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

    fb::LocalLightEntity* closestLightToCrosshair(float* outScreenDist = nullptr);
    void renderOverlay();

    inline bool showOverlay = false;
    inline bool showOnlyClosest = false;
    inline float overlayMaxDistance = 100.0f;

    json serialize(const LightDataEntry& entry);
    void deserialize(const json& j, LightDataEntry& entry);

    bool isUnresolvedName(const std::string& name);
    bool entryWorldPos(const LightDataEntry& entry, fb::Vec3& out);

    std::unordered_map<fb::LocalLightEntityData*, std::string> buildDisplayNames();
    enum class KeyMatch { None, Name, Exact };

    std::string makeLightKey(const LightDataEntry& entry, fb::LocalLightEntityData* dataPtr);
    KeyMatch    matchLightKey(const std::string& key, const LightDataEntry& entry, fb::LocalLightEntityData* dataPtr);
}