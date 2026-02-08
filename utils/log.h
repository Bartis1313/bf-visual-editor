#pragma once

#include <cstdint>
#include <string>
#include <format>

namespace logger
{
    enum Level : uint8_t
    {
        Level_Debug,
        Level_Info,
        Level_Warning,
        Level_Error
    };

    struct Entry
    {
        Level level;
        std::string message;
        std::string timestamp;
        uint32_t count = 1;
    };

    inline bool autoScroll = true;
    inline bool showTimestamps = true;
    inline bool collapseDuplicates = true;
    inline bool textMode = false;
    inline size_t maxEntries = 1000;
    inline Level minLevel = Level_Info;

    void addEntry(Level level, std::string message);
    void clear();
    void render(const char* title = "Console", bool* open = nullptr);

    template<typename... Args>
    void print(Level level, std::format_string<Args...> fmt, Args&&... args)
    {
        addEntry(level, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void debug(std::format_string<Args...> fmt, Args&&... args)
    {
        print(Level_Debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args)
    {
        print(Level_Info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning(std::format_string<Args...> fmt, Args&&... args)
    {
        print(Level_Warning, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args)
    {
        print(Level_Error, fmt, std::forward<Args>(args)...);
    }
}