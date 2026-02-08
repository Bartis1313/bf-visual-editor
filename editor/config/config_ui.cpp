#include "config.h"
#include "../editor_context.h"
#include "../states/states.h"
#include "../render/render.h"
#include <imgui.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace editor::config
{
    static char customConfigName[256] = "";
    static bool showSaveAsPopup = false;
    static bool showDeleteConfirm = false;
    static std::string configToDelete;

    void renderTab()
    {
        ImGui::TextColored(ImVec4{ 0.6f, 0.8f, 1.0f, 1.0f }, "Configuration Management");
        ImGui::Separator();

        if (!capturedMapName.empty())
        {
            ImGui::Text("Current Map: %s", sanitizeMapName(capturedMapName).c_str());

            std::string configPath = getConfigPath(capturedMapName);
            bool configExists = fs::exists(configPath);

            ImGui::Text("Config Path: %s", configPath.c_str());
            ImGui::Text("Config Exists: %s", configExists ? "Yes" : "No");
        }
        else
        {
            ImGui::TextColored(ImVec4{ 0.8f, 0.8f, 0.2f, 1.0f }, "No map loaded");
        }

        ImGui::Separator();

        ImGui::Text("Quick Save/Load (Map-based):");

        ImGui::BeginDisabled(capturedMapName.empty());

        if (ImGui::Button("Save for Map", ImVec2{ 120, 0 }))
        {
            if (saveForCurrentMap())
            {
                render::ImNotify::add(NotifyType::Success, "Config Saved", sanitizeMapName(capturedMapName));
            }
            else
            {
                render::ImNotify::add(NotifyType::Error, "Save Failed", "Could not save config");
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Load for Map", ImVec2{ 120, 0 }))
        {
            if (!loadForCurrentMap())
            {
                render::ImNotify::add(NotifyType::Error, "Load Failed", "Config not found for this map");
            }
        }

        ImGui::EndDisabled();

        ImGui::Spacing();

        ImGui::Text("Custom Config:");

        ImGui::SetNextItemWidth(200);
        ImGui::InputTextWithHint("##ConfigName", "Enter config name...", customConfigName, sizeof(customConfigName));

        ImGui::SameLine();

        bool hasCustomName = strlen(customConfigName) > 0;
        ImGui::BeginDisabled(!hasCustomName);

        if (ImGui::Button("Save As", ImVec2{ 80, 0 }))
        {
            std::string safeName = sanitizeMapName(customConfigName);
            if (!safeName.empty())
            {
                std::string fullPath = getConfigDir() + "/VisEnvEditor/" + safeName + ".json";
                if (fs::exists(fullPath))
                {
                    showSaveAsPopup = true;
                }
                else
                {
                    if (save(fullPath))
                    {
                        render::ImNotify::add(NotifyType::Success, "Config Saved", safeName);
                    }
                }
            }
        }

        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(!hasCustomName);

        if (ImGui::Button("Load", ImVec2{ 80, 0 }))
        {
            std::string safeName = sanitizeMapName(customConfigName);
            if (!safeName.empty())
            {
                std::string fullPath = getConfigDir() + "/VisEnvEditor/" + safeName + ".json";
                if (!fs::exists(fullPath))
                {
                    render::ImNotify::add(NotifyType::Error, "Load Failed", std::format("'{}' not found", safeName));
                }
                else
                {
                    load(fullPath);
                }
            }
        }

        ImGui::EndDisabled();

        if (showSaveAsPopup)
        {
            ImGui::OpenPopup("ConfirmOverwrite");
            showSaveAsPopup = false;
        }

        if (ImGui::BeginPopupModal("ConfirmOverwrite", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Config '%s' already exists.", customConfigName);
            ImGui::Text("Do you want to overwrite it?");
            ImGui::Separator();

            if (ImGui::Button("Overwrite", ImVec2{ 120, 0 }))
            {
                std::string safeName = sanitizeMapName(customConfigName);
                std::string fullPath = getConfigDir() + "/VisEnvEditor/" + safeName + ".json";
                if (save(fullPath))
                {
                    render::ImNotify::add(NotifyType::Success, "Config Saved", std::format("{} (overwritten)", safeName));
                }
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2{ 120, 0 }))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::Separator();

        ImGui::Text("Available Configs:");

        std::vector<std::string> configs = getAvailableConfigs();

        if (configs.empty())
        {
            ImGui::TextDisabled("No configs found");
        }
        else
        {
            ImGui::BeginChild("ConfigList", ImVec2{ 0, 200 }, true);

            for (const auto& configName : configs)
            {
                ImGui::PushID(configName.c_str());

                bool isCurrent = !capturedMapName.empty() &&
                    sanitizeMapName(capturedMapName) == configName;

                if (isCurrent)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.2f, 1.0f, 0.2f, 1.0f });

                if (ImGui::Selectable(configName.c_str(), isCurrent, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    strncpy_s(customConfigName, configName.c_str(), sizeof(customConfigName) - 1);

                    if (ImGui::IsMouseDoubleClicked(0))
                    {
                        std::string fullPath = getConfigDir() + "/VisEnvEditor/" + configName + ".json";
                        load(fullPath);
                    }
                }

                if (isCurrent)
                    ImGui::PopStyleColor();

                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem("Load"))
                    {
                        std::string fullPath = getConfigDir() + "/VisEnvEditor/" + configName + ".json";
                        load(fullPath);
                    }

                    if (ImGui::MenuItem("Copy Name"))
                    {
                        strncpy_s(customConfigName, configName.c_str(), sizeof(customConfigName) - 1);
                    }

                    if (ImGui::MenuItem("Rename..."))
                    {
                        strncpy_s(customConfigName, configName.c_str(), sizeof(customConfigName) - 1);
                    }

                    ImGui::Separator();

                    if (ImGui::MenuItem("Delete", nullptr, false))
                    {
                        configToDelete = configName;
                        showDeleteConfirm = true;
                    }

                    ImGui::EndPopup();
                }

                if (ImGui::IsItemHovered() && !ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId))
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Click: Select | Double-click: Load");
                    ImGui::Text("Right-click: More options");
                    if (isCurrent)
                        ImGui::TextColored(ImVec4{ 0.5f, 0.8f, 1.0f, 1.0f }, "(Matches current map)");
                    ImGui::EndTooltip();
                }

                ImGui::PopID();
            }

            ImGui::EndChild();
        }

        if (showDeleteConfirm)
        {
            ImGui::OpenPopup("ConfirmDelete");
            showDeleteConfirm = false;
        }

        if (ImGui::BeginPopupModal("ConfirmDelete", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to delete '%s'?", configToDelete.c_str());
            ImGui::TextColored(ImVec4{ 1.0f, 0.5f, 0.5f, 1.0f }, "This cannot be undone!");
            ImGui::Separator();

            if (ImGui::Button("Delete", ImVec2{ 120, 0 }))
            {
                std::string fullPath = getConfigDir() + "/VisEnvEditor/" + configToDelete + ".json";
                std::error_code ec;
                if (fs::remove(fullPath, ec))
                {
                    render::ImNotify::add(NotifyType::Info, "Config Deleted", configToDelete);
                }
                else
                {
                    render::ImNotify::add(NotifyType::Error, "Delete Failed", ec.message());
                }
                configToDelete.clear();
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2{ 120, 0 }))
            {
                configToDelete.clear();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::Separator();

        ImGui::Text("State Export/Import:");

        auto& stateOrder = states::getStateOrder();
        static int selectedExportState = 0;

        if (stateOrder.empty())
        {
            ImGui::TextDisabled("No states available");
        }
        else
        {
            if (selectedExportState >= static_cast<int>(stateOrder.size()))
                selectedExportState = 0;

            StateEditData* selectedData = states::getStateData(stateOrder[selectedExportState]);
            const char* selectedName = selectedData ? selectedData->stateName.c_str() : "Unknown";

            ImGui::SetNextItemWidth(200);
            if (ImGui::BeginCombo("State##Export", selectedName))
            {
                for (int i = 0; i < static_cast<int>(stateOrder.size()); i++)
                {
                    StateEditData* data = states::getStateData(stateOrder[i]);
                    if (data && ImGui::Selectable(data->stateName.c_str(), selectedExportState == i))
                        selectedExportState = i;
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();
            if (ImGui::Button("Export to Clipboard"))
            {
                if (exportStateToClipboard(selectedExportState))
                {
                    render::ImNotify::add(NotifyType::Success, "Exported", std::format("'{}' copied to clipboard", selectedName));
                }
                else
                {
                    render::ImNotify::add(NotifyType::Error, "Export Failed", "Could not export state");
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Import from Clipboard"))
            {
                if (importStateFromClipboard(selectedExportState))
                {
                    render::ImNotify::add(NotifyType::Success, "Imported", std::format("'{}' updated from clipboard", selectedName));
                }
                else
                {
                    render::ImNotify::add(NotifyType::Error, "Import Failed", "Invalid or incompatible clipboard data");
                }
            }
        }

        ImGui::Separator();

        ImGui::Text("Settings:");

        ImGui::Checkbox("Auto-load configs on map load", &autoLoadConfigs);
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("When enabled, automatically loads the config");
            ImGui::Text("for the current map when spawning in-game.");
            ImGui::EndTooltip();
        }

        ImGui::Checkbox("Auto-save on map unload", &autoSaveOnUnload);
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("When enabled, automatically saves the current");
            ImGui::Text("config when leaving the map.");
            ImGui::EndTooltip();
        }
    }
}