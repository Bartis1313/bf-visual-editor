#pragma once

#include "../../SDK/fb.h"

#include <Windows.h>

namespace editor::camera
{
    inline bool enabled = false;
    inline float speed = 8.0f; // m/s
    inline float boost = 4.0f; // held sprint / shift
    inline float lookSpeed = 200.0f; // rad/s per concept level unit
    inline float sensitivity = 0.12f; // cursor/raw path: degrees per mouse count
    inline bool invertPitch = false;
    inline float smoothing = 0.0f; // per-frame EMA, 0 = off, max 0.95
    inline float fovOverride = 0.0f; // vertical degrees, 0 = game's

    inline bool hideHud = false; // UISettings::drawEnable

    inline int toggleKey = VK_F9;
    inline int firstPersonKey = VK_F10;
    inline bool firstPerson = false; // the tap latch

    inline fb::Vec3 position{ };
    inline float yaw = 0.0f; // degrees
    inline float pitch = 0.0f;

    // GameRenderer::createUpdateJob pre-hook
    void onRenderViewParams(fb::RenderView* view);
    // BF4: after ClientCameraManager::getTransform(out, viewIndex)
    void onGameCameraTransform(fb::LinearTransform* out, int viewIndex);

    void resetToGame();

    // free camera on with the soldier blocked
    bool hidesCursor();
    // WM_INPUT relative mouse counts
    void onRawMouse(long dx, long dy);

    void renderTab();
}
