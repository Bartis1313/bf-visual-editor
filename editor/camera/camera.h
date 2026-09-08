#pragma once

#include "../../SDK/fb.h"

#include <Windows.h>

namespace editor::camera
{
    inline bool enabled = false;
    inline float speed = 8.0f; // m/s
    inline float boost = 4.0f; // held sprint / shift
    inline float lookSpeed = 200.0f; // engine cache path: rad/s per unit of concept level
    inline float sensitivity = 0.12f; // cursor/raw path: degrees per mouse count
    inline bool invertPitch = false;
    inline float smoothing = 0.0f; // 0 = off, up to 0.95 = per-frame EMA on the turn
    inline float fovOverride = 0.0f; // vertical degrees, 0 = game's

    inline bool hideHud = false; // UISettings::drawEnable

    inline int toggleKey = VK_F9;
    inline int firstPersonKey = VK_F10;
    inline bool firstPerson = false; // the tap latch

    inline fb::Vec3 position{ };
    inline float yaw = 0.0f; // degrees
    inline float pitch = 0.0f;

    // From the GameRenderer::createUpdateJob hook, before the original: the main view of the
    // frame's params. Reads input, seeds from the game camera on enable, writes the view.
    void onRenderViewParams(fb::RenderView* view);
    // BF4: after ClientCameraManager::getTransform(out, viewIndex)
    void onGameCameraTransform(fb::LinearTransform* out, int viewIndex);

    // Seed from the game camera again on the next frame.
    void resetToGame();

    // True while the soldier is blocked with the free camera showing: the window proc answers
    // WM_SETCURSOR with no cursor then.
    bool hidesCursor();
    // From the window proc on WM_INPUT: relative mouse counts, used by InputSource::Raw.
    void onRawMouse(long dx, long dy);

    void renderTab();
}
