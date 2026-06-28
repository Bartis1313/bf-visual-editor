#include "lights.h"
#include "../editor_context.h"
#include "../../utils/log.h"
#include "../render/render.h"

#include <imgui.h>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>

namespace editor::lights
{
    static std::unordered_map<fb::LocalLightEntityData*, LightDataEntry> entries;
    static bool scanned = false;
    static char filterBuffer[256] = { };
    static int typeFilter = 0;

    std::unordered_map<fb::LocalLightEntityData*, LightDataEntry>& getEntries() { return entries; }

    bool isUnresolvedName(const std::string& name)
    {
        return name.empty() || name == "(dynamic)" || name == "(unknown)" || name == "(unnamed)";
    }

    bool entryWorldPos(const LightDataEntry& entry, fb::Vec3& out)
    {
        for (fb::LocalLightEntity* e : entry.activeEntities)
        {
            if (!e)
                continue;
            out = e->position();
            return true;
        }
        return false;
    }

    std::unordered_map<fb::LocalLightEntityData*, std::string> buildDisplayNames()
    {
        std::unordered_map<fb::LocalLightEntityData*, std::string> result;
        result.reserve(entries.size());

        std::unordered_map<std::string, std::vector<fb::LocalLightEntityData*>> byName;
        for (auto& [ptr, entry] : entries)
        {
            if (isUnresolvedName(entry.assetName))
            {
                char buf[40];
                std::snprintf(buf, sizeof(buf), "light_%p", static_cast<void*>(ptr));
                result[ptr] = buf;
            }
            else
            {
                byName[entry.assetName].push_back(ptr);
            }
        }

        for (auto& [name, ptrs] : byName)
        {
            if (ptrs.size() == 1)
            {
                result[ptrs[0]] = name;
                continue;
            }

            std::sort(ptrs.begin(), ptrs.end(),
                [](fb::LocalLightEntityData* a, fb::LocalLightEntityData* b)
                {
                    fb::Vec3 pa{}, pb{};
                    const bool ha = entryWorldPos(entries[a], pa);
                    const bool hb = entryWorldPos(entries[b], pb);
                    if (ha != hb) return ha;
                    if (ha && hb)
                    {
                        if (pa.m_x != pb.m_x) return pa.m_x < pb.m_x;
                        if (pa.m_y != pb.m_y) return pa.m_y < pb.m_y;
                        if (pa.m_z != pb.m_z) return pa.m_z < pb.m_z;
                    }
                    return a < b;
                });

            for (size_t i = 0; i < ptrs.size(); ++i)
            {
                char buf[256];
                std::snprintf(buf, sizeof(buf), "%s (%zu)", name.c_str(), i + 1);
                result[ptrs[i]] = buf;
            }
        }

        return result;
    }

    void init()
    {
        clear();
    }

    void shutdown()
    {
        clear();
    }

    void clear()
    {
        entries.clear();
        scanned = false;
        filterBuffer[0] = '\0';
        typeFilter = 0;
    }

    static void RegisterLightData(fb::LocalLightEntityData* data, const std::string& assetName, const std::string& containerType, void* container)
    {
        if (!data)
            return;

        if (auto it = entries.find(data); it != entries.end())
        {
            auto& e = it->second;
            const bool placeholderName =
                e.assetName.empty() || e.assetName == "(dynamic)" || e.assetName == "(unknown)";
            const bool placeholderContainer =
                e.containerType.empty() || e.containerType == "Runtime";
            if (placeholderName) e.assetName = assetName;
            if (placeholderContainer) e.containerType = containerType;
            return;
        }

        LightDataEntry& entry = entries[data];
        entry.dataPtr = data;
        entry.assetName = assetName;
        entry.containerType = containerType;

        fb::TypeInfo* typeInfo = data->GetType();
        if (typeInfo && typeInfo->GetTypeCode() == fb::BasicTypesEnum::kTypeCode_Class)
        {
            uint32_t classId = static_cast<fb::ClassInfo*>(typeInfo)->m_ClassId;
            if (classId == fb::SpotLightEntityData::ClassId())
            {
                entry.isSpotLight = true;
                entry.lightType = "SpotLight";
            }
            else if (classId == fb::PointLightEntityData::ClassId())
            {
                entry.isPointLight = true;
                entry.lightType = "PointLight";
            }
            else
            {
                entry.lightType = "LocalLight";
            }
        }
    }

    template<typename ContainerType>
    static void ProcessLightContainer(ContainerType* container, const char* containerTypeName)
    {
        if (!container) return;

        std::string assetName = container->m_Name ? container->m_Name : "(unnamed)";
        for (auto el : container->m_Objects)
        {
            if (!el) continue;

            fb::TypeInfo* typeInfo = el->GetType();
            if (!typeInfo || typeInfo->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                continue;

            fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(typeInfo);
            if (classInfo->isSubclassOf((fb::ClassInfo*)fb::LocalLightEntityData::ClassInfoPtr()))
            {
                RegisterLightData(static_cast<fb::LocalLightEntityData*>(el), assetName, containerTypeName, container);
            }
        }
    }

    static void ProcessObjectBlueprint(fb::ObjectBlueprint* objBp, const char* containerTypeName = "ObjectBlueprint")
    {
        if (!objBp || !objBp->m_Object)
            return;

        const std::string assetName = objBp->m_Name ? objBp->m_Name : "(unnamed)";
        fb::TypeInfo* typeInfo = objBp->m_Object->GetType();
        if (!typeInfo || typeInfo->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
            return;

        fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(typeInfo);

        if (classInfo->m_ClassId == fb::SpotLightEntityData::ClassId() ||
            classInfo->m_ClassId == fb::PointLightEntityData::ClassId() ||
            classInfo->m_ClassId == fb::LocalLightEntityData::ClassId())
        {
            RegisterLightData(static_cast<fb::LocalLightEntityData*>(objBp->m_Object), assetName, containerTypeName, objBp);
        }
    }

    void scanAll()
    {
        fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
        if (!rm)
            return;

        logger::debug("Scanning ResourceManager for light data...");

        for (const auto comp : rm->m_compartments)
        {
            if (!comp) continue;

            for (const auto obj : comp->m_objects)
            {
                if (!obj) continue;

                fb::TypeInfo* typeInfo = obj->GetType();
                if (!typeInfo || typeInfo->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                    continue;

                fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(typeInfo);
                uint32_t classId = classInfo->m_ClassId;

                if (classId == fb::SpatialPrefabBlueprint::ClassId())
                    ProcessLightContainer(reinterpret_cast<fb::SpatialPrefabBlueprint*>(obj), "SpatialPrefabBlueprint");
                else if (classId == fb::ObjectBlueprint::ClassId())
                    ProcessObjectBlueprint(reinterpret_cast<fb::ObjectBlueprint*>(obj));
                else if (classId == fb::PrefabBlueprint::ClassId())
                    ProcessLightContainer(reinterpret_cast<fb::PrefabBlueprint*>(obj), "PrefabBlueprint");
                else if (classId == fb::WorldPartData::ClassId())
                    ProcessLightContainer(reinterpret_cast<fb::WorldPartData*>(obj), "WorldPartData");
                else if (classId == fb::SubWorldData::ClassId())
                    ProcessLightContainer(reinterpret_cast<fb::SubWorldData*>(obj), "SubWorldData");
                else if (classId == fb::LogicPrefabBlueprint::ClassId())
                    ProcessLightContainer(reinterpret_cast<fb::LogicPrefabBlueprint*>(obj), "LogicPrefabBlueprint");
                else if (classId == fb::VehicleBlueprint::ClassId())
                    ProcessObjectBlueprint(reinterpret_cast<fb::VehicleBlueprint*>(obj), "VehicleBlueprint");
            }
        }

        scanned = true;
        logger::debug("Scan complete: found {} light data entries", entries.size());
    }

    void scanExistingEntities()
    {
#if defined(BFVE_GAME_BF4)
        // sub_140CC7EE0 ctor uses sub_1407DAFC0(this+0x30, this, 1)
        fb::EntityList<fb::LocalLightEntity> lights{ (fb::ClassInfo*)fb::LocalLightEntity::ClassInfoPtr(), 0x30 };

        size_t dispatched = 0;
        fb::LocalLightEntity* light;
        while ((light = lights.nextOfKind()) != nullptr)
        {
            if (!light->m_data)
                continue;

            onEntityCreated(light, light->m_data);
            ++dispatched;
        }

        size_t totalActive = 0;
        for (const auto& [ptr, entry] : entries)
            totalActive += entry.activeEntities.size();

        logger::debug("[lights::scanExistingEntities] dispatched={} active={}",
            dispatched, totalActive);

#elif defined(BFVE_GAME_BF3)
        fb::EntityList<fb::LocalLightEntity> lights{ (fb::ClassInfo*)fb::LocalLightEntity::ClassInfoPtr() };
        size_t dispatched = 0;
        fb::LocalLightEntity* light;
        while ((light = lights.nextOfKind()) != nullptr)
        {
            if (!light->m_data)
                continue;

            onEntityCreated(light, light->m_data);
            ++dispatched;
        }
        size_t totalActive = 0;
        for (const auto& [ptr, entry] : entries)
            totalActive += entry.activeEntities.size();

        logger::debug("[lights::scanExistingEntities] dispatched={} active={}",
			dispatched, totalActive);   
#endif
    }

    void scanAndApplyOverrides()
    {
        scanAll();
        scanExistingEntities();
        {
            for (auto& [dataPtr, entry] : entries)
            {
                if (entry.hasOverride && !entry.activeEntities.empty())
                    applyOverride(entry);
            }
        }
        logger::debug("ScanAndApplyOverrides complete");
    }

    void captureOriginal(LightDataEntry& entry)
    {
        if (!entry.dataPtr || entry.origCaptured)
            return;

        auto data = entry.dataPtr;

        entry.origColor = data->m_Color;
        entry.origParticleColorScale = data->m_ParticleColorScale;
        entry.origEnlightenColorScale = data->m_EnlightenColorScale;
        entry.origRadius = data->m_Radius;
        entry.origIntensity = data->m_Intensity;
        entry.origAttenuationOffset = data->m_AttenuationOffset;
        entry.origEnlightenColorMode = static_cast<int>(data->m_EnlightenColorMode);
        entry.origVisible = data->m_Visible;
        entry.origSpecularEnable = data->m_SpecularEnable;
        entry.origEnlightenEnable = data->m_EnlightenEnable;

        entry.color = entry.origColor;
        entry.particleColorScale = entry.origParticleColorScale;
        entry.enlightenColorScale = entry.origEnlightenColorScale;
        entry.radius = entry.origRadius;
        entry.intensity = entry.origIntensity;
        entry.attenuationOffset = entry.origAttenuationOffset;
        entry.enlightenColorMode = entry.origEnlightenColorMode;
        entry.visible = entry.origVisible;
        entry.specularEnable = entry.origSpecularEnable;
        entry.enlightenEnable = entry.origEnlightenEnable;

        if (entry.isSpotLight)
        {
            auto spotData = static_cast<fb::SpotLightEntityData*>(data);
            entry.origSpotShape = static_cast<int>(spotData->m_Shape);
            entry.origConeInnerAngle = spotData->m_ConeInnerAngle;
            entry.origConeOuterAngle = spotData->m_ConeOuterAngle;
            entry.origFrustumFov = spotData->m_FrustumFov;
            entry.origFrustumAspect = spotData->m_FrustumAspect;
            entry.origOrthoWidth = spotData->m_OrthoWidth;
            entry.origOrthoHeight = spotData->m_OrthoHeight;
            entry.origCastShadowsEnable = spotData->m_CastShadowsEnable;
            entry.origCastShadowsMinLevel = static_cast<int>(spotData->m_CastShadowsMinLevel);

            entry.spotShape = entry.origSpotShape;
            entry.coneInnerAngle = entry.origConeInnerAngle;
            entry.coneOuterAngle = entry.origConeOuterAngle;
            entry.frustumFov = entry.origFrustumFov;
            entry.frustumAspect = entry.origFrustumAspect;
            entry.orthoWidth = entry.origOrthoWidth;
            entry.orthoHeight = entry.origOrthoHeight;
            entry.castShadowsEnable = entry.origCastShadowsEnable;
            entry.castShadowsMinLevel = entry.origCastShadowsMinLevel;
        }

        if (entry.isPointLight)
        {
            auto pointData = static_cast<fb::PointLightEntityData*>(data);
            entry.origPointWidth = pointData->m_Width;
            entry.origTranslucencyAmbient = pointData->m_TranslucencyAmbient;
            entry.origTranslucencyScale = pointData->m_TranslucencyScale;
            entry.origTranslucencyPower = pointData->m_TranslucencyPower;
            entry.origTranslucencyDistortion = pointData->m_TranslucencyDistortion;

            entry.pointWidth = entry.origPointWidth;
            entry.translucencyAmbient = entry.origTranslucencyAmbient;
            entry.translucencyScale = entry.origTranslucencyScale;
            entry.translucencyPower = entry.origTranslucencyPower;
            entry.translucencyDistortion = entry.origTranslucencyDistortion;
        }

        entry.origCaptured = true;
    }

    void applyToData(fb::LocalLightEntityData* data, const LightDataEntry& entry)
    {
        if (!data || !entry.hasOverride)
            return;

        data->m_Color = entry.color;
        data->m_ParticleColorScale = entry.particleColorScale;
        data->m_EnlightenColorScale = entry.enlightenColorScale;
        data->m_Radius = entry.radius;
        data->m_Intensity = entry.intensity;
        data->m_AttenuationOffset = entry.attenuationOffset;
        data->m_EnlightenColorMode = static_cast<fb::EnlightenColorMode>(entry.enlightenColorMode);
        data->m_Visible = entry.visible;
        data->m_SpecularEnable = entry.specularEnable;
        data->m_EnlightenEnable = entry.enlightenEnable;

        if (entry.isSpotLight)
        {
            auto spotData = static_cast<fb::SpotLightEntityData*>(data);
            spotData->m_Shape = static_cast<fb::SpotLightShape>(entry.spotShape);
            spotData->m_ConeInnerAngle = entry.coneInnerAngle;
            spotData->m_ConeOuterAngle = entry.coneOuterAngle;
            spotData->m_FrustumFov = entry.frustumFov;
            spotData->m_FrustumAspect = entry.frustumAspect;
            spotData->m_OrthoWidth = entry.orthoWidth;
            spotData->m_OrthoHeight = entry.orthoHeight;
            spotData->m_CastShadowsEnable = entry.castShadowsEnable;
            spotData->m_CastShadowsMinLevel = static_cast<fb::QualityLevel>(entry.castShadowsMinLevel);
        }
        else if (entry.isPointLight)
        {
            auto pointData = static_cast<fb::PointLightEntityData*>(data);
            pointData->m_Width = entry.pointWidth;
            pointData->m_TranslucencyAmbient = entry.translucencyAmbient;
            pointData->m_TranslucencyScale = entry.translucencyScale;
            pointData->m_TranslucencyPower = entry.translucencyPower;
            pointData->m_TranslucencyDistortion = entry.translucencyDistortion;
        }
    }

#if defined(BFVE_GAME_BF4)
    static void clearPropertyOverridesBF4(fb::LocalLightEntity* entity)
    {
        if (!entity)
            return;

        auto base = reinterpret_cast<char*>(entity);
        *reinterpret_cast<uint64_t*>(base + 0x40) &= ~1ULL; // m_Color
        *reinterpret_cast<uint64_t*>(base + 0x90) &= ~1ULL; // m_Intensity
        *reinterpret_cast<uint64_t*>(base + 0xA0) &= ~1ULL; // m_EnlightenColorScale
        *reinterpret_cast<uint64_t*>(base + 0xB0) &= ~1ULL; // m_Radius
        *reinterpret_cast<uint64_t*>(base + 0xC0) &= ~1ULL; // m_ParticleColorScale
    }
#endif

    void applyOverride(LightDataEntry& entry)
    {
        if (!entry.dataPtr)
            return;

        applyToData(entry.dataPtr, entry);

        for (auto entity : entry.activeEntities)
        {
            if (entity)
            {
#if defined(BFVE_GAME_BF4)
                clearPropertyOverridesBF4(entity);
#endif
                entity->setDirty();
            }
        }
    }

    void resetAll()
    {
        for (auto& [dataPtr, entry] : entries)
        {
            if (entry.origCaptured && entry.hasOverride)
            {
                entry.ResetToOriginal();
                applyOverride(entry);
            }
        }
    }

    void onEntityCreated(fb::LocalLightEntity* entity, fb::LocalLightEntityData* data)
    {
        if (!entity || !data)
            return;

        auto it = entries.find(data);
        if (it == entries.end())
        {
            LightDataEntry& entry = entries[data];
            entry.dataPtr = data;
            entry.assetName = "(dynamic)";
            entry.containerType = "Runtime";

            fb::TypeInfo* typeInfo = entity->GetType();
            if (typeInfo && typeInfo->GetTypeCode() == fb::BasicTypesEnum::kTypeCode_Class)
            {
                fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(typeInfo);
                if (classInfo->m_ClassId == fb::SpotLightEntity::ClassId())
                {
                    entry.isSpotLight = true;
                    entry.lightType = "SpotLight";
                }
                else if (classInfo->m_ClassId == fb::PointLightEntity::ClassId())
                {
                    entry.isPointLight = true;
                    entry.lightType = "PointLight";
                }
                else
                {
                    entry.lightType = "LocalLight";
                }
            }
            it = entries.find(data);
        }

        LightDataEntry& entry = it->second;
        entry.activeEntities.insert(entity);

        if (!entry.origCaptured)
            captureOriginal(entry);

        if (entry.hasOverride)
        {
            applyToData(data, entry);
#if defined(BFVE_GAME_BF4)
            clearPropertyOverridesBF4(entity);
#endif
            entity->setDirty();
        }
    }

    void onEntityDestroyed(fb::LocalLightEntity* entity)
    {
        if (!entity)
            return;

        for (auto& [dataPtr, entry] : entries)
        {
            if (entry.activeEntities.erase(entity))
                break;
        }
    }

    fb::LocalLightEntity* closestLightToCrosshair(float* outScreenDist)
    {
        const ImVec2 disp = ImGui::GetIO().DisplaySize;
        const ImVec2 center{ disp.x * 0.5f, disp.y * 0.5f };

        fb::LocalLightEntity* best = nullptr;
        float bestSqr = FLT_MAX;

        for (auto& [dataPtr, entry] : entries)
        {
            for (fb::LocalLightEntity* e : entry.activeEntities)
            {
                if (!e)
                    continue;

                ImVec2 sp;
                if (!render::worldToScreen(e->position(), sp))
                    continue;

                const float dx = sp.x - center.x;
                const float dy = sp.y - center.y;
                const float sqr = dx * dx + dy * dy;
                if (sqr < bestSqr)
                {
                    bestSqr = sqr;
                    best = e;
                }
            }
        }

        if (best && outScreenDist)
            *outScreenDist = std::sqrt(bestSqr);
        return best;
    }

    void renderOverlay()
    {
        if (!showOverlay)
            return;

        fb::LocalLightEntity* closest = closestLightToCrosshair();

        const auto displayNames = buildDisplayNames();

        const ImVec2 disp = ImGui::GetIO().DisplaySize;
        const ImVec2 center{ disp.x * 0.5f, disp.y * 0.5f };
        ImDrawList* dl = ImGui::GetBackgroundDrawList();
        ImFont* font = ImGui::GetFont();
        const float fontSize = ImGui::GetFontSize() * 1.25f;

        for (auto& [dataPtr, entry] : entries)
        {
            bool haveLabel = false;
            float bestCenterSqr = FLT_MAX;
            ImVec2 labelSp{};
            float labelMarkerR = 0.0f;
            float labelDepth = 0.0f;
            ImColor labelCol = render::Colors::LightBlue;

            for (fb::LocalLightEntity* e : entry.activeEntities)
            {
                if (!e)
                    continue;

                const bool isClosest = (e == closest);
                if (showOnlyClosest && !isClosest)
                    continue;

                const fb::Vec3& pos = e->position();
                ImVec2 sp{ };
                float depth = 0.0f;
                if (!render::worldToScreen(pos, sp, depth))
                    continue;

                if (depth > overlayMaxDistance)
                    continue;

                const ImColor col = isClosest ? render::Colors::Yellow : render::Colors::LightBlue;

                float markerR = 0.0f;
                if (isClosest)
                {
                    render::drawSphere(pos, 0.35f, 16, 8, col);
                    markerR = 20.0f;
                }
                else
                {
                    markerR = 700.0f / depth;
                    if (markerR < 3.0f)  markerR = 3.0f;
                    if (markerR > 26.0f) markerR = 26.0f;
                    render::circle(sp, markerR, col);
                }

                const float dx = sp.x - center.x, dy = sp.y - center.y;
                const float csqr = isClosest ? -1.0f : dx * dx + dy * dy;
                if (csqr < bestCenterSqr)
                {
                    bestCenterSqr = csqr;
                    labelSp = sp;
                    labelMarkerR = markerR;
                    labelDepth = depth;
                    labelCol = col;
                    haveLabel = true;
                }
            }

            if (!haveLabel)
                continue;

            const auto nameIt = displayNames.find(dataPtr);
            const char* dispName = nameIt != displayNames.end()
                ? nameIt->second.c_str() : entry.assetName.c_str();

            const size_t count = entry.activeEntities.size();
            char buf[256];
            if (count > 1)
                std::snprintf(buf, sizeof(buf), "%s [%s] x%zu  %.0fm",
                    dispName, entry.lightType.c_str(), count, labelDepth);
            else
                std::snprintf(buf, sizeof(buf), "%s [%s]  %.0fm",
                    dispName, entry.lightType.c_str(), labelDepth);

            const ImVec2 ts = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, buf);
            const ImVec2 tpos{ labelSp.x - ts.x * 0.5f, labelSp.y + labelMarkerR + 3.0f };
            dl->AddText(font, fontSize, ImVec2{ tpos.x + 1.0f, tpos.y + 1.0f }, IM_COL32(0, 0, 0, 200), buf);
            dl->AddText(font, fontSize, tpos, labelCol, buf);
        }
    }
}