#include "enlighten_spawn.h"
#include "../lights/lights.h"
#include "../render/render.h"
#include "../editor_context.h"
#include "../editor.h"
#include "../../SDK/fb.h"
#include "../../utils/log.h"

#include <Windows.h>
#include <imgui.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <mutex>

namespace editor::enlighten::spawn
{
    namespace
    {
        std::mutex g_mutex;
        std::vector<Request> g_queue;
        std::vector<size_t> g_destroyQueue;
        std::vector<Spawned> g_spawned;
        std::string g_error;

        // dtor queues removal, freed some updates later
        struct Retired
        {
            void* entity;
            void* data;
            int updates;
        };
        std::vector<Retired> g_retired;
        constexpr int RETIRE_UPDATES = 120;

        // ctor params at creator + 0x10: +0 bus, +8 id, +0x20 world transform
        // BF3 also reads +0xC, +0x14, +0x68
#if defined(BFVE_GAME_BF4)
        constexpr size_t PARAMS_SIZE = 96;
        constexpr size_t POINT_SIZE = 224;
        constexpr size_t SPOT_SIZE = 256;
        constexpr size_t POINT_DATA_SIZE = 0xD0;
        constexpr size_t SPOT_DATA_SIZE = 0x100;
#else
        constexpr size_t PARAMS_SIZE = 0xA0; // up to the data pointer at creator + 0xB0
        constexpr size_t POINT_SIZE = 0xA0;
        constexpr size_t SPOT_SIZE = 0xB0;
        constexpr size_t POINT_DATA_SIZE = sizeof(fb::PointLightEntityData);
        constexpr size_t SPOT_DATA_SIZE = sizeof(fb::SpotLightEntityData);
#endif
        struct Preview
        {
            int frame = -100;
            bool valid = false, spot = false;
            fb::Vec3 pos{}, dir{}, color{};
            float radius = 0.0f, outer = 0.0f;
        };
        std::mutex g_previewMutex;
        Preview g_preview;
        bool g_previewOn = true;

        alignas(16) uint8_t g_params[PARAMS_SIZE] = {};
        bool g_paramsSeen = false;
        bool g_destroying = false;
        bool g_selfCreating = false;
        constexpr uint32_t CLIENT_ID = 0x12121212; // ctor compares params + 8 against it

        fb::LinearTransform basis(const fb::Vec3& dir, bool flip, const fb::Vec3& pos)
        {
            fb::Vec3 f = fb::normalized(dir);
            if (fb::lengthSq(f) < 1e-12f) f = fb::vec3(0, -1, 0);
            return fb::lookTransform(flip ? -f : f, pos);
        }

        // 16 bytes slack in front for the instance GUID
        fb::LocalLightEntityData* newData(bool spot)
        {
            const size_t size = spot ? SPOT_DATA_SIZE : POINT_DATA_SIZE;
            auto* mem = static_cast<uint8_t*>(_aligned_malloc(size + 16, 16));
            if (!mem) return nullptr;
            std::memset(mem, 0, size + 16);
            uint8_t* data = mem + 16;
#if defined(BFVE_GAME_BF4)
            // point sub_140CD0ED0: LocalLightEntityData ctor + vtable + width 8 / 0.1
            // spot sub_140CD1180: SpotLightEntityData ctor
            using DataCtor = void* (__fastcall*)(void*);
            if (spot)
            {
                reinterpret_cast<DataCtor>(OFF_SpotLightEntityData_ctor)(data);
            }
            else
            {
                reinterpret_cast<DataCtor>(OFF_LocalLightEntityData_ctor)(data);
                *reinterpret_cast<uintptr_t*>(data) = OFF_vt_PointLightEntityData;
                *reinterpret_cast<uint64_t*>(data + 176) = 0;
                *reinterpret_cast<uint32_t*>(data + 184) = 0;
                *reinterpret_cast<float*>(data + 188) = 8.0f;
                *reinterpret_cast<float*>(data + 192) = 0.1f;
            }
#else
            // class default instance, ClassInfo +0x10
            auto* ci = reinterpret_cast<fb::ClassInfo*>(spot ? fb::SpotLightEntityData::ClassInfoPtr() : fb::PointLightEntityData::ClassInfoPtr());
            if (!ci->m_DefaultInstance) { _aligned_free(mem); return nullptr; }
            std::memcpy(data, ci->m_DefaultInstance, size);
            reinterpret_cast<fb::DataContainer*>(data)->m_flags &= ~0x100; // no instance GUID in front
#endif
            return reinterpret_cast<fb::LocalLightEntityData*>(data);
        }

        bool create(const Request& in)
        {
            Request r = in;
            r.spot = r.type == Type::Spot;
            fb::LocalLightEntityData* d = newData(r.spot);
            if (!d) { g_error = "no light data"; return false; }
            uint8_t* data = reinterpret_cast<uint8_t*>(d);
            static_cast<fb::DataContainer*>(d)->m_refCnt = 0x4000;
            d->m_Color = r.color;
            d->m_Intensity = r.intensity;
            d->m_Radius = r.radius;
            d->m_EnlightenEnable = r.enlighten;
            if (r.spot)
            {
                auto* sd = reinterpret_cast<fb::SpotLightEntityData*>(data);
                sd->m_ConeInnerAngle = r.innerAngle;
                sd->m_ConeOuterAngle = r.outerAngle;
            }
            else
            {
                reinterpret_cast<fb::PointLightEntityData*>(data)->m_Width = r.type == Type::Sphere ? r.width : 0.0f;
            }

            alignas(16) uint8_t params[PARAMS_SIZE];
            std::memcpy(params, g_params, sizeof(params));
            if (!g_paramsSeen)
            {
                std::memset(params, 0, sizeof(params));
                *reinterpret_cast<uint32_t*>(params + 8) = CLIENT_ID;
            }
            const fb::LinearTransform t = basis(r.dir, r.flipForward, r.pos);
            std::memcpy(params + 0x20, &t, 64);

            const size_t entSize = r.spot ? SPOT_SIZE : POINT_SIZE;
            auto* ent = static_cast<uint8_t*>(_aligned_malloc(entSize, 16));
            if (!ent) { g_error = "alloc"; _aligned_free(data - 16); return false; }
            std::memset(ent, 0, entSize);

            const uintptr_t ctor = r.spot ? OFF_SpotLightEntity_ctor : OFF_PointLightEntity_ctor;
            g_selfCreating = true;
#if defined(BFVE_GAME_BF4)
            reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(ctor)(ent, params, data);
#else
            reinterpret_cast<void* (__thiscall*)(void*, void*, void*)>(ctor)(ent, params, data);
#endif
            g_selfCreating = false;

            {
                auto& entries = lights::getEntries();
                auto it = entries.find(d);
                if (it != entries.end())
                {
                    it->second.assetName = std::string("spawned/") + r.name;
                    it->second.lightType = r.type == Type::Spot ? "SpotLight (spawned)" : r.type == Type::Sphere ? "SphereLight (spawned)" : "PointLight (spawned)";
                }
            }

            Spawned s;
            s.request = r;
            s.entity = ent;
            s.data = data;
            s.spotType = r.spot;
            g_spawned.push_back(s);
            logger::info("[enlighten] spawned {} light '{}' at ({:.1f} {:.1f} {:.1f}) entity {} data {} (params {})",
                r.spot ? "spot" : "point", r.name, r.pos.m_x, r.pos.m_y, r.pos.m_z, static_cast<void*>(ent), static_cast<void*>(data), g_paramsSeen ? "captured" : "default");
            return true;
        }

        void destroyOne(Spawned& s)
        {
            if (!s.entity) return;
            // vtable slot 8 = scalar deleting dtor, delete bit clear
            void** vt = *reinterpret_cast<void***>(s.entity);
#if defined(BFVE_GAME_BF4)
            reinterpret_cast<void* (__fastcall*)(void*, unsigned)>(vt[8])(s.entity, 0);
#else
            reinterpret_cast<void* (__thiscall*)(void*, unsigned)>(vt[8])(s.entity, 0);
#endif
            {
                std::lock_guard<std::recursive_mutex> guard(editor::lock());
                lights::forgetData(static_cast<fb::LocalLightEntityData*>(s.data));
            }
            g_retired.push_back({ s.entity, s.data, RETIRE_UPDATES });
            logger::info("[enlighten] destroyed spawned light '{}' (entity {})", s.request.name, s.entity);
            s.entity = nullptr;
            s.data = nullptr;
        }

        void tickRetired()
        {
            for (size_t i = 0; i < g_retired.size();)
            {
                if (--g_retired[i].updates > 0) { ++i; continue; }
                _aligned_free(g_retired[i].entity);
                if (g_retired[i].data) _aligned_free(static_cast<uint8_t*>(g_retired[i].data) - 16);
                g_retired.erase(g_retired.begin() + i);
            }
        }
    }

    void onCtorParams(const void* params)
    {
        if (g_paramsSeen || !params || g_destroying || g_selfCreating) return;
        std::memcpy(g_params, params, sizeof(g_params));
        g_paramsSeen = true;
        logger::info("[enlighten] light creation params captured: bus {} id {:#x}",
            *reinterpret_cast<void* const*>(g_params), *reinterpret_cast<const uint32_t*>(g_params + 8));
    }

    bool paramsCaptured() { return g_paramsSeen; }

    void request(const Request& r)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_queue.push_back(r);
    }

    size_t pendingCount()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        return g_queue.size();
    }

    const std::vector<Spawned>& list() { return g_spawned; }
    const std::string& lastError() { return g_error; }

    void requestDestroy(size_t index)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_destroyQueue.push_back(index);
    }

    void tickGameThread()
    {
        std::vector<Request> batch;
        std::vector<size_t> kill;
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            batch.swap(g_queue);
            kill.swap(g_destroyQueue);
        }
        tickRetired();
        std::sort(kill.rbegin(), kill.rend());
        kill.erase(std::unique(kill.begin(), kill.end()), kill.end());
        for (size_t i : kill) destroy(i);
        for (const Request& r : batch)
            if (!create(r))
                logger::warning("[enlighten] spawn failed: {}", g_error);
    }

    void destroy(size_t index)
    {
        if (index >= g_spawned.size()) return;
        destroyOne(g_spawned[index]);
        g_spawned.erase(g_spawned.begin() + index);
    }

    void destroyAll()
    {
        g_destroying = true;
        for (Spawned& s : g_spawned) destroyOne(s);
        g_spawned.clear();
        g_destroying = false;
        std::lock_guard<std::mutex> lock(g_mutex);
        g_queue.clear();
        g_destroyQueue.clear();
        g_paramsSeen = false;
    }
}

namespace editor::enlighten::spawn
{
    namespace
    {
        ImColor previewColor(const fb::Vec3& c, float alpha)
        {
            const float m = (std::max)({ c.m_x, c.m_y, c.m_z, 1e-3f });
            return ImColor(c.m_x / m, c.m_y / m, c.m_z / m, alpha);
        }

        void drawLight(const fb::Vec3& pos, const fb::Vec3& dir, bool spot, float radius, float outer, const ImColor& c)
        {
            const float s = 0.25f;
            render::line3(pos - fb::vec3(s, 0, 0), pos + fb::vec3(s, 0, 0), c, 2.0f);
            render::line3(pos - fb::vec3(0, s, 0), pos + fb::vec3(0, s, 0), c, 2.0f);
            render::line3(pos - fb::vec3(0, 0, s), pos + fb::vec3(0, 0, s), c, 2.0f);
            const float r = (std::min)(radius, 40.0f);
            if (!spot)
            {
                if (r > 0.0f) render::drawSphere(pos, r, 16, 8, ImColor(c.Value.x, c.Value.y, c.Value.z, c.Value.w * 0.5f), 1.0f);
                return;
            }
            fb::Vec3 f = fb::normalized(dir);
            if (fb::lengthSq(f) < 1e-8f) f = fb::vec3(0, -1, 0);
            const fb::Vec3 ref = std::fabs(f.m_y) < 0.9f ? fb::vec3(0, 1, 0) : fb::vec3(1, 0, 0);
            const fb::Vec3 u = fb::normalized(fb::cross(f, ref)), v = fb::cross(f, u);
            const float half = math::DEG2RAD((std::clamp)(outer, 1.0f, 179.0f) * 0.5f);
            const float len = (std::min)(r, 12.0f), rad = len * std::tan(half);
            const fb::Vec3 tip = pos + f * len;
            render::line3(pos, tip, c, 2.0f);
            fb::Vec3 prev{};
            for (int i = 0; i <= 16; ++i)
            {
                const float a = i * math::PI_2 / 16.0f;
                const fb::Vec3 p = tip + u * (std::cos(a) * rad) + v * (std::sin(a) * rad);
                if (i) render::line3(prev, p, c, 1.0f);
                if (i % 4 == 0) render::line3(pos, p, c, 1.0f);
                prev = p;
            }
        }
    }

    bool wantsAimRay() { return g_previewOn && ImGui::GetFrameCount() - g_preview.frame <= 2; }

    void renderOverlay()
    {
        Preview p;
        {
            std::lock_guard<std::mutex> lock(g_previewMutex);
            p = g_preview;
        }
        const bool depth = render::lineDepthTest;
        render::lineDepthTest = false;
        if (g_previewOn && p.valid && ImGui::GetFrameCount() - p.frame <= 2)
        {
            const ImColor c = previewColor(p.color, 0.9f);
            drawLight(p.pos, p.dir, p.spot, p.radius, p.outer, c);
            ImVec2 sp;
            if (render::worldToScreen(p.pos, sp)) render::label(ImVec2(sp.x + 8.0f, sp.y - 8.0f), "spawn here", c);
        }
        render::lineDepthTest = depth;
    }

    void renderUI()
    {
        {
            static spawn::Request req;
            ImGui::TextDisabled("%zu spawned, %zu pending, creation params %s", spawn::list().size(), spawn::pendingCount(),
                spawn::paramsCaptured() ? "captured" : "not seen yet (default bus)");
            ImGui::SetNextItemWidth(120.0f);
            ImGui::InputText("name##sp", req.name, sizeof(req.name));
            ImGui::SameLine();
            static const char* kTypes[] = { "point light", "sphere light", "spot light" };
            int ty = int(req.type);
            ImGui::SetNextItemWidth(120.0f);
            if (ImGui::Combo("##type", &ty, kTypes, 3)) req.type = spawn::Type(ty);
            req.spot = req.type == spawn::Type::Spot;
            if (req.type == spawn::Type::Sphere)
                ImGui::SliderFloat("sphere radius##sp", &req.width, 0.05f, 5.0f, "%.2f m");
            ImGui::SameLine();
            ImGui::Checkbox("Enlighten##sp", &req.enlighten);
            ImGui::ColorEdit3("color##sp", &req.color.m_x, ImGuiColorEditFlags_Float);
            ImGui::SliderFloat("intensity##sp", &req.intensity, 0.01f, 1000.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
            ImGui::SliderFloat("radius##sp", &req.radius, 0.5f, 100.0f, "%.1f m");
            if (req.spot)
            {
                ImGui::SliderFloat("inner##sp", &req.innerAngle, 0.0f, 179.0f, "%.0f");
                ImGui::SliderFloat("outer##sp", &req.outerAngle, 1.0f, 180.0f, "%.0f");
                ImGui::Checkbox("points along -forward", &req.flipForward);
            }
            ImGui::Checkbox("preview at aim", &g_previewOn);
            {
                Preview p;
                p.frame = ImGui::GetFrameCount();
                lights::AimRay ray;
                if (lights::lastAimRay(ray) && ray.valid)
                {
                    p.valid = true;
                    p.pos = ray.hit + ray.normal * 0.3f;
                    p.dir = -ray.normal;
                }
                p.spot = req.type == spawn::Type::Spot;
                p.color = req.color;
                p.radius = req.radius;
                p.outer = req.outerAngle;
                std::lock_guard<std::mutex> lock(g_previewMutex);
                g_preview = p;
            }
            if (ImGui::Button("spawn at camera"))
            {
                fb::Vec3 cam{}, fwd{};
                if (render::cameraPosition(cam) && render::cameraForward(fwd))
                {
                    req.pos = cam;
                    req.dir = fwd;
                    spawn::request(req);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("spawn at aim"))
            {
                lights::aimRayRequested = true;
                lights::AimRay ray;
                if (lights::lastAimRay(ray) && ray.valid)
                {
                    req.pos = ray.hit + ray.normal * 0.3f;
                    req.dir = -ray.normal;
                    spawn::request(req);
                }
            }
            if (!spawn::lastError().empty())
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.3f, 1.0f), "%s", spawn::lastError().c_str());
            const auto& sp = spawn::list();
            for (size_t i = 0; i < sp.size(); ++i)
            {
                ImGui::PushID(int(i) + 5000);
                ImGui::BulletText("%s (%s) at %.1f %.1f %.1f", sp[i].request.name, sp[i].request.type == spawn::Type::Spot ? "spot" : sp[i].request.type == spawn::Type::Sphere ? "sphere" : "point",
                    sp[i].request.pos.m_x, sp[i].request.pos.m_y, sp[i].request.pos.m_z);
                ImGui::SameLine();
                if (ImGui::SmallButton("destroy"))
                    spawn::requestDestroy(i);
                ImGui::PopID();
            }
            if (!sp.empty())
                ImGui::TextDisabled("edit them like any light in the Lights tab; destroyed on level unload");
        }

    }
}
