#include "enlighten_internal.h"
#include "../serialize/serialize.h"
#include "enlighten_spawn.h"
#include "enlighten_db.h"
#include "enlighten_relight.h"
#include "enlighten_look.h"
#include <cstring>

namespace editor::enlighten
{
    using namespace detail;

    json serialize()
    {
        json j;
        json sps = json::array();
        for (const spawn::Spawned& sp : spawn::list())
        {
            const spawn::Request& r = sp.request;
            json e;
            e["name"] = r.name;
            e["spot"] = r.spot;
            e["type"] = int(r.type);
            e["width"] = r.width;
            e["enlighten"] = r.enlighten;
            e["flip"] = r.flipForward;
            e["pos"] = { r.pos.m_x, r.pos.m_y, r.pos.m_z };
            e["dir"] = { r.dir.m_x, r.dir.m_y, r.dir.m_z };
            e["color"] = { r.color.m_x, r.color.m_y, r.color.m_z };
            e["intensity"] = r.intensity;
            e["radius"] = r.radius;
            e["inner"] = r.innerAngle;
            e["outer"] = r.outerAngle;
            sps.push_back(e);
        }
        if (!sps.empty()) j["spawnedLights"] = sps;
        if (relight::enabled || relight::scale != 1.0f)
            j["relight"] = { { "enabled", relight::enabled }, { "scale", relight::scale } };
        if (!look::isDefault())
        {
            const look::Settings& s = look::settings;
            j["look"] = { { "keepShippedColor", s.keepShippedColor }, { "saturation", s.saturation },
                { "tint", { s.tint[0], s.tint[1], s.tint[2] } }, { "sky", s.skyStrength }, { "sun", s.sunStrength } };
        }
        if (db::calibrationStrength != 1.0f || db::outputScale != 1.0f)
            j["solverOutput"] = { { "calibrationStrength", db::calibrationStrength }, { "scale", db::outputScale } };
        if (db::engineSolver || !db::liveProbes)
            j["solver"] = { { "engineSolver", db::engineSolver }, { "liveProbes", db::liveProbes } };

        if (g_rt.captured && g_rt.enabled)
        {
            json r;
            const auto* e = reinterpret_cast<const fb::EnlightenRuntimeSettings*>(g_rt.edit);
            const auto* o = reinterpret_cast<const fb::EnlightenRuntimeSettings*>(g_rt.orig);
#define X(f) if (e->f != o->f) r[#f] = e->f;
#define XB(f, l) X(f)
            ENL_RT_FLOATS(X) ENL_RT_UINTS(X) ENL_RT_INTS(X) ENL_RT_BOOLS(XB)
#undef X
#undef XB
            if (std::memcmp(&e->m_AlbedoDefaultColor, &o->m_AlbedoDefaultColor, sizeof(fb::Vec3)) != 0)
                JSON_SET_VEC3(r, "m_AlbedoDefaultColor", e->m_AlbedoDefaultColor);
            j["runtime"] = r;
        }
        return j;
    }

    void deserialize(const json& j)
    {
        if (j.contains("relight") && j["relight"].is_object())
        {
            JSON_GET_BOOL(j["relight"], "enabled", relight::enabled);
            JSON_GET(j["relight"], "scale", relight::scale);
        }
        if (j.contains("look") && j["look"].is_object())
        {
            look::Settings& s = look::settings;
            const json& l = j["look"];
            JSON_GET_BOOL(l, "keepShippedColour", s.keepShippedColor); // key before 2026-09-30
            JSON_GET_BOOL(l, "keepShippedColor", s.keepShippedColor);
            JSON_GET(l, "saturation", s.saturation);
            if (l.contains("tint") && l["tint"].is_array() && l["tint"].size() >= 3)
                for (int c = 0; c < 3; ++c) s.tint[c] = l["tint"][c].get<float>();
            JSON_GET(l, "sky", s.skyStrength);
            JSON_GET(l, "sun", s.sunStrength);
            ++look::generation;
        }
        if (j.contains("solverOutput") && j["solverOutput"].is_object())
        {
            JSON_GET(j["solverOutput"], "calibrationStrength", db::calibrationStrength);
            JSON_GET(j["solverOutput"], "scale", db::outputScale);
        }
        if (j.contains("solver") && j["solver"].is_object())
        {
            JSON_GET_BOOL(j["solver"], "engineSolver", db::engineSolver);
            JSON_GET_BOOL(j["solver"], "liveProbes", db::liveProbes);
        }
        if (j.contains("spawnedLights") && j["spawnedLights"].is_array() && spawn::list().empty())
        {
            for (const json& e : j["spawnedLights"])
            {
                spawn::Request r;
                if (e.contains("name") && e["name"].is_string()) std::snprintf(r.name, sizeof(r.name), "%s", e["name"].get<std::string>().c_str());
                JSON_GET_BOOL(e, "spot", r.spot);
                JSON_GET_ENUM(e, "type", r.type, spawn::Type);
                if (!e.contains("type")) r.type = r.spot ? spawn::Type::Spot : spawn::Type::Point;
                JSON_GET(e, "width", r.width);
                JSON_GET_BOOL(e, "enlighten", r.enlighten);
                JSON_GET_BOOL(e, "flip", r.flipForward);
                const auto arr3 = [&](const char* k, fb::Vec3& out) { if (e.contains(k) && e[k].is_array() && e[k].size() >= 3) out = fb::vec3(e[k][0].get<float>(), e[k][1].get<float>(), e[k][2].get<float>()); };
                arr3("pos", r.pos); arr3("dir", r.dir); arr3("color", r.color);
                JSON_GET(e, "intensity", r.intensity);
                JSON_GET(e, "radius", r.radius);
                JSON_GET(e, "inner", r.innerAngle);
                JSON_GET(e, "outer", r.outerAngle);
                spawn::request(r);
            }
        }
        if (j.contains("runtime") && j["runtime"].is_object())
        {
            if (g_rt.captured) applyRuntimeJson(j["runtime"]);
            else g_pendingRuntime = j["runtime"];
        }
    }

    void detail::applyRuntimeJson(const json& r)
    {
        auto* e = reinterpret_cast<fb::EnlightenRuntimeSettings*>(g_rt.edit);
#define X(f) JSON_GET(r, #f, e->f);
#define XB(f, l) JSON_GET_BOOL(r, #f, e->f);
        ENL_RT_FLOATS(X) ENL_RT_UINTS(X) ENL_RT_INTS(X) ENL_RT_BOOLS(XB)
#undef X
#undef XB
        JSON_GET_VEC3(r, "m_AlbedoDefaultColor", e->m_AlbedoDefaultColor);
        g_rt.enabled = true;
    }
}
