#include "camera.h"

#include "../editor.h"
#include "../editor_context.h"
#include "../../hooks/functions.h"
#include "../../utils/log.h"
#include "../render/math.h"


#include <imgui.h>

#include <Windows.h>

#include <cmath>
#include <cstring>
#include <algorithm>
#include <atomic>
#include <string>
#include <format>

namespace editor::camera
{
    namespace
    {
#if defined(BFVE_GAME_BF3)
        using SetTransformFn = void(__thiscall*)(fb::RenderView*, const fb::LinearTransform*);
        using SetFovFn = void(__thiscall*)(fb::RenderView*, float);
        using GetSettingsFn = void*(__thiscall*)(void* manager, const void* typeInfo);

        const fb::LinearTransform& gameTransform(const fb::RenderView* v) { return v->m_desc.transform; }
#elif defined(BFVE_GAME_BF4)
        using SetTransformFn = void(__fastcall*)(fb::RenderView*, const void*);
        using SetFovFn = void(__fastcall*)(fb::RenderView*, float);
        using GetSettingsFn = void*(__fastcall*)(void* manager, const void* typeInfo);

        struct TransformBlock { fb::LinearTransform a; fb::LinearTransform t; float w[4]; };
        const fb::LinearTransform& gameTransform(const fb::RenderView* v) { return v->m_Transform; }
#endif

        bool g_seeded = false;
        bool g_haveMouse = false;
        POINT g_center{ };
        LARGE_INTEGER g_lastTick{ };

        // WM_INPUT deltas accumulated by the window proc, drained once per frame.
        std::atomic<long> g_rawDx{ 0 }, g_rawDy{ 0 };
        std::atomic<bool> g_rawSeen{ false };
        float g_smoothYaw = 0.0f, g_smoothPitch = 0.0f;

        bool g_gameInputBlocked = false;
        bool g_active = false; // enabled and not showing first person
        bool g_holdsApplied = false;

        bool g_toggleWasDown = false;
        bool g_fpWasDown = false;
        ULONGLONG g_fpDownAt = 0;
        int* g_capturingKey = nullptr;

        template <typename T>
        struct Hold
        {
            T* target = nullptr;
            T saved{ };
            bool held = false;

            void hold(T* t, T value)
            {
                if (!t)
                    return;
                if (!held || target != t)
                {
                    target = t;
                    saved = *t;
                    held = true;
                }
                *t = value;
            }

            void release(T* current)
            {
                if (held && target && target == current)
                    *target = saved;
                held = false;
                target = nullptr;
            }
        };

        Hold<bool> g_hudHold;
        Hold<bool> g_foregroundHold;

        fb::Vec3 vec(float x, float y, float z) { fb::Vec3 v{ }; v.m_x = x; v.m_y = y; v.m_z = z; return v; }

        fb::Vec3 lookDir()
        {
            const float cy = std::cos(yaw * math::DEG), sy = std::sin(yaw * math::DEG);
            const float cp = std::cos(pitch * math::DEG), sp = std::sin(pitch * math::DEG);
            return vec(sy * cp, sp, -cy * cp);
        }

        fb::LinearTransform fromAngles()
        {
            const float cy = std::cos(yaw * math::DEG), sy = std::sin(yaw * math::DEG);
            const float cp = std::cos(pitch * math::DEG), sp = std::sin(pitch * math::DEG);

            fb::LinearTransform t{};
            t.m_right = vec(cy, 0.0f, sy);
            t.m_up = vec(-sp * sy, cp, sp * cy);
            t.m_forward = vec(-sy * cp, -sp, cy * cp);
            t.m_trans = position;
            return t;
        }

        void seedFrom(const fb::LinearTransform& t)
        {
            position = t.m_trans;

            const float dx = -t.m_forward.m_x, dy = -t.m_forward.m_y, dz = -t.m_forward.m_z;
            yaw = std::atan2(dx, -dz) / math::DEG;
            pitch = std::asin(std::clamp(dy, -1.0f, 1.0f)) / math::DEG;
            g_seeded = true;

            logger::info("[camera] free camera at {:.1f} {:.1f} {:.1f} yaw {:.0f} pitch {:.0f}",
                position.m_x, position.m_y, position.m_z, yaw, pitch);
        }

        float frameSeconds()
        {
            LARGE_INTEGER now{}, freq{};
            QueryPerformanceCounter(&now);
            QueryPerformanceFrequency(&freq);
            const float dt = g_lastTick.QuadPart
                ? float(now.QuadPart - g_lastTick.QuadPart) / float(freq.QuadPart) : 0.0f;
            g_lastTick = now;
            return (std::min)(dt, 0.1f);
        }

        // Turn by degrees. Smoothing is a per-frame EMA; the remainder carries over.
        void turn(float dYaw, float dPitch)
        {
            if (smoothing > 0.0f)
            {
                const float k = 1.0f - std::clamp(smoothing, 0.0f, 0.95f);
                g_smoothYaw += (dYaw - g_smoothYaw) * k;
                g_smoothPitch += (dPitch - g_smoothPitch) * k;
                dYaw = g_smoothYaw;
                dPitch = g_smoothPitch;
            }

            yaw += dYaw;
            pitch += invertPitch ? -dPitch : dPitch;
            pitch = std::clamp(pitch, -89.0f, 89.0f);
            if (yaw > 180.0f)
                yaw -= 360.0f;
            if (yaw < -180.0f)
                yaw += 360.0f;
        }

        void move(float forward, float right, float up, float dt, bool fast)
        {
            const float s = speed * dt * (fast ? boost : 1.0f);
            const fb::Vec3 d = lookDir();
            const fb::LinearTransform t = fromAngles();

            position.m_x += (d.m_x * forward + t.m_right.m_x * right) * s;
            position.m_y += (d.m_y * forward + t.m_right.m_y * right + up) * s;
            position.m_z += (d.m_z * forward + t.m_right.m_z * right) * s;
        }

        bool readEngineInput(float dt)
        {
            fb::BorderInputNode* bin = fb::BorderInputNode::GetInstance();
            if (!bin || !bin->m_inputCache)
                return false;

            const auto level = [&](fb::InputConceptIdentifiers id)
                {
                    return bin->m_inputCache->m_conceptCache[static_cast<uint32_t>(id)];
                };

            const float k = dt * lookSpeed / math::DEG;
            turn(level(fb::InputConceptIdentifiers::ConceptYaw) * k, level(fb::InputConceptIdentifiers::ConceptPitch) * k);

            move(level(fb::InputConceptIdentifiers::ConceptMoveFB), level(fb::InputConceptIdentifiers::ConceptMoveLR),
                level(fb::InputConceptIdentifiers::ConceptJump) - level(fb::InputConceptIdentifiers::ConceptCrouch),
                dt,
                level(fb::InputConceptIdentifiers::ConceptSprint) > 0.5f);
            return true;
        }

        bool down(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }

        bool windowCenter(POINT& center)
        {
            RECT rc{};
            if (!g_hWnd || !GetClientRect(g_hWnd, &rc))
                return false;
            center = { (rc.left + rc.right) / 2, (rc.top + rc.bottom) / 2 };
            ClientToScreen(g_hWnd, &center);
            return true;
        }

        // Cursor-delta fallback for when no WM_INPUT arrives: re-centre every frame so the
        // edge of the screen is never hit.
        void readCursorMouse()
        {
            POINT center{ };
            if (!windowCenter(center))
            {
                g_haveMouse = false;
                return;
            }

            POINT p{};
            if (g_haveMouse && center.x == g_center.x && center.y == g_center.y && GetCursorPos(&p))
                turn(float(p.x - center.x) * sensitivity, -float(p.y - center.y) * sensitivity);

            SetCursorPos(center.x, center.y);
            if (g_gameInputBlocked)
                SetCursor(nullptr); // cursor mode shows the OS cursor; hkWndProc answers WM_SETCURSOR too
            g_center = center;
            g_haveMouse = true;
        }

        void readRawInput(float dt)
        {
            if (g_rawSeen.load(std::memory_order_relaxed) && !g_gameInputBlocked)
            {
                const long dx = g_rawDx.exchange(0, std::memory_order_relaxed);
                const long dy = g_rawDy.exchange(0, std::memory_order_relaxed);
                turn(float(dx) * sensitivity, -float(dy) * sensitivity);
            }
            else
            {
                readCursorMouse();
            }

            const float fb = (down('W') ? 1.0f : 0.0f) - (down('S') ? 1.0f : 0.0f);
            const float lr = (down('D') ? 1.0f : 0.0f) - (down('A') ? 1.0f : 0.0f);
            const float ud = (down(VK_SPACE) ? 1.0f : 0.0f) - (down('C') ? 1.0f : 0.0f);
            move(fb, lr, ud, dt, down(VK_SHIFT));
        }

        // Keyboard typing mode + mouse cursor mode, what hkWndProc does when the menu opens.
        // Never touched while the menu is open: the menu owns those switches then.
        void setGameInputBlocked(bool block)
        {
            if (block == g_gameInputBlocked)
                return;

            fb::BorderInputNode* bin = fb::BorderInputNode::GetInstance();
            if (!bin)
                return;

            if (bin->m_keyboard)
                bin->m_keyboard->enableTypingMode(block);
#if defined(BFVE_GAME_BF3)
            if (bin->m_mouse)
                bin->m_mouse->enableCursorMode(block, 1);
#else
            if (bin->m_mouse && bin->m_mouse->m_pDevice && !bin->m_mouse->m_pDevice->m_UIOwnsInput)
                bin->m_mouse->enableCursorMode(block, 1);
#endif

            if (block)
                SetCursor(nullptr);

            g_gameInputBlocked = block;
            logger::info("[camera] game input {}", block ? "blocked" : "restored");
        }

        void readInput(float dt)
        {
            const bool capturing = !editor::isEnabled() && GetForegroundWindow() == g_hWnd;

            if (!editor::isEnabled())
                setGameInputBlocked(capturing);
            else
                g_gameInputBlocked = false;

            if (!capturing)
            {
                g_haveMouse = false;
                g_rawDx.store(0, std::memory_order_relaxed);
                g_rawDy.store(0, std::memory_order_relaxed);
                return;
            }

            if (!g_gameInputBlocked && readEngineInput(dt))
                return;

            readRawInput(dt);
        }

        bool* uiDrawEnable()
        {
            auto* settings = *reinterpret_cast<fb::UISettings**>(OFF_Settings_UISettings);
            if (!settings)
            {
                void* manager = *reinterpret_cast<void**>(OFF_g_settingsManager);
                if (!manager)
                    return nullptr;
                settings = static_cast<fb::UISettings*>(reinterpret_cast<GetSettingsFn>(OFF_SettingsManager_getSettings)(
                    manager, reinterpret_cast<const void*>(fb::UISettings::ClassInfoPtr())));
            }
            return settings ? &settings->m_DrawEnable : nullptr;
        }

        bool* foregroundEnable()
        {
            fb::WorldRenderSettings* wr = editor::getWorldRenderSettings();
            return wr ? &wr->m_ForegroundEnable : nullptr;
        }

        void applyHolds()
        {
            bool* hud = uiDrawEnable();
            if (hideHud)
                g_hudHold.hold(hud, false);
            else
                g_hudHold.release(hud);

            g_foregroundHold.hold(foregroundEnable(), false);

            g_holdsApplied = true;
        }

        void releaseAll()
        {
            g_hudHold.release(uiDrawEnable());
            g_foregroundHold.release(foregroundEnable());
            if (!editor::isEnabled())
                setGameInputBlocked(false);
            else
                g_gameInputBlocked = false;
            g_holdsApplied = false;
        }

        // Toggle key: on/off edge. First-person key: held = first person while down; a tap
        // (under 250 ms) flips the latch.
        bool pollKeys()
        {
            if (GetForegroundWindow() != g_hWnd || g_capturingKey)
                return firstPerson;

            const bool t = toggleKey && down(toggleKey);
            if (t && !g_toggleWasDown)
            {
                enabled = !enabled;
                g_seeded = false;
                firstPerson = false;
            }
            g_toggleWasDown = t;

            const bool fp = firstPersonKey && down(firstPersonKey);
            if (fp && !g_fpWasDown)
                g_fpDownAt = GetTickCount64();
            if (!fp && g_fpWasDown && GetTickCount64() - g_fpDownAt < 250)
                firstPerson = !firstPerson;
            g_fpWasDown = fp;

            return firstPerson || fp;
        }

        std::string keyName(int vk)
        {
            if (!vk)
                return "none";

            UINT sc = MapVirtualKeyA(UINT(vk), MAPVK_VK_TO_VSC);
            switch (vk)
            {
            case VK_INSERT: case VK_DELETE: case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
            case VK_LEFT: case VK_RIGHT: case VK_UP: case VK_DOWN: case VK_RMENU: case VK_RCONTROL:
            case VK_DIVIDE: case VK_NUMLOCK:
                sc |= 0x100;
                break;
            }

            char buf[64]{ };
            if (GetKeyNameTextA(LONG(sc) << 16, buf, sizeof(buf)) > 0)
                return buf;
            return std::format("VK {:#04x}", vk);
        }

        // A button showing the bound key; click, then press the new one (Escape clears).
        void keyBinding(const char* label, int* key)
        {
            ImGui::PushID(key);
            if (g_capturingKey == key)
            {
                ImGui::Button("press a key...", ImVec2(140.0f, 0.0f));
                for (int vk = VK_BACK; vk < 0xFF; ++vk)
                {
                    if (vk == VK_LBUTTON || vk == VK_RBUTTON || vk == VK_MBUTTON || !down(vk))
                        continue;
                    *key = vk == VK_ESCAPE ? 0 : vk;
                    g_capturingKey = nullptr;
                    break;
                }
            }
            else if (ImGui::Button(keyName(*key).c_str(), ImVec2(140.0f, 0.0f)))
            {
                g_capturingKey = key;
            }
            ImGui::SameLine();
            ImGui::TextUnformatted(label);
            ImGui::PopID();
        }
    }

    void resetToGame()
    {
        g_seeded = false;
    }

    bool hidesCursor()
    {
        return g_active && g_gameInputBlocked && !editor::isEnabled();
    }

    void onRawMouse(long dx, long dy)
    {
        g_rawSeen.store(true, std::memory_order_relaxed);
        if (!g_active || editor::isEnabled())
            return;
        g_rawDx.fetch_add(dx, std::memory_order_relaxed);
        g_rawDy.fetch_add(dy, std::memory_order_relaxed);
    }

    void onGameCameraTransform(fb::LinearTransform* out, int viewIndex)
    {
        if (!out || !g_active || !g_seeded || viewIndex != 0)
            return;
        *out = fromAngles();
    }

    void onRenderViewParams(fb::RenderView* view)
    {
        const float dt = frameSeconds();

        if (!view)
            return;

        const bool showFirstPerson = pollKeys();
        g_active = enabled && !showFirstPerson;

        if (!g_active)
        {
            // First person keeps the seed, so the way back lands where the camera was.
            if (g_holdsApplied || g_gameInputBlocked)
                releaseAll();
            if (!enabled)
                g_seeded = false;
            g_haveMouse = false;
            g_rawDx.store(0, std::memory_order_relaxed);
            g_rawDy.store(0, std::memory_order_relaxed);
            g_smoothYaw = g_smoothPitch = 0.0f;
            return;
        }

        if (!g_seeded)
            seedFrom(gameTransform(view));

        readInput(dt);
        applyHolds();

#if defined(BFVE_GAME_BF3)
        const fb::LinearTransform t = fromAngles();
        reinterpret_cast<SetTransformFn>(OFF_RenderView_setTransform)(view, &t);
#else
        TransformBlock block{ };
        block.t = fromAngles();
        block.a = block.t;
        reinterpret_cast<SetTransformFn>(OFF_RenderView_setTransform)(view, &block);
#endif

        if (fovOverride > 0.0f)
        {
            const float rad = std::clamp(fovOverride, 1.0f, 179.0f) * math::DEG;
            reinterpret_cast<SetFovFn>(OFF_RenderView_setFov)(view, rad);
        }
    }

    void renderTab()
    {
        if (ImGui::Checkbox("free camera", &enabled))
        {
            g_seeded = false;
            firstPerson = false;
        }
        ImGui::SameLine();
        ImGui::TextDisabled("WASD, space/C up-down, shift boost");

        keyBinding("toggle free camera", &toggleKey);
        keyBinding("first person: hold, or tap to latch", &firstPersonKey);
        ImGui::SameLine();
        ImGui::Checkbox("latched", &firstPerson);

        ImGui::Checkbox("hide HUD", &hideHud);
        ImGui::SameLine();
        ImGui::TextDisabled(g_gameInputBlocked ? "soldier blocked, foreground off" : "soldier has input");

        ImGui::SetNextItemWidth(160.0f);
        ImGui::DragFloat("speed", &speed, 0.1f, 0.1f, 500.0f, "%.1f m/s");
        ImGui::SetNextItemWidth(160.0f);
        ImGui::DragFloat("boost", &boost, 0.1f, 1.0f, 20.0f, "x%.1f");
        ImGui::SetNextItemWidth(160.0f);
        ImGui::DragFloat("look", &sensitivity, 0.005f, 0.01f, 1.0f, "%.3f deg/count");
        ImGui::SameLine();
        ImGui::Checkbox("invert pitch", &invertPitch);
        ImGui::SetNextItemWidth(160.0f);
        ImGui::DragFloat("smoothing", &smoothing, 0.01f, 0.0f, 0.95f, smoothing > 0.0f ? "%.2f" : "off");
        ImGui::SetNextItemWidth(160.0f);
        ImGui::DragFloat("fov", &fovOverride, 0.5f, 0.0f, 170.0f, fovOverride > 0.0f ? "%.0f deg" : "game");

        ImGui::Separator();
        ImGui::SetNextItemWidth(240.0f);
        ImGui::DragFloat3("position", &position.m_x, 0.1f);
        ImGui::SetNextItemWidth(160.0f);
        ImGui::DragFloat("yaw", &yaw, 0.5f, -180.0f, 180.0f);
        ImGui::SetNextItemWidth(160.0f);
        ImGui::DragFloat("pitch", &pitch, 0.5f, -89.0f, 89.0f);

        if (ImGui::Button("reset to the game camera"))
            resetToGame();

        ImGui::Separator();
        ImGui::TextDisabled("writes params.view in GameRenderer::createUpdateJob");
    }
}
