#include "global_ve.h"
#include "../copy/copy.h"

// colors from MP_Abandoned_Night/Lighting/VE_MP_Abandon_Night
namespace editor::global_ve
{
    namespace
    {
        fb::Vec3 scaled(const fb::Vec3& v, float s) { return fb::vec3(v.m_x * s, v.m_y * s, v.m_z * s); }
    }

    void applyNightPreset()
    {
        GlobalVEData& d = getData();
        if (!d.captured)
            return;
        copy::outdoorLight(&d.editOutdoorLight, &d.origOutdoorLight);
        d.editOutdoorLight.m_SunColor = fb::vec3(0.016f, 0.032f, 0.066f);
        d.editOutdoorLight.m_SkyColor = fb::vec3(0.005f, 0.005f, 0.016f);
        d.editOutdoorLight.m_GroundColor = fb::vec3(0.05f, 0.05f, 0.05f);
        d.editOutdoorLight.m_SkyEnvmapShadowScale = 0.25f;
        d.outdoorLightOverrideEnabled = true;

        copy::enlighten(&d.editEnlighten, &d.origEnlighten);
        d.editEnlighten.m_SkyBoxSkyColor = fb::vec3(0.004f, 0.006f, 0.013f);
        d.editEnlighten.m_SkyBoxGroundColor = fb::vec3(0.003f, 0.003f, 0.003f);
        d.editEnlighten.m_SkyBoxSunLightColor = fb::vec3(0.021f, 0.045f, 0.066f);
        d.editEnlighten.m_SkyBoxBackLightColor = fb::vec3(0.004f, 0.016f, 0.02f);
        d.editEnlighten.m_BounceScale = 1.0f;
        d.editEnlighten.m_SunScale = 1.0f;
        d.enlightenOverrideEnabled = true;

        copy::sky(&d.editSky, &d.origSky);
        d.editSky.m_BrightnessScale = d.origSky.m_BrightnessScale * 0.1f; // Zavod 5.0 -> 0.5
        d.editSky.m_SunSize = 0.0f;
        d.editSky.m_CloudLayerSunColor = scaled(d.origSky.m_CloudLayerSunColor, 0.1f);
        d.editSky.m_CloudLayer1Color = scaled(d.origSky.m_CloudLayer1Color, 0.1f);
        d.editSky.m_CloudLayer2Color = scaled(d.origSky.m_CloudLayer2Color, 0.1f);
        d.editSky.m_StaticEnvmapScale = d.origSky.m_StaticEnvmapScale * 0.05f;
        d.editSky.m_CustomEnvmapScale = d.origSky.m_CustomEnvmapScale * 0.05f;
        d.editSky.m_CustomEnvmapAmbient = 0.0f;
        d.skyOverrideEnabled = true;

        copy::fog(&d.editFog, &d.origFog);
        d.editFog.m_FogColor = fb::vec3(0.01f, 0.015f, 0.017f);
#if defined(BFVE_GAME_BF4)
        d.editFog.m_ForwardLightScatteringColor = scaled(d.origFog.m_ForwardLightScatteringColor, 0.1f);
        d.editFog.m_ForwardLightScatteringEnabled = false;
#endif
        d.fogOverrideEnabled = true;

        copy::tonemap(&d.editTonemap, &d.origTonemap);
        d.editTonemap.m_BloomScale = fb::vec3(0.235f, 0.203f, 0.144f);
        d.editTonemap.m_MiddleGray = 0.1f;
        d.editTonemap.m_MinExposure = 0.1f;
        d.editTonemap.m_MaxExposure = 3.0f;
        d.editTonemap.m_ExposureAdjustTime = 0.5f;
        d.tonemapOverrideEnabled = true;

        copy::colorCorrection(&d.editColorCorrection, &d.origColorCorrection);
        d.editColorCorrection.m_Contrast = fb::vec3(1.12f, 1.12f, 1.12f);
        d.editColorCorrection.m_Saturation = fb::vec3(0.8f, 0.8f, 0.8f);
        d.colorCorrectionOverrideEnabled = true;

        copy::sunFlare(&d.editSunFlare, &d.origSunFlare);
        d.editSunFlare.m_Enable = false;
        d.sunFlareOverrideEnabled = true;

        copy::characterLighting(&d.editCharacterLighting, &d.origCharacterLighting);
        d.editCharacterLighting.m_CharacterLightEnable = false;
        d.characterLightingOverrideEnabled = true;

        copy::vignette(&d.editVignette, &d.origVignette);
        d.editVignette.m_Color = fb::vec3(0.096f, 0.168f, 0.231f);
        d.editVignette.m_Opacity = 0.232f;
        d.editVignette.m_Exponent = 2.0f;
        d.editVignette.m_Enable = true;
        d.vignetteOverrideEnabled = true;

        d.globalOverrideEnabled = true;
    }
}
