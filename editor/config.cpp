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

json VisualEnvironmentEditor::SerializeWorldRenderSettings(const fb::WorldRenderSettings& s) const
{
    json j;
    j["ViewportScale"] = s.m_ViewportScale;
    j["FxaaQuality"] = s.m_FxaaQuality;
    j["FxaaForceVendor"] = s.m_FxaaForceVendor;
    j["FxaaEnable"] = s.m_FxaaEnable;
    j["CullScreenAreaScale"] = s.m_CullScreenAreaScale;
    j["MultisampleCount"] = s.m_MultisampleCount;
    j["MultisampleThreshold"] = s.m_MultisampleThreshold;
    j["HdrEnable"] = s.m_HdrEnable;
    j["HalfResEnable"] = s.m_HalfResEnable;
    j["BilateralHalfResCompositeEnable"] = s.m_BilateralHalfResCompositeEnable;
    j["TiledHalfResCompositeEnable"] = s.m_TiledHalfResCompositeEnable;
    j["TiledHalfResStencilOccludersEnable"] = s.m_TiledHalfResStencilOccludersEnable;
    j["OutputGammaCorrectionEnable"] = s.m_OutputGammaCorrectionEnable;
    j["Enable"] = s.m_Enable;

    j["ShadowmapsEnable"] = s.m_ShadowmapsEnable;
    j["GenerateShadowmapsEnable"] = s.m_GenerateShadowmapsEnable;
    j["ApplyShadowmapsEnable"] = s.m_ApplyShadowmapsEnable;
    j["SimpleShadowmapsEnable"] = s.m_SimpleShadowmapsEnable;
    j["TransparencyShadowmapsEnable"] = s.m_TransparencyShadowmapsEnable;
    j["CloudShadowEnable"] = s.m_CloudShadowEnable;
    j["ShadowmapResolution"] = s.m_ShadowmapResolution;
    j["ShadowmapQuality"] = s.m_ShadowmapQuality;
    j["ShadowmapSliceCount"] = s.m_ShadowmapSliceCount;
    j["ShadowmapViewDistance"] = s.m_ShadowmapViewDistance;
    j["ShadowmapViewDistanceScaleEnable"] = s.m_ShadowmapViewDistanceScaleEnable;
    j["ShadowMinScreenArea"] = s.m_ShadowMinScreenArea;
    j["ShadowmapMinScreenArea"] = s.m_ShadowmapMinScreenArea;
    j["ShadowViewportScale"] = s.m_ShadowViewportScale;
    j["ShadowmapSizeZScale"] = s.m_ShadowmapSizeZScale;
    j["ShadowmapSliceSchemeWeight"] = s.m_ShadowmapSliceSchemeWeight;
    j["ShadowmapFirstSliceScale"] = s.m_ShadowmapFirstSliceScale;
    j["ShadowmapExtrusionLength"] = s.m_ShadowmapExtrusionLength;
    j["ShadowmapMinFov"] = s.m_ShadowmapMinFov;
    j["ShadowmapPoissonFilterScale"] = s.m_ShadowmapPoissonFilterScale;
    j["ShadowmapFixedDepthEnable"] = s.m_ShadowmapFixedDepthEnable;
    j["ShadowmapFixedMovementEnable"] = s.m_ShadowmapFixedMovementEnable;
    j["ShadowmapCullVolumeEnable"] = s.m_ShadowmapCullVolumeEnable;
    j["ShadowmapAccumEnable"] = s.m_ShadowmapAccumEnable;
    j["ShadowmapAccumBilinearEnable"] = s.m_ShadowmapAccumBilinearEnable;
    j["ColoredShadowmapSlicesEnable"] = s.m_ColoredShadowmapSlicesEnable;
    j["ZBufferShadowTestEnable"] = s.m_ZBufferShadowTestEnable;
    j["DxShadowmap16BitEnable"] = s.m_DxShadowmap16BitEnable;
    j["OnlyShadowmapSlice"] = s.m_OnlyShadowmapSlice;

    j["MotionBlurEnable"] = s.m_MotionBlurEnable;
    j["MotionBlurGeometryPassEnable"] = s.m_MotionBlurGeometryPassEnable;
    j["MotionBlurStencilPassEnable"] = s.m_MotionBlurStencilPassEnable;
    j["MotionBlurScale"] = s.m_MotionBlurScale;
    j["MotionBlurMax"] = s.m_MotionBlurMax;
    j["MotionBlurNoiseScale"] = s.m_MotionBlurNoiseScale;
    j["MotionBlurQuality"] = s.m_MotionBlurQuality;
    j["MotionBlurMaxSampleCount"] = s.m_MotionBlurMaxSampleCount;
    j["MotionBlurFrameAverageCount"] = s.m_MotionBlurFrameAverageCount;
    j["MotionBlurMaxFrameTime"] = s.m_MotionBlurMaxFrameTime;
    j["ForceMotionBlurDepthCutoff"] = s.m_ForceMotionBlurDepthCutoff;
    j["ForceMotionBlurCutoffGradientScale"] = s.m_ForceMotionBlurCutoffGradientScale;

    j["OutdoorLightEnable"] = s.m_OutdoorLightEnable;
    j["OutdoorKeyLightEnable"] = s.m_OutdoorKeyLightEnable;
    j["OutdoorSkyLightEnable"] = s.m_OutdoorSkyLightEnable;
    j["OutdoorLightSpecularEnable"] = s.m_OutdoorLightSpecularEnable;
    j["OutdoorLightTilingEnable"] = s.m_OutdoorLightTilingEnable;
    j["OutdoorLightTileRenderEnable"] = s.m_OutdoorLightTileRenderEnable;
    j["OutdoorLightTileBlendEnable"] = s.m_OutdoorLightTileBlendEnable;
    j["OutdoorLightTileSimpleShaderEnable"] = s.m_OutdoorLightTileSimpleShaderEnable;
    j["OutdoorLightTileBatchCount"] = s.m_OutdoorLightTileBatchCount;
    j["LightIntensityScale"] = s.m_LightIntensityScale;
    j["LightForceIntensity"] = s.m_LightForceIntensity;
    j["LightRadiusScale"] = s.m_LightRadiusScale;
    j["LightAttenuationThreshold"] = s.m_LightAttenuationThreshold;
    j["LightAttenuationThresholdEnable"] = s.m_LightAttenuationThresholdEnable;
    j["LightWidthEnable"] = s.m_LightWidthEnable;
    j["LightIntensityNormalizationEnable"] = s.m_LightIntensityNormalizationEnable;
    j["SpecularLightingEnable"] = s.m_SpecularLightingEnable;
    j["SkinLightingEnable"] = s.m_SkinLightingEnable;
    j["TranslucencyLightingEnable"] = s.m_TranslucencyLightingEnable;
    j["EmissiveEnable"] = s.m_EmissiveEnable;
    j["UnlitEnable"] = s.m_UnlitEnable;
    j["DrawLightSources"] = s.m_DrawLightSources;

    j["LightLodSpecularFadeAreaStart"] = s.m_LightLodSpecularFadeAreaStart;
    j["LightLodSpecularFadeAreaEnd"] = s.m_LightLodSpecularFadeAreaEnd;
    j["LightLodRadiusFactor"] = s.m_LightLodRadiusFactor;
    j["LightLodFadeArea"] = s.m_LightLodFadeArea;
    j["LightLodMinArea"] = s.m_LightLodMinArea;

    j["LightCullEnable"] = s.m_LightCullEnable;
    j["LightDepthCullEnable"] = s.m_LightDepthCullEnable;
    j["LightNormalCullEnable"] = s.m_LightNormalCullEnable;
    j["LightConeCullEnable"] = s.m_LightConeCullEnable;
    j["LightStencilMinArea"] = s.m_LightStencilMinArea;
    j["LightStencilMethodEnable"] = s.m_LightStencilMethodEnable;
    j["LightVolumeMethodEnable"] = s.m_LightVolumeMethodEnable;
    j["LightVolumeDepthTestEnable"] = s.m_LightVolumeDepthTestEnable;
    j["LightTileMinArea"] = s.m_LightTileMinArea;
    j["LightTileOverlayEnable"] = s.m_LightTileOverlayEnable;
    j["LightOverdrawMaxLayerCount"] = s.m_LightOverdrawMaxLayerCount;

    j["PointLightsEnable"] = s.m_PointLightsEnable;
    j["MaxPointLightCount"] = s.m_MaxPointLightCount;
    j["SpotLightsEnable"] = s.m_SpotLightsEnable;
    j["MaxSpotLightCount"] = s.m_MaxSpotLightCount;
    j["MaxSpotLightShadowCount"] = s.m_MaxSpotLightShadowCount;
    j["SpotLightShadowmapResolution"] = s.m_SpotLightShadowmapResolution;
    j["SpotLightShadowmapQuality"] = s.m_SpotLightShadowmapQuality;
    j["SpotLightNearPlane"] = s.m_SpotLightNearPlane;
    j["SpotLightShadowmapPoissonFilterScale"] = s.m_SpotLightShadowmapPoissonFilterScale;
    j["SpotLightShadowmapEnable"] = s.m_SpotLightShadowmapEnable;
    j["DxSpotLightShadowmap16BitEnable"] = s.m_DxSpotLightShadowmap16BitEnable;
    j["SpotLightShadomapLevel"] = static_cast<int>(s.m_SpotLightShadomapLevel);
    j["LineLightsEnable"] = s.m_LineLightsEnable;
    j["ConeLightsEnable"] = s.m_ConeLightsEnable;
    j["LightProbesEnable"] = s.m_LightProbesEnable;

    j["SkyEnable"] = s.m_SkyEnable;
    j["SkyFogEnable"] = s.m_SkyFogEnable;
    j["SkyEnvmapEnable"] = s.m_SkyEnvmapEnable;
    j["SkyEnvmapUpdateEnable"] = s.m_SkyEnvmapUpdateEnable;
    j["SkyEnvmapForceUpdateEnable"] = s.m_SkyEnvmapForceUpdateEnable;
    j["SkyEnvmapMipmapGenEnable"] = s.m_SkyEnvmapMipmapGenEnable;
    j["SkyEnvmapUse8BitTexture"] = s.m_SkyEnvmapUse8BitTexture;
    j["SkyEnvmapResolution"] = s.m_SkyEnvmapResolution;
    j["SkyEnvmapSidesPerFrameCount"] = s.m_SkyEnvmapSidesPerFrameCount;
    j["SkyEnvmapFilterWidth"] = s.m_SkyEnvmapFilterWidth;
    j["SkyEnvmapFilterMode"] = static_cast<int>(s.m_SkyEnvmapFilterMode);
    j["SkyVisibilityEnvmapScalingEnable"] = s.m_SkyVisibilityEnvmapScalingEnable;
    j["DrawDebugSkyEnvmapMipLevel"] = s.m_DrawDebugSkyEnvmapMipLevel;

    j["DynamicEnvmapEnable"] = s.m_DynamicEnvmapEnable;
    j["DynamicEnvmapLightingEnable"] = s.m_DynamicEnvmapLightingEnable;
    j["DynamicEnvmapMipmapGenEnable"] = s.m_DynamicEnvmapMipmapGenEnable;
    j["DynamicEnvmapResolution"] = s.m_DynamicEnvmapResolution;
    j["DynamicEnvmapFilterWidth"] = s.m_DynamicEnvmapFilterWidth;
    j["DynamicEnvmapFilterMode"] = static_cast<int>(s.m_DynamicEnvmapFilterMode);
    j["DynamicEnvmapDefaultPosition"] = SerializeVec3(s.m_DynamicEnvmapDefaultPosition);
    j["DrawDebugDynamicEnvmapMipLevel"] = s.m_DrawDebugDynamicEnvmapMipLevel;
    j["StaticEnvmapEnable"] = s.m_StaticEnvmapEnable;
    j["CustomEnvmapEnable"] = s.m_CustomEnvmapEnable;

    j["PlanarReflectionEnable"] = s.m_PlanarReflectionEnable;
    j["PlanarReflectionBlur"] = s.m_PlanarReflectionBlur;
    j["PlanarReflectionWidth"] = s.m_PlanarReflectionWidth;
    j["PlanarReflectionHeight"] = s.m_PlanarReflectionHeight;
    j["PlanarReflectionCullFOV"] = s.m_PlanarReflectionCullFOV;
    j["TempPlanarReflectionY"] = s.m_TempPlanarReflectionY;
    j["PlanarReflectionDebugCullMode"] = s.m_PlanarReflectionDebugCullMode;
    j["FreezePlanarReflectionCullFrustum"] = s.m_FreezePlanarReflectionCullFrustum;
    j["ReflectionEnvmapSize"] = s.m_ReflectionEnvmapSize;

    j["SubSurfaceColor"] = SerializeVec3(s.m_SubSurfaceColor);
    j["SubSurfaceRolloffKeyLight"] = s.m_SubSurfaceRolloffKeyLight;
    j["SubSurfaceRolloffLocalLight"] = s.m_SubSurfaceRolloffLocalLight;

    j["DecalVolumeEnable"] = s.m_DecalVolumeEnable;
    j["MaxDecalVolumeCount"] = s.m_MaxDecalVolumeCount;
    j["DecalVolumeScale"] = s.m_DecalVolumeScale;
    j["MaxDestructionVolumeCount"] = s.m_MaxDestructionVolumeCount;

    j["ZPassEnable"] = s.m_ZPassEnable;
    j["MainOpaqueZPassEnable"] = s.m_MainOpaqueZPassEnable;
    j["OccluderMeshZPrepassEnable"] = s.m_OccluderMeshZPrepassEnable;
    j["OccluderMeshZPrepassDebugEnable"] = s.m_OccluderMeshZPrepassDebugEnable;
    j["ForegroundEnable"] = s.m_ForegroundEnable;
    j["ForegroundZPassEnable"] = s.m_ForegroundZPassEnable;
    j["ForegroundDepthClearEnable"] = s.m_ForegroundDepthClearEnable;
    j["ForegroundAsMainEnable"] = s.m_ForegroundAsMainEnable;
    j["ForegroundTransparentEnable"] = s.m_ForegroundTransparentEnable;
    j["DrawTransparent"] = s.m_DrawTransparent;
    j["DrawTransparentDecal"] = s.m_DrawTransparentDecal;
    j["ScreenEffectEnable"] = s.m_ScreenEffectEnable;
    j["FinalPostEnable"] = s.m_FinalPostEnable;
    j["ViewFxEnable"] = s.m_ViewFxEnable;

    j["LensFlaresEnable"] = s.m_LensFlaresEnable;
    j["HalfResLensFlaresEnable"] = s.m_HalfResLensFlaresEnable;
    j["LensFlareOcclusionEnable"] = s.m_LensFlareOcclusionEnable;

    j["GBufferClearEnable"] = s.m_GBufferClearEnable;
    j["GBufferAlphaTestSimpleEnable"] = s.m_GBufferAlphaTestSimpleEnable;
    j["DxGBufferLight16BitEnable"] = s.m_DxGBufferLight16BitEnable;
    j["DxGBufferNormal16BitEnable"] = s.m_DxGBufferNormal16BitEnable;
    j["DxLinearDepth32BitFormatEnable"] = s.m_DxLinearDepth32BitFormatEnable;
    j["DxDeferredCsPathEnable"] = s.m_DxDeferredCsPathEnable;
    j["GBufferTestCount"] = s.m_GBufferTestCount;
    j["GBufferLayout"] = static_cast<int>(s.m_GBufferLayout);

    j["OverrideDynamicAO"] = s.m_OverrideDynamicAO;

    j["WireframeEnable"] = s.m_WireframeEnable;
    j["OpaqueSortBySolutionEnable"] = s.m_OpaqueSortBySolutionEnable;
    j["GenericEntityRendererEnable"] = s.m_GenericEntityRendererEnable;
    j["GenericEntityMaxVisibleEntityCount"] = s.m_GenericEntityMaxVisibleEntityCount;
    j["SetupJobEnable"] = s.m_SetupJobEnable;
    j["DynamicCullZBufferTestEnable"] = s.m_DynamicCullZBufferTestEnable;
    j["DynamicCullDrawOccludedBoxesEnable"] = s.m_DynamicCullDrawOccludedBoxesEnable;
    j["ViewMode"] = static_cast<int>(s.m_ViewMode);
    j["OnlyTileIndex"] = s.m_OnlyTileIndex;
    j["OnlyLightTileX"] = s.m_OnlyLightTileX;
    j["OnlyLightTileY"] = s.m_OnlyLightTileY;
    j["OnlyLightTileIndex"] = s.m_OnlyLightTileIndex;

    bool hasDebug = s.m_DrawDebugGBuffer || s.m_DrawDebugZBufferEnable || s.m_DrawDebugShadowmaps;
    if (hasDebug)
    {
        j["DrawDebugGBuffer"] = s.m_DrawDebugGBuffer;
        j["DrawDebugZBufferEnable"] = s.m_DrawDebugZBufferEnable;
        j["DrawDebugVelocityBuffer"] = s.m_DrawDebugVelocityBuffer;
        j["DrawDebugMultisampleClassify"] = s.m_DrawDebugMultisampleClassify;
        j["DrawDebugHalfResEnvironment"] = s.m_DrawDebugHalfResEnvironment;
        j["DrawDebugShadowmaps"] = s.m_DrawDebugShadowmaps;
        j["DrawDebugQuarterShadowmaps"] = s.m_DrawDebugQuarterShadowmaps;
        j["DrawDebugTransShadowmap"] = s.m_DrawDebugTransShadowmap;
        j["DrawDebugSpotLightShadowmaps"] = s.m_DrawDebugSpotLightShadowmaps;
        j["DrawShadowFrustums"] = s.m_DrawShadowFrustums;
        j["DrawDebugLightSources"] = s.m_DrawDebugLightSources;
        j["DrawDebugLightShadowSources"] = s.m_DrawDebugLightShadowSources;
        j["DrawDebugLightStats"] = s.m_DrawDebugLightStats;
        j["DrawDebugLightTiles"] = s.m_DrawDebugLightTiles;
        j["DrawDebugLightTileVolumes"] = s.m_DrawDebugLightTileVolumes;
        j["DrawDebugLightTileFbGrid"] = s.m_DrawDebugLightTileFbGrid;
        j["DrawDebugLightTileGrid"] = s.m_DrawDebugLightTileGrid;
        j["DrawDebugLightTileSources"] = s.m_DrawDebugLightTileSources;
        j["DrawDebugDynamicAO"] = s.m_DrawDebugDynamicAO;
        j["DrawDebugDynamicEnvmap"] = s.m_DrawDebugDynamicEnvmap;
        j["DrawDebugSkyEnvmap"] = s.m_DrawDebugSkyEnvmap;
        j["DrawDebugPlanarReflection"] = s.m_DrawDebugPlanarReflection;
        j["DrawDebugPlanarReflectionCullFrustum"] = s.m_DrawDebugPlanarReflectionCullFrustum;
        j["DrawDebugDecalVolumes"] = s.m_DrawDebugDecalVolumes;
        j["DrawDebugDestructionVolumes"] = s.m_DrawDebugDestructionVolumes;
        j["DrawDebugLensFlares"] = s.m_DrawDebugLensFlares;
        j["DrawDebugLensFlareOccluders"] = s.m_DrawDebugLensFlareOccluders;
        j["DrawDebugWorldOcclusions"] = s.m_DrawDebugWorldOcclusions;
        j["DrawDebugVisibleEntityTypes"] = s.m_DrawDebugVisibleEntityTypes;
        j["DrawBoundingSpheres"] = s.m_DrawBoundingSpheres;
        j["DrawSolidBoundingBoxes"] = s.m_DrawSolidBoundingBoxes;
        j["DrawLineBoundingBoxes"] = s.m_DrawLineBoundingBoxes;
        j["DrawFrustums"] = s.m_DrawFrustums;
    }

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

void VisualEnvironmentEditor::DeserializeWorldRenderSettings(const json& j, fb::WorldRenderSettings& s)
{
    JSON_GET(j, "ViewportScale", s.m_ViewportScale);
    JSON_GET(j, "FxaaQuality", s.m_FxaaQuality);
    JSON_GET(j, "FxaaForceVendor", s.m_FxaaForceVendor);
    JSON_GET_BOOL(j, "FxaaEnable", s.m_FxaaEnable);
    JSON_GET(j, "CullScreenAreaScale", s.m_CullScreenAreaScale);
    JSON_GET(j, "MultisampleCount", s.m_MultisampleCount);
    JSON_GET(j, "MultisampleThreshold", s.m_MultisampleThreshold);
    JSON_GET_BOOL(j, "HdrEnable", s.m_HdrEnable);
    JSON_GET_BOOL(j, "HalfResEnable", s.m_HalfResEnable);
    JSON_GET_BOOL(j, "BilateralHalfResCompositeEnable", s.m_BilateralHalfResCompositeEnable);
    JSON_GET_BOOL(j, "TiledHalfResCompositeEnable", s.m_TiledHalfResCompositeEnable);
    JSON_GET_BOOL(j, "TiledHalfResStencilOccludersEnable", s.m_TiledHalfResStencilOccludersEnable);
    JSON_GET_BOOL(j, "OutputGammaCorrectionEnable", s.m_OutputGammaCorrectionEnable);
    JSON_GET_BOOL(j, "Enable", s.m_Enable);

    JSON_GET_BOOL(j, "ShadowmapsEnable", s.m_ShadowmapsEnable);
    JSON_GET_BOOL(j, "GenerateShadowmapsEnable", s.m_GenerateShadowmapsEnable);
    JSON_GET_BOOL(j, "ApplyShadowmapsEnable", s.m_ApplyShadowmapsEnable);
    JSON_GET_BOOL(j, "SimpleShadowmapsEnable", s.m_SimpleShadowmapsEnable);
    JSON_GET_BOOL(j, "TransparencyShadowmapsEnable", s.m_TransparencyShadowmapsEnable);
    JSON_GET_BOOL(j, "CloudShadowEnable", s.m_CloudShadowEnable);
    JSON_GET(j, "ShadowmapResolution", s.m_ShadowmapResolution);
    JSON_GET(j, "ShadowmapQuality", s.m_ShadowmapQuality);
    JSON_GET(j, "ShadowmapSliceCount", s.m_ShadowmapSliceCount);
    JSON_GET(j, "ShadowmapViewDistance", s.m_ShadowmapViewDistance);
    JSON_GET_BOOL(j, "ShadowmapViewDistanceScaleEnable", s.m_ShadowmapViewDistanceScaleEnable);
    JSON_GET(j, "ShadowMinScreenArea", s.m_ShadowMinScreenArea);
    JSON_GET(j, "ShadowmapMinScreenArea", s.m_ShadowmapMinScreenArea);
    JSON_GET(j, "ShadowViewportScale", s.m_ShadowViewportScale);
    JSON_GET(j, "ShadowmapSizeZScale", s.m_ShadowmapSizeZScale);
    JSON_GET(j, "ShadowmapSliceSchemeWeight", s.m_ShadowmapSliceSchemeWeight);
    JSON_GET(j, "ShadowmapFirstSliceScale", s.m_ShadowmapFirstSliceScale);
    JSON_GET(j, "ShadowmapExtrusionLength", s.m_ShadowmapExtrusionLength);
    JSON_GET(j, "ShadowmapMinFov", s.m_ShadowmapMinFov);
    JSON_GET(j, "ShadowmapPoissonFilterScale", s.m_ShadowmapPoissonFilterScale);
    JSON_GET_BOOL(j, "ShadowmapFixedDepthEnable", s.m_ShadowmapFixedDepthEnable);
    JSON_GET_BOOL(j, "ShadowmapFixedMovementEnable", s.m_ShadowmapFixedMovementEnable);
    JSON_GET_BOOL(j, "ShadowmapCullVolumeEnable", s.m_ShadowmapCullVolumeEnable);
    JSON_GET_BOOL(j, "ShadowmapAccumEnable", s.m_ShadowmapAccumEnable);
    JSON_GET_BOOL(j, "ShadowmapAccumBilinearEnable", s.m_ShadowmapAccumBilinearEnable);
    JSON_GET_BOOL(j, "ColoredShadowmapSlicesEnable", s.m_ColoredShadowmapSlicesEnable);
    JSON_GET_BOOL(j, "ZBufferShadowTestEnable", s.m_ZBufferShadowTestEnable);
    JSON_GET_BOOL(j, "DxShadowmap16BitEnable", s.m_DxShadowmap16BitEnable);
    JSON_GET(j, "OnlyShadowmapSlice", s.m_OnlyShadowmapSlice);

    JSON_GET_BOOL(j, "MotionBlurEnable", s.m_MotionBlurEnable);
    JSON_GET_BOOL(j, "MotionBlurGeometryPassEnable", s.m_MotionBlurGeometryPassEnable);
    JSON_GET_BOOL(j, "MotionBlurStencilPassEnable", s.m_MotionBlurStencilPassEnable);
    JSON_GET(j, "MotionBlurScale", s.m_MotionBlurScale);
    JSON_GET(j, "MotionBlurMax", s.m_MotionBlurMax);
    JSON_GET(j, "MotionBlurNoiseScale", s.m_MotionBlurNoiseScale);
    JSON_GET(j, "MotionBlurQuality", s.m_MotionBlurQuality);
    JSON_GET(j, "MotionBlurMaxSampleCount", s.m_MotionBlurMaxSampleCount);
    JSON_GET(j, "MotionBlurFrameAverageCount", s.m_MotionBlurFrameAverageCount);
    JSON_GET(j, "MotionBlurMaxFrameTime", s.m_MotionBlurMaxFrameTime);
    JSON_GET(j, "ForceMotionBlurDepthCutoff", s.m_ForceMotionBlurDepthCutoff);
    JSON_GET(j, "ForceMotionBlurCutoffGradientScale", s.m_ForceMotionBlurCutoffGradientScale);

    JSON_GET_BOOL(j, "OutdoorLightEnable", s.m_OutdoorLightEnable);
    JSON_GET_BOOL(j, "OutdoorKeyLightEnable", s.m_OutdoorKeyLightEnable);
    JSON_GET_BOOL(j, "OutdoorSkyLightEnable", s.m_OutdoorSkyLightEnable);
    JSON_GET_BOOL(j, "OutdoorLightSpecularEnable", s.m_OutdoorLightSpecularEnable);
    JSON_GET_BOOL(j, "OutdoorLightTilingEnable", s.m_OutdoorLightTilingEnable);
    JSON_GET_BOOL(j, "OutdoorLightTileRenderEnable", s.m_OutdoorLightTileRenderEnable);
    JSON_GET_BOOL(j, "OutdoorLightTileBlendEnable", s.m_OutdoorLightTileBlendEnable);
    JSON_GET_BOOL(j, "OutdoorLightTileSimpleShaderEnable", s.m_OutdoorLightTileSimpleShaderEnable);
    JSON_GET(j, "OutdoorLightTileBatchCount", s.m_OutdoorLightTileBatchCount);
    JSON_GET(j, "LightIntensityScale", s.m_LightIntensityScale);
    JSON_GET(j, "LightForceIntensity", s.m_LightForceIntensity);
    JSON_GET(j, "LightRadiusScale", s.m_LightRadiusScale);
    JSON_GET(j, "LightAttenuationThreshold", s.m_LightAttenuationThreshold);
    JSON_GET_BOOL(j, "LightAttenuationThresholdEnable", s.m_LightAttenuationThresholdEnable);
    JSON_GET_BOOL(j, "LightWidthEnable", s.m_LightWidthEnable);
    JSON_GET_BOOL(j, "LightIntensityNormalizationEnable", s.m_LightIntensityNormalizationEnable);
    JSON_GET_BOOL(j, "SpecularLightingEnable", s.m_SpecularLightingEnable);
    JSON_GET_BOOL(j, "SkinLightingEnable", s.m_SkinLightingEnable);
    JSON_GET_BOOL(j, "TranslucencyLightingEnable", s.m_TranslucencyLightingEnable);
    JSON_GET_BOOL(j, "EmissiveEnable", s.m_EmissiveEnable);
    JSON_GET_BOOL(j, "UnlitEnable", s.m_UnlitEnable);
    JSON_GET_BOOL(j, "DrawLightSources", s.m_DrawLightSources);

    JSON_GET(j, "LightLodSpecularFadeAreaStart", s.m_LightLodSpecularFadeAreaStart);
    JSON_GET(j, "LightLodSpecularFadeAreaEnd", s.m_LightLodSpecularFadeAreaEnd);
    JSON_GET(j, "LightLodRadiusFactor", s.m_LightLodRadiusFactor);
    JSON_GET(j, "LightLodFadeArea", s.m_LightLodFadeArea);
    JSON_GET(j, "LightLodMinArea", s.m_LightLodMinArea);

    JSON_GET_BOOL(j, "LightCullEnable", s.m_LightCullEnable);
    JSON_GET_BOOL(j, "LightDepthCullEnable", s.m_LightDepthCullEnable);
    JSON_GET_BOOL(j, "LightNormalCullEnable", s.m_LightNormalCullEnable);
    JSON_GET_BOOL(j, "LightConeCullEnable", s.m_LightConeCullEnable);
    JSON_GET(j, "LightStencilMinArea", s.m_LightStencilMinArea);
    JSON_GET_BOOL(j, "LightStencilMethodEnable", s.m_LightStencilMethodEnable);
    JSON_GET_BOOL(j, "LightVolumeMethodEnable", s.m_LightVolumeMethodEnable);
    JSON_GET_BOOL(j, "LightVolumeDepthTestEnable", s.m_LightVolumeDepthTestEnable);
    JSON_GET(j, "LightTileMinArea", s.m_LightTileMinArea);
    JSON_GET_BOOL(j, "LightTileOverlayEnable", s.m_LightTileOverlayEnable);
    JSON_GET(j, "LightOverdrawMaxLayerCount", s.m_LightOverdrawMaxLayerCount);

    JSON_GET_BOOL(j, "PointLightsEnable", s.m_PointLightsEnable);
    JSON_GET(j, "MaxPointLightCount", s.m_MaxPointLightCount);
    JSON_GET_BOOL(j, "SpotLightsEnable", s.m_SpotLightsEnable);
    JSON_GET(j, "MaxSpotLightCount", s.m_MaxSpotLightCount);
    JSON_GET(j, "MaxSpotLightShadowCount", s.m_MaxSpotLightShadowCount);
    JSON_GET(j, "SpotLightShadowmapResolution", s.m_SpotLightShadowmapResolution);
    JSON_GET(j, "SpotLightShadowmapQuality", s.m_SpotLightShadowmapQuality);
    JSON_GET(j, "SpotLightNearPlane", s.m_SpotLightNearPlane);
    JSON_GET(j, "SpotLightShadowmapPoissonFilterScale", s.m_SpotLightShadowmapPoissonFilterScale);
    JSON_GET_BOOL(j, "SpotLightShadowmapEnable", s.m_SpotLightShadowmapEnable);
    JSON_GET_BOOL(j, "DxSpotLightShadowmap16BitEnable", s.m_DxSpotLightShadowmap16BitEnable);
    JSON_GET_ENUM(j, "SpotLightShadomapLevel", s.m_SpotLightShadomapLevel, fb::QualityLevel);
    JSON_GET_BOOL(j, "LineLightsEnable", s.m_LineLightsEnable);
    JSON_GET_BOOL(j, "ConeLightsEnable", s.m_ConeLightsEnable);
    JSON_GET_BOOL(j, "LightProbesEnable", s.m_LightProbesEnable);

    JSON_GET_BOOL(j, "SkyEnable", s.m_SkyEnable);
    JSON_GET_BOOL(j, "SkyFogEnable", s.m_SkyFogEnable);
    JSON_GET_BOOL(j, "SkyEnvmapEnable", s.m_SkyEnvmapEnable);
    JSON_GET_BOOL(j, "SkyEnvmapUpdateEnable", s.m_SkyEnvmapUpdateEnable);
    JSON_GET_BOOL(j, "SkyEnvmapForceUpdateEnable", s.m_SkyEnvmapForceUpdateEnable);
    JSON_GET_BOOL(j, "SkyEnvmapMipmapGenEnable", s.m_SkyEnvmapMipmapGenEnable);
    JSON_GET_BOOL(j, "SkyEnvmapUse8BitTexture", s.m_SkyEnvmapUse8BitTexture);
    JSON_GET(j, "SkyEnvmapResolution", s.m_SkyEnvmapResolution);
    JSON_GET(j, "SkyEnvmapSidesPerFrameCount", s.m_SkyEnvmapSidesPerFrameCount);
    JSON_GET(j, "SkyEnvmapFilterWidth", s.m_SkyEnvmapFilterWidth);
    JSON_GET_ENUM(j, "SkyEnvmapFilterMode", s.m_SkyEnvmapFilterMode, fb::MipmapFilterMode);
    JSON_GET_BOOL(j, "SkyVisibilityEnvmapScalingEnable", s.m_SkyVisibilityEnvmapScalingEnable);
    JSON_GET(j, "DrawDebugSkyEnvmapMipLevel", s.m_DrawDebugSkyEnvmapMipLevel);

    JSON_GET_BOOL(j, "DynamicEnvmapEnable", s.m_DynamicEnvmapEnable);
    JSON_GET_BOOL(j, "DynamicEnvmapLightingEnable", s.m_DynamicEnvmapLightingEnable);
    JSON_GET_BOOL(j, "DynamicEnvmapMipmapGenEnable", s.m_DynamicEnvmapMipmapGenEnable);
    JSON_GET(j, "DynamicEnvmapResolution", s.m_DynamicEnvmapResolution);
    JSON_GET(j, "DynamicEnvmapFilterWidth", s.m_DynamicEnvmapFilterWidth);
    JSON_GET_ENUM(j, "DynamicEnvmapFilterMode", s.m_DynamicEnvmapFilterMode, fb::MipmapFilterMode);
    JSON_GET_VEC3(j, "DynamicEnvmapDefaultPosition", s.m_DynamicEnvmapDefaultPosition);
    JSON_GET(j, "DrawDebugDynamicEnvmapMipLevel", s.m_DrawDebugDynamicEnvmapMipLevel);
    JSON_GET_BOOL(j, "StaticEnvmapEnable", s.m_StaticEnvmapEnable);
    JSON_GET_BOOL(j, "CustomEnvmapEnable", s.m_CustomEnvmapEnable);

    JSON_GET_BOOL(j, "PlanarReflectionEnable", s.m_PlanarReflectionEnable);
    JSON_GET_BOOL(j, "PlanarReflectionBlur", s.m_PlanarReflectionBlur);
    JSON_GET(j, "PlanarReflectionWidth", s.m_PlanarReflectionWidth);
    JSON_GET(j, "PlanarReflectionHeight", s.m_PlanarReflectionHeight);
    JSON_GET(j, "PlanarReflectionCullFOV", s.m_PlanarReflectionCullFOV);
    JSON_GET(j, "TempPlanarReflectionY", s.m_TempPlanarReflectionY);
    JSON_GET_BOOL(j, "PlanarReflectionDebugCullMode", s.m_PlanarReflectionDebugCullMode);
    JSON_GET_BOOL(j, "FreezePlanarReflectionCullFrustum", s.m_FreezePlanarReflectionCullFrustum);
    JSON_GET(j, "ReflectionEnvmapSize", s.m_ReflectionEnvmapSize);

    JSON_GET_VEC3(j, "SubSurfaceColor", s.m_SubSurfaceColor);
    JSON_GET(j, "SubSurfaceRolloffKeyLight", s.m_SubSurfaceRolloffKeyLight);
    JSON_GET(j, "SubSurfaceRolloffLocalLight", s.m_SubSurfaceRolloffLocalLight);

    JSON_GET_BOOL(j, "DecalVolumeEnable", s.m_DecalVolumeEnable);
    JSON_GET(j, "MaxDecalVolumeCount", s.m_MaxDecalVolumeCount);
    JSON_GET(j, "DecalVolumeScale", s.m_DecalVolumeScale);
    JSON_GET(j, "MaxDestructionVolumeCount", s.m_MaxDestructionVolumeCount);

    JSON_GET_BOOL(j, "ZPassEnable", s.m_ZPassEnable);
    JSON_GET_BOOL(j, "MainOpaqueZPassEnable", s.m_MainOpaqueZPassEnable);
    JSON_GET_BOOL(j, "OccluderMeshZPrepassEnable", s.m_OccluderMeshZPrepassEnable);
    JSON_GET_BOOL(j, "OccluderMeshZPrepassDebugEnable", s.m_OccluderMeshZPrepassDebugEnable);
    JSON_GET_BOOL(j, "ForegroundEnable", s.m_ForegroundEnable);
    JSON_GET_BOOL(j, "ForegroundZPassEnable", s.m_ForegroundZPassEnable);
    JSON_GET_BOOL(j, "ForegroundDepthClearEnable", s.m_ForegroundDepthClearEnable);
    JSON_GET_BOOL(j, "ForegroundAsMainEnable", s.m_ForegroundAsMainEnable);
    JSON_GET_BOOL(j, "ForegroundTransparentEnable", s.m_ForegroundTransparentEnable);
    JSON_GET_BOOL(j, "DrawTransparent", s.m_DrawTransparent);
    JSON_GET_BOOL(j, "DrawTransparentDecal", s.m_DrawTransparentDecal);
    JSON_GET_BOOL(j, "ScreenEffectEnable", s.m_ScreenEffectEnable);
    JSON_GET_BOOL(j, "FinalPostEnable", s.m_FinalPostEnable);
    JSON_GET_BOOL(j, "ViewFxEnable", s.m_ViewFxEnable);

    JSON_GET_BOOL(j, "LensFlaresEnable", s.m_LensFlaresEnable);
    JSON_GET_BOOL(j, "HalfResLensFlaresEnable", s.m_HalfResLensFlaresEnable);
    JSON_GET_BOOL(j, "LensFlareOcclusionEnable", s.m_LensFlareOcclusionEnable);

    JSON_GET_BOOL(j, "GBufferClearEnable", s.m_GBufferClearEnable);
    JSON_GET_BOOL(j, "GBufferAlphaTestSimpleEnable", s.m_GBufferAlphaTestSimpleEnable);
    JSON_GET_BOOL(j, "DxGBufferLight16BitEnable", s.m_DxGBufferLight16BitEnable);
    JSON_GET_BOOL(j, "DxGBufferNormal16BitEnable", s.m_DxGBufferNormal16BitEnable);
    JSON_GET_BOOL(j, "DxLinearDepth32BitFormatEnable", s.m_DxLinearDepth32BitFormatEnable);
    JSON_GET_BOOL(j, "DxDeferredCsPathEnable", s.m_DxDeferredCsPathEnable);
    JSON_GET(j, "GBufferTestCount", s.m_GBufferTestCount);
    JSON_GET_ENUM(j, "GBufferLayout", s.m_GBufferLayout, fb::ShaderGBufferLayout);

    JSON_GET_BOOL(j, "OverrideDynamicAO", s.m_OverrideDynamicAO);

    JSON_GET_BOOL(j, "WireframeEnable", s.m_WireframeEnable);
    JSON_GET_BOOL(j, "OpaqueSortBySolutionEnable", s.m_OpaqueSortBySolutionEnable);
    JSON_GET_BOOL(j, "GenericEntityRendererEnable", s.m_GenericEntityRendererEnable);
    JSON_GET(j, "GenericEntityMaxVisibleEntityCount", s.m_GenericEntityMaxVisibleEntityCount);
    JSON_GET_BOOL(j, "SetupJobEnable", s.m_SetupJobEnable);
    JSON_GET_BOOL(j, "DynamicCullZBufferTestEnable", s.m_DynamicCullZBufferTestEnable);
    JSON_GET_BOOL(j, "DynamicCullDrawOccludedBoxesEnable", s.m_DynamicCullDrawOccludedBoxesEnable);
    JSON_GET_ENUM(j, "ViewMode", s.m_ViewMode, fb::WorldViewMode);
    JSON_GET(j, "OnlyTileIndex", s.m_OnlyTileIndex);
    JSON_GET(j, "OnlyLightTileX", s.m_OnlyLightTileX);
    JSON_GET(j, "OnlyLightTileY", s.m_OnlyLightTileY);
    JSON_GET(j, "OnlyLightTileIndex", s.m_OnlyLightTileIndex);

    JSON_GET_BOOL(j, "DrawDebugGBuffer", s.m_DrawDebugGBuffer);
    JSON_GET_BOOL(j, "DrawDebugZBufferEnable", s.m_DrawDebugZBufferEnable);
    JSON_GET_BOOL(j, "DrawDebugVelocityBuffer", s.m_DrawDebugVelocityBuffer);
    JSON_GET_BOOL(j, "DrawDebugMultisampleClassify", s.m_DrawDebugMultisampleClassify);
    JSON_GET_BOOL(j, "DrawDebugHalfResEnvironment", s.m_DrawDebugHalfResEnvironment);
    JSON_GET_BOOL(j, "DrawDebugShadowmaps", s.m_DrawDebugShadowmaps);
    JSON_GET_BOOL(j, "DrawDebugQuarterShadowmaps", s.m_DrawDebugQuarterShadowmaps);
    JSON_GET_BOOL(j, "DrawDebugTransShadowmap", s.m_DrawDebugTransShadowmap);
    JSON_GET_BOOL(j, "DrawDebugSpotLightShadowmaps", s.m_DrawDebugSpotLightShadowmaps);
    JSON_GET_BOOL(j, "DrawShadowFrustums", s.m_DrawShadowFrustums);
    JSON_GET_BOOL(j, "DrawDebugLightSources", s.m_DrawDebugLightSources);
    JSON_GET_BOOL(j, "DrawDebugLightShadowSources", s.m_DrawDebugLightShadowSources);
    JSON_GET_BOOL(j, "DrawDebugLightStats", s.m_DrawDebugLightStats);
    JSON_GET_BOOL(j, "DrawDebugLightTiles", s.m_DrawDebugLightTiles);
    JSON_GET_BOOL(j, "DrawDebugLightTileVolumes", s.m_DrawDebugLightTileVolumes);
    JSON_GET_BOOL(j, "DrawDebugLightTileFbGrid", s.m_DrawDebugLightTileFbGrid);
    JSON_GET_BOOL(j, "DrawDebugLightTileGrid", s.m_DrawDebugLightTileGrid);
    JSON_GET_BOOL(j, "DrawDebugLightTileSources", s.m_DrawDebugLightTileSources);
    JSON_GET_BOOL(j, "DrawDebugDynamicAO", s.m_DrawDebugDynamicAO);
    JSON_GET_BOOL(j, "DrawDebugDynamicEnvmap", s.m_DrawDebugDynamicEnvmap);
    JSON_GET_BOOL(j, "DrawDebugSkyEnvmap", s.m_DrawDebugSkyEnvmap);
    JSON_GET_BOOL(j, "DrawDebugPlanarReflection", s.m_DrawDebugPlanarReflection);
    JSON_GET_BOOL(j, "DrawDebugPlanarReflectionCullFrustum", s.m_DrawDebugPlanarReflectionCullFrustum);
    JSON_GET_BOOL(j, "DrawDebugDecalVolumes", s.m_DrawDebugDecalVolumes);
    JSON_GET_BOOL(j, "DrawDebugDestructionVolumes", s.m_DrawDebugDestructionVolumes);
    JSON_GET_BOOL(j, "DrawDebugLensFlares", s.m_DrawDebugLensFlares);
    JSON_GET_BOOL(j, "DrawDebugLensFlareOccluders", s.m_DrawDebugLensFlareOccluders);
    JSON_GET_BOOL(j, "DrawDebugWorldOcclusions", s.m_DrawDebugWorldOcclusions);
    JSON_GET_BOOL(j, "DrawDebugVisibleEntityTypes", s.m_DrawDebugVisibleEntityTypes);
    JSON_GET_BOOL(j, "DrawBoundingSpheres", s.m_DrawBoundingSpheres);
    JSON_GET_BOOL(j, "DrawSolidBoundingBoxes", s.m_DrawSolidBoundingBoxes);
    JSON_GET_BOOL(j, "DrawLineBoundingBoxes", s.m_DrawLineBoundingBoxes);
    JSON_GET_BOOL(j, "DrawFrustums", s.m_DrawFrustums);
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
            json wrsJson = SerializeWorldRenderSettings(m_EditWorldRenderSettings);
            wrsJson["enabled"] = m_WorldRenderOverrideEnabled;
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
            const json& wrsJson = root["worldRender"];
            m_WorldRenderOverrideEnabled = wrsJson.value("enabled", false);

            if (m_WorldRenderOverrideEnabled && m_HasCapturedWorldRenderSettings)
            {
                DeserializeWorldRenderSettings(wrsJson, m_EditWorldRenderSettings);
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