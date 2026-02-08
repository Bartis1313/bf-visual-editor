#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include "../../SDK/sdk.h"
#include <algorithm>
#include <ranges>

// funcs kept uppercase to match imgui
namespace editor::ui
{
    inline bool showOriginalValues = true;
    inline bool highlightModified = true;
    inline ImVec4 modifiedColor = { 1.0f, 0.8f, 0.2f, 1.0f };
    inline ImVec4 originalColor = { 0.5f, 0.8f, 1.0f, 1.0f };

    bool Vec2Edit(const char* label, fb::Vec2* current, const fb::Vec2* original);
    bool Vec3Edit(const char* label, fb::Vec3* current, const fb::Vec3* original, bool isColor = false);
    bool Vec4Edit(const char* label, fb::Vec4* current, const fb::Vec4* original, bool isColor = false);
    bool FloatEdit(const char* label, float* current, const float* original, float min = 0.f, float max = 0.f, const char* format = "%.3f");
    bool BoolEdit(const char* label, bool* current, const bool* original);
    bool IntEdit(const char* label, int* current, const int* original, int min = 0, int max = 0);
    bool UIntEdit(const char* label, uint32_t* current, const uint32_t* original, uint32_t min = 0, uint32_t max = 0);

    template<typename E>
    bool EnumCombo(const char* label, int* current, const int* original)
    {
        using NamesType = std::decay_t<decltype(magic_enum::enum_names<E>())>;
        const auto names = magic_enum::enum_names<E>();
        constexpr auto count = magic_enum::enum_count<E>();

        bool mod = *current != *original;
        if (mod && highlightModified)
            ImGui::PushStyleColor(ImGuiCol_Text, modifiedColor);

        auto getter = [](void* data, int idx, const char** out_text) -> bool {
            const auto& arr = *static_cast<const NamesType*>(data);
            if (idx < 0 || idx >= static_cast<int>(arr.size())) return false;
            if (out_text) *out_text = arr[idx].data();
            return true;
            };

        bool changed = ImGui::Combo(label, current, getter, (void*)&names, count);

        if (mod && highlightModified)
            ImGui::PopStyleColor();

        if (showOriginalValues && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(originalColor, "Original: %s",
                (*original >= 0 && *original < static_cast<int>(count)) ? names[*original].data() : "Unknown");
            ImGui::EndTooltip();
        }

        if (mod)
        {
            ImGui::SameLine();
            ImGui::PushID(label);
            if (ImGui::SmallButton("R")) { *current = *original; changed = true; }
            ImGui::PopID();
        }

        return changed;
    }

    template<typename E>
    bool EnumCombo(const char* label, int* current)
    {
        using NamesType = std::decay_t<decltype(magic_enum::enum_names<E>())>;
        const auto names = magic_enum::enum_names<E>();
        constexpr auto count = magic_enum::enum_count<E>();

        auto getter = [](void* data, int idx, const char** out_text) -> bool {
            const auto& arr = *static_cast<const NamesType*>(data);
            if (idx < 0 || idx >= static_cast<int>(arr.size())) return false;
            if (out_text) *out_text = arr[idx].data();
            return true;
            };

        return ImGui::Combo(label, current, getter, (void*)&names, count);
    }

    inline bool containsIgnoreCase(const std::string& haystack, const std::string& needle)
    {
        auto itr = std::ranges::search(haystack, needle,
            [](char a, char b)
            {
                return std::tolower(static_cast<unsigned char>(a)) ==
                    std::tolower(static_cast<unsigned char>(b));
            });

        return itr.begin() != haystack.end();
    }
}