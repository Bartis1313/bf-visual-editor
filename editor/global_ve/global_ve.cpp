#include "global_ve.h"
#include "../editor_context.h"
#include "../copy/copy.h"

namespace editor::global_ve
{
    GlobalVEData data;

    GlobalVEData& getData() { return data; }

    void init()
    {
        clear();
    }

    void shutdown()
    {
        clear();
    }

    void clear()
    {
        data.Reset();
    }

    bool hasCaptured()
    {
        return data.captured;
    }

    bool isEnabled()
    {
        return data.globalOverrideEnabled;
    }

    int enabledCount()
    {
        return data.EnabledOverrideCount();
    }

    void capture(fb::VisualEnvironment* ve)
    {
        if (!ve || data.captured)
            return;

        if (levelUnloadingSignaled || !isSafeToOperate())
            return;

        copy::outdoorLight(&data.origOutdoorLight, &ve->outdoorLight);
        copy::outdoorLight(&data.editOutdoorLight, &ve->outdoorLight);

        copy::enlighten(&data.origEnlighten, &ve->enlighten);
        copy::enlighten(&data.editEnlighten, &ve->enlighten);

        copy::tonemap(&data.origTonemap, &ve->tonemap);
        copy::tonemap(&data.editTonemap, &ve->tonemap);

        copy::colorCorrection(&data.origColorCorrection, &ve->colorCorrection);
        copy::colorCorrection(&data.editColorCorrection, &ve->colorCorrection);

        copy::sky(&data.origSky, &ve->sky);
        copy::sky(&data.editSky, &ve->sky);

        copy::fog(&data.origFog, &ve->fog);
        copy::fog(&data.editFog, &ve->fog);

        copy::wind(&data.origWind, &ve->wind);
        copy::wind(&data.editWind, &ve->wind);

        copy::sunFlare(&data.origSunFlare, &ve->sunFlare);
        copy::sunFlare(&data.editSunFlare, &ve->sunFlare);

        copy::dynamicAO(&data.origDynamicAO, &ve->dynamicAO);
        copy::dynamicAO(&data.editDynamicAO, &ve->dynamicAO);

        copy::dof(&data.origDof, &ve->dof);
        copy::dof(&data.editDof, &ve->dof);

        copy::vignette(&data.origVignette, &ve->vignette);
        copy::vignette(&data.editVignette, &ve->vignette);

        copy::filmGrain(&data.origFilmGrain, &ve->filmGrain);
        copy::filmGrain(&data.editFilmGrain, &ve->filmGrain);

        copy::lensScope(&data.origLensScope, &ve->lensScope);
        copy::lensScope(&data.editLensScope, &ve->lensScope);

        copy::cameraParams(&data.origCameraParams, &ve->cameraParams);
        copy::cameraParams(&data.editCameraParams, &ve->cameraParams);

        copy::screenEffect(&data.origScreenEffect, &ve->screenEffect);
        copy::screenEffect(&data.editScreenEffect, &ve->screenEffect);

        copy::damageEffect(&data.origDamageEffect, &ve->damageEffect);
        copy::damageEffect(&data.editDamageEffect, &ve->damageEffect);

        copy::planarReflection(&data.origPlanarReflection, &ve->planarReflection);
        copy::planarReflection(&data.editPlanarReflection, &ve->planarReflection);

        copy::dynamicEnvmap(&data.origDynamicEnvmap, &ve->dynamicEnvmap);
        copy::dynamicEnvmap(&data.editDynamicEnvmap, &ve->dynamicEnvmap);

        copy::characterLighting(&data.origCharacterLighting, &ve->characterLighting);
        copy::characterLighting(&data.editCharacterLighting, &ve->characterLighting);

        copy::motionBlur(&data.origMotionBlur, &ve->motionBlur);
        copy::motionBlur(&data.editMotionBlur, &ve->motionBlur);

        data.captured = true;
    }

    void apply(fb::VisualEnvironment* ve)
    {
        if (!ve || !data.captured || !data.globalOverrideEnabled)
            return;

        if (data.outdoorLightOverrideEnabled)
            copy::outdoorLight(&ve->outdoorLight, &data.editOutdoorLight);

        if (data.enlightenOverrideEnabled)
            copy::enlighten(&ve->enlighten, &data.editEnlighten);

        if (data.tonemapOverrideEnabled)
            copy::tonemap(&ve->tonemap, &data.editTonemap);

        if (data.colorCorrectionOverrideEnabled)
            copy::colorCorrection(&ve->colorCorrection, &data.editColorCorrection);

        if (data.skyOverrideEnabled)
            copy::sky(&ve->sky, &data.editSky);

        if (data.fogOverrideEnabled)
            copy::fog(&ve->fog, &data.editFog);

        if (data.windOverrideEnabled)
            copy::wind(&ve->wind, &data.editWind);

        if (data.sunFlareOverrideEnabled)
            copy::sunFlare(&ve->sunFlare, &data.editSunFlare);

        if (data.dynamicAOOverrideEnabled)
            copy::dynamicAO(&ve->dynamicAO, &data.editDynamicAO);

        if (data.dofOverrideEnabled)
            copy::dof(&ve->dof, &data.editDof);

        if (data.vignetteOverrideEnabled)
            copy::vignette(&ve->vignette, &data.editVignette);

        if (data.filmGrainOverrideEnabled)
            copy::filmGrain(&ve->filmGrain, &data.editFilmGrain);

        if (data.lensScopeOverrideEnabled)
            copy::lensScope(&ve->lensScope, &data.editLensScope);

        if (data.cameraParamsOverrideEnabled)
            copy::cameraParams(&ve->cameraParams, &data.editCameraParams);

        if (data.screenEffectOverrideEnabled)
            copy::screenEffect(&ve->screenEffect, &data.editScreenEffect);

        if (data.damageEffectOverrideEnabled)
            copy::damageEffect(&ve->damageEffect, &data.editDamageEffect);

        if (data.planarReflectionOverrideEnabled)
            copy::planarReflection(&ve->planarReflection, &data.editPlanarReflection);

        if (data.dynamicEnvmapOverrideEnabled)
            copy::dynamicEnvmap(&ve->dynamicEnvmap, &data.editDynamicEnvmap);

        if (data.characterLightingOverrideEnabled)
            copy::characterLighting(&ve->characterLighting, &data.editCharacterLighting);

        if (data.motionBlurOverrideEnabled)
            copy::motionBlur(&ve->motionBlur, &data.editMotionBlur);
    }

    void onUpdated(fb::VisualEnvironment* ve)
    {
        if (!ve)
            return;

        if (!data.captured)
            capture(ve);

        if (data.captured && data.globalOverrideEnabled)
            apply(ve);
    }

    void resetToOriginal()
    {
        if (!data.captured)
            return;

        copy::outdoorLight(&data.editOutdoorLight, &data.origOutdoorLight);
        copy::enlighten(&data.editEnlighten, &data.origEnlighten);
        copy::tonemap(&data.editTonemap, &data.origTonemap);
        copy::colorCorrection(&data.editColorCorrection, &data.origColorCorrection);
        copy::sky(&data.editSky, &data.origSky);
        copy::fog(&data.editFog, &data.origFog);
        copy::wind(&data.editWind, &data.origWind);
        copy::sunFlare(&data.editSunFlare, &data.origSunFlare);
        copy::dynamicAO(&data.editDynamicAO, &data.origDynamicAO);
        copy::dof(&data.editDof, &data.origDof);
        copy::vignette(&data.editVignette, &data.origVignette);
        copy::filmGrain(&data.editFilmGrain, &data.origFilmGrain);
        copy::lensScope(&data.editLensScope, &data.origLensScope);
        copy::cameraParams(&data.editCameraParams, &data.origCameraParams);
        copy::screenEffect(&data.editScreenEffect, &data.origScreenEffect);
        copy::damageEffect(&data.editDamageEffect, &data.origDamageEffect);
        copy::planarReflection(&data.editPlanarReflection, &data.origPlanarReflection);
        copy::dynamicEnvmap(&data.editDynamicEnvmap, &data.origDynamicEnvmap);
        copy::characterLighting(&data.editCharacterLighting, &data.origCharacterLighting);
        copy::motionBlur(&data.editMotionBlur, &data.origMotionBlur);
    }

    void resetComponent(int index)
    {
        if (!data.captured)
            return;

        switch (index)
        {
        case 0:  copy::outdoorLight(&data.editOutdoorLight, &data.origOutdoorLight); break;
        case 1:  copy::enlighten(&data.editEnlighten, &data.origEnlighten); break;
        case 2:  copy::tonemap(&data.editTonemap, &data.origTonemap); break;
        case 3:  copy::colorCorrection(&data.editColorCorrection, &data.origColorCorrection); break;
        case 4:  copy::sky(&data.editSky, &data.origSky); break;
        case 5:  copy::fog(&data.editFog, &data.origFog); break;
        case 6:  copy::wind(&data.editWind, &data.origWind); break;
        case 7:  copy::sunFlare(&data.editSunFlare, &data.origSunFlare); break;
        case 8:  copy::dynamicAO(&data.editDynamicAO, &data.origDynamicAO); break;
        case 9:  copy::dof(&data.editDof, &data.origDof); break;
        case 10: copy::vignette(&data.editVignette, &data.origVignette); break;
        case 11: copy::filmGrain(&data.editFilmGrain, &data.origFilmGrain); break;
        case 12: copy::lensScope(&data.editLensScope, &data.origLensScope); break;
        case 13: copy::cameraParams(&data.editCameraParams, &data.origCameraParams); break;
        case 14: copy::screenEffect(&data.editScreenEffect, &data.origScreenEffect); break;
        case 15: copy::damageEffect(&data.editDamageEffect, &data.origDamageEffect); break;
        case 16: copy::planarReflection(&data.editPlanarReflection, &data.origPlanarReflection); break;
        case 17: copy::dynamicEnvmap(&data.editDynamicEnvmap, &data.origDynamicEnvmap); break;
        case 18: copy::characterLighting(&data.editCharacterLighting, &data.origCharacterLighting); break;
        case 19: copy::motionBlur(&data.editMotionBlur, &data.origMotionBlur); break;
        }
    }
}