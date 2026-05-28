#include "comps_serialize.h"

#include "../serialize/serialize.h"

#if defined(BFVE_GAME_BF4)

namespace editor::comps
{
    json serializeOutdoorLight(const fb::CapturedOutdoorLightComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["sunColor"] = serialize::vec3ToJson(c.m_SunColor);
        j["skyColor"] = serialize::vec3ToJson(c.m_SkyColor);
        j["groundColor"] = serialize::vec3ToJson(c.m_GroundColor);
        j["sunRotationX"] = c.m_SunRotationX;
        j["sunRotationY"] = c.m_SunRotationY;
        j["sunSpecularScale"] = c.m_SunSpecularScale;
        j["skyLightAngleFactor"] = c.m_SkyLightAngleFactor;
        j["skyEnvmapShadowScale"] = c.m_SkyEnvmapShadowScale;
        j["sunShadowHeightScale"] = c.m_SunShadowHeightScale;
        j["shadowSunRotationEnable"] = c.m_ShadowSunRotationEnable;
        j["shadowSunRotationX"] = c.m_ShadowSunRotationX;
        j["shadowSunRotationY"] = c.m_ShadowSunRotationY;
        j["cloudShadowEnable"] = c.m_CloudShadowEnable;
        j["cloudShadowIsTopDown"] = c.m_CloudShadowIsTopDown;
        j["cloudShadowSpeed"] = serialize::vec2ToJson(c.m_CloudShadowSpeed);
        j["cloudShadowCoverage"] = c.m_CloudShadowCoverage;
        j["cloudShadowSize"] = c.m_CloudShadowSize;
        j["cloudShadowExponent"] = c.m_CloudShadowExponent;
        j["cloudShadowStartFade"] = c.m_CloudShadowStartFade;
        j["cloudShadowsFadeDistance"] = c.m_CloudShadowsFadeDistance;
        j["cloudXZTranslation"] = serialize::vec2ToJson(c.m_CloudXZTranslation);
        j["translucencyDistortion"] = c.m_TranslucencyDistortion;
        j["translucencyAmbient"] = c.m_TranslucencyAmbient;
        j["translucencyScale"] = c.m_TranslucencyScale;
        j["translucencyPower"] = c.m_TranslucencyPower;
        return j;
    }

    void deserializeOutdoorLight(const json& j, fb::CapturedOutdoorLightComponentData& c)
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
        JSON_GET_BOOL(j, "shadowSunRotationEnable", c.m_ShadowSunRotationEnable);
        JSON_GET(j, "shadowSunRotationX", c.m_ShadowSunRotationX);
        JSON_GET(j, "shadowSunRotationY", c.m_ShadowSunRotationY);
        JSON_GET_BOOL(j, "cloudShadowEnable", c.m_CloudShadowEnable);
        JSON_GET_BOOL(j, "cloudShadowIsTopDown", c.m_CloudShadowIsTopDown);
        JSON_GET_VEC2(j, "cloudShadowSpeed", c.m_CloudShadowSpeed);
        JSON_GET(j, "cloudShadowCoverage", c.m_CloudShadowCoverage);
        JSON_GET(j, "cloudShadowSize", c.m_CloudShadowSize);
        JSON_GET(j, "cloudShadowExponent", c.m_CloudShadowExponent);
        JSON_GET(j, "cloudShadowStartFade", c.m_CloudShadowStartFade);
        JSON_GET(j, "cloudShadowsFadeDistance", c.m_CloudShadowsFadeDistance);
        JSON_GET_VEC2(j, "cloudXZTranslation", c.m_CloudXZTranslation);
        JSON_GET(j, "translucencyDistortion", c.m_TranslucencyDistortion);
        JSON_GET(j, "translucencyAmbient", c.m_TranslucencyAmbient);
        JSON_GET(j, "translucencyScale", c.m_TranslucencyScale);
        JSON_GET(j, "translucencyPower", c.m_TranslucencyPower);
    }

    json serializeEnlighten(const fb::CapturedEnlightenComponentData& c)
    {
        json j;
        j["skyBoxEnable"] = c.m_SkyBoxEnable;
        j["skyBoxSkyColor"] = serialize::vec3ToJson(c.m_SkyBoxSkyColor);
        j["skyBoxGroundColor"] = serialize::vec3ToJson(c.m_SkyBoxGroundColor);
        j["skyBoxSunLightColor"] = serialize::vec3ToJson(c.m_SkyBoxSunLightColor);
        j["skyBoxBackLightColor"] = serialize::vec3ToJson(c.m_SkyBoxBackLightColor);
        j["terrainColor"] = serialize::vec3ToJson(c.m_TerrainColor);
        j["opaqueAlphaTestSimpleScale"] = serialize::vec3ToJson(c.m_OpaqueAlphaTestSimpleScale);
        j["overrideLightProbe"] = serialize::vec3ToJson(c.m_OverrideLightProbe);
        j["bounceScale"] = c.m_BounceScale;
        j["sunScale"] = c.m_SunScale;
        j["cullDistance"] = c.m_CullDistance;
        j["cullRadius"] = c.m_CullRadius;
        j["skyBoxSunLightColorSize"] = c.m_SkyBoxSunLightColorSize;
        j["skyBoxBackLightColorSize"] = c.m_SkyBoxBackLightColorSize;
        j["skyBoxBackLightRotationX"] = c.m_SkyBoxBackLightRotationX;
        j["skyBoxBackLightRotationY"] = c.m_SkyBoxBackLightRotationY;
        return j;
    }

    void deserializeEnlighten(const json& j, fb::CapturedEnlightenComponentData& c)
    {
        JSON_GET_BOOL(j, "skyBoxEnable", c.m_SkyBoxEnable);
        JSON_GET_VEC3(j, "skyBoxSkyColor", c.m_SkyBoxSkyColor);
        JSON_GET_VEC3(j, "skyBoxGroundColor", c.m_SkyBoxGroundColor);
        JSON_GET_VEC3(j, "skyBoxSunLightColor", c.m_SkyBoxSunLightColor);
        JSON_GET_VEC3(j, "skyBoxBackLightColor", c.m_SkyBoxBackLightColor);
        JSON_GET_VEC3(j, "terrainColor", c.m_TerrainColor);
        JSON_GET_VEC3(j, "opaqueAlphaTestSimpleScale", c.m_OpaqueAlphaTestSimpleScale);
        JSON_GET_VEC3(j, "overrideLightProbe", c.m_OverrideLightProbe);
        JSON_GET(j, "bounceScale", c.m_BounceScale);
        JSON_GET(j, "sunScale", c.m_SunScale);
        JSON_GET(j, "cullDistance", c.m_CullDistance);
        JSON_GET(j, "cullRadius", c.m_CullRadius);
        JSON_GET(j, "skyBoxSunLightColorSize", c.m_SkyBoxSunLightColorSize);
        JSON_GET(j, "skyBoxBackLightColorSize", c.m_SkyBoxBackLightColorSize);
        JSON_GET(j, "skyBoxBackLightRotationX", c.m_SkyBoxBackLightRotationX);
        JSON_GET(j, "skyBoxBackLightRotationY", c.m_SkyBoxBackLightRotationY);
    }

    json serializeTonemap(const fb::CapturedTonemapComponentData& c)
    {
        json j;
        j["tonemapMethod"] = static_cast<int>(c.m_TonemapMethod);
        j["bloomScale"] = serialize::vec3ToJson(c.m_BloomScale);
        j["minExposure"] = c.m_MinExposure;
        j["maxExposure"] = c.m_MaxExposure;
        j["middleGray"] = c.m_MiddleGray;
        j["exposureAdjustTime"] = c.m_ExposureAdjustTime;
        j["chromostereopsisEnable"] = c.m_ChromostereopsisEnable;
        j["chromostereopsisOffset"] = c.m_ChromostereopsisOffset;
        j["chromostereopsisScale"] = c.m_ChromostereopsisScale;
        return j;
    }

    void deserializeTonemap(const json& j, fb::CapturedTonemapComponentData& c)
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

    json serializeColorCorrection(const fb::CapturedColorCorrectionComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["colorGradingEnable"] = c.m_ColorGradingEnable;
        j["brightness"] = serialize::vec3ToJson(c.m_Brightness);
        j["contrast"] = serialize::vec3ToJson(c.m_Contrast);
        j["saturation"] = serialize::vec3ToJson(c.m_Saturation);
        j["hue"] = c.m_Hue;
        return j;
    }

    void deserializeColorCorrection(const json& j, fb::CapturedColorCorrectionComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "colorGradingEnable", c.m_ColorGradingEnable);
        JSON_GET_VEC3(j, "brightness", c.m_Brightness);
        JSON_GET_VEC3(j, "contrast", c.m_Contrast);
        JSON_GET_VEC3(j, "saturation", c.m_Saturation);
        JSON_GET(j, "hue", c.m_Hue);
    }

    json serializeSky(const fb::CapturedSkyComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["indirectCubeMapOverride"] = c.m_IndirectCubeMapOverride;
        j["brightnessScale"] = c.m_BrightnessScale;
        j["sunSize"] = c.m_SunSize;
        j["sunScale"] = c.m_SunScale;
        j["skyVisibilityExponent"] = c.m_SkyVisibilityExponent;
        j["skyEnvmap8BitTexScale"] = c.m_SkyEnvmap8BitTexScale;
        j["cloudLayerSunColor"] = serialize::vec3ToJson(c.m_CloudLayerSunColor);
        j["cloudLayer1Color"] = serialize::vec3ToJson(c.m_CloudLayer1Color);
        j["cloudLayer1Altitude"] = c.m_CloudLayer1Altitude;
        j["cloudLayer1TileFactor"] = c.m_CloudLayer1TileFactor;
        j["cloudLayer1Rotation"] = c.m_CloudLayer1Rotation;
        j["cloudLayer1Speed"] = c.m_CloudLayer1Speed;
        j["cloudLayer1SunLightIntensity"] = c.m_CloudLayer1SunLightIntensity;
        j["cloudLayer1SunLightPower"] = c.m_CloudLayer1SunLightPower;
        j["cloudLayer1AmbientLightIntensity"] = c.m_CloudLayer1AmbientLightIntensity;
        j["cloudLayer1AlphaMul"] = c.m_CloudLayer1AlphaMul;
        j["cloudLayer2Color"] = serialize::vec3ToJson(c.m_CloudLayer2Color);
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
        j["EnvmapScale"] = c.m_StaticEnvmapScale;
        return j;
    }

    void deserializeSky(const json& j, fb::CapturedSkyComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "indirectCubeMapOverride", c.m_IndirectCubeMapOverride);
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
        JSON_GET(j, "EnvmapScale", c.m_StaticEnvmapScale);
    }

    json serializeFog(const fb::CapturedFogComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["start"] = c.m_Start;
        j["end"] = c.m_End;
        j["fogDistanceMultiplier"] = c.m_FogDistanceMultiplier;
        j["curve"] = serialize::vec4ToJson(c.m_Curve);
        j["fogColorEnable"] = c.m_FogColorEnable;
        j["fogColor"] = serialize::vec3ToJson(c.m_FogColor);
        j["fogColorStart"] = c.m_FogColorStart;
        j["fogColorEnd"] = c.m_FogColorEnd;
        j["fogColorCurve"] = serialize::vec4ToJson(c.m_FogColorCurve);
        j["fogGradientEnable"] = c.m_FogGradientEnable;
        j["heightFogEnable"] = c.m_HeightFogEnable;
        j["heightFogAltitude"] = c.m_HeightFogAltitude;
        j["heightFogDepth"] = c.m_HeightFogDepth;
        j["heightFogVisibilityRange"] = c.m_HeightFogVisibilityRange;
        j["heightFogFollowCamera"] = c.m_HeightFogFollowCamera;
        j["transparencyFadeStart"] = c.m_TransparencyFadeStart;
        j["transparencyFadeEnd"] = c.m_TransparencyFadeEnd;
        j["transparencyFadeClamp"] = c.m_TransparencyFadeClamp;
        j["transparencyFadeCurve"] = serialize::vec4ToJson(c.m_TransparencyFadeCurve);
        j["forwardLightScatteringEnabled"] = c.m_ForwardLightScatteringEnabled;
        j["forwardLightScatteringColor"] = serialize::vec3ToJson(c.m_ForwardLightScatteringColor);
        j["forwardLightScatteringPhaseG"] = c.m_ForwardLightScatteringPhaseG;
        j["forwardLightScatteringStrength"] = c.m_ForwardLightScatteringStrength;
        j["forwardLightScatteringPresence"] = c.m_ForwardLightScatteringPresence;
        j["forwardLightScatteringMaxBlurLength"] = c.m_ForwardLightScatteringMaxBlurLength;
        j["forwardLightScatteringExtinction"] = c.m_ForwardLightScatteringExtinction;
        j["forwardLightScatteringSmoothness"] = c.m_ForwardLightScatteringSmoothness;
        return j;
    }

    void deserializeFog(const json& j, fb::CapturedFogComponentData& c)
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
        JSON_GET_VEC4(j, "transparencyFadeCurve", c.m_TransparencyFadeCurve);
        JSON_GET_BOOL(j, "forwardLightScatteringEnabled", c.m_ForwardLightScatteringEnabled);
        JSON_GET_VEC3(j, "forwardLightScatteringColor", c.m_ForwardLightScatteringColor);
        JSON_GET(j, "forwardLightScatteringPhaseG", c.m_ForwardLightScatteringPhaseG);
        JSON_GET(j, "forwardLightScatteringStrength", c.m_ForwardLightScatteringStrength);
        JSON_GET(j, "forwardLightScatteringPresence", c.m_ForwardLightScatteringPresence);
        JSON_GET(j, "forwardLightScatteringMaxBlurLength", c.m_ForwardLightScatteringMaxBlurLength);
        JSON_GET(j, "forwardLightScatteringExtinction", c.m_ForwardLightScatteringExtinction);
        JSON_GET(j, "forwardLightScatteringSmoothness", c.m_ForwardLightScatteringSmoothness);
    }

    json serializeWind(const fb::CapturedWindComponentData& c)
    {
        json j;
        j["windDirection"] = c.m_WindDirection;
        j["windStrength"] = c.m_WindStrength;
        j["windVariationMultiplier"] = c.m_WindVariationMultiplier;
        j["windVariationRateMultiplier"] = c.m_WindVariationRateMultiplier;
        j["windMicroVariationMultiplier"] = c.m_WindMicroVariationMultiplier;
        j["turbulenceMultiplier"] = c.m_TurbulenceMultiplier;
        j["turbulenceScale"] = c.m_TurbulenceScale;
        return j;
    }

    void deserializeWind(const json& j, fb::CapturedWindComponentData& c)
    {
        JSON_GET(j, "windDirection", c.m_WindDirection);
        JSON_GET(j, "windStrength", c.m_WindStrength);
        JSON_GET(j, "windVariationMultiplier", c.m_WindVariationMultiplier);
        JSON_GET(j, "windVariationRateMultiplier", c.m_WindVariationRateMultiplier);
        JSON_GET(j, "windMicroVariationMultiplier", c.m_WindMicroVariationMultiplier);
        JSON_GET(j, "turbulenceMultiplier", c.m_TurbulenceMultiplier);
        JSON_GET(j, "turbulenceScale", c.m_TurbulenceScale);
    }

    json serializeSunFlare(const fb::CapturedSunFlareComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["debugDrawOccluder"] = c.m_DebugDrawOccluder;
        j["occluderSize"] = c.m_OccluderSize;

#define SERIALIZE_ELEMENT(n) \
        j["element" #n "Enable"] = c.m_Element##n##Enable; \
        j["element" #n "RotationAlignedToRay"] = c.m_Element##n##RotationAlignedToRay; \
        j["element" #n "Size"] = serialize::vec2ToJson(c.m_Element##n##Size); \
        j["element" #n "RayDistance"] = c.m_Element##n##RayDistance; \
        j["element" #n "RotationLocal"] = c.m_Element##n##RotationLocal; \
        j["element" #n "RotationDistMultiplier"] = c.m_Element##n##RotationDistMultiplier; \
        j["element" #n "AlphaScreenPosCurve"] = serialize::vec4ToJson(c.m_Element##n##AlphaScreenPosCurve); \
        j["element" #n "AlphaOccluderCurve"] = serialize::vec4ToJson(c.m_Element##n##AlphaOccluderCurve); \
        j["element" #n "SizeScreenPosCurve"] = serialize::vec4ToJson(c.m_Element##n##SizeScreenPosCurve); \
        j["element" #n "SizeOccluderCurve"] = serialize::vec4ToJson(c.m_Element##n##SizeOccluderCurve); \
        j["element" #n "RotationDistCurve"] = serialize::vec4ToJson(c.m_Element##n##RotationDistCurve);

        SERIALIZE_ELEMENT(1);
        SERIALIZE_ELEMENT(2);
        SERIALIZE_ELEMENT(3);
        SERIALIZE_ELEMENT(4);
        SERIALIZE_ELEMENT(5);

#undef SERIALIZE_ELEMENT
        return j;
    }

    void deserializeSunFlare(const json& j, fb::CapturedSunFlareComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "debugDrawOccluder", c.m_DebugDrawOccluder);
        JSON_GET(j, "occluderSize", c.m_OccluderSize);

#define DESERIALIZE_ELEMENT(n) \
        JSON_GET_BOOL(j, "element" #n "Enable", c.m_Element##n##Enable); \
        JSON_GET_BOOL(j, "element" #n "RotationAlignedToRay", c.m_Element##n##RotationAlignedToRay); \
        JSON_GET_VEC2(j, "element" #n "Size", c.m_Element##n##Size); \
        JSON_GET(j, "element" #n "RayDistance", c.m_Element##n##RayDistance); \
        JSON_GET(j, "element" #n "RotationLocal", c.m_Element##n##RotationLocal); \
        JSON_GET(j, "element" #n "RotationDistMultiplier", c.m_Element##n##RotationDistMultiplier); \
        JSON_GET_VEC4(j, "element" #n "AlphaScreenPosCurve", c.m_Element##n##AlphaScreenPosCurve); \
        JSON_GET_VEC4(j, "element" #n "AlphaOccluderCurve", c.m_Element##n##AlphaOccluderCurve); \
        JSON_GET_VEC4(j, "element" #n "SizeScreenPosCurve", c.m_Element##n##SizeScreenPosCurve); \
        JSON_GET_VEC4(j, "element" #n "SizeOccluderCurve", c.m_Element##n##SizeOccluderCurve); \
        JSON_GET_VEC4(j, "element" #n "RotationDistCurve", c.m_Element##n##RotationDistCurve);

        DESERIALIZE_ELEMENT(1) DESERIALIZE_ELEMENT(2) DESERIALIZE_ELEMENT(3)
        DESERIALIZE_ELEMENT(4) DESERIALIZE_ELEMENT(5)

#undef DESERIALIZE_ELEMENT
    }

    json serializeDynamicAO(const fb::CapturedDynamicAOComponentData& c)
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

    void deserializeDynamicAO(const json& j, fb::CapturedDynamicAOComponentData& c)
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

    json serializeDof(const fb::CapturedDofComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["debugDrawFocusPlane"] = c.m_DebugDrawFocusPlane;
        j["focusDistance"] = c.m_FocusDistance;
        j["blurFactor"] = c.m_BlurFactor;
        j["blurAdd"] = c.m_BlurAdd;

        j["simpleDofBlurFilter"] = static_cast<int>(c.m_SimpleDofBlurFilter);
        j["simpleDofMaxBlur"] = c.m_SimpleDofMaxBlur;
        j["simpleDofNearStart"] = c.m_SimpleDofNearStart;
        j["simpleDofNearEnd"] = c.m_SimpleDofNearEnd;
        j["simpleDofFarStart"] = c.m_SimpleDofFarStart;
        j["simpleDofFarEnd"] = c.m_SimpleDofFarEnd;

        j["ironsightsDofActive"] = c.m_IronsightsDofActive;
        j["ironsightsDofCircleBlur"] = c.m_IronsightsDofCircleBlur;
        j["hipToIronsightsFade"] = c.m_HipToIronsightsFade;
        j["ironsightsDofStartFade"] = c.m_IronsightsDofStartFade;
        j["ironsightsFocalDistance"] = c.m_IronsightsFocalDistance;
        j["ironsightsDofCircleDistance"] = c.m_IronsightsDofCircleDistance;

        j["spriteDofNearStart"] = c.m_SpriteDofNearStart;
        j["spriteDofNearEnd"] = c.m_SpriteDofNearEnd;
        j["spriteDofFarStart"] = c.m_SpriteDofFarStart;
        j["spriteDofFarEnd"] = c.m_SpriteDofFarEnd;
        j["spriteDofMaxBlur"] = c.m_SpriteDofMaxBlur;
        return j;
    }

    void deserializeDof(const json& j, fb::CapturedDofComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "debugDrawFocusPlane", c.m_DebugDrawFocusPlane);
        JSON_GET(j, "focusDistance", c.m_FocusDistance);
        JSON_GET(j, "blurFactor", c.m_BlurFactor);
        JSON_GET(j, "blurAdd", c.m_BlurAdd);

        JSON_GET_ENUM(j, "simpleDofBlurFilter", c.m_SimpleDofBlurFilter, fb::BlurFilter);
        JSON_GET(j, "simpleDofMaxBlur", c.m_SimpleDofMaxBlur);
        JSON_GET(j, "simpleDofNearStart", c.m_SimpleDofNearStart);
        JSON_GET(j, "simpleDofNearEnd", c.m_SimpleDofNearEnd);
        JSON_GET(j, "simpleDofFarStart", c.m_SimpleDofFarStart);
        JSON_GET(j, "simpleDofFarEnd", c.m_SimpleDofFarEnd);

        JSON_GET_BOOL(j, "ironsightsDofActive", c.m_IronsightsDofActive);
        JSON_GET_BOOL(j, "ironsightsDofCircleBlur", c.m_IronsightsDofCircleBlur);
        JSON_GET(j, "hipToIronsightsFade", c.m_HipToIronsightsFade);
        JSON_GET(j, "ironsightsDofStartFade", c.m_IronsightsDofStartFade);
        JSON_GET(j, "ironsightsFocalDistance", c.m_IronsightsFocalDistance);
        JSON_GET(j, "ironsightsDofCircleDistance", c.m_IronsightsDofCircleDistance);

        JSON_GET(j, "spriteDofNearStart", c.m_SpriteDofNearStart);
        JSON_GET(j, "spriteDofNearEnd", c.m_SpriteDofNearEnd);
        JSON_GET(j, "spriteDofFarStart", c.m_SpriteDofFarStart);
        JSON_GET(j, "spriteDofFarEnd", c.m_SpriteDofFarEnd);
        JSON_GET(j, "spriteDofMaxBlur", c.m_SpriteDofMaxBlur);
    }

    json serializeVignette(const fb::CapturedVignetteComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["color"] = serialize::vec3ToJson(c.m_Color);
        j["scale"] = serialize::vec2ToJson(c.m_Scale);
        j["opacity"] = c.m_Opacity;
        j["exponent"] = c.m_Exponent;
        return j;
    }

    void deserializeVignette(const json& j, fb::CapturedVignetteComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_VEC3(j, "color", c.m_Color);
        JSON_GET_VEC2(j, "scale", c.m_Scale);
        JSON_GET(j, "opacity", c.m_Opacity);
        JSON_GET(j, "exponent", c.m_Exponent);
    }

    json serializeFilmGrain(const fb::CapturedFilmGrainComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["linearFilteringEnable"] = c.m_LinearFilteringEnable;
        j["randomEnable"] = c.m_RandomEnable;
        j["colorScale"] = serialize::vec3ToJson(c.m_ColorScale);
        j["textureScale"] = serialize::vec2ToJson(c.m_TextureScale);
        return j;
    }

    void deserializeFilmGrain(const json& j, fb::CapturedFilmGrainComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "linearFilteringEnable", c.m_LinearFilteringEnable);
        JSON_GET_BOOL(j, "randomEnable", c.m_RandomEnable);
        JSON_GET_VEC3(j, "colorScale", c.m_ColorScale);
        JSON_GET_VEC2(j, "textureScale", c.m_TextureScale);
    }

    json serializeLensScope(const fb::CapturedLensScopeComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["blurCenter"] = serialize::vec2ToJson(c.m_BlurCenter);
        j["blurScale"] = c.m_BlurScale;
        j["radialBlendDistanceCoefficients"] = serialize::vec2ToJson(c.m_RadialBlendDistanceCoefficients);
        j["chromaticAberrationColor1"] = serialize::vec3ToJson(c.m_ChromaticAberrationColor1);
        j["chromaticAberrationColor2"] = serialize::vec3ToJson(c.m_ChromaticAberrationColor2);
        j["chromaticAberrationDisplacement1"] = serialize::vec2ToJson(c.m_ChromaticAberrationDisplacement1);
        j["chromaticAberrationDisplacement2"] = serialize::vec2ToJson(c.m_ChromaticAberrationDisplacement2);
        j["chromaticAberrationStrengths"] = serialize::vec2ToJson(c.m_ChromaticAberrationStrengths);
        return j;
    }

    void deserializeLensScope(const json& j, fb::CapturedLensScopeComponentData& c)
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

    json serializeCameraParams(const fb::CapturedCameraParamsComponentData& c)
    {
        json j;
        j["viewDistance"] = c.m_ViewDistance;
        j["nearPlane"] = c.m_NearPlane;
        j["sunShadowmapViewDistance"] = c.m_SunShadowmapViewDistance;
        return j;
    }

    void deserializeCameraParams(const json& j, fb::CapturedCameraParamsComponentData& c)
    {
        JSON_GET(j, "viewDistance", c.m_ViewDistance);
        JSON_GET(j, "nearPlane", c.m_NearPlane);
        JSON_GET(j, "sunShadowmapViewDistance", c.m_SunShadowmapViewDistance);
    }

    json serializeScreenEffect(const fb::CapturedScreenEffectComponentData& c)
    {
        json j;
        j["frameType"] = static_cast<int>(c.m_FrameType);
        j["screenEffectParams"] = serialize::vec4ToJson(c.m_ScreenEffectParams);
        j["frameWidth"] = c.m_FrameWidth;
        j["outerFrameOpacity"] = c.m_OuterFrameOpacity;
        j["innerFrameOpacity"] = c.m_InnerFrameOpacity;
        j["angle"] = c.m_Angle;
        return j;
    }

    void deserializeScreenEffect(const json& j, fb::CapturedScreenEffectComponentData& c)
    {
        JSON_GET_ENUM(j, "frameType", c.m_FrameType, fb::ScreenEffectFrameType);
        JSON_GET_VEC4(j, "screenEffectParams", c.m_ScreenEffectParams);
        JSON_GET(j, "frameWidth", c.m_FrameWidth);
        JSON_GET(j, "outerFrameOpacity", c.m_OuterFrameOpacity);
        JSON_GET(j, "innerFrameOpacity", c.m_InnerFrameOpacity);
        JSON_GET(j, "angle", c.m_Angle);
    }

    json serializeDamageEffect(const fb::CapturedDamageEffectComponentData& c)
    {
        json j;
        j["debugDamage"] = c.m_DebugDamage;
        j["topDamage"] = serialize::vec4ToJson(c.m_TopDamage);
        j["bottomDamage"] = serialize::vec4ToJson(c.m_BottomDamage);
        j["leftDamage"] = serialize::vec4ToJson(c.m_LeftDamage);
        j["rightDamage"] = serialize::vec4ToJson(c.m_RightDamage);
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

    void deserializeDamageEffect(const json& j, fb::CapturedDamageEffectComponentData& c)
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

    json serializePlanarReflection(const fb::CapturedPlanarReflectionComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["skyRenderEnable"] = c.m_SkyRenderEnable;
        j["terrainReflectionsEnable"] = c.m_TerrainReflectionsEnable;
        j["overideOutdoorLightColors"] = c.m_OverideOutdoorLightColors;
        j["groundHeight"] = c.m_GroundHeight;
        j["viewDistance"] = c.m_ViewDistance;
        j["keyColorReflection"] = serialize::vec3ToJson(c.m_KeyColorReflection);
        j["skyColorReflection"] = serialize::vec3ToJson(c.m_SkyColorReflection);
        j["groundColorReflection"] = serialize::vec3ToJson(c.m_GroundColorReflection);
        j["horizontalBlurFilter"] = static_cast<int>(c.m_HorizontalBlurFilter);
        j["horizontalDeviation"] = c.m_HorizontalDeviation;
        j["verticalBlurFilter"] = static_cast<int>(c.m_VerticalBlurFilter);
        j["verticalDeviation"] = c.m_VerticalDeviation;
        return j;
    }

    void deserializePlanarReflection(const json& j, fb::CapturedPlanarReflectionComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "skyRenderEnable", c.m_SkyRenderEnable);
        JSON_GET_BOOL(j, "terrainReflectionsEnable", c.m_TerrainReflectionsEnable);
        JSON_GET_BOOL(j, "overideOutdoorLightColors", c.m_OverideOutdoorLightColors);
        JSON_GET(j, "groundHeight", c.m_GroundHeight);
        JSON_GET(j, "viewDistance", c.m_ViewDistance);
        JSON_GET_VEC3(j, "keyColorReflection", c.m_KeyColorReflection);
        JSON_GET_VEC3(j, "skyColorReflection", c.m_SkyColorReflection);
        JSON_GET_VEC3(j, "groundColorReflection", c.m_GroundColorReflection);
        JSON_GET_ENUM(j, "horizontalBlurFilter", c.m_HorizontalBlurFilter, fb::BlurFilter);
        JSON_GET(j, "horizontalDeviation", c.m_HorizontalDeviation);
        JSON_GET_ENUM(j, "verticalBlurFilter", c.m_VerticalBlurFilter, fb::BlurFilter);
        JSON_GET(j, "verticalDeviation", c.m_VerticalDeviation);
    }

    json serializeDynamicEnvmap(const fb::CapturedDynamicEnvmapComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["terrainReflectionsEnable"] = c.m_TerrainReflectionsEnable;
        j["skyColorEnvmap"] = serialize::vec3ToJson(c.m_SkyColorEnvmap);
        j["groundColorEnvmap"] = serialize::vec3ToJson(c.m_GroundColorEnvmap);
        j["keyColorEnvmap"] = serialize::vec3ToJson(c.m_KeyColorEnvmap);
        return j;
    }

    void deserializeDynamicEnvmap(const json& j, fb::CapturedDynamicEnvmapComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "terrainReflectionsEnable", c.m_TerrainReflectionsEnable);
        JSON_GET_VEC3(j, "skyColorEnvmap", c.m_SkyColorEnvmap);
        JSON_GET_VEC3(j, "groundColorEnvmap", c.m_GroundColorEnvmap);
        JSON_GET_VEC3(j, "keyColorEnvmap", c.m_KeyColorEnvmap);
    }

    json serializeCharacterLighting(const fb::CapturedCharacterLightingComponentData& c)
    {
        json j;
        j["characterLightEnable"] = c.m_CharacterLightEnable;
        j["firstPersonEnable"] = c.m_FirstPersonEnable;
        j["lockToCameraDirection"] = c.m_LockToCameraDirection;
        j["characterLightingMode"] = static_cast<int>(c.m_CharacterLightingMode);
        j["topLight"] = serialize::vec3ToJson(c.m_TopLight);
        j["bottomLight"] = serialize::vec3ToJson(c.m_BottomLight);
        j["topLightDirX"] = c.m_TopLightDirX;
        j["topLightDirY"] = c.m_TopLightDirY;
        j["cameraUpRotation"] = c.m_CameraUpRotation;
        j["blendFactor"] = c.m_BlendFactor;
        j["startFadeDistance"] = c.m_StartFadeDistance;
        j["endFadeDistance"] = c.m_EndFadeDistance;
        j["startFadeOutDistance"] = c.m_StartFadeOutDistance;
        j["endFadeOutDistance"] = c.m_EndFadeOutDistance;
        return j;
    }

    void deserializeCharacterLighting(const json& j, fb::CapturedCharacterLightingComponentData& c)
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
        JSON_GET(j, "startFadeDistance", c.m_StartFadeDistance);
        JSON_GET(j, "endFadeDistance", c.m_EndFadeDistance);
        JSON_GET(j, "startFadeOutDistance", c.m_StartFadeOutDistance);
        JSON_GET(j, "endFadeOutDistance", c.m_EndFadeOutDistance);
    }

    json serializeMotionBlur(const fb::CapturedMotionBlurComponentData& c)
    {
        json j;
        j["motionBlurEnable"] = c.m_MotionBlurEnable;
        j["motionBlurCentered"] = c.m_MotionBlurCentered;
        j["motionBlurScale"] = c.m_MotionBlurScale;
        j["motionBlurCutoffRadius"] = c.m_MotionBlurCutoffRadius;
        j["cutoffGradientScale"] = c.m_CutoffGradientScale;
        j["radialBlurEnable"] = c.m_RadialBlurEnable;
        j["radialBlurCenter"] = serialize::vec2ToJson(c.m_RadialBlurCenter);
        j["radialBlurOffset"] = c.m_RadialBlurOffset;
        j["radialBlurScale"] = c.m_RadialBlurScale;
        return j;
    }

    void deserializeMotionBlur(const json& j, fb::CapturedMotionBlurComponentData& c)
    {
        JSON_GET_BOOL(j, "motionBlurEnable", c.m_MotionBlurEnable);
        JSON_GET_BOOL(j, "motionBlurCentered", c.m_MotionBlurCentered);
        JSON_GET(j, "motionBlurScale", c.m_MotionBlurScale);
        JSON_GET(j, "motionBlurCutoffRadius", c.m_MotionBlurCutoffRadius);
        JSON_GET(j, "cutoffGradientScale", c.m_CutoffGradientScale);
        JSON_GET_BOOL(j, "radialBlurEnable", c.m_RadialBlurEnable);
        JSON_GET_VEC2(j, "radialBlurCenter", c.m_RadialBlurCenter);
        JSON_GET(j, "radialBlurOffset", c.m_RadialBlurOffset);
        JSON_GET(j, "radialBlurScale", c.m_RadialBlurScale);
    }

    json serializeVehicleLighting(const fb::CapturedVehicleLightingComponentData& c)
    {
        json j;
        j["vehicleLightEnable"] = c.m_VehicleLightEnable;
        j["firstPersonEnable"] = c.m_FirstPersonEnable;
        j["lockToCameraDirection"] = c.m_LockToCameraDirection;
        j["vehicleLightingMode"] = static_cast<int>(c.m_VehicleLightingMode);
        j["topLight"] = serialize::vec3ToJson(c.m_TopLight);
        j["bottomLight"] = serialize::vec3ToJson(c.m_BottomLight);
        j["topLightDirX"] = c.m_TopLightDirX;
        j["topLightDirY"] = c.m_TopLightDirY;
        j["cameraUpRotation"] = c.m_CameraUpRotation;
        j["blendFactor"] = c.m_BlendFactor;
        j["startFadeDistance"] = c.m_StartFadeDistance;
        j["endFadeDistance"] = c.m_EndFadeDistance;
        j["startFadeOutDistance"] = c.m_StartFadeOutDistance;
        j["endFadeOutDistance"] = c.m_EndFadeOutDistance;
        return j;
    }

    void deserializeVehicleLighting(const json& j, fb::CapturedVehicleLightingComponentData& c)
    {
        JSON_GET_BOOL(j, "vehicleLightEnable", c.m_VehicleLightEnable);
        JSON_GET_BOOL(j, "firstPersonEnable", c.m_FirstPersonEnable);
        JSON_GET_BOOL(j, "lockToCameraDirection", c.m_LockToCameraDirection);
        JSON_GET_ENUM(j, "vehicleLightingMode", c.m_VehicleLightingMode, fb::VehicleLightingMode);
        JSON_GET_VEC3(j, "topLight", c.m_TopLight);
        JSON_GET_VEC3(j, "bottomLight", c.m_BottomLight);
        JSON_GET(j, "topLightDirX", c.m_TopLightDirX);
        JSON_GET(j, "topLightDirY", c.m_TopLightDirY);
        JSON_GET(j, "cameraUpRotation", c.m_CameraUpRotation);
        JSON_GET(j, "blendFactor", c.m_BlendFactor);
        JSON_GET(j, "startFadeDistance", c.m_StartFadeDistance);
        JSON_GET(j, "endFadeDistance", c.m_EndFadeDistance);
        JSON_GET(j, "startFadeOutDistance", c.m_StartFadeOutDistance);
        JSON_GET(j, "endFadeOutDistance", c.m_EndFadeOutDistance);
    }

    json serializeSubSurfaceScattering(const fb::CapturedSubSurfaceScatteringComponentData& c)
    {
        json j;
        j["advancedSssEnable"] = c.m_AdvancedSssEnable;
        j["simpleSssColor"] = serialize::vec3ToJson(c.m_SimpleSssColor);
        j["simpleSssRolloffKeyLight"] = c.m_SimpleSssRolloffKeyLight;
        j["simpleSssRolloffLocalLight"] = c.m_SimpleSssRolloffLocalLight;
        j["advancedSssMat0Width"] = serialize::vec3ToJson(c.m_AdvancedSssMat0Width);
        j["advancedSssMat1Width"] = serialize::vec3ToJson(c.m_AdvancedSssMat1Width);
        j["advancedSssMat2Width"] = serialize::vec3ToJson(c.m_AdvancedSssMat2Width);
        j["advancedSssMat3Width"] = serialize::vec3ToJson(c.m_AdvancedSssMat3Width);
        j["advancedSssMat4Width"] = serialize::vec3ToJson(c.m_AdvancedSssMat4Width);
        return j;
    }

    void deserializeSubSurfaceScattering(const json& j, fb::CapturedSubSurfaceScatteringComponentData& c)
    {
        JSON_GET_BOOL(j, "advancedSssEnable", c.m_AdvancedSssEnable);
        JSON_GET_VEC3(j, "simpleSssColor", c.m_SimpleSssColor);
        JSON_GET(j, "simpleSssRolloffKeyLight", c.m_SimpleSssRolloffKeyLight);
        JSON_GET(j, "simpleSssRolloffLocalLight", c.m_SimpleSssRolloffLocalLight);
        JSON_GET_VEC3(j, "advancedSssMat0Width", c.m_AdvancedSssMat0Width);
        JSON_GET_VEC3(j, "advancedSssMat1Width", c.m_AdvancedSssMat1Width);
        JSON_GET_VEC3(j, "advancedSssMat2Width", c.m_AdvancedSssMat2Width);
        JSON_GET_VEC3(j, "advancedSssMat3Width", c.m_AdvancedSssMat3Width);
        JSON_GET_VEC3(j, "advancedSssMat4Width", c.m_AdvancedSssMat4Width);
    }
}

#endif // BFVE_GAME_BF4
