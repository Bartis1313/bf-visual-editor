#include "serialize.h"

namespace editor::serialize
{
    json vec2ToJson(const fb::Vec2& v)
    {
        return { {"x", v.m_x}, {"y", v.m_y} };
    }

    json vec3ToJson(const fb::Vec3& v)
    {
        return { {"x", v.m_x}, {"y", v.m_y}, {"z", v.m_z} };
    }

    json vec4ToJson(const fb::Vec4& v)
    {
        return { {"x", v.m_x}, {"y", v.m_y}, {"z", v.m_z}, {"w", v.m_w} };
    }

    void jsonToVec2(const json& j, fb::Vec2& v)
    {
        JSON_GET(j, "x", v.m_x);
        JSON_GET(j, "y", v.m_y);
    }

    void jsonToVec3(const json& j, fb::Vec3& v)
    {
        JSON_GET(j, "x", v.m_x);
        JSON_GET(j, "y", v.m_y);
        JSON_GET(j, "z", v.m_z);
    }

    void jsonToVec4(const json& j, fb::Vec4& v)
    {
        JSON_GET(j, "x", v.m_x);
        JSON_GET(j, "y", v.m_y);
        JSON_GET(j, "z", v.m_z);
        JSON_GET(j, "w", v.m_w);
    }
}