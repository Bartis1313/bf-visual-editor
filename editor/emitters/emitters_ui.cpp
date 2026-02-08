#include "emitters.h"
#include "../ui/ui_helpers.h"
#include <imgui.h>
#include <algorithm>
#include <cctype>

namespace editor::emitters
{
    static char searchBuf[64] = {};

    static std::pair<size_t, size_t> countEmittersInNode(const EmitterTreeNode& node)
    {
        auto& emitterMap = getMap();
        size_t total = node.emitters.size();
        size_t active = 0;

        for (fb::EmitterTemplateData* data : node.emitters)
        {
            auto it = emitterMap.find(data);
            if (it != emitterMap.end() && it->second.lastTemplate)
                active++;
        }

        for (const auto& [name, child] : node.children)
        {
            auto [childTotal, childActive] = countEmittersInNode(child);
            total += childTotal;
            active += childActive;
        }

        return { total, active };
    }

    static bool nodeHasMatch(const EmitterTreeNode& node, const char* search)
    {
        auto& emitterMap = getMap();

        for (fb::EmitterTemplateData* data : node.emitters)
        {
            auto it = emitterMap.find(data);
            if (it != emitterMap.end() && ui::containsIgnoreCase(it->second.name, search))
                return true;
        }

        for (const auto& [name, child] : node.children)
        {
            if (nodeHasMatch(child, search))
                return true;
        }

        return false;
    }

    static void renderEmitterSelectable(fb::EmitterTemplateData* data, EmitterEditData& edit)
    {
        bool isActive = edit.lastTemplate != nullptr;

        const char* displayName = edit.name.c_str();
        if (size_t pos = edit.name.find_last_of('/'); pos != std::string::npos)
            displayName = edit.name.c_str() + pos + 1;

        if (isActive)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f });

        if (ImGui::Selectable(displayName, getSelected() == data))
            setSelected(data);

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

    static void renderEmitterTreeNode(EmitterTreeNode& node)
    {
        auto& emitterMap = getMap();
        const char* search = searchBuf;
        bool isSearching = search[0] != '\0';

        for (auto& [name, child] : node.children)
        {
            if (isSearching && !nodeHasMatch(child, search))
                continue;

            auto [total, active] = countEmittersInNode(child);

            char label[256];
            sprintf_s(label, sizeof(label), active > 0 ? "%s (%zu/%zu)" : "%s (%zu)",
                name.c_str(), active > 0 ? active : total, total);

            if (active > 0)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));

            bool open = ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen);

            if (active > 0)
                ImGui::PopStyleColor();

            if (open)
            {
                renderEmitterTreeNode(child);
                ImGui::TreePop();
            }
        }

        for (fb::EmitterTemplateData* data : node.emitters)
        {
            auto it = emitterMap.find(data);
            if (it == emitterMap.end())
                continue;

            if (isSearching && !ui::containsIgnoreCase(it->second.name, search))
                continue;

            renderEmitterSelectable(data, it->second);
        }
    }

    static void renderColorProcessor(EmitterEditData& edit)
    {
        fb::UpdateColorData* colorProc = edit.colorProcessor;
        if (!colorProc)
        {
            ImGui::TextDisabled("No color processor");
            return;
        }

        const auto& oc = edit.originalColor;
        ui::Vec3Edit("Base Color", &colorProc->m_Color, &oc.color, true);

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
        edit.modified |= ui::Vec3Edit("Color 0", &poly->m_Color0, &oc.color0, true);
        edit.modified |= ui::Vec3Edit("Color 1", &poly->m_Color1, &oc.color1, true);
        edit.modified |= ui::Vec4Edit("Coefficients", &poly->m_Coefficients, &oc.coefficients, false);
    }

    static void renderEmitterProperties()
    {
        fb::EmitterTemplateData* selected = getSelected();
        if (!selected)
        {
            ImGui::TextDisabled("Select an emitter");
            return;
        }

        auto& emitterMap = getMap();
        auto it = emitterMap.find(selected);
        if (it == emitterMap.end())
            return;

        EmitterEditData& edit = it->second;
        fb::EmitterTemplateData* d = edit.data;

        if (!edit.captured)
        {
            edit.original.captureFrom(d);
            edit.captured = true;
        }

        const auto& o = edit.original;

        ImGui::Text("%s", edit.name.c_str());
        if (edit.lastTemplate)
            ImGui::TextColored(ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f }, "Active");
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Basic", ImGuiTreeNodeFlags_DefaultOpen))
        {
            edit.modified |= ui::FloatEdit("Time Scale", &d->m_TimeScale, &o.timeScale);
            edit.modified |= ui::FloatEdit("Lifetime", &d->m_Lifetime, &o.lifetime);
            edit.modified |= ui::UIntEdit("Max Count", &d->m_MaxCount, &o.maxCount);
            edit.modified |= ui::UIntEdit("Lifetime Frames", &d->m_LifetimeFrameCount, &o.lifetimeFrameCount);
        }

        if (ImGui::CollapsingHeader("Distance & Culling"))
        {
            edit.modified |= ui::FloatEdit("Max Spawn Distance", &d->m_MaxSpawnDistance, &o.maxSpawnDistance);
            edit.modified |= ui::FloatEdit("Visible After Dist", &d->m_VisibleAfterDistance, &o.visibleAfterDistance);
            edit.modified |= ui::FloatEdit("Culling Factor", &d->m_ParticleCullingFactor, &o.particleCullingFactor);
            edit.modified |= ui::FloatEdit("Mesh Culling Dist", &d->m_MeshCullingDistance, &o.meshCullingDistance);
            edit.modified |= ui::FloatEdit("Min Screen Area", &d->m_MinScreenArea, &o.minScreenArea);
            edit.modified |= ui::FloatEdit("Dist Scale Near", &d->m_DistanceScaleNearValue, &o.distanceScaleNearValue);
            edit.modified |= ui::FloatEdit("Dist Scale Far", &d->m_DistanceScaleFarValue, &o.distanceScaleFarValue);
            edit.modified |= ui::FloatEdit("Dist Scale Length", &d->m_DistanceScaleLength, &o.distanceScaleLength);
            edit.modified |= ui::BoolEdit("Exclusion Volume Cull", &d->m_ExclusionVolumeCullEnable, &o.exclusionVolumeCullEnable);
        }

        if (ImGui::CollapsingHeader("Lighting"))
        {
            edit.modified |= ui::FloatEdit("Light Multiplier", &d->m_LightMultiplier, &o.lightMultiplier);
            edit.modified |= ui::FloatEdit("Light Wrap Around", &d->m_LightWrapAroundFactor, &o.lightWrapAroundFactor);
            edit.modified |= ui::FloatEdit("Vertex/Pixel Blend", &d->m_VertexPixelLightingBlendFactor, &o.vertexPixelLightingBlendFactor);
            edit.modified |= ui::FloatEdit("Global/Local Blend", &d->m_GlobalLocalNormalBlendFactor, &o.globalLocalNormalBlendFactor);
            edit.modified |= ui::FloatEdit("Soft Particles Fade", &d->m_SoftParticlesFadeDistanceMultiplier, &o.softParticlesFadeDistanceMultiplier);
            edit.modified |= ui::BoolEdit("Emissive", &d->m_Emissive, &o.emissive);
        }

        if (ImGui::CollapsingHeader("Point Light"))
        {
            edit.modified |= ui::BoolEdit("Act As Point Light", &d->m_ActAsPointLight, &o.actAsPointLight);
            edit.modified |= ui::Vec3Edit("Color", &d->m_PointLightColor, &o.pointLightColor, true);
            edit.modified |= ui::Vec4Edit("Intensity", &d->m_PointLightIntensity, &o.pointLightIntensity, false);
            edit.modified |= ui::Vec3Edit("Pivot", &d->m_PointLightPivot, &o.pointLightPivot, false);
            edit.modified |= ui::FloatEdit("Radius", &d->m_PointLightRadius, &o.pointLightRadius);
            edit.modified |= ui::FloatEdit("Min Clamp", &d->m_PointLightMinClamp, &o.pointLightMinClamp);
            edit.modified |= ui::FloatEdit("Max Clamp", &d->m_PointLightMaxClamp, &o.pointLightMaxClamp);
            edit.modified |= ui::FloatEdit("Random Min", &d->m_PointLightRandomIntensityMin, &o.pointLightRandomIntensityMin);
            edit.modified |= ui::FloatEdit("Random Max", &d->m_PointLightRandomIntensityMax, &o.pointLightRandomIntensityMax);
        }

        if (ImGui::CollapsingHeader("Rendering"))
        {
            edit.modified |= ui::BoolEdit("Opaque", &d->m_Opaque, &o.opaque);
            edit.modified |= ui::BoolEdit("Force Full Res", &d->m_ForceFullRes, &o.forceFullRes);
            edit.modified |= ui::BoolEdit("Sun Shadow", &d->m_TransparencySunShadowEnable, &o.transparencySunShadowEnable);
            edit.modified |= ui::BoolEdit("Force Nice Sorting", &d->m_ForceNiceSorting, &o.forceNiceSorting);
        }

        if (ImGui::CollapsingHeader("Behavior"))
        {
            edit.modified |= ui::BoolEdit("Local Space", &d->m_LocalSpace, &o.localSpace);
            edit.modified |= ui::BoolEdit("Follow Spawn Source", &d->m_FollowSpawnSource, &o.followSpawnSource);
            edit.modified |= ui::BoolEdit("Repeat Spawning", &d->m_RepeatParticleSpawning, &o.repeatParticleSpawning);
            edit.modified |= ui::BoolEdit("Kill With Emitter", &d->m_KillParticlesWithEmitter, &o.killParticlesWithEmitter);
        }

        if (ImGui::CollapsingHeader("Color Processor"))
        {
            renderColorProcessor(edit);
        }

        ImGui::Separator();
        if (ImGui::Button("Reset"))
        {
            edit.original.restoreTo(d);
            edit.modified = false;
        }
    }

    void renderTab()
    {
        if (ImGui::Button("Scan"))
            scan();

        ImGui::SameLine();
        ImGui::Text("(%zu)", getMap().size());

        ImGui::InputText("Search", searchBuf, sizeof(searchBuf));
        ImGui::Separator();

        ImGui::BeginChild("EmitterList", ImVec2{ 350, 0 }, ImGuiChildFlags_ResizeX);
        renderEmitterTreeNode(getTree());
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("EmitterProps", ImVec2{ 0, 0 }, true);
        renderEmitterProperties();
        ImGui::EndChild();
    }
}