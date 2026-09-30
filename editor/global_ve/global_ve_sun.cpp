#include "global_ve.h"
#include "../lights/lights.h"
#include <imgui.h>
#include <cmath>

// disc = SunColor * Sky.SunScale, SkyRenderModule::draw 0x140CE3220
namespace editor::global_ve::sun
{
    namespace
    {
        size_t g_followed = 0;
    }

    Settings& settings()
    {
        static Settings s;
        return s;
    }

    bool disc(float* rgb, float& size)
    {
        const Settings& s = settings();
        if (!s.discColorEnabled) return false;
        for (int c = 0; c < 3; ++c) rgb[c] = s.discColor[c] * s.discIntensity;
        size = s.discSizeEnabled ? s.discSize : -1.0f;
        return true;
    }

    // as VisualEnvironmentManager::updateSunFlare
    fb::Vec3 direction(float rotationX, float rotationY)
    {
        const float x = math::DEG2RAD(rotationX), y = math::DEG2RAD(rotationY);
        return fb::vec3(std::sin(x) * std::cos(y), std::sin(y), std::cos(x) * std::cos(y));
    }

    void onUpdated(fb::VisualEnvironment* ve)
    {
        if (!ve) return;
        if (settings().flaresFollow)
        {
            const fb::Vec3 d = direction(ve->outdoorLight.m_SunRotationX, ve->outdoorLight.m_SunRotationY);
            g_followed = lights::followSunFlares(&d);
        }
        else if (g_followed)
            g_followed = lights::followSunFlares(nullptr);
    }

    void clear()
    {
        g_followed = 0;
    }

    void renderUI()
    {
        Settings& s = settings();
        ImGui::Checkbox("level sun flares follow the sun", &s.flaresFollow);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("lens flares placed far away in the level (a painted sun) are turned to the sun rotation");
        if (s.flaresFollow)
        {
            ImGui::SameLine();
            ImGui::TextDisabled("%zu flare(s)", g_followed);
        }
        ImGui::Checkbox("own sun disc color", &s.discColorEnabled);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("replaces sun light color x sky sun scale, for the disc only");
        if (s.discColorEnabled)
        {
            ImGui::ColorEdit3("disc color", s.discColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
            ImGui::SliderFloat("disc brightness", &s.discIntensity, 0.0f, 50.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
            ImGui::Checkbox("disc size", &s.discSizeEnabled);
            if (s.discSizeEnabled)
            {
                ImGui::SameLine();
                ImGui::SliderFloat("##discsize", &s.discSize, 0.0f, 0.05f, "%.4f", ImGuiSliderFlags_Logarithmic);
            }
        }
    }

    json serialize()
    {
        const Settings& s = settings();
        json j;
        j["flaresFollow"] = s.flaresFollow;
        j["discColorEnabled"] = s.discColorEnabled;
        j["discColor"] = { s.discColor[0], s.discColor[1], s.discColor[2] };
        j["discIntensity"] = s.discIntensity;
        j["discSizeEnabled"] = s.discSizeEnabled;
        j["discSize"] = s.discSize;
        return j;
    }

    void deserialize(const json& j)
    {
        Settings& s = settings();
        s.flaresFollow = j.value("flaresFollow", false);
        // keys before 2026-09-30: discColourEnabled, discColour
        s.discColorEnabled = j.value("discColorEnabled", j.value("discColourEnabled", false));
        const char* discKey = j.contains("discColor") ? "discColor" : "discColour";
        if (j.contains(discKey) && j[discKey].is_array() && j[discKey].size() >= 3)
            for (int c = 0; c < 3; ++c) s.discColor[c] = j[discKey][c].get<float>();
        s.discIntensity = j.value("discIntensity", s.discIntensity);
        s.discSizeEnabled = j.value("discSizeEnabled", false);
        s.discSize = j.value("discSize", s.discSize);
    }
}
