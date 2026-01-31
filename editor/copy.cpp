#include "editor.h"

void VisualEnvironmentEditor::CopyOutdoorLight(fb::OutdoorLightComponentData* dst, const fb::OutdoorLightComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_SunColor = src->m_SunColor;
    dst->m_SkyColor = src->m_SkyColor;
    dst->m_GroundColor = src->m_GroundColor;
    dst->m_SunRotationX = src->m_SunRotationX;
    dst->m_SunRotationY = src->m_SunRotationY;
    dst->m_SunSpecularScale = src->m_SunSpecularScale;
    dst->m_SkyLightAngleFactor = src->m_SkyLightAngleFactor;
    dst->m_SkyEnvmapShadowScale = src->m_SkyEnvmapShadowScale;
    dst->m_SunShadowHeightScale = src->m_SunShadowHeightScale;
    dst->m_CloudShadowEnable = src->m_CloudShadowEnable;
    dst->m_CloudShadowSpeed = src->m_CloudShadowSpeed;
    dst->m_CloudShadowCoverage = src->m_CloudShadowCoverage;
    dst->m_CloudShadowSize = src->m_CloudShadowSize;
    dst->m_CloudShadowExponent = src->m_CloudShadowExponent;
    dst->m_TranslucencyDistortion = src->m_TranslucencyDistortion;
    dst->m_TranslucencyAmbient = src->m_TranslucencyAmbient;
    dst->m_TranslucencyScale = src->m_TranslucencyScale;
    dst->m_TranslucencyPower = src->m_TranslucencyPower;
}

void VisualEnvironmentEditor::CopyEnlighten(fb::EnlightenComponentData* dst, const fb::EnlightenComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_SkyBoxEnable = src->m_SkyBoxEnable;
    dst->m_SkyBoxSkyColor = src->m_SkyBoxSkyColor;
    dst->m_SkyBoxGroundColor = src->m_SkyBoxGroundColor;
    dst->m_SkyBoxSunLightColor = src->m_SkyBoxSunLightColor;
    dst->m_SkyBoxBackLightColor = src->m_SkyBoxBackLightColor;
    dst->m_TerrainColor = src->m_TerrainColor;
    dst->m_BounceScale = src->m_BounceScale;
    dst->m_SunScale = src->m_SunScale;
    dst->m_CullDistance = src->m_CullDistance;
    dst->m_SkyBoxSunLightColorSize = src->m_SkyBoxSunLightColorSize;
    dst->m_SkyBoxBackLightColorSize = src->m_SkyBoxBackLightColorSize;
    dst->m_SkyBoxBackLightRotationX = src->m_SkyBoxBackLightRotationX;
    dst->m_SkyBoxBackLightRotationY = src->m_SkyBoxBackLightRotationY;
}

void VisualEnvironmentEditor::CopyTonemap(fb::TonemapComponentData* dst, const fb::TonemapComponentData* src)
{
    if (!dst || !src) return;
    dst->m_TonemapMethod = src->m_TonemapMethod;
    dst->m_BloomScale = src->m_BloomScale;
    dst->m_MinExposure = src->m_MinExposure;
    dst->m_MaxExposure = src->m_MaxExposure;
    dst->m_MiddleGray = src->m_MiddleGray;
    dst->m_ExposureAdjustTime = src->m_ExposureAdjustTime;
    dst->m_ChromostereopsisEnable = src->m_ChromostereopsisEnable;
    dst->m_ChromostereopsisOffset = src->m_ChromostereopsisOffset;
    dst->m_ChromostereopsisScale = src->m_ChromostereopsisScale;
}

void VisualEnvironmentEditor::CopyColorCorrection(fb::ColorCorrectionComponentData* dst, const fb::ColorCorrectionComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_ColorGradingEnable = src->m_ColorGradingEnable;
    dst->m_Brightness = src->m_Brightness;
    dst->m_Contrast = src->m_Contrast;
    dst->m_Saturation = src->m_Saturation;
    dst->m_Hue = src->m_Hue;
}

void VisualEnvironmentEditor::CopySky(fb::SkyComponentData* dst, const fb::SkyComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_BrightnessScale = src->m_BrightnessScale;
    dst->m_SunSize = src->m_SunSize;
    dst->m_SunScale = src->m_SunScale;
    dst->m_SkyVisibilityExponent = src->m_SkyVisibilityExponent;
    dst->m_SkyEnvmap8BitTexScale = src->m_SkyEnvmap8BitTexScale;
    dst->m_CloudLayerSunColor = src->m_CloudLayerSunColor;
    dst->m_CloudLayer1Color = src->m_CloudLayer1Color;
    dst->m_CloudLayer1Altitude = src->m_CloudLayer1Altitude;
    dst->m_CloudLayer1TileFactor = src->m_CloudLayer1TileFactor;
    dst->m_CloudLayer1Rotation = src->m_CloudLayer1Rotation;
    dst->m_CloudLayer1Speed = src->m_CloudLayer1Speed;
    dst->m_CloudLayer1SunLightIntensity = src->m_CloudLayer1SunLightIntensity;
    dst->m_CloudLayer1SunLightPower = src->m_CloudLayer1SunLightPower;
    dst->m_CloudLayer1AmbientLightIntensity = src->m_CloudLayer1AmbientLightIntensity;
    dst->m_CloudLayer1AlphaMul = src->m_CloudLayer1AlphaMul;
    dst->m_CloudLayer2Color = src->m_CloudLayer2Color;
    dst->m_CloudLayer2Altitude = src->m_CloudLayer2Altitude;
    dst->m_CloudLayer2TileFactor = src->m_CloudLayer2TileFactor;
    dst->m_CloudLayer2Rotation = src->m_CloudLayer2Rotation;
    dst->m_CloudLayer2Speed = src->m_CloudLayer2Speed;
    dst->m_CloudLayer2SunLightIntensity = src->m_CloudLayer2SunLightIntensity;
    dst->m_CloudLayer2SunLightPower = src->m_CloudLayer2SunLightPower;
    dst->m_CloudLayer2AmbientLightIntensity = src->m_CloudLayer2AmbientLightIntensity;
    dst->m_CloudLayer2AlphaMul = src->m_CloudLayer2AlphaMul;
    dst->m_PanoramicRotation = src->m_PanoramicRotation;
    dst->m_PanoramicTileFactor = src->m_PanoramicTileFactor;
    dst->m_PanoramicUVMinX = src->m_PanoramicUVMinX;
    dst->m_PanoramicUVMaxX = src->m_PanoramicUVMaxX;
    dst->m_PanoramicUVMinY = src->m_PanoramicUVMinY;
    dst->m_PanoramicUVMaxY = src->m_PanoramicUVMaxY;
    dst->m_CustomEnvmapScale = src->m_CustomEnvmapScale;
    dst->m_CustomEnvmapAmbient = src->m_CustomEnvmapAmbient;
    dst->m_StaticEnvmapScale = src->m_StaticEnvmapScale;
}

void VisualEnvironmentEditor::CopyFog(fb::FogComponentData* dst, const fb::FogComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_Start = src->m_Start;
    dst->m_End = src->m_End;
    dst->m_FogDistanceMultiplier = src->m_FogDistanceMultiplier;
    dst->m_Curve = src->m_Curve;
    dst->m_FogColorEnable = src->m_FogColorEnable;
    dst->m_FogColor = src->m_FogColor;
    dst->m_FogColorStart = src->m_FogColorStart;
    dst->m_FogColorEnd = src->m_FogColorEnd;
    dst->m_FogColorCurve = src->m_FogColorCurve;
    dst->m_FogGradientEnable = src->m_FogGradientEnable;
    dst->m_HeightFogEnable = src->m_HeightFogEnable;
    dst->m_HeightFogAltitude = src->m_HeightFogAltitude;
    dst->m_HeightFogDepth = src->m_HeightFogDepth;
    dst->m_HeightFogVisibilityRange = src->m_HeightFogVisibilityRange;
    dst->m_HeightFogFollowCamera = src->m_HeightFogFollowCamera;
    dst->m_TransparencyFadeStart = src->m_TransparencyFadeStart;
    dst->m_TransparencyFadeEnd = src->m_TransparencyFadeEnd;
    dst->m_TransparencyFadeClamp = src->m_TransparencyFadeClamp;
}

void VisualEnvironmentEditor::CopyWind(fb::WindComponentData* dst, const fb::WindComponentData* src)
{
    if (!dst || !src) return;
    dst->m_WindDirection = src->m_WindDirection;
    dst->m_WindStrength = src->m_WindStrength;
}

void VisualEnvironmentEditor::CopySunFlare(fb::SunFlareComponentData* dst, const fb::SunFlareComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_DebugDrawOccluder = src->m_DebugDrawOccluder;
    dst->m_OccluderSize = src->m_OccluderSize;

#define COPY_ELEMENT(n) \
    dst->m_Element##n##Enable = src->m_Element##n##Enable; \
    dst->m_Element##n##Size = src->m_Element##n##Size; \
    dst->m_Element##n##RayDistance = src->m_Element##n##RayDistance; \
    dst->m_Element##n##AlphaScreenPosCurve = src->m_Element##n##AlphaScreenPosCurve; \
    dst->m_Element##n##AlphaOccluderCurve = src->m_Element##n##AlphaOccluderCurve; \
    dst->m_Element##n##SizeScreenPosCurve = src->m_Element##n##SizeScreenPosCurve; \
    dst->m_Element##n##SizeOccluderCurve = src->m_Element##n##SizeOccluderCurve;

    COPY_ELEMENT(1) COPY_ELEMENT(2) COPY_ELEMENT(3) COPY_ELEMENT(4) COPY_ELEMENT(5)
#undef COPY_ELEMENT
}

void VisualEnvironmentEditor::CopyDynamicAO(fb::DynamicAOComponentData* dst, const fb::DynamicAOComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_SsaoRadius = src->m_SsaoRadius;
    dst->m_SsaoFade = src->m_SsaoFade;
    dst->m_SsaoMaxDistanceInner = src->m_SsaoMaxDistanceInner;
    dst->m_SsaoMaxDistanceOuter = src->m_SsaoMaxDistanceOuter;
    dst->m_HbaoRadius = src->m_HbaoRadius;
    dst->m_HbaoAttenuation = src->m_HbaoAttenuation;
    dst->m_HbaoAngleBias = src->m_HbaoAngleBias;
    dst->m_HbaoPowerExponent = src->m_HbaoPowerExponent;
    dst->m_HbaoContrast = src->m_HbaoContrast;
    dst->m_HbaoMaxFootprintRadius = src->m_HbaoMaxFootprintRadius;
}

void VisualEnvironmentEditor::CopyDof(fb::DofComponentData* dst, const fb::DofComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_BlurFilter = src->m_BlurFilter;
    dst->m_FocusDistance = src->m_FocusDistance;
    dst->m_NearDistanceScale = src->m_NearDistanceScale;
    dst->m_FarDistanceScale = src->m_FarDistanceScale;
    dst->m_BlurFilterDeviation = src->m_BlurFilterDeviation;
    dst->m_BlurAdd = src->m_BlurAdd;
    dst->m_Scale = src->m_Scale;
    dst->m_DiffusionDofEnable = src->m_DiffusionDofEnable;
    dst->m_DiffusionDofFocalLength = src->m_DiffusionDofFocalLength;
    dst->m_DiffusionDofAperture = src->m_DiffusionDofAperture;
}

void VisualEnvironmentEditor::CopyVignette(fb::VignetteComponentData* dst, const fb::VignetteComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_Color = src->m_Color;
    dst->m_Scale = src->m_Scale;
    dst->m_Opacity = src->m_Opacity;
    dst->m_Exponent = src->m_Exponent;
}

void VisualEnvironmentEditor::CopyFilmGrain(fb::FilmGrainComponentData* dst, const fb::FilmGrainComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_LinearFilteringEnable = src->m_LinearFilteringEnable;
    dst->m_RandomEnable = src->m_RandomEnable;
    dst->m_ColorScale = src->m_ColorScale;
    dst->m_TextureScale = src->m_TextureScale;
}

void VisualEnvironmentEditor::CopyLensScope(fb::LensScopeComponentData* dst, const fb::LensScopeComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_BlurCenter = src->m_BlurCenter;
    dst->m_BlurScale = src->m_BlurScale;
    dst->m_RadialBlendDistanceCoefficients = src->m_RadialBlendDistanceCoefficients;
    dst->m_ChromaticAberrationColor1 = src->m_ChromaticAberrationColor1;
    dst->m_ChromaticAberrationColor2 = src->m_ChromaticAberrationColor2;
    dst->m_ChromaticAberrationDisplacement1 = src->m_ChromaticAberrationDisplacement1;
    dst->m_ChromaticAberrationDisplacement2 = src->m_ChromaticAberrationDisplacement2;
    dst->m_ChromaticAberrationStrengths = src->m_ChromaticAberrationStrengths;
}

void VisualEnvironmentEditor::CopyCameraParams(fb::CameraParamsComponentData* dst, const fb::CameraParamsComponentData* src)
{
    if (!dst || !src) return;
    dst->m_ViewDistance = src->m_ViewDistance;
    dst->m_NearPlane = src->m_NearPlane;
    dst->m_SunShadowmapViewDistance = src->m_SunShadowmapViewDistance;
}

void VisualEnvironmentEditor::CopyScreenEffect(fb::ScreenEffectComponentData* dst, const fb::ScreenEffectComponentData* src)
{
    if (!dst || !src) return;
    dst->m_FrameType = src->m_FrameType;
    dst->m_ScreenEffectParams = src->m_ScreenEffectParams;
    dst->m_FrameWidth = src->m_FrameWidth;
    dst->m_OuterFrameOpacity = src->m_OuterFrameOpacity;
    dst->m_InnerFrameOpacity = src->m_InnerFrameOpacity;
    dst->m_Angle = src->m_Angle;
}

void VisualEnvironmentEditor::CopyDamageEffect(fb::DamageEffectComponentData* dst, const fb::DamageEffectComponentData* src)
{
    if (!dst || !src) return;
    dst->m_DebugDamage = src->m_DebugDamage;
    dst->m_TopDamage = src->m_TopDamage;
    dst->m_BottomDamage = src->m_BottomDamage;
    dst->m_LeftDamage = src->m_LeftDamage;
    dst->m_RightDamage = src->m_RightDamage;
    dst->m_FrameWidth = src->m_FrameWidth;
    dst->m_OuterFrameOpacity = src->m_OuterFrameOpacity;
    dst->m_InnerFrameOpacity = src->m_InnerFrameOpacity;
    dst->m_StartCriticalEffectHealthThreshold = src->m_StartCriticalEffectHealthThreshold;
    dst->m_EndCriticalEffectHealthThreshold = src->m_EndCriticalEffectHealthThreshold;
    dst->m_MinDamagePercentageThreshold = src->m_MinDamagePercentageThreshold;
    dst->m_FallofTime = src->m_FallofTime;
    dst->m_MaxOpacityDamagePercentage = src->m_MaxOpacityDamagePercentage;
}

void VisualEnvironmentEditor::CopyPlanarReflection(fb::PlanarReflectionComponentData* dst, const fb::PlanarReflectionComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_SkyRenderEnable = src->m_SkyRenderEnable;
    dst->m_GroundHeight = src->m_GroundHeight;
    dst->m_HorizontalBlurFilter = src->m_HorizontalBlurFilter;
    dst->m_HorizontalDeviation = src->m_HorizontalDeviation;
    dst->m_VerticalBlurFilter = src->m_VerticalBlurFilter;
    dst->m_VerticalDeviation = src->m_VerticalDeviation;
}

void VisualEnvironmentEditor::CopyDynamicEnvmap(fb::DynamicEnvmapComponentData* dst, const fb::DynamicEnvmapComponentData* src)
{
    if (!dst || !src) return;
    dst->m_Enable = src->m_Enable;
    dst->m_SkyColorEnvmap = src->m_SkyColorEnvmap;
    dst->m_GroundColorEnvmap = src->m_GroundColorEnvmap;
    dst->m_KeyColorEnvmap = src->m_KeyColorEnvmap;
}

void VisualEnvironmentEditor::CopyCharacterLighting(fb::CharacterLightingComponentData* dst, const fb::CharacterLightingComponentData* src)
{
    if (!dst || !src) return;
    dst->m_CharacterLightEnable = src->m_CharacterLightEnable;
    dst->m_FirstPersonEnable = src->m_FirstPersonEnable;
    dst->m_LockToCameraDirection = src->m_LockToCameraDirection;
    dst->m_CharacterLightingMode = src->m_CharacterLightingMode;
    dst->m_TopLight = src->m_TopLight;
    dst->m_BottomLight = src->m_BottomLight;
    dst->m_TopLightDirX = src->m_TopLightDirX;
    dst->m_TopLightDirY = src->m_TopLightDirY;
    dst->m_CameraUpRotation = src->m_CameraUpRotation;
    dst->m_BlendFactor = src->m_BlendFactor;
}

void VisualEnvironmentEditor::CopyMotionBlur(fb::MotionBlurComponentData* dst, const fb::MotionBlurComponentData* src)
{
    if (!dst || !src) return;
    dst->m_MotionBlurEnable = src->m_MotionBlurEnable;
    dst->m_MotionBlurCutoffRadius = src->m_MotionBlurCutoffRadius;
    dst->m_CutoffGradientScale = src->m_CutoffGradientScale;
}

void VisualEnvironmentEditor::CopyWorldRenderSettings(fb::WorldRenderSettings* dst, const fb::WorldRenderSettings* src)
{
    if (!dst || !src) return;

    constexpr size_t offset = sizeof(fb::DataContainer) + sizeof(fb::WorldRenderSettings::SKIPME);
    constexpr size_t size = sizeof(fb::WorldRenderSettings) - offset;

    memcpy(reinterpret_cast<char*>(dst) + offset, reinterpret_cast<const char*>(src) + offset, size);
}