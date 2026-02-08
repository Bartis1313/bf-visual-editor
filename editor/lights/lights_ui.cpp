#include "lights.h"
#include "../editor_context.h"
#include "../ui/ui_helpers.h"
#include <imgui.h>
#include <algorithm>

namespace editor::lights
{
    static char filterBuffer[256] = { };
    static int typeFilter = 0;

    void renderTab()
    {
        ImGui::TextColored(ImVec4{ 0.6f, 0.8f, 1.0f, 1.0f }, "Light Data Editing");
        ImGui::Separator();

        ImGui::InputTextWithHint("##LightFilter", "Filter by asset name...", filterBuffer, sizeof(filterBuffer));

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);

        const char* typeFilters[] = { "All", "SpotLight", "PointLight", "LocalLight" };
        ImGui::Combo("##LightType", &typeFilter, typeFilters, IM_ARRAYSIZE(typeFilters));

        ImGui::SameLine();
        if (ImGui::Button("Scan Lights"))
            scanAndApplyOverrides();

        ImGui::SameLine();
        if (ImGui::Button("Reset All"))
            resetAll();

        ImGui::Separator();

        std::vector<std::pair<fb::LocalLightEntityData*, LightDataEntry*>> entries;
        {
            for (auto& [ptr, entry] : getEntries())
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

        ImGui::Text("Light Types: %zu | Active Instances: %zu | Overrides: %zu",
            entries.size(), totalActive, totalWithOverride);
        ImGui::Separator();

        std::string filterStr = filterBuffer;
        std::transform(filterStr.begin(), filterStr.end(), filterStr.begin(), ::tolower);
        std::sort(entries.begin(), entries.end(),
            [](const auto& a, const auto& b) { return a.second->assetName < b.second->assetName; });

        ImGui::BeginChild("LightList", ImVec2(0, 0), true);

        for (auto& [dataPtr, entry] : entries)
        {
            if (typeFilter == 1 && !entry->isSpotLight)
                continue;
            if (typeFilter == 2 && !entry->isPointLight)
                continue;
            if (typeFilter == 3 && (entry->isSpotLight || entry->isPointLight))
                continue;

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
                    ImGui::TextColored(ui::originalColor, "Original Values:");
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
                renderEditor(*entry);
                ImGui::Unindent();
            }

            ImGui::PopID();
        }

        ImGui::EndChild();
    }

    void renderEditor(LightDataEntry& entry)
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

            if (ui::FloatEdit("Radius", &entry.radius, &entry.origRadius))
                changed = true;
            if (ui::FloatEdit("Intensity", &entry.intensity, &entry.origIntensity))
                changed = true;
            if (ImGui::DragFloat("Attenuation Offset", &entry.attenuationOffset, 0.01f))
                changed = true;

            if (ImGui::Checkbox("Visible", &entry.visible)) changed = true;
            ImGui::SameLine();
            if (ImGui::Checkbox("Specular", &entry.specularEnable)) changed = true;
            ImGui::SameLine();
            if (ImGui::Checkbox("Enlighten", &entry.enlightenEnable)) changed = true;

            if (ui::EnumCombo<fb::EnlightenColorMode>("Enlighten Color Mode", &entry.enlightenColorMode))
                changed = true;

            ImGui::TreePop();
        }

        if (entry.isSpotLight && ImGui::TreeNode("SpotLight Properties##Light"))
        {
            const char* shapes[] = { "Cone", "Frustum", "Ortho" };
            if (ImGui::Combo("Shape", &entry.spotShape, shapes, 3)) changed = true;
            if (ImGui::SliderFloat("Cone Inner Angle", &entry.coneInnerAngle, 0.0f, 90.0f)) changed = true;
            if (ImGui::SliderFloat("Cone Outer Angle", &entry.coneOuterAngle, 0.0f, 90.0f)) changed = true;
            if (ImGui::DragFloat("Frustum FOV", &entry.frustumFov, 0.1f)) changed = true;
            if (ImGui::DragFloat("Frustum Aspect", &entry.frustumAspect, 0.01f)) changed = true;
            if (ImGui::DragFloat("Ortho Width", &entry.orthoWidth, 0.1f)) changed = true;
            if (ImGui::DragFloat("Ortho Height", &entry.orthoHeight, 0.1f)) changed = true;
            if (ImGui::Checkbox("Cast Shadows", &entry.castShadowsEnable)) changed = true;
            if (ui::EnumCombo<fb::QualityLevel>("Shadow Min Level", &entry.castShadowsMinLevel)) changed = true;

            ImGui::TreePop();
        }

        if (entry.isPointLight && ImGui::TreeNode("PointLight Properties##Light"))
        {
            if (ImGui::DragFloat("Width", &entry.pointWidth, 0.1f)) changed = true;
            if (ImGui::DragFloat("Translucency Ambient", &entry.translucencyAmbient, 0.01f)) changed = true;
            if (ImGui::DragFloat("Translucency Scale", &entry.translucencyScale, 0.01f)) changed = true;
            if (ImGui::DragScalar("Translucency Power", ImGuiDataType_U32, &entry.translucencyPower)) changed = true;
            if (ImGui::DragFloat("Translucency Distortion", &entry.translucencyDistortion, 0.01f)) changed = true;

            ImGui::TreePop();
        }

        if (changed && entry.hasOverride)
        {
            applyOverride(entry);
        }
    }
}