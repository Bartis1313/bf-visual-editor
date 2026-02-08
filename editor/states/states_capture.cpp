#include "states.h"
#include "../editor_context.h"
#include "../copy/copy.h"

// rewrite of this is needed, to keep this macro hell out
namespace editor::states
{
    void captureStateData(fb::VisualEnvironmentState* state, StateEditData& data)
    {
        if (!state || levelUnloadingSignaled || !isSafeToOperate())
            return;

        data.captured = true;
        data.statePtr = state;

#define CAPTURE_COMPONENT(ptr, hasFlag, capturedPtr, origMember, editMember, CopyFunc) \
        if (!levelUnloadingSignaled && state->ptr) { \
            if (!data.hasFlag || data.capturedPtr != static_cast<void*>(state->ptr)) { \
                data.hasFlag = true; \
                data.capturedPtr = static_cast<void*>(state->ptr); \
                copy::CopyFunc(&data.origMember, state->ptr); \
                copy::CopyFunc(&data.editMember, state->ptr); \
            } \
        }

        CAPTURE_COMPONENT(outdoorLight, hasOutdoorLight, capturedOutdoorLight, origOutdoorLight, editOutdoorLight, outdoorLight);
        CAPTURE_COMPONENT(enlighten, hasEnlighten, capturedEnlighten, origEnlighten, editEnlighten, enlighten);
        CAPTURE_COMPONENT(tonemap, hasTonemap, capturedTonemap, origTonemap, editTonemap, tonemap);
        CAPTURE_COMPONENT(colorCorrection, hasColorCorrection, capturedColorCorrection, origColorCorrection, editColorCorrection, colorCorrection);
        CAPTURE_COMPONENT(sky, hasSky, capturedSky, origSky, editSky, sky);
        CAPTURE_COMPONENT(fog, hasFog, capturedFog, origFog, editFog, fog);
        CAPTURE_COMPONENT(wind, hasWind, capturedWind, origWind, editWind, wind);
        CAPTURE_COMPONENT(sunFlare, hasSunFlare, capturedSunFlare, origSunFlare, editSunFlare, sunFlare);
        CAPTURE_COMPONENT(dynamicAO, hasDynamicAO, capturedDynamicAO, origDynamicAO, editDynamicAO, dynamicAO);
        CAPTURE_COMPONENT(dof, hasDof, capturedDof, origDof, editDof, dof);
        CAPTURE_COMPONENT(vignette, hasVignette, capturedVignette, origVignette, editVignette, vignette);
        CAPTURE_COMPONENT(filmGrain, hasFilmGrain, capturedFilmGrain, origFilmGrain, editFilmGrain, filmGrain);
        CAPTURE_COMPONENT(lensScope, hasLensScope, capturedLensScope, origLensScope, editLensScope, lensScope);
        CAPTURE_COMPONENT(cameraParams, hasCameraParams, capturedCameraParams, origCameraParams, editCameraParams, cameraParams);
        CAPTURE_COMPONENT(screenEffect, hasScreenEffect, capturedScreenEffect, origScreenEffect, editScreenEffect, screenEffect);
        CAPTURE_COMPONENT(damageEffect, hasDamageEffect, capturedDamageEffect, origDamageEffect, editDamageEffect, damageEffect);
        CAPTURE_COMPONENT(planarReflection, hasPlanarReflection, capturedPlanarReflection, origPlanarReflection, editPlanarReflection, planarReflection);
        CAPTURE_COMPONENT(dynamicEnvmap, hasDynamicEnvmap, capturedDynamicEnvmap, origDynamicEnvmap, editDynamicEnvmap, dynamicEnvmap);
        CAPTURE_COMPONENT(characterLighting, hasCharacterLighting, capturedCharacterLighting, origCharacterLighting, editCharacterLighting, characterLighting);
        CAPTURE_COMPONENT(motionBlur, hasMotionBlur, capturedMotionBlur, origMotionBlur, editMotionBlur, motionBlur);

#undef CAPTURE_COMPONENT
    }

    void applyEdits(fb::VisualEnvironmentState* state, const StateEditData& data)
    {
        if (!state || !data.captured || levelUnloadingSignaled || !isSafeToOperate())
            return;

#define APPLY_COMPONENT(ptr, hasFlag, editMember, CopyFunc) \
        if (!levelUnloadingSignaled && data.hasFlag && state->ptr) { \
            copy::CopyFunc(state->ptr, &data.editMember); \
        }

        APPLY_COMPONENT(outdoorLight, hasOutdoorLight, editOutdoorLight, outdoorLight);
        APPLY_COMPONENT(enlighten, hasEnlighten, editEnlighten, enlighten);
        APPLY_COMPONENT(tonemap, hasTonemap, editTonemap, tonemap);
        APPLY_COMPONENT(colorCorrection, hasColorCorrection, editColorCorrection, colorCorrection);
        APPLY_COMPONENT(sky, hasSky, editSky, sky);
        APPLY_COMPONENT(fog, hasFog, editFog, fog);
        APPLY_COMPONENT(wind, hasWind, editWind, wind);
        APPLY_COMPONENT(sunFlare, hasSunFlare, editSunFlare, sunFlare);
        APPLY_COMPONENT(dynamicAO, hasDynamicAO, editDynamicAO, dynamicAO);
        APPLY_COMPONENT(dof, hasDof, editDof, dof);
        APPLY_COMPONENT(vignette, hasVignette, editVignette, vignette);
        APPLY_COMPONENT(filmGrain, hasFilmGrain, editFilmGrain, filmGrain);
        APPLY_COMPONENT(lensScope, hasLensScope, editLensScope, lensScope);
        APPLY_COMPONENT(cameraParams, hasCameraParams, editCameraParams, cameraParams);
        APPLY_COMPONENT(screenEffect, hasScreenEffect, editScreenEffect, screenEffect);
        APPLY_COMPONENT(damageEffect, hasDamageEffect, editDamageEffect, damageEffect);
        APPLY_COMPONENT(planarReflection, hasPlanarReflection, editPlanarReflection, planarReflection);
        APPLY_COMPONENT(dynamicEnvmap, hasDynamicEnvmap, editDynamicEnvmap, dynamicEnvmap);
        APPLY_COMPONENT(characterLighting, hasCharacterLighting, editCharacterLighting, characterLighting);
        APPLY_COMPONENT(motionBlur, hasMotionBlur, editMotionBlur, motionBlur);

#undef APPLY_COMPONENT
    }

    void restoreState(fb::VisualEnvironmentState* state, const StateEditData& data)
    {
        if (!state || !data.captured)
            return;

#define RESTORE_COMPONENT(ptr, hasFlag, origMember, CopyFunc) \
        if (data.hasFlag && state->ptr) { copy::CopyFunc(state->ptr, &data.origMember); }

        RESTORE_COMPONENT(outdoorLight, hasOutdoorLight, origOutdoorLight, outdoorLight);
        RESTORE_COMPONENT(enlighten, hasEnlighten, origEnlighten, enlighten);
        RESTORE_COMPONENT(tonemap, hasTonemap, origTonemap, tonemap);
        RESTORE_COMPONENT(colorCorrection, hasColorCorrection, origColorCorrection, colorCorrection);
        RESTORE_COMPONENT(sky, hasSky, origSky, sky);
        RESTORE_COMPONENT(fog, hasFog, origFog, fog);
        RESTORE_COMPONENT(wind, hasWind, origWind, wind);
        RESTORE_COMPONENT(sunFlare, hasSunFlare, origSunFlare, sunFlare);
        RESTORE_COMPONENT(dynamicAO, hasDynamicAO, origDynamicAO, dynamicAO);
        RESTORE_COMPONENT(dof, hasDof, origDof, dof);
        RESTORE_COMPONENT(vignette, hasVignette, origVignette, vignette);
        RESTORE_COMPONENT(filmGrain, hasFilmGrain, origFilmGrain, filmGrain);
        RESTORE_COMPONENT(lensScope, hasLensScope, origLensScope, lensScope);
        RESTORE_COMPONENT(cameraParams, hasCameraParams, origCameraParams, cameraParams);
        RESTORE_COMPONENT(screenEffect, hasScreenEffect, origScreenEffect, screenEffect);
        RESTORE_COMPONENT(damageEffect, hasDamageEffect, origDamageEffect, damageEffect);
        RESTORE_COMPONENT(planarReflection, hasPlanarReflection, origPlanarReflection, planarReflection);
        RESTORE_COMPONENT(dynamicEnvmap, hasDynamicEnvmap, origDynamicEnvmap, dynamicEnvmap);
        RESTORE_COMPONENT(characterLighting, hasCharacterLighting, origCharacterLighting, characterLighting);
        RESTORE_COMPONENT(motionBlur, hasMotionBlur, origMotionBlur, motionBlur);

#undef RESTORE_COMPONENT
    }

    void resetToOriginal(StateEditData& data)
    {
        if (!data.captured)
            return;

#define RESET_COMPONENT(hasFlag, origMember, editMember, CopyFunc) \
        if (data.hasFlag) { copy::CopyFunc(&data.editMember, &data.origMember); }

        RESET_COMPONENT(hasOutdoorLight, origOutdoorLight, editOutdoorLight, outdoorLight);
        RESET_COMPONENT(hasEnlighten, origEnlighten, editEnlighten, enlighten);
        RESET_COMPONENT(hasTonemap, origTonemap, editTonemap, tonemap);
        RESET_COMPONENT(hasColorCorrection, origColorCorrection, editColorCorrection, colorCorrection);
        RESET_COMPONENT(hasSky, origSky, editSky, sky);
        RESET_COMPONENT(hasFog, origFog, editFog, fog);
        RESET_COMPONENT(hasWind, origWind, editWind, wind);
        RESET_COMPONENT(hasSunFlare, origSunFlare, editSunFlare, sunFlare);
        RESET_COMPONENT(hasDynamicAO, origDynamicAO, editDynamicAO, dynamicAO);
        RESET_COMPONENT(hasDof, origDof, editDof, dof);
        RESET_COMPONENT(hasVignette, origVignette, editVignette, vignette);
        RESET_COMPONENT(hasFilmGrain, origFilmGrain, editFilmGrain, filmGrain);
        RESET_COMPONENT(hasLensScope, origLensScope, editLensScope, lensScope);
        RESET_COMPONENT(hasCameraParams, origCameraParams, editCameraParams, cameraParams);
        RESET_COMPONENT(hasScreenEffect, origScreenEffect, editScreenEffect, screenEffect);
        RESET_COMPONENT(hasDamageEffect, origDamageEffect, editDamageEffect, damageEffect);
        RESET_COMPONENT(hasPlanarReflection, origPlanarReflection, editPlanarReflection, planarReflection);
        RESET_COMPONENT(hasDynamicEnvmap, origDynamicEnvmap, editDynamicEnvmap, dynamicEnvmap);
        RESET_COMPONENT(hasCharacterLighting, origCharacterLighting, editCharacterLighting, characterLighting);
        RESET_COMPONENT(hasMotionBlur, origMotionBlur, editMotionBlur, motionBlur);

#undef RESET_COMPONENT
    }

    void restoreEditValuesFromEntry(StateEditData& dst, const StateEditData& src)
    {
        dst.overrideEnabled = src.overrideEnabled;

#define RESTORE_IF_BOTH_HAVE(hasFlag, editMember, CopyFunc) \
        if (dst.hasFlag && src.hasFlag) { \
            copy::CopyFunc(&dst.editMember, &src.editMember); \
        }

        RESTORE_IF_BOTH_HAVE(hasOutdoorLight, editOutdoorLight, outdoorLight);
        RESTORE_IF_BOTH_HAVE(hasEnlighten, editEnlighten, enlighten);
        RESTORE_IF_BOTH_HAVE(hasTonemap, editTonemap, tonemap);
        RESTORE_IF_BOTH_HAVE(hasColorCorrection, editColorCorrection, colorCorrection);
        RESTORE_IF_BOTH_HAVE(hasSky, editSky, sky);
        RESTORE_IF_BOTH_HAVE(hasFog, editFog, fog);
        RESTORE_IF_BOTH_HAVE(hasWind, editWind, wind);
        RESTORE_IF_BOTH_HAVE(hasSunFlare, editSunFlare, sunFlare);
        RESTORE_IF_BOTH_HAVE(hasDynamicAO, editDynamicAO, dynamicAO);
        RESTORE_IF_BOTH_HAVE(hasDof, editDof, dof);
        RESTORE_IF_BOTH_HAVE(hasVignette, editVignette, vignette);
        RESTORE_IF_BOTH_HAVE(hasFilmGrain, editFilmGrain, filmGrain);
        RESTORE_IF_BOTH_HAVE(hasLensScope, editLensScope, lensScope);
        RESTORE_IF_BOTH_HAVE(hasCameraParams, editCameraParams, cameraParams);
        RESTORE_IF_BOTH_HAVE(hasScreenEffect, editScreenEffect, screenEffect);
        RESTORE_IF_BOTH_HAVE(hasDamageEffect, editDamageEffect, damageEffect);
        RESTORE_IF_BOTH_HAVE(hasPlanarReflection, editPlanarReflection, planarReflection);
        RESTORE_IF_BOTH_HAVE(hasDynamicEnvmap, editDynamicEnvmap, dynamicEnvmap);
        RESTORE_IF_BOTH_HAVE(hasCharacterLighting, editCharacterLighting, characterLighting);
        RESTORE_IF_BOTH_HAVE(hasMotionBlur, editMotionBlur, motionBlur);

#undef RESTORE_IF_BOTH_HAVE
    }

    void copyEditDataDirectly(StateEditData& dst, const StateEditData& src)
    {
        dst.overrideEnabled = src.overrideEnabled;

        if (dst.hasOutdoorLight) copy::outdoorLight(&dst.editOutdoorLight, &src.editOutdoorLight);
        if (dst.hasEnlighten) copy::enlighten(&dst.editEnlighten, &src.editEnlighten);
        if (dst.hasTonemap) copy::tonemap(&dst.editTonemap, &src.editTonemap);
        if (dst.hasColorCorrection) copy::colorCorrection(&dst.editColorCorrection, &src.editColorCorrection);
        if (dst.hasSky) copy::sky(&dst.editSky, &src.editSky);
        if (dst.hasFog) copy::fog(&dst.editFog, &src.editFog);
        if (dst.hasWind) copy::wind(&dst.editWind, &src.editWind);
        if (dst.hasSunFlare) copy::sunFlare(&dst.editSunFlare, &src.editSunFlare);
        if (dst.hasDynamicAO) copy::dynamicAO(&dst.editDynamicAO, &src.editDynamicAO);
        if (dst.hasDof) copy::dof(&dst.editDof, &src.editDof);
        if (dst.hasVignette) copy::vignette(&dst.editVignette, &src.editVignette);
        if (dst.hasFilmGrain) copy::filmGrain(&dst.editFilmGrain, &src.editFilmGrain);
        if (dst.hasLensScope) copy::lensScope(&dst.editLensScope, &src.editLensScope);
        if (dst.hasCameraParams) copy::cameraParams(&dst.editCameraParams, &src.editCameraParams);
        if (dst.hasScreenEffect) copy::screenEffect(&dst.editScreenEffect, &src.editScreenEffect);
        if (dst.hasDamageEffect) copy::damageEffect(&dst.editDamageEffect, &src.editDamageEffect);
        if (dst.hasPlanarReflection) copy::planarReflection(&dst.editPlanarReflection, &src.editPlanarReflection);
        if (dst.hasDynamicEnvmap) copy::dynamicEnvmap(&dst.editDynamicEnvmap, &src.editDynamicEnvmap);
        if (dst.hasCharacterLighting) copy::characterLighting(&dst.editCharacterLighting, &src.editCharacterLighting);
        if (dst.hasMotionBlur) copy::motionBlur(&dst.editMotionBlur, &src.editMotionBlur);
    }
}