#pragma once

#include "../../SDK/sdk.h"

namespace editor::copy
{
    void outdoorLight(fb::OutdoorLightComponentData* dst, const fb::OutdoorLightComponentData* src);
    void enlighten(fb::EnlightenComponentData* dst, const fb::EnlightenComponentData* src);
    void tonemap(fb::TonemapComponentData* dst, const fb::TonemapComponentData* src);
    void colorCorrection(fb::ColorCorrectionComponentData* dst, const fb::ColorCorrectionComponentData* src);
    void sky(fb::SkyComponentData* dst, const fb::SkyComponentData* src);
    void fog(fb::FogComponentData* dst, const fb::FogComponentData* src);
    void wind(fb::WindComponentData* dst, const fb::WindComponentData* src);
    void sunFlare(fb::SunFlareComponentData* dst, const fb::SunFlareComponentData* src);
    void dynamicAO(fb::DynamicAOComponentData* dst, const fb::DynamicAOComponentData* src);
    void dof(fb::DofComponentData* dst, const fb::DofComponentData* src);
    void vignette(fb::VignetteComponentData* dst, const fb::VignetteComponentData* src);
    void filmGrain(fb::FilmGrainComponentData* dst, const fb::FilmGrainComponentData* src);
    void lensScope(fb::LensScopeComponentData* dst, const fb::LensScopeComponentData* src);
    void cameraParams(fb::CameraParamsComponentData* dst, const fb::CameraParamsComponentData* src);
    void screenEffect(fb::ScreenEffectComponentData* dst, const fb::ScreenEffectComponentData* src);
    void damageEffect(fb::DamageEffectComponentData* dst, const fb::DamageEffectComponentData* src);
    void planarReflection(fb::PlanarReflectionComponentData* dst, const fb::PlanarReflectionComponentData* src);
    void dynamicEnvmap(fb::DynamicEnvmapComponentData* dst, const fb::DynamicEnvmapComponentData* src);
    void characterLighting(fb::CharacterLightingComponentData* dst, const fb::CharacterLightingComponentData* src);
    void motionBlur(fb::MotionBlurComponentData* dst, const fb::MotionBlurComponentData* src);
    void worldRenderSettings(fb::WorldRenderSettings* dst, const fb::WorldRenderSettings* src);
}