#include <cmath>
#include "render.h"

#include <vector>
#include <cstring>
#include <cfloat>
#include "math.h"
#include <imgui_internal.h>

bool render::engineReady()
{
    return fb::DebugRenderer2::Singleton() != nullptr;
}

ImVec2 render::displaySize()
{
    // The update thread draws overlays before the first Present has created the context.
    return ImGui::GetCurrentContext() ? ImGui::GetIO().DisplaySize : ImVec2{ 0.0f, 0.0f };
}

void render::line2(const ImVec2& a, const ImVec2& b, const ImColor& c, float thickness)
{
    if (backend == Backend::Engine)
    {
        if (fb::DebugRenderer2* r = fb::DebugRenderer2::Singleton())
        {
            const float p[2] = { a.x, a.y }, q[2] = { b.x, b.y };
            r->drawLine2d(p, q, ImU32(c));
            if (thickness > 1.5f)
            {
                const float p2[2] = { a.x, a.y + 1.0f }, q2[2] = { b.x, b.y + 1.0f };
                r->drawLine2d(p2, q2, ImU32(c));
            }
        }
        return;
    }
    ImGui::GetBackgroundDrawList()->AddLine(a, b, c, thickness);
}

void render::line3(const fb::Vec3& a, const fb::Vec3& b, const ImColor& c, float thickness)
{
    if (backend == Backend::Engine)
    {
        fb::DebugRenderer* r = fb::DebugRenderer::Singleton();
        if (!r)
            return;
        r->setDepthTest(lineDepthTest);
        r->setDepthWrite(false);
        r->setTransparent(c.Value.w < 0.99f);
        const unsigned int col = ImU32(c);
        fb::Vec3 cam{ };
        if (lineWidthWorld > 0.0f && cameraPosition(cam))
        {
            // a quad facing the camera: side = normalize(dir x toCamera) * width / 2
            const fb::Vec3 d{ b.m_x - a.m_x, b.m_y - a.m_y, b.m_z - a.m_z };
            const fb::Vec3 t{ cam.m_x - (a.m_x + b.m_x) * 0.5f, cam.m_y - (a.m_y + b.m_y) * 0.5f, cam.m_z - (a.m_z + b.m_z) * 0.5f };
            fb::Vec3 s{ d.m_y * t.m_z - d.m_z * t.m_y, d.m_z * t.m_x - d.m_x * t.m_z, d.m_x * t.m_y - d.m_y * t.m_x };
            const float len = std::sqrt(s.m_x * s.m_x + s.m_y * s.m_y + s.m_z * s.m_z);
            if (len > 1e-6f)
            {
                const float k = lineWidthWorld * 0.5f / len;
                s = { s.m_x * k, s.m_y * k, s.m_z * k };
                r->setDoubleSided(true);
                fb::DebugRenderVertex* v = r->beginVertices(fb::DebugRenderer::Triangles3d, 6);
                if (!v)
                    return;
                const fb::DebugRenderVertex q[4] =
                {
                    { a.m_x - s.m_x, a.m_y - s.m_y, a.m_z - s.m_z, col, 0.0f, 1.0f, 0.0f, 0 },
                    { a.m_x + s.m_x, a.m_y + s.m_y, a.m_z + s.m_z, col, 0.0f, 1.0f, 0.0f, 0 },
                    { b.m_x + s.m_x, b.m_y + s.m_y, b.m_z + s.m_z, col, 0.0f, 1.0f, 0.0f, 0 },
                    { b.m_x - s.m_x, b.m_y - s.m_y, b.m_z - s.m_z, col, 0.0f, 1.0f, 0.0f, 0 }
                };
                v[0] = q[0]; v[1] = q[1]; v[2] = q[2];
                v[3] = q[0]; v[4] = q[2]; v[5] = q[3];
                r->endVertices(fb::DebugRenderer::Triangles3d, v + 6);
                return;
            }
        }
        fb::DebugRenderVertex* v = r->beginVertices(fb::DebugRenderer::Lines3d, 2);
        if (!v)
            return;
        v[0] = { a.m_x, a.m_y, a.m_z, col, 0.0f, 1.0f, 0.0f, 0 };
        v[1] = { b.m_x, b.m_y, b.m_z, col, 0.0f, 1.0f, 0.0f, 0 };
        r->endVertices(fb::DebugRenderer::Lines3d, v + 2);
        return;
    }
    ImVec2 pa{ }, pb{ };
    if (worldToScreen(a, pa) && worldToScreen(b, pb))
        line2(pa, pb, c, thickness);
}

void render::rect2(const ImVec2& mn, const ImVec2& mx, const ImColor& c, bool filled, float thickness)
{
    if (backend == Backend::Engine)
    {
        if (fb::DebugRenderer2* r = fb::DebugRenderer2::Singleton())
        {
            const float p[2] = { mn.x, mn.y }, q[2] = { mx.x, mx.y };
            if (filled)
                r->drawRect2d(p, q, ImU32(c));
            else
                r->drawLineRect2d(p, q, ImU32(c));
        }
        return;
    }
    if (filled)
        ImGui::GetBackgroundDrawList()->AddRectFilled(mn, mx, c);
    else
        ImGui::GetBackgroundDrawList()->AddRect(mn, mx, c, 0.0f, 0, thickness);
}

void render::label(const ImVec2& pos, const char* text, const ImColor& c, float scale, bool shadow)
{
    if (!text || !*text)
        return;
    if (backend == Backend::Engine)
    {
        if (fb::DebugRenderer2* r = fb::DebugRenderer2::Singleton())
            r->drawText(int(pos.x), int(pos.y), text, ImU32(c), scale); // no shadow: plain glyphs
        return;
    }
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    ImFont* font = ImGui::GetFont();
    const float size = ImGui::GetFontSize() * scale;
    if (shadow)
        dl->AddText(font, size, ImVec2{ pos.x + 1.0f, pos.y + 1.0f }, IM_COL32(0, 0, 0, int(c.Value.w * 200.0f)), text);
    dl->AddText(font, size, pos, c, text);
}

float render::textWidth(const char* text, float scale)
{
    if (!text)
        return 0.0f;
    if (backend == Backend::Engine)
        return float(std::strlen(text)) * 8.0f * scale; // the debug font is ~8 px per glyph
    return ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize() * scale, FLT_MAX, 0.0f, text).x;
}

float render::textHeight(float scale)
{
    if (backend == Backend::Engine)
        return 16.0f * scale;
    return ImGui::GetFontSize() * scale;
}

void render::drawSphere(const fb::Vec3& pos, float radius, int segments, int rings, const ImColor& color, float thickness)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    auto drawRing = [drawList, color, thickness](const std::vector<fb::Vec3>& points, bool closed)
        {
            if (backend == Backend::Engine)
            {
                for (size_t i = 0; i < points.size(); i++)
                {
                    const size_t next = (i + 1) % points.size();
                    if (!closed && next == 0)
                        break;
                    line3(points[i], points[next], color, thickness);
                }
                return;
            }
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
                    line2(projected[i].first, projected[next].first, color, thickness);
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

namespace
{
    // little optimazation
    struct ProjCache
    {
        int frame = -1;
        bool valid = false;
        float m[4][4];
        ImVec2 size;
    } g_proj;

    static void ensureProjCache()
    {
        const int frame = ImGui::GetFrameCount();
        if (g_proj.frame == frame)
            return;

        g_proj.frame = frame;
        g_proj.valid = false;

        fb::GameRenderer* renderer = fb::GameRenderer::Singleton();
        if (!renderer)
            return;
        fb::RenderView* rv = fb::getActiveRenderView(renderer);
        if (!rv)
            return;
        fb::updateRenderView(rv);
        const fb::LinearTransform* vp = fb::getViewProjectionMatrix(rv);
        if (!vp)
            return;

        const float* rows[4] =
        {
            reinterpret_cast<const float*>(&vp->m_right),
            reinterpret_cast<const float*>(&vp->m_up),
            reinterpret_cast<const float*>(&vp->m_forward),
            reinterpret_cast<const float*>(&vp->m_trans),
        };
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                g_proj.m[i][j] = rows[i][j];

        g_proj.size = ImGui::GetCurrentContext() ? ImGui::GetIO().DisplaySize : ImVec2{ 0.0f, 0.0f };
        g_proj.valid = g_proj.size.x > 0.0f;
    }
}

bool render::worldToScreen(const fb::Vec3& world, ImVec2& out, float& depth)
{
    ensureProjCache();
    if (!g_proj.valid)
        return false;

    const auto& m = g_proj.m;
    const float w = m[0][3] * world.m_x + m[1][3] * world.m_y + m[2][3] * world.m_z + m[3][3];
    depth = w;
    if (w < 0.01f)
        return false;

    const float x = m[0][0] * world.m_x + m[1][0] * world.m_y + m[2][0] * world.m_z + m[3][0];
    const float y = m[0][1] * world.m_x + m[1][1] * world.m_y + m[2][1] * world.m_z + m[3][1];

    const float hw = g_proj.size.x * 0.5f;
    const float hh = g_proj.size.y * 0.5f;

    const float ox = hw + hw * x / w;
    const float oy = hh - hh * y / w;

    if (ox < 0.0f || ox >= g_proj.size.x || oy < 0.0f || oy >= g_proj.size.y)
        return false;

    out.x = ox;
    out.y = oy;
    return true;
}

static const fb::LinearTransform* cameraTransform()
{
    fb::GameRenderer* renderer = fb::GameRenderer::Singleton();
    fb::RenderView* rv = renderer ? fb::getActiveRenderView(renderer) : nullptr;
    if (!rv)
        return nullptr;
#if defined(BFVE_GAME_BF3)
    return &rv->m_desc.transform;
#else
    return &rv->m_Transform;
#endif
}

bool render::cameraPosition(fb::Vec3& out)
{
    const fb::LinearTransform* t = cameraTransform();
    if (!t)
        return false;
    out = t->m_trans;
    return true;
}

bool render::cameraForward(fb::Vec3& out)
{
    const fb::LinearTransform* t = cameraTransform();
    if (!t)
        return false;
    const fb::Vec3& f = t->m_forward;
    const float len = std::sqrt(f.m_x * f.m_x + f.m_y * f.m_y + f.m_z * f.m_z);
    if (len < 1e-6f)
        return false;
    out.m_x = -f.m_x / len;
    out.m_y = -f.m_y / len;
    out.m_z = -f.m_z / len;
    return true;
}

bool render::worldToScreen(const fb::Vec3& world, ImVec2& out)
{
    float depth;
    return worldToScreen(world, out, depth);
}

void render::circle(const ImVec2& center, float radius, const ImColor& color, int segments, float thickness)
{
    if (backend == Backend::Engine)
    {
        const int n = segments < 6 ? 6 : segments;
        ImVec2 prev{ center.x + radius, center.y };
        for (int i = 1; i <= n; ++i)
        {
            const float t = math::PI_2 * float(i) / float(n);
            const ImVec2 cur{ center.x + radius * std::cos(t), center.y + radius * std::sin(t) };
            line2(prev, cur, color, thickness);
            prev = cur;
        }
        return;
    }
    ImGui::GetBackgroundDrawList()->AddCircle(center, radius, color, segments, thickness);
}

void render::text(const ImVec2& pos, ImFont* font, const std::string& text, const ImColor& color, bool centered, bool dropShadow)
{
    if (backend == Backend::Engine)
    {
        render::text(pos, text, color, centered, dropShadow);
        return;
    }
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
    if (backend == Backend::Engine)
    {
        ImVec2 p = pos;
        if (centered)
            p.x -= textWidth(text.c_str()) * 0.5f;
        label(p, text.c_str(), color, 1.0f, false);
        return;
    }
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
    line2(start, end, color, thickness);
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