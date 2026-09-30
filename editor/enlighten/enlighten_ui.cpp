#include "enlighten_internal.h"
#include "../ui/ui_helpers.h"
#include "enlighten_db.h"
#include "enlighten_gen.h"
#include "enlighten_inspect.h"
#include "enlighten_relight.h"
#include "enlighten_look.h"

#include <imgui.h>

namespace editor::enlighten
{
    using namespace detail;

    bool look::renderUI(bool relight)
    {
        look::Settings& s = look::settings;
        bool changed = false;
        changed |= ImGui::Checkbox("keep shipped color", &s.keepShippedColor);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("only the brightness changes; hue and saturation stay the baked atlas's");
        changed |= ImGui::SliderFloat("saturation", &s.saturation, 0.0f, 2.0f, "%.2f");
        changed |= ImGui::ColorEdit3("tint", s.tint, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
        if (relight)
        {
            changed |= ImGui::SliderFloat("sky part", &s.skyStrength, 0.0f, 4.0f, "x%.2f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("light the texel gets from open sky (sky visibility)");
            changed |= ImGui::SliderFloat("sun / bounce part", &s.sunStrength, 0.0f, 4.0f, "x%.2f");
        }
        if (ImGui::SmallButton("reset look")) { s = look::Settings{}; changed = true; }
        if (changed) ++look::generation;
        return changed;
    }

    namespace
    {
        void renderRuntime()
        {
            if (!g_rt.captured)
            {
                ImGui::TextDisabled("runtime settings not captured yet");
                return;
            }
            auto* e = reinterpret_cast<fb::EnlightenRuntimeSettings*>(g_rt.edit);
            const auto* o = reinterpret_cast<const fb::EnlightenRuntimeSettings*>(g_rt.orig);

            ImGui::Checkbox("Override runtime settings", &g_rt.enabled);
            ImGui::SameLine();
            if (ImGui::SmallButton("revert"))
                runtimeRevert();
            ImGui::SameLine();
            if (ImGui::SmallButton("kick solver"))
                runtimeKick(g_rt.kickLength);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Enable off for two updates, then every force-update flag on");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(90.0f);
            ImGui::SliderInt("updates##kick", &g_rt.kickLength, 3, 600);
            if (g_rt.kickFrames > 0)
            {
                ImGui::SameLine();
                ImGui::TextDisabled("kicking %d", g_rt.kickFrames);
            }

#define B(f, l) ui::BoolEdit(l "##rt", &e->f, &o->f);
            if (ImGui::TreeNode("Solver##rt"))
            {
                ENL_RT_BOOLS_SOLVER(B)
                ui::UIntEdit("Job count", &e->m_JobCount, &o->m_JobCount);
                ui::UIntEdit("Min system update count", &e->m_MinSystemUpdateCount, &o->m_MinSystemUpdateCount);
#if defined(BFVE_GAME_BF4)
                ui::FloatEdit("Max per-frame solve time", &e->m_MaxPerFrameSolveTime, &o->m_MaxPerFrameSolveTime);
#endif
                ui::FloatEdit("Temporal coherence threshold", &e->m_TemporalCoherenceThreshold, &o->m_TemporalCoherenceThreshold);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Input lighting##rt"))
            {
                ui::FloatEdit("Sky box scale", &e->m_SkyBoxScale, &o->m_SkyBoxScale);
                ui::Vec3Edit("Albedo default color", &e->m_AlbedoDefaultColor, &o->m_AlbedoDefaultColor, true);
                ENL_RT_BOOLS_INPUT(B)
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Outputs##rt"))
            {
                ENL_RT_BOOLS_OUTPUT(B)
                ui::FloatEdit("Local light force radius", &e->m_LocalLightForceRadius, &o->m_LocalLightForceRadius);
#if defined(BFVE_GAME_BF3)
                ui::UIntEdit("Light probe max update solves", &e->m_LightProbeMaxUpdateSolveCount, &o->m_LightProbeMaxUpdateSolveCount);
#else
                ui::FloatEdit("Cube map force global scale", &e->m_CubeMapForceGlobalScale, &o->m_CubeMapForceGlobalScale);
                ui::UIntEdit("Cube map max update count", &e->m_CubeMapMaxUpdateCount, &o->m_CubeMapMaxUpdateCount);
                ui::UIntEdit("Cube map convolution samples", &e->m_CubeMapConvolutionSampleCount, &o->m_CubeMapConvolutionSampleCount);
                ui::UIntEdit("Light probe max source solves", &e->m_LightProbeMaxSourceSolveCount, &o->m_LightProbeMaxSourceSolveCount);
                ui::UIntEdit("Light probe max instance updates", &e->m_LightProbeMaxInstanceUpdateCount, &o->m_LightProbeMaxInstanceUpdateCount);
                ui::UIntEdit("Light probe lookup grid res", &e->m_LightProbeLookupTableGridRes, &o->m_LightProbeLookupTableGridRes);
#endif
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Debug draw##rt"))
            {
                ImGui::TextDisabled("retail may compile these out");
                ENL_RT_BOOLS_DEBUG(B)
                ui::IntEdit("System dependencies", &e->m_DrawDebugSystemDependenciesEnable, &o->m_DrawDebugSystemDependenciesEnable);
                ui::IntEdit("System bounding boxes", &e->m_DrawDebugSystemBoundingBoxEnable, &o->m_DrawDebugSystemBoundingBoxEnable);
                ui::FloatEdit("Light probe size", &e->m_DrawDebugLightProbeSize, &o->m_DrawDebugLightProbeSize);
                ImGui::TreePop();
            }
#undef B
        }
    }

    void renderTab()
    {
        if (!g_scanned)
        {
            ImGui::TextDisabled("waiting for the level to settle");
            return;
        }
        if (g_staticAsset)
            ImGui::TextDisabled("static data %s (%ux%u)", g_staticName.c_str(), g_orig[0].width, g_orig[0].height);
        else
            ImGui::TextDisabled(g_dynamicEnable ? "dynamic level: the engine solves it from its own systems" : "no Enlighten atlas on this level");

        ImGui::SeparatorText("Runtime");
        renderRuntime();

        ImGui::SeparatorText("Lightmap");
        relight::renderUI();
        inspect::renderOverlayUI();

        ImGui::SeparatorText("Lightmap look");
        if (look::renderUI(relight::enabled) && db::liveDatabase()) runtimeRefresh(180);

        ImGui::SeparatorText("Engine solver");
        db::renderUI();

        ImGui::SeparatorText("Generated systems");
        gen::renderUI();

        ImGui::SeparatorText("Texel inspector");
        inspect::renderUI();
    }
}
