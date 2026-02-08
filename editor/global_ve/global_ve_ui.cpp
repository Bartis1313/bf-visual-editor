#include "global_ve.h"
#include "../ui/ui_helpers.h"
#include "../comps/comps.h"
#include <imgui.h>

namespace editor::global_ve
{
    static void renderComponentSection(const char* id, const char* displayName, bool* enableFlag, int resetIdx, auto renderFunc, auto* editData, const auto* origData)
    {
        ImGui::PushID(id);
        ImGui::Checkbox("", enableFlag);
        ImGui::SameLine();

        bool open = ImGui::TreeNodeEx(displayName, ImGuiTreeNodeFlags_AllowItemOverlap);

        if (*enableFlag)
        {
            ImGui::SameLine(ImGui::GetWindowWidth() - 60);
            if (ImGui::SmallButton("Reset"))
                resetComponent(resetIdx);
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
    }

    void renderTab()
    {
        auto& data = getData();

        if (!data.captured)
        {
            ImGui::TextColored(ImVec4{ 0.8f, 0.8f, 0.2f, 1.0f }, "Global VE data not captured yet. Spawn in-game.");
            return;
        }

        bool masterEnabled = data.globalOverrideEnabled;
        if (masterEnabled)
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.4f, 0.2f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4{ 0.5f, 0.3f, 0.3f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4{ 1.0f, 0.4f, 0.4f, 1.0f });
        }

        ImGui::Checkbox("Enable Global VE Overrides", &data.globalOverrideEnabled);

        if (masterEnabled)
        {
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::TextColored(ImVec4{ 1.0f, 0.6f, 0.2f, 1.0f }, "(%d active)", data.EnabledOverrideCount());
        }

        ImGui::SameLine(ImGui::GetWindowWidth() - 180);
        if (ImGui::Button("Reset All"))
            resetToOriginal();

        ImGui::SameLine();
        if (ImGui::Button("Disable All"))
        {
            data.outdoorLightOverrideEnabled = false;
            data.enlightenOverrideEnabled = false;
            data.tonemapOverrideEnabled = false;
            data.colorCorrectionOverrideEnabled = false;
            data.skyOverrideEnabled = false;
            data.fogOverrideEnabled = false;
            data.windOverrideEnabled = false;
            data.sunFlareOverrideEnabled = false;
            data.dynamicAOOverrideEnabled = false;
            data.dofOverrideEnabled = false;
            data.vignetteOverrideEnabled = false;
            data.filmGrainOverrideEnabled = false;
            data.lensScopeOverrideEnabled = false;
            data.cameraParamsOverrideEnabled = false;
            data.screenEffectOverrideEnabled = false;
            data.damageEffectOverrideEnabled = false;
            data.planarReflectionOverrideEnabled = false;
            data.dynamicEnvmapOverrideEnabled = false;
            data.characterLightingOverrideEnabled = false;
            data.motionBlurOverrideEnabled = false;
        }

        ImGui::Separator();

        if (!data.globalOverrideEnabled)
        {
            ImGui::TextColored(ImVec4{ 0.7f, 0.7f, 0.3f, 1.0f },
                "Global overrides disabled. Enable checkbox above to apply edits to final output.");
            ImGui::Separator();
        }

        ImGui::BeginChild("GlobalVEComponents", ImVec2{ 0, 0 }, false);

        renderComponentSection("OutdoorLight", "Outdoor Light",
            &data.outdoorLightOverrideEnabled, 0,
            comps::renderOutdoorLightComponent,
            &data.editOutdoorLight, &data.origOutdoorLight);

        renderComponentSection("Enlighten", "Enlighten",
            &data.enlightenOverrideEnabled, 1,
            comps::renderEnlightenComponent,
            &data.editEnlighten, &data.origEnlighten);

        renderComponentSection("Tonemap", "Tonemap",
            &data.tonemapOverrideEnabled, 2,
            comps::renderTonemapComponent,
            &data.editTonemap, &data.origTonemap);

        renderComponentSection("ColorCorrection", "Color Correction",
            &data.colorCorrectionOverrideEnabled, 3,
            comps::renderColorCorrectionComponent,
            &data.editColorCorrection, &data.origColorCorrection);

        renderComponentSection("Sky", "Sky",
            &data.skyOverrideEnabled, 4,
            comps::renderSkyComponent,
            &data.editSky, &data.origSky);

        renderComponentSection("Fog", "Fog",
            &data.fogOverrideEnabled, 5,
            comps::renderFogComponent,
            &data.editFog, &data.origFog);

        renderComponentSection("Wind", "Wind",
            &data.windOverrideEnabled, 6,
            comps::renderWindComponent,
            &data.editWind, &data.origWind);

        renderComponentSection("SunFlare", "Sun Flare",
            &data.sunFlareOverrideEnabled, 7,
            comps::renderSunFlareComponent,
            &data.editSunFlare, &data.origSunFlare);

        renderComponentSection("DynamicAO", "Dynamic AO",
            &data.dynamicAOOverrideEnabled, 8,
            comps::renderDynamicAOComponent,
            &data.editDynamicAO, &data.origDynamicAO);

        renderComponentSection("Dof", "Depth of Field",
            &data.dofOverrideEnabled, 9,
            comps::renderDofComponent,
            &data.editDof, &data.origDof);

        renderComponentSection("Vignette", "Vignette",
            &data.vignetteOverrideEnabled, 10,
            comps::renderVignetteComponent,
            &data.editVignette, &data.origVignette);

        renderComponentSection("FilmGrain", "Film Grain",
            &data.filmGrainOverrideEnabled, 11,
            comps::renderFilmGrainComponent,
            &data.editFilmGrain, &data.origFilmGrain);

        renderComponentSection("LensScope", "Lens Scope",
            &data.lensScopeOverrideEnabled, 12,
            comps::renderLensScopeComponent,
            &data.editLensScope, &data.origLensScope);

        renderComponentSection("CameraParams", "Camera Params",
            &data.cameraParamsOverrideEnabled, 13,
            comps::renderCameraParamsComponent,
            &data.editCameraParams, &data.origCameraParams);

        renderComponentSection("ScreenEffect", "Screen Effect",
            &data.screenEffectOverrideEnabled, 14,
            comps::renderScreenEffectComponent,
            &data.editScreenEffect, &data.origScreenEffect);

        renderComponentSection("DamageEffect", "Damage Effect",
            &data.damageEffectOverrideEnabled, 15,
            comps::renderDamageEffectComponent,
            &data.editDamageEffect, &data.origDamageEffect);

        renderComponentSection("PlanarReflection", "Planar Reflection",
            &data.planarReflectionOverrideEnabled, 16,
            comps::renderPlanarReflectionComponent,
            &data.editPlanarReflection, &data.origPlanarReflection);

        renderComponentSection("DynamicEnvmap", "Dynamic Envmap",
            &data.dynamicEnvmapOverrideEnabled, 17,
            comps::renderDynamicEnvmapComponent,
            &data.editDynamicEnvmap, &data.origDynamicEnvmap);

        renderComponentSection("CharacterLighting", "Character Lighting",
            &data.characterLightingOverrideEnabled, 18,
            comps::renderCharacterLightingComponent,
            &data.editCharacterLighting, &data.origCharacterLighting);

        renderComponentSection("MotionBlur", "Motion Blur",
            &data.motionBlurOverrideEnabled, 19,
            comps::renderMotionBlurComponent,
            &data.editMotionBlur, &data.origMotionBlur);

        ImGui::EndChild();
    }
}