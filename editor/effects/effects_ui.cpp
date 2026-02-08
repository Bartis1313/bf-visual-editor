#include "effects.h"

#include <imgui.h>
#include "../render/render.h"
#include "../ui/ui_helpers.h"

namespace editor::effects
{
    static void DrawTransformSphere(const fb::LinearTransform& t, float axisLength)
    {
        if (!selected)
            return;

        ImVec2 origin;
        if (!render::worldToScreen(t.m_trans, origin))
            return;

        render::drawSphere(t.m_trans, 0.5f, 32, 12, IM_COL32_WHITE);

        fb::Vec3 xWorld = {
            t.m_trans.m_x + t.m_right.m_x * axisLength,
            t.m_trans.m_y + t.m_right.m_y * axisLength,
            t.m_trans.m_z + t.m_right.m_z * axisLength
        };
        fb::Vec3 yWorld = {
            t.m_trans.m_x + t.m_up.m_x * axisLength,
            t.m_trans.m_y + t.m_up.m_y * axisLength,
            t.m_trans.m_z + t.m_up.m_z * axisLength
        };
        fb::Vec3 zWorld = {
            t.m_trans.m_x + t.m_forward.m_x * axisLength,
            t.m_trans.m_y + t.m_forward.m_y * axisLength,
            t.m_trans.m_z + t.m_forward.m_z * axisLength
        };

        if (ImVec2 xEnd; render::worldToScreen(xWorld, xEnd))
        {
            render::line(origin, xEnd, ImColor{ 255, 0, 0, 255 }, 2.0f);
            render::text(xEnd, "X", ImColor{ 255, 0, 0, 255 });
        }

        if (ImVec2 yEnd; render::worldToScreen(yWorld, yEnd))
        {
            render::line(origin, yEnd, ImColor{ 0, 255, 0, 255 }, 2.0f);
            render::text(yEnd, "Y", ImColor{ 0, 255, 0, 255 });
        }

        if (ImVec2 zEnd; render::worldToScreen(zWorld, zEnd))
        {
            render::line(origin, zEnd, ImColor{ 0, 0, 255, 255 }, 2.0f);
            render::text(zEnd, "Z", ImColor{ 0, 0, 255, 255 });
        }
    }

    void renderOverlay()
    {
        if (showLocalSphere)
            DrawTransformSphere(transform, 2.0f);

        if (showSpawnedEffects)
        {
            for (const auto& effect : spawned)
            {
                if (ImVec2 pos; render::worldToScreen(effect.transform.m_trans, pos))
                {
                    if (size_t posString = effect.effectName.rfind("/"); posString != std::string::npos)
                    {
                        render::text(pos, effect.effectName.substr(posString), ImColor{ 0, 255, 255, 128 });
                    }
                }
            }
        }
    }

    void renderTab()
    {
        if (ImGui::Button("Scan Effects"))
            scanAssets();

        ImGui::SameLine();
        ImGui::Text("(%zu)", assets.size());

        static char effectSearch[256] = "";
        ImGui::InputText("Search##Effect", effectSearch, sizeof(effectSearch));

        if (ImGui::BeginCombo("Effect", selected ? selected->m_Name : "None"))
        {
            for (const auto& [name, asset] : assets)
            {
                if (effectSearch[0] && !ui::containsIgnoreCase(name, effectSearch))
                    continue;

                if (ImGui::Selectable(name.c_str(), selected == asset))
                    selected = asset;
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", &transform.m_trans.m_x, 0.1f);
        ImGui::DragFloat3("Right (X)", &transform.m_right.m_x, 0.01f);
        ImGui::DragFloat3("Up (Y)", &transform.m_up.m_x, 0.01f);
        ImGui::DragFloat3("Forward (Z)", &transform.m_forward.m_x, 0.01f);

        if (ImGui::Button("Identity"))
        {
            transform.m_right = { 1.0f, 0.0f, 0.0f };
            transform.m_up = { 0.0f, 1.0f, 0.0f };
            transform.m_forward = { 0.0f, 0.0f, 1.0f };
        }

        ImGui::SameLine();
        if (ImGui::Button("From Camera (Position)"))
        {
            fb::GameRenderer::Singleton()->m_viewParams.view.Update();
            transform.m_trans = fb::GameRenderer::Singleton()->m_viewParams.view.m_viewMatrixInverse.m_trans;
        }

        ImGui::Separator();
        ImGui::Checkbox("Show sphere", &showLocalSphere);
        ImGui::Checkbox("Show spawned effects", &showSpawnedEffects);
        ImGui::Separator();

        ImGui::BeginDisabled(!selected);
        if (ImGui::Button("Spawn Effect", ImVec2{ 120, 25 }))
        {
            uint32_t handle = spawnAtTransform(selected, transform);
            if (handle != 0)
            {
                spawned.push_back({ handle, transform, selected->m_Name }); // ignore this warning, it will not be displayed on nullptr
            }
        }
        ImGui::EndDisabled();

        ImGui::SameLine();
        ImGui::BeginDisabled(spawned.empty());
        if (ImGui::Button("Stop All", ImVec2{ 120, 25 }))
        {
            stopAll();
        }
        ImGui::EndDisabled();

        if (!spawned.empty())
        {
            ImGui::Separator();
            ImGui::Text("Spawned Effects (%zu)", spawned.size());

            int toRemove = -1;
            for (size_t i = 0; i < spawned.size(); i++)
            {
                const auto& effect = spawned[i];

                ImGui::PushID(static_cast<int>(i));

                ImGui::Text("[%u] %s", effect.handle, effect.effectName.c_str());
                ImGui::SameLine();

                if (ImGui::SmallButton("Go"))
                    transform = effect.transform;

                ImGui::SameLine();
                if (ImGui::SmallButton("Stop"))
                {
                    stop(effect.handle);
                    toRemove = static_cast<int>(i);
                }

                ImGui::PopID();
            }

            if (toRemove >= 0)
                spawned.erase(spawned.begin() + toRemove);
        }
    }
}