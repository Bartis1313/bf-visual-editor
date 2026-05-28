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

    bool HdrColor3Edit(const char* label, fb::Vec3* c, const fb::Vec3* o)
    {
        bool mod = memcmp(c, o, sizeof(fb::Vec3)) != 0;
        if (mod && highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, modifiedColor);

        float I = (std::max)({ c->m_x, c->m_y, c->m_z });
        if (I < 1e-4f) I = 1e-4f;
        float n[3] = { c->m_x / I, c->m_y / I, c->m_z / I };

        ImGui::PushID(label);

        bool colorChanged = ImGui::ColorEdit3("##tint", n, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(90.0f);
        float drag = (std::max)(I * 0.05f, 0.01f);
        bool intensityChanged = ImGui::DragFloat("##intensity", &I, drag,
            0.0f, 0.0f, "x%.2f");
        if (I < 0.0f) I = 0.0f;

        ImGui::SameLine();
        ImGui::TextUnformatted(label);

        bool changed = colorChanged || intensityChanged;
        if (changed)
        {
            c->m_x = n[0] * I;
            c->m_y = n[1] * I;
            c->m_z = n[2] * I;
        }

        ImGui::PopID();

        if (mod && highlightModified)
            ImGui::PopStyleColor();

        if (showOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            float oI = (std::max)({ o->m_x, o->m_y, o->m_z });
            ImGui::TextColored(originalColor, "Original: (%.3f, %.3f, %.3f)  intensity %.3f", o->m_x, o->m_y, o->m_z, oI);
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

    bool CurveVec4Edit(const char* label, fb::Vec4* c, const fb::Vec4* o)
    {
        constexpr int N = 48;
        float samples[N];
        for (int i = 0; i < N; ++i)
        {
            float x = (float)i / (float)(N - 1);
            float x2 = x * x;
            float y = c->m_x + c->m_y * x + c->m_z * x2 + c->m_w * x2 * x;
            samples[i] = y;
        }
        float vmin = samples[0], vmax = samples[0];
        for (int i = 1; i < N; ++i)
        {
            vmin = (std::min)(vmin, samples[i]);
            vmax = (std::max)(vmax, samples[i]);
        }
        if (vmax - vmin < 1e-4f) vmax = vmin + 1.0f;

        char overlay[64];
        std::snprintf(overlay, sizeof(overlay), "y(0)=%.3f  y(1)=%.3f", samples[0], samples[N - 1]);

        char plotId[128];
        std::snprintf(plotId, sizeof(plotId), "##curve_%s", label);
        ImGui::PlotLines(plotId, samples, N, 0, overlay, vmin, vmax, ImVec2(0.0f, 40.0f));

        return Vec4Edit(label, c, o, false);
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