#pragma once

#include <string>
#include <vector>

namespace editor::ui::filedlg
{
    enum class Mode
    {
        Open, // must exist
        Save // may not; warns before overwriting
    };

    // `extensions` are lowercase and without the dot ("png", "dds"). Empty shows everything.
    void open(const char* title, Mode mode, const std::string& startDir,
              const std::string& suggestedName, std::vector<std::string> extensions);

    // Call every frame. Returns true on the single frame a file was confirmed, with the
    // full path in `outPath`.
    bool draw(std::string& outPath);

    bool isOpen();

    // Drops every cached preview. Called on shutdown, and on device loss.
    void releasePreviews();
}
