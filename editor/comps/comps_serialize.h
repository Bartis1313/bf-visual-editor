#pragma once

#include "../../SDK/fb.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace editor::comps
{
    json serializeOutdoorLight(const fb::CapturedOutdoorLightComponentData& c);
    json serializeEnlighten(const fb::CapturedEnlightenComponentData& c);
    json serializeTonemap(const fb::CapturedTonemapComponentData& c);
    json serializeColorCorrection(const fb::CapturedColorCorrectionComponentData& c);
    json serializeSky(const fb::CapturedSkyComponentData& c);
    json serializeFog(const fb::CapturedFogComponentData& c);
    json serializeWind(const fb::CapturedWindComponentData& c);
    json serializeSunFlare(const fb::CapturedSunFlareComponentData& c);
    json serializeDynamicAO(const fb::CapturedDynamicAOComponentData& c);
    json serializeDof(const fb::CapturedDofComponentData& c);
    json serializeVignette(const fb::CapturedVignetteComponentData& c);
    json serializeFilmGrain(const fb::CapturedFilmGrainComponentData& c);
    json serializeLensScope(const fb::CapturedLensScopeComponentData& c);
    json serializeCameraParams(const fb::CapturedCameraParamsComponentData& c);
    json serializeScreenEffect(const fb::CapturedScreenEffectComponentData& c);
    json serializeDamageEffect(const fb::CapturedDamageEffectComponentData& c);
    json serializePlanarReflection(const fb::CapturedPlanarReflectionComponentData& c);
    json serializeDynamicEnvmap(const fb::CapturedDynamicEnvmapComponentData& c);
    json serializeCharacterLighting(const fb::CapturedCharacterLightingComponentData& c);
    json serializeMotionBlur(const fb::CapturedMotionBlurComponentData& c);

    void deserializeOutdoorLight(const json& j, fb::CapturedOutdoorLightComponentData& c);
    void deserializeEnlighten(const json& j, fb::CapturedEnlightenComponentData& c);
    void deserializeTonemap(const json& j, fb::CapturedTonemapComponentData& c);
    void deserializeColorCorrection(const json& j, fb::CapturedColorCorrectionComponentData& c);
    void deserializeSky(const json& j, fb::CapturedSkyComponentData& c);
    void deserializeFog(const json& j, fb::CapturedFogComponentData& c);
    void deserializeWind(const json& j, fb::CapturedWindComponentData& c);
    void deserializeSunFlare(const json& j, fb::CapturedSunFlareComponentData& c);
    void deserializeDynamicAO(const json& j, fb::CapturedDynamicAOComponentData& c);
    void deserializeDof(const json& j, fb::CapturedDofComponentData& c);
    void deserializeVignette(const json& j, fb::CapturedVignetteComponentData& c);
    void deserializeFilmGrain(const json& j, fb::CapturedFilmGrainComponentData& c);
    void deserializeLensScope(const json& j, fb::CapturedLensScopeComponentData& c);
    void deserializeCameraParams(const json& j, fb::CapturedCameraParamsComponentData& c);
    void deserializeScreenEffect(const json& j, fb::CapturedScreenEffectComponentData& c);
    void deserializeDamageEffect(const json& j, fb::CapturedDamageEffectComponentData& c);
    void deserializePlanarReflection(const json& j, fb::CapturedPlanarReflectionComponentData& c);
    void deserializeDynamicEnvmap(const json& j, fb::CapturedDynamicEnvmapComponentData& c);
    void deserializeCharacterLighting(const json& j, fb::CapturedCharacterLightingComponentData& c);
    void deserializeMotionBlur(const json& j, fb::CapturedMotionBlurComponentData& c);

#if defined(BFVE_GAME_BF4)
    json serializeVehicleLighting(const fb::CapturedVehicleLightingComponentData& c);
    json serializeSubSurfaceScattering(const fb::CapturedSubSurfaceScatteringComponentData& c);
    void deserializeVehicleLighting(const json& j, fb::CapturedVehicleLightingComponentData& c);
    void deserializeSubSurfaceScattering(const json& j, fb::CapturedSubSurfaceScatteringComponentData& c);
#endif
}