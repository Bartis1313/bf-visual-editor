#pragma once

#include "../../SDK/fb.h"
#include <cstring>
#include <utility>

namespace editor::copy
{
    void outdoorLight(fb::CapturedOutdoorLightComponentData* dst, const fb::CapturedOutdoorLightComponentData* src);
    void enlighten(fb::CapturedEnlightenComponentData* dst, const fb::CapturedEnlightenComponentData* src);
    void tonemap(fb::CapturedTonemapComponentData* dst, const fb::CapturedTonemapComponentData* src);
    void colorCorrection(fb::CapturedColorCorrectionComponentData* dst, const fb::CapturedColorCorrectionComponentData* src);
    void sky(fb::CapturedSkyComponentData* dst, const fb::CapturedSkyComponentData* src);
    void fog(fb::CapturedFogComponentData* dst, const fb::CapturedFogComponentData* src);
    void wind(fb::CapturedWindComponentData* dst, const fb::CapturedWindComponentData* src);
    void sunFlare(fb::CapturedSunFlareComponentData* dst, const fb::CapturedSunFlareComponentData* src);
    void dynamicAO(fb::CapturedDynamicAOComponentData* dst, const fb::CapturedDynamicAOComponentData* src);
    void dof(fb::CapturedDofComponentData* dst, const fb::CapturedDofComponentData* src);
    void vignette(fb::CapturedVignetteComponentData* dst, const fb::CapturedVignetteComponentData* src);
    void filmGrain(fb::CapturedFilmGrainComponentData* dst, const fb::CapturedFilmGrainComponentData* src);
    void lensScope(fb::CapturedLensScopeComponentData* dst, const fb::CapturedLensScopeComponentData* src);
    void cameraParams(fb::CapturedCameraParamsComponentData* dst, const fb::CapturedCameraParamsComponentData* src);
    void screenEffect(fb::CapturedScreenEffectComponentData* dst, const fb::CapturedScreenEffectComponentData* src);
    void damageEffect(fb::CapturedDamageEffectComponentData* dst, const fb::CapturedDamageEffectComponentData* src);
    void planarReflection(fb::CapturedPlanarReflectionComponentData* dst, const fb::CapturedPlanarReflectionComponentData* src);
    void dynamicEnvmap(fb::CapturedDynamicEnvmapComponentData* dst, const fb::CapturedDynamicEnvmapComponentData* src);
    void characterLighting(fb::CapturedCharacterLightingComponentData* dst, const fb::CapturedCharacterLightingComponentData* src);
    void motionBlur(fb::CapturedMotionBlurComponentData* dst, const fb::CapturedMotionBlurComponentData* src);
    void worldRenderSettings(fb::WorldRenderSettings* dst, const fb::WorldRenderSettings* src);
#if defined(BFVE_GAME_BF4)
    void vehicleLighting(fb::CapturedVehicleLightingComponentData* dst, const fb::CapturedVehicleLightingComponentData* src);
    void subSurfaceScattering(fb::CapturedSubSurfaceScatteringComponentData* dst, const fb::CapturedSubSurfaceScatteringComponentData* src);
#endif

#if defined(BFVE_GAME_BF4)
    inline constexpr std::pair<size_t, size_t> MaskInfo_OutdoorLight      { 156, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Enlighten         { 172, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Tonemap           {  52, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_ColorCorrection   {  68, 1 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Sky               { 240, 8 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Fog               { 164, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Wind              {  32, 1 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_SunFlare          { 576, 8 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_DynamicAO         {  48, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Dof               {  92, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Vignette          {  40, 1 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_FilmGrain         {  44, 1 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_LensScope         {  84, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_CameraParams      {  16, 1 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_ScreenEffect      {   0, 0 }; // these are special
    inline constexpr std::pair<size_t, size_t> MaskInfo_DamageEffect      {   0, 0 }; // these are special
    inline constexpr std::pair<size_t, size_t> MaskInfo_PlanarReflection  {  80, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_DynamicEnvmap     {  56, 1 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_CharacterLighting {  76, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_VehicleLighting   {  76, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_SubSurfaceScattering { 112, 4 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_MotionBlur        {  36, 1 };
#else
    // bf3 doesnt have this
    inline constexpr std::pair<size_t, size_t> MaskInfo_OutdoorLight      { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Enlighten         { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Tonemap           { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_ColorCorrection   { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Sky               { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Fog               { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Wind              { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_SunFlare          { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_DynamicAO         { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Dof               { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_Vignette          { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_FilmGrain         { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_LensScope         { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_CameraParams      { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_ScreenEffect      { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_DamageEffect      { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_PlanarReflection  { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_DynamicEnvmap     { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_CharacterLighting { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_MotionBlur        { 0, 0 };
    inline constexpr std::pair<size_t, size_t> MaskInfo_VehicleLighting   { 0, 0 }; // not in bf3
    inline constexpr std::pair<size_t, size_t> MaskInfo_SubSurfaceScattering { 0, 0 }; // not in bf3
#endif

    template<typename T>
    inline void forceOverrides(T* slot, const std::pair<size_t, size_t>& info)
    {
        if (!slot || info.second == 0)
            return;

        memset(reinterpret_cast<char*>(slot) + info.first, 0xFF, info.second);
    }
}
