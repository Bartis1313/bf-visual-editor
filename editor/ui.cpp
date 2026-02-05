#include "editor.h"
#include "../utils/log.h"
#include "math.h"
#include "render.h"

#include <magic_enum/magic_enum.hpp>

void VisualEnvironmentEditor::RenderGlobalVEEditor()
{
    if (!m_GlobalVEData.captured)
    {
        ImGui::TextColored(ImVec4{ 0.8f, 0.8f, 0.2f, 1.0f }, "Global VE data not captured yet. Spawn in-game.");
        return;
    }

    bool masterEnabled = m_GlobalVEData.globalOverrideEnabled;
    if (masterEnabled)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.4f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4{ 0.5f, 0.3f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4{ 1.0f, 0.4f, 0.4f, 1.0f });
    }

    ImGui::Checkbox("Enable Global VE Overrides", &m_GlobalVEData.globalOverrideEnabled);

    if (masterEnabled)
    {
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4{ 1.0f, 0.6f, 0.2f, 1.0f }, "(%d active)", m_GlobalVEData.EnabledOverrideCount());
    }

    ImGui::SameLine(ImGui::GetWindowWidth() - 180);
    if (ImGui::Button("Reset All"))
        ResetGlobalVEToOriginal();

    ImGui::SameLine();
    if (ImGui::Button("Disable All"))
    {
        m_GlobalVEData.outdoorLightOverrideEnabled = false;
        m_GlobalVEData.enlightenOverrideEnabled = false;
        m_GlobalVEData.tonemapOverrideEnabled = false;
        m_GlobalVEData.colorCorrectionOverrideEnabled = false;
        m_GlobalVEData.skyOverrideEnabled = false;
        m_GlobalVEData.fogOverrideEnabled = false;
        m_GlobalVEData.windOverrideEnabled = false;
        m_GlobalVEData.sunFlareOverrideEnabled = false;
        m_GlobalVEData.dynamicAOOverrideEnabled = false;
        m_GlobalVEData.dofOverrideEnabled = false;
        m_GlobalVEData.vignetteOverrideEnabled = false;
        m_GlobalVEData.filmGrainOverrideEnabled = false;
        m_GlobalVEData.lensScopeOverrideEnabled = false;
        m_GlobalVEData.cameraParamsOverrideEnabled = false;
        m_GlobalVEData.screenEffectOverrideEnabled = false;
        m_GlobalVEData.damageEffectOverrideEnabled = false;
        m_GlobalVEData.planarReflectionOverrideEnabled = false;
        m_GlobalVEData.dynamicEnvmapOverrideEnabled = false;
        m_GlobalVEData.characterLightingOverrideEnabled = false;
        m_GlobalVEData.motionBlurOverrideEnabled = false;
    }

    ImGui::Separator();

    if (!m_GlobalVEData.globalOverrideEnabled)
    {
        ImGui::TextColored(ImVec4{ 0.7f, 0.7f, 0.3f, 1.0f }, "Global overrides disabled. Enable checkbox above to apply edits to final output.");
        ImGui::Separator();
    }

    ImGui::BeginChild("GlobalVEComponents", ImVec2{ 0, 0 }, false);

    auto RenderComponentSection = [this](const char* id, const char* displayName, bool* enableFlag,
        int resetIdx, auto renderFunc, auto* editData, const auto* origData)
        {
            ImGui::PushID(id);
            ImGui::Checkbox("", enableFlag);
            ImGui::SameLine();

            bool open = ImGui::TreeNodeEx(displayName, ImGuiTreeNodeFlags_AllowItemOverlap);

            if (*enableFlag)
            {
                ImGui::SameLine(ImGui::GetWindowWidth() - 60);
                if (ImGui::SmallButton("Reset"))
                    ResetGlobalVEComponent(resetIdx);
            }

            if (open)
            {
                if (!*enableFlag)
                {
                    ImGui::TextDisabled("(Enable checkbox to edit)");
                }
                else
                {
                    ImGui::Indent();
                    renderFunc(editData, origData);
                    ImGui::Unindent();
                }
                ImGui::TreePop();
            }

            ImGui::PopID();
        };

    RenderComponentSection("OutdoorLight", "Outdoor Light", &m_GlobalVEData.outdoorLightOverrideEnabled, 0,
        [this](auto e, const auto o) { RenderOutdoorLightComponent(e, o); },
        &m_GlobalVEData.editOutdoorLight, &m_GlobalVEData.origOutdoorLight);

    RenderComponentSection("Enlighten", "Enlighten", &m_GlobalVEData.enlightenOverrideEnabled, 1,
        [this](auto e, const auto o) { RenderEnlightenComponent(e, o); },
        &m_GlobalVEData.editEnlighten, &m_GlobalVEData.origEnlighten);

    RenderComponentSection("Tonemap", "Tonemap", &m_GlobalVEData.tonemapOverrideEnabled, 2,
        [this](auto e, const auto o) { RenderTonemapComponent(e, o); },
        &m_GlobalVEData.editTonemap, &m_GlobalVEData.origTonemap);

    RenderComponentSection("ColorCorrection", "Color Correction", &m_GlobalVEData.colorCorrectionOverrideEnabled, 3,
        [this](auto e, const auto o) { RenderColorCorrectionComponent(e, o); },
        &m_GlobalVEData.editColorCorrection, &m_GlobalVEData.origColorCorrection);

    RenderComponentSection("Sky", "Sky", &m_GlobalVEData.skyOverrideEnabled, 4,
        [this](auto e, const auto o) { RenderSkyComponent(e, o); },
        &m_GlobalVEData.editSky, &m_GlobalVEData.origSky);

    RenderComponentSection("Fog", "Fog", &m_GlobalVEData.fogOverrideEnabled, 5,
        [this](auto e, const auto o) { RenderFogComponent(e, o); },
        &m_GlobalVEData.editFog, &m_GlobalVEData.origFog);

    RenderComponentSection("Wind", "Wind", &m_GlobalVEData.windOverrideEnabled, 6,
        [this](auto e, const auto o) { RenderWindComponent(e, o); },
        &m_GlobalVEData.editWind, &m_GlobalVEData.origWind);

    RenderComponentSection("SunFlare", "Sun Flare", &m_GlobalVEData.sunFlareOverrideEnabled, 7,
        [this](auto e, const auto o) { RenderSunFlareComponent(e, o); },
        &m_GlobalVEData.editSunFlare, &m_GlobalVEData.origSunFlare);

    RenderComponentSection("DynamicAO", "Dynamic AO", &m_GlobalVEData.dynamicAOOverrideEnabled, 8,
        [this](auto e, const auto o) { RenderDynamicAOComponent(e, o); },
        &m_GlobalVEData.editDynamicAO, &m_GlobalVEData.origDynamicAO);

    RenderComponentSection("Dof", "Depth of Field", &m_GlobalVEData.dofOverrideEnabled, 9,
        [this](auto e, const auto o) { RenderDofComponent(e, o); },
        &m_GlobalVEData.editDof, &m_GlobalVEData.origDof);

    RenderComponentSection("Vignette", "Vignette", &m_GlobalVEData.vignetteOverrideEnabled, 10,
        [this](auto e, const auto o) { RenderVignetteComponent(e, o); },
        &m_GlobalVEData.editVignette, &m_GlobalVEData.origVignette);

    RenderComponentSection("FilmGrain", "Film Grain", &m_GlobalVEData.filmGrainOverrideEnabled, 11,
        [this](auto e, const auto o) { RenderFilmGrainComponent(e, o); },
        &m_GlobalVEData.editFilmGrain, &m_GlobalVEData.origFilmGrain);

    RenderComponentSection("LensScope", "Lens Scope", &m_GlobalVEData.lensScopeOverrideEnabled, 12,
        [this](auto e, const auto o) { RenderLensScopeComponent(e, o); },
        &m_GlobalVEData.editLensScope, &m_GlobalVEData.origLensScope);

    RenderComponentSection("CameraParams", "Camera Params", &m_GlobalVEData.cameraParamsOverrideEnabled, 13,
        [this](auto e, const auto o) { RenderCameraParamsComponent(e, o); },
        &m_GlobalVEData.editCameraParams, &m_GlobalVEData.origCameraParams);

    RenderComponentSection("ScreenEffect", "Screen Effect", &m_GlobalVEData.screenEffectOverrideEnabled, 14,
        [this](auto e, const auto o) { RenderScreenEffectComponent(e, o); },
        &m_GlobalVEData.editScreenEffect, &m_GlobalVEData.origScreenEffect);

    RenderComponentSection("DamageEffect", "Damage Effect", &m_GlobalVEData.damageEffectOverrideEnabled, 15,
        [this](auto e, const auto o) { RenderDamageEffectComponent(e, o); },
        &m_GlobalVEData.editDamageEffect, &m_GlobalVEData.origDamageEffect);

    RenderComponentSection("PlanarReflection", "Planar Reflection", &m_GlobalVEData.planarReflectionOverrideEnabled, 16,
        [this](auto e, const auto o) { RenderPlanarReflectionComponent(e, o); },
        &m_GlobalVEData.editPlanarReflection, &m_GlobalVEData.origPlanarReflection);

    RenderComponentSection("DynamicEnvmap", "Dynamic Envmap", &m_GlobalVEData.dynamicEnvmapOverrideEnabled, 17,
        [this](auto e, const auto o) { RenderDynamicEnvmapComponent(e, o); },
        &m_GlobalVEData.editDynamicEnvmap, &m_GlobalVEData.origDynamicEnvmap);

    RenderComponentSection("CharacterLighting", "Character Lighting", &m_GlobalVEData.characterLightingOverrideEnabled, 18,
        [this](auto e, const auto o) { RenderCharacterLightingComponent(e, o); },
        &m_GlobalVEData.editCharacterLighting, &m_GlobalVEData.origCharacterLighting);

    RenderComponentSection("MotionBlur", "Motion Blur", &m_GlobalVEData.motionBlurOverrideEnabled, 19,
        [this](auto e, const auto o) { RenderMotionBlurComponent(e, o); },
        &m_GlobalVEData.editMotionBlur, &m_GlobalVEData.origMotionBlur);

    ImGui::EndChild();
}

void VisualEnvironmentEditor::RenderMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Config", nullptr, false, m_HasCapturedOriginals))
                SaveConfigForCurrentMap();

            if (ImGui::MenuItem("Load Config", nullptr, false, m_HasCapturedOriginals))
                LoadConfigForCurrentMap();

            ImGui::Separator();

            if (ImGui::MenuItem("Reset All States", nullptr, false, !m_StateDataMap.empty()))
            {
                for (auto& [state, data] : m_StateDataMap)
                    ResetStateToOriginal(data);
            }

            if (ImGui::MenuItem("Reset World Render", nullptr, false, m_HasCapturedWorldRenderSettings))
                CopyWorldRenderSettings(&m_EditWorldRenderSettings, &m_OriginalWorldRenderSettings);

            if (ImGui::MenuItem("Reset All Lights", nullptr, false, !m_LightDataEntries.empty()))
            {
                std::lock_guard<std::mutex> lock(m_LightDataMutex);
                for (auto& [dataPtr, entry] : m_LightDataEntries)
                {
                    if (entry.origCaptured && entry.hasOverride)
                    {
                        entry.ResetToOriginal();
                        ApplyLightOverride(entry);
                    }
                }
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Force Re-capture All"))
            {
                InvalidateCapture();
                ClearLightData();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options"))
        {
            ImGui::Checkbox("Enable Overrides", &m_OverridesEnabled);
            ImGui::Separator();
            ImGui::Checkbox("Show Original Values", &m_ShowOriginalValues);
            ImGui::Checkbox("Highlight Modified", &m_HighlightModified);
            ImGui::Checkbox("Auto-load Configs", &m_AutoLoadConfigs);
            ImGui::Separator();
            ImGui::ColorEdit4("Modified Color", &m_ModifiedColor.x, ImGuiColorEditFlags_NoInputs);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("States"))
        {
            if (ImGui::MenuItem("Enable All"))
            {
                for (auto& [state, data] : m_StateDataMap)
                    data.overrideEnabled = true;
            }
            if (ImGui::MenuItem("Disable All"))
            {
                for (auto& [state, data] : m_StateDataMap)
                    data.overrideEnabled = false;
            }
            ImGui::Separator();

            for (int i = 0; i < static_cast<int>(m_StateOrder.size()); i++)
            {
                auto state = m_StateOrder[i];
                auto itr = m_StateDataMap.find(state);
                if (itr != m_StateDataMap.end())
                {
                    char label[128];
                    sprintf_s(label, "[%d] %s##StateMenu%d", itr->second.stateIndex, itr->second.stateName.c_str(), i);
                    ImGui::Checkbox(label, &itr->second.overrideEnabled);
                }
            }
            ImGui::EndMenu();
        }

        if (ImGui::SmallButton(m_ShowConsole ? "Hide Console" : "Show Console"))
        {
            m_ShowConsole = !m_ShowConsole;
        }

        ImGui::EndMenuBar();
    }
}

void VisualEnvironmentEditor::RenderStatusBar()
{
    const char* stateStr = "?";
    ImVec4 stateColor = ImVec4(1, 1, 1, 1);

    switch (m_EditorState)
    {
    case EditorState::Idle:      stateStr = "IDLE";      stateColor = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f }; break;
    case EditorState::Loading:   stateStr = "LOADING";   stateColor = ImVec4{ 1.0f, 1.0f, 0.2f, 1.0f }; break;
    case EditorState::Ready:     stateStr = "READY";     stateColor = ImVec4{ 0.2f, 0.8f, 1.0f, 1.0f }; break;
    case EditorState::Active:    stateStr = m_OverridesEnabled ? "ACTIVE" : "PAUSED";
        stateColor = m_OverridesEnabled ? ImVec4(0.2f, 1.0f, 0.4f, 1.0f) : ImVec4{ 1.0f, 0.8f, 0.2f, 1.0f }; break;
    case EditorState::Unloading: stateStr = "UNLOADING"; stateColor = ImVec4{ 1.0f, 0.4f, 0.2f, 1.0f }; break;
    }

    ImGui::TextColored(stateColor, "[%s]", stateStr);
    ImGui::SameLine();

    ImGui::Text("Map: %s | States: %zu", m_CapturedMapName.empty() ? "(none)" : m_CapturedMapName.c_str(), m_StateOrder.size());

    if (!m_CapturedMapName.empty())
        ImGui::Text("%s", SanitizeMapName(m_CapturedMapName).c_str());
    else
        ImGui::TextDisabled("(no map)");

    if (m_HasCapturedOriginals)
    {
        int activeStates = 0;
        int totalStateComponents = 0;
        for (const auto& [state, data] : m_StateDataMap)
        {
            if (data.captured && data.overrideEnabled)
                activeStates++;
            totalStateComponents += data.ComponentCount();
        }

        ImGui::SameLine();
        ImGui::TextDisabled("| States: %d/%zu", activeStates, m_StateOrder.size());

        if (m_GlobalVEData.captured)
        {
            ImGui::SameLine();
            if (m_GlobalVEData.globalOverrideEnabled)
            {
                ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f),
                    "| Global: %d", m_GlobalVEData.EnabledOverrideCount());
            }
            else
            {
                ImGui::TextDisabled("| Global: OFF");
            }
        }
    }
}

void VisualEnvironmentEditor::RenderStateSelector()
{
    for (int i = 0; i < static_cast<int>(m_StateOrder.size()); i++)
    {
        auto state = m_StateOrder[i];
        if (!state)
            continue;

        auto itr = m_StateDataMap.find(state);
        if (itr == m_StateDataMap.end())
            continue;

        const auto& data = itr->second;
        StateHash hash = ComputeStateHash(state);
        int compCount = __popcnt(hash.componentMask);

        std::string displayName = data.stateName;
        char label[256];
        ::sprintf_s(label, "%d: P%d [%d comps]%s%s%s",
            i,
            state->priority,
            compCount,
            displayName.empty() ? "" : " - ",
            displayName.c_str(),
            data.overrideEnabled ? " *" : "");

        bool isSelected = (m_SelectedStateIndex == i);
        if (state->excluded)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f });
        else if (data.overrideEnabled)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.3f, 1.0f, 0.3f, 1.0f });
        else
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });

        if (ImGui::Selectable(label, isSelected))
        {
            m_SelectedStateIndex = i;
        }

        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("Hash: P%d V%.2f M%08X", hash.priority, hash.visibility, hash.componentMask);
            ImGui::Separator();

            ImGui::Text("Priority: %d", state->priority);
            ImGui::Text("State ID: %d (runtime)", state->stateId);
            ImGui::Text("Visibility: %.3f", state->visibility);
            ImGui::Text("Excluded: %s", state->excluded ? "Yes" : "No");
            ImGui::Separator();

            ImGui::Text("Components (%d):", compCount);

            if (hash.componentMask & Comp_OutdoorLight)      ImGui::BulletText("OutdoorLight");
            if (hash.componentMask & Comp_Enlighten)         ImGui::BulletText("Enlighten");
            if (hash.componentMask & Comp_Tonemap)           ImGui::BulletText("Tonemap");
            if (hash.componentMask & Comp_ColorCorrection)   ImGui::BulletText("ColorCorrection");
            if (hash.componentMask & Comp_Sky)               ImGui::BulletText("Sky");
            if (hash.componentMask & Comp_Fog)               ImGui::BulletText("Fog");
            if (hash.componentMask & Comp_Wind)              ImGui::BulletText("Wind");
            if (hash.componentMask & Comp_Sunflare)          ImGui::BulletText("Sunflare");
            if (hash.componentMask & Comp_DynamicAO)         ImGui::BulletText("DynamicAO");
            if (hash.componentMask & Comp_Dof)               ImGui::BulletText("DoF");
            if (hash.componentMask & Comp_Vignette)          ImGui::BulletText("Vignette");
            if (hash.componentMask & Comp_FilmGrain)         ImGui::BulletText("FilmGrain");
            if (hash.componentMask & Comp_LensScope)         ImGui::BulletText("LensScope");
            if (hash.componentMask & Comp_CameraParams)      ImGui::BulletText("CameraParams");
            if (hash.componentMask & Comp_ScreenEffect)      ImGui::BulletText("ScreenEffect");
            if (hash.componentMask & Comp_DamageEffect)      ImGui::BulletText("DamageEffect");
            if (hash.componentMask & Comp_PlanarReflection)  ImGui::BulletText("PlanarReflection");
            if (hash.componentMask & Comp_DynamicEnvmap)     ImGui::BulletText("DynamicEnvmap");
            if (hash.componentMask & Comp_CharacterLighting) ImGui::BulletText("CharacterLighting");
            if (hash.componentMask & Comp_MotionBlur)        ImGui::BulletText("MotionBlur");

            if (data.overrideEnabled)
            {
                ImGui::Separator();
                ImGui::TextColored(ImVec4{ 0.3f, 1.0f, 0.3f, 1.0f }, "Override Active");

                if (FindEditEntry(hash))
                    ImGui::TextColored(ImVec4{ 0.5f, 0.8f, 1.0f, 1.0f }, "In Edit List");
            }

            ImGui::EndTooltip();
        }
    }
}

void VisualEnvironmentEditor::RenderStateEditor(fb::VisualEnvironmentState* state, StateEditData& data)
{
    if (!state)
    {
        ImGui::TextColored(ImVec4{ 0.8f, 0.4f, 0.4f, 1.0f }, "No state selected.");
        return;
    }

    int liveCount = StateEditData::CountLiveComponents(state);
    int capturedCount = data.ComponentCount();

    ImGui::TextColored(ImVec4{ 0.6f, 0.8f, 1.0f, 1.0f }, "[%d] %s", data.stateIndex, data.stateName.c_str());
    ImGui::SameLine();
    ImGui::TextDisabled("(%d/%d components)", capturedCount, liveCount);

    ImGui::Text("Visibility: %.3f | Priority: %d", state->visibility, state->priority);

    if (capturedCount < liveCount)
    {
        ImGui::TextColored(ImVec4{ 1.0f, 0.8f, 0.2f, 1.0f }, "! %d new component(s) - capturing automatically", liveCount - capturedCount);
    }

    bool prevOverride = data.overrideEnabled;
    ImGui::Checkbox("Override Enabled", &data.overrideEnabled);
    ImGui::SameLine();
    if (ImGui::Button("Reset to Original"))
        ResetStateToOriginal(data);
    ImGui::SameLine();
    if (ImGui::Button("Force Re-capture"))
        data.ResetCaptures();

    ImGui::Separator();

    if (!data.captured || capturedCount == 0)
    {
        ImGui::TextColored(ImVec4{ 0.8f, 0.4f, 0.4f, 1.0f }, "No components captured yet. Waiting for components to load...");
        return;
    }

    RenderComponentEditors(data);

    if (data.overrideEnabled)
    {
        AddOrUpdateEditEntry(state, data);
    }
    else if (prevOverride && !data.overrideEnabled)
    {
        RemoveEditEntry(ComputeStateHash(state));
    }
}

void VisualEnvironmentEditor::Render()
{
    // TODO: SHOULD BE NOT HERE
    if (m_ShowLocalSphere)
        DrawTransformSphere(m_SelectedEffectMatrix, 2.0f);

    // TODO: SHOULD BE NOT HERE
    if (m_ShowSpawnedEffects)
    {
        for (const auto& effect : m_SpawnedEffects)
        {
            if (ImVec2 pos; render::worldToScreen(effect.transform.m_trans, pos))
            {
                if (size_t posString = effect.effectName.rfind("/"); posString != std::string::npos)
                {
                    render::text(pos, effect.effectName.substr(posString), ImColor{ 0, 255, 255, 128 });
                }
            }
        }
    }

    if (!m_Enabled)
        return;

    ImGui::SetNextWindowSize(ImVec2{ 700, 900 }, ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Visual Environment Editor", &m_Enabled, ImGuiWindowFlags_MenuBar))
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
            if (!m_HasCapturedOriginals)
            {
                ImGui::TextWrapped("No data captured yet. Spawn in-game to begin capturing.");
            }
            else if (m_StateOrder.empty())
            {
                ImGui::TextWrapped("No visual environment states found.");
            }
            else
            {
                ImGui::BeginChild("LeftPanel", ImVec2{ 0, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
                ImGui::SameLine();
                if (ImGui::Button("Scan Names"))
                    ScanVEDataFromResourceManager();

                RenderStateSelector();
                ImGui::EndChild();

                ImGui::SameLine();

                ImGui::BeginChild("RightPanel", ImVec2{ 0, 0 }, ImGuiChildFlags_Border);
                if (m_SelectedStateIndex >= 0 && m_SelectedStateIndex < static_cast<int>(m_StateOrder.size()))
                {
                    auto state = m_StateOrder[m_SelectedStateIndex];
                    auto itr = m_StateDataMap.find(state);
                    if (itr != m_StateDataMap.end())
                        RenderStateEditor(state, itr->second);
                }
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Global VE"))
        {
            RenderGlobalVEEditor();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Lights"))
        {
            RenderLightsTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Emitters"))
        {
            RenderEmitterTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Play effect"))
        {
            RenderEffectSpawnerTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("World Render"))
        {
            if (m_HasCapturedWorldRenderSettings)
            {
                ImGui::Checkbox("Enable World Render Override", &m_WorldRenderOverrideEnabled);
                if (ImGui::Button("Reset to Original"))
                    CopyWorldRenderSettings(&m_EditWorldRenderSettings, &m_OriginalWorldRenderSettings);
                ImGui::Separator();
                RenderWorldRenderSettingsComponent(&m_EditWorldRenderSettings, &m_OriginalWorldRenderSettings);

                if (m_WorldRenderOverrideEnabled)
                {
                    ApplyWorldRenderSettings();
                }
            }
            else
            {
                ImGui::TextColored(ImVec4{ 0.8f, 0.8f, 0.2f, 1.0f }, "Not captured yet. Spawn in-game.");
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Config"))
        {
            RenderConfigSection();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();

    if (m_ShowConsole)
    {
        logger::Render("Console", &m_ShowConsole);
    }
}

void VisualEnvironmentEditor::RenderComponentEditors(StateEditData& data)
{
    ImGui::BeginChild("ComponentList");

#define RENDER_STATE_COMPONENT(label, hasFlag, editMember, origMember, renderFunc) \
    if (data.hasFlag) { \
        ImGui::PushID(label); \
        bool modified = memcmp(&data.editMember, &data.origMember, sizeof(data.editMember)) != 0; \
        if (modified && m_HighlightModified) \
            ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor); \
        bool open = ImGui::CollapsingHeader(label); \
        if (modified && m_HighlightModified) \
            ImGui::PopStyleColor(); \
        if (open) { \
            ImGui::Indent(); \
            renderFunc(&data.editMember, &data.origMember); \
            ImGui::Unindent(); \
        } \
        ImGui::PopID(); \
    }

    RENDER_STATE_COMPONENT("Outdoor Light", hasOutdoorLight, editOutdoorLight, origOutdoorLight, RenderOutdoorLightComponent)
        RENDER_STATE_COMPONENT("Enlighten", hasEnlighten, editEnlighten, origEnlighten, RenderEnlightenComponent)
        RENDER_STATE_COMPONENT("Tonemap", hasTonemap, editTonemap, origTonemap, RenderTonemapComponent)
        RENDER_STATE_COMPONENT("Color Correction", hasColorCorrection, editColorCorrection, origColorCorrection, RenderColorCorrectionComponent)
        RENDER_STATE_COMPONENT("Sky", hasSky, editSky, origSky, RenderSkyComponent)
        RENDER_STATE_COMPONENT("Fog", hasFog, editFog, origFog, RenderFogComponent)
        RENDER_STATE_COMPONENT("Wind", hasWind, editWind, origWind, RenderWindComponent)
        RENDER_STATE_COMPONENT("Sun Flare", hasSunFlare, editSunFlare, origSunFlare, RenderSunFlareComponent)
        RENDER_STATE_COMPONENT("Dynamic AO", hasDynamicAO, editDynamicAO, origDynamicAO, RenderDynamicAOComponent)
        RENDER_STATE_COMPONENT("Depth of Field", hasDof, editDof, origDof, RenderDofComponent)
        RENDER_STATE_COMPONENT("Vignette", hasVignette, editVignette, origVignette, RenderVignetteComponent)
        RENDER_STATE_COMPONENT("Film Grain", hasFilmGrain, editFilmGrain, origFilmGrain, RenderFilmGrainComponent)
        RENDER_STATE_COMPONENT("Lens Scope", hasLensScope, editLensScope, origLensScope, RenderLensScopeComponent)
        RENDER_STATE_COMPONENT("Camera Params", hasCameraParams, editCameraParams, origCameraParams, RenderCameraParamsComponent)
        RENDER_STATE_COMPONENT("Screen Effect", hasScreenEffect, editScreenEffect, origScreenEffect, RenderScreenEffectComponent)
        RENDER_STATE_COMPONENT("Damage Effect", hasDamageEffect, editDamageEffect, origDamageEffect, RenderDamageEffectComponent)
        RENDER_STATE_COMPONENT("Planar Reflection", hasPlanarReflection, editPlanarReflection, origPlanarReflection, RenderPlanarReflectionComponent)
        RENDER_STATE_COMPONENT("Dynamic Envmap", hasDynamicEnvmap, editDynamicEnvmap, origDynamicEnvmap, RenderDynamicEnvmapComponent)
        RENDER_STATE_COMPONENT("Character Lighting", hasCharacterLighting, editCharacterLighting, origCharacterLighting, RenderCharacterLightingComponent)
        RENDER_STATE_COMPONENT("Motion Blur", hasMotionBlur, editMotionBlur, origMotionBlur, RenderMotionBlurComponent)

#undef RENDER_STATE_COMPONENT

        ImGui::EndChild();
}

void VisualEnvironmentEditor::RenderConfigSection()
{
    ImGui::TextColored(ImVec4{ 0.6f, 0.8f, 1.0f, 1.0f }, "Configuration");
    ImGui::Separator();

    ImGui::Checkbox("Auto-load configs on map join", &m_AutoLoadConfigs);
    ImGui::Separator();

    if (!m_CapturedMapName.empty())
    {
        ImGui::Text("Current map: %s", SanitizeMapName(m_CapturedMapName).c_str());

        if (ImGui::Button("Save for this map"))
        {
            if (SaveConfigForCurrentMap())
                ImGui::OpenPopup("ConfigSaved");
        }
        ImGui::SameLine();
        if (ImGui::Button("Load for this map"))
        {
            if (LoadConfigForCurrentMap())
                ImGui::OpenPopup("ConfigLoaded");
        }
    }
    else
    {
        ImGui::TextColored(ImVec4{ 0.8f, 0.4f, 0.4f, 1.0f }, "No map captured yet.");
    }

    ImGui::Separator();
    ImGui::Text("Custom config:");
    ImGui::InputText("##ConfigName", m_ConfigNameBuffer, sizeof(m_ConfigNameBuffer));
    ImGui::SameLine();
    if (ImGui::Button("Save##Custom") && strlen(m_ConfigNameBuffer) > 0)
    {
        std::string path = GetConfigDir() + "/" + m_ConfigDirectory + "/" + m_ConfigNameBuffer + ".json";
        SaveConfig(path);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load##Custom") && strlen(m_ConfigNameBuffer) > 0)
    {
        std::string path = GetConfigDir() + "/" + m_ConfigDirectory + "/" + m_ConfigNameBuffer + ".json";
        LoadConfig(path);
    }

    ImGui::Separator();
    if (ImGui::TreeNode("Available Configs"))
    {
        auto configs = GetAvailableConfigs();
        for (const auto& name : configs)
        {
            if (ImGui::Selectable(name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
            {
                if (ImGui::IsMouseDoubleClicked(0))
                    LoadConfig(GetConfigDir() + "/" + m_ConfigDirectory + "/" + name + ".json");
                else
                    strncpy_s(m_ConfigNameBuffer, name.c_str(), sizeof(m_ConfigNameBuffer) - 1);
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::BeginPopupModal("ConfigSaved", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Configuration saved successfully!");
        if (ImGui::Button("OK", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopupModal("ConfigLoaded", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Configuration loaded successfully!");
        if (ImGui::Button("OK", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void VisualEnvironmentEditor::RenderOutdoorLightComponent(fb::OutdoorLightComponentData* e, const fb::OutdoorLightComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

    if (ImGui::TreeNode("Sun##OL"))
    {
        Vec3Edit("Sun Color", &e->m_SunColor, &o->m_SunColor, true);
        // temp solution to normalize, engine will crash on ub values
        FloatEdit("Sun Rotation X", &e->m_SunRotationX, &o->m_SunRotationX, -179.9f, 179.9f);
        FloatEdit("Sun Rotation Y", &e->m_SunRotationY, &o->m_SunRotationY, -179.9f, 179.9f);
        FloatEdit("Sun Specular Scale", &e->m_SunSpecularScale, &o->m_SunSpecularScale);
        FloatEdit("Sun Shadow Height Scale", &e->m_SunShadowHeightScale, &o->m_SunShadowHeightScale);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Sky##OL"))
    {
        Vec3Edit("Sky Color", &e->m_SkyColor, &o->m_SkyColor, true);
        Vec3Edit("Ground Color", &e->m_GroundColor, &o->m_GroundColor, true);
        FloatEdit("Sky Light Angle Factor", &e->m_SkyLightAngleFactor, &o->m_SkyLightAngleFactor);
        FloatEdit("Sky Envmap Shadow Scale", &e->m_SkyEnvmapShadowScale, &o->m_SkyEnvmapShadowScale);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Cloud Shadow##OL"))
    {
        BoolEdit("Enable##CS", &e->m_CloudShadowEnable, &o->m_CloudShadowEnable);
        Vec2Edit("Speed", &e->m_CloudShadowSpeed, &o->m_CloudShadowSpeed);
        FloatEdit("Coverage", &e->m_CloudShadowCoverage, &o->m_CloudShadowCoverage, 0.0f, 1.0f);
        FloatEdit("Size", &e->m_CloudShadowSize, &o->m_CloudShadowSize);
        FloatEdit("Exponent", &e->m_CloudShadowExponent, &o->m_CloudShadowExponent);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Translucency##OL"))
    {
        FloatEdit("Distortion", &e->m_TranslucencyDistortion, &o->m_TranslucencyDistortion);
        FloatEdit("Ambient", &e->m_TranslucencyAmbient, &o->m_TranslucencyAmbient);
        FloatEdit("Scale", &e->m_TranslucencyScale, &o->m_TranslucencyScale);
        FloatEdit("Power", &e->m_TranslucencyPower, &o->m_TranslucencyPower);
        ImGui::TreePop();
    }
}

void VisualEnvironmentEditor::RenderEnlightenComponent(fb::EnlightenComponentData* e, const fb::EnlightenComponentData* o)
{
    ImGui::Text("Disabling is not prefered!\nDisable if you want to set custom ground/sky lights");

    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
    BoolEdit("SkyBox Enable", &e->m_SkyBoxEnable, &o->m_SkyBoxEnable);

    if (ImGui::TreeNode("SkyBox Colors##EN"))
    {
        ImGui::Text("These only work with dynamic envmap or something, I don't know");

        Vec3Edit("Sky Color", &e->m_SkyBoxSkyColor, &o->m_SkyBoxSkyColor, true);
        Vec3Edit("Ground Color", &e->m_SkyBoxGroundColor, &o->m_SkyBoxGroundColor, true);
        Vec3Edit("Sun Light Color", &e->m_SkyBoxSunLightColor, &o->m_SkyBoxSunLightColor, true);
        Vec3Edit("Back Light Color", &e->m_SkyBoxBackLightColor, &o->m_SkyBoxBackLightColor, true);
        Vec3Edit("Terrain Color", &e->m_TerrainColor, &o->m_TerrainColor, true);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Light Settings##EN"))
    {
        FloatEdit("Bounce Scale", &e->m_BounceScale, &o->m_BounceScale);
        FloatEdit("Sun Scale", &e->m_SunScale, &o->m_SunScale);
        FloatEdit("Cull Distance", &e->m_CullDistance, &o->m_CullDistance);
        FloatEdit("Sun Light Color Size", &e->m_SkyBoxSunLightColorSize, &o->m_SkyBoxSunLightColorSize);
        FloatEdit("Back Light Color Size", &e->m_SkyBoxBackLightColorSize, &o->m_SkyBoxBackLightColorSize);
        FloatEdit("Back Light Rotation X", &e->m_SkyBoxBackLightRotationX, &o->m_SkyBoxBackLightRotationX);
        FloatEdit("Back Light Rotation Y", &e->m_SkyBoxBackLightRotationY, &o->m_SkyBoxBackLightRotationY);
        ImGui::TreePop();
    }
}

void VisualEnvironmentEditor::RenderTonemapComponent(fb::TonemapComponentData* e, const fb::TonemapComponentData* o)
{
    int method = static_cast<int>(e->m_TonemapMethod);
    int origMethod = static_cast<int>(o->m_TonemapMethod);
    if (EnumCombo<fb::TonemapMethod>("Tonemap Method", &method, &origMethod))
        e->m_TonemapMethod = static_cast<fb::TonemapMethod>(method);

    Vec3Edit("Bloom Scale", &e->m_BloomScale, &o->m_BloomScale);
    FloatEdit("Min Exposure", &e->m_MinExposure, &o->m_MinExposure);
    FloatEdit("Max Exposure", &e->m_MaxExposure, &o->m_MaxExposure);
    FloatEdit("Middle Gray", &e->m_MiddleGray, &o->m_MiddleGray);
    FloatEdit("Exposure Adjust Time", &e->m_ExposureAdjustTime, &o->m_ExposureAdjustTime);

    if (ImGui::TreeNode("Chromostereopsis##TM"))
    {
        BoolEdit("Enable##Chromo", &e->m_ChromostereopsisEnable, &o->m_ChromostereopsisEnable);
        FloatEdit("Offset", &e->m_ChromostereopsisOffset, &o->m_ChromostereopsisOffset);
        FloatEdit("Scale##Chromo", &e->m_ChromostereopsisScale, &o->m_ChromostereopsisScale);
        ImGui::TreePop();
    }
}

void VisualEnvironmentEditor::RenderColorCorrectionComponent(fb::ColorCorrectionComponentData* e, const fb::ColorCorrectionComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
    BoolEdit("Color Grading Enable", &e->m_ColorGradingEnable, &o->m_ColorGradingEnable);
    Vec3Edit("Brightness", &e->m_Brightness, &o->m_Brightness);
    Vec3Edit("Contrast", &e->m_Contrast, &o->m_Contrast);
    Vec3Edit("Saturation", &e->m_Saturation, &o->m_Saturation);
    FloatEdit("Hue", &e->m_Hue, &o->m_Hue, -180.0f, 180.0f);
}

void VisualEnvironmentEditor::RenderSkyComponent(fb::SkyComponentData* e, const fb::SkyComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

    if (ImGui::TreeNode("General##Sky"))
    {
        FloatEdit("Brightness Scale", &e->m_BrightnessScale, &o->m_BrightnessScale);
        FloatEdit("Sun Size", &e->m_SunSize, &o->m_SunSize);
        FloatEdit("Sun Scale", &e->m_SunScale, &o->m_SunScale);
        FloatEdit("Sky Visibility Exponent", &e->m_SkyVisibilityExponent, &o->m_SkyVisibilityExponent);
        FloatEdit("Sky Envmap 8Bit Tex Scale", &e->m_SkyEnvmap8BitTexScale, &o->m_SkyEnvmap8BitTexScale);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Cloud Layer 1##Sky"))
    {
        Vec3Edit("Color##CL1", &e->m_CloudLayer1Color, &o->m_CloudLayer1Color, true);
        FloatEdit("Altitude##CL1", &e->m_CloudLayer1Altitude, &o->m_CloudLayer1Altitude);
        FloatEdit("Tile Factor##CL1", &e->m_CloudLayer1TileFactor, &o->m_CloudLayer1TileFactor);
        FloatEdit("Rotation##CL1", &e->m_CloudLayer1Rotation, &o->m_CloudLayer1Rotation, 0.0f, 360.0f);
        FloatEdit("Speed##CL1", &e->m_CloudLayer1Speed, &o->m_CloudLayer1Speed);
        FloatEdit("Sun Light Intensity##CL1", &e->m_CloudLayer1SunLightIntensity, &o->m_CloudLayer1SunLightIntensity);
        FloatEdit("Sun Light Power##CL1", &e->m_CloudLayer1SunLightPower, &o->m_CloudLayer1SunLightPower);
        FloatEdit("Ambient Light Intensity##CL1", &e->m_CloudLayer1AmbientLightIntensity, &o->m_CloudLayer1AmbientLightIntensity);
        FloatEdit("Alpha Mul##CL1", &e->m_CloudLayer1AlphaMul, &o->m_CloudLayer1AlphaMul, 0.0f, 1.0f);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Cloud Layer 2##Sky"))
    {
        Vec3Edit("Color##CL2", &e->m_CloudLayer2Color, &o->m_CloudLayer2Color, true);
        FloatEdit("Altitude##CL2", &e->m_CloudLayer2Altitude, &o->m_CloudLayer2Altitude);
        FloatEdit("Tile Factor##CL2", &e->m_CloudLayer2TileFactor, &o->m_CloudLayer2TileFactor);
        FloatEdit("Rotation##CL2", &e->m_CloudLayer2Rotation, &o->m_CloudLayer2Rotation, 0.0f, 360.0f);
        FloatEdit("Speed##CL2", &e->m_CloudLayer2Speed, &o->m_CloudLayer2Speed);
        FloatEdit("Sun Light Intensity##CL2", &e->m_CloudLayer2SunLightIntensity, &o->m_CloudLayer2SunLightIntensity);
        FloatEdit("Sun Light Power##CL2", &e->m_CloudLayer2SunLightPower, &o->m_CloudLayer2SunLightPower);
        FloatEdit("Ambient Light Intensity##CL2", &e->m_CloudLayer2AmbientLightIntensity, &o->m_CloudLayer2AmbientLightIntensity);
        FloatEdit("Alpha Mul##CL2", &e->m_CloudLayer2AlphaMul, &o->m_CloudLayer2AlphaMul, 0.0f, 1.0f);
        ImGui::TreePop();
    }

    Vec3Edit("Cloud Layer Sun Color", &e->m_CloudLayerSunColor, &o->m_CloudLayerSunColor, true);

    if (ImGui::TreeNode("Panoramic##Sky"))
    {
        FloatEdit("Rotation##Pan", &e->m_PanoramicRotation, &o->m_PanoramicRotation, 0.0f, 360.0f);
        FloatEdit("Tile Factor##Pan", &e->m_PanoramicTileFactor, &o->m_PanoramicTileFactor);
        FloatEdit("UV Min X", &e->m_PanoramicUVMinX, &o->m_PanoramicUVMinX, 0.0f, 1.0f);
        FloatEdit("UV Max X", &e->m_PanoramicUVMaxX, &o->m_PanoramicUVMaxX, 0.0f, 1.0f);
        FloatEdit("UV Min Y", &e->m_PanoramicUVMinY, &o->m_PanoramicUVMinY, 0.0f, 1.0f);
        FloatEdit("UV Max Y", &e->m_PanoramicUVMaxY, &o->m_PanoramicUVMaxY, 0.0f, 1.0f);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Envmap##Sky"))
    {
        FloatEdit("Custom Envmap Scale", &e->m_CustomEnvmapScale, &o->m_CustomEnvmapScale);
        FloatEdit("Custom Envmap Ambient", &e->m_CustomEnvmapAmbient, &o->m_CustomEnvmapAmbient);
        FloatEdit("Static Envmap Scale", &e->m_StaticEnvmapScale, &o->m_StaticEnvmapScale);
        ImGui::TreePop();
    }
}

void VisualEnvironmentEditor::RenderFogComponent(fb::FogComponentData* e, const fb::FogComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

    if (ImGui::TreeNode("Basic Fog##Fog"))
    {
        FloatEdit("Start", &e->m_Start, &o->m_Start);
        FloatEdit("End", &e->m_End, &o->m_End);
        FloatEdit("Distance Multiplier", &e->m_FogDistanceMultiplier, &o->m_FogDistanceMultiplier);
        Vec4Edit("Curve", &e->m_Curve, &o->m_Curve);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Fog Color##Fog"))
    {
        BoolEdit("Enable##FogColor", &e->m_FogColorEnable, &o->m_FogColorEnable);
        Vec3Edit("Color", &e->m_FogColor, &o->m_FogColor, true);
        FloatEdit("Color Start", &e->m_FogColorStart, &o->m_FogColorStart);
        FloatEdit("Color End", &e->m_FogColorEnd, &o->m_FogColorEnd);
        Vec4Edit("Color Curve", &e->m_FogColorCurve, &o->m_FogColorCurve);
        ImGui::TreePop();
    }

    BoolEdit("Gradient Enable", &e->m_FogGradientEnable, &o->m_FogGradientEnable);

    if (ImGui::TreeNode("Height Fog##Fog"))
    {
        BoolEdit("Enable##HeightFog", &e->m_HeightFogEnable, &o->m_HeightFogEnable);
        FloatEdit("Altitude", &e->m_HeightFogAltitude, &o->m_HeightFogAltitude);
        FloatEdit("Depth", &e->m_HeightFogDepth, &o->m_HeightFogDepth);
        FloatEdit("Visibility Range", &e->m_HeightFogVisibilityRange, &o->m_HeightFogVisibilityRange);
        FloatEdit("Follow Camera", &e->m_HeightFogFollowCamera, &o->m_HeightFogFollowCamera, 0.0f, 1.0f);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Transparency Fade##Fog"))
    {
        FloatEdit("Start##TransFade", &e->m_TransparencyFadeStart, &o->m_TransparencyFadeStart);
        FloatEdit("End##TransFade", &e->m_TransparencyFadeEnd, &o->m_TransparencyFadeEnd);
        FloatEdit("Clamp", &e->m_TransparencyFadeClamp, &o->m_TransparencyFadeClamp);
        ImGui::TreePop();
    }
}

void VisualEnvironmentEditor::RenderWindComponent(fb::WindComponentData* e, const fb::WindComponentData* o)
{
    FloatEdit("Direction", &e->m_WindDirection, &o->m_WindDirection, 0.0f, 360.0f);
    FloatEdit("Strength", &e->m_WindStrength, &o->m_WindStrength);
}

void VisualEnvironmentEditor::RenderSunFlareComponent(fb::SunFlareComponentData* e, const fb::SunFlareComponentData* o)
{
    ImGui::Indent();

    BoolEdit("Enable##SunFlare", &e->m_Enable, &o->m_Enable);
    BoolEdit("Debug Draw Occluder", &e->m_DebugDrawOccluder, &o->m_DebugDrawOccluder);
    FloatEdit("Occluder Size", &e->m_OccluderSize, &o->m_OccluderSize);

    for (int i = 1; i <= 5; i++)
    {
        char label[64];
        sprintf_s(label, "Element %d", i);

        if (ImGui::TreeNode(label))
        {
            bool* enable = nullptr;
            fb::Vec2* size = nullptr;
            float* rayDistance = nullptr;
            fb::Vec4* alphaScreenPosCurve = nullptr;
            fb::Vec4* alphaOccluderCurve = nullptr;
            fb::Vec4* sizeScreenPosCurve = nullptr;
            fb::Vec4* sizeOccluderCurve = nullptr;

            const bool* origEnable = nullptr;
            const fb::Vec2* origSize = nullptr;
            const float* origRayDistance = nullptr;
            const fb::Vec4* origAlphaScreenPosCurve = nullptr;
            const fb::Vec4* origAlphaOccluderCurve = nullptr;
            const fb::Vec4* origSizeScreenPosCurve = nullptr;
            const fb::Vec4* origSizeOccluderCurve = nullptr;

            // should have edited sdk dump for this
            switch (i)
            {
            case 1:
                enable = &e->m_Element1Enable;
                size = &e->m_Element1Size;
                rayDistance = &e->m_Element1RayDistance;
                alphaScreenPosCurve = &e->m_Element1AlphaScreenPosCurve;
                alphaOccluderCurve = &e->m_Element1AlphaOccluderCurve;
                sizeScreenPosCurve = &e->m_Element1SizeScreenPosCurve;
                sizeOccluderCurve = &e->m_Element1SizeOccluderCurve;
                origEnable = &o->m_Element1Enable;
                origSize = &o->m_Element1Size;
                origRayDistance = &o->m_Element1RayDistance;
                origAlphaScreenPosCurve = &o->m_Element1AlphaScreenPosCurve;
                origAlphaOccluderCurve = &o->m_Element1AlphaOccluderCurve;
                origSizeScreenPosCurve = &o->m_Element1SizeScreenPosCurve;
                origSizeOccluderCurve = &o->m_Element1SizeOccluderCurve;
                break;
            case 2:
                enable = &e->m_Element2Enable;
                size = &e->m_Element2Size;
                rayDistance = &e->m_Element2RayDistance;
                alphaScreenPosCurve = &e->m_Element2AlphaScreenPosCurve;
                alphaOccluderCurve = &e->m_Element2AlphaOccluderCurve;
                sizeScreenPosCurve = &e->m_Element2SizeScreenPosCurve;
                sizeOccluderCurve = &e->m_Element2SizeOccluderCurve;
                origEnable = &o->m_Element2Enable;
                origSize = &o->m_Element2Size;
                origRayDistance = &o->m_Element2RayDistance;
                origAlphaScreenPosCurve = &o->m_Element2AlphaScreenPosCurve;
                origAlphaOccluderCurve = &o->m_Element2AlphaOccluderCurve;
                origSizeScreenPosCurve = &o->m_Element2SizeScreenPosCurve;
                origSizeOccluderCurve = &o->m_Element2SizeOccluderCurve;
                break;
            case 3:
                enable = &e->m_Element3Enable;
                size = &e->m_Element3Size;
                rayDistance = &e->m_Element3RayDistance;
                alphaScreenPosCurve = &e->m_Element3AlphaScreenPosCurve;
                alphaOccluderCurve = &e->m_Element3AlphaOccluderCurve;
                sizeScreenPosCurve = &e->m_Element3SizeScreenPosCurve;
                sizeOccluderCurve = &e->m_Element3SizeOccluderCurve;
                origEnable = &o->m_Element3Enable;
                origSize = &o->m_Element3Size;
                origRayDistance = &o->m_Element3RayDistance;
                origAlphaScreenPosCurve = &o->m_Element3AlphaScreenPosCurve;
                origAlphaOccluderCurve = &o->m_Element3AlphaOccluderCurve;
                origSizeScreenPosCurve = &o->m_Element3SizeScreenPosCurve;
                origSizeOccluderCurve = &o->m_Element3SizeOccluderCurve;
                break;
            case 4:
                enable = &e->m_Element4Enable;
                size = &e->m_Element4Size;
                rayDistance = &e->m_Element4RayDistance;
                alphaScreenPosCurve = &e->m_Element4AlphaScreenPosCurve;
                alphaOccluderCurve = &e->m_Element4AlphaOccluderCurve;
                sizeScreenPosCurve = &e->m_Element4SizeScreenPosCurve;
                sizeOccluderCurve = &e->m_Element4SizeOccluderCurve;
                origEnable = &o->m_Element4Enable;
                origSize = &o->m_Element4Size;
                origRayDistance = &o->m_Element4RayDistance;
                origAlphaScreenPosCurve = &o->m_Element4AlphaScreenPosCurve;
                origAlphaOccluderCurve = &o->m_Element4AlphaOccluderCurve;
                origSizeScreenPosCurve = &o->m_Element4SizeScreenPosCurve;
                origSizeOccluderCurve = &o->m_Element4SizeOccluderCurve;
                break;
            case 5:
                enable = &e->m_Element5Enable;
                size = &e->m_Element5Size;
                rayDistance = &e->m_Element5RayDistance;
                alphaScreenPosCurve = &e->m_Element5AlphaScreenPosCurve;
                alphaOccluderCurve = &e->m_Element5AlphaOccluderCurve;
                sizeScreenPosCurve = &e->m_Element5SizeScreenPosCurve;
                sizeOccluderCurve = &e->m_Element5SizeOccluderCurve;
                origEnable = &o->m_Element5Enable;
                origSize = &o->m_Element5Size;
                origRayDistance = &o->m_Element5RayDistance;
                origAlphaScreenPosCurve = &o->m_Element5AlphaScreenPosCurve;
                origAlphaOccluderCurve = &o->m_Element5AlphaOccluderCurve;
                origSizeScreenPosCurve = &o->m_Element5SizeScreenPosCurve;
                origSizeOccluderCurve = &o->m_Element5SizeOccluderCurve;
                break;
            }

            BoolEdit("Enable", enable, origEnable);
            Vec2Edit("Size", size, origSize);
            FloatEdit("Ray Distance", rayDistance, origRayDistance);
            Vec4Edit("Alpha Screen Pos Curve", alphaScreenPosCurve, origAlphaScreenPosCurve);
            Vec4Edit("Alpha Occluder Curve", alphaOccluderCurve, origAlphaOccluderCurve);
            Vec4Edit("Size Screen Pos Curve", sizeScreenPosCurve, origSizeScreenPosCurve);
            Vec4Edit("Size Occluder Curve", sizeOccluderCurve, origSizeOccluderCurve);

            ImGui::TreePop();
        }
    }

    ImGui::Unindent();
}

void VisualEnvironmentEditor::RenderDynamicAOComponent(fb::DynamicAOComponentData* e, const fb::DynamicAOComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

    if (ImGui::TreeNode("SSAO##DAO"))
    {
        FloatEdit("Radius", &e->m_SsaoRadius, &o->m_SsaoRadius);
        FloatEdit("Fade", &e->m_SsaoFade, &o->m_SsaoFade);
        FloatEdit("Max Distance Inner", &e->m_SsaoMaxDistanceInner, &o->m_SsaoMaxDistanceInner);
        FloatEdit("Max Distance Outer", &e->m_SsaoMaxDistanceOuter, &o->m_SsaoMaxDistanceOuter);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("HBAO##DAO"))
    {
        FloatEdit("Radius", &e->m_HbaoRadius, &o->m_HbaoRadius);
        FloatEdit("Attenuation", &e->m_HbaoAttenuation, &o->m_HbaoAttenuation);
        FloatEdit("Angle Bias", &e->m_HbaoAngleBias, &o->m_HbaoAngleBias);
        FloatEdit("Power Exponent", &e->m_HbaoPowerExponent, &o->m_HbaoPowerExponent);
        FloatEdit("Contrast", &e->m_HbaoContrast, &o->m_HbaoContrast);
        FloatEdit("Max Footprint Radius", &e->m_HbaoMaxFootprintRadius, &o->m_HbaoMaxFootprintRadius);
        ImGui::TreePop();
    }
}

void VisualEnvironmentEditor::RenderDofComponent(fb::DofComponentData* e, const fb::DofComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

    int filter = static_cast<int>(e->m_BlurFilter);
    int origFilter = static_cast<int>(o->m_BlurFilter);
    if (EnumCombo<fb::BlurFilter>("Blur Filter", &filter, &origFilter))
        e->m_BlurFilter = static_cast<fb::BlurFilter>(filter);

    FloatEdit("Focus Distance", &e->m_FocusDistance, &o->m_FocusDistance);
    FloatEdit("Near Distance Scale", &e->m_NearDistanceScale, &o->m_NearDistanceScale);
    FloatEdit("Far Distance Scale", &e->m_FarDistanceScale, &o->m_FarDistanceScale);
    FloatEdit("Blur Filter Deviation", &e->m_BlurFilterDeviation, &o->m_BlurFilterDeviation);
    FloatEdit("Blur Add", &e->m_BlurAdd, &o->m_BlurAdd);
    FloatEdit("Scale", &e->m_Scale, &o->m_Scale);

    if (ImGui::TreeNode("Diffusion DoF##DoF"))
    {
        BoolEdit("Enable##DiffDoF", &e->m_DiffusionDofEnable, &o->m_DiffusionDofEnable);
        FloatEdit("Focal Length", &e->m_DiffusionDofFocalLength, &o->m_DiffusionDofFocalLength);
        FloatEdit("Aperture", &e->m_DiffusionDofAperture, &o->m_DiffusionDofAperture);
        ImGui::TreePop();
    }
}

void VisualEnvironmentEditor::RenderVignetteComponent(fb::VignetteComponentData* e, const fb::VignetteComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
    Vec3Edit("Color", &e->m_Color, &o->m_Color, true);
    Vec2Edit("Scale", &e->m_Scale, &o->m_Scale);
    FloatEdit("Opacity", &e->m_Opacity, &o->m_Opacity, 0.0f, 1.0f);
    FloatEdit("Exponent", &e->m_Exponent, &o->m_Exponent);
}

void VisualEnvironmentEditor::RenderFilmGrainComponent(fb::FilmGrainComponentData* e, const fb::FilmGrainComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
    BoolEdit("Linear Filtering", &e->m_LinearFilteringEnable, &o->m_LinearFilteringEnable);
    BoolEdit("Random Enable", &e->m_RandomEnable, &o->m_RandomEnable);
    Vec3Edit("Color Scale", &e->m_ColorScale, &o->m_ColorScale);
    Vec2Edit("Texture Scale", &e->m_TextureScale, &o->m_TextureScale);
}

void VisualEnvironmentEditor::RenderLensScopeComponent(fb::LensScopeComponentData* e, const fb::LensScopeComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
    Vec2Edit("Blur Center", &e->m_BlurCenter, &o->m_BlurCenter);
    FloatEdit("Blur Scale", &e->m_BlurScale, &o->m_BlurScale);
    Vec2Edit("Radial Blend Coeffs", &e->m_RadialBlendDistanceCoefficients, &o->m_RadialBlendDistanceCoefficients);

    if (ImGui::TreeNode("Chromatic Aberration##LS"))
    {
        Vec3Edit("Color 1", &e->m_ChromaticAberrationColor1, &o->m_ChromaticAberrationColor1, true);
        Vec3Edit("Color 2", &e->m_ChromaticAberrationColor2, &o->m_ChromaticAberrationColor2, true);
        Vec2Edit("Displacement 1", &e->m_ChromaticAberrationDisplacement1, &o->m_ChromaticAberrationDisplacement1);
        Vec2Edit("Displacement 2", &e->m_ChromaticAberrationDisplacement2, &o->m_ChromaticAberrationDisplacement2);
        Vec2Edit("Strengths", &e->m_ChromaticAberrationStrengths, &o->m_ChromaticAberrationStrengths);
        ImGui::TreePop();
    }
}

void VisualEnvironmentEditor::RenderCameraParamsComponent(fb::CameraParamsComponentData* e, const fb::CameraParamsComponentData* o)
{
    FloatEdit("View Distance", &e->m_ViewDistance, &o->m_ViewDistance);
    FloatEdit("Near Plane", &e->m_NearPlane, &o->m_NearPlane);
    FloatEdit("Sun Shadowmap View Distance", &e->m_SunShadowmapViewDistance, &o->m_SunShadowmapViewDistance);
}

void VisualEnvironmentEditor::RenderScreenEffectComponent(fb::ScreenEffectComponentData* e, const fb::ScreenEffectComponentData* o)
{
    int type = static_cast<int>(e->m_FrameType);
    int origType = static_cast<int>(o->m_FrameType);
    if (EnumCombo<fb::ScreenEffectFrameType>("Frame Type", &type, &origType))
        e->m_FrameType = static_cast<fb::ScreenEffectFrameType>(type);

    Vec4Edit("Screen Effect Params", &e->m_ScreenEffectParams, &o->m_ScreenEffectParams);
    FloatEdit("Frame Width", &e->m_FrameWidth, &o->m_FrameWidth);
    FloatEdit("Outer Frame Opacity", &e->m_OuterFrameOpacity, &o->m_OuterFrameOpacity, 0.0f, 1.0f);
    FloatEdit("Inner Frame Opacity", &e->m_InnerFrameOpacity, &o->m_InnerFrameOpacity, 0.0f, 1.0f);
    FloatEdit("Angle", &e->m_Angle, &o->m_Angle, 0.0f, 360.0f);
}

void VisualEnvironmentEditor::RenderDamageEffectComponent(fb::DamageEffectComponentData* e, const fb::DamageEffectComponentData* o)
{
    BoolEdit("Debug Damage", &e->m_DebugDamage, &o->m_DebugDamage);

    if (ImGui::TreeNode("Damage Direction##DE"))
    {
        Vec4Edit("Top", &e->m_TopDamage, &o->m_TopDamage);
        Vec4Edit("Bottom", &e->m_BottomDamage, &o->m_BottomDamage);
        Vec4Edit("Left", &e->m_LeftDamage, &o->m_LeftDamage);
        Vec4Edit("Right", &e->m_RightDamage, &o->m_RightDamage);
        ImGui::TreePop();
    }

    FloatEdit("Frame Width", &e->m_FrameWidth, &o->m_FrameWidth);
    FloatEdit("Outer Frame Opacity", &e->m_OuterFrameOpacity, &o->m_OuterFrameOpacity, 0.0f, 1.0f);
    FloatEdit("Inner Frame Opacity", &e->m_InnerFrameOpacity, &o->m_InnerFrameOpacity, 0.0f, 1.0f);
    FloatEdit("Start Critical Threshold", &e->m_StartCriticalEffectHealthThreshold, &o->m_StartCriticalEffectHealthThreshold, 0.0f, 1.0f);
    FloatEdit("End Critical Threshold", &e->m_EndCriticalEffectHealthThreshold, &o->m_EndCriticalEffectHealthThreshold, 0.0f, 1.0f);
    FloatEdit("Min Damage % Threshold", &e->m_MinDamagePercentageThreshold, &o->m_MinDamagePercentageThreshold);
    FloatEdit("Falloff Time", &e->m_FallofTime, &o->m_FallofTime);
    FloatEdit("Max Opacity Damage %", &e->m_MaxOpacityDamagePercentage, &o->m_MaxOpacityDamagePercentage);
}

void VisualEnvironmentEditor::RenderPlanarReflectionComponent(fb::PlanarReflectionComponentData* e, const fb::PlanarReflectionComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
    BoolEdit("Sky Render Enable", &e->m_SkyRenderEnable, &o->m_SkyRenderEnable);
    FloatEdit("Ground Height", &e->m_GroundHeight, &o->m_GroundHeight);

    int hFilter = static_cast<int>(e->m_HorizontalBlurFilter);
    int origHFilter = static_cast<int>(o->m_HorizontalBlurFilter);
    if (EnumCombo<fb::BlurFilter>("H Blur Filter", &hFilter, &origHFilter))
        e->m_HorizontalBlurFilter = static_cast<fb::BlurFilter>(hFilter);

    FloatEdit("H Deviation", &e->m_HorizontalDeviation, &o->m_HorizontalDeviation);

    int vFilter = static_cast<int>(e->m_VerticalBlurFilter);
    int origVFilter = static_cast<int>(o->m_VerticalBlurFilter);
    if (EnumCombo<fb::BlurFilter>("V Blur Filter", &vFilter, &origVFilter))
        e->m_VerticalBlurFilter = static_cast<fb::BlurFilter>(vFilter);

    FloatEdit("V Deviation", &e->m_VerticalDeviation, &o->m_VerticalDeviation);
}

void VisualEnvironmentEditor::RenderDynamicEnvmapComponent(fb::DynamicEnvmapComponentData* e, const fb::DynamicEnvmapComponentData* o)
{
    BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
    Vec3Edit("Sky Color", &e->m_SkyColorEnvmap, &o->m_SkyColorEnvmap, true);
    Vec3Edit("Ground Color", &e->m_GroundColorEnvmap, &o->m_GroundColorEnvmap, true);
    Vec3Edit("Key Color", &e->m_KeyColorEnvmap, &o->m_KeyColorEnvmap, true);
}

void VisualEnvironmentEditor::RenderCharacterLightingComponent(fb::CharacterLightingComponentData* e, const fb::CharacterLightingComponentData* o)
{
    BoolEdit("Enable", &e->m_CharacterLightEnable, &o->m_CharacterLightEnable);
    BoolEdit("First Person Enable", &e->m_FirstPersonEnable, &o->m_FirstPersonEnable);
    BoolEdit("Lock to Camera Direction", &e->m_LockToCameraDirection, &o->m_LockToCameraDirection);

    int mode = static_cast<int>(e->m_CharacterLightingMode);
    int origMode = static_cast<int>(o->m_CharacterLightingMode);
    if (EnumCombo<fb::CharacterLightingMode>("Lighting Mode", &mode, &origMode))
        e->m_CharacterLightingMode = static_cast<fb::CharacterLightingMode>(mode);

    Vec3Edit("Top Light", &e->m_TopLight, &o->m_TopLight, true);
    Vec3Edit("Bottom Light", &e->m_BottomLight, &o->m_BottomLight, true);
    FloatEdit("Top Light Dir X", &e->m_TopLightDirX, &o->m_TopLightDirX);
    FloatEdit("Top Light Dir Y", &e->m_TopLightDirY, &o->m_TopLightDirY);
    FloatEdit("Camera Up Rotation", &e->m_CameraUpRotation, &o->m_CameraUpRotation);
    FloatEdit("Blend Factor", &e->m_BlendFactor, &o->m_BlendFactor, 0.0f, 1.0f);
}

void VisualEnvironmentEditor::RenderMotionBlurComponent(fb::MotionBlurComponentData* e, const fb::MotionBlurComponentData* o)
{
    BoolEdit("Enable", &e->m_MotionBlurEnable, &o->m_MotionBlurEnable);
    FloatEdit("Cutoff Radius", &e->m_MotionBlurCutoffRadius, &o->m_MotionBlurCutoffRadius);
    FloatEdit("Cutoff Gradient Scale", &e->m_CutoffGradientScale, &o->m_CutoffGradientScale);
}

void VisualEnvironmentEditor::RenderWorldRenderSettingsComponent(fb::WorldRenderSettings* e, const fb::WorldRenderSettings* o)
{
    if (ImGui::TreeNode("Viewport & Quality"))
    {
        FloatEdit("Viewport Scale", &e->m_ViewportScale, &o->m_ViewportScale, 0.1f, 2.0f);
        UIntEdit("FXAA Quality", &e->m_FxaaQuality, &o->m_FxaaQuality, 0, 5);
        IntEdit("FXAA Force Vendor", &e->m_FxaaForceVendor, &o->m_FxaaForceVendor, -1, 3);
        BoolEdit("FXAA Enable", &e->m_FxaaEnable, &o->m_FxaaEnable);
        FloatEdit("Cull Screen Area Scale", &e->m_CullScreenAreaScale, &o->m_CullScreenAreaScale);
        UIntEdit("Multisample Count", &e->m_MultisampleCount, &o->m_MultisampleCount, 1, 8);
        FloatEdit("Multisample Threshold", &e->m_MultisampleThreshold, &o->m_MultisampleThreshold);
        BoolEdit("HDR Enable", &e->m_HdrEnable, &o->m_HdrEnable);
        BoolEdit("Half Res Enable", &e->m_HalfResEnable, &o->m_HalfResEnable);
        BoolEdit("Bilateral Half Res Composite", &e->m_BilateralHalfResCompositeEnable, &o->m_BilateralHalfResCompositeEnable);
        BoolEdit("Tiled Half Res Composite", &e->m_TiledHalfResCompositeEnable, &o->m_TiledHalfResCompositeEnable);
        BoolEdit("Tiled Half Res Stencil Occluders", &e->m_TiledHalfResStencilOccludersEnable, &o->m_TiledHalfResStencilOccludersEnable);
        BoolEdit("Output Gamma Correction", &e->m_OutputGammaCorrectionEnable, &o->m_OutputGammaCorrectionEnable);
        BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Shadows"))
    {
        BoolEdit("Shadowmaps Enable", &e->m_ShadowmapsEnable, &o->m_ShadowmapsEnable);
        BoolEdit("Generate Shadowmaps", &e->m_GenerateShadowmapsEnable, &o->m_GenerateShadowmapsEnable);
        BoolEdit("Apply Shadowmaps", &e->m_ApplyShadowmapsEnable, &o->m_ApplyShadowmapsEnable);
        BoolEdit("Simple Shadowmaps", &e->m_SimpleShadowmapsEnable, &o->m_SimpleShadowmapsEnable);
        BoolEdit("Transparency Shadowmaps", &e->m_TransparencyShadowmapsEnable, &o->m_TransparencyShadowmapsEnable);
        BoolEdit("Cloud Shadow", &e->m_CloudShadowEnable, &o->m_CloudShadowEnable);

        ImGui::Separator();
        UIntEdit("Resolution", &e->m_ShadowmapResolution, &o->m_ShadowmapResolution, 256, 4096);
        UIntEdit("Quality", &e->m_ShadowmapQuality, &o->m_ShadowmapQuality, 0, 4);
        UIntEdit("Slice Count", &e->m_ShadowmapSliceCount, &o->m_ShadowmapSliceCount, 1, 8);
        FloatEdit("View Distance", &e->m_ShadowmapViewDistance, &o->m_ShadowmapViewDistance);
        BoolEdit("View Distance Scale Enable", &e->m_ShadowmapViewDistanceScaleEnable, &o->m_ShadowmapViewDistanceScaleEnable);
        FloatEdit("Min Screen Area", &e->m_ShadowMinScreenArea, &o->m_ShadowMinScreenArea);
        FloatEdit("Shadowmap Min Screen Area", &e->m_ShadowmapMinScreenArea, &o->m_ShadowmapMinScreenArea);
        FloatEdit("Viewport Scale", &e->m_ShadowViewportScale, &o->m_ShadowViewportScale);
        FloatEdit("Size Z Scale", &e->m_ShadowmapSizeZScale, &o->m_ShadowmapSizeZScale);
        FloatEdit("Slice Scheme Weight", &e->m_ShadowmapSliceSchemeWeight, &o->m_ShadowmapSliceSchemeWeight);
        FloatEdit("First Slice Scale", &e->m_ShadowmapFirstSliceScale, &o->m_ShadowmapFirstSliceScale);
        FloatEdit("Extrusion Length", &e->m_ShadowmapExtrusionLength, &o->m_ShadowmapExtrusionLength);
        FloatEdit("Min FOV", &e->m_ShadowmapMinFov, &o->m_ShadowmapMinFov);
        FloatEdit("Poisson Filter Scale", &e->m_ShadowmapPoissonFilterScale, &o->m_ShadowmapPoissonFilterScale);

        ImGui::Separator();
        BoolEdit("Fixed Depth", &e->m_ShadowmapFixedDepthEnable, &o->m_ShadowmapFixedDepthEnable);
        BoolEdit("Fixed Movement", &e->m_ShadowmapFixedMovementEnable, &o->m_ShadowmapFixedMovementEnable);
        BoolEdit("Cull Volume", &e->m_ShadowmapCullVolumeEnable, &o->m_ShadowmapCullVolumeEnable);
        BoolEdit("Accum Enable", &e->m_ShadowmapAccumEnable, &o->m_ShadowmapAccumEnable);
        BoolEdit("Accum Bilinear", &e->m_ShadowmapAccumBilinearEnable, &o->m_ShadowmapAccumBilinearEnable);
        BoolEdit("Colored Slices", &e->m_ColoredShadowmapSlicesEnable, &o->m_ColoredShadowmapSlicesEnable);
        BoolEdit("Z Buffer Test", &e->m_ZBufferShadowTestEnable, &o->m_ZBufferShadowTestEnable);
        BoolEdit("DX 16-Bit", &e->m_DxShadowmap16BitEnable, &o->m_DxShadowmap16BitEnable);

        IntEdit("Only Slice (debug)", &e->m_OnlyShadowmapSlice, &o->m_OnlyShadowmapSlice, -1, 8);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Motion Blur"))
    {
        BoolEdit("Enable", &e->m_MotionBlurEnable, &o->m_MotionBlurEnable);
        BoolEdit("Geometry Pass", &e->m_MotionBlurGeometryPassEnable, &o->m_MotionBlurGeometryPassEnable);
        BoolEdit("Stencil Pass", &e->m_MotionBlurStencilPassEnable, &o->m_MotionBlurStencilPassEnable);
        FloatEdit("Scale", &e->m_MotionBlurScale, &o->m_MotionBlurScale, 0.0f, 2.0f);
        FloatEdit("Max", &e->m_MotionBlurMax, &o->m_MotionBlurMax);
        FloatEdit("Noise Scale", &e->m_MotionBlurNoiseScale, &o->m_MotionBlurNoiseScale);
        UIntEdit("Quality", &e->m_MotionBlurQuality, &o->m_MotionBlurQuality, 0, 4);
        UIntEdit("Max Sample Count", &e->m_MotionBlurMaxSampleCount, &o->m_MotionBlurMaxSampleCount, 1, 64);
        UIntEdit("Frame Average Count", &e->m_MotionBlurFrameAverageCount, &o->m_MotionBlurFrameAverageCount, 1, 16);
        FloatEdit("Max Frame Time", &e->m_MotionBlurMaxFrameTime, &o->m_MotionBlurMaxFrameTime);
        FloatEdit("Force Depth Cutoff", &e->m_ForceMotionBlurDepthCutoff, &o->m_ForceMotionBlurDepthCutoff);
        FloatEdit("Cutoff Gradient Scale", &e->m_ForceMotionBlurCutoffGradientScale, &o->m_ForceMotionBlurCutoffGradientScale);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Lighting - General"))
    {
        BoolEdit("Outdoor Light Enable", &e->m_OutdoorLightEnable, &o->m_OutdoorLightEnable);
        BoolEdit("Outdoor Key Light", &e->m_OutdoorKeyLightEnable, &o->m_OutdoorKeyLightEnable);
        BoolEdit("Outdoor Sky Light", &e->m_OutdoorSkyLightEnable, &o->m_OutdoorSkyLightEnable);
        BoolEdit("Outdoor Light Specular", &e->m_OutdoorLightSpecularEnable, &o->m_OutdoorLightSpecularEnable);
        BoolEdit("Outdoor Light Tiling", &e->m_OutdoorLightTilingEnable, &o->m_OutdoorLightTilingEnable);
        BoolEdit("Outdoor Light Tile Render", &e->m_OutdoorLightTileRenderEnable, &o->m_OutdoorLightTileRenderEnable);
        BoolEdit("Outdoor Light Tile Blend", &e->m_OutdoorLightTileBlendEnable, &o->m_OutdoorLightTileBlendEnable);
        BoolEdit("Outdoor Light Tile Simple Shader", &e->m_OutdoorLightTileSimpleShaderEnable, &o->m_OutdoorLightTileSimpleShaderEnable);
        UIntEdit("Outdoor Light Tile Batch Count", &e->m_OutdoorLightTileBatchCount, &o->m_OutdoorLightTileBatchCount);

        ImGui::Separator();
        FloatEdit("Intensity Scale", &e->m_LightIntensityScale, &o->m_LightIntensityScale);
        FloatEdit("Force Intensity", &e->m_LightForceIntensity, &o->m_LightForceIntensity);
        FloatEdit("Radius Scale", &e->m_LightRadiusScale, &o->m_LightRadiusScale);
        FloatEdit("Attenuation Threshold", &e->m_LightAttenuationThreshold, &o->m_LightAttenuationThreshold);
        BoolEdit("Attenuation Threshold Enable", &e->m_LightAttenuationThresholdEnable, &o->m_LightAttenuationThresholdEnable);
        BoolEdit("Width Enable", &e->m_LightWidthEnable, &o->m_LightWidthEnable);
        BoolEdit("Intensity Normalization", &e->m_LightIntensityNormalizationEnable, &o->m_LightIntensityNormalizationEnable);

        ImGui::Separator();
        BoolEdit("Specular Lighting", &e->m_SpecularLightingEnable, &o->m_SpecularLightingEnable);
        BoolEdit("Skin Lighting", &e->m_SkinLightingEnable, &o->m_SkinLightingEnable);
        BoolEdit("Translucency Lighting", &e->m_TranslucencyLightingEnable, &o->m_TranslucencyLightingEnable);
        BoolEdit("Emissive Enable", &e->m_EmissiveEnable, &o->m_EmissiveEnable);
        BoolEdit("Unlit Enable", &e->m_UnlitEnable, &o->m_UnlitEnable);
        BoolEdit("Draw Light Sources", &e->m_DrawLightSources, &o->m_DrawLightSources);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Lighting - LOD"))
    {
        FloatEdit("LOD Specular Fade Start", &e->m_LightLodSpecularFadeAreaStart, &o->m_LightLodSpecularFadeAreaStart);
        FloatEdit("LOD Specular Fade End", &e->m_LightLodSpecularFadeAreaEnd, &o->m_LightLodSpecularFadeAreaEnd);
        FloatEdit("LOD Radius Factor", &e->m_LightLodRadiusFactor, &o->m_LightLodRadiusFactor);
        FloatEdit("LOD Fade Area", &e->m_LightLodFadeArea, &o->m_LightLodFadeArea);
        FloatEdit("LOD Min Area", &e->m_LightLodMinArea, &o->m_LightLodMinArea);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Lighting - Culling"))
    {
        BoolEdit("Cull Enable", &e->m_LightCullEnable, &o->m_LightCullEnable);
        BoolEdit("Depth Cull", &e->m_LightDepthCullEnable, &o->m_LightDepthCullEnable);
        BoolEdit("Normal Cull", &e->m_LightNormalCullEnable, &o->m_LightNormalCullEnable);
        BoolEdit("Cone Cull", &e->m_LightConeCullEnable, &o->m_LightConeCullEnable);
        FloatEdit("Stencil Min Area", &e->m_LightStencilMinArea, &o->m_LightStencilMinArea);
        BoolEdit("Stencil Method", &e->m_LightStencilMethodEnable, &o->m_LightStencilMethodEnable);
        BoolEdit("Volume Method", &e->m_LightVolumeMethodEnable, &o->m_LightVolumeMethodEnable);
        BoolEdit("Volume Depth Test", &e->m_LightVolumeDepthTestEnable, &o->m_LightVolumeDepthTestEnable);
        FloatEdit("Tile Min Area", &e->m_LightTileMinArea, &o->m_LightTileMinArea);
        BoolEdit("Tile Overlay", &e->m_LightTileOverlayEnable, &o->m_LightTileOverlayEnable);
        UIntEdit("Overdraw Max Layer Count", &e->m_LightOverdrawMaxLayerCount, &o->m_LightOverdrawMaxLayerCount);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Light Types"))
    {
        BoolEdit("Point Lights", &e->m_PointLightsEnable, &o->m_PointLightsEnable);
        UIntEdit("Max Point Light Count", &e->m_MaxPointLightCount, &o->m_MaxPointLightCount, 0, 256);

        ImGui::Separator();
        BoolEdit("Spot Lights", &e->m_SpotLightsEnable, &o->m_SpotLightsEnable);
        UIntEdit("Max Spot Light Count", &e->m_MaxSpotLightCount, &o->m_MaxSpotLightCount, 0, 64);
        UIntEdit("Max Spot Shadow Count", &e->m_MaxSpotLightShadowCount, &o->m_MaxSpotLightShadowCount, 0, 32);
        UIntEdit("Spot Shadowmap Resolution", &e->m_SpotLightShadowmapResolution, &o->m_SpotLightShadowmapResolution, 128, 2048);
        UIntEdit("Spot Shadowmap Quality", &e->m_SpotLightShadowmapQuality, &o->m_SpotLightShadowmapQuality, 0, 4);
        FloatEdit("Spot Near Plane", &e->m_SpotLightNearPlane, &o->m_SpotLightNearPlane);
        FloatEdit("Spot Poisson Filter Scale", &e->m_SpotLightShadowmapPoissonFilterScale, &o->m_SpotLightShadowmapPoissonFilterScale);
        BoolEdit("Spot Shadowmap Enable", &e->m_SpotLightShadowmapEnable, &o->m_SpotLightShadowmapEnable);
        BoolEdit("DX Spot Shadowmap 16-Bit", &e->m_DxSpotLightShadowmap16BitEnable, &o->m_DxSpotLightShadowmap16BitEnable);

        ImGui::Separator();
        BoolEdit("Line Lights", &e->m_LineLightsEnable, &o->m_LineLightsEnable);
        BoolEdit("Cone Lights", &e->m_ConeLightsEnable, &o->m_ConeLightsEnable);
        BoolEdit("Light Probes", &e->m_LightProbesEnable, &o->m_LightProbesEnable);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Sky"))
    {
        BoolEdit("Sky Enable", &e->m_SkyEnable, &o->m_SkyEnable);
        BoolEdit("Sky Fog Enable", &e->m_SkyFogEnable, &o->m_SkyFogEnable);

        ImGui::Separator();
        BoolEdit("Sky Envmap Enable", &e->m_SkyEnvmapEnable, &o->m_SkyEnvmapEnable);
        BoolEdit("Sky Envmap Update", &e->m_SkyEnvmapUpdateEnable, &o->m_SkyEnvmapUpdateEnable);
        BoolEdit("Sky Envmap Force Update", &e->m_SkyEnvmapForceUpdateEnable, &o->m_SkyEnvmapForceUpdateEnable);
        BoolEdit("Sky Envmap Mipmap Gen", &e->m_SkyEnvmapMipmapGenEnable, &o->m_SkyEnvmapMipmapGenEnable);
        BoolEdit("Sky Envmap 8-Bit Texture", &e->m_SkyEnvmapUse8BitTexture, &o->m_SkyEnvmapUse8BitTexture);
        UIntEdit("Sky Envmap Resolution", &e->m_SkyEnvmapResolution, &o->m_SkyEnvmapResolution, 16, 512);
        UIntEdit("Sky Envmap Sides Per Frame", &e->m_SkyEnvmapSidesPerFrameCount, &o->m_SkyEnvmapSidesPerFrameCount, 1, 6);
        FloatEdit("Sky Envmap Filter Width", &e->m_SkyEnvmapFilterWidth, &o->m_SkyEnvmapFilterWidth);
        EnumCombo<fb::MipmapFilterMode>("Sky Envmap Filter Mode", reinterpret_cast<int*>(&e->m_SkyEnvmapFilterMode), reinterpret_cast<const int*>(&o->m_SkyEnvmapFilterMode));
        BoolEdit("Sky Visibility Envmap Scaling", &e->m_SkyVisibilityEnvmapScalingEnable, &o->m_SkyVisibilityEnvmapScalingEnable);
        IntEdit("Debug Sky Envmap Mip Level", &e->m_DrawDebugSkyEnvmapMipLevel, &o->m_DrawDebugSkyEnvmapMipLevel, -1, 10);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Dynamic Environment Map"))
    {
        BoolEdit("Enable", &e->m_DynamicEnvmapEnable, &o->m_DynamicEnvmapEnable);
        BoolEdit("Lighting Enable", &e->m_DynamicEnvmapLightingEnable, &o->m_DynamicEnvmapLightingEnable);
        BoolEdit("Mipmap Gen", &e->m_DynamicEnvmapMipmapGenEnable, &o->m_DynamicEnvmapMipmapGenEnable);
        UIntEdit("Resolution", &e->m_DynamicEnvmapResolution, &o->m_DynamicEnvmapResolution, 16, 512);
        FloatEdit("Filter Width", &e->m_DynamicEnvmapFilterWidth, &o->m_DynamicEnvmapFilterWidth);
        EnumCombo<fb::MipmapFilterMode>("Dynamic Mode", reinterpret_cast<int*>(&e->m_DynamicEnvmapFilterMode), reinterpret_cast<const int*>(&o->m_DynamicEnvmapFilterMode));
        Vec3Edit("Default Position", &e->m_DynamicEnvmapDefaultPosition, &o->m_DynamicEnvmapDefaultPosition);
        IntEdit("Debug Mip Level", &e->m_DrawDebugDynamicEnvmapMipLevel, &o->m_DrawDebugDynamicEnvmapMipLevel, -1, 10);

        ImGui::Separator();
        BoolEdit("Static Envmap Enable", &e->m_StaticEnvmapEnable, &o->m_StaticEnvmapEnable);
        BoolEdit("Custom Envmap Enable", &e->m_CustomEnvmapEnable, &o->m_CustomEnvmapEnable);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Reflections"))
    {
        BoolEdit("Planar Reflection Enable", &e->m_PlanarReflectionEnable, &o->m_PlanarReflectionEnable);
        BoolEdit("Planar Reflection Blur", &e->m_PlanarReflectionBlur, &o->m_PlanarReflectionBlur);
        UIntEdit("Planar Reflection Width", &e->m_PlanarReflectionWidth, &o->m_PlanarReflectionWidth, 128, 2048);
        UIntEdit("Planar Reflection Height", &e->m_PlanarReflectionHeight, &o->m_PlanarReflectionHeight, 128, 2048);
        FloatEdit("Planar Reflection Cull FOV", &e->m_PlanarReflectionCullFOV, &o->m_PlanarReflectionCullFOV);
        FloatEdit("Temp Planar Reflection Y", &e->m_TempPlanarReflectionY, &o->m_TempPlanarReflectionY);
        BoolEdit("Debug Cull Mode", &e->m_PlanarReflectionDebugCullMode, &o->m_PlanarReflectionDebugCullMode);
        BoolEdit("Freeze Cull Frustum", &e->m_FreezePlanarReflectionCullFrustum, &o->m_FreezePlanarReflectionCullFrustum);
        UIntEdit("Reflection Envmap Size", &e->m_ReflectionEnvmapSize, &o->m_ReflectionEnvmapSize, 16, 512);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Subsurface Scattering"))
    {
        Vec3Edit("Color", &e->m_SubSurfaceColor, &o->m_SubSurfaceColor, true);
        FloatEdit("Rolloff Key Light", &e->m_SubSurfaceRolloffKeyLight, &o->m_SubSurfaceRolloffKeyLight);
        FloatEdit("Rolloff Local Light", &e->m_SubSurfaceRolloffLocalLight, &o->m_SubSurfaceRolloffLocalLight);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Decals & Volumes"))
    {
        BoolEdit("Decal Volume Enable", &e->m_DecalVolumeEnable, &o->m_DecalVolumeEnable);
        UIntEdit("Max Decal Volume Count", &e->m_MaxDecalVolumeCount, &o->m_MaxDecalVolumeCount, 0, 256);
        FloatEdit("Decal Volume Scale", &e->m_DecalVolumeScale, &o->m_DecalVolumeScale);
        UIntEdit("Max Destruction Volume Count", &e->m_MaxDestructionVolumeCount, &o->m_MaxDestructionVolumeCount, 0, 256);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Render Passes"))
    {
        BoolEdit("Z Pass", &e->m_ZPassEnable, &o->m_ZPassEnable);
        BoolEdit("Main Opaque Z Pass", &e->m_MainOpaqueZPassEnable, &o->m_MainOpaqueZPassEnable);
        BoolEdit("Occluder Mesh Z Prepass", &e->m_OccluderMeshZPrepassEnable, &o->m_OccluderMeshZPrepassEnable);
        BoolEdit("Occluder Mesh Z Prepass Debug", &e->m_OccluderMeshZPrepassDebugEnable, &o->m_OccluderMeshZPrepassDebugEnable);

        ImGui::Separator();
        BoolEdit("Foreground Enable", &e->m_ForegroundEnable, &o->m_ForegroundEnable);
        BoolEdit("Foreground Z Pass", &e->m_ForegroundZPassEnable, &o->m_ForegroundZPassEnable);
        BoolEdit("Foreground Depth Clear", &e->m_ForegroundDepthClearEnable, &o->m_ForegroundDepthClearEnable);
        BoolEdit("Foreground As Main", &e->m_ForegroundAsMainEnable, &o->m_ForegroundAsMainEnable);
        BoolEdit("Foreground Transparent", &e->m_ForegroundTransparentEnable, &o->m_ForegroundTransparentEnable);

        ImGui::Separator();
        BoolEdit("Draw Transparent", &e->m_DrawTransparent, &o->m_DrawTransparent);
        BoolEdit("Draw Transparent Decal", &e->m_DrawTransparentDecal, &o->m_DrawTransparentDecal);
        BoolEdit("Screen Effect Enable", &e->m_ScreenEffectEnable, &o->m_ScreenEffectEnable);
        BoolEdit("Final Post Enable", &e->m_FinalPostEnable, &o->m_FinalPostEnable);
        BoolEdit("View FX Enable", &e->m_ViewFxEnable, &o->m_ViewFxEnable);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Lens Flares"))
    {
        BoolEdit("Enable", &e->m_LensFlaresEnable, &o->m_LensFlaresEnable);
        BoolEdit("Half Res", &e->m_HalfResLensFlaresEnable, &o->m_HalfResLensFlaresEnable);
        BoolEdit("Occlusion Enable", &e->m_LensFlareOcclusionEnable, &o->m_LensFlareOcclusionEnable);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("GBuffer"))
    {
        BoolEdit("Clear Enable", &e->m_GBufferClearEnable, &o->m_GBufferClearEnable);
        BoolEdit("Alpha Test Simple", &e->m_GBufferAlphaTestSimpleEnable, &o->m_GBufferAlphaTestSimpleEnable);
        BoolEdit("DX Light 16-Bit", &e->m_DxGBufferLight16BitEnable, &o->m_DxGBufferLight16BitEnable);
        BoolEdit("DX Normal 16-Bit", &e->m_DxGBufferNormal16BitEnable, &o->m_DxGBufferNormal16BitEnable);
        BoolEdit("DX Linear Depth 32-Bit", &e->m_DxLinearDepth32BitFormatEnable, &o->m_DxLinearDepth32BitFormatEnable);
        BoolEdit("DX Deferred CS Path", &e->m_DxDeferredCsPathEnable, &o->m_DxDeferredCsPathEnable);
        UIntEdit("Test Count", &e->m_GBufferTestCount, &o->m_GBufferTestCount);
        EnumCombo<fb::ShaderGBufferLayout>("Layout", reinterpret_cast<int*>(&e->m_GBufferLayout), reinterpret_cast<const int*>(&o->m_GBufferLayout));
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Dynamic AO"))
    {
        BoolEdit("Override Dynamic AO", &e->m_OverrideDynamicAO, &o->m_OverrideDynamicAO);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Misc Rendering"))
    {
        BoolEdit("Wireframe", &e->m_WireframeEnable, &o->m_WireframeEnable);
        BoolEdit("Opaque Sort By Solution", &e->m_OpaqueSortBySolutionEnable, &o->m_OpaqueSortBySolutionEnable);
        BoolEdit("Generic Entity Renderer", &e->m_GenericEntityRendererEnable, &o->m_GenericEntityRendererEnable);
        UIntEdit("Generic Entity Max Visible", &e->m_GenericEntityMaxVisibleEntityCount, &o->m_GenericEntityMaxVisibleEntityCount);
        BoolEdit("Setup Job Enable", &e->m_SetupJobEnable, &o->m_SetupJobEnable);
        BoolEdit("Dynamic Cull Z Buffer Test", &e->m_DynamicCullZBufferTestEnable, &o->m_DynamicCullZBufferTestEnable);
        BoolEdit("Dynamic Cull Draw Occluded Boxes", &e->m_DynamicCullDrawOccludedBoxesEnable, &o->m_DynamicCullDrawOccludedBoxesEnable);
        EnumCombo<fb::WorldViewMode>("View Mode", reinterpret_cast<int*>(&e->m_ViewMode), reinterpret_cast<const int*>(&o->m_ViewMode));
        IntEdit("Only Tile Index", &e->m_OnlyTileIndex, &o->m_OnlyTileIndex, -1, 64);
        IntEdit("Only Light Tile X", &e->m_OnlyLightTileX, &o->m_OnlyLightTileX, -1, 64);
        IntEdit("Only Light Tile Y", &e->m_OnlyLightTileY, &o->m_OnlyLightTileY, -1, 64);
        IntEdit("Only Light Tile Index", &e->m_OnlyLightTileIndex, &o->m_OnlyLightTileIndex, -1, 256);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Debug Visualization"))
    {
        if (ImGui::TreeNode("Buffers"))
        {
            BoolEdit("GBuffer", &e->m_DrawDebugGBuffer, &o->m_DrawDebugGBuffer);
            BoolEdit("Z Buffer", &e->m_DrawDebugZBufferEnable, &o->m_DrawDebugZBufferEnable);
            BoolEdit("Velocity Buffer", &e->m_DrawDebugVelocityBuffer, &o->m_DrawDebugVelocityBuffer);
            BoolEdit("Multisample Classify", &e->m_DrawDebugMultisampleClassify, &o->m_DrawDebugMultisampleClassify);
            BoolEdit("Half Res Environment", &e->m_DrawDebugHalfResEnvironment, &o->m_DrawDebugHalfResEnvironment);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Shadows"))
        {
            BoolEdit("Shadowmaps", &e->m_DrawDebugShadowmaps, &o->m_DrawDebugShadowmaps);
            BoolEdit("Quarter Shadowmaps", &e->m_DrawDebugQuarterShadowmaps, &o->m_DrawDebugQuarterShadowmaps);
            BoolEdit("Trans Shadowmap", &e->m_DrawDebugTransShadowmap, &o->m_DrawDebugTransShadowmap);
            BoolEdit("Spot Light Shadowmaps", &e->m_DrawDebugSpotLightShadowmaps, &o->m_DrawDebugSpotLightShadowmaps);
            BoolEdit("Shadow Frustums", &e->m_DrawShadowFrustums, &o->m_DrawShadowFrustums);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lights"))
        {
            BoolEdit("Light Sources", &e->m_DrawDebugLightSources, &o->m_DrawDebugLightSources);
            BoolEdit("Light Shadow Sources", &e->m_DrawDebugLightShadowSources, &o->m_DrawDebugLightShadowSources);
            BoolEdit("Light Stats", &e->m_DrawDebugLightStats, &o->m_DrawDebugLightStats);
            BoolEdit("Light Tiles", &e->m_DrawDebugLightTiles, &o->m_DrawDebugLightTiles);
            BoolEdit("Light Tile Volumes", &e->m_DrawDebugLightTileVolumes, &o->m_DrawDebugLightTileVolumes);
            BoolEdit("Light Tile FB Grid", &e->m_DrawDebugLightTileFbGrid, &o->m_DrawDebugLightTileFbGrid);
            BoolEdit("Light Tile Grid", &e->m_DrawDebugLightTileGrid, &o->m_DrawDebugLightTileGrid);
            BoolEdit("Light Tile Sources", &e->m_DrawDebugLightTileSources, &o->m_DrawDebugLightTileSources);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Environment"))
        {
            BoolEdit("Dynamic AO", &e->m_DrawDebugDynamicAO, &o->m_DrawDebugDynamicAO);
            BoolEdit("Dynamic Envmap", &e->m_DrawDebugDynamicEnvmap, &o->m_DrawDebugDynamicEnvmap);
            BoolEdit("Sky Envmap", &e->m_DrawDebugSkyEnvmap, &o->m_DrawDebugSkyEnvmap);
            BoolEdit("Planar Reflection", &e->m_DrawDebugPlanarReflection, &o->m_DrawDebugPlanarReflection);
            BoolEdit("Planar Reflection Cull Frustum", &e->m_DrawDebugPlanarReflectionCullFrustum, &o->m_DrawDebugPlanarReflectionCullFrustum);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Volumes & Misc"))
        {
            BoolEdit("Decal Volumes", &e->m_DrawDebugDecalVolumes, &o->m_DrawDebugDecalVolumes);
            BoolEdit("Destruction Volumes", &e->m_DrawDebugDestructionVolumes, &o->m_DrawDebugDestructionVolumes);
            BoolEdit("Lens Flares", &e->m_DrawDebugLensFlares, &o->m_DrawDebugLensFlares);
            BoolEdit("Lens Flare Occluders", &e->m_DrawDebugLensFlareOccluders, &o->m_DrawDebugLensFlareOccluders);
            BoolEdit("World Occlusions", &e->m_DrawDebugWorldOcclusions, &o->m_DrawDebugWorldOcclusions);
            BoolEdit("Visible Entity Types", &e->m_DrawDebugVisibleEntityTypes, &o->m_DrawDebugVisibleEntityTypes);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Bounding Geometry"))
        {
            BoolEdit("Bounding Spheres", &e->m_DrawBoundingSpheres, &o->m_DrawBoundingSpheres);
            BoolEdit("Solid Bounding Boxes", &e->m_DrawSolidBoundingBoxes, &o->m_DrawSolidBoundingBoxes);
            BoolEdit("Line Bounding Boxes", &e->m_DrawLineBoundingBoxes, &o->m_DrawLineBoundingBoxes);
            BoolEdit("Frustums", &e->m_DrawFrustums, &o->m_DrawFrustums);
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

bool VisualEnvironmentEditor::Vec2Edit(const char* label, fb::Vec2* c, const fb::Vec2* o)
{
    bool mod = memcmp(c, o, sizeof(fb::Vec2)) != 0;
    if (mod && m_HighlightModified)
        ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);

    float v[2] = { c->m_x, c->m_y };
    bool changed = ImGui::DragFloat2(label, v, 0.01f);
    if (changed)
    {
        c->m_x = v[0];
        c->m_y = v[1];
    }

    if (mod && m_HighlightModified)
        ImGui::PopStyleColor();

    if (m_ShowOriginalValues && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextColored(m_OriginalColor, "Original: (%.3f, %.3f)", o->m_x, o->m_y);
        ImGui::EndTooltip();
    }

    if (mod)
    {
        ImGui::SameLine();
        ImGui::PushID(label);
        if (ImGui::SmallButton("R"))
        {
            *c = *o;
            changed = true;
        }
        ImGui::PopID();
    }

    return changed;
}

bool VisualEnvironmentEditor::Vec3Edit(const char* label, fb::Vec3* c, const fb::Vec3* o, bool isColor)
{
    bool mod = memcmp(c, o, sizeof(fb::Vec3)) != 0;
    if (mod && m_HighlightModified)
        ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);

    float v[3] = { c->m_x, c->m_y, c->m_z };
    bool changed = isColor ? ImGui::ColorEdit3(label, v, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float) : ImGui::DragFloat3(label, v, 0.01f);
    if (changed)
    {
        c->m_x = v[0];
        c->m_y = v[1];
        c->m_z = v[2];
    }

    if (mod && m_HighlightModified)
        ImGui::PopStyleColor();

    if (m_ShowOriginalValues && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextColored(m_OriginalColor, "Original: (%.3f, %.3f, %.3f)", o->m_x, o->m_y, o->m_z);
        ImGui::EndTooltip();
    }

    if (mod)
    {
        ImGui::SameLine();
        ImGui::PushID(label);
        if (ImGui::SmallButton("R"))
        {
            *c = *o;
            changed = true;
        }
        ImGui::PopID();
    }

    return changed;
}

bool VisualEnvironmentEditor::Vec4Edit(const char* label, fb::Vec4* c, const fb::Vec4* o, bool isColor)
{
    bool mod = memcmp(c, o, sizeof(fb::Vec4)) != 0;
    if (mod && m_HighlightModified)
        ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);

    float v[4] = { c->m_x, c->m_y, c->m_z, c->m_w };
    bool changed = isColor ? ImGui::ColorEdit4(label, v, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float) : ImGui::DragFloat4(label, v, 0.01f);
    if (changed)
    {
        c->m_x = v[0];
        c->m_y = v[1];
        c->m_z = v[2];
        c->m_w = v[3];
    }

    if (mod && m_HighlightModified)
        ImGui::PopStyleColor();

    if (m_ShowOriginalValues && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextColored(m_OriginalColor, "Original: (%.3f, %.3f, %.3f, %.3f)", o->m_x, o->m_y, o->m_z, o->m_w);
        ImGui::EndTooltip();
    }

    if (mod)
    {
        ImGui::SameLine();
        ImGui::PushID(label);
        if (ImGui::SmallButton("R"))
        {
            *c = *o;
            changed = true;
        }
        ImGui::PopID();
    }

    return changed;
}

bool VisualEnvironmentEditor::FloatEdit(const char* label, float* c, const float* o, float min, float max, const char* format)
{
    bool mod = *c != *o;
    if (mod && m_HighlightModified)
        ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);

    bool changed;
    if (min != 0.0f || max != 0.0f)
        changed = ImGui::SliderFloat(label, c, min, max, format);
    else
        changed = ImGui::DragFloat(label, c, 0.01f, 0.0f, 0.0f, format);

    if (mod && m_HighlightModified)
        ImGui::PopStyleColor();

    if (m_ShowOriginalValues && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextColored(m_OriginalColor, "Original: %.3f", *o);
        ImGui::EndTooltip();
    }

    if (mod)
    {
        ImGui::SameLine();
        ImGui::PushID(label);
        if (ImGui::SmallButton("R"))
        {
            *c = *o;
            changed = true;
        }
        ImGui::PopID();
    }

    return changed;
}

bool VisualEnvironmentEditor::BoolEdit(const char* label, bool* c, const bool* o)
{
    bool mod = *c != *o;
    if (mod && m_HighlightModified)
        ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);

    bool changed = ImGui::Checkbox(label, c);

    if (mod && m_HighlightModified)
        ImGui::PopStyleColor();

    if (m_ShowOriginalValues && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextColored(m_OriginalColor, "Original: %s", *o ? "true" : "false");
        ImGui::EndTooltip();
    }

    if (mod)
    {
        ImGui::SameLine();
        ImGui::PushID(label);
        if (ImGui::SmallButton("R"))
        {
            *c = *o;
            changed = true;
        }
        ImGui::PopID();
    }

    return changed;
}

bool VisualEnvironmentEditor::IntEdit(const char* label, int* c, const int* o, int min, int max)
{
    bool mod = *c != *o;
    if (mod && m_HighlightModified)
        ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);

    bool changed;
    if (min != 0 || max != 0)
        changed = ImGui::SliderInt(label, c, min, max);
    else
        changed = ImGui::DragInt(label, c);

    if (mod && m_HighlightModified)
        ImGui::PopStyleColor();

    if (m_ShowOriginalValues && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextColored(m_OriginalColor, "Original: %d", *o);
        ImGui::EndTooltip();
    }

    if (mod)
    {
        ImGui::SameLine();
        ImGui::PushID(label);
        if (ImGui::SmallButton("R"))
        {
            *c = *o;
            changed = true;
        }
        ImGui::PopID();
    }

    return changed;
}

bool VisualEnvironmentEditor::UIntEdit(const char* label, uint32_t* c, const uint32_t* o, uint32_t min, uint32_t max)
{
    bool mod = *c != *o;
    if (mod && m_HighlightModified)
        ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);

    bool changed;
    if (min != 0 || max != 0)
        changed = ImGui::SliderScalar(label, ImGuiDataType_U32, c, &min, &max);
    else
        changed = ImGui::DragScalar(label, ImGuiDataType_U32, c);

    if (mod && m_HighlightModified)
        ImGui::PopStyleColor();

    if (m_ShowOriginalValues && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextColored(m_OriginalColor, "Original: %u", *o);
        ImGui::EndTooltip();
    }

    if (mod)
    {
        ImGui::SameLine();
        ImGui::PushID(label);
        if (ImGui::SmallButton("R"))
        {
            *c = *o;
            changed = true;
        }
        ImGui::PopID();
    }

    return changed;
}

void VisualEnvironmentEditor::RenderLightsTab()
{
    ImGui::TextColored(ImVec4{ 0.6f, 0.8f, 1.0f, 1.0f }, "Light Data Editing");
    ImGui::Separator();

    ImGui::InputTextWithHint("##LightFilter", "Filter by asset name...", m_LightFilterBuffer, sizeof(m_LightFilterBuffer));

    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);

    // no need for enumcombo here, special case
    const char* typeFilters[] = { "All", "SpotLight", "PointLight", "LocalLight" };
    ImGui::Combo("##LightType", &m_LightTypeFilter, typeFilters, IM_ARRAYSIZE(typeFilters));

    ImGui::SameLine();
    if (ImGui::Button("Scan Lights"))
        ScanAndApplyLightOverrides();

    ImGui::SameLine();
    if (ImGui::Button("Reset All"))
    {
        std::lock_guard<std::mutex> lock(m_LightDataMutex);
        for (auto& [dataPtr, entry] : m_LightDataEntries)
        {
            if (entry.origCaptured)
            {
                entry.ResetToOriginal();
                entry.hasOverride = false;
                ApplyLightOverride(entry);
            }
        }
    }

    ImGui::Separator();

    std::vector<std::pair<fb::LocalLightEntityData*, LightDataEntry*>> entries;
    {
        std::lock_guard<std::mutex> lock(m_LightDataMutex);
        for (auto& [ptr, entry] : m_LightDataEntries)
            entries.emplace_back(ptr, &entry);
    }

    if (entries.empty())
    {
        ImGui::TextColored(ImVec4{ 0.8f, 0.8f, 0.2f, 1.0f }, "No light data loaded. Spawn in-game or click 'Scan Lights'.");
        return;
    }

    size_t totalActive = 0;
    size_t totalWithOverride = 0;
    for (const auto& [ptr, entry] : entries)
    {
        totalActive += entry->ActiveCount();
        if (entry->hasOverride)
            totalWithOverride++;
    }

    ImGui::Text("Light Types: %zu | Active Instances: %zu | Overrides: %zu", entries.size(), totalActive, totalWithOverride);
    ImGui::Separator();

    std::string filterStr = m_LightFilterBuffer;
    std::transform(filterStr.begin(), filterStr.end(), filterStr.begin(), ::tolower);
    std::sort(entries.begin(), entries.end(),
        [](const auto& a, const auto& b) { return a.second->assetName < b.second->assetName; });

    ImGui::BeginChild("LightList", ImVec2(0, 0), true);

    for (auto& [dataPtr, entry] : entries)
    {
        if (m_LightTypeFilter == 1 && !entry->isSpotLight) continue;
        if (m_LightTypeFilter == 2 && !entry->isPointLight) continue;
        if (m_LightTypeFilter == 3 && (entry->isSpotLight || entry->isPointLight)) continue;

        if (!filterStr.empty())
        {
            std::string lowerName = entry->assetName;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            if (lowerName.find(filterStr) == std::string::npos)
                continue;
        }

        ImVec4 textColor;
        if (entry->hasOverride)
            textColor = ImVec4{ 0.2f, 1.0f, 0.2f, 1.0f };
        else if (entry->ActiveCount() == 0)
            textColor = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };
        else
            textColor = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };

        ImGui::PushID(dataPtr);

        // prob icon better
        const char* typePrefix = entry->isSpotLight ? "[S]" : (entry->isPointLight ? "[P]" : "[L]");
        ImGui::TextColored(ImVec4{ 0.6f, 0.6f, 0.8f, 1.0f }, "%s", typePrefix);
        ImGui::SameLine();

        char header[512];
        sprintf_s(header, "%s - %zu active%s###LightHeader",
            entry->assetName.c_str(),
            entry->ActiveCount(),
            entry->hasOverride ? " [MODIFIED]" : "");

        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        bool open = ImGui::CollapsingHeader(header);
        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("Asset: %s", entry->assetName.c_str());
            ImGui::Text("Type: %s", entry->lightType.c_str());
            ImGui::Text("Data Ptr: %p", dataPtr);
            ImGui::Text("Active Entities: %zu", entry->ActiveCount());
            ImGui::Text("Original Captured: %s", entry->origCaptured ? "Yes" : "No");
            ImGui::Text("Override: %s", entry->hasOverride ? "Enabled" : "Disabled");
            if (entry->origCaptured)
            {
                ImGui::Separator();
                ImGui::TextColored(m_OriginalColor, "Original Values:");
                ImGui::Text("  Color: (%.2f, %.2f, %.2f)",
                    entry->origColor.m_x, entry->origColor.m_y, entry->origColor.m_z);
                ImGui::Text("  Radius: %.2f", entry->origRadius);
                ImGui::Text("  Intensity: %.2f", entry->origIntensity);
            }
            ImGui::EndTooltip();
        }

        if (open)
        {
            ImGui::Indent();
            RenderLightDataEditor(*entry);
            ImGui::Unindent();
        }

        ImGui::PopID();
    }

    ImGui::EndChild();
}

void VisualEnvironmentEditor::RenderLightDataEditor(LightDataEntry& entry)
{
    if (!entry.origCaptured)
    {
        ImGui::TextColored(ImVec4{ 0.8f, 0.8f, 0.2f, 1.0f }, "No entity spawned yet - values not captured");
        return;
    }

    bool changed = false;
    if (ImGui::Checkbox("Enable Override", &entry.hasOverride))
    {
        if (entry.hasOverride)
            changed = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset to Original"))
    {
        entry.ResetToOriginal();
        if (entry.hasOverride)
            changed = true;
    }

    if (!entry.hasOverride)
    {
        ImGui::TextDisabled("Enable override to edit values");

        if (ImGui::TreeNode("Current Values (read-only)"))
        {
            ImGui::TextDisabled("Color: (%.3f, %.3f, %.3f)", entry.color.m_x, entry.color.m_y, entry.color.m_z);
            ImGui::TextDisabled("Radius: %.3f", entry.radius);
            ImGui::TextDisabled("Intensity: %.3f", entry.intensity);
            ImGui::TextDisabled("Visible: %s", entry.visible ? "Yes" : "No");
            ImGui::TreePop();
        }
        return;
    }

    ImGui::Separator();

    if (ImGui::TreeNodeEx("Common##Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        float color[3] = { entry.color.m_x, entry.color.m_y, entry.color.m_z };
        if (ImGui::ColorEdit3("Color", color, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float))
        {
            entry.color = { color[0], color[1], color[2] };
            changed = true;
        }
        if (m_ShowOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(m_OriginalColor, "Original: (%.3f, %.3f, %.3f)",
                entry.origColor.m_x, entry.origColor.m_y, entry.origColor.m_z);
            ImGui::EndTooltip();
        }

        bool radiusMod = entry.radius != entry.origRadius;
        if (radiusMod && m_HighlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);
        if (ImGui::DragFloat("Radius", &entry.radius, 0.1f, 0.0f, 1000.0f))
            changed = true;
        if (radiusMod && m_HighlightModified)
            ImGui::PopStyleColor();
        if (radiusMod)
        {
            ImGui::SameLine();
            if (ImGui::SmallButton("R##Radius"))
            {
                entry.radius = entry.origRadius;
                changed = true;
            }
        }

        bool intensityMod = entry.intensity != entry.origIntensity;
        if (intensityMod && m_HighlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);
        if (ImGui::DragFloat("Intensity", &entry.intensity, 0.01f, 0.0f, 100.0f))
            changed = true;
        if (intensityMod && m_HighlightModified)
            ImGui::PopStyleColor();
        if (intensityMod)
        {
            ImGui::SameLine();
            if (ImGui::SmallButton("R##Intensity"))
            {
                entry.intensity = entry.origIntensity;
                changed = true;
            }
        }

        if (ImGui::DragFloat("Attenuation Offset", &entry.attenuationOffset, 0.01f))
            changed = true;

        if (ImGui::Checkbox("Visible", &entry.visible))
            changed = true;
        ImGui::SameLine();
        if (ImGui::Checkbox("Specular", &entry.specularEnable))
            changed = true;
        ImGui::SameLine();
        if (ImGui::Checkbox("Enlighten", &entry.enlightenEnable))
            changed = true;

        if (EnumCombo<fb::EnlightenColorMode>("Enlighten Color Mode", &entry.enlightenColorMode))
            changed = true;

        if (ImGui::TreeNode("Color Scales##Light"))
        {
            float particleColor[3] = { entry.particleColorScale.m_x, entry.particleColorScale.m_y, entry.particleColorScale.m_z };
            if (ImGui::ColorEdit3("Particle Color Scale", particleColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float))
            {
                entry.particleColorScale = { particleColor[0], particleColor[1], particleColor[2] };
                changed = true;
            }

            float enlightenColor[3] = { entry.enlightenColorScale.m_x, entry.enlightenColorScale.m_y, entry.enlightenColorScale.m_z };
            if (ImGui::ColorEdit3("Enlighten Color Scale", enlightenColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float))
            {
                entry.enlightenColorScale = { enlightenColor[0], enlightenColor[1], enlightenColor[2] };
                changed = true;
            }
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    if (entry.isSpotLight && ImGui::TreeNode("SpotLight Properties##Light"))
    {
        const char* shapes[] = { "Cone", "Frustum", "Ortho" };
        if (ImGui::Combo("Shape", &entry.spotShape, shapes, 3))
            changed = true;

        if (ImGui::SliderFloat("Cone Inner Angle", &entry.coneInnerAngle, 0.0f, 90.0f))
            changed = true;

        if (ImGui::SliderFloat("Cone Outer Angle", &entry.coneOuterAngle, 0.0f, 90.0f))
            changed = true;

        if (ImGui::DragFloat("Frustum FOV", &entry.frustumFov, 0.1f))
            changed = true;

        if (ImGui::DragFloat("Frustum Aspect", &entry.frustumAspect, 0.01f))
            changed = true;

        if (ImGui::DragFloat("Ortho Width", &entry.orthoWidth, 0.1f))
            changed = true;

        if (ImGui::DragFloat("Ortho Height", &entry.orthoHeight, 0.1f))
            changed = true;

        if (ImGui::Checkbox("Cast Shadows", &entry.castShadowsEnable))
            changed = true;

        if (EnumCombo<fb::QualityLevel>("Shadow Min Level", &entry.castShadowsMinLevel))
            changed = true;

        ImGui::TreePop();
    }

    if (entry.isPointLight && ImGui::TreeNode("PointLight Properties##Light"))
    {
        if (ImGui::DragFloat("Width", &entry.pointWidth, 0.1f))
            changed = true;

        if (ImGui::DragFloat("Translucency Ambient", &entry.translucencyAmbient, 0.01f))
            changed = true;

        if (ImGui::DragFloat("Translucency Scale", &entry.translucencyScale, 0.01f))
            changed = true;

        if (ImGui::DragScalar("Translucency Power", ImGuiDataType_U32, &entry.translucencyPower))
            changed = true;

        if (ImGui::DragFloat("Translucency Distortion", &entry.translucencyDistortion, 0.01f))
            changed = true;

        ImGui::TreePop();
    }

    if (m_ShowOriginalValues && ImGui::TreeNode("Original Values##Light"))
    {
        ImGui::TextColored(m_OriginalColor, "Color: (%.3f, %.3f, %.3f)",
            entry.origColor.m_x, entry.origColor.m_y, entry.origColor.m_z);
        ImGui::TextColored(m_OriginalColor, "Radius: %.3f", entry.origRadius);
        ImGui::TextColored(m_OriginalColor, "Intensity: %.3f", entry.origIntensity);
        ImGui::TextColored(m_OriginalColor, "Attenuation: %.3f", entry.origAttenuationOffset);
        ImGui::TextColored(m_OriginalColor, "Visible: %s | Specular: %s | Enlighten: %s",
            entry.origVisible ? "Yes" : "No",
            entry.origSpecularEnable ? "Yes" : "No",
            entry.origEnlightenEnable ? "Yes" : "No");

        if (entry.isSpotLight)
        {
            ImGui::TextColored(m_OriginalColor, "Spot: Shape=%d Inner=%.1f Outer=%.1f",
                entry.origSpotShape, entry.origConeInnerAngle, entry.origConeOuterAngle);
        }
        if (entry.isPointLight)
        {
            ImGui::TextColored(m_OriginalColor, "Point: Width=%.3f", entry.origPointWidth);
        }

        ImGui::TreePop();
    }

    if (changed && entry.hasOverride)
    {
        ApplyLightOverride(entry);
    }
}

static bool containsIgnoreCase(const std::string& haystack, const std::string& needle)
{
    if (needle.empty()) return true;

    auto itr = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.end(),
        [](char a, char b) { return std::tolower(a) == std::tolower(b); });
    return itr != haystack.end();
}

void VisualEnvironmentEditor::RenderEmitterProperties()
{
    if (!m_SelectedEmitter)
    {
        ImGui::TextDisabled("Select an emitter");
        return;
    }

    auto it = m_EmitterMap.find(m_SelectedEmitter);
    if (it == m_EmitterMap.end())
        return;

    EmitterEditData& edit = it->second;
    fb::EmitterTemplateData* d = edit.data;

    if (!edit.captured)
    {
        edit.original.CaptureFrom(d);
        edit.captured = true;
    }

    const auto& o = edit.original;

    ImGui::Text("%s", edit.name.c_str());
    if (edit.lastTemplate)
        ImGui::TextColored(ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f }, "Active");
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Basic", ImGuiTreeNodeFlags_DefaultOpen))
    {
        edit.modified |= FloatEdit("Time Scale", &d->m_TimeScale, &o.timeScale);
        edit.modified |= FloatEdit("Lifetime", &d->m_Lifetime, &o.lifetime);
        edit.modified |= UIntEdit("Max Count", &d->m_MaxCount, &o.maxCount);
        edit.modified |= UIntEdit("Lifetime Frames", &d->m_LifetimeFrameCount, &o.lifetimeFrameCount);
    }

    if (ImGui::CollapsingHeader("Distance & Culling"))
    {
        edit.modified |= FloatEdit("Max Spawn Distance", &d->m_MaxSpawnDistance, &o.maxSpawnDistance);
        edit.modified |= FloatEdit("Visible After Dist", &d->m_VisibleAfterDistance, &o.visibleAfterDistance);
        edit.modified |= FloatEdit("Culling Factor", &d->m_ParticleCullingFactor, &o.particleCullingFactor);
        edit.modified |= FloatEdit("Mesh Culling Dist", &d->m_MeshCullingDistance, &o.meshCullingDistance);
        edit.modified |= FloatEdit("Min Screen Area", &d->m_MinScreenArea, &o.minScreenArea);
        edit.modified |= FloatEdit("Dist Scale Near", &d->m_DistanceScaleNearValue, &o.distanceScaleNearValue);
        edit.modified |= FloatEdit("Dist Scale Far", &d->m_DistanceScaleFarValue, &o.distanceScaleFarValue);
        edit.modified |= FloatEdit("Dist Scale Length", &d->m_DistanceScaleLength, &o.distanceScaleLength);
        edit.modified |= BoolEdit("Exclusion Volume Cull", &d->m_ExclusionVolumeCullEnable, &o.exclusionVolumeCullEnable);
    }

    if (ImGui::CollapsingHeader("Lighting"))
    {
        edit.modified |= FloatEdit("Light Multiplier", &d->m_LightMultiplier, &o.lightMultiplier);
        edit.modified |= FloatEdit("Light Wrap Around", &d->m_LightWrapAroundFactor, &o.lightWrapAroundFactor);
        edit.modified |= FloatEdit("Vertex/Pixel Blend", &d->m_VertexPixelLightingBlendFactor, &o.vertexPixelLightingBlendFactor);
        edit.modified |= FloatEdit("Global/Local Blend", &d->m_GlobalLocalNormalBlendFactor, &o.globalLocalNormalBlendFactor);
        edit.modified |= FloatEdit("Soft Particles Fade", &d->m_SoftParticlesFadeDistanceMultiplier, &o.softParticlesFadeDistanceMultiplier);
        edit.modified |= BoolEdit("Emissive", &d->m_Emissive, &o.emissive);
    }

    if (ImGui::CollapsingHeader("Point Light"))
    {
        edit.modified |= BoolEdit("Act As Point Light", &d->m_ActAsPointLight, &o.actAsPointLight);
        edit.modified |= Vec3Edit("Color", &d->m_PointLightColor, &o.pointLightColor, true);
        edit.modified |= Vec4Edit("Intensity", &d->m_PointLightIntensity, &o.pointLightIntensity, false);
        edit.modified |= Vec3Edit("Pivot", &d->m_PointLightPivot, &o.pointLightPivot, false);
        edit.modified |= FloatEdit("Radius", &d->m_PointLightRadius, &o.pointLightRadius);
        edit.modified |= FloatEdit("Min Clamp", &d->m_PointLightMinClamp, &o.pointLightMinClamp);
        edit.modified |= FloatEdit("Max Clamp", &d->m_PointLightMaxClamp, &o.pointLightMaxClamp);
        edit.modified |= FloatEdit("Random Min", &d->m_PointLightRandomIntensityMin, &o.pointLightRandomIntensityMin);
        edit.modified |= FloatEdit("Random Max", &d->m_PointLightRandomIntensityMax, &o.pointLightRandomIntensityMax);
    }

    if (ImGui::CollapsingHeader("Rendering"))
    {
        edit.modified |= BoolEdit("Opaque", &d->m_Opaque, &o.opaque);
        edit.modified |= BoolEdit("Force Full Res", &d->m_ForceFullRes, &o.forceFullRes);
        edit.modified |= BoolEdit("Sun Shadow", &d->m_TransparencySunShadowEnable, &o.transparencySunShadowEnable);
        edit.modified |= BoolEdit("Force Nice Sorting", &d->m_ForceNiceSorting, &o.forceNiceSorting);
    }

    if (ImGui::CollapsingHeader("Behavior"))
    {
        edit.modified |= BoolEdit("Local Space", &d->m_LocalSpace, &o.localSpace);
        edit.modified |= BoolEdit("Follow Spawn Source", &d->m_FollowSpawnSource, &o.followSpawnSource);
        edit.modified |= BoolEdit("Repeat Spawning", &d->m_RepeatParticleSpawning, &o.repeatParticleSpawning);
        edit.modified |= BoolEdit("Kill With Emitter", &d->m_KillParticlesWithEmitter, &o.killParticlesWithEmitter);
    }

    if (ImGui::CollapsingHeader("Color Processor"))
    {
        RenderColorProcessor(edit);
    }

    ImGui::Separator();
    if (ImGui::Button("Reset"))
    {
        edit.original.RestoreTo(d);
        edit.modified = false;
    }
}

void VisualEnvironmentEditor::RenderColorProcessor(EmitterEditData& edit)
{
    fb::UpdateColorData* colorProc = edit.colorProcessor;
    if (!colorProc)
    {
        ImGui::TextDisabled("No color processor");
        return;
    }

    const auto& oc = edit.originalColor;
    Vec3Edit("Base Color", &colorProc->m_Color, &oc.color, true);

    if (!colorProc->m_Pre)
        return;

    fb::TypeInfo* preType = colorProc->m_Pre->GetType();
    if (!preType || preType->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
        return;

    fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(preType);
    if (classInfo->m_ClassId != fb::PolynomialColorInterpData::ClassId())
        return;

    fb::PolynomialColorInterpData* poly = static_cast<fb::PolynomialColorInterpData*>(colorProc->m_Pre);
    ImGui::Separator();
    ImGui::Text("Polynomial Interpolation");
    edit.modified |= Vec3Edit("Color 0", &poly->m_Color0, &oc.color0, true);
    edit.modified |= Vec3Edit("Color 1", &poly->m_Color1, &oc.color1, true);
    edit.modified |= Vec4Edit("Coefficients", &poly->m_Coefficients, &oc.coefficients, false);
}

std::pair<size_t, size_t> VisualEnvironmentEditor::CountEmittersInNode(const EmitterTreeNode& node)
{
    size_t total = node.emitters.size();
    size_t active = 0;

    for (fb::EmitterTemplateData* data : node.emitters)
    {
        auto itr = m_EmitterMap.find(data);
        if (itr != m_EmitterMap.end() && itr->second.lastTemplate)
            active++;
    }

    for (const auto& [name, child] : node.children)
    {
        auto [childTotal, childActive] = CountEmittersInNode(child);
        total += childTotal;
        active += childActive;
    }

    return { total, active };
}

bool VisualEnvironmentEditor::NodeHasMatch(const EmitterTreeNode& node, const char* search)
{
    for (fb::EmitterTemplateData* data : node.emitters)
    {
        auto itr = m_EmitterMap.find(data);
        if (itr != m_EmitterMap.end() && containsIgnoreCase(itr->second.name, search))
            return true;
    }
    for (const auto& [name, child] : node.children)
    {
        if (NodeHasMatch(child, search))
            return true;
    }
    return false;
}

void VisualEnvironmentEditor::RenderEmitterTreeNode(EmitterTreeNode& node)
{
    const char* search = m_EmitterSearchBuf;
    bool isSearching = search[0] != '\0';

    for (auto& [name, child] : node.children)
    {
        if (isSearching && !NodeHasMatch(child, search))
            continue;

        auto [total, active] = CountEmittersInNode(child);

        char label[256];
        sprintf_s(label, sizeof(label), active > 0 ? "%s (%u/%u)" : "%s (%u)", name.c_str(), active > 0 ? active : total, total);

        if (active > 0)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));

        bool open = ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen);

        if (active > 0)
            ImGui::PopStyleColor();

        if (open)
        {
            RenderEmitterTreeNode(child);
            ImGui::TreePop();
        }
    }

    for (fb::EmitterTemplateData* data : node.emitters)
    {
        auto itr = m_EmitterMap.find(data);
        if (itr == m_EmitterMap.end())
            continue;

        if (isSearching && !containsIgnoreCase(itr->second.name, search))
            continue;

        RenderEmitterSelectable(data, itr->second);
    }
}

void VisualEnvironmentEditor::RenderEmitterSelectable(fb::EmitterTemplateData* data, EmitterEditData& edit)
{
    bool isActive = edit.lastTemplate != nullptr;

    const char* displayName = edit.name.c_str();
    if (size_t pos = edit.name.find_last_of('/'); pos != std::string::npos)
        displayName = edit.name.c_str() + pos + 1;

    if (isActive)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f });

    if (ImGui::Selectable(displayName, m_SelectedEmitter == data))
        m_SelectedEmitter = data;

    if (isActive)
        ImGui::PopStyleColor();

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(edit.name.c_str());
        if (isActive)
            ImGui::TextColored(ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f }, "Active");
        ImGui::EndTooltip();
    }
}

void VisualEnvironmentEditor::RenderEmitterTab()
{
    if (ImGui::Button("Scan"))
        ScanEmitters();

    ImGui::SameLine();
    ImGui::Text("(%zu)", m_EmitterMap.size());

    ImGui::InputText("Search", m_EmitterSearchBuf, sizeof(m_EmitterSearchBuf));
    ImGui::Separator();

    ImGui::BeginChild("EmitterList", ImVec2{ 350, 0 }, ImGuiChildFlags_ResizeX);
    RenderEmitterTreeNode(m_EmitterTree);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("EmitterProps", ImVec2{ 0, 0 }, true);
    RenderEmitterProperties();
    ImGui::EndChild();
}

void VisualEnvironmentEditor::DrawTransformSphere(const fb::LinearTransform& t, float axisLength)
{
    if (!m_SelectedEffect)
        return;

    ImVec2 origin;
    if (!render::worldToScreen(t.m_trans, origin))
        return;

    fb::Vec3 xWorld = { t.m_trans.m_x + t.m_right.m_x * axisLength,
                        t.m_trans.m_y + t.m_right.m_y * axisLength,
                        t.m_trans.m_z + t.m_right.m_z * axisLength };
    fb::Vec3 yWorld = { t.m_trans.m_x + t.m_up.m_x * axisLength,
                        t.m_trans.m_y + t.m_up.m_y * axisLength,
                        t.m_trans.m_z + t.m_up.m_z * axisLength };
    fb::Vec3 zWorld = { t.m_trans.m_x + t.m_forward.m_x * axisLength,
                        t.m_trans.m_y + t.m_forward.m_y * axisLength,
                        t.m_trans.m_z + t.m_forward.m_z * axisLength };

    if (ImVec2 xEnd; render::worldToScreen(xWorld, xEnd))
    {
        render::line(origin, xEnd, ImColor{ 255, 0, 0, 255 }, 2.0f);
        render::text(xEnd, "X", ImColor{ 255, 0, 0, 255 });
    }

    if (ImVec2 yEnd; render::worldToScreen(yWorld, yEnd))
    {
        render::line(origin, yEnd, ImColor{ 0, 255, 0, 255 }, 2.0f);
        render::text(yEnd, "Y", ImColor{ 0, 255, 0, 255 });
    }

    if (ImVec2 zEnd; render::worldToScreen(zWorld, zEnd))
    {
        render::line(origin, zEnd, ImColor{ 0, 0, 255, 255 }, 2.0f);
        render::text(zEnd, "Z", ImColor{ 0, 0, 255, 255 });
    }

    render::drawSphere(t.m_trans, 0.5f, 32, 12, IM_COL32_WHITE);
}

void VisualEnvironmentEditor::RenderEffectSpawnerTab()
{
    if (ImGui::Button("Scan Effects"))
        ScanEffectAssets();

    ImGui::SameLine();
    ImGui::Text("(%zu)", m_EffectAssets.size());

    static char effectSearch[256] = "";
    ImGui::InputText("Search##Effect", effectSearch, sizeof(effectSearch));

    if (ImGui::BeginCombo("Effect", m_SelectedEffect ? m_SelectedEffect->m_Name : "None"))
    {
        for (const auto& [name, asset] : m_EffectAssets)
        {
            if (effectSearch[0] && !containsIgnoreCase(name, effectSearch))
                continue;

            if (ImGui::Selectable(name.c_str(), m_SelectedEffect == asset))
            {
                m_SelectedEffect = asset;
            }
        }
        ImGui::EndCombo();
    }

    m_SpawnedEffects;

    ImGui::Separator();

    ImGui::Text("Transform");
    ImGui::DragFloat3("Position", &m_SelectedEffectMatrix.m_trans.m_x, 0.1f);
    ImGui::DragFloat3("Right (X)", &m_SelectedEffectMatrix.m_right.m_x, 0.01f);
    ImGui::DragFloat3("Up (Y)", &m_SelectedEffectMatrix.m_up.m_x, 0.01f);
    ImGui::DragFloat3("Forward (Z)", &m_SelectedEffectMatrix.m_forward.m_x, 0.01f);

    if (ImGui::Button("Identity"))
    {
        m_SelectedEffectMatrix.m_right = { 1.0f, 0.0f, 0.0f };
        m_SelectedEffectMatrix.m_up = { 0.0f, 1.0f, 0.0f };
        m_SelectedEffectMatrix.m_forward = { 0.0f, 0.0f, 1.0f };
    }

    ImGui::SameLine();
    if (ImGui::Button("From Camera (Position)"))
    {
        fb::GameRenderer::Singleton()->m_viewParams.view.Update();
        m_SelectedEffectMatrix.m_trans = fb::GameRenderer::Singleton()->m_viewParams.view.m_viewMatrixInverse.m_trans;
    }

    ImGui::Separator();
    ImGui::Checkbox("Show sphere", &m_ShowLocalSphere);
    ImGui::Checkbox("Show spawned effects", &m_ShowSpawnedEffects);
    ImGui::Separator();

    ImGui::BeginDisabled(!m_SelectedEffect);
    if (ImGui::Button("Spawn Effect", ImVec2{ 120, 25 }))
    {
        uint32_t handle = SpawnEffectAtTransform(m_SelectedEffect, m_SelectedEffectMatrix);
        if (handle != 0)
        {
            m_SpawnedEffects.push_back({ handle, m_SelectedEffectMatrix, m_SelectedEffect->m_Name });
        }
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    ImGui::BeginDisabled(m_SpawnedEffects.empty());
    if (ImGui::Button("Stop All", ImVec2{ 120, 25 }))
    {
        for (const auto& effect : m_SpawnedEffects)
            StopEffect(effect.handle);

        m_SpawnedEffects.clear();
    }
    ImGui::EndDisabled();

    if (!m_SpawnedEffects.empty())
    {
        ImGui::Separator();
        ImGui::Text("Spawned Effects (%zu)", m_SpawnedEffects.size());

        int toRemove = -1;
        for (size_t i = 0; i < m_SpawnedEffects.size(); i++)
        {
            const auto& effect = m_SpawnedEffects[i];

            ImGui::PushID(static_cast<int>(i));

            ImGui::Text("[%u] %s", effect.handle, effect.effectName.c_str());
            ImGui::SameLine();

            if (ImGui::SmallButton("Go"))
            {
                m_SelectedEffectMatrix = effect.transform;
            }
            ImGui::SameLine();

            if (ImGui::SmallButton("Stop"))
            {
                StopEffect(effect.handle);
                toRemove = static_cast<int>(i);
            }

            ImGui::PopID();
        }

        if (toRemove >= 0)
            m_SpawnedEffects.erase(m_SpawnedEffects.begin() + toRemove);
    }
}