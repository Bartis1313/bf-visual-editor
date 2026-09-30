#include "editor.h"
#include "editor_context.h"
#include "states/states.h"
#include "lights/lights.h"
#include "emitters/emitters.h"
#include "effects/effects.h"
#include "global_ve/global_ve.h"
#include "world_render/world_render.h"
#include "textures/textures.h"
#include "enlighten/enlighten.h"
#include "enlighten/enlighten_db.h"
#include "config/config.h"
#include "ui/ui_helpers.h"
#include "ui/file_dialog.h"
#include "camera/camera.h"
#include "../utils/log.h"
#include "render/render.h"

#include <imgui.h>
#include <Windows.h>
#include <filesystem>
#include <mutex>

namespace fs = std::filesystem;

namespace editor
{
    bool showConsole = false;

    // taken by Present, VE update and entity hooks
    static std::recursive_mutex g_lock;
    using Lock = std::lock_guard<std::recursive_mutex>;

    std::recursive_mutex& lock()
    {
        return g_lock;
    }

    // level being unloaded
    static std::string unloadingMap;

    void init()
    {
        try
        {
            fs::create_directories(fs::path(getEditorRoot()));
            fs::create_directories(fs::path(getDumpsDir()));
            logger::setFile(getEditorRoot() + "/log.txt");
        }
        catch (const fs::filesystem_error& err)
        {
            logger::error("Init error: {}", err.what());
        }

        {
            HMODULE self = nullptr;
            GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCSTR>(&init), &self);
            char path[MAX_PATH] = {};
            GetModuleFileNameA(self, path, MAX_PATH);
            WIN32_FILE_ATTRIBUTE_DATA fa{};
            SYSTEMTIME st{};
            if (GetFileAttributesExA(path, GetFileExInfoStandard, &fa))
            {
                FILETIME lt{};
                FileTimeToLocalFileTime(&fa.ftLastWriteTime, &lt);
                FileTimeToSystemTime(&lt, &st);
            }
            logger::info("build {} ({:04}-{:02}-{:02} {:02}:{:02}:{:02})", path, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        }
        states::init();
        lights::init();
        emitters::init();
        effects::init();
        global_ve::init();
        world_render::init();
        textures::init();
        enlighten::init();
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
        textures::shutdown();
        enlighten::shutdown();
        config::shutdown();

        ui::filedlg::releasePreviews();
    }

    bool& isEnabled()
    {
        return enabled;
    }

    static bool g_levelScanPending = false;

    static bool resourcesSettled()
    {
        static uint32_t settled = 0;
        fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
        if (!rm || rm->m_bundleLoadInProgress)
        {
            settled = 0;
            return false;
        }
        if (settled < 1000)
            ++settled;
        return settled > 30;
    }

    static void onLevelLoaded()
    {
        std::string currentMap = getCurrentMapName();
        logger::info("OnLevelLoaded - Map: {}", currentMap.c_str());

        levelUnloadingSignaled = false;
        capturedMapName.clear();
        hasCapturedOriginals = false;

        states::clear();
        emitters::clear();
        effects::clear();
        global_ve::clear();
        world_render::clear();
        enlighten::clear();

        g_levelScanPending = true;

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

    void onLevelUnloadBegin()
    {
        Lock guard(g_lock);
        if (levelUnloadingSignaled)
            return;

        logger::info("OnLevelUnloading - Map: {}", capturedMapName.c_str());

        unloadingMap = getCurrentMapName();
        levelUnloadingSignaled = true;
        safeToOperate = false;

        // before db::onLevelUnload stops the solver
        if (autoSaveOnUnload && !capturedMapName.empty())
        {
            logger::info("Auto-saving config for map: {}", capturedMapName.c_str());
            config::saveForCurrentMap();
        }

        enlighten::db::onLevelUnload();

        states::clear();
        lights::clear();
        emitters::clear();
        global_ve::clear();
        effects::stopAll();
        textures::clear();
        enlighten::clear();

        setEditorState(EditorState::Unloading);
    }

    static void tryRecoverFromUnloading(fb::VisualEnvironmentManager* manager)
    {
        if (!manager || manager->m_states.empty())
            return;

        std::string currentMap = getCurrentMapName();
        if (currentMap.empty() || currentMap == unloadingMap)
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

#if defined(BFVE_GAME_BF4)
    void onManagerUpdate_BF4(fb::VisualEnvironmentManager* manager)
    {
        if (!manager)
            return;
        Lock guard(g_lock);

        if (detectedMapChange())
        {
            states::clear();
            world_render::clear();
            hasCapturedOriginals = false;
            capturedMapName.clear();
            setEditorState(EditorState::Ready);
        }

        if (editorState == EditorState::Idle)
            setEditorState(EditorState::Ready);

        states::syncWithManager(manager);

        // not gated on resourcesSettled()
        const bool levelStable = editorState != EditorState::Unloading;
        if (levelStable)
        {
            lights::tickAimRay(); // update thread
            textures::tickGameThread();
        }
        if (levelStable && render::backend == render::Backend::Engine && ImGui::GetCurrentContext())
        {
            // DebugRenderer queues are per thread
            lights::renderOverlay();
            emitters::renderOverlay();
            effects::renderOverlay();
            enlighten::renderOverlay();
        }

        if (!hasCapturedOriginals)
        {
            std::string currentMap = getCurrentMapName();
            if (!isPlayerAlive() || manager->m_states.empty() || currentMap.empty())
                return;

            capturedMapName = currentMap;
            hasCapturedOriginals = true;

            states::scanVEDataNames();
            states::scanExistingEntities();
            lights::scanAll();
            lights::scanExistingEntities();
            emitters::scan();
            world_render::capture();
            setEditorState(EditorState::Active);

            logger::info("[BF4] initial capture (map: {}, states: {}, lights: {}, emitters: {})",
                currentMap, manager->m_states.size(),
                lights::getEntries().size(), emitters::getMap().size());

            if (autoLoadConfigs)
            {
                std::string configPath = getConfigPath(capturedMapName);
                if (fs::exists(configPath))
                    config::load(configPath);
            }
            return;
        }

        states::refreshData();
    }
#endif

    void onManagerUpdateBegin(fb::VisualEnvironmentManager* manager)
    {
        if (!manager)
            return;
        Lock guard(g_lock);

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

        const bool settled = resourcesSettled();
        const bool levelStable = editorState != EditorState::Unloading;
        if (levelStable)
        {
            lights::tickAimRay();
            textures::tickGameThread();
        }
        if (levelStable && render::backend == render::Backend::Engine && ImGui::GetCurrentContext())
        {
            // DebugRenderer2 queues are per thread
            lights::renderOverlay();
            emitters::renderOverlay();
            effects::renderOverlay();
            enlighten::renderOverlay();
        }
        if (g_levelScanPending && settled)
        {
            g_levelScanPending = false;
            lights::scanAll();
            if (textures::autoScan)
                textures::requestRescan();
        }

        if (!hasCapturedOriginals)
        {
            if (settled)
                tryInitialCapture();
            return;
        }
        // BF3 has no light ctor hook, walk every ~2 s
        static uint32_t lightWalkTick = 0;
        if (levelStable && (++lightWalkTick % 120) == 0)
            lights::refreshEntities();

        if (settled)
            lights::rescanIfResourcesGrew();

        states::refreshData();
    }

    void onManagerUpdateEnd(fb::VisualEnvironmentManager* manager)
    {
        if (!manager)
            return;
        Lock guard(g_lock);

        // state edits only with overrides on
        bool appliedAny = false;
        const bool applyStates = overridesEnabled && !states::getEditList().empty();
        for (auto state : manager->m_states)
        {
            if (!applyStates)
                break;
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
        {
            manager->setDirty(true);
#if defined(BFVE_GAME_BF4)
            manager->forceStatesDirty();
            manager->forceLutUpload();
#endif
        }

#if defined(BFVE_GAME_BF4)
        auto& g = global_ve::getData();
        bool anyGlobal = false;
#define ANY_GLOBAL(Type, field) anyGlobal = anyGlobal || g.field##OverrideEnabled;
        VE_COMPONENTS(ANY_GLOBAL)
#undef ANY_GLOBAL
        onVisualEnvironmentUpdated(&manager->getEnv());
        if (overridesEnabled && anyGlobal && g.globalOverrideEnabled)
        {
            manager->setDirty(true);
            manager->forceStatesDirty();
            manager->forceLutUpload();

            if (g.characterLightingOverrideEnabled)
                manager->rebakeCharacterLightingSH();
            if (g.vehicleLightingOverrideEnabled)
                manager->rebakeVehicleLightingSH();
        }
#endif
    }

    void onVisualEnvironmentUpdated(fb::VisualEnvironment* ve)
    {
        Lock guard(g_lock);
        if (!ve || levelUnloadingSignaled || editorState == EditorState::Unloading)
            return;

        // sky texture pointers are not carried by copy::sky
        textures::applySkyOverrides(ve);
        // before the Enlighten solver reads the values
        if (hasCapturedOriginals && overridesEnabled)
            global_ve::onUpdated(ve);
        global_ve::sun::onUpdated(ve);
        enlighten::onUpdated(ve);
    }

    void onMessage(uint32_t category, uint32_t type)
    {
        Lock guard(g_lock);
        if (category == "Client"_fbhash)
        {
            if (type == "ClientLevelUnloadedMessage"_fbhash)
            {
                onLevelUnloadBegin();
            }
            else if (type == "ClientLevelLoadedMessage"_fbhash)
            {
                onLevelLoaded();
            }
        }
    }

    void onVisualEnvironmentEntityCreated(fb::VisualEnvironmentEntity* entity, fb::VisualEnvironmentEntityData* data)
    {
        Lock guard(g_lock);
        states::onEntityCreated(entity, data);
    }

    void onVisualEnvironmentEntityDestroyed(fb::VisualEnvironmentEntity* entity)
    {
        Lock guard(g_lock);
        states::onEntityDestroyed(entity);
    }

    void onLightEntityCreated(fb::LocalLightEntity* entity, fb::LocalLightEntityData* data)
    {
        Lock guard(g_lock);
        lights::onEntityCreated(entity, data);
    }

    void onLightEntityDestroyed(fb::LocalLightEntity* entity)
    {
        Lock guard(g_lock);
        lights::onEntityDestroyed(entity);
    }

    void onEmitterCreated(fb::EmitterTemplate* emitter, fb::EmitterTemplateData* data)
    {
        Lock guard(g_lock);
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
        case EditorState::Idle: stateStr = "IDLE"; stateColor = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f }; break;
        case EditorState::Loading: stateStr = "LOADING"; stateColor = ImVec4{ 1.0f, 1.0f, 0.2f, 1.0f }; break;
        case EditorState::Ready: stateStr = "READY"; stateColor = ImVec4{ 0.2f, 0.8f, 1.0f, 1.0f }; break;
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

            if (ImGui::BeginTabItem("Emitters", nullptr,
                    emitters::focusTab ? ImGuiTabItemFlags_SetSelected : 0))
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

            if (ImGui::BeginTabItem("Enlighten"))
            {
                enlighten::renderTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Textures", nullptr,
                    textures::focusTab ? ImGuiTabItemFlags_SetSelected : 0))
            {
                textures::renderTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Camera"))
            {
                camera::renderTab();
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
        Lock guard(g_lock);
        const bool levelStable = !levelUnloadingSignaled && editorState != EditorState::Unloading;
        if (levelStable)
        {
            textures::tick();
            enlighten::tick();
            lights::tick();
            lights::tickGeometryCopies();
        }

        if (levelStable && render::backend == render::Backend::ImGui) // engine backend draws on the update thread
        {
            effects::renderOverlay();
            lights::renderOverlay();
            emitters::renderOverlay();
            enlighten::renderOverlay();
        }
        render::ImNotify::handle();

        renderMenu();

        if (showConsole)
            logger::render("Console", &showConsole);
    }
}