#include "global_ve.h"

#include "../comps/comps_serialize.h"

namespace editor::global_ve
{
    json serialize()
    {
        json j;
        auto& d = getData();
        j["globalOverrideEnabled"] = d.globalOverrideEnabled;

#define SAVE_COMPONENT(name, enabled, edit) \
        if (d.enabled) { \
            j[#name]["enabled"] = true; \
            j[#name]["data"] = comps::serialize##name(d.edit); \
        }

        SAVE_COMPONENT(OutdoorLight, outdoorLightOverrideEnabled, editOutdoorLight);
        SAVE_COMPONENT(Enlighten, enlightenOverrideEnabled, editEnlighten);
        SAVE_COMPONENT(Tonemap, tonemapOverrideEnabled, editTonemap);
        SAVE_COMPONENT(ColorCorrection, colorCorrectionOverrideEnabled, editColorCorrection);
        SAVE_COMPONENT(Sky, skyOverrideEnabled, editSky);
        SAVE_COMPONENT(Fog, fogOverrideEnabled, editFog);
        SAVE_COMPONENT(Wind, windOverrideEnabled, editWind);
        SAVE_COMPONENT(SunFlare, sunFlareOverrideEnabled, editSunFlare);
        SAVE_COMPONENT(DynamicAO, dynamicAOOverrideEnabled, editDynamicAO);
        SAVE_COMPONENT(Dof, dofOverrideEnabled, editDof);
        SAVE_COMPONENT(Vignette, vignetteOverrideEnabled, editVignette);
        SAVE_COMPONENT(FilmGrain, filmGrainOverrideEnabled, editFilmGrain);
        SAVE_COMPONENT(LensScope, lensScopeOverrideEnabled, editLensScope);
        SAVE_COMPONENT(CameraParams, cameraParamsOverrideEnabled, editCameraParams);
        SAVE_COMPONENT(ScreenEffect, screenEffectOverrideEnabled, editScreenEffect);
        SAVE_COMPONENT(DamageEffect, damageEffectOverrideEnabled, editDamageEffect);
        SAVE_COMPONENT(PlanarReflection, planarReflectionOverrideEnabled, editPlanarReflection);
        SAVE_COMPONENT(DynamicEnvmap, dynamicEnvmapOverrideEnabled, editDynamicEnvmap);
        SAVE_COMPONENT(CharacterLighting, characterLightingOverrideEnabled, editCharacterLighting);
        SAVE_COMPONENT(MotionBlur, motionBlurOverrideEnabled, editMotionBlur);

#undef SAVE_COMPONENT

        return j;
    }

    void deserialize(const json& j)
    {
        auto& d = getData();
        d.globalOverrideEnabled = j.value("globalOverrideEnabled", false);

#define LOAD_COMPONENT(name, enabled, edit) \
        if (j.contains(#name) && j[#name].value("enabled", false)) { \
            d.enabled = true; \
            if (j[#name].contains("data")) \
                comps::deserialize##name(j[#name]["data"], d.edit); \
        }

        LOAD_COMPONENT(OutdoorLight, outdoorLightOverrideEnabled, editOutdoorLight);
        LOAD_COMPONENT(Enlighten, enlightenOverrideEnabled, editEnlighten);
        LOAD_COMPONENT(Tonemap, tonemapOverrideEnabled, editTonemap);
        LOAD_COMPONENT(ColorCorrection, colorCorrectionOverrideEnabled, editColorCorrection);
        LOAD_COMPONENT(Sky, skyOverrideEnabled, editSky);
        LOAD_COMPONENT(Fog, fogOverrideEnabled, editFog);
        LOAD_COMPONENT(Wind, windOverrideEnabled, editWind);
        LOAD_COMPONENT(SunFlare, sunFlareOverrideEnabled, editSunFlare);
        LOAD_COMPONENT(DynamicAO, dynamicAOOverrideEnabled, editDynamicAO);
        LOAD_COMPONENT(Dof, dofOverrideEnabled, editDof);
        LOAD_COMPONENT(Vignette, vignetteOverrideEnabled, editVignette);
        LOAD_COMPONENT(FilmGrain, filmGrainOverrideEnabled, editFilmGrain);
        LOAD_COMPONENT(LensScope, lensScopeOverrideEnabled, editLensScope);
        LOAD_COMPONENT(CameraParams, cameraParamsOverrideEnabled, editCameraParams);
        LOAD_COMPONENT(ScreenEffect, screenEffectOverrideEnabled, editScreenEffect);
        LOAD_COMPONENT(DamageEffect, damageEffectOverrideEnabled, editDamageEffect);
        LOAD_COMPONENT(PlanarReflection, planarReflectionOverrideEnabled, editPlanarReflection);
        LOAD_COMPONENT(DynamicEnvmap, dynamicEnvmapOverrideEnabled, editDynamicEnvmap);
        LOAD_COMPONENT(CharacterLighting, characterLightingOverrideEnabled, editCharacterLighting);
        LOAD_COMPONENT(MotionBlur, motionBlurOverrideEnabled, editMotionBlur);

#undef LOAD_COMPONENT
    }
}