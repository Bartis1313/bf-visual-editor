#include "lights.h"
#include "../editor_context.h"
#include "../../utils/log.h"

namespace editor::lights
{
    static std::unordered_map<fb::LocalLightEntityData*, LightDataEntry> entries;
    static bool scanned = false;
    static char filterBuffer[256] = { };
    static int typeFilter = 0;

    std::unordered_map<fb::LocalLightEntityData*, LightDataEntry>& getEntries() { return entries; }

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
        if (!data || entries.count(data))
            return;

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

    static void ProcessObjectBlueprint(fb::ObjectBlueprint* objBp)
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
            RegisterLightData(static_cast<fb::LocalLightEntityData*>(objBp->m_Object), assetName, "ObjectBlueprint", objBp);
        }
    }

    void scanAll()
    {
        entries.clear();

        fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
        if (!rm) return;

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
                    ProcessLightContainer(static_cast<fb::SpatialPrefabBlueprint*>(obj), "SpatialPrefabBlueprint");
                else if (classId == fb::ObjectBlueprint::ClassId())
                    ProcessObjectBlueprint(static_cast<fb::ObjectBlueprint*>(obj));
                else if (classId == fb::PrefabBlueprint::ClassId())
                    ProcessLightContainer(static_cast<fb::PrefabBlueprint*>(obj), "PrefabBlueprint");
                else if (classId == fb::WorldPartData::ClassId())
                    ProcessLightContainer(static_cast<fb::WorldPartData*>(obj), "WorldPartData");
                else if (classId == fb::SubWorldData::ClassId())
                    ProcessLightContainer(static_cast<fb::SubWorldData*>(obj), "SubWorldData");
                else if (classId == fb::LogicPrefabBlueprint::ClassId())
                    ProcessLightContainer(static_cast<fb::LogicPrefabBlueprint*>(obj), "LogicPrefabBlueprint");
            }
        }

        scanned = true;
        logger::debug("Scan complete: found {} light data entries", entries.size());
    }

    void scanExistingEntities()
    {
        fb::EntityList<fb::SpotLightEntity> spotLights{ (fb::ClassInfo*)fb::SpotLightEntity::ClassInfoPtr() };
        fb::EntityList<fb::PointLightEntity> pointLights{ (fb::ClassInfo*)fb::PointLightEntity::ClassInfoPtr() };
        fb::EntityList<fb::LocalLightEntity> localLights{ (fb::ClassInfo*)fb::LocalLightEntity::ClassInfoPtr() };

        auto processLight = [](fb::LocalLightEntity* light)
            {
                if (!light || !light->m_data) return;
                onEntityCreated(light, light->m_data);
            };

        fb::SpotLightEntity* spot;
        while ((spot = spotLights.nextOfKind()) != nullptr)
            processLight(spot);

        fb::PointLightEntity* point;
        while ((point = pointLights.nextOfKind()) != nullptr)
            processLight(point);

        fb::LocalLightEntity* local;
        while ((local = localLights.nextOfKind()) != nullptr)
            processLight(local);

        size_t totalActive = 0;
        {
            for (const auto& [ptr, entry] : entries)
                totalActive += entry.activeEntities.size();
        }

        logger::debug("Scanned existing entities: {} active", totalActive);
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

    void applyOverride(LightDataEntry& entry)
    {
        if (!entry.dataPtr)
            return;

        applyToData(entry.dataPtr, entry);

        for (auto entity : entry.activeEntities)
        {
            if (entity)
                entity->setDirty();
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
}