#include "states.h"
#include "../editor_context.h"
#include "../ui/ui_helpers.h"
#include "../comps/comps.h"
#include <imgui.h>

namespace editor::states
{
    void renderTab()
    {
        auto& stateOrder = getStateOrder();

        if (!hasCapturedOriginals)
        {
            ImGui::TextWrapped("No data captured yet. Spawn in-game to begin capturing.");
            return;
        }

        if (stateOrder.empty())
        {
            ImGui::TextWrapped("No visual environment states found.");
            return;
        }

        ImGui::BeginChild("LeftPanel", ImVec2{ 0, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
        ImGui::SameLine();
        if (ImGui::Button("Scan Names"))
            scanVEDataNames();

        renderStateSelector();
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("RightPanel", ImVec2{ 0, 0 }, ImGuiChildFlags_Border);
        int selectedIdx = getSelectedIndex();
        if (selectedIdx >= 0 && selectedIdx < static_cast<int>(stateOrder.size()))
        {
            auto state = stateOrder[selectedIdx];
            StateEditData* data = getStateData(state);
            if (data)
                renderStateEditor(state, *data);
        }
        ImGui::EndChild();
    }

    void renderStatesMenu()
    {
        auto& stateOrder = getStateOrder();

        if (ImGui::MenuItem("Enable All"))
        {
            for (auto* state : stateOrder)
            {
                if (StateEditData* data = getStateData(state))
                    data->overrideEnabled = true;
            }
        }

        if (ImGui::MenuItem("Disable All"))
        {
            for (auto* state : stateOrder)
            {
                if (StateEditData* data = getStateData(state))
                    data->overrideEnabled = false;
            }
        }

        ImGui::Separator();

        for (int i = 0; i < static_cast<int>(stateOrder.size()); i++)
        {
            auto* state = stateOrder[i];
            StateEditData* data = getStateData(state);
            if (data)
            {
                char label[128];
                sprintf_s(label, "[%d] %s##StateMenu%d", data->stateIndex, data->stateName.c_str(), i);
                ImGui::Checkbox(label, &data->overrideEnabled);
            }
        }
    }

    void renderStateSelector()
    {
        auto& stateOrder = getStateOrder();
        int selectedIdx = getSelectedIndex();

        for (int i = 0; i < static_cast<int>(stateOrder.size()); i++)
        {
            auto* state = stateOrder[i];
            if (!state) continue;

            StateEditData* data = getStateData(state);
            if (!data) continue;

            StateHash hash = computeHash(state);
            int compCount = __popcnt(hash.componentMask);

            char label[256];
            sprintf_s(label, "%d: P%d [%d comps]%s%s%s",
                i,
                state->priority,
                compCount,
                data->stateName.empty() ? "" : " - ",
                data->stateName.c_str(),
                data->overrideEnabled ? " *" : "");

            bool isSelected = (selectedIdx == i);

            if (state->excluded)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f });
            else if (data->overrideEnabled)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.3f, 1.0f, 0.3f, 1.0f });
            else
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });

            if (ImGui::Selectable(label, isSelected))
                setSelectedIndex(i);

            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Hash: P%d V%.2f M%08X", hash.priority, hash.visibility, hash.componentMask);
                ImGui::Separator();
                ImGui::Text("Priority: %d", state->priority);
                ImGui::Text("State ID: %d", state->stateId);
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

                if (data->overrideEnabled)
                {
                    ImGui::Separator();
                    ImGui::TextColored(ImVec4{ 0.3f, 1.0f, 0.3f, 1.0f }, "Override Active");
                    if (findEditEntry(hash))
                        ImGui::TextColored(ImVec4{ 0.5f, 0.8f, 1.0f, 1.0f }, "In Edit List");
                }

                ImGui::EndTooltip();
            }
        }
    }

    void renderStateEditor(fb::VisualEnvironmentState* state, StateEditData& data)
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
            ImGui::TextColored(ImVec4{ 1.0f, 0.8f, 0.2f, 1.0f },
                "! %d new component(s) - capturing automatically", liveCount - capturedCount);
        }

        bool prevOverride = data.overrideEnabled;
        ImGui::Checkbox("Override Enabled", &data.overrideEnabled);
        ImGui::SameLine();
        if (ImGui::Button("Reset to Original"))
            resetToOriginal(data);
        ImGui::SameLine();
        if (ImGui::Button("Force Re-capture"))
            data.ResetCaptures();

        ImGui::Separator();

        if (!data.captured || capturedCount == 0)
        {
            ImGui::TextColored(ImVec4{ 0.8f, 0.4f, 0.4f, 1.0f },
                "No components captured yet. Waiting for components to load...");
            return;
        }

        renderComponentEditors(data);

        if (data.overrideEnabled)
        {
            addOrUpdateEditEntry(state, data);
        }
        else if (prevOverride && !data.overrideEnabled)
        {
            removeEditEntry(computeHash(state));
        }
    }

    template<typename T, typename RenderFunc>
    static void renderStateComponent(const char* label, bool hasFlag, T& editMember, const T& origMember, RenderFunc renderFunc)
    {
        if (!hasFlag)
            return;

        ImGui::PushID(label);

        bool modified = std::memcmp(&editMember, &origMember, sizeof(T)) != 0;

        if (modified && ui::highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, ui::modifiedColor);

        bool open = ImGui::CollapsingHeader(label);

        if (modified && ui::highlightModified)
            ImGui::PopStyleColor();

        if (open)
        {
            ImGui::Indent();
            renderFunc(&editMember, &origMember);
            ImGui::Unindent();
        }

        ImGui::PopID();
    }

    void renderComponentEditors(StateEditData& data)
    {
        ImGui::BeginChild("ComponentList");

        renderStateComponent("Outdoor Light", data.hasOutdoorLight, data.editOutdoorLight, data.origOutdoorLight, comps::renderOutdoorLightComponent);
        renderStateComponent("Enlighten", data.hasEnlighten, data.editEnlighten, data.origEnlighten, comps::renderEnlightenComponent);
        renderStateComponent("Tonemap", data.hasTonemap, data.editTonemap, data.origTonemap, comps::renderTonemapComponent);
        renderStateComponent("Color Correction", data.hasColorCorrection, data.editColorCorrection, data.origColorCorrection, comps::renderColorCorrectionComponent);
        renderStateComponent("Sky", data.hasSky, data.editSky, data.origSky, comps::renderSkyComponent);
        renderStateComponent("Fog", data.hasFog, data.editFog, data.origFog, comps::renderFogComponent);
        renderStateComponent("Wind", data.hasWind, data.editWind, data.origWind, comps::renderWindComponent);
        renderStateComponent("Sun Flare", data.hasSunFlare, data.editSunFlare, data.origSunFlare, comps::renderSunFlareComponent);
        renderStateComponent("Dynamic AO", data.hasDynamicAO, data.editDynamicAO, data.origDynamicAO, comps::renderDynamicAOComponent);
        renderStateComponent("Depth of Field", data.hasDof, data.editDof, data.origDof, comps::renderDofComponent);
        renderStateComponent("Vignette", data.hasVignette, data.editVignette, data.origVignette, comps::renderVignetteComponent);
        renderStateComponent("Film Grain", data.hasFilmGrain, data.editFilmGrain, data.origFilmGrain, comps::renderFilmGrainComponent);
        renderStateComponent("Lens Scope", data.hasLensScope, data.editLensScope, data.origLensScope, comps::renderLensScopeComponent);
        renderStateComponent("Camera Params", data.hasCameraParams, data.editCameraParams, data.origCameraParams, comps::renderCameraParamsComponent);
        renderStateComponent("Screen Effect", data.hasScreenEffect, data.editScreenEffect, data.origScreenEffect, comps::renderScreenEffectComponent);
        renderStateComponent("Damage Effect", data.hasDamageEffect, data.editDamageEffect, data.origDamageEffect, comps::renderDamageEffectComponent);
        renderStateComponent("Planar Reflection", data.hasPlanarReflection, data.editPlanarReflection, data.origPlanarReflection, comps::renderPlanarReflectionComponent);
        renderStateComponent("Dynamic Envmap", data.hasDynamicEnvmap, data.editDynamicEnvmap, data.origDynamicEnvmap, comps::renderDynamicEnvmapComponent);
        renderStateComponent("Character Lighting", data.hasCharacterLighting, data.editCharacterLighting, data.origCharacterLighting, comps::renderCharacterLightingComponent);
        renderStateComponent("Motion Blur", data.hasMotionBlur, data.editMotionBlur, data.origMotionBlur, comps::renderMotionBlurComponent);

        ImGui::EndChild();
    }
}