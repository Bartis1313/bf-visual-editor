#pragma once

#include "../../SDK/fb.h"

namespace editor::comps
{
    void renderOutdoorLightComponent(fb::CapturedOutdoorLightComponentData* edit, const fb::CapturedOutdoorLightComponentData* orig);
    void renderEnlightenComponent(fb::CapturedEnlightenComponentData* edit, const fb::CapturedEnlightenComponentData* orig);
    void renderTonemapComponent(fb::CapturedTonemapComponentData* edit, const fb::CapturedTonemapComponentData* orig);
    void renderColorCorrectionComponent(fb::CapturedColorCorrectionComponentData* edit, const fb::CapturedColorCorrectionComponentData* orig);
    void renderSkyComponent(fb::CapturedSkyComponentData* edit, const fb::CapturedSkyComponentData* orig);
    void renderFogComponent(fb::CapturedFogComponentData* edit, const fb::CapturedFogComponentData* orig);
    void renderWindComponent(fb::CapturedWindComponentData* edit, const fb::CapturedWindComponentData* orig);
    void renderSunFlareComponent(fb::CapturedSunFlareComponentData* edit, const fb::CapturedSunFlareComponentData* orig);
    void renderDynamicAOComponent(fb::CapturedDynamicAOComponentData* edit, const fb::CapturedDynamicAOComponentData* orig);
    void renderDofComponent(fb::CapturedDofComponentData* edit, const fb::CapturedDofComponentData* orig);
    void renderVignetteComponent(fb::CapturedVignetteComponentData* edit, const fb::CapturedVignetteComponentData* orig);
    void renderFilmGrainComponent(fb::CapturedFilmGrainComponentData* edit, const fb::CapturedFilmGrainComponentData* orig);
    void renderLensScopeComponent(fb::CapturedLensScopeComponentData* edit, const fb::CapturedLensScopeComponentData* orig);
    void renderCameraParamsComponent(fb::CapturedCameraParamsComponentData* edit, const fb::CapturedCameraParamsComponentData* orig);
    void renderScreenEffectComponent(fb::CapturedScreenEffectComponentData* edit, const fb::CapturedScreenEffectComponentData* orig);
    void renderDamageEffectComponent(fb::CapturedDamageEffectComponentData* edit, const fb::CapturedDamageEffectComponentData* orig);
    void renderPlanarReflectionComponent(fb::CapturedPlanarReflectionComponentData* edit, const fb::CapturedPlanarReflectionComponentData* orig);
    void renderDynamicEnvmapComponent(fb::CapturedDynamicEnvmapComponentData* edit, const fb::CapturedDynamicEnvmapComponentData* orig);
    void renderCharacterLightingComponent(fb::CapturedCharacterLightingComponentData* edit, const fb::CapturedCharacterLightingComponentData* orig);
    void renderMotionBlurComponent(fb::CapturedMotionBlurComponentData* edit, const fb::CapturedMotionBlurComponentData* orig);
    void renderWorldRenderSettingsComponent(fb::WorldRenderSettings* edit, const fb::WorldRenderSettings* orig);
#if defined(BFVE_GAME_BF4)
    void renderVehicleLightingComponent(fb::CapturedVehicleLightingComponentData* edit, const fb::CapturedVehicleLightingComponentData* orig);
    void renderSubSurfaceScatteringComponent(fb::CapturedSubSurfaceScatteringComponentData* edit, const fb::CapturedSubSurfaceScatteringComponentData* orig);
#endif
}
