#include "editor.h"
#include "editor_context.h"
#include "states/states.h"
#include "lights/lights.h"
#include "emitters/emitters.h"
#include "effects/effects.h"
#include "global_ve/global_ve.h"
#include "world_render/world_render.h"
#include "config/config.h"
#include "ui/ui_helpers.h"
#include "../utils/log.h"
#include "render/render.h"

#include <imgui.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace editor
{
    bool showConsole = false;

    void init()
    {
        try
        {
            fs::path configPath = fs::path(getConfigDir()) / "VisEnvEditor";
            if (!fs::exists(configPath))
                fs::create_directories(configPath);
        }
        catch (const fs::filesystem_error& err)
        {
            logger::error("Init error: {}", err.what());
        }

        states::init();
        lights::init();
        emitters::init();
        effects::init();
        global_ve::init();
        world_render::init();
        config::init();
    }

    void shutdown()
    {
        states::shutdown();
        lights::shutdown();
        emitters::shutdown();
        effects::shutdown();
        global_ve::shutdown();
        world_render::shutdown();
        config::shutdown();
    }

    bool& isEnabled()
    {
        return enabled;
    }

    static void onLevelLoaded()
    {
        std::string currentMap = getCurrentMapName();
        logger::info("OnLevelLoaded - Map: {}", currentMap.c_str());

        levelUnloadingSignaled = false;
        capturedMapName.clear();
        hasCapturedOriginals = false;

        states::clear();
        lights::clear();
        emitters::clear();
        effects::clear();
        global_ve::clear();
        world_render::clear();

        if (autoLoadConfigs && !currentMap.empty())
        {
            std::string configPath = getConfigPath(currentMap);
            if (std::filesystem::exists(configPath))
            {
                logger::info("Auto-loading config for map: {}", currentMap.c_str());
                config::load(configPath);
            }
        }

        setEditorState(EditorState::Ready);
    }

    static void onLevelUnloading()
    {
        logger::info("OnLevelUnloading - Map: {}", capturedMapName.c_str());

        levelUnloadingSignaled = true;
        safeToOperate = false;

        if (autoSaveOnUnload && !capturedMapName.empty())
        {
            logger::info("Auto-saving config for map: {}", capturedMapName.c_str());
            config::saveForCurrentMap();
        }

        states::clear();
        lights::clear();
        emitters::clear();
        global_ve::clear();
        effects::stopAll();

        setEditorState(EditorState::Unloading);
    }

    static void tryRecoverFromUnloading(fb::VisualEnvironmentManager* manager)
    {
        if (!manager || manager->m_states.empty())
            return;

        std::string currentMap = getCurrentMapName();
        if (currentMap.empty())
            return;

        logger::debug("Recovering from Unloading state, map: {}", currentMap.c_str());
        levelUnloadingSignaled = false;
        setEditorState(EditorState::Ready);
    }

    static bool detectedMapChange()
    {
        if (!hasCapturedOriginals || capturedMapName.empty())
            return false;

        std::string currentMap = getCurrentMapName();
        return !currentMap.empty() && currentMap != capturedMapName;
    }

    static void handleMapChange()
    {
        states::clear();
        lights::clear();
        hasCapturedOriginals = false;
        capturedMapName.clear();
        world_render::clear();
        setEditorState(EditorState::Ready);
    }

    static void tryInitialCapture()
    {
        std::string currentMap = getCurrentMapName();

        if (!isPlayerAlive() || states::getStateOrder().empty() || currentMap.empty())
            return;

        capturedMapName = currentMap;
        hasCapturedOriginals = true;

        states::scanVEDataNames();
        lights::scanAll();
        lights::scanExistingEntities();
        emitters::scan();
        effects::scanAssets();        
        world_render::capture();

        setEditorState(EditorState::Active);

        if (autoLoadConfigs)
        {
            std::string configPath = getConfigPath(capturedMapName);
            if (fs::exists(configPath))
                config::load(configPath);
        }
    }

    void onManagerUpdateBegin(fb::VisualEnvironmentManager* manager)
    {
        if (!manager)
            return;

        if (editorState == EditorState::Unloading || levelUnloadingSignaled)
        {
            tryRecoverFromUnloading(manager);
        }

        if (detectedMapChange())
        {
            handleMapChange();
        }

        if (editorState == EditorState::Idle)
        {
            setEditorState(EditorState::Ready);
        }

        states::syncWithManager(manager);

        if (!hasCapturedOriginals)
        {
            tryInitialCapture();
            return;
        }

        states::refreshData();
    }

    void onManagerUpdateEnd(fb::VisualEnvironmentManager* manager)
    {
        if (!manager || !overridesEnabled)
            return;

        auto& editList = states::getEditList();
        if (editList.empty())
            return;

        bool appliedAny = false;
        for (auto state : manager->m_states)
        {
            if (!state || state->excluded)
                continue;

            StateHash currentHash = states::computeHash(state);
            StateEditEntry* entry = states::findEditEntry(currentHash);

            if (!entry)
            {
                entry = states::findEditEntryByPriorityAndMask(currentHash.priority, currentHash.componentMask);
                if (entry)
                    entry->hash.visibility = currentHash.visibility;
            }

            if (entry && entry->editData.overrideEnabled)
            {
                states::applyEdits(state, entry->editData);
                appliedAny = true;
            }
        }

        if (appliedAny)
            manager->setDirty(true);
    }

    void onVisualEnvironmentUpdated(fb::VisualEnvironment* ve)
    {
        if (!ve || levelUnloadingSignaled || editorState == EditorState::Unloading)
            return;

        if (!hasCapturedOriginals || !overridesEnabled)
            return;

        global_ve::onUpdated(ve);
    }

    void onMessage(uint32_t category, uint32_t type)
    {
        if (category == "Client"_fbhash)
        {
            if (type == "ClientLevelUnloadedMessage"_fbhash)
            {
                onLevelUnloading();
            }
            else if (type == "ClientLevelLoadedMessage"_fbhash)
            {
                onLevelLoaded();
            }
            /*else if (type == "ClientEnteredIngameMessage"_fbhash)
            {
                world_render::Capture();
            }*/
        }
    }

    void onVisualEnvironmentEntityCreated(fb::VisualEnvironmentEntity* entity, fb::VisualEnvironmentEntityData* data)
    {
        states::onEntityCreated(entity, data);
    }

    void onVisualEnvironmentEntityDestroyed(fb::VisualEnvironmentEntity* entity)
    {
        states::onEntityDestroyed(entity);
    }

    void onLightEntityCreated(fb::LocalLightEntity* entity, fb::LocalLightEntityData* data)
    {
        lights::onEntityCreated(entity, data);
    }

    void onLightEntityDestroyed(fb::LocalLightEntity* entity)
    {
        lights::onEntityDestroyed(entity);
    }

    void onEmitterCreated(fb::EmitterTemplate* emitter, fb::EmitterTemplateData* data)
    {
        emitters::onCreated(emitter, data);
    }

    static void RenderMenuBar()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save Config", nullptr, false, hasCapturedOriginals))
                    config::saveForCurrentMap();

                if (ImGui::MenuItem("Load Config", nullptr, false, hasCapturedOriginals))
                    config::loadForCurrentMap();

                ImGui::Separator();

                if (ImGui::MenuItem("Reset All States", nullptr, false, !states::getStateOrder().empty()))
                    states::resetAll();

                if (ImGui::MenuItem("Reset World Render", nullptr, false, world_render::hasCaptured()))
                    world_render::reset();

                if (ImGui::MenuItem("Reset All Lights"))
                    lights::resetAll();

                ImGui::Separator();

                if (ImGui::MenuItem("Force Re-capture All"))
                {
                    hasCapturedOriginals = false;
                    capturedMapName.clear();
                    states::clear();
                    lights::clear();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Options"))
            {
                ImGui::Checkbox("Enable Overrides", &overridesEnabled);
                ImGui::Separator();
                ImGui::Checkbox("Show Original Values", &ui::showOriginalValues);
                ImGui::Checkbox("Highlight Modified", &ui::highlightModified);
                ImGui::Checkbox("Auto-load Configs", &autoLoadConfigs);
                ImGui::Separator();
                ImGui::ColorEdit4("Modified Color", &ui::modifiedColor.x, ImGuiColorEditFlags_NoInputs);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("States"))
            {
                states::renderStatesMenu();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::Checkbox("Enable console", &showConsole);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    static void RenderStatusBar()
    {
        const char* stateStr = "?";
        ImVec4 stateColor = ImVec4(1, 1, 1, 1);

        switch (editorState)
        {
        case EditorState::Idle:      stateStr = "IDLE";      stateColor = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f }; break;
        case EditorState::Loading:   stateStr = "LOADING";   stateColor = ImVec4{ 1.0f, 1.0f, 0.2f, 1.0f }; break;
        case EditorState::Ready:     stateStr = "READY";     stateColor = ImVec4{ 0.2f, 0.8f, 1.0f, 1.0f }; break;
        case EditorState::Active:
            stateStr = overridesEnabled ? "ACTIVE" : "PAUSED";
            stateColor = overridesEnabled ? ImVec4(0.2f, 1.0f, 0.4f, 1.0f) : ImVec4{ 1.0f, 0.8f, 0.2f, 1.0f };
            break;
        case EditorState::Unloading: stateStr = "UNLOADING"; stateColor = ImVec4{ 1.0f, 0.4f, 0.2f, 1.0f }; break;
        }

        ImGui::TextColored(stateColor, "[%s]", stateStr);
        ImGui::SameLine();

        if (!capturedMapName.empty())
            ImGui::Text("%s", sanitizeMapName(capturedMapName).c_str());
        else
            ImGui::TextDisabled("(no map)");

        if (hasCapturedOriginals)
        {
            int activeStates = states::getActiveOverrideCount();
            ImGui::SameLine();
            ImGui::TextDisabled("| States: %d/%zu", activeStates, states::getStateOrder().size());

            if (global_ve::hasCaptured())
            {
                ImGui::SameLine();
                if (global_ve::isEnabled())
                    ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "| Global: %d", global_ve::enabledCount());
                else
                    ImGui::TextDisabled("| Global: OFF");
            }
        }
    }

    static void renderMenu()
    {
        if (!enabled)
            return;

        ImGui::SetNextWindowSize(ImVec2{ 700, 900 }, ImGuiCond_FirstUseEver);

        if (!ImGui::Begin("Visual Environment Editor", &enabled, ImGuiWindowFlags_MenuBar))
        {
            ImGui::End();
            return;
        }

        RenderMenuBar();
        RenderStatusBar();
        ImGui::Separator();

        if (ImGui::BeginTabBar("EditorTabs"))
        {
            if (ImGui::BeginTabItem("States"))
            {
                states::renderTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Global VE"))
            {
                global_ve::renderTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Lights"))
            {
                lights::renderTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Emitters"))
            {
                emitters::renderTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Effects"))
            {
                effects::renderTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("World Render"))
            {
                world_render::renderTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Config"))
            {
                config::renderTab();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    void render()
    {
        effects::renderOverlay();
        render::ImNotify::handle();

        renderMenu();

        if (showConsole)
            logger::render("Console", &showConsole);
    }
}