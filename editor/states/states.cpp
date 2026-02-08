#include "states.h"
#include "../editor_context.h"
#include "../copy/copy.h"
#include "../../utils/log.h"
#include <set>
#include <algorithm>

namespace editor::states
{
    std::map<fb::VisualEnvironmentState*, StateEditData> stateDataMap;
    std::unordered_map<fb::VisualEnvironmentState*, StateEntityInfo> stateEntityMap;
    std::vector<fb::VisualEnvironmentState*> stateOrder;
    std::vector<StateEditEntry> editList;
    int selectedIndex = 0;
    size_t lastKnownCount = 0;
    std::unordered_map<fb::VisualEnvironmentEntityData*, std::string> veDataNameMap;
    bool veDataScanned = false;

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
        stateDataMap.clear();
        stateEntityMap.clear();
        stateOrder.clear();
        editList.clear();
        selectedIndex = 0;
        lastKnownCount = 0;
        veDataNameMap.clear();
        veDataScanned = false;
    }

    std::vector<fb::VisualEnvironmentState*>& getStateOrder()
    {
        return stateOrder;
    }

    StateEditData* getStateData(fb::VisualEnvironmentState* state)
    {
        auto itr = stateDataMap.find(state);
        return itr != stateDataMap.end() ? &itr->second : nullptr;
    }

    int getSelectedIndex() { return selectedIndex; }
    void setSelectedIndex(int idx) { selectedIndex = idx; }

    int getActiveOverrideCount()
    {
        int count = 0;
        for (const auto& [state, data] : stateDataMap)
        {
            if (data.captured && data.overrideEnabled)
                count++;
        }
        return count;
    }

    std::vector<StateEditEntry>& getEditList()
    {
        return editList;
    }

    std::unordered_map<fb::VisualEnvironmentEntityData*, std::string>& getVEDataNameMap()
    {
        return veDataNameMap;
    }

    StateHash computeHash(fb::VisualEnvironmentState* state)
    {
        StateHash hash{ };
        if (!state)
            return hash;

        hash.priority = state->priority;
        hash.visibility = state->visibility;
        hash.componentMask = 0U;

        if (state->outdoorLight)      hash.componentMask |= Comp_OutdoorLight;
        if (state->enlighten)         hash.componentMask |= Comp_Enlighten;
        if (state->tonemap)           hash.componentMask |= Comp_Tonemap;
        if (state->colorCorrection)   hash.componentMask |= Comp_ColorCorrection;
        if (state->sky)               hash.componentMask |= Comp_Sky;
        if (state->fog)               hash.componentMask |= Comp_Fog;
        if (state->wind)              hash.componentMask |= Comp_Wind;
        if (state->sunFlare)          hash.componentMask |= Comp_Sunflare;
        if (state->dynamicAO)         hash.componentMask |= Comp_DynamicAO;
        if (state->dof)               hash.componentMask |= Comp_Dof;
        if (state->vignette)          hash.componentMask |= Comp_Vignette;
        if (state->filmGrain)         hash.componentMask |= Comp_FilmGrain;
        if (state->lensScope)         hash.componentMask |= Comp_LensScope;
        if (state->cameraParams)      hash.componentMask |= Comp_CameraParams;
        if (state->screenEffect)      hash.componentMask |= Comp_ScreenEffect;
        if (state->damageEffect)      hash.componentMask |= Comp_DamageEffect;
        if (state->planarReflection)  hash.componentMask |= Comp_PlanarReflection;
        if (state->dynamicEnvmap)     hash.componentMask |= Comp_DynamicEnvmap;
        if (state->characterLighting) hash.componentMask |= Comp_CharacterLighting;
        if (state->motionBlur)        hash.componentMask |= Comp_MotionBlur;

        return hash;
    }

    StateEditEntry* findEditEntry(const StateHash& hash)
    {
        for (auto& entry : editList)
        {
            if (entry.hash == hash)
                return &entry;
        }
        return nullptr;
    }

    StateEditEntry* findEditEntryByPriorityAndMask(int priority, uint32_t componentMask)
    {
        for (auto& entry : editList)
        {
            if (entry.hash.priority == priority && entry.hash.componentMask == componentMask)
                return &entry;
        }
        return nullptr;
    }

    void addOrUpdateEditEntry(fb::VisualEnvironmentState* state, const StateEditData& data)
    {
        StateHash hash = computeHash(state);

        if (StateEditEntry* entry = findEditEntry(hash))
        {
            entry->editData = data;
        }
        else
        {
            StateEditEntry newEntry;
            newEntry.hash = hash;
            newEntry.editData = data;
            editList.push_back(std::move(newEntry));
        }
    }

    void removeEditEntry(const StateHash& hash)
    {
        editList.erase(
            std::remove_if(editList.begin(), editList.end(),
                [&hash](const StateEditEntry& e) { return e.hash == hash; }),
            editList.end());
    }

    static void sortStatesByPriority()
    {
        std::sort(stateOrder.begin(), stateOrder.end(),
            [](fb::VisualEnvironmentState* a, fb::VisualEnvironmentState* b) {
                if (!a) return false;
                if (!b) return true;
                return a->priority < b->priority;
            });
    }

    static void onStateRemoved(fb::VisualEnvironmentState* state)
    {
        if (!state)
            return;

        auto itr = stateDataMap.find(state);
        if (itr != stateDataMap.end())
        {
            logger::debug("OnStateRemoved: P{}", state->priority);
            stateDataMap.erase(itr);
        }

        auto orderIt = std::ranges::find(stateOrder, state);
        if (orderIt != stateOrder.end())
            stateOrder.erase(orderIt);

        stateEntityMap.erase(state);
    }

    void syncWithManager(fb::VisualEnvironmentManager* manager)
    {
        if (!manager)
            return;

        std::set<fb::VisualEnvironmentState*> currentStates;
        for (size_t i = 0; i < manager->m_states.size(); i++)
        {
            auto state = manager->m_states[i];
            if (state) currentStates.insert(state);
        }

        std::vector<fb::VisualEnvironmentState*> removed;
        for (const auto& [state, data] : stateDataMap)
        {
            if (currentStates.find(state) == currentStates.end())
                removed.push_back(state);
        }

        for (auto state : removed)
            onStateRemoved(state);

        stateOrder.clear();
        for (size_t i = 0; i < manager->m_states.size(); i++)
        {
            auto state = manager->m_states[i];
            if (!state || state->excluded)
                continue;

            stateOrder.push_back(state);

            auto itr = stateDataMap.find(state);
            if (itr == stateDataMap.end())
            {
                logger::debug("SyncWithManager: New state detected, capturing");
                captureState(state);
            }
            else
            {
                itr->second.stateIndex = static_cast<int>(i);
            }
        }

        sortStatesByPriority();
        lastKnownCount = manager->m_states.size();

        if (selectedIndex >= static_cast<int>(stateOrder.size()))
            selectedIndex = stateOrder.empty() ? 0 : static_cast<int>(stateOrder.size()) - 1;
    }

    void refreshData()
    {
        if (!isSafeToOperate() || stateOrder.empty())
            return;

        for (fb::VisualEnvironmentState* state : stateOrder)
        {
            if (!state || state->excluded)
                continue;

            auto itr = stateDataMap.find(state);
            if (itr != stateDataMap.end())
            {
                captureStateData(state, itr->second);
            }
        }
    }

    std::string getStateName(fb::VisualEnvironmentState* state, int index)
    {
        if (!state)
            return "Unknown";

        auto itr = stateEntityMap.find(state);
        if (itr != stateEntityMap.end() && !itr->second.name.empty())
            return itr->second.name;

        char buf[32];
        snprintf(buf, sizeof(buf), "State_%d", index);
        return buf;
    }

    void captureState(fb::VisualEnvironmentState* state)
    {
        if (!state)
            return;

        auto existingIt = stateDataMap.find(state);
        if (existingIt != stateDataMap.end() && existingIt->second.captured)
            return;

        StateHash hash = computeHash(state);
        logger::debug("CaptureState: P{} M{:08X} (id={})", hash.priority, hash.componentMask, state->stateId);

        StateEditData& data = stateDataMap[state];
        data.statePtr = state;
        data.stateName = getStateName(state, static_cast<int>(stateOrder.size()));

        captureStateData(state, data);

        if (std::ranges::find(stateOrder, state) == stateOrder.end())
        {
            stateOrder.push_back(state);
            sortStatesByPriority();
        }

        StateEditEntry* existingEdit = findEditEntry(hash);
        if (existingEdit)
        {
            logger::debug("CaptureState: Found existing edit entry, restoring values");
            restoreEditValuesFromEntry(data, existingEdit->editData);

            if (overridesEnabled && data.overrideEnabled)
            {
                applyEdits(state, data);
                if (auto mgr = getManager())
                    mgr->setDirty(true);
            }
        }
    }

    void restoreAll()
    {
        for (auto& [state, data] : stateDataMap)
        {
            if (state && data.captured)
                restoreState(state, data);
        }

        if (auto* mgr = getManager())
            mgr->setDirty(true);
    }

    void resetAll()
    {
        for (auto& [state, data] : stateDataMap)
        {
            resetToOriginal(data);
        }
    }

    void onEntityCreated(fb::VisualEnvironmentEntity* entity, fb::VisualEnvironmentEntityData* data)
    {
        if (!entity)
            return;

        fb::VisualEnvironmentState* state = &entity->m_state;
        StateEntityInfo info{ };
        info.entity = entity;
        info.data = data;
        info.priority = data ? data->m_Priority : 0;

        auto itr = veDataNameMap.find(data);
        if (itr != veDataNameMap.end())
            info.name = itr->second;

        if (info.name.empty() && entity->m_referenceObjectData)
        {
            auto refData = entity->m_referenceObjectData;
            info.priority = refData->m_Priority;
            info.excluded = refData->m_Excluded;

            if (refData->m_Blueprint && refData->m_Blueprint->m_Name)
            {
                info.name = refData->m_Blueprint->tryGetDebugName();
            }
        }

        stateEntityMap[state] = info;
        logger::debug("VE Entity Created: '{}' P{}", info.name.c_str(), info.priority);
    }

    void onEntityDestroyed(fb::VisualEnvironmentEntity* entity)
    {
        if (!entity)
            return;

        fb::VisualEnvironmentState* state = &entity->m_state;
        auto itr = stateEntityMap.find(state);
        if (itr != stateEntityMap.end())
        {
            logger::debug("VE Entity Destroyed: '{}' P{}", itr->second.name.c_str(), itr->second.priority);
            stateEntityMap.erase(itr);
        }
    }

    void scanVEDataNames()
    {
        veDataNameMap.clear();

        fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
        if (!rm)
            return;

        logger::debug("Scanning VE data names...");

        for (const auto& comp : rm->m_compartments)
        {
            if (!comp)
                continue;

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

                veDataNameMap[veData] = objBp->tryGetDebugName();
            }
        }

        logger::debug("Found {} VE data entries", veDataNameMap.size());
        veDataScanned = true;
    }
}