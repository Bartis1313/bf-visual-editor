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
#define DISABLE(Type, field) data.field##OverrideEnabled = false;
            VE_COMPONENTS(DISABLE)
#undef DISABLE
        }

        ImGui::Separator();

        if (!data.globalOverrideEnabled)
        {
            ImGui::TextColored(ImVec4{ 0.7f, 0.7f, 0.3f, 1.0f }, "Global overrides disabled. Enable checkbox above to apply edits to final output.");
            ImGui::Separator();
        }

        ImGui::BeginChild("GlobalVEComponents", ImVec2{ 0, 0 }, false);

        int idx = 0;
#define RENDER(Type, field) \
        renderComponentSection(#Type, PRETTY_CASE_NAME(#Type), \
            &data.field##OverrideEnabled, idx++, \
            comps::render##Type##Component, \
            &data.edit##Type, &data.orig##Type);
        VE_COMPONENTS(RENDER)
#undef RENDER

        ImGui::EndChild();
    }
}