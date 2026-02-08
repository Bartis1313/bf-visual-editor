#pragma once

#include <string>
#include <vector>
#include "../SDK/sdk.h"

namespace editor
{
    enum class EditorState
    {
        Idle,
        Loading,
        Ready,
        Active,
        Unloading
    };

    inline EditorState editorState = EditorState::Idle;
    inline bool levelUnloadingSignaled = false;
    inline bool safeToOperate = false;
    inline bool enabled = true;
    inline bool overridesEnabled = true;
    inline bool autoLoadConfigs = true;
    inline bool autoSaveOnUnload = false;
    inline std::string capturedMapName;
    inline bool hasCapturedOriginals = false;

    // utils
    bool isPlayerAlive();
    std::string getCurrentMapName();
    fb::VisualEnvironmentManager* getManager();
    fb::WorldRenderSettings* getWorldRenderSettings();
    std::string getConfigDir();
    std::string getConfigPath(const std::string& mapName);
    std::string sanitizeMapName(const std::string& mapName);
    std::vector<std::string> getAvailableConfigs();

    bool isSafeToOperate();
    void setEditorState(EditorState newState);
}