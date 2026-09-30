#pragma once

#include "../types.h"
#include "../../SDK/fb.h"
#include <cstdint>
#include <string>
#include <vector>

// PointLightEntity 0xE0, SpotLightEntity 0x100, engine ctors
// registered with light manager qword_14273D898
namespace editor::enlighten::spawn
{
    enum class Type : int { Point = 0, Sphere = 1, Spot = 2 };

    struct Request
    {
        Type type = Type::Point;
        bool spot = false;
        float width = 0.5f; // PointLightEntityData::Width
        fb::Vec3 pos = fb::vec3(0, 0, 0);
        fb::Vec3 dir = fb::vec3(0, -1, 0);
        fb::Vec3 color = fb::vec3(1, 1, 1);
        float intensity = 5.0f;
        float radius = 8.0f;
        float innerAngle = 30.0f, outerAngle = 60.0f;
        bool enlighten = true;
        bool flipForward = true;
        char name[32] = "spawned";
    };

    struct Spawned
    {
        Request request;
        void* entity = nullptr; // fb::LocalLightEntity*
        void* data = nullptr; // LocalLightEntityData, ours
        bool spotType = false;
    };

    void onCtorParams(const void* params); // ctor hook
    bool paramsCaptured();

    void request(const Request& r); // any thread, created on next VE update
    void tickGameThread(); // VE update thread
    void destroy(size_t index); // VE update thread
    void requestDestroy(size_t index); // any thread
    void destroyAll(); // level unload
    const std::vector<Spawned>& list();
    size_t pendingCount();
    const std::string& lastError();
    void renderUI();
    void renderOverlay();
    bool wantsAimRay(); // render thread
}
