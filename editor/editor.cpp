#include "editor.h"
#include <fstream>
#include <ShlObj.h>
#include <set>
#include "../utils/log.h"

namespace fs = std::filesystem;

VisualEnvironmentEditor::VisualEnvironmentEditor()
{
    try
    {
        fs::path configPath = fs::path(GetConfigDir()) / m_ConfigDirectory;
        if (!fs::exists(configPath))
            fs::create_directories(configPath);
    }
    catch (const fs::filesystem_error& err)
    {
        LOG_ERROR("Error %s %s", __FUNCTION__, err.what());
    }
}

bool VisualEnvironmentEditor::IsPlayerAlive() const
{
    fb::ClientGameContext* ctx = fb::ClientGameContext::GetInstance();
    if (!ctx || !ctx->m_clientPlayerManager)
        return false;
    fb::ClientPlayer* localPlayer = ctx->m_clientPlayerManager->m_localPlayer;
    return localPlayer && localPlayer->isAlive();
}

std::string VisualEnvironmentEditor::GetCurrentMapName() const
{
    fb::ClientGameContext* ctx = fb::ClientGameContext::GetInstance();
    if (!ctx || !ctx->m_level || !ctx->m_level->m_levelData || !ctx->m_level->m_levelData->name)
        return {};
    return ctx->m_level->m_levelData->name;
}

fb::WorldRenderSettings* VisualEnvironmentEditor::GetWorldRenderSettings() const
{
    fb::ClientGameContext* ctx = fb::ClientGameContext::GetInstance();
    if (!ctx || !ctx->m_level || !ctx->m_level->m_worldRenderModule || !ctx->m_level->m_worldRenderModule->m_worldRenderer)
        return nullptr;
    return ctx->m_level->m_worldRenderModule->m_worldRenderer->m_worldRenderSettings;
}

fb::VisualEnvironmentManager* VisualEnvironmentEditor::GetManager() const
{
    return fb::VisualEnvironmentManager::GetInstance();
}

std::string VisualEnvironmentEditor::GetConfigDir() const
{
    PWSTR path = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path)))
    {
        int size = WideCharToMultiByte(CP_UTF8, 0, path, -1, nullptr, 0, nullptr, nullptr);
        std::string result(size - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, path, -1, &result[0], size, nullptr, nullptr);
        CoTaskMemFree(path);
        return result;
    }
    return ".";
}

std::string VisualEnvironmentEditor::GetConfigPath(const std::string& mapName) const
{
    return GetConfigDir() + "/" + m_ConfigDirectory + "/" + SanitizeMapName(mapName) + ".json";
}

std::string VisualEnvironmentEditor::SanitizeMapName(const std::string& mapName) const
{
    // fb::clientPlayerCommands::getClientLevelName
    size_t pos = mapName.find_last_of('/');
    if (pos != std::string::npos)
        return mapName.substr(pos + 1);
    return mapName;
}

std::vector<std::string> VisualEnvironmentEditor::GetAvailableConfigs() const
{
    std::vector<std::string> configs;
    try
    {
        fs::path configDir = fs::path(GetConfigDir()) / m_ConfigDirectory;
        if (fs::exists(configDir))
            for (const auto& entry : fs::directory_iterator(configDir))
                if (entry.path().extension() == ".json")
                    configs.push_back(entry.path().stem().string());
    }
    catch (const fs::filesystem_error& err)
    {
        LOG_ERROR("Error %s %s", __FUNCTION__, err.what());
    }
    std::sort(configs.begin(), configs.end());
    return configs;
}

std::string VisualEnvironmentEditor::GetStateName(fb::VisualEnvironmentState* state, int index) const
{
    if (!state)
        return "Unknown";

    auto it = m_StateEntityMap.find(state);
    if (it != m_StateEntityMap.end() && !it->second.name.empty())
    {
        return it->second.name;
    }

    char buf[32];
    snprintf(buf, sizeof(buf), "State_%d", index);
    return buf;
}

void VisualEnvironmentEditor::SetEditorState(EditorState newState)
{
    if (m_EditorState == newState)
        return;

    m_EditorState = newState;
    m_SafeToOperate = (newState == EditorState::Active || newState == EditorState::Ready);
}

bool VisualEnvironmentEditor::IsSafeToOperate() const
{
    return m_SafeToOperate &&
        !m_LevelUnloadingSignaled &&
        m_EditorState != EditorState::Unloading &&
        m_EditorState != EditorState::Idle;
}

void VisualEnvironmentEditor::OnLevelLoaded()
{

    std::string currentMap = GetCurrentMapName();
    LOG_INFO("OnLevelLoaded - CurrentMap: %s\n", currentMap.c_str());

    m_LevelUnloadingSignaled = false;

    m_StateDataMap.clear();
    m_StateOrder.clear();
    m_LastKnownStateCount = 0;
    m_SelectedStateIndex = 0;
    m_HasCapturedOriginals = false;
    m_HasCapturedWorldRenderSettings = false;
    m_CapturedMapName.clear();
    m_VEDataNameMap.clear();
    m_VEDataScanned = false;

    ClearLightData();

    SetEditorState(EditorState::Ready);
}

void VisualEnvironmentEditor::OnLevelUnloading()
{
    LOG_INFO("Level unloading: %s", m_CapturedMapName.c_str());

    m_LevelUnloadingSignaled = true;
    m_SafeToOperate = false;

    m_EditList.clear();
    m_StateDataMap.clear();
    m_StateOrder.clear();
    m_LastKnownStateCount = 0;
    m_HasCapturedOriginals = false;
    m_HasCapturedWorldRenderSettings = false;
    m_CapturedMapName.clear();
    m_VEDataNameMap.clear();

    ClearLightData();
    m_GlobalVEData.Reset();

    SetEditorState(EditorState::Unloading);
}

void VisualEnvironmentEditor::TryRecoverFromUnloading(fb::VisualEnvironmentManager* manager)
{
    if (!manager || manager->m_states.empty())
        return;

    std::string currentMap = GetCurrentMapName();
    if (currentMap.empty())
        return;

    LOG_DEBUG("Recovering from Unloading state, map: %s, states: %zu\n", currentMap.c_str(), manager->m_states.size());

    m_LevelUnloadingSignaled = false;

    SetEditorState(EditorState::Ready);
}

void VisualEnvironmentEditor::InvalidateCapture()
{
    m_HasCapturedOriginals = false;
    m_HasCapturedWorldRenderSettings = false;
    m_CapturedMapName.clear();
    m_SelectedStateIndex = 0;
    m_GlobalVEData.Reset();
}

void VisualEnvironmentEditor::SyncStateList(fb::VisualEnvironmentManager* manager)
{
    if (!manager)
        return;

    std::set<fb::VisualEnvironmentState*> currentStates;
    for (size_t i = 0; i < manager->m_states.size(); i++)
    {
        auto state = manager->m_states[i];
        if (state)
            currentStates.insert(state);
    }

    std::vector<fb::VisualEnvironmentState*> removedStates;
    for (const auto& [state, data] : m_StateDataMap)
    {
        if (currentStates.find(state) == currentStates.end())
        {
            removedStates.push_back(state);
        }
    }

    for (auto state : removedStates)
    {
        LOG_DEBUG("SyncStateList: State removed from manager, calling OnStateRemoved");
        OnStateRemoved(state);
    }

    m_StateOrder.clear();
    for (size_t i = 0; i < manager->m_states.size(); i++)
    {
        auto state = manager->m_states[i];
        if (!state || state->excluded)
            continue;

        m_StateOrder.push_back(state);

        auto itr = m_StateDataMap.find(state);
        if (itr == m_StateDataMap.end())
        {
            LOG_DEBUG("SyncStateList: New state detected, calling CaptureState");
            CaptureState(state);
        }
        else
        {
            itr->second.stateIndex = static_cast<int>(i);
        }
    }

    SortStatesByPriority();
    m_LastKnownStateCount = manager->m_states.size();

    if (m_SelectedStateIndex >= static_cast<int>(m_StateOrder.size()))
        m_SelectedStateIndex = m_StateOrder.empty() ? 0 : static_cast<int>(m_StateOrder.size()) - 1;
}

void VisualEnvironmentEditor::OnManagerUpdateBegin(fb::VisualEnvironmentManager* manager)
{
    if (!manager)
        return;

    size_t currentCount = manager->m_states.size();
    if (m_LevelUnloadingSignaled || m_EditorState == EditorState::Unloading)
    {
        if (currentCount > 0)
        {
            std::string currentMap = GetCurrentMapName();
            if (!currentMap.empty())
            {
                TryRecoverFromUnloading(manager);
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
    }

    if (currentCount == 0)
    {
        if (m_HasCapturedOriginals && m_EditorState == EditorState::Active)
        {
            OnLevelUnloading();
        }
        return;
    }

    if (m_LastKnownStateCount > 0 && currentCount < m_LastKnownStateCount)
    {
        m_StateDataMap.clear();
        m_StateOrder.clear();
        m_LastKnownStateCount = currentCount;
        m_SelectedStateIndex = 0;
        return;
    }

    std::string currentMap = GetCurrentMapName();
    if (m_HasCapturedOriginals && !m_CapturedMapName.empty() &&
        !currentMap.empty() && currentMap != m_CapturedMapName)
    {
        m_StateDataMap.clear();
        m_StateOrder.clear();
        m_LastKnownStateCount = 0;
        m_SelectedStateIndex = 0;
        m_HasCapturedOriginals = false;
        m_HasCapturedWorldRenderSettings = false;
        m_CapturedMapName.clear();
        ClearLightData();
        SetEditorState(EditorState::Ready);
    }

    if (m_EditorState == EditorState::Idle && currentCount > 0)
    {
        SetEditorState(EditorState::Ready);
    }

    SyncStateList(manager);

    if (!m_HasCapturedOriginals)
    {
        if (IsPlayerAlive() && !m_StateOrder.empty() && !currentMap.empty())
        {
            m_CapturedMapName = currentMap;
            m_HasCapturedOriginals = true;
            CaptureWorldRenderSettings();

            if (!m_LightDataScanned)
            {
                ScanAllLightData();
                ScanExistingLightEntities();
            }

            if (!m_VEDataScanned)
            {
                ScanVEDataFromResourceManager();
            }

            SetEditorState(EditorState::Active);

            if (m_AutoLoadConfigs)
            {
                std::string configPath = GetConfigPath(m_CapturedMapName);
                if (fs::exists(configPath))
                    LoadConfig(configPath);
            }
        }
        return;
    }

    if (IsSafeToOperate() && !m_StateOrder.empty())
    {
        for (fb::VisualEnvironmentState* state : m_StateOrder)
        {
            if (!state || state->excluded)
                continue;

            auto itr = m_StateDataMap.find(state);
            if (itr != m_StateDataMap.end())
            {
                CaptureStateData(state, itr->second);
            }
        }
    }
}

void VisualEnvironmentEditor::OnManagerUpdateEnd(fb::VisualEnvironmentManager* manager)
{
    if (!manager || !m_OverridesEnabled || m_EditList.empty())
        return;

    bool appliedAny = false;
    for (auto state : manager->m_states)
    {
        if (!state || state->excluded)
            continue;

        StateHash currentHash = ComputeStateHash(state);
        StateEditEntry* entry = FindEditEntry(currentHash);

        if (!entry)
        {
            entry = FindEditEntryByPriorityAndMask(currentHash.priority, currentHash.componentMask);
            if (entry)
            {
                entry->hash.visibility = currentHash.visibility;
            }
        }

        if (entry)
        {
            ApplyStateEdits(state, entry->editData);
            appliedAny = true;
        }
    }

    if (appliedAny)
        manager->setDirty(true);

    ApplyWorldRenderSettings();

}

void VisualEnvironmentEditor::OnVisualEnvironmentUpdated(fb::VisualEnvironment* ve)
{
    if (!ve)
        return;

    if (m_LevelUnloadingSignaled || m_EditorState == EditorState::Unloading)
        return;

    if (!m_HasCapturedOriginals || !m_OverridesEnabled)
        return;

    if (!m_GlobalVEData.captured)
    {
        CaptureGlobalVEData(ve);
    }

    if (m_GlobalVEData.captured && m_GlobalVEData.globalOverrideEnabled)
    {
        ApplyGlobalVEEdits(ve);
    }
}

void VisualEnvironmentEditor::OnMessage(uint32_t category, uint32_t type)
{
    if (category == MessageIds::LevelLoaded_Category && type == MessageIds::LevelLoaded_Type)
    {
        OnLevelLoaded();
        return;
    }

    if (category == MessageIds::RoundUnloaded_Category && type == MessageIds::RoundUnloaded_Type)
    {
        OnLevelUnloading();
        return;
    }
}

void VisualEnvironmentEditor::OnVisualEnvironmentEntityCreated(fb::VisualEnvironmentEntity* entity, fb::VisualEnvironmentEntityData* data)
{
    if (!entity)
        return;

    fb::VisualEnvironmentState* state = &entity->m_state;
    StateEntityInfo info;
    info.entity = entity;
    info.data = data;
    info.priority = data ? data->m_Priority : 0;

    auto itr = m_VEDataNameMap.find(data);
    if (itr != m_VEDataNameMap.end())
    {
        info.name = itr->second;
    }

    if (info.name.empty() && entity->m_referenceObjectData)
    {
        auto refData = entity->m_referenceObjectData;
        info.priority = refData->m_Priority;
        info.excluded = refData->m_Excluded;

        if (refData->m_Blueprint && refData->m_Blueprint->m_Name)
        {
            info.name = refData->m_Blueprint->m_Name;
            size_t pos = info.name.find_last_of("/\\");
            if (pos != std::string::npos)
                info.name = info.name.substr(pos + 1);
        }
    }

    m_StateEntityMap[state] = info;

    LOG_INFO("Created: '%s' P%d, data=%p, state=%p", info.name.c_str(), info.priority, data, state);
}

void VisualEnvironmentEditor::OnVisualEnvironmentEntityDestroyed(fb::VisualEnvironmentEntity* entity)
{
    if (!entity)
        return;

    fb::VisualEnvironmentState* state = &entity->m_state;

    auto itr = m_StateEntityMap.find(state);
    if (itr != m_StateEntityMap.end())
    {
        LOG_INFO("Destroyed: '%s' P%d", itr->second.name.c_str(), itr->second.priority);
        m_StateEntityMap.erase(itr);
    }
}

template<typename ContainerType>
void VisualEnvironmentEditor::ProcessLightContainer(ContainerType* container, const char* containerTypeName)
{
    if (!container)
        return;

    std::string assetName = container->m_Name ? container->m_Name : "(unnamed)";
    for (auto el : container->m_Objects)
    {
        if (!el)
            continue;

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


void VisualEnvironmentEditor::ScanAllLightData()
{
    std::lock_guard<std::mutex> lock(m_LightDataMutex);
    m_LightDataEntries.clear();

    fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
    if (!rm)
        return;

    LOG_DEBUG("Scanning ResourceManager for light data...");

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
                ProcessObjectBlueprint(static_cast<fb::ObjectBlueprint*>(obj));  // no array of objects, so it's special
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

    m_LightDataScanned = true;
    LOG_DEBUG("Scan complete: found %zu light data entries", m_LightDataEntries.size());
}

void VisualEnvironmentEditor::RegisterLightData(fb::LocalLightEntityData* data, const std::string& assetName, const std::string& containerType, void* container)
{
    if (!data || m_LightDataEntries.count(data))
        return;

    LightDataEntry& entry = m_LightDataEntries[data];
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

void VisualEnvironmentEditor::ProcessObjectBlueprint(fb::ObjectBlueprint* objBp)
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

void VisualEnvironmentEditor::ClearLightData()
{
    std::lock_guard<std::mutex> lock(m_LightDataMutex);
    m_LightDataEntries.clear();
    m_LightDataScanned = false;
}

void VisualEnvironmentEditor::OnLightEntityCreated(fb::LocalLightEntity* entity, fb::LocalLightEntityData* data)
{
    if (!entity || !data)
        return;

    std::lock_guard<std::mutex> lock(m_LightDataMutex);

    auto itr = m_LightDataEntries.find(data);
    if (itr == m_LightDataEntries.end())
    {
        LightDataEntry& entry = m_LightDataEntries[data];
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

        itr = m_LightDataEntries.find(data);
    }

    LightDataEntry& entry = itr->second;
    entry.activeEntities.insert(entity);

    if (!entry.origCaptured)
    {
        CaptureOriginalLightData(entry);
    }

    if (entry.hasOverride)
    {
        ApplyLightOverrideToData(data, entry);
        entity->setDirty();
    }
}


void VisualEnvironmentEditor::OnLightEntityDestroyed(fb::LocalLightEntity* entity)
{
    if (!entity)
        return;

    std::lock_guard<std::mutex> lock(m_LightDataMutex);

    for (auto& [dataPtr, entry] : m_LightDataEntries)
    {
        if (entry.activeEntities.erase(entity))
            break;
    }
}

void VisualEnvironmentEditor::ScanExistingLightEntities()
{
    fb::EntityList<fb::SpotLightEntity> spotLights{ (fb::ClassInfo*)fb::SpotLightEntity::ClassInfoPtr() };
    fb::EntityList<fb::PointLightEntity> pointLights{ (fb::ClassInfo*)fb::PointLightEntity::ClassInfoPtr() };
    fb::EntityList<fb::LocalLightEntity> localLights{ (fb::ClassInfo*)fb::LocalLightEntity::ClassInfoPtr() };

    auto processLight = [this](fb::LocalLightEntity* light)
        {
            if (!light || !light->m_data)
                return;
            OnLightEntityCreated(light, light->m_data);
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
        std::lock_guard<std::mutex> lock(m_LightDataMutex);
        for (const auto& [ptr, entry] : m_LightDataEntries)
            totalActive += entry.activeEntities.size();
    }

    LOG_DEBUG("Scanned existing entities: %zu active\n", totalActive);
}

void VisualEnvironmentEditor::CaptureOriginalLightData(LightDataEntry& entry)
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

    // SpotLight specific
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

    // PointLight specific
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

void VisualEnvironmentEditor::ApplyLightOverrideToData(fb::LocalLightEntityData* data, const LightDataEntry& entry)
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

void VisualEnvironmentEditor::ApplyLightOverride(LightDataEntry& entry)
{
    if (!entry.dataPtr)
        return;

    ApplyLightOverrideToData(entry.dataPtr, entry);

    for (auto entity : entry.activeEntities)
    {
        if (entity)
            entity->setDirty();
    }
}

void VisualEnvironmentEditor::ScanAndApplyLightOverrides()
{
    ScanAllLightData();
    ScanExistingLightEntities();
    {
        std::lock_guard<std::mutex> lock(m_LightDataMutex);
        for (auto& [dataPtr, entry] : m_LightDataEntries)
        {
            if (entry.hasOverride && !entry.activeEntities.empty())
            {
                ApplyLightOverride(entry);
            }
        }
    }

    LOG_DEBUG("ScanAndApplyLightOverrides complete\n");
}

bool VisualEnvironmentEditor::HasCapturedState(fb::VisualEnvironmentState* state) const
{
    auto itr = m_StateDataMap.find(state);
    return itr != m_StateDataMap.end() && itr->second.captured;
}

void VisualEnvironmentEditor::SortStatesByPriority()
{
    std::sort(m_StateOrder.begin(), m_StateOrder.end(),
        [](fb::VisualEnvironmentState* a, fb::VisualEnvironmentState* b)
        {
            if (!a) return false;
            if (!b) return true;
            return a->priority < b->priority;
        });
}

void VisualEnvironmentEditor::ApplyStateOverride(fb::VisualEnvironmentState* state, StateEditData& data)
{
    if (!state || !data.captured || !data.overrideEnabled)
        return;

    ApplyStateEdits(state, data);

    fb::VisualEnvironmentManager* manager = GetManager();
    if (manager)
        manager->setDirty(true);
}

void VisualEnvironmentEditor::CaptureStateData(fb::VisualEnvironmentState* state, StateEditData& data)
{
    if (!state || m_LevelUnloadingSignaled || !IsSafeToOperate())
        return;

    data.captured = true;
    data.statePtr = state;

#define CAPTURE_COMPONENT(ptr, hasFlag, capturedPtr, origMember, editMember, CopyFunc) \
    if (!m_LevelUnloadingSignaled && state->ptr) { \
        if (!data.hasFlag || data.capturedPtr != static_cast<void*>(state->ptr)) { \
            data.hasFlag = true; \
            data.capturedPtr = static_cast<void*>(state->ptr); \
            CopyFunc(&data.origMember, state->ptr); \
            CopyFunc(&data.editMember, state->ptr); \
        } \
    }

    CAPTURE_COMPONENT(outdoorLight, hasOutdoorLight, capturedOutdoorLight, origOutdoorLight, editOutdoorLight, CopyOutdoorLight);
    CAPTURE_COMPONENT(enlighten, hasEnlighten, capturedEnlighten, origEnlighten, editEnlighten, CopyEnlighten);
    CAPTURE_COMPONENT(tonemap, hasTonemap, capturedTonemap, origTonemap, editTonemap, CopyTonemap);
    CAPTURE_COMPONENT(colorCorrection, hasColorCorrection, capturedColorCorrection, origColorCorrection, editColorCorrection, CopyColorCorrection);
    CAPTURE_COMPONENT(sky, hasSky, capturedSky, origSky, editSky, CopySky);
    CAPTURE_COMPONENT(fog, hasFog, capturedFog, origFog, editFog, CopyFog);
    CAPTURE_COMPONENT(wind, hasWind, capturedWind, origWind, editWind, CopyWind);
    CAPTURE_COMPONENT(sunFlare, hasSunFlare, capturedSunFlare, origSunFlare, editSunFlare, CopySunFlare);
    CAPTURE_COMPONENT(dynamicAO, hasDynamicAO, capturedDynamicAO, origDynamicAO, editDynamicAO, CopyDynamicAO);
    CAPTURE_COMPONENT(dof, hasDof, capturedDof, origDof, editDof, CopyDof);
    CAPTURE_COMPONENT(vignette, hasVignette, capturedVignette, origVignette, editVignette, CopyVignette);
    CAPTURE_COMPONENT(filmGrain, hasFilmGrain, capturedFilmGrain, origFilmGrain, editFilmGrain, CopyFilmGrain);
    CAPTURE_COMPONENT(lensScope, hasLensScope, capturedLensScope, origLensScope, editLensScope, CopyLensScope);
    CAPTURE_COMPONENT(cameraParams, hasCameraParams, capturedCameraParams, origCameraParams, editCameraParams, CopyCameraParams);
    CAPTURE_COMPONENT(screenEffect, hasScreenEffect, capturedScreenEffect, origScreenEffect, editScreenEffect, CopyScreenEffect);
    CAPTURE_COMPONENT(damageEffect, hasDamageEffect, capturedDamageEffect, origDamageEffect, editDamageEffect, CopyDamageEffect);
    CAPTURE_COMPONENT(planarReflection, hasPlanarReflection, capturedPlanarReflection, origPlanarReflection, editPlanarReflection, CopyPlanarReflection);
    CAPTURE_COMPONENT(dynamicEnvmap, hasDynamicEnvmap, capturedDynamicEnvmap, origDynamicEnvmap, editDynamicEnvmap, CopyDynamicEnvmap);
    CAPTURE_COMPONENT(characterLighting, hasCharacterLighting, capturedCharacterLighting, origCharacterLighting, editCharacterLighting, CopyCharacterLighting);
    CAPTURE_COMPONENT(motionBlur, hasMotionBlur, capturedMotionBlur, origMotionBlur, editMotionBlur, CopyMotionBlur);

#undef CAPTURE_COMPONENT
}

void VisualEnvironmentEditor::CaptureState(fb::VisualEnvironmentState* state)
{
    if (!state)
        return;

    auto existingIt = m_StateDataMap.find(state);
    if (existingIt != m_StateDataMap.end() && existingIt->second.captured)
        return;

    StateHash hash = ComputeStateHash(state);

    LOG_INFO("CaptureState: P%d M%08X (id=%d)", hash.priority, hash.componentMask, state->stateId);

    StateEditData& data = m_StateDataMap[state];
    data.statePtr = state;
    data.stateName = GetStateName(state, static_cast<int>(m_StateOrder.size()));

    CaptureStateData(state, data);

    if (std::find(m_StateOrder.begin(), m_StateOrder.end(), state) == m_StateOrder.end())
    {
        m_StateOrder.push_back(state);
        SortStatesByPriority();
    }

    StateEditEntry* existingEdit = FindEditEntry(hash);
    if (existingEdit)
    {
        LOG_INFO("CaptureState: Found existing edit entry, restoring values");
        RestoreEditValuesFromEntry(data, existingEdit->editData);

        if (m_OverridesEnabled && data.overrideEnabled)
        {
            ApplyStateEdits(state, data);
            fb::VisualEnvironmentManager* manager = GetManager();
            if (manager)
                manager->setDirty(true);
        }
    }
}


void VisualEnvironmentEditor::OnStateRemoved(fb::VisualEnvironmentState* state)
{
    if (!state)
        return;

    auto itr = m_StateDataMap.find(state);
    if (itr == m_StateDataMap.end())
        return;

    LOG_DEBUG("OnStateRemoved: P%d (override=%s)", state->priority, itr->second.overrideEnabled ? "YES" : "no");

    m_StateDataMap.erase(itr);
    auto orderIt = std::find(m_StateOrder.begin(), m_StateOrder.end(), state);
    if (orderIt != m_StateOrder.end())
    {
        m_StateOrder.erase(orderIt);
    }

}
void VisualEnvironmentEditor::ApplyStateEdits(fb::VisualEnvironmentState* state, const StateEditData& data)
{
    if (!state || !data.captured || m_LevelUnloadingSignaled || !IsSafeToOperate())
        return;

#define APPLY_COMPONENT(ptr, hasFlag, editMember, CopyFunc) \
    if (!m_LevelUnloadingSignaled && data.hasFlag && state->ptr) { \
        CopyFunc(state->ptr, &data.editMember); \
    }

    APPLY_COMPONENT(outdoorLight, hasOutdoorLight, editOutdoorLight, CopyOutdoorLight);
    APPLY_COMPONENT(enlighten, hasEnlighten, editEnlighten, CopyEnlighten);
    APPLY_COMPONENT(tonemap, hasTonemap, editTonemap, CopyTonemap);
    APPLY_COMPONENT(colorCorrection, hasColorCorrection, editColorCorrection, CopyColorCorrection);
    APPLY_COMPONENT(sky, hasSky, editSky, CopySky);
    APPLY_COMPONENT(fog, hasFog, editFog, CopyFog);
    APPLY_COMPONENT(wind, hasWind, editWind, CopyWind);
    APPLY_COMPONENT(sunFlare, hasSunFlare, editSunFlare, CopySunFlare);
    APPLY_COMPONENT(dynamicAO, hasDynamicAO, editDynamicAO, CopyDynamicAO);
    APPLY_COMPONENT(dof, hasDof, editDof, CopyDof);
    APPLY_COMPONENT(vignette, hasVignette, editVignette, CopyVignette);
    APPLY_COMPONENT(filmGrain, hasFilmGrain, editFilmGrain, CopyFilmGrain);
    APPLY_COMPONENT(lensScope, hasLensScope, editLensScope, CopyLensScope);
    APPLY_COMPONENT(cameraParams, hasCameraParams, editCameraParams, CopyCameraParams);
    APPLY_COMPONENT(screenEffect, hasScreenEffect, editScreenEffect, CopyScreenEffect);
    APPLY_COMPONENT(damageEffect, hasDamageEffect, editDamageEffect, CopyDamageEffect);
    APPLY_COMPONENT(planarReflection, hasPlanarReflection, editPlanarReflection, CopyPlanarReflection);
    APPLY_COMPONENT(dynamicEnvmap, hasDynamicEnvmap, editDynamicEnvmap, CopyDynamicEnvmap);
    APPLY_COMPONENT(characterLighting, hasCharacterLighting, editCharacterLighting, CopyCharacterLighting);
    APPLY_COMPONENT(motionBlur, hasMotionBlur, editMotionBlur, CopyMotionBlur);

#undef APPLY_COMPONENT
}

void VisualEnvironmentEditor::RestoreState(fb::VisualEnvironmentState* state, const StateEditData& data)
{
    if (!state || !data.captured)
        return;

#define RESTORE_COMPONENT(ptr, hasFlag, origMember, CopyFunc) \
    if (data.hasFlag && state->ptr) { CopyFunc(state->ptr, &data.origMember); }

    RESTORE_COMPONENT(outdoorLight, hasOutdoorLight, origOutdoorLight, CopyOutdoorLight);
    RESTORE_COMPONENT(enlighten, hasEnlighten, origEnlighten, CopyEnlighten);
    RESTORE_COMPONENT(tonemap, hasTonemap, origTonemap, CopyTonemap);
    RESTORE_COMPONENT(colorCorrection, hasColorCorrection, origColorCorrection, CopyColorCorrection);
    RESTORE_COMPONENT(sky, hasSky, origSky, CopySky);
    RESTORE_COMPONENT(fog, hasFog, origFog, CopyFog);
    RESTORE_COMPONENT(wind, hasWind, origWind, CopyWind);
    RESTORE_COMPONENT(sunFlare, hasSunFlare, origSunFlare, CopySunFlare);
    RESTORE_COMPONENT(dynamicAO, hasDynamicAO, origDynamicAO, CopyDynamicAO);
    RESTORE_COMPONENT(dof, hasDof, origDof, CopyDof);
    RESTORE_COMPONENT(vignette, hasVignette, origVignette, CopyVignette);
    RESTORE_COMPONENT(filmGrain, hasFilmGrain, origFilmGrain, CopyFilmGrain);
    RESTORE_COMPONENT(lensScope, hasLensScope, origLensScope, CopyLensScope);
    RESTORE_COMPONENT(cameraParams, hasCameraParams, origCameraParams, CopyCameraParams);
    RESTORE_COMPONENT(screenEffect, hasScreenEffect, origScreenEffect, CopyScreenEffect);
    RESTORE_COMPONENT(damageEffect, hasDamageEffect, origDamageEffect, CopyDamageEffect);
    RESTORE_COMPONENT(planarReflection, hasPlanarReflection, origPlanarReflection, CopyPlanarReflection);
    RESTORE_COMPONENT(dynamicEnvmap, hasDynamicEnvmap, origDynamicEnvmap, CopyDynamicEnvmap);
    RESTORE_COMPONENT(characterLighting, hasCharacterLighting, origCharacterLighting, CopyCharacterLighting);
    RESTORE_COMPONENT(motionBlur, hasMotionBlur, origMotionBlur, CopyMotionBlur);

#undef RESTORE_COMPONENT
}

void VisualEnvironmentEditor::RestoreAllStates()
{
    for (auto& [state, data] : m_StateDataMap)
    {
        if (state && data.captured)
            RestoreState(state, data);
    }

    fb::VisualEnvironmentManager* manager = GetManager();
    if (manager)
        manager->setDirty(true);
}

void VisualEnvironmentEditor::ResetStateToOriginal(StateEditData& data)
{
    if (!data.captured)
        return;

#define RESET_COMPONENT(hasFlag, origMember, editMember, CopyFunc) \
    if (data.hasFlag) { CopyFunc(&data.editMember, &data.origMember); }

    RESET_COMPONENT(hasOutdoorLight, origOutdoorLight, editOutdoorLight, CopyOutdoorLight);
    RESET_COMPONENT(hasEnlighten, origEnlighten, editEnlighten, CopyEnlighten);
    RESET_COMPONENT(hasTonemap, origTonemap, editTonemap, CopyTonemap);
    RESET_COMPONENT(hasColorCorrection, origColorCorrection, editColorCorrection, CopyColorCorrection);
    RESET_COMPONENT(hasSky, origSky, editSky, CopySky);
    RESET_COMPONENT(hasFog, origFog, editFog, CopyFog);
    RESET_COMPONENT(hasWind, origWind, editWind, CopyWind);
    RESET_COMPONENT(hasSunFlare, origSunFlare, editSunFlare, CopySunFlare);
    RESET_COMPONENT(hasDynamicAO, origDynamicAO, editDynamicAO, CopyDynamicAO);
    RESET_COMPONENT(hasDof, origDof, editDof, CopyDof);
    RESET_COMPONENT(hasVignette, origVignette, editVignette, CopyVignette);
    RESET_COMPONENT(hasFilmGrain, origFilmGrain, editFilmGrain, CopyFilmGrain);
    RESET_COMPONENT(hasLensScope, origLensScope, editLensScope, CopyLensScope);
    RESET_COMPONENT(hasCameraParams, origCameraParams, editCameraParams, CopyCameraParams);
    RESET_COMPONENT(hasScreenEffect, origScreenEffect, editScreenEffect, CopyScreenEffect);
    RESET_COMPONENT(hasDamageEffect, origDamageEffect, editDamageEffect, CopyDamageEffect);
    RESET_COMPONENT(hasPlanarReflection, origPlanarReflection, editPlanarReflection, CopyPlanarReflection);
    RESET_COMPONENT(hasDynamicEnvmap, origDynamicEnvmap, editDynamicEnvmap, CopyDynamicEnvmap);
    RESET_COMPONENT(hasCharacterLighting, origCharacterLighting, editCharacterLighting, CopyCharacterLighting);
    RESET_COMPONENT(hasMotionBlur, origMotionBlur, editMotionBlur, CopyMotionBlur);

#undef RESET_COMPONENT
}

void VisualEnvironmentEditor::CaptureWorldRenderSettings()
{
    fb::WorldRenderSettings* settings = GetWorldRenderSettings();
    if (!settings)
        return;
    CopyWorldRenderSettings(&m_OriginalWorldRenderSettings, settings);
    CopyWorldRenderSettings(&m_EditWorldRenderSettings, settings);
    m_HasCapturedWorldRenderSettings = true;
}

void VisualEnvironmentEditor::RestoreWorldRenderSettings()
{
    if (!m_HasCapturedWorldRenderSettings)
        return;
    fb::WorldRenderSettings* settings = GetWorldRenderSettings();
    if (settings)
        CopyWorldRenderSettings(settings, &m_OriginalWorldRenderSettings);
}

void VisualEnvironmentEditor::ApplyWorldRenderSettings()
{
    if (!m_HasCapturedWorldRenderSettings || !m_WorldRenderOverrideEnabled)
        return;

    if (!IsSafeToOperate())
        return;

    fb::WorldRenderSettings* settings = GetWorldRenderSettings();
    if (!settings)
        return;

    CopyWorldRenderSettings(settings, &m_EditWorldRenderSettings);
}

void VisualEnvironmentEditor::CaptureGlobalVEData(fb::VisualEnvironment* ve)
{
    if (!ve || m_GlobalVEData.captured)
        return;

    if (m_LevelUnloadingSignaled || !IsSafeToOperate())
        return;

    CopyOutdoorLight(&m_GlobalVEData.origOutdoorLight, &ve->outdoorLight);
    CopyOutdoorLight(&m_GlobalVEData.editOutdoorLight, &ve->outdoorLight);

    CopyEnlighten(&m_GlobalVEData.origEnlighten, &ve->enlighten);
    CopyEnlighten(&m_GlobalVEData.editEnlighten, &ve->enlighten);

    CopyTonemap(&m_GlobalVEData.origTonemap, &ve->tonemap);
    CopyTonemap(&m_GlobalVEData.editTonemap, &ve->tonemap);

    CopyColorCorrection(&m_GlobalVEData.origColorCorrection, &ve->colorCorrection);
    CopyColorCorrection(&m_GlobalVEData.editColorCorrection, &ve->colorCorrection);

    CopySky(&m_GlobalVEData.origSky, &ve->sky);
    CopySky(&m_GlobalVEData.editSky, &ve->sky);

    CopyFog(&m_GlobalVEData.origFog, &ve->fog);
    CopyFog(&m_GlobalVEData.editFog, &ve->fog);

    CopyWind(&m_GlobalVEData.origWind, &ve->wind);
    CopyWind(&m_GlobalVEData.editWind, &ve->wind);

    CopySunFlare(&m_GlobalVEData.origSunFlare, &ve->sunFlare);
    CopySunFlare(&m_GlobalVEData.editSunFlare, &ve->sunFlare);

    CopyDynamicAO(&m_GlobalVEData.origDynamicAO, &ve->dynamicAO);
    CopyDynamicAO(&m_GlobalVEData.editDynamicAO, &ve->dynamicAO);

    CopyDof(&m_GlobalVEData.origDof, &ve->dof);
    CopyDof(&m_GlobalVEData.editDof, &ve->dof);

    CopyVignette(&m_GlobalVEData.origVignette, &ve->vignette);
    CopyVignette(&m_GlobalVEData.editVignette, &ve->vignette);

    CopyFilmGrain(&m_GlobalVEData.origFilmGrain, &ve->filmGrain);
    CopyFilmGrain(&m_GlobalVEData.editFilmGrain, &ve->filmGrain);

    CopyLensScope(&m_GlobalVEData.origLensScope, &ve->lensScope);
    CopyLensScope(&m_GlobalVEData.editLensScope, &ve->lensScope);

    CopyCameraParams(&m_GlobalVEData.origCameraParams, &ve->cameraParams);
    CopyCameraParams(&m_GlobalVEData.editCameraParams, &ve->cameraParams);

    CopyScreenEffect(&m_GlobalVEData.origScreenEffect, &ve->screenEffect);
    CopyScreenEffect(&m_GlobalVEData.editScreenEffect, &ve->screenEffect);

    CopyDamageEffect(&m_GlobalVEData.origDamageEffect, &ve->damageEffect);
    CopyDamageEffect(&m_GlobalVEData.editDamageEffect, &ve->damageEffect);

    CopyPlanarReflection(&m_GlobalVEData.origPlanarReflection, &ve->planarReflection);
    CopyPlanarReflection(&m_GlobalVEData.editPlanarReflection, &ve->planarReflection);

    CopyDynamicEnvmap(&m_GlobalVEData.origDynamicEnvmap, &ve->dynamicEnvmap);
    CopyDynamicEnvmap(&m_GlobalVEData.editDynamicEnvmap, &ve->dynamicEnvmap);

    CopyCharacterLighting(&m_GlobalVEData.origCharacterLighting, &ve->characterLighting);
    CopyCharacterLighting(&m_GlobalVEData.editCharacterLighting, &ve->characterLighting);

    CopyMotionBlur(&m_GlobalVEData.origMotionBlur, &ve->motionBlur);
    CopyMotionBlur(&m_GlobalVEData.editMotionBlur, &ve->motionBlur);

    m_GlobalVEData.captured = true;
}

void VisualEnvironmentEditor::ApplyGlobalVEEdits(fb::VisualEnvironment* ve)
{
    if (!ve || !m_GlobalVEData.captured || !m_GlobalVEData.globalOverrideEnabled)
        return;

    if (m_GlobalVEData.outdoorLightOverrideEnabled)
        CopyOutdoorLight(&ve->outdoorLight, &m_GlobalVEData.editOutdoorLight);

    if (m_GlobalVEData.enlightenOverrideEnabled)
        CopyEnlighten(&ve->enlighten, &m_GlobalVEData.editEnlighten);

    if (m_GlobalVEData.tonemapOverrideEnabled)
        CopyTonemap(&ve->tonemap, &m_GlobalVEData.editTonemap);

    if (m_GlobalVEData.colorCorrectionOverrideEnabled)
        CopyColorCorrection(&ve->colorCorrection, &m_GlobalVEData.editColorCorrection);

    if (m_GlobalVEData.skyOverrideEnabled)
        CopySky(&ve->sky, &m_GlobalVEData.editSky);

    if (m_GlobalVEData.fogOverrideEnabled)
        CopyFog(&ve->fog, &m_GlobalVEData.editFog);

    if (m_GlobalVEData.windOverrideEnabled)
        CopyWind(&ve->wind, &m_GlobalVEData.editWind);

    if (m_GlobalVEData.sunFlareOverrideEnabled)
        CopySunFlare(&ve->sunFlare, &m_GlobalVEData.editSunFlare);

    if (m_GlobalVEData.dynamicAOOverrideEnabled)
        CopyDynamicAO(&ve->dynamicAO, &m_GlobalVEData.editDynamicAO);

    if (m_GlobalVEData.dofOverrideEnabled)
        CopyDof(&ve->dof, &m_GlobalVEData.editDof);

    if (m_GlobalVEData.vignetteOverrideEnabled)
        CopyVignette(&ve->vignette, &m_GlobalVEData.editVignette);

    if (m_GlobalVEData.filmGrainOverrideEnabled)
        CopyFilmGrain(&ve->filmGrain, &m_GlobalVEData.editFilmGrain);

    if (m_GlobalVEData.lensScopeOverrideEnabled)
        CopyLensScope(&ve->lensScope, &m_GlobalVEData.editLensScope);

    if (m_GlobalVEData.cameraParamsOverrideEnabled)
        CopyCameraParams(&ve->cameraParams, &m_GlobalVEData.editCameraParams);

    if (m_GlobalVEData.screenEffectOverrideEnabled)
        CopyScreenEffect(&ve->screenEffect, &m_GlobalVEData.editScreenEffect);

    if (m_GlobalVEData.damageEffectOverrideEnabled)
        CopyDamageEffect(&ve->damageEffect, &m_GlobalVEData.editDamageEffect);

    if (m_GlobalVEData.planarReflectionOverrideEnabled)
        CopyPlanarReflection(&ve->planarReflection, &m_GlobalVEData.editPlanarReflection);

    if (m_GlobalVEData.dynamicEnvmapOverrideEnabled)
        CopyDynamicEnvmap(&ve->dynamicEnvmap, &m_GlobalVEData.editDynamicEnvmap);

    if (m_GlobalVEData.characterLightingOverrideEnabled)
        CopyCharacterLighting(&ve->characterLighting, &m_GlobalVEData.editCharacterLighting);

    if (m_GlobalVEData.motionBlurOverrideEnabled)
        CopyMotionBlur(&ve->motionBlur, &m_GlobalVEData.editMotionBlur);
}

void VisualEnvironmentEditor::ResetGlobalVEToOriginal()
{
    if (!m_GlobalVEData.captured)
        return;

    CopyOutdoorLight(&m_GlobalVEData.editOutdoorLight, &m_GlobalVEData.origOutdoorLight);
    CopyEnlighten(&m_GlobalVEData.editEnlighten, &m_GlobalVEData.origEnlighten);
    CopyTonemap(&m_GlobalVEData.editTonemap, &m_GlobalVEData.origTonemap);
    CopyColorCorrection(&m_GlobalVEData.editColorCorrection, &m_GlobalVEData.origColorCorrection);
    CopySky(&m_GlobalVEData.editSky, &m_GlobalVEData.origSky);
    CopyFog(&m_GlobalVEData.editFog, &m_GlobalVEData.origFog);
    CopyWind(&m_GlobalVEData.editWind, &m_GlobalVEData.origWind);
    CopySunFlare(&m_GlobalVEData.editSunFlare, &m_GlobalVEData.origSunFlare);
    CopyDynamicAO(&m_GlobalVEData.editDynamicAO, &m_GlobalVEData.origDynamicAO);
    CopyDof(&m_GlobalVEData.editDof, &m_GlobalVEData.origDof);
    CopyVignette(&m_GlobalVEData.editVignette, &m_GlobalVEData.origVignette);
    CopyFilmGrain(&m_GlobalVEData.editFilmGrain, &m_GlobalVEData.origFilmGrain);
    CopyLensScope(&m_GlobalVEData.editLensScope, &m_GlobalVEData.origLensScope);
    CopyCameraParams(&m_GlobalVEData.editCameraParams, &m_GlobalVEData.origCameraParams);
    CopyScreenEffect(&m_GlobalVEData.editScreenEffect, &m_GlobalVEData.origScreenEffect);
    CopyDamageEffect(&m_GlobalVEData.editDamageEffect, &m_GlobalVEData.origDamageEffect);
    CopyPlanarReflection(&m_GlobalVEData.editPlanarReflection, &m_GlobalVEData.origPlanarReflection);
    CopyDynamicEnvmap(&m_GlobalVEData.editDynamicEnvmap, &m_GlobalVEData.origDynamicEnvmap);
    CopyCharacterLighting(&m_GlobalVEData.editCharacterLighting, &m_GlobalVEData.origCharacterLighting);
    CopyMotionBlur(&m_GlobalVEData.editMotionBlur, &m_GlobalVEData.origMotionBlur);
}

void VisualEnvironmentEditor::ResetGlobalVEComponent(int componentIndex)
{
    if (!m_GlobalVEData.captured)
        return;

    switch (componentIndex)
    {
    case 0:  CopyOutdoorLight(&m_GlobalVEData.editOutdoorLight, &m_GlobalVEData.origOutdoorLight); break;
    case 1:  CopyEnlighten(&m_GlobalVEData.editEnlighten, &m_GlobalVEData.origEnlighten); break;
    case 2:  CopyTonemap(&m_GlobalVEData.editTonemap, &m_GlobalVEData.origTonemap); break;
    case 3:  CopyColorCorrection(&m_GlobalVEData.editColorCorrection, &m_GlobalVEData.origColorCorrection); break;
    case 4:  CopySky(&m_GlobalVEData.editSky, &m_GlobalVEData.origSky); break;
    case 5:  CopyFog(&m_GlobalVEData.editFog, &m_GlobalVEData.origFog); break;
    case 6:  CopyWind(&m_GlobalVEData.editWind, &m_GlobalVEData.origWind); break;
    case 7:  CopySunFlare(&m_GlobalVEData.editSunFlare, &m_GlobalVEData.origSunFlare); break;
    case 8:  CopyDynamicAO(&m_GlobalVEData.editDynamicAO, &m_GlobalVEData.origDynamicAO); break;
    case 9:  CopyDof(&m_GlobalVEData.editDof, &m_GlobalVEData.origDof); break;
    case 10: CopyVignette(&m_GlobalVEData.editVignette, &m_GlobalVEData.origVignette); break;
    case 11: CopyFilmGrain(&m_GlobalVEData.editFilmGrain, &m_GlobalVEData.origFilmGrain); break;
    case 12: CopyLensScope(&m_GlobalVEData.editLensScope, &m_GlobalVEData.origLensScope); break;
    case 13: CopyCameraParams(&m_GlobalVEData.editCameraParams, &m_GlobalVEData.origCameraParams); break;
    case 14: CopyScreenEffect(&m_GlobalVEData.editScreenEffect, &m_GlobalVEData.origScreenEffect); break;
    case 15: CopyDamageEffect(&m_GlobalVEData.editDamageEffect, &m_GlobalVEData.origDamageEffect); break;
    case 16: CopyPlanarReflection(&m_GlobalVEData.editPlanarReflection, &m_GlobalVEData.origPlanarReflection); break;
    case 17: CopyDynamicEnvmap(&m_GlobalVEData.editDynamicEnvmap, &m_GlobalVEData.origDynamicEnvmap); break;
    case 18: CopyCharacterLighting(&m_GlobalVEData.editCharacterLighting, &m_GlobalVEData.origCharacterLighting); break;
    case 19: CopyMotionBlur(&m_GlobalVEData.editMotionBlur, &m_GlobalVEData.origMotionBlur); break;
    }
}

void VisualEnvironmentEditor::ScanVEDataFromResourceManager()
{
    m_VEDataNameMap.clear();

    fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
    if (!rm)
        return;

    LOG_INFO("Starting resource scan...");

    for (const auto& comp : rm->m_compartments)
    {
        if (!comp) continue;

        for (const auto& obj : comp->m_objects)
        {
            if (!obj) continue;

            fb::TypeInfo* typeInfo = obj->GetType();
            if (!typeInfo || typeInfo->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                continue;

            fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(typeInfo);

            if (!classInfo->isSubclassOf((fb::ClassInfo*)fb::ObjectBlueprint::ClassInfoPtr()))
                continue;

            fb::ObjectBlueprint* objBp = static_cast<fb::ObjectBlueprint*>(obj);
            if (!objBp->m_Object)
                continue;

            fb::TypeInfo* objTypeInfo = objBp->m_Object->GetType();
            if (!objTypeInfo || objTypeInfo->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                continue;

            fb::ClassInfo* objClassInfo = static_cast<fb::ClassInfo*>(objTypeInfo);
            if (objClassInfo->m_ClassId != fb::VisualEnvironmentEntityData::ClassId())
                continue;

            fb::VisualEnvironmentEntityData* veData = static_cast<fb::VisualEnvironmentEntityData*>(objBp->m_Object);
            std::string name;
            if (objBp->m_Name)
            {
                name = objBp->m_Name;
                size_t pos = name.find_last_of("/\\");
                if (pos != std::string::npos)
                    name = name.substr(pos + 1);
            }

            m_VEDataNameMap[veData] = name;
            LOG_DEBUG("VEData %p -> '%s'", veData, name.c_str());
        }
    }

    LOG_INFO("Found %zu VEData entries", m_VEDataNameMap.size());

    m_VEDataScanned = true;
}