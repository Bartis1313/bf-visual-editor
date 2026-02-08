#include "editor_context.h"
#include <ShlObj.h>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

namespace editor
{
    static const char* CONFIG_DIRECTORY = "VisEnvEditor";

    // not really alive, we just check attached player
    bool isPlayerAlive()
    {
        fb::ClientGameContext* ctx = fb::ClientGameContext::GetInstance();
        if (!ctx || !ctx->m_clientPlayerManager)
            return false;
        fb::ClientPlayer* localPlayer = ctx->m_clientPlayerManager->m_localPlayer;
        return localPlayer && localPlayer->isAlive();
    }

    std::string getCurrentMapName()
    {
        fb::ClientGameContext* ctx = fb::ClientGameContext::GetInstance();
        if (!ctx || !ctx->m_level || !ctx->m_level->m_levelData || !ctx->m_level->m_levelData->name)
            return {};
        return ctx->m_level->m_levelData->name;
    }

    fb::VisualEnvironmentManager* getManager()
    {
        return fb::VisualEnvironmentManager::GetInstance();
    }

    fb::WorldRenderSettings* getWorldRenderSettings()
    {
        fb::ClientGameContext* ctx = fb::ClientGameContext::GetInstance();
        if (!ctx || !ctx->m_level || !ctx->m_level->m_worldRenderModule || !ctx->m_level->m_worldRenderModule->m_worldRenderer)
            return nullptr;
        return ctx->m_level->m_worldRenderModule->m_worldRenderer->m_worldRenderSettings;
    }

    std::string getConfigDir()
    {
        PWSTR path = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path)))
        {
            int size = WideCharToMultiByte(CP_UTF8, 0, path, -1, nullptr, 0, nullptr, nullptr);
            std::string result(size - 1, 0);
            WideCharToMultiByte(CP_UTF8, 0, path, -1, &result[0], size, nullptr, nullptr);
            CoTaskMemFree(path);
            return result;
        }
        return "."; // fs::current_path();
    }

    std::string getConfigPath(const std::string& mapName)
    {
        return getConfigDir() + "/" + CONFIG_DIRECTORY + "/" + sanitizeMapName(mapName) + ".json";
    }

    std::string sanitizeMapName(const std::string& mapName)
    {
        // fb::clientPlayerCommands::getClientLevelName
        // SHOULD use tryGetDebugname custom
        size_t pos = mapName.find_last_of('/');
        if (pos != std::string::npos)
            return mapName.substr(pos + 1);
        return mapName;
    }

    std::vector<std::string> getAvailableConfigs()
    {
        std::vector<std::string> configs;
        try
        {
            fs::path configDir = fs::path(getConfigDir()) / CONFIG_DIRECTORY;
            if (fs::exists(configDir))
            {
                for (const auto& entry : fs::directory_iterator(configDir))
                {
                    if (entry.path().extension() == ".json")
                        configs.push_back(entry.path().stem().string());
                }
            }
        }
        catch (const fs::filesystem_error&) {}

        std::sort(configs.begin(), configs.end());
        return configs;
    }

    bool isSafeToOperate()
    {
        return safeToOperate &&
            !levelUnloadingSignaled &&
            editorState != EditorState::Unloading &&
            editorState != EditorState::Idle;
    }

    void setEditorState(EditorState newState)
    {
        if (editorState == newState)
            return;
        editorState = newState;
        safeToOperate = (newState == EditorState::Active || newState == EditorState::Ready);
    }
}