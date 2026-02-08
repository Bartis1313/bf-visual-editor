#include "render.h"

#include <vector>
#include "math.h"
#include <imgui_internal.h>

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

namespace render
{
    static std::vector<ImNotify> m_allNotifies;
}

void render::ImNotify::handle()
{
    if (m_allNotifies.empty())
        return;

    const ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    ImFont* font = ImGui::GetFont();

    constexpr float referenceHeight = 1080.0f;
    constexpr float minScale = 0.7f;
    constexpr float maxScale = 2.0f;
    const float scale = ImClamp(screenSize.y / referenceHeight, minScale, maxScale);

    const float baseFontSize = ImGui::GetFontSize();
    const float titleFontSize = baseFontSize * scale;
    const float messageFontSize = baseFontSize * scale * 0.9f;
    const float padding = 10.0f * scale;
    const float widthRect = ImClamp(screenSize.x * 0.15f, 200.0f, 400.0f);
    const float rounding = 6.0f * scale;
    const float borderThickness = 1.5f;
    const float separatorHeight = 1.0f;

    constexpr float animationIN = 0.2f;
    constexpr float animationOUT = 0.4f;
    constexpr float slideDistance = 50.0f;

    float yOffset = padding;

    for (size_t i = 0; auto& notify : m_allNotifies)
    {
        const double elapsed = ImGui::GetTime() - notify.time;

        const float inRaw = static_cast<float>(ImClamp(elapsed / animationIN, 0.0, 1.0));
        const float in = 1.0f - (1.0f - inRaw) * (1.0f - inRaw);

        const float outRaw = static_cast<float>(ImClamp((elapsed - notify.maxTime) / animationOUT, 0.0, 1.0));
        const float out = outRaw * outRaw;

        const float alpha = in * (1.0f - out);
        notify.alpha = alpha;

        if (alpha <= 0.001f && elapsed > notify.maxTime)
        {
            m_allNotifies.erase(m_allNotifies.begin() + i);
            continue;
        }

        ImVec4 bgColor{ };
        ImVec4 borderColor{ };
        ImVec4 titleColor{ };

        switch (notify.type)
        {
        case NotifyType::Success:
            bgColor = ImVec4{ 0.05f, 0.15f, 0.05f, 0.95f };
            borderColor = ImVec4{ 0.2f, 0.8f, 0.2f, 0.7f };
            titleColor = ImVec4{ 0.3f, 1.0f, 0.3f, 1.0f };
            break;
        case NotifyType::Warning:
            bgColor = ImVec4{ 0.15f, 0.12f, 0.02f, 0.95f };
            borderColor = ImVec4{ 0.9f, 0.7f, 0.1f, 0.7f };
            titleColor = ImVec4{ 1.0f, 0.85f, 0.3f, 1.0f };
            break;
        case NotifyType::Error:
            bgColor = ImVec4{ 0.18f, 0.04f, 0.04f, 0.95f };
            borderColor = ImVec4{ 0.9f, 0.2f, 0.2f, 0.7f };
            titleColor = ImVec4{ 1.0f, 0.4f, 0.4f, 1.0f };
            break;
        case NotifyType::Info:
        default:
            bgColor = ImVec4{ 0.04f, 0.08f, 0.15f, 0.95f };
            borderColor = ImVec4{ 0.3f, 0.5f, 0.9f, 0.7f };
            titleColor = ImVec4{ 0.5f, 0.8f, 1.0f, 1.0f };
            break;
        }

        const ImVec4 messageColor = ImVec4{ 0.9f, 0.9f, 0.9f, 0.95f };
        const ImVec4 separatorColor = ImVec4{ titleColor.x, titleColor.y, titleColor.z, 0.4f };
        auto applyAlpha = [alpha](const ImVec4& col) -> ImU32 {
            return ImGui::ColorConvertFloat4ToU32(ImVec4{ col.x, col.y, col.z, col.w * alpha });
            };

        const float textWidth = widthRect - padding * 2.0f;

        ImVec2 titleSize = font->CalcTextSizeA(titleFontSize, FLT_MAX, textWidth,
            notify.title.c_str(), nullptr, nullptr);
        ImVec2 messageSize = font->CalcTextSizeA(messageFontSize, FLT_MAX, textWidth,
            notify.message.c_str(), nullptr, nullptr);

        const float boxHeight = padding + titleSize.y + padding * 0.5f + separatorHeight +
            padding * 0.5f + messageSize.y + padding;

        const float slideIn = (1.0f - in) * slideDistance;
        const float slideOut = out * slideDistance * 2.0f;
        const float xPos = screenSize.x - padding - widthRect + (slideIn + slideOut);

        const ImVec2 boxMin = ImVec2{ xPos, yOffset };
        const ImVec2 boxMax = ImVec2{ xPos + widthRect, yOffset + boxHeight };

        drawList->AddRectFilled(boxMin, boxMax, applyAlpha(bgColor), rounding);
        drawList->AddRect(boxMin, boxMax, applyAlpha(borderColor), rounding, 0, borderThickness);

        const ImVec2 titlePos = ImVec2{ boxMin.x + padding, boxMin.y + padding };
        drawList->AddText(font, titleFontSize, titlePos, applyAlpha(titleColor), notify.title.c_str(), nullptr, textWidth);

        const float sepY = titlePos.y + titleSize.y + padding * 0.5f;
        drawList->AddLine(
            ImVec2{ boxMin.x + padding, sepY },
            ImVec2{ boxMax.x - padding, sepY },
            applyAlpha(separatorColor),
            separatorHeight
        );

        const ImVec2 messagePos = ImVec2{ boxMin.x + padding, sepY + padding * 0.5f + separatorHeight };
        drawList->AddText(font, messageFontSize, messagePos, applyAlpha(messageColor), notify.message.c_str(), nullptr, textWidth);

        yOffset += (boxHeight + padding) * ImClamp(alpha * 2.0f, 0.0f, 1.0f);

        ++i;
    }
}
void render::ImNotify::add(NotifyType type, const std::string& title, const std::string& message, float time)
{
    m_allNotifies.emplace_back(ImNotify{ title, message, time, type });
}