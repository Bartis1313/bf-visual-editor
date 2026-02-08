#pragma once

#include "../../SDK/sdk.h"

namespace editor::comps
{
    void renderOutdoorLightComponent(fb::OutdoorLightComponentData* edit, const fb::OutdoorLightComponentData* orig);
    void renderEnlightenComponent(fb::EnlightenComponentData* edit, const fb::EnlightenComponentData* orig);
    void renderTonemapComponent(fb::TonemapComponentData* edit, const fb::TonemapComponentData* orig);
    void renderColorCorrectionComponent(fb::ColorCorrectionComponentData* edit, const fb::ColorCorrectionComponentData* orig);
    void renderSkyComponent(fb::SkyComponentData* edit, const fb::SkyComponentData* orig);
    void renderFogComponent(fb::FogComponentData* edit, const fb::FogComponentData* orig);
    void renderWindComponent(fb::WindComponentData* edit, const fb::WindComponentData* orig);
    void renderSunFlareComponent(fb::SunFlareComponentData* edit, const fb::SunFlareComponentData* orig);
    void renderDynamicAOComponent(fb::DynamicAOComponentData* edit, const fb::DynamicAOComponentData* orig);
    void renderDofComponent(fb::DofComponentData* edit, const fb::DofComponentData* orig);
    void renderVignetteComponent(fb::VignetteComponentData* edit, const fb::VignetteComponentData* orig);
    void renderFilmGrainComponent(fb::FilmGrainComponentData* edit, const fb::FilmGrainComponentData* orig);
    void renderLensScopeComponent(fb::LensScopeComponentData* edit, const fb::LensScopeComponentData* orig);
    void renderCameraParamsComponent(fb::CameraParamsComponentData* edit, const fb::CameraParamsComponentData* orig);
    void renderScreenEffectComponent(fb::ScreenEffectComponentData* edit, const fb::ScreenEffectComponentData* orig);
    void renderDamageEffectComponent(fb::DamageEffectComponentData* edit, const fb::DamageEffectComponentData* orig);
    void renderPlanarReflectionComponent(fb::PlanarReflectionComponentData* edit, const fb::PlanarReflectionComponentData* orig);
    void renderDynamicEnvmapComponent(fb::DynamicEnvmapComponentData* edit, const fb::DynamicEnvmapComponentData* orig);
    void renderCharacterLightingComponent(fb::CharacterLightingComponentData* edit, const fb::CharacterLightingComponentData* orig);
    void renderMotionBlurComponent(fb::MotionBlurComponentData* edit, const fb::MotionBlurComponentData* orig);
    void renderWorldRenderSettingsComponent(fb::WorldRenderSettings* edit, const fb::WorldRenderSettings* orig);
}
