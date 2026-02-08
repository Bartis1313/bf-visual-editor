#pragma once

#include "../../SDK/sdk.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace editor::comps
{
    json serializeOutdoorLight(const fb::OutdoorLightComponentData& c);
    json serializeEnlighten(const fb::EnlightenComponentData& c);
    json serializeTonemap(const fb::TonemapComponentData& c);
    json serializeColorCorrection(const fb::ColorCorrectionComponentData& c);
    json serializeSky(const fb::SkyComponentData& c);
    json serializeFog(const fb::FogComponentData& c);
    json serializeWind(const fb::WindComponentData& c);
    json serializeSunFlare(const fb::SunFlareComponentData& c);
    json serializeDynamicAO(const fb::DynamicAOComponentData& c);
    json serializeDof(const fb::DofComponentData& c);
    json serializeVignette(const fb::VignetteComponentData& c);
    json serializeFilmGrain(const fb::FilmGrainComponentData& c);
    json serializeLensScope(const fb::LensScopeComponentData& c);
    json serializeCameraParams(const fb::CameraParamsComponentData& c);
    json serializeScreenEffect(const fb::ScreenEffectComponentData& c);
    json serializeDamageEffect(const fb::DamageEffectComponentData& c);
    json serializePlanarReflection(const fb::PlanarReflectionComponentData& c);
    json serializeDynamicEnvmap(const fb::DynamicEnvmapComponentData& c);
    json serializeCharacterLighting(const fb::CharacterLightingComponentData& c);
    json serializeMotionBlur(const fb::MotionBlurComponentData& c);

    void deserializeOutdoorLight(const json& j, fb::OutdoorLightComponentData& c);
    void deserializeEnlighten(const json& j, fb::EnlightenComponentData& c);
    void deserializeTonemap(const json& j, fb::TonemapComponentData& c);
    void deserializeColorCorrection(const json& j, fb::ColorCorrectionComponentData& c);
    void deserializeSky(const json& j, fb::SkyComponentData& c);
    void deserializeFog(const json& j, fb::FogComponentData& c);
    void deserializeWind(const json& j, fb::WindComponentData& c);
    void deserializeSunFlare(const json& j, fb::SunFlareComponentData& c);
    void deserializeDynamicAO(const json& j, fb::DynamicAOComponentData& c);
    void deserializeDof(const json& j, fb::DofComponentData& c);
    void deserializeVignette(const json& j, fb::VignetteComponentData& c);
    void deserializeFilmGrain(const json& j, fb::FilmGrainComponentData& c);
    void deserializeLensScope(const json& j, fb::LensScopeComponentData& c);
    void deserializeCameraParams(const json& j, fb::CameraParamsComponentData& c);
    void deserializeScreenEffect(const json& j, fb::ScreenEffectComponentData& c);
    void deserializeDamageEffect(const json& j, fb::DamageEffectComponentData& c);
    void deserializePlanarReflection(const json& j, fb::PlanarReflectionComponentData& c);
    void deserializeDynamicEnvmap(const json& j, fb::DynamicEnvmapComponentData& c);
    void deserializeCharacterLighting(const json& j, fb::CharacterLightingComponentData& c);
    void deserializeMotionBlur(const json& j, fb::MotionBlurComponentData& c);
}