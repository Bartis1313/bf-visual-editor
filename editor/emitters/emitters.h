#pragma once

#include "../types.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>
#include <map>

using json = nlohmann::json;

namespace editor::emitters
{
    void init();
    void shutdown();
    void clear();

    void scan();

    void onCreated(fb::EmitterTemplate* emitter, fb::EmitterTemplateData* data);

    std::unordered_map<fb::EmitterTemplateData*, EmitterEditData>& getMap();
    EmitterTreeNode& getTree();
    fb::EmitterTemplateData* getSelected();
    void setSelected(fb::EmitterTemplateData* data);

    std::vector<PendingEmitterEdit>& getPendingEdits();
    void applyPendingEdits();
    void captureCurrentState(const EmitterEditData& edit, EmitterSnapshot& outTemplate, EmitterColorSnapshot& outColor);

    void renderTab();

    void serializeSnapshot(json& j, const EmitterSnapshot& s);
    void deserializeSnapshot(const json& j, EmitterSnapshot& s);
    void serializeColorSnapshot(json& j, const EmitterColorSnapshot& s);
    void deserializeColorSnapshot(const json& j, EmitterColorSnapshot& s);
}