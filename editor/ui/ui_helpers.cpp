#include "ui_helpers.h"

namespace editor::ui
{
    bool Vec2Edit(const char* label, fb::Vec2* c, const fb::Vec2* o)
    {
        bool mod = memcmp(c, o, sizeof(fb::Vec2)) != 0;
        if (mod && highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, modifiedColor);

        float v[2] = { c->m_x, c->m_y };
        bool changed = ImGui::DragFloat2(label, v, 0.01f);
        if (changed) { c->m_x = v[0]; c->m_y = v[1]; }

        if (mod && highlightModified)
            ImGui::PopStyleColor();

        if (showOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(originalColor, "Original: (%.3f, %.3f)", o->m_x, o->m_y);
            ImGui::EndTooltip();
        }

        if (mod)
        {
            ImGui::SameLine();
            ImGui::PushID(label);
            if (ImGui::SmallButton("R")) { *c = *o; changed = true; }
            ImGui::PopID();
        }

        return changed;
    }

    bool Vec3Edit(const char* label, fb::Vec3* c, const fb::Vec3* o, bool isColor)
    {
        bool mod = memcmp(c, o, sizeof(fb::Vec3)) != 0;
        if (mod && highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, modifiedColor);

        float v[3] = { c->m_x, c->m_y, c->m_z };
        bool changed = isColor
            ? ImGui::ColorEdit3(label, v, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float)
            : ImGui::DragFloat3(label, v, 0.01f);
        if (changed) { c->m_x = v[0]; c->m_y = v[1]; c->m_z = v[2]; }

        if (mod && highlightModified)
            ImGui::PopStyleColor();

        if (showOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(originalColor, "Original: (%.3f, %.3f, %.3f)", o->m_x, o->m_y, o->m_z);
            ImGui::EndTooltip();
        }

        if (mod)
        {
            ImGui::SameLine();
            ImGui::PushID(label);
            if (ImGui::SmallButton("R")) { *c = *o; changed = true; }
            ImGui::PopID();
        }

        return changed;
    }

    bool Vec4Edit(const char* label, fb::Vec4* c, const fb::Vec4* o, bool isColor)
    {
        bool mod = memcmp(c, o, sizeof(fb::Vec4)) != 0;
        if (mod && highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, modifiedColor);

        float v[4] = { c->m_x, c->m_y, c->m_z, c->m_w };
        bool changed = isColor
            ? ImGui::ColorEdit4(label, v, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float)
            : ImGui::DragFloat4(label, v, 0.01f);
        if (changed) { c->m_x = v[0]; c->m_y = v[1]; c->m_z = v[2]; c->m_w = v[3]; }

        if (mod && highlightModified)
            ImGui::PopStyleColor();

        if (showOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(originalColor, "Original: (%.3f, %.3f, %.3f, %.3f)", o->m_x, o->m_y, o->m_z, o->m_w);
            ImGui::EndTooltip();
        }

        if (mod)
        {
            ImGui::SameLine();
            ImGui::PushID(label);
            if (ImGui::SmallButton("R")) { *c = *o; changed = true; }
            ImGui::PopID();
        }

        return changed;
    }

    bool FloatEdit(const char* label, float* c, const float* o, float min, float max, const char* format)
    {
        bool mod = *c != *o;
        if (mod && highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, modifiedColor);

        bool changed = (min != 0.0f || max != 0.0f)
            ? ImGui::SliderFloat(label, c, min, max, format)
            : ImGui::DragFloat(label, c, 0.01f, 0.0f, 0.0f, format);

        if (mod && highlightModified)
            ImGui::PopStyleColor();

        if (showOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(originalColor, "Original: %.3f", *o);
            ImGui::EndTooltip();
        }

        if (mod)
        {
            ImGui::SameLine();
            ImGui::PushID(label);
            if (ImGui::SmallButton("R")) { *c = *o; changed = true; }
            ImGui::PopID();
        }

        return changed;
    }

    bool BoolEdit(const char* label, bool* c, const bool* o)
    {
        bool mod = *c != *o;
        if (mod && highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, modifiedColor);

        bool changed = ImGui::Checkbox(label, c);

        if (mod && highlightModified)
            ImGui::PopStyleColor();

        if (showOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(originalColor, "Original: %s", *o ? "true" : "false");
            ImGui::EndTooltip();
        }

        if (mod)
        {
            ImGui::SameLine();
            ImGui::PushID(label);
            if (ImGui::SmallButton("R")) { *c = *o; changed = true; }
            ImGui::PopID();
        }

        return changed;
    }

    bool IntEdit(const char* label, int* c, const int* o, int min, int max)
    {
        bool mod = *c != *o;
        if (mod && highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, modifiedColor);

        bool changed = (min != 0 || max != 0)
            ? ImGui::SliderInt(label, c, min, max)
            : ImGui::DragInt(label, c);

        if (mod && highlightModified)
            ImGui::PopStyleColor();

        if (showOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(originalColor, "Original: %d", *o);
            ImGui::EndTooltip();
        }

        if (mod)
        {
            ImGui::SameLine();
            ImGui::PushID(label);
            if (ImGui::SmallButton("R")) { *c = *o; changed = true; }
            ImGui::PopID();
        }

        return changed;
    }

    bool UIntEdit(const char* label, uint32_t* c, const uint32_t* o, uint32_t min, uint32_t max)
    {
        bool mod = *c != *o;
        if (mod && highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, modifiedColor);

        bool changed = (min != 0 || max != 0)
            ? ImGui::SliderScalar(label, ImGuiDataType_U32, c, &min, &max)
            : ImGui::DragScalar(label, ImGuiDataType_U32, c);

        if (mod && highlightModified)
            ImGui::PopStyleColor();

        if (showOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(originalColor, "Original: %u", *o);
            ImGui::EndTooltip();
        }

        if (mod)
        {
            ImGui::SameLine();
            ImGui::PushID(label);
            if (ImGui::SmallButton("R")) { *c = *o; changed = true; }
            ImGui::PopID();
        }

        return changed;
    }
}