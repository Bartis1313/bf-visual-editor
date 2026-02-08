#include "config.h"
#include "../editor_context.h"
#include "../states/states.h"
#include "../lights/lights.h"
#include "../emitters/emitters.h"
#include "../global_ve/global_ve.h"
#include "../world_render/world_render.h"
#include "../serialize/serialize.h"
#include "../../utils/log.h"
#include "../render/render.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace editor::config
{
    // TODO: MOVE THIS!!!!!!
    static constexpr float CONFIG_VERSION = 2.0f;

    void init()
    {

    }

    void shutdown()
    {

    }

    static json buildConfigJson()
    {
        json root;
        root["version"] = CONFIG_VERSION;
        root["mapName"] = capturedMapName;

        json statesJson = json::array();
        auto& editList = states::getEditList();
        for (const auto& entry : editList)
        {
            json stateEntry;
            stateEntry["priority"] = entry.hash.priority;
            stateEntry["componentMask"] = entry.hash.componentMask;
            stateEntry["visibility"] = entry.hash.visibility;
            stateEntry["data"] = states::serializeStateData(entry.editData);
            statesJson.push_back(stateEntry);
        }
        root["states"] = statesJson;

        json lightsJson = json::object();
        {
            for (const auto& [dataPtr, entry] : lights::getEntries())
            {
                if (entry.hasOverride)
                    lightsJson[entry.assetName] = lights::serialize(entry);
            }
        }
        root["lights"] = lightsJson;

        json emittersJson = json::object();
        for (const auto& [dataPtr, edit] : emitters::getMap())
        {
            if (!edit.modified)
                continue;

            json emitterEntry;
            EmitterSnapshot current;
            EmitterColorSnapshot colorCurrent;
            emitters::captureCurrentState(edit, current, colorCurrent);

            emitters::serializeSnapshot(emitterEntry["template"], current);
            if (colorCurrent.exists)
            {
                emitterEntry["hasColor"] = true;
                emitters::serializeColorSnapshot(emitterEntry["color"], colorCurrent);
            }
            else
            {
                emitterEntry["hasColor"] = false;
            }

            emittersJson[edit.name] = emitterEntry;
        }
        root["emitters"] = emittersJson;

        if (global_ve::hasCaptured())
        {
            root["globalVE"] = global_ve::serialize();
        }

        if (world_render::hasCaptured() && world_render::isOverrideEnabled())
        {
            root["worldRender"] = world_render::serialize();
        }

        return root;
    }

    static void applyConfigJson(const json& root)
    {
        size_t componentsLoaded = 0;
        size_t lightsApplied = 0;
        size_t emittersApplied = 0;

        if (root.contains("states") && root["states"].is_array())
        {
            auto& editList = states::getEditList();
            editList.clear();

            for (const auto& stateEntry : root["states"])
            {
                StateEditEntry entry{ };
                entry.hash.priority = stateEntry.value("priority", 0);
                entry.hash.componentMask = stateEntry.value("componentMask", 0u);
                entry.hash.visibility = stateEntry.value("visibility", 1.0f);

                if (stateEntry.contains("data") && stateEntry["data"].is_object())
                {
                    states::deserializeStateData(stateEntry["data"], entry.editData);

                    for (const auto& [key, value] : stateEntry["data"].items())
                    {
                        if (key != "overrideEnabled" && value.is_object())
                            componentsLoaded++;
                    }
                }

                editList.push_back(entry);
            }

            auto& stateOrder = states::getStateOrder();
            for (auto* state : stateOrder)
            {
                if (!state)
                    continue;

                StateHash hash = states::computeHash(state);
                StateEditEntry* matchedEntry = states::findEditEntry(hash);

                if (!matchedEntry)
                    matchedEntry = states::findEditEntryByPriorityAndMask(hash.priority, hash.componentMask);

                if (matchedEntry)
                {
                    StateEditData* data = states::getStateData(state);
                    if (data)
                        states::restoreEditValuesFromEntry(*data, matchedEntry->editData);
                }
            }

            logger::info("Config: Loaded {} state components", componentsLoaded);
        }

        if (root.contains("lights") && root["lights"].is_object())
        {
            for (const auto& [name, lightJson] : root["lights"].items())
            {
                for (auto& [dataPtr, entry] : lights::getEntries())
                {
                    if (entry.assetName == name)
                    {
                        lights::deserialize(lightJson, entry);
                        if (entry.hasOverride)
                        {
                            lights::applyOverride(entry);
                            lightsApplied++;
                        }
                        break;
                    }
                }
            }

            logger::info("Config: Applied {} light overrides", lightsApplied);
        }

        if (root.contains("emitters") && root["emitters"].is_object())
        {
            auto& emitterMap = emitters::getMap();
            auto& pendingEdits = emitters::getPendingEdits();
            size_t pending = 0;

            for (const auto& [name, emitterJson] : root["emitters"].items())
            {
                bool found = false;
                for (auto& [dataPtr, edit] : emitterMap)
                {
                    if (edit.name == name)
                    {
                        EmitterSnapshot snap;
                        emitters::deserializeSnapshot(emitterJson["template"], snap);
                        snap.restoreTo(edit.data);

                        if (emitterJson.value("hasColor", false) && edit.colorProcessor)
                        {
                            EmitterColorSnapshot colorSnap;
                            emitters::deserializeColorSnapshot(emitterJson["color"], colorSnap);
                            colorSnap.restoreTo(edit.colorProcessor);
                        }

                        edit.modified = true;
                        emittersApplied++;
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    PendingEmitterEdit pendingEdit;
                    pendingEdit.name = name;
                    emitters::deserializeSnapshot(emitterJson["template"], pendingEdit.templateData);

                    if (emitterJson.value("hasColor", false))
                    {
                        pendingEdit.hasColorData = true;
                        emitters::deserializeColorSnapshot(emitterJson["color"], pendingEdit.colorData);
                    }

                    pendingEdits.push_back(pendingEdit);
                    pending++;
                }
            }

            logger::info("Config: Applied {} emitter overrides, {} pending", emittersApplied, pending);
        }

        if (root.contains("globalVE"))
        {
            global_ve::deserialize(root["globalVE"]);
            logger::info("Config: Loaded global VE overrides");
        }

        if (root.contains("worldRender"))
        {
            world_render::deserialize(root["worldRender"]);
            logger::info("Config: Loaded world render settings");

            world_render::apply();
        }

        std::string summary{ };
        bool anyLoaded = (componentsLoaded + lightsApplied + emittersApplied) > 0;

        if (componentsLoaded > 0)
            summary += std::format("{} states\n", componentsLoaded);
        if (lightsApplied > 0)
            summary += std::format("{} lights\n", lightsApplied);
        if (emittersApplied > 0)
            summary += std::format("{} emitters", emittersApplied);

        if (summary.empty())
            summary = "No changes applied";

        if (!summary.empty() && summary.back() == '\n')
            summary.pop_back();

        render::ImNotify::add(anyLoaded ? NotifyType::Success : NotifyType::Warning, "Config Loaded", summary);
    }

    bool save(const std::string& path)
    {
        try
        {
            fs::path filePath{ path };
            fs::create_directories(filePath.parent_path());

            json root = buildConfigJson();

            std::ofstream file{ path };
            if (!file.is_open())
            {
                logger::error("Config::Save - Failed to open file: {}", path.c_str());
                render::ImNotify::add(NotifyType::Error, "Save Failed", std::string{ "Couldn't open this file " } + path);
                return false;
            }

            file << root.dump(2);

            int stateCount = root["states"].size();
            int lightCount = root["lights"].size();
            int emitterCount = root["emitters"].size();

            std::string filename = filePath.filename().string();
            std::string summary = std::format("Saved to {}\n{} states, {} lights, {} emitters", filename, stateCount, lightCount, emitterCount);

            render::ImNotify::add(NotifyType::Success, "Config saved", summary);

            logger::info("Config saved to: {}", path.c_str());

            return true;
        }
        catch (const std::exception& e)
        {
            logger::error("Config::Save exception: {}", e.what());

            render::ImNotify::add(NotifyType::Error, "Config save failed", e.what());

            return false;
        }
    }

    bool load(const std::string& path)
    {
        try
        {
            if (!fs::exists(path))
            {
                logger::error("Config::Load - File not found: {}", path.c_str());
                render::ImNotify::add(NotifyType::Error, "Load Failed", std::string{ "File not found "} + path);

                return false;
            }

            std::ifstream file{ path };
            if (!file.is_open())
            {
                logger::error("Config::Load - Failed to open file: {}", path.c_str());
                render::ImNotify::add(NotifyType::Error, "Load Failed", std::string{ "Couldn't open this file " } + path);

                return false;
            }

            json root = json::parse(file);

            float version = root.value("version", 1.0f);
            if (version != CONFIG_VERSION)
            {
                logger::warning("Config::Load - Version mismatch: {} vs {}", version, CONFIG_VERSION);
                render::ImNotify::add(NotifyType::Warning, "Version Mismatch", std::format("Config v{:.1f}, expected v{:.1f}", version, CONFIG_VERSION));
            }

            applyConfigJson(root);

            logger::info("Config loaded from: {}", path.c_str());
            return true;
        }
        catch (const std::exception& e)
        {
            logger::info("Config::Load exception: {}", e.what());
            render::ImNotify::add(NotifyType::Error, "Load failed", e.what());
            return false;
        }
    }

    bool saveForCurrentMap()
    {
        if (capturedMapName.empty())
        {
            logger::error("Config::SaveForCurrentMap - No map name");
            render::ImNotify::add(NotifyType::Error, "Save failed", "No map name");

            return false;
        }

        return save(getConfigPath(capturedMapName));
    }

    bool loadForCurrentMap()
    {
        if (capturedMapName.empty())
        {
            logger::error("Config::LoadForCurrentMap - No map name");
            render::ImNotify::add(NotifyType::Error, "Load failed", "No map name");
            return false;
        }

        return load(getConfigPath(capturedMapName));
    }

    bool exportStateToClipboard(int stateIndex)
    {
        auto& stateOrder = states::getStateOrder();
        if (stateIndex < 0 || stateIndex >= static_cast<int>(stateOrder.size()))
            return false;

        fb::VisualEnvironmentState* state = stateOrder[stateIndex];
        StateEditData* data = states::getStateData(state);
        if (!data)
            return false;

        json stateJson{ };
        stateJson["priority"] = state->priority;
        stateJson["componentMask"] = states::computeHash(state).componentMask;
        stateJson["data"] = states::serializeStateData(*data);

        std::string jsonStr = stateJson.dump(2);
        ImGui::SetClipboardText(jsonStr.c_str());

        logger::info("Exported state %d to clipboard", stateIndex);
        return true;
    }

    bool importStateFromClipboard(int stateIndex)
    {
        auto& stateOrder = states::getStateOrder();
        if (stateIndex < 0 || stateIndex >= static_cast<int>(stateOrder.size()))
            return false;

        fb::VisualEnvironmentState* state = stateOrder[stateIndex];
        StateEditData* data = states::getStateData(state);
        if (!data)
            return false;

        const char* clipboardText = ImGui::GetClipboardText();
        if (!clipboardText || clipboardText[0] == '\0')
            return false;

        try
        {
            json stateJson = json::parse(clipboardText);

            if (stateJson.contains("data"))
            {
                StateEditData importedData{ };
                states::deserializeStateData(stateJson["data"], importedData);
                states::copyEditDataDirectly(*data, importedData);
                data->overrideEnabled = true;

                states::addOrUpdateEditEntry(state, *data);

                logger::info("Imported state from clipboard to index %d", stateIndex);
                return true;
            }
        }
        catch (const std::exception& e)
        {
            logger::info("ImportStateFromClipboard exception: %s", e.what());
        }

        return false;
    }
}