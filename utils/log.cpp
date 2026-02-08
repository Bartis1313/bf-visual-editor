#include "log.h"
#include <imgui.h>
#include <chrono>
#include <mutex>
#include <vector>
#include <magic_enum/magic_enum.hpp>

namespace logger
{
    std::vector<Entry> entries;
    std::mutex mutex;
    bool scrollToBottom = false;
    char filterText[128] = "";
    int levelFilter = 0;
    std::string textBuffer;

    static const char* getLevelName(Level level)
    {
        return magic_enum::enum_name(level).data();
    }

    static ImVec4 getLevelColor(Level level)
    {
        switch (level)
        {
        case Level_Debug:   return { 0.6f, 0.6f, 0.6f, 1.0f };
        case Level_Info:    return { 1.0f, 1.0f, 1.0f, 1.0f };
        case Level_Warning: return { 1.0f, 0.8f, 0.2f, 1.0f };
        case Level_Error:   return { 1.0f, 0.3f, 0.3f, 1.0f };
        default:            return { 1.0f, 1.0f, 1.0f, 1.0f };
        }
    }

    static std::string getTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::tm ti{ };
        localtime_s(&ti, &time);
        return std::format("{:02d}:{:02d}:{:02d}.{:03d}",
            ti.tm_hour, ti.tm_min, ti.tm_sec, static_cast<int>(ms.count()));
    }

    static std::string formatEntry(const Entry& e, bool includeTimestamp)
    {
        std::string result;
        if (includeTimestamp)
            result = std::format("[{}] ", e.timestamp);

        if (e.count > 1)
            result += std::format("[{}] {} (x{})", getLevelName(e.level), e.message, e.count);
        else
            result += std::format("[{}] {}", getLevelName(e.level), e.message);

        return result;
    }

    static void copyToClipboard(const std::string& text)
    {
        ImGui::SetClipboardText(text.c_str());
    }

    static std::string buildFilteredText(Level minDisplay, bool hasFilter)
    {
        std::string result{ };
        result.reserve(entries.size() * 80);

        for (const auto& e : entries)
        {
            if (e.level < minDisplay)
                continue;
            if (hasFilter && e.message.find(filterText) == std::string::npos)
                continue;

            result += formatEntry(e, showTimestamps);
            result += '\n';
        }
        return result;
    }

    void addEntry(Level level, std::string message)
    {
        if (level < minLevel)
            return;

        std::puts(message.c_str());

        std::lock_guard lock(mutex);

        if (collapseDuplicates && !entries.empty())
        {
            auto& last = entries.back();
            if (last.level == level && last.message == message)
            {
                last.count++;
                last.timestamp = getTimestamp();
                return;
            }
        }

        entries.push_back({
            .level = level,
            .message = std::move(message),
            .timestamp = getTimestamp()
            });

        if (entries.size() > maxEntries)
        {
            entries.erase(entries.begin(), entries.begin() + static_cast<ptrdiff_t>(entries.size() - maxEntries));
        }

        scrollToBottom = true;
    }

    void clear()
    {
        std::lock_guard lock(mutex);
        entries.clear();
        textBuffer.clear();
    }

    void render(const char* title, bool* open)
    {
        if (!ImGui::Begin(title, open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::End();
            return;
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options##log"))
            {
                ImGui::Checkbox("Auto-scroll", &autoScroll);
                ImGui::Checkbox("Timestamps", &showTimestamps);
                ImGui::Checkbox("Collapse Duplicates", &collapseDuplicates);
                ImGui::Checkbox("Text Mode (selectable)", &textMode);
                ImGui::Separator();
                ImGui::TextUnformatted("Min Level:");
                ImGui::RadioButton("Debug", reinterpret_cast<int*>(&minLevel), Level_Debug);
                ImGui::RadioButton("Info", reinterpret_cast<int*>(&minLevel), Level_Info);
                ImGui::RadioButton("Warning", reinterpret_cast<int*>(&minLevel), Level_Warning);
                ImGui::RadioButton("Error", reinterpret_cast<int*>(&minLevel), Level_Error);
                ImGui::EndMenu();
            }
            if (ImGui::SmallButton("Clear"))
                clear();
            if (ImGui::SmallButton("Copy All"))
            {
                std::lock_guard lock(mutex);
                copyToClipboard(buildFilteredText(static_cast<Level>(levelFilter), filterText[0] != '\0'));
            }
            ImGui::EndMenuBar();
        }

        ImGui::SetNextItemWidth(200);
        ImGui::InputTextWithHint("##filter", "Filter...", filterText, sizeof(filterText));
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        constexpr auto levelNames = magic_enum::enum_names<Level>();
        if (ImGui::BeginCombo("##level", magic_enum::enum_name(static_cast<Level>(levelFilter)).data()))
        {
            for (const auto& [value, name] : magic_enum::enum_entries<Level>())
            {
                const bool selected = (levelFilter == static_cast<int>(value));
                if (ImGui::Selectable(name.data(), selected))
                    levelFilter = static_cast<int>(value);
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
       
        ImGui::SameLine();

        std::lock_guard lock(mutex);
        ImGui::Text("(%zu)", entries.size());
        ImGui::Separator();

        const bool hasFilter = filterText[0] != '\0';
        const Level minDisplay = static_cast<Level>(levelFilter);

        if (textMode)
        {
            textBuffer = buildFilteredText(minDisplay, hasFilter);

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));

            ImVec2 size = ImGui::GetContentRegionAvail();
            ImGui::InputTextMultiline(
                "##logtext",
                textBuffer.data(),
                textBuffer.size() + 1,
                size,
                ImGuiInputTextFlags_ReadOnly
            );

            ImGui::PopStyleColor();
        }
        else
        {
            ImGui::BeginChild("Content##logger", { 0, 0 }, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);

            for (int i = 0; const auto& e : entries)
            {
                if (e.level < minDisplay)
                    continue;
                if (hasFilter && e.message.find(filterText) == std::string::npos)
                    continue;

                ImGui::PushID(i++);
                ImGui::PushStyleColor(ImGuiCol_Text, getLevelColor(e.level));

                if (showTimestamps)
                {
                    ImGui::TextDisabled("[%s]", e.timestamp.c_str());
                    ImGui::SameLine();
                }

                const std::string lineText = (e.count > 1)
                    ? std::format("[{}] {} (x{})", getLevelName(e.level), e.message, e.count)
                    : std::format("[{}] {}", getLevelName(e.level), e.message);

                ImGui::TextUnformatted(lineText.c_str());

                if (ImGui::BeginPopupContextItem("ctx"))
                {
                    if (ImGui::MenuItem("Copy"))
                        copyToClipboard(formatEntry(e, showTimestamps));
                    if (ImGui::MenuItem("Copy Message Only"))
                        copyToClipboard(e.message);
                    ImGui::EndPopup();
                }

                ImGui::PopStyleColor();
                ImGui::PopID();
            }

            if (scrollToBottom && autoScroll)
            {
                ImGui::SetScrollHereY(1.0f);
                scrollToBottom = false;
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }
}