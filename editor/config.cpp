#include "editor.h"
#include <fstream>
#include <ShlObj.h>
#include "../utils/log.h"

namespace fs = std::filesystem;

#define JSON_SET(j, key, val) j[key] = val
#define JSON_SET_VEC2(j, key, v) j[key] = SerializeVec2(v)
#define JSON_SET_VEC3(j, key, v) j[key] = SerializeVec3(v)
#define JSON_SET_VEC4(j, key, v) j[key] = SerializeVec4(v)

#define JSON_GET(j, key, var) if ((j).contains(key) && (j)[key].is_number()) var = (j)[key].get<decltype(var)>()
#define JSON_GET_BOOL(j, key, var) if ((j).contains(key) && (j)[key].is_boolean()) var = (j)[key].get<bool>()
#define JSON_GET_INT(j, key, var) if ((j).contains(key) && (j)[key].is_number_integer()) var = (j)[key].get<int>()
#define JSON_GET_ENUM(j, key, var, type) if ((j).contains(key) && (j)[key].is_number_integer()) var = static_cast<type>((j)[key].get<int>())
#define JSON_GET_VEC2(j, key, var) if ((j).contains(key)) DeserializeVec2((j)[key], var)
#define JSON_GET_VEC3(j, key, var) if ((j).contains(key)) DeserializeVec3((j)[key], var)
#define JSON_GET_VEC4(j, key, var) if ((j).contains(key)) DeserializeVec4((j)[key], var)

json VisualEnvironmentEditor::SerializeVec2(const fb::Vec2& v) const
{
    return { {"x", v.m_x}, {"y", v.m_y} };
}

json VisualEnvironmentEditor::SerializeVec3(const fb::Vec3& v) const
{
    return { {"x", v.m_x}, {"y", v.m_y}, {"z", v.m_z} };
}

json VisualEnvironmentEditor::SerializeVec4(const fb::Vec4& v) const
{
    return { {"x", v.m_x}, {"y", v.m_y}, {"z", v.m_z}, {"w", v.m_w} };
}

void VisualEnvironmentEditor::DeserializeVec2(const json& j, fb::Vec2& v) const
{
    JSON_GET(j, "x", v.m_x);
    JSON_GET(j, "y", v.m_y);
}

void VisualEnvironmentEditor::DeserializeVec3(const json& j, fb::Vec3& v) const
{
    JSON_GET(j, "x", v.m_x);
    JSON_GET(j, "y", v.m_y);
    JSON_GET(j, "z", v.m_z);
}

void VisualEnvironmentEditor::DeserializeVec4(const json& j, fb::Vec4& v) const
{
    JSON_GET(j, "x", v.m_x);
    JSON_GET(j, "y", v.m_y);
    JSON_GET(j, "z", v.m_z);
    JSON_GET(j, "w", v.m_w);
}

json VisualEnvironmentEditor::SerializeLightData(const LightDataEntry& entry) const
{
    json j;

    j["lightType"] = entry.lightType;
    j["hasOverride"] = entry.hasOverride;

    if (!entry.hasOverride)
        return j;

    JSON_SET_VEC3(j, "color", entry.color);
    JSON_SET_VEC3(j, "particleColorScale", entry.particleColorScale);
    JSON_SET_VEC3(j, "enlightenColorScale", entry.enlightenColorScale);
    JSON_SET(j, "radius", entry.radius);
    JSON_SET(j, "intensity", entry.intensity);
    JSON_SET(j, "attenuationOffset", entry.attenuationOffset);
    JSON_SET(j, "enlightenColorMode", entry.enlightenColorMode);
    JSON_SET(j, "visible", entry.visible);
    JSON_SET(j, "specularEnable", entry.specularEnable);
    JSON_SET(j, "enlightenEnable", entry.enlightenEnable);

    // SpotLight specific
    if (entry.isSpotLight)
    {
        JSON_SET(j, "spotShape", entry.spotShape);
        JSON_SET(j, "coneInnerAngle", entry.coneInnerAngle);
        JSON_SET(j, "coneOuterAngle", entry.coneOuterAngle);
        JSON_SET(j, "frustumFov", entry.frustumFov);
        JSON_SET(j, "frustumAspect", entry.frustumAspect);
        JSON_SET(j, "orthoWidth", entry.orthoWidth);
        JSON_SET(j, "orthoHeight", entry.orthoHeight);
        JSON_SET(j, "castShadowsEnable", entry.castShadowsEnable);
        JSON_SET(j, "castShadowsMinLevel", entry.castShadowsMinLevel);
    }

    // PointLight specific
    if (entry.isPointLight)
    {
        JSON_SET(j, "pointWidth", entry.pointWidth);
        JSON_SET(j, "translucencyAmbient", entry.translucencyAmbient);
        JSON_SET(j, "translucencyScale", entry.translucencyScale);
        JSON_SET(j, "translucencyPower", entry.translucencyPower);
        JSON_SET(j, "translucencyDistortion", entry.translucencyDistortion);
    }

    return j;
}

void VisualEnvironmentEditor::DeserializeLightData(const json& j, LightDataEntry& entry)
{
    JSON_GET_BOOL(j, "hasOverride", entry.hasOverride);

    if (!entry.hasOverride)
        return;

    JSON_GET_VEC3(j, "color", entry.color);
    JSON_GET_VEC3(j, "particleColorScale", entry.particleColorScale);
    JSON_GET_VEC3(j, "enlightenColorScale", entry.enlightenColorScale);
    JSON_GET(j, "radius", entry.radius);
    JSON_GET(j, "intensity", entry.intensity);
    JSON_GET(j, "attenuationOffset", entry.attenuationOffset);
    JSON_GET_INT(j, "enlightenColorMode", entry.enlightenColorMode);
    JSON_GET_BOOL(j, "visible", entry.visible);
    JSON_GET_BOOL(j, "specularEnable", entry.specularEnable);
    JSON_GET_BOOL(j, "enlightenEnable", entry.enlightenEnable);

    // SpotLight specific
    if (entry.isSpotLight)
    {
        JSON_GET_INT(j, "spotShape", entry.spotShape);
        JSON_GET(j, "coneInnerAngle", entry.coneInnerAngle);
        JSON_GET(j, "coneOuterAngle", entry.coneOuterAngle);
        JSON_GET(j, "frustumFov", entry.frustumFov);
        JSON_GET(j, "frustumAspect", entry.frustumAspect);
        JSON_GET(j, "orthoWidth", entry.orthoWidth);
        JSON_GET(j, "orthoHeight", entry.orthoHeight);
        JSON_GET_BOOL(j, "castShadowsEnable", entry.castShadowsEnable);
        JSON_GET_INT(j, "castShadowsMinLevel", entry.castShadowsMinLevel);
    }

    // PointLight specific
    if (entry.isPointLight)
    {
        JSON_GET(j, "pointWidth", entry.pointWidth);
        JSON_GET(j, "translucencyAmbient", entry.translucencyAmbient);
        JSON_GET(j, "translucencyScale", entry.translucencyScale);
        JSON_GET(j, "translucencyPower", entry.translucencyPower);
        JSON_GET(j, "translucencyDistortion", entry.translucencyDistortion);
    }
}

json VisualEnvironmentEditor::SerializeStateData(const StateEditData& data) const
{
    json j;
    j["overrideEnabled"] = data.overrideEnabled;

    if (!data.captured)
        return j;

#define SERIALIZE_COMPONENT(name, hasFlag, editMember, serializeFunc) \
    if (data.hasFlag) { j[name] = serializeFunc(data.editMember); }

    SERIALIZE_COMPONENT("outdoorLight", hasOutdoorLight, editOutdoorLight, SerializeOutdoorLight);
    SERIALIZE_COMPONENT("enlighten", hasEnlighten, editEnlighten, SerializeEnlighten);
    SERIALIZE_COMPONENT("tonemap", hasTonemap, editTonemap, SerializeTonemap);
    SERIALIZE_COMPONENT("colorCorrection", hasColorCorrection, editColorCorrection, SerializeColorCorrection);
    SERIALIZE_COMPONENT("sky", hasSky, editSky, SerializeSky);
    SERIALIZE_COMPONENT("fog", hasFog, editFog, SerializeFog);
    SERIALIZE_COMPONENT("wind", hasWind, editWind, SerializeWind);
    SERIALIZE_COMPONENT("sunflare", hasSunFlare, editSunFlare, SerializeSunflare);
    SERIALIZE_COMPONENT("dynamicAO", hasDynamicAO, editDynamicAO, SerializeDynamicAO);
    SERIALIZE_COMPONENT("dof", hasDof, editDof, SerializeDof);
    SERIALIZE_COMPONENT("vignette", hasVignette, editVignette, SerializeVignette);
    SERIALIZE_COMPONENT("filmGrain", hasFilmGrain, editFilmGrain, SerializeFilmGrain);
    SERIALIZE_COMPONENT("lensScope", hasLensScope, editLensScope, SerializeLensScope);
    SERIALIZE_COMPONENT("cameraParams", hasCameraParams, editCameraParams, SerializeCameraParams);
    SERIALIZE_COMPONENT("screenEffect", hasScreenEffect, editScreenEffect, SerializeScreenEffect);
    SERIALIZE_COMPONENT("damageEffect", hasDamageEffect, editDamageEffect, SerializeDamageEffect);
    SERIALIZE_COMPONENT("planarReflection", hasPlanarReflection, editPlanarReflection, SerializePlanarReflection);
    SERIALIZE_COMPONENT("dynamicEnvmap", hasDynamicEnvmap, editDynamicEnvmap, SerializeDynamicEnvmap);
    SERIALIZE_COMPONENT("characterLighting", hasCharacterLighting, editCharacterLighting, SerializeCharacterLighting);
    SERIALIZE_COMPONENT("motionBlur", hasMotionBlur, editMotionBlur, SerializeMotionBlur);

#undef SERIALIZE_COMPONENT

    return j;
}

json VisualEnvironmentEditor::SerializeOutdoorLight(const fb::OutdoorLightComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["sunColor"] = SerializeVec3(c.m_SunColor);
    j["skyColor"] = SerializeVec3(c.m_SkyColor);
    j["groundColor"] = SerializeVec3(c.m_GroundColor);
    j["sunRotationX"] = c.m_SunRotationX;
    j["sunRotationY"] = c.m_SunRotationY;
    j["sunSpecularScale"] = c.m_SunSpecularScale;
    j["skyLightAngleFactor"] = c.m_SkyLightAngleFactor;
    j["skyEnvmapShadowScale"] = c.m_SkyEnvmapShadowScale;
    j["sunShadowHeightScale"] = c.m_SunShadowHeightScale;
    j["cloudShadowEnable"] = c.m_CloudShadowEnable;
    j["cloudShadowSpeed"] = SerializeVec2(c.m_CloudShadowSpeed);
    j["cloudShadowCoverage"] = c.m_CloudShadowCoverage;
    j["cloudShadowSize"] = c.m_CloudShadowSize;
    j["cloudShadowExponent"] = c.m_CloudShadowExponent;
    j["translucencyDistortion"] = c.m_TranslucencyDistortion;
    j["translucencyAmbient"] = c.m_TranslucencyAmbient;
    j["translucencyScale"] = c.m_TranslucencyScale;
    j["translucencyPower"] = c.m_TranslucencyPower;
    return j;
}

json VisualEnvironmentEditor::SerializeEnlighten(const fb::EnlightenComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["skyBoxEnable"] = c.m_SkyBoxEnable;
    j["skyBoxSkyColor"] = SerializeVec3(c.m_SkyBoxSkyColor);
    j["skyBoxGroundColor"] = SerializeVec3(c.m_SkyBoxGroundColor);
    j["skyBoxSunLightColor"] = SerializeVec3(c.m_SkyBoxSunLightColor);
    j["skyBoxBackLightColor"] = SerializeVec3(c.m_SkyBoxBackLightColor);
    j["terrainColor"] = SerializeVec3(c.m_TerrainColor);
    j["bounceScale"] = c.m_BounceScale;
    j["sunScale"] = c.m_SunScale;
    j["cullDistance"] = c.m_CullDistance;
    j["skyBoxSunLightColorSize"] = c.m_SkyBoxSunLightColorSize;
    j["skyBoxBackLightColorSize"] = c.m_SkyBoxBackLightColorSize;
    j["skyBoxBackLightRotationX"] = c.m_SkyBoxBackLightRotationX;
    j["skyBoxBackLightRotationY"] = c.m_SkyBoxBackLightRotationY;
    return j;
}

json VisualEnvironmentEditor::SerializeTonemap(const fb::TonemapComponentData& c) const
{
    json j;
    j["tonemapMethod"] = static_cast<int>(c.m_TonemapMethod);
    j["bloomScale"] = SerializeVec3(c.m_BloomScale);
    j["minExposure"] = c.m_MinExposure;
    j["maxExposure"] = c.m_MaxExposure;
    j["middleGray"] = c.m_MiddleGray;
    j["exposureAdjustTime"] = c.m_ExposureAdjustTime;
    j["chromostereopsisEnable"] = c.m_ChromostereopsisEnable;
    j["chromostereopsisOffset"] = c.m_ChromostereopsisOffset;
    j["chromostereopsisScale"] = c.m_ChromostereopsisScale;
    return j;
}

json VisualEnvironmentEditor::SerializeColorCorrection(const fb::ColorCorrectionComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["colorGradingEnable"] = c.m_ColorGradingEnable;
    j["brightness"] = SerializeVec3(c.m_Brightness);
    j["contrast"] = SerializeVec3(c.m_Contrast);
    j["saturation"] = SerializeVec3(c.m_Saturation);
    j["hue"] = c.m_Hue;
    return j;
}

json VisualEnvironmentEditor::SerializeSky(const fb::SkyComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["brightnessScale"] = c.m_BrightnessScale;
    j["sunSize"] = c.m_SunSize;
    j["sunScale"] = c.m_SunScale;
    j["skyVisibilityExponent"] = c.m_SkyVisibilityExponent;
    j["skyEnvmap8BitTexScale"] = c.m_SkyEnvmap8BitTexScale;
    j["cloudLayerSunColor"] = SerializeVec3(c.m_CloudLayerSunColor);
    j["cloudLayer1Color"] = SerializeVec3(c.m_CloudLayer1Color);
    j["cloudLayer1Altitude"] = c.m_CloudLayer1Altitude;
    j["cloudLayer1TileFactor"] = c.m_CloudLayer1TileFactor;
    j["cloudLayer1Rotation"] = c.m_CloudLayer1Rotation;
    j["cloudLayer1Speed"] = c.m_CloudLayer1Speed;
    j["cloudLayer1SunLightIntensity"] = c.m_CloudLayer1SunLightIntensity;
    j["cloudLayer1SunLightPower"] = c.m_CloudLayer1SunLightPower;
    j["cloudLayer1AmbientLightIntensity"] = c.m_CloudLayer1AmbientLightIntensity;
    j["cloudLayer1AlphaMul"] = c.m_CloudLayer1AlphaMul;
    j["cloudLayer2Color"] = SerializeVec3(c.m_CloudLayer2Color);
    j["cloudLayer2Altitude"] = c.m_CloudLayer2Altitude;
    j["cloudLayer2TileFactor"] = c.m_CloudLayer2TileFactor;
    j["cloudLayer2Rotation"] = c.m_CloudLayer2Rotation;
    j["cloudLayer2Speed"] = c.m_CloudLayer2Speed;
    j["cloudLayer2SunLightIntensity"] = c.m_CloudLayer2SunLightIntensity;
    j["cloudLayer2SunLightPower"] = c.m_CloudLayer2SunLightPower;
    j["cloudLayer2AmbientLightIntensity"] = c.m_CloudLayer2AmbientLightIntensity;
    j["cloudLayer2AlphaMul"] = c.m_CloudLayer2AlphaMul;
    j["panoramicRotation"] = c.m_PanoramicRotation;
    j["panoramicTileFactor"] = c.m_PanoramicTileFactor;
    j["panoramicUVMinX"] = c.m_PanoramicUVMinX;
    j["panoramicUVMaxX"] = c.m_PanoramicUVMaxX;
    j["panoramicUVMinY"] = c.m_PanoramicUVMinY;
    j["panoramicUVMaxY"] = c.m_PanoramicUVMaxY;
    j["customEnvmapScale"] = c.m_CustomEnvmapScale;
    j["customEnvmapAmbient"] = c.m_CustomEnvmapAmbient;
    j["staticEnvmapScale"] = c.m_StaticEnvmapScale;
    return j;
}

json VisualEnvironmentEditor::SerializeFog(const fb::FogComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["start"] = c.m_Start;
    j["end"] = c.m_End;
    j["fogDistanceMultiplier"] = c.m_FogDistanceMultiplier;
    j["curve"] = SerializeVec4(c.m_Curve);
    j["fogColorEnable"] = c.m_FogColorEnable;
    j["fogColor"] = SerializeVec3(c.m_FogColor);
    j["fogColorStart"] = c.m_FogColorStart;
    j["fogColorEnd"] = c.m_FogColorEnd;
    j["fogColorCurve"] = SerializeVec4(c.m_FogColorCurve);
    j["fogGradientEnable"] = c.m_FogGradientEnable;
    j["heightFogEnable"] = c.m_HeightFogEnable;
    j["heightFogAltitude"] = c.m_HeightFogAltitude;
    j["heightFogDepth"] = c.m_HeightFogDepth;
    j["heightFogVisibilityRange"] = c.m_HeightFogVisibilityRange;
    j["heightFogFollowCamera"] = c.m_HeightFogFollowCamera;
    j["transparencyFadeStart"] = c.m_TransparencyFadeStart;
    j["transparencyFadeEnd"] = c.m_TransparencyFadeEnd;
    j["transparencyFadeClamp"] = c.m_TransparencyFadeClamp;
    return j;
}

json VisualEnvironmentEditor::SerializeWind(const fb::WindComponentData& c) const
{
    return { {"windDirection", c.m_WindDirection}, {"windStrength", c.m_WindStrength} };
}

json VisualEnvironmentEditor::SerializeSunflare(const fb::SunFlareComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["debugDrawOccluder"] = c.m_DebugDrawOccluder;
    j["occluderSize"] = c.m_OccluderSize;

#define SERIALIZE_ELEMENT(n) \
    j["element" #n "Enable"] = c.m_Element##n##Enable; \
    j["element" #n "Size"] = SerializeVec2(c.m_Element##n##Size); \
    j["element" #n "RayDistance"] = c.m_Element##n##RayDistance;

    SERIALIZE_ELEMENT(1) SERIALIZE_ELEMENT(2) SERIALIZE_ELEMENT(3)
        SERIALIZE_ELEMENT(4) SERIALIZE_ELEMENT(5)

#undef SERIALIZE_ELEMENT
        return j;
}

json VisualEnvironmentEditor::SerializeDynamicAO(const fb::DynamicAOComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["ssaoRadius"] = c.m_SsaoRadius;
    j["ssaoFade"] = c.m_SsaoFade;
    j["ssaoMaxDistanceInner"] = c.m_SsaoMaxDistanceInner;
    j["ssaoMaxDistanceOuter"] = c.m_SsaoMaxDistanceOuter;
    j["hbaoRadius"] = c.m_HbaoRadius;
    j["hbaoAttenuation"] = c.m_HbaoAttenuation;
    j["hbaoAngleBias"] = c.m_HbaoAngleBias;
    j["hbaoPowerExponent"] = c.m_HbaoPowerExponent;
    j["hbaoContrast"] = c.m_HbaoContrast;
    j["hbaoMaxFootprintRadius"] = c.m_HbaoMaxFootprintRadius;
    return j;
}

json VisualEnvironmentEditor::SerializeDof(const fb::DofComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["blurFilter"] = static_cast<int>(c.m_BlurFilter);
    j["focusDistance"] = c.m_FocusDistance;
    j["nearDistanceScale"] = c.m_NearDistanceScale;
    j["farDistanceScale"] = c.m_FarDistanceScale;
    j["blurFilterDeviation"] = c.m_BlurFilterDeviation;
    j["blurAdd"] = c.m_BlurAdd;
    j["scale"] = c.m_Scale;
    j["diffusionDofEnable"] = c.m_DiffusionDofEnable;
    j["diffusionDofFocalLength"] = c.m_DiffusionDofFocalLength;
    j["diffusionDofAperture"] = c.m_DiffusionDofAperture;
    return j;
}

json VisualEnvironmentEditor::SerializeVignette(const fb::VignetteComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["color"] = SerializeVec3(c.m_Color);
    j["scale"] = SerializeVec2(c.m_Scale);
    j["opacity"] = c.m_Opacity;
    j["exponent"] = c.m_Exponent;
    return j;
}

json VisualEnvironmentEditor::SerializeFilmGrain(const fb::FilmGrainComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["linearFilteringEnable"] = c.m_LinearFilteringEnable;
    j["randomEnable"] = c.m_RandomEnable;
    j["colorScale"] = SerializeVec3(c.m_ColorScale);
    j["textureScale"] = SerializeVec2(c.m_TextureScale);
    return j;
}

json VisualEnvironmentEditor::SerializeLensScope(const fb::LensScopeComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["blurCenter"] = SerializeVec2(c.m_BlurCenter);
    j["blurScale"] = c.m_BlurScale;
    j["radialBlendDistanceCoefficients"] = SerializeVec2(c.m_RadialBlendDistanceCoefficients);
    j["chromaticAberrationColor1"] = SerializeVec3(c.m_ChromaticAberrationColor1);
    j["chromaticAberrationColor2"] = SerializeVec3(c.m_ChromaticAberrationColor2);
    j["chromaticAberrationDisplacement1"] = SerializeVec2(c.m_ChromaticAberrationDisplacement1);
    j["chromaticAberrationDisplacement2"] = SerializeVec2(c.m_ChromaticAberrationDisplacement2);
    j["chromaticAberrationStrengths"] = SerializeVec2(c.m_ChromaticAberrationStrengths);
    return j;
}

json VisualEnvironmentEditor::SerializeCameraParams(const fb::CameraParamsComponentData& c) const
{
    json j;
    j["viewDistance"] = c.m_ViewDistance;
    j["nearPlane"] = c.m_NearPlane;
    j["sunShadowmapViewDistance"] = c.m_SunShadowmapViewDistance;
    return j;
}

json VisualEnvironmentEditor::SerializeScreenEffect(const fb::ScreenEffectComponentData& c) const
{
    json j;
    j["frameType"] = static_cast<int>(c.m_FrameType);
    j["screenEffectParams"] = SerializeVec4(c.m_ScreenEffectParams);
    j["frameWidth"] = c.m_FrameWidth;
    j["outerFrameOpacity"] = c.m_OuterFrameOpacity;
    j["innerFrameOpacity"] = c.m_InnerFrameOpacity;
    j["angle"] = c.m_Angle;
    return j;
}

json VisualEnvironmentEditor::SerializeDamageEffect(const fb::DamageEffectComponentData& c) const
{
    json j;
    j["debugDamage"] = c.m_DebugDamage;
    j["topDamage"] = SerializeVec4(c.m_TopDamage);
    j["bottomDamage"] = SerializeVec4(c.m_BottomDamage);
    j["leftDamage"] = SerializeVec4(c.m_LeftDamage);
    j["rightDamage"] = SerializeVec4(c.m_RightDamage);
    j["frameWidth"] = c.m_FrameWidth;
    j["outerFrameOpacity"] = c.m_OuterFrameOpacity;
    j["innerFrameOpacity"] = c.m_InnerFrameOpacity;
    j["startCriticalEffectHealthThreshold"] = c.m_StartCriticalEffectHealthThreshold;
    j["endCriticalEffectHealthThreshold"] = c.m_EndCriticalEffectHealthThreshold;
    j["minDamagePercentageThreshold"] = c.m_MinDamagePercentageThreshold;
    j["fallofTime"] = c.m_FallofTime;
    j["maxOpacityDamagePercentage"] = c.m_MaxOpacityDamagePercentage;
    return j;
}

json VisualEnvironmentEditor::SerializePlanarReflection(const fb::PlanarReflectionComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["skyRenderEnable"] = c.m_SkyRenderEnable;
    j["groundHeight"] = c.m_GroundHeight;
    j["horizontalBlurFilter"] = static_cast<int>(c.m_HorizontalBlurFilter);
    j["horizontalDeviation"] = c.m_HorizontalDeviation;
    j["verticalBlurFilter"] = static_cast<int>(c.m_VerticalBlurFilter);
    j["verticalDeviation"] = c.m_VerticalDeviation;
    return j;
}

json VisualEnvironmentEditor::SerializeDynamicEnvmap(const fb::DynamicEnvmapComponentData& c) const
{
    json j;
    j["enable"] = c.m_Enable;
    j["skyColorEnvmap"] = SerializeVec3(c.m_SkyColorEnvmap);
    j["groundColorEnvmap"] = SerializeVec3(c.m_GroundColorEnvmap);
    j["keyColorEnvmap"] = SerializeVec3(c.m_KeyColorEnvmap);
    return j;
}

json VisualEnvironmentEditor::SerializeCharacterLighting(const fb::CharacterLightingComponentData& c) const
{
    json j;
    j["characterLightEnable"] = c.m_CharacterLightEnable;
    j["firstPersonEnable"] = c.m_FirstPersonEnable;
    j["lockToCameraDirection"] = c.m_LockToCameraDirection;
    j["characterLightingMode"] = static_cast<int>(c.m_CharacterLightingMode);
    j["topLight"] = SerializeVec3(c.m_TopLight);
    j["bottomLight"] = SerializeVec3(c.m_BottomLight);
    j["topLightDirX"] = c.m_TopLightDirX;
    j["topLightDirY"] = c.m_TopLightDirY;
    j["cameraUpRotation"] = c.m_CameraUpRotation;
    j["blendFactor"] = c.m_BlendFactor;
    return j;
}

json VisualEnvironmentEditor::SerializeMotionBlur(const fb::MotionBlurComponentData& c) const
{
    json j;
    j["motionBlurEnable"] = c.m_MotionBlurEnable;
    j["motionBlurCutoffRadius"] = c.m_MotionBlurCutoffRadius;
    j["cutoffGradientScale"] = c.m_CutoffGradientScale;
    return j;
}

void VisualEnvironmentEditor::DeserializeStateData(const json& j, StateEditData& data)
{
    data.overrideEnabled = j.value("overrideEnabled", true);
    data.captured = true; // trick to mark it to load

#define DESERIALIZE_COMPONENT(name, hasFlag, editMember, deserializeFunc) \
    if (j.contains(name)) { \
        data.hasFlag = true; \
        deserializeFunc(j[name], data.editMember); \
    }

    DESERIALIZE_COMPONENT("outdoorLight", hasOutdoorLight, editOutdoorLight, DeserializeOutdoorLight);
    DESERIALIZE_COMPONENT("enlighten", hasEnlighten, editEnlighten, DeserializeEnlighten);
    DESERIALIZE_COMPONENT("tonemap", hasTonemap, editTonemap, DeserializeTonemap);
    DESERIALIZE_COMPONENT("colorCorrection", hasColorCorrection, editColorCorrection, DeserializeColorCorrection);
    DESERIALIZE_COMPONENT("sky", hasSky, editSky, DeserializeSky);
    DESERIALIZE_COMPONENT("fog", hasFog, editFog, DeserializeFog);
    DESERIALIZE_COMPONENT("wind", hasWind, editWind, DeserializeWind);
    DESERIALIZE_COMPONENT("sunflare", hasSunFlare, editSunFlare, DeserializeSunflare);
    DESERIALIZE_COMPONENT("dynamicAO", hasDynamicAO, editDynamicAO, DeserializeDynamicAO);
    DESERIALIZE_COMPONENT("dof", hasDof, editDof, DeserializeDof);
    DESERIALIZE_COMPONENT("vignette", hasVignette, editVignette, DeserializeVignette);
    DESERIALIZE_COMPONENT("filmGrain", hasFilmGrain, editFilmGrain, DeserializeFilmGrain);
    DESERIALIZE_COMPONENT("lensScope", hasLensScope, editLensScope, DeserializeLensScope);
    DESERIALIZE_COMPONENT("cameraParams", hasCameraParams, editCameraParams, DeserializeCameraParams);
    DESERIALIZE_COMPONENT("screenEffect", hasScreenEffect, editScreenEffect, DeserializeScreenEffect);
    DESERIALIZE_COMPONENT("damageEffect", hasDamageEffect, editDamageEffect, DeserializeDamageEffect);
    DESERIALIZE_COMPONENT("planarReflection", hasPlanarReflection, editPlanarReflection, DeserializePlanarReflection);
    DESERIALIZE_COMPONENT("dynamicEnvmap", hasDynamicEnvmap, editDynamicEnvmap, DeserializeDynamicEnvmap);
    DESERIALIZE_COMPONENT("characterLighting", hasCharacterLighting, editCharacterLighting, DeserializeCharacterLighting);
    DESERIALIZE_COMPONENT("motionBlur", hasMotionBlur, editMotionBlur, DeserializeMotionBlur);

#undef DESERIALIZE_COMPONENT
}

void VisualEnvironmentEditor::DeserializeOutdoorLight(const json& j, fb::OutdoorLightComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_VEC3(j, "sunColor", c.m_SunColor);
    JSON_GET_VEC3(j, "skyColor", c.m_SkyColor);
    JSON_GET_VEC3(j, "groundColor", c.m_GroundColor);
    JSON_GET(j, "sunRotationX", c.m_SunRotationX);
    JSON_GET(j, "sunRotationY", c.m_SunRotationY);
    JSON_GET(j, "sunSpecularScale", c.m_SunSpecularScale);
    JSON_GET(j, "skyLightAngleFactor", c.m_SkyLightAngleFactor);
    JSON_GET(j, "skyEnvmapShadowScale", c.m_SkyEnvmapShadowScale);
    JSON_GET(j, "sunShadowHeightScale", c.m_SunShadowHeightScale);
    JSON_GET_BOOL(j, "cloudShadowEnable", c.m_CloudShadowEnable);
    JSON_GET_VEC2(j, "cloudShadowSpeed", c.m_CloudShadowSpeed);
    JSON_GET(j, "cloudShadowCoverage", c.m_CloudShadowCoverage);
    JSON_GET(j, "cloudShadowSize", c.m_CloudShadowSize);
    JSON_GET(j, "cloudShadowExponent", c.m_CloudShadowExponent);
    JSON_GET(j, "translucencyDistortion", c.m_TranslucencyDistortion);
    JSON_GET(j, "translucencyAmbient", c.m_TranslucencyAmbient);
    JSON_GET(j, "translucencyScale", c.m_TranslucencyScale);
    JSON_GET(j, "translucencyPower", c.m_TranslucencyPower);
}

void VisualEnvironmentEditor::DeserializeEnlighten(const json& j, fb::EnlightenComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_BOOL(j, "skyBoxEnable", c.m_SkyBoxEnable);
    JSON_GET_VEC3(j, "skyBoxSkyColor", c.m_SkyBoxSkyColor);
    JSON_GET_VEC3(j, "skyBoxGroundColor", c.m_SkyBoxGroundColor);
    JSON_GET_VEC3(j, "skyBoxSunLightColor", c.m_SkyBoxSunLightColor);
    JSON_GET_VEC3(j, "skyBoxBackLightColor", c.m_SkyBoxBackLightColor);
    JSON_GET_VEC3(j, "terrainColor", c.m_TerrainColor);
    JSON_GET(j, "bounceScale", c.m_BounceScale);
    JSON_GET(j, "sunScale", c.m_SunScale);
    JSON_GET(j, "cullDistance", c.m_CullDistance);
    JSON_GET(j, "skyBoxSunLightColorSize", c.m_SkyBoxSunLightColorSize);
    JSON_GET(j, "skyBoxBackLightColorSize", c.m_SkyBoxBackLightColorSize);
    JSON_GET(j, "skyBoxBackLightRotationX", c.m_SkyBoxBackLightRotationX);
    JSON_GET(j, "skyBoxBackLightRotationY", c.m_SkyBoxBackLightRotationY);
}

void VisualEnvironmentEditor::DeserializeTonemap(const json& j, fb::TonemapComponentData& c)
{
    JSON_GET_ENUM(j, "tonemapMethod", c.m_TonemapMethod, fb::TonemapMethod);
    JSON_GET_VEC3(j, "bloomScale", c.m_BloomScale);
    JSON_GET(j, "minExposure", c.m_MinExposure);
    JSON_GET(j, "maxExposure", c.m_MaxExposure);
    JSON_GET(j, "middleGray", c.m_MiddleGray);
    JSON_GET(j, "exposureAdjustTime", c.m_ExposureAdjustTime);
    JSON_GET_BOOL(j, "chromostereopsisEnable", c.m_ChromostereopsisEnable);
    JSON_GET(j, "chromostereopsisOffset", c.m_ChromostereopsisOffset);
    JSON_GET(j, "chromostereopsisScale", c.m_ChromostereopsisScale);
}

void VisualEnvironmentEditor::DeserializeColorCorrection(const json& j, fb::ColorCorrectionComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_BOOL(j, "colorGradingEnable", c.m_ColorGradingEnable);
    JSON_GET_VEC3(j, "brightness", c.m_Brightness);
    JSON_GET_VEC3(j, "contrast", c.m_Contrast);
    JSON_GET_VEC3(j, "saturation", c.m_Saturation);
    JSON_GET(j, "hue", c.m_Hue);
}

void VisualEnvironmentEditor::DeserializeSky(const json& j, fb::SkyComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET(j, "brightnessScale", c.m_BrightnessScale);
    JSON_GET(j, "sunSize", c.m_SunSize);
    JSON_GET(j, "sunScale", c.m_SunScale);
    JSON_GET(j, "skyVisibilityExponent", c.m_SkyVisibilityExponent);
    JSON_GET(j, "skyEnvmap8BitTexScale", c.m_SkyEnvmap8BitTexScale);
    JSON_GET_VEC3(j, "cloudLayerSunColor", c.m_CloudLayerSunColor);
    JSON_GET_VEC3(j, "cloudLayer1Color", c.m_CloudLayer1Color);
    JSON_GET(j, "cloudLayer1Altitude", c.m_CloudLayer1Altitude);
    JSON_GET(j, "cloudLayer1TileFactor", c.m_CloudLayer1TileFactor);
    JSON_GET(j, "cloudLayer1Rotation", c.m_CloudLayer1Rotation);
    JSON_GET(j, "cloudLayer1Speed", c.m_CloudLayer1Speed);
    JSON_GET(j, "cloudLayer1SunLightIntensity", c.m_CloudLayer1SunLightIntensity);
    JSON_GET(j, "cloudLayer1SunLightPower", c.m_CloudLayer1SunLightPower);
    JSON_GET(j, "cloudLayer1AmbientLightIntensity", c.m_CloudLayer1AmbientLightIntensity);
    JSON_GET(j, "cloudLayer1AlphaMul", c.m_CloudLayer1AlphaMul);
    JSON_GET_VEC3(j, "cloudLayer2Color", c.m_CloudLayer2Color);
    JSON_GET(j, "cloudLayer2Altitude", c.m_CloudLayer2Altitude);
    JSON_GET(j, "cloudLayer2TileFactor", c.m_CloudLayer2TileFactor);
    JSON_GET(j, "cloudLayer2Rotation", c.m_CloudLayer2Rotation);
    JSON_GET(j, "cloudLayer2Speed", c.m_CloudLayer2Speed);
    JSON_GET(j, "cloudLayer2SunLightIntensity", c.m_CloudLayer2SunLightIntensity);
    JSON_GET(j, "cloudLayer2SunLightPower", c.m_CloudLayer2SunLightPower);
    JSON_GET(j, "cloudLayer2AmbientLightIntensity", c.m_CloudLayer2AmbientLightIntensity);
    JSON_GET(j, "cloudLayer2AlphaMul", c.m_CloudLayer2AlphaMul);
    JSON_GET(j, "panoramicRotation", c.m_PanoramicRotation);
    JSON_GET(j, "panoramicTileFactor", c.m_PanoramicTileFactor);
    JSON_GET(j, "panoramicUVMinX", c.m_PanoramicUVMinX);
    JSON_GET(j, "panoramicUVMaxX", c.m_PanoramicUVMaxX);
    JSON_GET(j, "panoramicUVMinY", c.m_PanoramicUVMinY);
    JSON_GET(j, "panoramicUVMaxY", c.m_PanoramicUVMaxY);
    JSON_GET(j, "customEnvmapScale", c.m_CustomEnvmapScale);
    JSON_GET(j, "customEnvmapAmbient", c.m_CustomEnvmapAmbient);
    JSON_GET(j, "staticEnvmapScale", c.m_StaticEnvmapScale);
}

void VisualEnvironmentEditor::DeserializeFog(const json& j, fb::FogComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET(j, "start", c.m_Start);
    JSON_GET(j, "end", c.m_End);
    JSON_GET(j, "fogDistanceMultiplier", c.m_FogDistanceMultiplier);
    JSON_GET_VEC4(j, "curve", c.m_Curve);
    JSON_GET_BOOL(j, "fogColorEnable", c.m_FogColorEnable);
    JSON_GET_VEC3(j, "fogColor", c.m_FogColor);
    JSON_GET(j, "fogColorStart", c.m_FogColorStart);
    JSON_GET(j, "fogColorEnd", c.m_FogColorEnd);
    JSON_GET_VEC4(j, "fogColorCurve", c.m_FogColorCurve);
    JSON_GET_BOOL(j, "fogGradientEnable", c.m_FogGradientEnable);
    JSON_GET_BOOL(j, "heightFogEnable", c.m_HeightFogEnable);
    JSON_GET(j, "heightFogAltitude", c.m_HeightFogAltitude);
    JSON_GET(j, "heightFogDepth", c.m_HeightFogDepth);
    JSON_GET(j, "heightFogVisibilityRange", c.m_HeightFogVisibilityRange);
    JSON_GET(j, "heightFogFollowCamera", c.m_HeightFogFollowCamera);
    JSON_GET(j, "transparencyFadeStart", c.m_TransparencyFadeStart);
    JSON_GET(j, "transparencyFadeEnd", c.m_TransparencyFadeEnd);
    JSON_GET(j, "transparencyFadeClamp", c.m_TransparencyFadeClamp);
}

void VisualEnvironmentEditor::DeserializeWind(const json& j, fb::WindComponentData& c)
{
    JSON_GET(j, "windDirection", c.m_WindDirection);
    JSON_GET(j, "windStrength", c.m_WindStrength);
}

void VisualEnvironmentEditor::DeserializeSunflare(const json& j, fb::SunFlareComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_BOOL(j, "debugDrawOccluder", c.m_DebugDrawOccluder);
    JSON_GET(j, "occluderSize", c.m_OccluderSize);

#define DESERIALIZE_ELEMENT(n) \
    JSON_GET_BOOL(j, "element" #n "Enable", c.m_Element##n##Enable); \
    JSON_GET_VEC2(j, "element" #n "Size", c.m_Element##n##Size); \
    JSON_GET(j, "element" #n "RayDistance", c.m_Element##n##RayDistance);

    DESERIALIZE_ELEMENT(1) DESERIALIZE_ELEMENT(2) DESERIALIZE_ELEMENT(3)
        DESERIALIZE_ELEMENT(4) DESERIALIZE_ELEMENT(5)

#undef DESERIALIZE_ELEMENT
}

void VisualEnvironmentEditor::DeserializeDynamicAO(const json& j, fb::DynamicAOComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET(j, "ssaoRadius", c.m_SsaoRadius);
    JSON_GET(j, "ssaoFade", c.m_SsaoFade);
    JSON_GET(j, "ssaoMaxDistanceInner", c.m_SsaoMaxDistanceInner);
    JSON_GET(j, "ssaoMaxDistanceOuter", c.m_SsaoMaxDistanceOuter);
    JSON_GET(j, "hbaoRadius", c.m_HbaoRadius);
    JSON_GET(j, "hbaoAttenuation", c.m_HbaoAttenuation);
    JSON_GET(j, "hbaoAngleBias", c.m_HbaoAngleBias);
    JSON_GET(j, "hbaoPowerExponent", c.m_HbaoPowerExponent);
    JSON_GET(j, "hbaoContrast", c.m_HbaoContrast);
    JSON_GET(j, "hbaoMaxFootprintRadius", c.m_HbaoMaxFootprintRadius);
}

void VisualEnvironmentEditor::DeserializeDof(const json& j, fb::DofComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_ENUM(j, "blurFilter", c.m_BlurFilter, fb::BlurFilter);
    JSON_GET(j, "focusDistance", c.m_FocusDistance);
    JSON_GET(j, "nearDistanceScale", c.m_NearDistanceScale);
    JSON_GET(j, "farDistanceScale", c.m_FarDistanceScale);
    JSON_GET(j, "blurFilterDeviation", c.m_BlurFilterDeviation);
    JSON_GET(j, "blurAdd", c.m_BlurAdd);
    JSON_GET(j, "scale", c.m_Scale);
    JSON_GET_BOOL(j, "diffusionDofEnable", c.m_DiffusionDofEnable);
    JSON_GET(j, "diffusionDofFocalLength", c.m_DiffusionDofFocalLength);
    JSON_GET(j, "diffusionDofAperture", c.m_DiffusionDofAperture);
}

void VisualEnvironmentEditor::DeserializeVignette(const json& j, fb::VignetteComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_VEC3(j, "color", c.m_Color);
    JSON_GET_VEC2(j, "scale", c.m_Scale);
    JSON_GET(j, "opacity", c.m_Opacity);
    JSON_GET(j, "exponent", c.m_Exponent);
}

void VisualEnvironmentEditor::DeserializeFilmGrain(const json& j, fb::FilmGrainComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_BOOL(j, "linearFilteringEnable", c.m_LinearFilteringEnable);
    JSON_GET_BOOL(j, "randomEnable", c.m_RandomEnable);
    JSON_GET_VEC3(j, "colorScale", c.m_ColorScale);
    JSON_GET_VEC2(j, "textureScale", c.m_TextureScale);
}

void VisualEnvironmentEditor::DeserializeLensScope(const json& j, fb::LensScopeComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_VEC2(j, "blurCenter", c.m_BlurCenter);
    JSON_GET(j, "blurScale", c.m_BlurScale);
    JSON_GET_VEC2(j, "radialBlendDistanceCoefficients", c.m_RadialBlendDistanceCoefficients);
    JSON_GET_VEC3(j, "chromaticAberrationColor1", c.m_ChromaticAberrationColor1);
    JSON_GET_VEC3(j, "chromaticAberrationColor2", c.m_ChromaticAberrationColor2);
    JSON_GET_VEC2(j, "chromaticAberrationDisplacement1", c.m_ChromaticAberrationDisplacement1);
    JSON_GET_VEC2(j, "chromaticAberrationDisplacement2", c.m_ChromaticAberrationDisplacement2);
    JSON_GET_VEC2(j, "chromaticAberrationStrengths", c.m_ChromaticAberrationStrengths);
}

void VisualEnvironmentEditor::DeserializeCameraParams(const json& j, fb::CameraParamsComponentData& c)
{
    JSON_GET(j, "viewDistance", c.m_ViewDistance);
    JSON_GET(j, "nearPlane", c.m_NearPlane);
    JSON_GET(j, "sunShadowmapViewDistance", c.m_SunShadowmapViewDistance);
}

void VisualEnvironmentEditor::DeserializeScreenEffect(const json& j, fb::ScreenEffectComponentData& c)
{
    JSON_GET_ENUM(j, "frameType", c.m_FrameType, fb::ScreenEffectFrameType);
    JSON_GET_VEC4(j, "screenEffectParams", c.m_ScreenEffectParams);
    JSON_GET(j, "frameWidth", c.m_FrameWidth);
    JSON_GET(j, "outerFrameOpacity", c.m_OuterFrameOpacity);
    JSON_GET(j, "innerFrameOpacity", c.m_InnerFrameOpacity);
    JSON_GET(j, "angle", c.m_Angle);
}

void VisualEnvironmentEditor::DeserializeDamageEffect(const json& j, fb::DamageEffectComponentData& c)
{
    JSON_GET_BOOL(j, "debugDamage", c.m_DebugDamage);
    JSON_GET_VEC4(j, "topDamage", c.m_TopDamage);
    JSON_GET_VEC4(j, "bottomDamage", c.m_BottomDamage);
    JSON_GET_VEC4(j, "leftDamage", c.m_LeftDamage);
    JSON_GET_VEC4(j, "rightDamage", c.m_RightDamage);
    JSON_GET(j, "frameWidth", c.m_FrameWidth);
    JSON_GET(j, "outerFrameOpacity", c.m_OuterFrameOpacity);
    JSON_GET(j, "innerFrameOpacity", c.m_InnerFrameOpacity);
    JSON_GET(j, "startCriticalEffectHealthThreshold", c.m_StartCriticalEffectHealthThreshold);
    JSON_GET(j, "endCriticalEffectHealthThreshold", c.m_EndCriticalEffectHealthThreshold);
    JSON_GET(j, "minDamagePercentageThreshold", c.m_MinDamagePercentageThreshold);
    JSON_GET(j, "fallofTime", c.m_FallofTime);
    JSON_GET(j, "maxOpacityDamagePercentage", c.m_MaxOpacityDamagePercentage);
}

void VisualEnvironmentEditor::DeserializePlanarReflection(const json& j, fb::PlanarReflectionComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_BOOL(j, "skyRenderEnable", c.m_SkyRenderEnable);
    JSON_GET(j, "groundHeight", c.m_GroundHeight);
    JSON_GET_ENUM(j, "horizontalBlurFilter", c.m_HorizontalBlurFilter, fb::BlurFilter);
    JSON_GET(j, "horizontalDeviation", c.m_HorizontalDeviation);
    JSON_GET_ENUM(j, "verticalBlurFilter", c.m_VerticalBlurFilter, fb::BlurFilter);
    JSON_GET(j, "verticalDeviation", c.m_VerticalDeviation);
}

void VisualEnvironmentEditor::DeserializeDynamicEnvmap(const json& j, fb::DynamicEnvmapComponentData& c)
{
    JSON_GET_BOOL(j, "enable", c.m_Enable);
    JSON_GET_VEC3(j, "skyColorEnvmap", c.m_SkyColorEnvmap);
    JSON_GET_VEC3(j, "groundColorEnvmap", c.m_GroundColorEnvmap);
    JSON_GET_VEC3(j, "keyColorEnvmap", c.m_KeyColorEnvmap);
}

void VisualEnvironmentEditor::DeserializeCharacterLighting(const json& j, fb::CharacterLightingComponentData& c)
{
    JSON_GET_BOOL(j, "characterLightEnable", c.m_CharacterLightEnable);
    JSON_GET_BOOL(j, "firstPersonEnable", c.m_FirstPersonEnable);
    JSON_GET_BOOL(j, "lockToCameraDirection", c.m_LockToCameraDirection);
    JSON_GET_ENUM(j, "characterLightingMode", c.m_CharacterLightingMode, fb::CharacterLightingMode);
    JSON_GET_VEC3(j, "topLight", c.m_TopLight);
    JSON_GET_VEC3(j, "bottomLight", c.m_BottomLight);
    JSON_GET(j, "topLightDirX", c.m_TopLightDirX);
    JSON_GET(j, "topLightDirY", c.m_TopLightDirY);
    JSON_GET(j, "cameraUpRotation", c.m_CameraUpRotation);
    JSON_GET(j, "blendFactor", c.m_BlendFactor);
}

void VisualEnvironmentEditor::DeserializeMotionBlur(const json& j, fb::MotionBlurComponentData& c)
{
    JSON_GET_BOOL(j, "motionBlurEnable", c.m_MotionBlurEnable);
    JSON_GET(j, "motionBlurCutoffRadius", c.m_MotionBlurCutoffRadius);
    JSON_GET(j, "cutoffGradientScale", c.m_CutoffGradientScale);
}

bool VisualEnvironmentEditor::SaveConfig(const std::filesystem::path& filePath)
{
    try
    {
        json root;

        root["version"] = 2; // this is only to later not fugg up config-based changes on loading old configs
        root["mapName"] = m_CapturedMapName;
        root["timestamp"] = std::time(nullptr);

        json statesArray = json::array();

        for (const auto& entry : m_EditList)
        {
            json stateJson;

            stateJson["hash"]["priority"] = entry.hash.priority;
            stateJson["hash"]["visibility"] = entry.hash.visibility;
            stateJson["hash"]["componentMask"] = entry.hash.componentMask;
            stateJson["editData"] = SerializeStateData(entry.editData);

            statesArray.push_back(stateJson);
        }

        root["states"] = statesArray;

        if (m_GlobalVEData.captured)
        {
            json globalJson;
            globalJson["globalOverrideEnabled"] = m_GlobalVEData.globalOverrideEnabled;

#define SAVE_GLOBAL_COMPONENT(name, enabled, edit, serialize) \
                globalJson[#name]["enabled"] = m_GlobalVEData.enabled; \
                if (m_GlobalVEData.enabled) { \
                    globalJson[#name]["data"] = serialize(m_GlobalVEData.edit); \
                }

            SAVE_GLOBAL_COMPONENT(outdoorLight, outdoorLightOverrideEnabled, editOutdoorLight, SerializeOutdoorLight);
            SAVE_GLOBAL_COMPONENT(enlighten, enlightenOverrideEnabled, editEnlighten, SerializeEnlighten);
            SAVE_GLOBAL_COMPONENT(tonemap, tonemapOverrideEnabled, editTonemap, SerializeTonemap);
            SAVE_GLOBAL_COMPONENT(colorCorrection, colorCorrectionOverrideEnabled, editColorCorrection, SerializeColorCorrection);
            SAVE_GLOBAL_COMPONENT(sky, skyOverrideEnabled, editSky, SerializeSky);
            SAVE_GLOBAL_COMPONENT(fog, fogOverrideEnabled, editFog, SerializeFog);
            SAVE_GLOBAL_COMPONENT(wind, windOverrideEnabled, editWind, SerializeWind);
            SAVE_GLOBAL_COMPONENT(sunFlare, sunFlareOverrideEnabled, editSunFlare, SerializeSunflare);
            SAVE_GLOBAL_COMPONENT(dynamicAO, dynamicAOOverrideEnabled, editDynamicAO, SerializeDynamicAO);
            SAVE_GLOBAL_COMPONENT(dof, dofOverrideEnabled, editDof, SerializeDof);
            SAVE_GLOBAL_COMPONENT(vignette, vignetteOverrideEnabled, editVignette, SerializeVignette);
            SAVE_GLOBAL_COMPONENT(filmGrain, filmGrainOverrideEnabled, editFilmGrain, SerializeFilmGrain);
            SAVE_GLOBAL_COMPONENT(lensScope, lensScopeOverrideEnabled, editLensScope, SerializeLensScope);
            SAVE_GLOBAL_COMPONENT(cameraParams, cameraParamsOverrideEnabled, editCameraParams, SerializeCameraParams);
            SAVE_GLOBAL_COMPONENT(screenEffect, screenEffectOverrideEnabled, editScreenEffect, SerializeScreenEffect);
            SAVE_GLOBAL_COMPONENT(damageEffect, damageEffectOverrideEnabled, editDamageEffect, SerializeDamageEffect);
            SAVE_GLOBAL_COMPONENT(planarReflection, planarReflectionOverrideEnabled, editPlanarReflection, SerializePlanarReflection);
            SAVE_GLOBAL_COMPONENT(dynamicEnvmap, dynamicEnvmapOverrideEnabled, editDynamicEnvmap, SerializeDynamicEnvmap);
            SAVE_GLOBAL_COMPONENT(characterLighting, characterLightingOverrideEnabled, editCharacterLighting, SerializeCharacterLighting);
            SAVE_GLOBAL_COMPONENT(motionBlur, motionBlurOverrideEnabled, editMotionBlur, SerializeMotionBlur);

#undef SAVE_GLOBAL_COMPONENT

            root["globalVE"] = globalJson;
        }

        if (m_HasCapturedWorldRenderSettings && m_WorldRenderOverrideEnabled)
        {
            json wrsJson;
            wrsJson["enabled"] = m_WorldRenderOverrideEnabled;
            wrsJson["viewportScale"] = m_EditWorldRenderSettings.m_viewportScale;
            wrsJson["fxaaQuality"] = m_EditWorldRenderSettings.m_fxaaQuality;
            wrsJson["shadowmapResolution"] = m_EditWorldRenderSettings.m_shadowmapResolution;
            wrsJson["shadowmapQuality"] = m_EditWorldRenderSettings.m_shadowmapQuality;
            wrsJson["shadowViewDistance"] = m_EditWorldRenderSettings.m_shadowViewDistance;
            wrsJson["motionBlurScale"] = m_EditWorldRenderSettings.m_motionBlurScale;
            wrsJson["motionBlurMax"] = m_EditWorldRenderSettings.m_motionBlurMax;
            wrsJson["motionBlurQuality"] = m_EditWorldRenderSettings.m_motionBlurQuality;
            wrsJson["drawShadows"] = m_EditWorldRenderSettings.m_drawShadows;
            wrsJson["skyEnable"] = m_EditWorldRenderSettings.m_skyEnable;
            wrsJson["sunEnabled"] = m_EditWorldRenderSettings.m_sunEnabled;
            wrsJson["depthOfField"] = m_EditWorldRenderSettings.m_depthOfField;
            wrsJson["drawReflection"] = m_EditWorldRenderSettings.m_drawReflection;

            root["worldRender"] = wrsJson;
        }

        {
            std::lock_guard<std::mutex> lock(m_LightDataMutex);
            json lightsArray = json::array();

            for (const auto& [dataPtr, entry] : m_LightDataEntries)
            {
                if (entry.hasOverride && entry.origCaptured)
                {
                    lightsArray.push_back(SerializeLightData(entry));
                }
            }

            if (!lightsArray.empty())
            {
                root["lights"] = lightsArray;
            }
        }

        std::ofstream file{ filePath };
        if (!file.is_open())
        {
            LOG_ERROR("SaveConfig: Failed to open file for writing: %s", filePath.c_str());
            return false;
        }

        file << root.dump(2);
        file.close();

        LOG_INFO("SaveConfig: Saved %zu states, %zu lights to %s", m_EditList.size(), root.contains("lights") ? root["lights"].size() : 0, filePath.c_str());

        return true;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("SaveConfig: Exception: %s", e.what());
        return false;
    }
}

bool VisualEnvironmentEditor::LoadConfig(const std::filesystem::path& filePath)
{
    try
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            LOG_ERROR("LoadConfig: Failed to open file: %s", filePath.c_str());
            return false;
        }

        json root = json::parse(file);
        file.close();

        // need to verify here, prob use macro from api.h
        int version = root.value("version", 1);
        LOG_INFO("LoadConfig: Loading config version %d from %s", version, filePath.c_str());

        LOG_INFO("LoadConfig: m_StateOrder has %zu states", m_StateOrder.size());
        for (size_t i = 0; i < m_StateOrder.size(); i++)
        {
            auto state = m_StateOrder[i];
            if (state)
            {
                StateHash h = ComputeStateHash(state);
                LOG_INFO("  [%zu] P%d M%08X V%.2f", i, h.priority, h.componentMask, h.visibility);
            }
        }

        if (root.contains("states") && root["states"].is_array())
        {
            LOG_INFO("LoadConfig: Found %zu states in config", root["states"].size());

            int loadedCount = 0;
            int appliedCount = 0;

            for (const auto& stateJson : root["states"])
            {
                int savedPriority = 0;
                uint32_t savedMask = 0;

                if (stateJson.contains("hash"))
                {
                    savedPriority = stateJson["hash"].value("priority", 0);
                    savedMask = stateJson["hash"].value("componentMask", 0u);
                    LOG_INFO("LoadConfig: Reading state P%d M%08X from config", savedPriority, savedMask);
                }
                else
                {
                    LOG_WARN("LoadConfig: State entry missing hash, skipping");
                    continue;
                }

                StateEditData editData;
                if (stateJson.contains("editData"))
                {
                    DeserializeStateData(stateJson["editData"], editData);
                    editData.overrideEnabled = true;
                    LOG_INFO("LoadConfig: Deserialized editData, hasOutdoorLight=%d, hasFog=%d, compCount=%d",
                        editData.hasOutdoorLight, editData.hasFog, editData.ComponentCount());
                }
                else
                {
                    LOG_WARN("LoadConfig: State entry missing editData, skipping");
                    continue;
                }

                loadedCount++;

                bool found = false;
                for (auto state : m_StateOrder)
                {
                    if (!state)
                        continue;

                    StateHash currentHash = ComputeStateHash(state);

                    if (currentHash.priority == savedPriority &&
                        currentHash.componentMask == savedMask)
                    {
                        LOG_INFO("LoadConfig: MATCHED state P%d to live state!", savedPriority);

                        auto itr = m_StateDataMap.find(state);
                        if (itr != m_StateDataMap.end())
                        {
                            LOG_INFO("LoadConfig: Found in StateDataMap, dst.hasOutdoorLight=%d, dst.hasFog=%d", itr->second.hasOutdoorLight, itr->second.hasFog);

                            CopyEditDataDirectly(itr->second, editData);
                            itr->second.overrideEnabled = true;

                            LOG_INFO("LoadConfig: After copy, dst.hasOutdoorLight=%d", itr->second.hasOutdoorLight);
                        }
                        else
                        {
                            LOG_WARN("LoadConfig: State not in m_StateDataMap!");
                        }

                        StateEditEntry entry;
                        entry.hash = currentHash;
                        entry.editData = itr->second;

                        if (auto existing = FindEditEntry(currentHash))
                        {
                            existing->editData = entry.editData;
                        }
                        else
                        {
                            m_EditList.push_back(std::move(entry));
                        }

                        ApplyStateEdits(state, itr->second);
                        LOG_INFO("LoadConfig: Applied edits to state P%d", savedPriority);

                        appliedCount++;
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    LOG_INFO("LoadConfig: State P%d M%08X NOT FOUND in m_StateOrder, saving to edit list", savedPriority, savedMask);

                    StateEditEntry entry;
                    entry.hash.priority = savedPriority;
                    entry.hash.componentMask = savedMask;
                    entry.hash.visibility = 1.0f;
                    entry.editData = editData;
                    m_EditList.push_back(std::move(entry));
                }
            }

            LOG_INFO("LoadConfig: Loaded %d states, applied %d immediately", loadedCount, appliedCount);
        }
        else
        {
            LOG_WARN("LoadConfig: No 'states' array in config!");
        }

        if (root.contains("globalVE"))
        {
            const auto& globalJson = root["globalVE"];
            m_GlobalVEData.globalOverrideEnabled = globalJson.value("globalOverrideEnabled", false);

#define LOAD_GLOBAL_COMPONENT(name, enabled, edit, deserialize) \
                if (globalJson.contains(#name)) { \
                    m_GlobalVEData.enabled = globalJson[#name].value("enabled", false); \
                    if (m_GlobalVEData.enabled && globalJson[#name].contains("data")) { \
                        deserialize(globalJson[#name]["data"], m_GlobalVEData.edit); \
                    } \
                }

            LOAD_GLOBAL_COMPONENT(outdoorLight, outdoorLightOverrideEnabled, editOutdoorLight, DeserializeOutdoorLight);
            LOAD_GLOBAL_COMPONENT(enlighten, enlightenOverrideEnabled, editEnlighten, DeserializeEnlighten);
            LOAD_GLOBAL_COMPONENT(tonemap, tonemapOverrideEnabled, editTonemap, DeserializeTonemap);
            LOAD_GLOBAL_COMPONENT(colorCorrection, colorCorrectionOverrideEnabled, editColorCorrection, DeserializeColorCorrection);
            LOAD_GLOBAL_COMPONENT(sky, skyOverrideEnabled, editSky, DeserializeSky);
            LOAD_GLOBAL_COMPONENT(fog, fogOverrideEnabled, editFog, DeserializeFog);
            LOAD_GLOBAL_COMPONENT(wind, windOverrideEnabled, editWind, DeserializeWind);
            LOAD_GLOBAL_COMPONENT(sunFlare, sunFlareOverrideEnabled, editSunFlare, DeserializeSunflare);
            LOAD_GLOBAL_COMPONENT(dynamicAO, dynamicAOOverrideEnabled, editDynamicAO, DeserializeDynamicAO);
            LOAD_GLOBAL_COMPONENT(dof, dofOverrideEnabled, editDof, DeserializeDof);
            LOAD_GLOBAL_COMPONENT(vignette, vignetteOverrideEnabled, editVignette, DeserializeVignette);
            LOAD_GLOBAL_COMPONENT(filmGrain, filmGrainOverrideEnabled, editFilmGrain, DeserializeFilmGrain);
            LOAD_GLOBAL_COMPONENT(lensScope, lensScopeOverrideEnabled, editLensScope, DeserializeLensScope);
            LOAD_GLOBAL_COMPONENT(cameraParams, cameraParamsOverrideEnabled, editCameraParams, DeserializeCameraParams);
            LOAD_GLOBAL_COMPONENT(screenEffect, screenEffectOverrideEnabled, editScreenEffect, DeserializeScreenEffect);
            LOAD_GLOBAL_COMPONENT(damageEffect, damageEffectOverrideEnabled, editDamageEffect, DeserializeDamageEffect);
            LOAD_GLOBAL_COMPONENT(planarReflection, planarReflectionOverrideEnabled, editPlanarReflection, DeserializePlanarReflection);
            LOAD_GLOBAL_COMPONENT(dynamicEnvmap, dynamicEnvmapOverrideEnabled, editDynamicEnvmap, DeserializeDynamicEnvmap);
            LOAD_GLOBAL_COMPONENT(characterLighting, characterLightingOverrideEnabled, editCharacterLighting, DeserializeCharacterLighting);
            LOAD_GLOBAL_COMPONENT(motionBlur, motionBlurOverrideEnabled, editMotionBlur, DeserializeMotionBlur);

#undef LOAD_GLOBAL_COMPONENT
        }

        if (root.contains("worldRender"))
        {
            const auto& wrsJson = root["worldRender"];
            m_WorldRenderOverrideEnabled = wrsJson.value("enabled", false);

            if (m_WorldRenderOverrideEnabled)
            {
                m_EditWorldRenderSettings.m_viewportScale = wrsJson.value("viewportScale", m_EditWorldRenderSettings.m_viewportScale);
                m_EditWorldRenderSettings.m_fxaaQuality = wrsJson.value("fxaaQuality", m_EditWorldRenderSettings.m_fxaaQuality);
                m_EditWorldRenderSettings.m_shadowmapResolution = wrsJson.value("shadowmapResolution", m_EditWorldRenderSettings.m_shadowmapResolution);
                m_EditWorldRenderSettings.m_shadowmapQuality = wrsJson.value("shadowmapQuality", m_EditWorldRenderSettings.m_shadowmapQuality);
                m_EditWorldRenderSettings.m_shadowViewDistance = wrsJson.value("shadowViewDistance", m_EditWorldRenderSettings.m_shadowViewDistance);
                m_EditWorldRenderSettings.m_motionBlurScale = wrsJson.value("motionBlurScale", m_EditWorldRenderSettings.m_motionBlurScale);
                m_EditWorldRenderSettings.m_motionBlurMax = wrsJson.value("motionBlurMax", m_EditWorldRenderSettings.m_motionBlurMax);
                m_EditWorldRenderSettings.m_motionBlurQuality = wrsJson.value("motionBlurQuality", m_EditWorldRenderSettings.m_motionBlurQuality);
                m_EditWorldRenderSettings.m_drawShadows = wrsJson.value("drawShadows", m_EditWorldRenderSettings.m_drawShadows);
                m_EditWorldRenderSettings.m_skyEnable = wrsJson.value("skyEnable", m_EditWorldRenderSettings.m_skyEnable);
                m_EditWorldRenderSettings.m_sunEnabled = wrsJson.value("sunEnabled", m_EditWorldRenderSettings.m_sunEnabled);
                m_EditWorldRenderSettings.m_depthOfField = wrsJson.value("depthOfField", m_EditWorldRenderSettings.m_depthOfField);
                m_EditWorldRenderSettings.m_drawReflection = wrsJson.value("drawReflection", m_EditWorldRenderSettings.m_drawReflection);
            }
        }

        if (root.contains("lights") && root["lights"].is_array())
        {
            std::lock_guard<std::mutex> lock(m_LightDataMutex);

            for (const auto& lightJson : root["lights"])
            {
                std::string assetName = lightJson.value("assetName", "");
                if (assetName.empty())
                    continue;

                for (auto& [dataPtr, entry] : m_LightDataEntries)
                {
                    if (entry.assetName == assetName)
                    {
                        DeserializeLightData(lightJson, entry);
                        entry.hasOverride = true;

                        if (!entry.activeEntities.empty())
                        {
                            ApplyLightOverride(entry);
                        }
                        break;
                    }
                }
            }
        }

        fb::VisualEnvironmentManager* manager = GetManager();
        if (manager)
            manager->setDirty(true);

        LOG_INFO("LoadConfig: Successfully loaded config from %s", filePath.c_str());
        return true;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("LoadConfig: Exception: %s", e.what());
        return false;
    }
}

bool VisualEnvironmentEditor::SaveConfigForCurrentMap()
{
    if (m_CapturedMapName.empty())
        return false;
    return SaveConfig(GetConfigPath(m_CapturedMapName));
}

bool VisualEnvironmentEditor::LoadConfigForCurrentMap()
{
    std::string mapName = GetCurrentMapName();
    if (mapName.empty())
        return false;
    std::string path = GetConfigPath(mapName);
    if (!fs::exists(path))
        return false;
    return LoadConfig(path);
}