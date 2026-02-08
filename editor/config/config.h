#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace editor::config
{
    void init();
    void shutdown();

    bool save(const std::string& path);
    bool load(const std::string& path);
    bool saveForCurrentMap();
    bool loadForCurrentMap();

    bool exportStateToClipboard(int stateIndex);
    bool importStateFromClipboard(int stateIndex);

    void renderTab();
}