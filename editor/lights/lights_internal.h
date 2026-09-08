#pragma once

#include "../types.h"
#include <cstdint>
#include <cstring>
#include <vector>

namespace editor::lights::detail
{
    // Bounded copy of an engine string into a UI-sized buffer.
    inline bool copyEngineString(const char* src, char* out, size_t cap)
    {
        out[0] = '\0';
        if (!src || cap < 2)
            return false;
        strncpy_s(out, cap, src, _TRUNCATE);
        return out[0] != '\0';
    }

    inline float distanceSqr(const fb::Vec3& a, const fb::Vec3& b)
    {
        const float dx = a.m_x - b.m_x, dy = a.m_y - b.m_y, dz = a.m_z - b.m_z;
        return dx * dx + dy * dy + dz * dz;
    }

    struct DriverVec { float x, y, z, w; };
}

namespace editor::lights
{
    // lights_flares.cpp
    void clearFlarePointers(); // claims, palette, list - on level change
    void linkFlaresByProximity(); // BF3: pair realized flares with lights by distance

#if defined(BFVE_GAME_BF4)
    void collectHeads(uintptr_t classInfo, uint16_t expectedClassId, std::vector<void*>& out);
#endif
}
