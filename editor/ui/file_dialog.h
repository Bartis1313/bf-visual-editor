#pragma once

#include <string>
#include <vector>

namespace editor::ui::filedlg
{
    enum class Mode
    {
        Open, // must exist
        Save // warns before overwriting
    };

    // lowercase extensions without the dot, empty = all
    void open(const char* title, Mode mode, const std::string& startDir,
              const std::string& suggestedName, std::vector<std::string> extensions);

    // true on the frame a file is confirmed
    bool draw(std::string& outPath);

    bool isOpen();

    // shutdown and device loss
    void releasePreviews();
}
