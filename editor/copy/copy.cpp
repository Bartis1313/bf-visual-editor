#include "copy.h"
#include <cstring>

// COPY RULES:
// instead of memcpy whole object we copy only important fields
// this is because changing textures, realm will be very invalid
// and for trick we still use memcpy but partialy
namespace editor::copy
{
    void outdoorLight(fb::OutdoorLightComponentData* dst, const fb::OutdoorLightComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::OutdoorLightComponentData;

        memcpy(&dst->m_SunColor, &src->m_SunColor, offsetof(T, _0x0078) - offsetof(T, m_SunColor));
        memcpy(&dst->m_SkyColor, &src->m_SkyColor, offsetof(T, m_Realm) - offsetof(T, m_SkyColor));
        memcpy(&dst->m_SkyLightAngleFactor, &src->m_SkyLightAngleFactor, offsetof(T, m_CloudShadowTexture) - offsetof(T, m_SkyLightAngleFactor));
        memcpy(&dst->m_TranslucencyDistortion, &src->m_TranslucencyDistortion, offsetof(T, _0x00DE) - offsetof(T, m_TranslucencyDistortion));
    }

    void enlighten(fb::EnlightenComponentData* dst, const fb::EnlightenComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::EnlightenComponentData;

        memcpy(&dst->m_SkyBoxSkyColor, &src->m_SkyBoxSkyColor, offsetof(T, m_Realm) - offsetof(T, m_SkyBoxSkyColor));
        memcpy(&dst->m_SkyBoxBackLightRotationY, &src->m_SkyBoxBackLightRotationY, offsetof(T, _0x00D2) - offsetof(T, m_SkyBoxBackLightRotationY));
    }

    void tonemap(fb::TonemapComponentData* dst, const fb::TonemapComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::TonemapComponentData;

        memcpy(&dst->m_BloomScale, &src->m_BloomScale, offsetof(T, m_Realm) - offsetof(T, m_BloomScale));
        memcpy(&dst->m_TonemapMethod, &src->m_TonemapMethod, offsetof(T, _0x0091) - offsetof(T, m_TonemapMethod));
    }

    void colorCorrection(fb::ColorCorrectionComponentData* dst, const fb::ColorCorrectionComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::ColorCorrectionComponentData;

        memcpy(&dst->m_Contrast, &src->m_Contrast, offsetof(T, m_Realm) - offsetof(T, m_Contrast));

        dst->m_Hue = src->m_Hue;
        dst->m_ColorGradingEnable = src->m_ColorGradingEnable;
        dst->m_Enable = src->m_Enable;
    }

    void sky(fb::SkyComponentData* dst, const fb::SkyComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::SkyComponentData;

        memcpy(&dst->m_CloudLayerSunColor, &src->m_CloudLayerSunColor, offsetof(T, m_SkyGradientTexture) - offsetof(T, m_CloudLayerSunColor));
        memcpy(&dst->m_SunSize, &src->m_SunSize, offsetof(T, m_Realm) - offsetof(T, m_SunSize));
        dst->m_PanoramicUVMinX = src->m_PanoramicUVMinX;
        dst->m_PanoramicTileFactor = src->m_PanoramicTileFactor;
        memcpy(&dst->m_CloudLayer1Altitude, &src->m_CloudLayer1Altitude, offsetof(T, m_CloudLayer1Texture) - offsetof(T, m_CloudLayer1Altitude));
        memcpy(&dst->m_CloudLayer2Altitude, &src->m_CloudLayer2Altitude, offsetof(T, m_CloudLayer2Texture) - offsetof(T, m_CloudLayer2Altitude));
        memcpy(&dst->m_SkyVisibilityExponent, &src->m_SkyVisibilityExponent, offsetof(T, m_CustomEnvmapTexture) - offsetof(T, m_SkyVisibilityExponent));
        memcpy(&dst->m_CustomEnvmapScale, &src->m_CustomEnvmapScale, offsetof(T, _0x012D) - offsetof(T, m_CustomEnvmapScale));
    }

    void fog(fb::FogComponentData* dst, const fb::FogComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::FogComponentData;

        memcpy(&dst->m_Curve, &src->m_Curve, offsetof(T, m_Realm) - offsetof(T, m_Curve));
        memcpy(&dst->m_End, &src->m_End, offsetof(T, _0x00C8) - offsetof(T, m_End));
    }

    void wind(fb::WindComponentData* dst, const fb::WindComponentData* src)
    {
        if (!dst || !src)
            return;

        dst->m_WindDirection = src->m_WindDirection;
        dst->m_WindStrength = src->m_WindStrength;
    }

    void sunFlare(fb::SunFlareComponentData* dst, const fb::SunFlareComponentData* src)
    {
        if (!dst || !src) 
            return;

        dst->m_Enable = src->m_Enable;
        dst->m_DebugDrawOccluder = src->m_DebugDrawOccluder;
        dst->m_OccluderSize = src->m_OccluderSize;
        
        // should be SDK changee to wrap it under static array
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

    void dynamicAO(fb::DynamicAOComponentData* dst, const fb::DynamicAOComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::DynamicAOComponentData;

        memcpy(&dst->m_SsaoMaxDistanceOuter, &src->m_SsaoMaxDistanceOuter, offsetof(T, _0x008D) - offsetof(T, m_SsaoMaxDistanceOuter));
    }

    void dof(fb::DofComponentData* dst, const fb::DofComponentData* src)
    {
        if (!dst || !src) 
            return;

        using T = fb::DofComponentData;

        memcpy(&dst->m_NearDistanceScale, &src->m_NearDistanceScale, offsetof(T, _0x008A) - offsetof(T, m_NearDistanceScale));
    }

    void vignette(fb::VignetteComponentData* dst, const fb::VignetteComponentData* src)
    {
        if (!dst || !src) 
            return;

        dst->m_Enable = src->m_Enable;
        dst->m_Color = src->m_Color;
        dst->m_Scale = src->m_Scale;
        dst->m_Opacity = src->m_Opacity;
        dst->m_Exponent = src->m_Exponent;
    }

    void filmGrain(fb::FilmGrainComponentData* dst, const fb::FilmGrainComponentData* src)
    {
        if (!dst || !src) 
            return;

        dst->m_Enable = src->m_Enable;
        dst->m_LinearFilteringEnable = src->m_LinearFilteringEnable;
        dst->m_RandomEnable = src->m_RandomEnable;
        dst->m_ColorScale = src->m_ColorScale;
        dst->m_TextureScale = src->m_TextureScale;
    }

    void lensScope(fb::LensScopeComponentData* dst, const fb::LensScopeComponentData* src)
    {
        if (!dst || !src) 
            return;

        using T = fb::LensScopeComponentData;

        memcpy(&dst->m_ChromaticAberrationColor1, &src->m_ChromaticAberrationColor1, offsetof(T, _0x0078) - offsetof(T, m_ChromaticAberrationColor1));
        memcpy(&dst->m_ChromaticAberrationColor2, &src->m_ChromaticAberrationColor2, offsetof(T, m_Realm) - offsetof(T, m_ChromaticAberrationColor2));

        dst->m_Enable = src->m_Enable;
    }

    void cameraParams(fb::CameraParamsComponentData* dst, const fb::CameraParamsComponentData* src)
    {
        if (!dst || !src) 
            return;

        dst->m_ViewDistance = src->m_ViewDistance;
        dst->m_NearPlane = src->m_NearPlane;
        dst->m_SunShadowmapViewDistance = src->m_SunShadowmapViewDistance;
    }

    void screenEffect(fb::ScreenEffectComponentData* dst, const fb::ScreenEffectComponentData* src)
    {
        if (!dst || !src) 
            return;

        dst->m_FrameType = src->m_FrameType;
        dst->m_ScreenEffectParams = src->m_ScreenEffectParams;
        dst->m_FrameWidth = src->m_FrameWidth;
        dst->m_OuterFrameOpacity = src->m_OuterFrameOpacity;
        dst->m_InnerFrameOpacity = src->m_InnerFrameOpacity;
        dst->m_Angle = src->m_Angle;
    }

    void damageEffect(fb::DamageEffectComponentData* dst, const fb::DamageEffectComponentData* src)
    {
        if (!dst || !src) 
            return;

        using T = fb::DamageEffectComponentData;

        memcpy(&dst->m_RightDamage, &src->m_RightDamage, offsetof(T, m_Shader) - offsetof(T, m_RightDamage));
        memcpy(&dst->m_StartCriticalEffectHealthThreshold, &src->m_StartCriticalEffectHealthThreshold, offsetof(T, m_Realm) - offsetof(T, m_StartCriticalEffectHealthThreshold));

        dst->m_DebugDamage = src->m_DebugDamage;
    }

    void planarReflection(fb::PlanarReflectionComponentData* dst, const fb::PlanarReflectionComponentData* src)
    {
        if (!dst || !src) 
            return;

        using T = fb::PlanarReflectionComponentData;

        memcpy(&dst->m_VerticalBlurFilter, &src->m_VerticalBlurFilter, offsetof(T, _0x0076) - offsetof(T, m_VerticalBlurFilter));
    }

    void dynamicEnvmap(fb::DynamicEnvmapComponentData* dst, const fb::DynamicEnvmapComponentData* src)
    {
        if (!dst || !src) 
            return;

        dst->m_Enable = src->m_Enable;
        dst->m_SkyColorEnvmap = src->m_SkyColorEnvmap;
        dst->m_GroundColorEnvmap = src->m_GroundColorEnvmap;
        dst->m_KeyColorEnvmap = src->m_KeyColorEnvmap;
    }

    void characterLighting(fb::CharacterLightingComponentData* dst, const fb::CharacterLightingComponentData* src)
    {
        if (!dst || !src) 
            return;

        using T = fb::CharacterLightingComponentData;

        memcpy(&dst->m_BottomLight, &src->m_BottomLight, offsetof(T, _0x0097) - offsetof(T, m_BottomLight));
    }

    void motionBlur(fb::MotionBlurComponentData* dst, const fb::MotionBlurComponentData* src)
    {
        if (!dst || !src)
            return;

        dst->m_MotionBlurEnable = src->m_MotionBlurEnable;
        dst->m_MotionBlurCutoffRadius = src->m_MotionBlurCutoffRadius;
        dst->m_CutoffGradientScale = src->m_CutoffGradientScale;
    }

    void worldRenderSettings(fb::WorldRenderSettings* dst, const fb::WorldRenderSettings* src)
    {
        if (!dst || !src)
            return;
        
        constexpr size_t startOffset = sizeof(fb::DataContainer) + sizeof(fb::WorldRenderSettings::SKIPME);
        constexpr size_t endOffset = offsetof(fb::WorldRenderSettings, m_ViewFxEnable) + sizeof(fb::WorldRenderSettings::m_ViewFxEnable);
        constexpr size_t copySize = endOffset - startOffset;

        memcpy(reinterpret_cast<char*>(dst) + startOffset, reinterpret_cast<const char*>(src) + startOffset, copySize);
    }
}