#include "render.h"

#include <vector>
#include "math.h"

void render::drawSphere(const fb::Vec3& pos, float radius, int segments, int rings, const ImColor& color, float thickness)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    auto drawRing = [drawList, color, thickness](const std::vector<fb::Vec3>& points, bool closed)
        {
            std::vector<std::pair<ImVec2, bool>> projected;
            projected.reserve(points.size());

            for (const auto& p : points)
            {
                ImVec2 screen;
                bool valid = worldToScreen(p, screen);
                projected.push_back({ screen, valid });
            }

            for (size_t i = 0; i < projected.size(); i++)
            {
                size_t next = (i + 1) % projected.size();
                if (!closed && next == 0)
                    break;

                if (projected[i].second && projected[next].second)
                {
                    drawList->AddLine(projected[i].first, projected[next].first, color, thickness);
                }
            }
        };

    for (int r = 1; r < rings; r++)
    {
        float phi = math::PI * r / static_cast<float>(rings);
        float ringRadius = radius * std::sin(phi);
        float z = radius * std::cos(phi);

        std::vector<fb::Vec3> points;
        points.reserve(segments + 1);

        for (int i = 0; i <= segments; i++)
        {
            float theta = math::PI_2 * i / static_cast<float>(segments);
            points.push_back(
                {
                    pos.m_x + ringRadius * std::cos(theta),
                    pos.m_y + ringRadius * std::sin(theta),
                    pos.m_z + z
                });
        }
        drawRing(points, true);
    }

    int numMeridians = segments;
    for (int m = 0; m < numMeridians; m++)
    {
        float theta = math::PI_2 * m / static_cast<float>(numMeridians);

        std::vector<fb::Vec3> points;
        points.reserve(rings + 1);

        for (int i = 0; i <= rings; i++)
        {
            float phi = math::PI * i / static_cast<float>(rings);
            points.push_back(
                {
                    pos.m_x + radius * std::sin(phi) * std::cos(theta),
                    pos.m_y + radius * std::sin(phi) * std::sin(theta),
                    pos.m_z + radius * std::cos(phi)
                });
        }
        drawRing(points, false);
    }
}

bool render::worldToScreen(const fb::Vec3& world, ImVec2& out)
{
    fb::GameRenderer* renderer = fb::GameRenderer::Singleton();
    if (!renderer)
        return false;

    renderer->m_viewParams.view.Update();

    alignas(16) fb::Vec3 w = world;
    alignas(16) fb::Vec3 o{ };

    fb::LinearTransform* view = &renderer->m_viewParams.view.m_viewMatrix;
    fb::LinearTransform* projection = &renderer->m_viewParams.view.m_projectionMatrix;

    typedef bool(__cdecl* tWorldToScreen)(fb::Vec3*, fb::LinearTransform*, fb::LinearTransform*, fb::Vec3*);
    tWorldToScreen _WorldToScreen = (tWorldToScreen)0x00764A90;

    bool ret = _WorldToScreen(&w, view, projection, &o);
    if (!ret)
        return false;

    out.x = o.m_x;
    out.y = o.m_y;

    return true;
}

void render::text(const ImVec2& pos, ImFont* font, const std::string& text, const ImColor& color, bool centered, bool dropShadow)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    ImGui::PushFont(font);

    ImVec2 _pos = pos;
    if (auto tsize = ImGui::CalcTextSize(text.c_str()); centered)
        _pos.x -= tsize.x / 2.0f;

    if (dropShadow)
    {
        ImColor dropColor{ 0.0f, 0.0f, 0.0f, color.Value.w };
        drawList->AddText(_pos, dropColor, text.c_str());
    }

    drawList->AddText(_pos, color, text.c_str());

    ImGui::PopFont();
}

void render::text(const ImVec2& pos, const std::string& text, const ImColor& color, bool centered, bool dropShadow)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    ImVec2 _pos = pos;
    if (auto tsize = ImGui::CalcTextSize(text.c_str()); centered)
        _pos.x -= tsize.x / 2.0f;

    if (dropShadow)
    {
        ImColor dropColor{ 0.0f, 0.0f, 0.0f, color.Value.w };
        drawList->AddText(_pos, dropColor, text.c_str());
    }

    drawList->AddText(_pos, color, text.c_str());
}

void render::line(const ImVec2& start, const ImVec2& end, const ImColor& color, float thickness)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    drawList->AddLine(start, end, color, thickness);
}