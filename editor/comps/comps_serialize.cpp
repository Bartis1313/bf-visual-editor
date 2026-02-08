#include "comps_serialize.h"

#include "../serialize/serialize.h"

namespace editor::comps
{
    json serializeOutdoorLight(const fb::OutdoorLightComponentData& c)
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
        j["cloudShadowEnable"] = c.m_CloudShadowEnable;
        j["cloudShadowSpeed"] = serialize::vec2ToJson(c.m_CloudShadowSpeed);
        j["cloudShadowCoverage"] = c.m_CloudShadowCoverage;
        j["cloudShadowSize"] = c.m_CloudShadowSize;
        j["cloudShadowExponent"] = c.m_CloudShadowExponent;
        j["translucencyDistortion"] = c.m_TranslucencyDistortion;
        j["translucencyAmbient"] = c.m_TranslucencyAmbient;
        j["translucencyScale"] = c.m_TranslucencyScale;
        j["translucencyPower"] = c.m_TranslucencyPower;
        return j;
    }

    void deserializeOutdoorLight(const json& j, fb::OutdoorLightComponentData& c)
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

    json serializeEnlighten(const fb::EnlightenComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["skyBoxEnable"] = c.m_SkyBoxEnable;
        j["skyBoxSkyColor"] = serialize::vec3ToJson(c.m_SkyBoxSkyColor);
        j["skyBoxGroundColor"] = serialize::vec3ToJson(c.m_SkyBoxGroundColor);
        j["skyBoxSunLightColor"] = serialize::vec3ToJson(c.m_SkyBoxSunLightColor);
        j["skyBoxBackLightColor"] = serialize::vec3ToJson(c.m_SkyBoxBackLightColor);
        j["terrainColor"] = serialize::vec3ToJson(c.m_TerrainColor);
        j["bounceScale"] = c.m_BounceScale;
        j["sunScale"] = c.m_SunScale;
        j["cullDistance"] = c.m_CullDistance;
        j["skyBoxSunLightColorSize"] = c.m_SkyBoxSunLightColorSize;
        j["skyBoxBackLightColorSize"] = c.m_SkyBoxBackLightColorSize;
        j["skyBoxBackLightRotationX"] = c.m_SkyBoxBackLightRotationX;
        j["skyBoxBackLightRotationY"] = c.m_SkyBoxBackLightRotationY;
        return j;
    }

    void deserializeEnlighten(const json& j, fb::EnlightenComponentData& c)
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

    json serializeTonemap(const fb::TonemapComponentData& c)
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

    void deserializeTonemap(const json& j, fb::TonemapComponentData& c)
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

    json serializeColorCorrection(const fb::ColorCorrectionComponentData& c)
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

    void deserializeColorCorrection(const json& j, fb::ColorCorrectionComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "colorGradingEnable", c.m_ColorGradingEnable);
        JSON_GET_VEC3(j, "brightness", c.m_Brightness);
        JSON_GET_VEC3(j, "contrast", c.m_Contrast);
        JSON_GET_VEC3(j, "saturation", c.m_Saturation);
        JSON_GET(j, "hue", c.m_Hue);
    }

    json serializeSky(const fb::SkyComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
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

    void deserializeSky(const json& j, fb::SkyComponentData& c)
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
        JSON_GET(j, "EnvmapScale", c.m_StaticEnvmapScale);
    }

    json serializeFog(const fb::FogComponentData& c)
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
        return j;
    }

    void deserializeFog(const json& j, fb::FogComponentData& c)
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

    json serializeWind(const fb::WindComponentData& c)
    {
        return { {"windDirection", c.m_WindDirection}, {"windStrength", c.m_WindStrength} };
    }

    void deserializeWind(const json& j, fb::WindComponentData& c)
    {
        JSON_GET(j, "windDirection", c.m_WindDirection);
        JSON_GET(j, "windStrength", c.m_WindStrength);
    }

    json serializeSunFlare(const fb::SunFlareComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["debugDrawOccluder"] = c.m_DebugDrawOccluder;
        j["occluderSize"] = c.m_OccluderSize;

#define SERIALIZE_ELEMENT(n) \
        j["element" #n "Enable"] = c.m_Element##n##Enable; \
        j["element" #n "Size"] = serialize::vec2ToJson(c.m_Element##n##Size); \
        j["element" #n "RayDistance"] = c.m_Element##n##RayDistance;

        SERIALIZE_ELEMENT(1);
        SERIALIZE_ELEMENT(2);
        SERIALIZE_ELEMENT(3);
        SERIALIZE_ELEMENT(4);
        SERIALIZE_ELEMENT(5);

#undef SERIALIZE_ELEMENT
            return j;
    }

    void deserializeSunFlare(const json& j, fb::SunFlareComponentData& c)
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

    json serializeDynamicAO(const fb::DynamicAOComponentData& c)
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

    void deserializeDynamicAO(const json& j, fb::DynamicAOComponentData& c)
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

    json serializeDof(const fb::DofComponentData& c)
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

    void deserializeDof(const json& j, fb::DofComponentData& c)
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

    json serializeVignette(const fb::VignetteComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["color"] = serialize::vec3ToJson(c.m_Color);
        j["scale"] = serialize::vec2ToJson(c.m_Scale);
        j["opacity"] = c.m_Opacity;
        j["exponent"] = c.m_Exponent;
        return j;
    }

    void deserializeVignette(const json& j, fb::VignetteComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_VEC3(j, "color", c.m_Color);
        JSON_GET_VEC2(j, "scale", c.m_Scale);
        JSON_GET(j, "opacity", c.m_Opacity);
        JSON_GET(j, "exponent", c.m_Exponent);
    }

    json serializeFilmGrain(const fb::FilmGrainComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["linearFilteringEnable"] = c.m_LinearFilteringEnable;
        j["randomEnable"] = c.m_RandomEnable;
        j["colorScale"] = serialize::vec3ToJson(c.m_ColorScale);
        j["textureScale"] = serialize::vec2ToJson(c.m_TextureScale);
        return j;
    }

    void deserializeFilmGrain(const json& j, fb::FilmGrainComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "linearFilteringEnable", c.m_LinearFilteringEnable);
        JSON_GET_BOOL(j, "randomEnable", c.m_RandomEnable);
        JSON_GET_VEC3(j, "colorScale", c.m_ColorScale);
        JSON_GET_VEC2(j, "textureScale", c.m_TextureScale);
    }

    json serializeLensScope(const fb::LensScopeComponentData& c)
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

    void deserializeLensScope(const json& j, fb::LensScopeComponentData& c)
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

    json serializeCameraParams(const fb::CameraParamsComponentData& c)
    {
        json j;
        j["viewDistance"] = c.m_ViewDistance;
        j["nearPlane"] = c.m_NearPlane;
        j["sunShadowmapViewDistance"] = c.m_SunShadowmapViewDistance;
        return j;
    }

    void deserializeCameraParams(const json& j, fb::CameraParamsComponentData& c)
    {
        JSON_GET(j, "viewDistance", c.m_ViewDistance);
        JSON_GET(j, "nearPlane", c.m_NearPlane);
        JSON_GET(j, "sunShadowmapViewDistance", c.m_SunShadowmapViewDistance);
    }

    json serializeScreenEffect(const fb::ScreenEffectComponentData& c)
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

    void deserializeScreenEffect(const json& j, fb::ScreenEffectComponentData& c)
    {
        JSON_GET_ENUM(j, "frameType", c.m_FrameType, fb::ScreenEffectFrameType);
        JSON_GET_VEC4(j, "screenEffectParams", c.m_ScreenEffectParams);
        JSON_GET(j, "frameWidth", c.m_FrameWidth);
        JSON_GET(j, "outerFrameOpacity", c.m_OuterFrameOpacity);
        JSON_GET(j, "innerFrameOpacity", c.m_InnerFrameOpacity);
        JSON_GET(j, "angle", c.m_Angle);
    }

    json serializeDamageEffect(const fb::DamageEffectComponentData& c)
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

    void deserializeDamageEffect(const json& j, fb::DamageEffectComponentData& c)
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

    json serializePlanarReflection(const fb::PlanarReflectionComponentData& c)
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

    void deserializePlanarReflection(const json& j, fb::PlanarReflectionComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_BOOL(j, "skyRenderEnable", c.m_SkyRenderEnable);
        JSON_GET(j, "groundHeight", c.m_GroundHeight);
        JSON_GET_ENUM(j, "horizontalBlurFilter", c.m_HorizontalBlurFilter, fb::BlurFilter);
        JSON_GET(j, "horizontalDeviation", c.m_HorizontalDeviation);
        JSON_GET_ENUM(j, "verticalBlurFilter", c.m_VerticalBlurFilter, fb::BlurFilter);
        JSON_GET(j, "verticalDeviation", c.m_VerticalDeviation);
    }

    json serializeDynamicEnvmap(const fb::DynamicEnvmapComponentData& c)
    {
        json j;
        j["enable"] = c.m_Enable;
        j["skyColorEnvmap"] = serialize::vec3ToJson(c.m_SkyColorEnvmap);
        j["groundColorEnvmap"] = serialize::vec3ToJson(c.m_GroundColorEnvmap);
        j["keyColorEnvmap"] = serialize::vec3ToJson(c.m_KeyColorEnvmap);
        return j;
    }

    void deserializeDynamicEnvmap(const json& j, fb::DynamicEnvmapComponentData& c)
    {
        JSON_GET_BOOL(j, "enable", c.m_Enable);
        JSON_GET_VEC3(j, "skyColorEnvmap", c.m_SkyColorEnvmap);
        JSON_GET_VEC3(j, "groundColorEnvmap", c.m_GroundColorEnvmap);
        JSON_GET_VEC3(j, "keyColorEnvmap", c.m_KeyColorEnvmap);
    }

    json serializeCharacterLighting(const fb::CharacterLightingComponentData& c)
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
        return j;
    }

    void deserializeCharacterLighting(const json& j, fb::CharacterLightingComponentData& c)
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

    json serializeMotionBlur(const fb::MotionBlurComponentData& c)
    {
        json j;
        j["motionBlurEnable"] = c.m_MotionBlurEnable;
        j["motionBlurCutoffRadius"] = c.m_MotionBlurCutoffRadius;
        j["cutoffGradientScale"] = c.m_CutoffGradientScale;
        return j;
    }

    void deserializeMotionBlur(const json& j, fb::MotionBlurComponentData& c)
    {
        JSON_GET_BOOL(j, "motionBlurEnable", c.m_MotionBlurEnable);
        JSON_GET(j, "motionBlurCutoffRadius", c.m_MotionBlurCutoffRadius);
        JSON_GET(j, "cutoffGradientScale", c.m_CutoffGradientScale);
    }
}