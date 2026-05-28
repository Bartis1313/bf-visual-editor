#include "copy.h"
#include <cstring>
#include <cstddef>

#if defined(BFVE_GAME_BF4)

// COPY RULES:
// instead of memcpy whole object we copy only important fields
// this is because changing textures, realm will be very invalid
// and for trick we still use memcpy but partialy
namespace editor::copy
{
    void outdoorLight(fb::CapturedOutdoorLightComponentData* dst, const fb::CapturedOutdoorLightComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedOutdoorLightComponentData;

        memcpy(&dst->m_SunColor, &src->m_SunColor, offsetof(T, m_Realm) - offsetof(T, m_SunColor));
        memcpy(&dst->m_SunRotationX, &src->m_SunRotationX, offsetof(T, _0x00D4) - offsetof(T, m_SunRotationX));
        memcpy(&dst->m_CloudShadowSpeed, &src->m_CloudShadowSpeed, offsetof(T, _0x011C) - offsetof(T, m_CloudShadowSpeed));
    }

    void enlighten(fb::CapturedEnlightenComponentData* dst, const fb::CapturedEnlightenComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedEnlightenComponentData;

        memcpy(&dst->m_TerrainColor, &src->m_TerrainColor, offsetof(T, m_Realm) - offsetof(T, m_TerrainColor));
        memcpy(&dst->m_BounceScale, &src->m_BounceScale, offsetof(T, _0x0114) - offsetof(T, m_BounceScale));
        dst->m_SkyBoxEnable = src->m_SkyBoxEnable;
    }

    void tonemap(fb::CapturedTonemapComponentData* dst, const fb::CapturedTonemapComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedTonemapComponentData;

        dst->m_BloomScale = src->m_BloomScale;
        memcpy(&dst->m_TonemapMethod, &src->m_TonemapMethod, offsetof(T, _0x00B1) - offsetof(T, m_TonemapMethod));
    }

    void colorCorrection(fb::CapturedColorCorrectionComponentData* dst, const fb::CapturedColorCorrectionComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedColorCorrectionComponentData;
        memcpy(&dst->m_Brightness, &src->m_Brightness, offsetof(T, m_Realm) - offsetof(T, m_Brightness));
        dst->m_Hue = src->m_Hue;
        dst->m_Enable = src->m_Enable;
        dst->m_ColorGradingEnable = src->m_ColorGradingEnable;
    }

    void sky(fb::CapturedSkyComponentData* dst, const fb::CapturedSkyComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedSkyComponentData;

        memcpy(&dst->m_CloudLayerSunColor, &src->m_CloudLayerSunColor, offsetof(T, m_Realm) - offsetof(T, m_CloudLayerSunColor));
        dst->m_BrightnessScale = src->m_BrightnessScale;
        memcpy(&dst->m_SunSize, &src->m_SunSize, offsetof(T, m_PanoramicTexture) - offsetof(T, m_SunSize));
        memcpy(&dst->m_CloudLayer1Altitude, &src->m_CloudLayer1Altitude, offsetof(T, m_CloudLayer1Texture) - offsetof(T, m_CloudLayer1Altitude));
        memcpy(&dst->m_CloudLayer2Altitude, &src->m_CloudLayer2Altitude, offsetof(T, m_CloudLayer2Texture) - offsetof(T, m_CloudLayer2Altitude));
        memcpy(&dst->m_StaticEnvmapScale, &src->m_StaticEnvmapScale, offsetof(T, m_CustomEnvmapTexture) - offsetof(T, m_StaticEnvmapScale));
        memcpy(&dst->m_CustomEnvmapScale, &src->m_CustomEnvmapScale, offsetof(T, _0x016E) - offsetof(T, m_CustomEnvmapScale));
    }

    void fog(fb::CapturedFogComponentData* dst, const fb::CapturedFogComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedFogComponentData;

        memcpy(&dst->m_Curve, &src->m_Curve, offsetof(T, m_Realm) - offsetof(T, m_Curve));
        memcpy(&dst->m_FogDistanceMultiplier, &src->m_FogDistanceMultiplier, offsetof(T, _0x0121) - offsetof(T, m_FogDistanceMultiplier));
    }

    void wind(fb::CapturedWindComponentData* dst, const fb::CapturedWindComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedWindComponentData;

        memcpy(&dst->m_WindDirection, &src->m_WindDirection, (offsetof(T, m_TurbulenceScale) + sizeof(float)) - offsetof(T, m_WindDirection));
    }

    void sunFlare(fb::CapturedSunFlareComponentData* dst, const fb::CapturedSunFlareComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedSunFlareComponentData;

        memcpy(&dst->m_Element1SizeOccluderCurve, &src->m_Element1SizeOccluderCurve, offsetof(T, m_Realm) - offsetof(T, m_Element1SizeOccluderCurve));
        dst->m_OccluderSize = src->m_OccluderSize;
        memcpy(&dst->m_Element1RayDistance, &src->m_Element1RayDistance, offsetof(T, _0x0234) - offsetof(T, m_Element1RayDistance));
        memcpy(&dst->m_Element2RayDistance, &src->m_Element2RayDistance, offsetof(T, _0x0254) - offsetof(T, m_Element2RayDistance));
        memcpy(&dst->m_Element3RayDistance, &src->m_Element3RayDistance, offsetof(T, _0x0274) - offsetof(T, m_Element3RayDistance));
        memcpy(&dst->m_Element4RayDistance, &src->m_Element4RayDistance, offsetof(T, _0x0294) - offsetof(T, m_Element4RayDistance));
        memcpy(&dst->m_Element5RayDistance, &src->m_Element5RayDistance, (offsetof(T, m_Element5RotationAlignedToRay) + 1) - offsetof(T, m_Element5RayDistance));
    }

    void dynamicAO(fb::CapturedDynamicAOComponentData* dst, const fb::CapturedDynamicAOComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedDynamicAOComponentData;

        memcpy(&dst->m_SsaoFade, &src->m_SsaoFade, offsetof(T, _0x00AD) - offsetof(T, m_SsaoFade));
    }

    void dof(fb::CapturedDofComponentData* dst, const fb::CapturedDofComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedDofComponentData;

        memcpy(&dst->m_FocusDistance, &src->m_FocusDistance, offsetof(T, _0x00CC) - offsetof(T, m_FocusDistance));
        memcpy(&dst->m_Enable, &src->m_Enable, offsetof(T, _0x00DC) - offsetof(T, m_Enable));
    }

    void vignette(fb::CapturedVignetteComponentData* dst, const fb::CapturedVignetteComponentData* src)
    {
        if (!dst || !src)
            return;

        dst->m_Color = src->m_Color;
        dst->m_Scale = src->m_Scale;
        dst->m_Exponent = src->m_Exponent;
        dst->m_Opacity = src->m_Opacity;
        dst->m_Enable = src->m_Enable;
    }

    void filmGrain(fb::CapturedFilmGrainComponentData* dst, const fb::CapturedFilmGrainComponentData* src)
    {
        if (!dst || !src)
            return;

        dst->m_ColorScale = src->m_ColorScale;
        dst->m_TextureScale = src->m_TextureScale;
        dst->m_Enable = src->m_Enable;
        dst->m_LinearFilteringEnable = src->m_LinearFilteringEnable;
        dst->m_RandomEnable = src->m_RandomEnable;
    }

    void lensScope(fb::CapturedLensScopeComponentData* dst, const fb::CapturedLensScopeComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedLensScopeComponentData;

        memcpy(&dst->m_ChromaticAberrationColor1, &src->m_ChromaticAberrationColor1, offsetof(T, m_Realm) - offsetof(T, m_ChromaticAberrationColor1));
        memcpy(&dst->m_BlurScale, &src->m_BlurScale, offsetof(T, _0x00D1) - offsetof(T, m_BlurScale));
    }

    void cameraParams(fb::CapturedCameraParamsComponentData* dst, const fb::CapturedCameraParamsComponentData* src)
    {
        if (!dst || !src)
            return;

        dst->m_ViewDistance = src->m_ViewDistance;
        dst->m_NearPlane = src->m_NearPlane;
        dst->m_SunShadowmapViewDistance = src->m_SunShadowmapViewDistance;
    }

    void screenEffect(fb::CapturedScreenEffectComponentData* dst, const fb::CapturedScreenEffectComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedScreenEffectComponentData;

        dst->m_ScreenEffectParams = src->m_ScreenEffectParams;
        dst->m_FrameType = src->m_FrameType;
        memcpy(&dst->m_FrameWidth, &src->m_FrameWidth, (offsetof(T, m_Angle) + sizeof(float)) - offsetof(T, m_FrameWidth));
    }

    void damageEffect(fb::CapturedDamageEffectComponentData* dst, const fb::CapturedDamageEffectComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedDamageEffectComponentData;

        memcpy(&dst->m_TopDamage, &src->m_TopDamage, offsetof(T, m_Realm) - offsetof(T, m_TopDamage));
        memcpy(&dst->m_FrameWidth, &src->m_FrameWidth, offsetof(T, _0x00F1) - offsetof(T, m_FrameWidth));
    }

    void planarReflection(fb::CapturedPlanarReflectionComponentData* dst, const fb::CapturedPlanarReflectionComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedPlanarReflectionComponentData;

        memcpy(&dst->m_KeyColorReflection, &src->m_KeyColorReflection, offsetof(T, m_Realm) - offsetof(T, m_KeyColorReflection));
        memcpy(&dst->m_GroundHeight, &src->m_GroundHeight, (offsetof(T, m_OverideOutdoorLightColors) + 1) - offsetof(T, m_GroundHeight));
    }

    void dynamicEnvmap(fb::CapturedDynamicEnvmapComponentData* dst, const fb::CapturedDynamicEnvmapComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedDynamicEnvmapComponentData;

        memcpy(&dst->m_KeyColorEnvmap, &src->m_KeyColorEnvmap, offsetof(T, m_Realm) - offsetof(T, m_KeyColorEnvmap));

        dst->m_Enable = src->m_Enable;
        dst->m_TerrainReflectionsEnable = src->m_TerrainReflectionsEnable;
    }

    void characterLighting(fb::CapturedCharacterLightingComponentData* dst, const fb::CapturedCharacterLightingComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedCharacterLightingComponentData;
        memcpy(&dst->m_TopLight, &src->m_TopLight, offsetof(T, m_Realm) - offsetof(T, m_TopLight));
        memcpy(&dst->m_CameraUpRotation, &src->m_CameraUpRotation, offsetof(T, _0x00CB) - offsetof(T, m_CameraUpRotation));
    }

    void motionBlur(fb::CapturedMotionBlurComponentData* dst, const fb::CapturedMotionBlurComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedMotionBlurComponentData;
        memcpy(&dst->m_MotionBlurScale, &src->m_MotionBlurScale, offsetof(T, _0x00A3) - offsetof(T, m_MotionBlurScale));
    }

    void vehicleLighting(fb::CapturedVehicleLightingComponentData* dst, const fb::CapturedVehicleLightingComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedVehicleLightingComponentData;

        memcpy(&dst->m_TopLight, &src->m_TopLight, offsetof(T, m_Realm) - offsetof(T, m_TopLight));
        memcpy(&dst->m_CameraUpRotation, &src->m_CameraUpRotation, offsetof(T, _0x00CB) - offsetof(T, m_CameraUpRotation));
    }

    void subSurfaceScattering(fb::CapturedSubSurfaceScatteringComponentData* dst, const fb::CapturedSubSurfaceScatteringComponentData* src)
    {
        if (!dst || !src)
            return;

        using T = fb::CapturedSubSurfaceScatteringComponentData;

        memcpy(&dst->m_SimpleSssColor, &src->m_SimpleSssColor, offsetof(T, m_Realm) - offsetof(T, m_SimpleSssColor));
        memcpy(&dst->m_SimpleSssRolloffKeyLight, &src->m_SimpleSssRolloffKeyLight, offsetof(T, _0x00ED) - offsetof(T, m_SimpleSssRolloffKeyLight));
    }

    void worldRenderSettings(fb::WorldRenderSettings* dst, const fb::WorldRenderSettings* src)
    {
        if (!dst || !src)
            return;

        constexpr size_t startOffset = sizeof(fb::DataContainer); // 0x10
        constexpr size_t endOffset = sizeof(fb::WorldRenderSettings); // 0x2C0
        memcpy(reinterpret_cast<char*>(dst) + startOffset, reinterpret_cast<const char*>(src) + startOffset, endOffset - startOffset);
    }
}

#endif // BFVE_GAME_BF4
