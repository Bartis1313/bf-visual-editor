#pragma once

#include <nlohmann/json.hpp>
#include "../../SDK/sdk.h"

using json = nlohmann::json;

namespace editor::serialize
{
    json vec2ToJson(const fb::Vec2& v);
    json vec3ToJson(const fb::Vec3& v);
    json vec4ToJson(const fb::Vec4& v);
    void jsonToVec2(const json& j, fb::Vec2& v);
    void jsonToVec3(const json& j, fb::Vec3& v);
    void jsonToVec4(const json& j, fb::Vec4& v);
}

#define JSON_SET(j, key, val) j[key] = val
#define JSON_SET_VEC2(j, key, v) j[key] = editor::serialize::vec2ToJson(v)
#define JSON_SET_VEC3(j, key, v) j[key] = editor::serialize::vec3ToJson(v)
#define JSON_SET_VEC4(j, key, v) j[key] = editor::serialize::vec4ToJson(v)

#define JSON_GET(j, key, var) if ((j).contains(key) && (j)[key].is_number()) var = (j)[key].get<decltype(var)>()
#define JSON_GET_BOOL(j, key, var) if ((j).contains(key) && (j)[key].is_boolean()) var = (j)[key].get<bool>()
#define JSON_GET_INT(j, key, var) if ((j).contains(key) && (j)[key].is_number_integer()) var = (j)[key].get<int>()
#define JSON_GET_ENUM(j, key, var, type) if ((j).contains(key) && (j)[key].is_number_integer()) var = static_cast<type>((j)[key].get<int>())
#define JSON_GET_VEC2(j, key, var) if ((j).contains(key)) editor::serialize::jsonToVec2((j)[key], var)
#define JSON_GET_VEC3(j, key, var) if ((j).contains(key)) editor::serialize::jsonToVec3((j)[key], var)
#define JSON_GET_VEC4(j, key, var) if ((j).contains(key)) editor::serialize::jsonToVec4((j)[key], var)