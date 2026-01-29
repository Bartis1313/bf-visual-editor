#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <cstdarg>
#include <imgui.h>

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
        Level       level;
        std::string message;
        std::string timestamp;
        uint32_t    count = 1;
    };

    inline bool     AutoScroll = true;
    inline bool     ShowTimestamps = true;
    inline bool     CollapseDuplicates = true;
    inline size_t   MaxEntries = 1000;
    inline Level    MinLevel = Level_Info;

    void vPrint(Level level, const char* fmt, va_list args);
    void Print(Level level, const char* fmt, ...);
    void Clear();
    void Render(const char* title = "Console", bool* p_open = nullptr);

    inline void Debug(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vPrint(Level_Debug, fmt, args);
        va_end(args);
    }

    inline void Info(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vPrint(Level_Info, fmt, args);
        va_end(args);
    }

    inline void Warning(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vPrint(Level_Warning, fmt, args);
        va_end(args);
    }

    inline void Error(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vPrint(Level_Error, fmt, args);
        va_end(args);
    }
}

#define LOG_DEBUG(fmt, ...)   logger::Print(logger::Level_Debug, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)    logger::Print(logger::Level_Info, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) logger::Print(logger::Level_Warning, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)   logger::Print(logger::Level_Error, fmt, ##__VA_ARGS__)