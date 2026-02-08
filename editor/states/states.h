#pragma once

#include "../types.h"
#include <vector>
#include <map>
#include <unordered_map>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace editor::states
{
    void init();
    void shutdown();
    void clear();

    void syncWithManager(fb::VisualEnvironmentManager* manager);
    void refreshData();

    std::vector<fb::VisualEnvironmentState*>& getStateOrder();
    StateEditData* getStateData(fb::VisualEnvironmentState* state);
    int getSelectedIndex();
    void setSelectedIndex(int idx);
    int getActiveOverrideCount();

    std::vector<StateEditEntry>& getEditList();
    StateHash computeHash(fb::VisualEnvironmentState* state);
    StateEditEntry* findEditEntry(const StateHash& hash);
    StateEditEntry* findEditEntryByPriorityAndMask(int priority, uint32_t componentMask);
    void addOrUpdateEditEntry(fb::VisualEnvironmentState* state, const StateEditData& data);
    void removeEditEntry(const StateHash& hash);

    void captureState(fb::VisualEnvironmentState* state);
    void captureStateData(fb::VisualEnvironmentState* state, StateEditData& data);
    void applyEdits(fb::VisualEnvironmentState* state, const StateEditData& data);
    void restoreState(fb::VisualEnvironmentState* state, const StateEditData& data);
    void restoreAll();
    void resetAll();
    void resetToOriginal(StateEditData& data);

    void onEntityCreated(fb::VisualEnvironmentEntity* entity, fb::VisualEnvironmentEntityData* data);
    void onEntityDestroyed(fb::VisualEnvironmentEntity* entity);
    std::string getStateName(fb::VisualEnvironmentState* state, int index);

    void scanVEDataNames();
    std::unordered_map<fb::VisualEnvironmentEntityData*, std::string>& getVEDataNameMap();

    void restoreEditValuesFromEntry(StateEditData& dst, const StateEditData& src);
    void copyEditDataDirectly(StateEditData& dst, const StateEditData& src);

    void renderTab();
    void renderStatesMenu();
    void renderStateSelector();
    void renderStateEditor(fb::VisualEnvironmentState* state, StateEditData& data);
    void renderComponentEditors(StateEditData& data);

    json serializeStateData(const StateEditData& data);
    void deserializeStateData(const json& j, StateEditData& data);
}