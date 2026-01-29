#include "log.h"
#include <chrono>
#include <ctime>

namespace logger
{
    static std::vector<Entry> g_Entries;
    static std::mutex         g_Mutex;
    static bool               g_ScrollToBottom = false;
    static char               g_FilterText[128] = "";
    static int                g_LevelFilter = 0;

    static const char* GetLevelName(Level level)
    {
        switch (level)
        {
        case Level_Debug:   return "DBG";
        case Level_Info:    return "INF";
        case Level_Warning: return "WRN";
        case Level_Error:   return "ERR";
        default:            return "???";
        }
    }

    static ImVec4 GetLevelColor(Level level)
    {
        switch (level)
        {
        case Level_Debug:   return ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };
        case Level_Info:    return ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
        case Level_Warning: return ImVec4{ 1.0f, 0.8f, 0.2f, 1.0f };
        case Level_Error:   return ImVec4{ 1.0f, 0.3f, 0.3f, 1.0f };
        default:            return ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
        }
    }

    static std::string GetTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        struct tm ti;
        localtime_s(&ti, &time);

        char buf[32];
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d.%03d",
            ti.tm_hour, ti.tm_min, ti.tm_sec, (int)ms.count());
        return buf;
    }

    void vPrint(Level level, const char* fmt, va_list args)
    {
        if (level < MinLevel)
            return;

        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), fmt, args);

        vprintf(fmt, args);
        printf("\n");

        std::lock_guard<std::mutex> lock(g_Mutex);

        if (CollapseDuplicates && !g_Entries.empty())
        {
            auto& last = g_Entries.back();
            if (last.level == level && last.message == buffer)
            {
                last.count++;
                last.timestamp = GetTimestamp();
                return;
            }
        }

        Entry entry;
        entry.level = level;
        entry.message = buffer;
        entry.timestamp = GetTimestamp();
        g_Entries.push_back(std::move(entry));

        if (g_Entries.size() > MaxEntries)
        {
            g_Entries.erase(g_Entries.begin(),
                g_Entries.begin() + (g_Entries.size() - MaxEntries));
        }

        g_ScrollToBottom = true;
    }

    void Print(Level level, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vPrint(level, fmt, args);
        va_end(args);
    }

    void Clear()
    {
        std::lock_guard<std::mutex> lock(g_Mutex);
        g_Entries.clear();
    }

    void Render(const char* title, bool* p_open)
    {
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::End();
            return;
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::Checkbox("Auto-scroll", &AutoScroll);
                ImGui::Checkbox("Timestamps", &ShowTimestamps);
                ImGui::Checkbox("Collapse Duplicates", &CollapseDuplicates);
                ImGui::Separator();
                ImGui::Text("Min Level:");
                ImGui::RadioButton("Debug", (int*)&MinLevel, Level_Debug);
                ImGui::RadioButton("Info", (int*)&MinLevel, Level_Info);
                ImGui::RadioButton("Warning", (int*)&MinLevel, Level_Warning);
                ImGui::RadioButton("Error", (int*)&MinLevel, Level_Error);
                ImGui::EndMenu();
            }
            if (ImGui::SmallButton("Clear"))
                Clear();
            ImGui::EndMenuBar();
        }

        ImGui::SetNextItemWidth(200);
        ImGui::InputTextWithHint("##filter", "Filter...", g_FilterText, sizeof(g_FilterText));
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        const char* levels[] = { "All", "Debug+", "Info+", "Warning+", "Error" };
        ImGui::Combo("##level", &g_LevelFilter, levels, IM_ARRAYSIZE(levels));
        ImGui::SameLine();

        std::lock_guard<std::mutex> lock(g_Mutex);
        ImGui::Text("(%zu)", g_Entries.size());
        ImGui::Separator();

        ImGui::BeginChild("Content", ImVec2{ 0, 0 }, 0, ImGuiWindowFlags_HorizontalScrollbar);

        Level minDisplay = (Level)g_LevelFilter;
        bool hasFilter = g_FilterText[0] != '\0';

        for (const auto& e : g_Entries)
        {
            if (e.level < minDisplay)
                continue;
            if (hasFilter && e.message.find(g_FilterText) == std::string::npos)
                continue;

            ImGui::PushStyleColor(ImGuiCol_Text, GetLevelColor(e.level));

            if (ShowTimestamps)
            {
                ImGui::TextDisabled("[%s]", e.timestamp.c_str());
                ImGui::SameLine();
            }

            if (e.count > 1)
                ImGui::Text("[%s] %s (x%u)", GetLevelName(e.level), e.message.c_str(), e.count);
            else
                ImGui::Text("[%s] %s", GetLevelName(e.level), e.message.c_str());

            ImGui::PopStyleColor();
        }

        if (g_ScrollToBottom && AutoScroll)
        {
            ImGui::SetScrollHereY(1.0f);
            g_ScrollToBottom = false;
        }

        ImGui::EndChild();
        ImGui::End();
    }
}