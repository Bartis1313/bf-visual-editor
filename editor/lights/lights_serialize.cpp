#include "lights.h"
#include "../serialize/serialize.h"

#include <cstdio>
#include <cmath>

namespace editor::lights
{
    std::string makeLightKey(const LightDataEntry& entry, fb::LocalLightEntityData* dataPtr, const std::string& displayName)
    {
        const bool unresolved = isUnresolvedName(entry.assetName);

        fb::Vec3 pos{};
        if (entry.saveWithPosition && entryWorldPos(entry, pos))
        {
            const char* base = unresolved ? "light" : entry.assetName.c_str();
            char buf[192];
            std::snprintf(buf, sizeof(buf), "%s@%.2f,%.2f,%.2f", base, pos.m_x, pos.m_y, pos.m_z);
            return buf;
        }

        if (unresolved)
        {
            char ptrKey[32];
            std::snprintf(ptrKey, sizeof(ptrKey), "ptr:%p", static_cast<void*>(dataPtr));
            return ptrKey;
        }

        return unresolved ? entry.assetName : displayName;
    }

    KeyMatch matchLightKey(const std::string& key, const LightDataEntry& entry, fb::LocalLightEntityData* dataPtr, const std::string& displayName)
    {
        if (key.rfind("ptr:", 0) == 0)
        {
            char ptrKey[32];
            std::snprintf(ptrKey, sizeof(ptrKey), "ptr:%p", static_cast<void*>(dataPtr));
            return key == ptrKey ? KeyMatch::Exact : KeyMatch::None;
        }

        const auto at = key.find('@');
        if (at == std::string::npos)
        {
            if (displayName == key)
                return KeyMatch::Exact;
            if (entry.assetName == key)
                return KeyMatch::Name;
            return KeyMatch::None;
        }

        const std::string namePart = key.substr(0, at);
        float x = 0.f, y = 0.f, z = 0.f;
        if (sscanf_s(key.c_str() + at + 1, "%f,%f,%f", &x, &y, &z) != 3)
            return KeyMatch::None;

        const bool nameMatches =
            entry.assetName == namePart ||
            (namePart == "light" && isUnresolvedName(entry.assetName));

        fb::Vec3 pos{};
        if (entryWorldPos(entry, pos) &&
            std::fabs(pos.m_x - x) < 0.01f &&
            std::fabs(pos.m_y - y) < 0.01f &&
            std::fabs(pos.m_z - z) < 0.01f)
        {
            return KeyMatch::Exact;
        }

        return nameMatches ? KeyMatch::Name : KeyMatch::None;
    }

    json serialize(const LightDataEntry& entry)
    {
        json j;

        j["lightType"] = entry.lightType;
        j["hasOverride"] = entry.hasOverride;
        j["saveWithPosition"] = entry.saveWithPosition;

        if (!entry.hasOverride)
            return j;

        JSON_SET_VEC3(j, "color", entry.color);
        JSON_SET_VEC3(j, "particleColorScale", entry.particleColorScale);
        JSON_SET_VEC3(j, "enlightenColorScale", entry.enlightenColorScale);
        JSON_SET(j, "radius", entry.radius);
        JSON_SET(j, "intensity", entry.intensity);
        JSON_SET(j, "attenuationOffset", entry.attenuationOffset);
        JSON_SET(j, "enlightenColorMode", entry.enlightenColorMode);
        JSON_SET(j, "visible", entry.visible);
        JSON_SET(j, "specularEnable", entry.specularEnable);
        JSON_SET(j, "enlightenEnable", entry.enlightenEnable);

        if (entry.isSpotLight)
        {
            JSON_SET(j, "spotShape", entry.spotShape);
            JSON_SET(j, "coneInnerAngle", entry.coneInnerAngle);
            JSON_SET(j, "coneOuterAngle", entry.coneOuterAngle);
            JSON_SET(j, "frustumFov", entry.frustumFov);
            JSON_SET(j, "frustumAspect", entry.frustumAspect);
            JSON_SET(j, "orthoWidth", entry.orthoWidth);
            JSON_SET(j, "orthoHeight", entry.orthoHeight);
            JSON_SET(j, "castShadowsEnable", entry.castShadowsEnable);
            JSON_SET(j, "castShadowsMinLevel", entry.castShadowsMinLevel);
        }

        if (entry.isPointLight)
        {
            JSON_SET(j, "pointWidth", entry.pointWidth);
            JSON_SET(j, "translucencyAmbient", entry.translucencyAmbient);
            JSON_SET(j, "translucencyScale", entry.translucencyScale);
            JSON_SET(j, "translucencyPower", entry.translucencyPower);
            JSON_SET(j, "translucencyDistortion", entry.translucencyDistortion);
        }

        return j;
    }

    void deserialize(const json& j, LightDataEntry& entry)
    {
        JSON_GET_BOOL(j, "hasOverride", entry.hasOverride);
        JSON_GET_BOOL(j, "saveWithPosition", entry.saveWithPosition);

        if (!entry.hasOverride)
            return;

        JSON_GET_VEC3(j, "color", entry.color);
        JSON_GET_VEC3(j, "particleColorScale", entry.particleColorScale);
        JSON_GET_VEC3(j, "enlightenColorScale", entry.enlightenColorScale);
        JSON_GET(j, "radius", entry.radius);
        JSON_GET(j, "intensity", entry.intensity);
        JSON_GET(j, "attenuationOffset", entry.attenuationOffset);
        JSON_GET_INT(j, "enlightenColorMode", entry.enlightenColorMode);
        JSON_GET_BOOL(j, "visible", entry.visible);
        JSON_GET_BOOL(j, "specularEnable", entry.specularEnable);
        JSON_GET_BOOL(j, "enlightenEnable", entry.enlightenEnable);

        if (entry.isSpotLight)
        {
            JSON_GET_INT(j, "spotShape", entry.spotShape);
            JSON_GET(j, "coneInnerAngle", entry.coneInnerAngle);
            JSON_GET(j, "coneOuterAngle", entry.coneOuterAngle);
            JSON_GET(j, "frustumFov", entry.frustumFov);
            JSON_GET(j, "frustumAspect", entry.frustumAspect);
            JSON_GET(j, "orthoWidth", entry.orthoWidth);
            JSON_GET(j, "orthoHeight", entry.orthoHeight);
            JSON_GET_BOOL(j, "castShadowsEnable", entry.castShadowsEnable);
            JSON_GET_INT(j, "castShadowsMinLevel", entry.castShadowsMinLevel);
        }

        if (entry.isPointLight)
        {
            JSON_GET(j, "pointWidth", entry.pointWidth);
            JSON_GET(j, "translucencyAmbient", entry.translucencyAmbient);
            JSON_GET(j, "translucencyScale", entry.translucencyScale);
            JSON_GET(j, "translucencyPower", entry.translucencyPower);
            JSON_GET(j, "translucencyDistortion", entry.translucencyDistortion);
        }
    }
}