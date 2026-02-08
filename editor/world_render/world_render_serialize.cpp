#include "world_render.h"
#include "../serialize/serialize.h"

namespace editor::world_render
{
    json serialize()
    {
        json j;
        fb::WorldRenderSettings* s = getEdit();

        j["overrideEnabled"] = isOverrideEnabled();

        j["ViewportScale"] = s->m_ViewportScale;
        j["FxaaQuality"] = s->m_FxaaQuality;
        j["FxaaForceVendor"] = s->m_FxaaForceVendor;
        j["FxaaEnable"] = s->m_FxaaEnable;
        j["CullScreenAreaScale"] = s->m_CullScreenAreaScale;
        j["MultisampleCount"] = s->m_MultisampleCount;
        j["MultisampleThreshold"] = s->m_MultisampleThreshold;
        j["HdrEnable"] = s->m_HdrEnable;
        j["HalfResEnable"] = s->m_HalfResEnable;
        j["BilateralHalfResCompositeEnable"] = s->m_BilateralHalfResCompositeEnable;
        j["TiledHalfResCompositeEnable"] = s->m_TiledHalfResCompositeEnable;
        j["TiledHalfResStencilOccludersEnable"] = s->m_TiledHalfResStencilOccludersEnable;
        j["OutputGammaCorrectionEnable"] = s->m_OutputGammaCorrectionEnable;
        j["Enable"] = s->m_Enable;

        j["ShadowmapsEnable"] = s->m_ShadowmapsEnable;
        j["GenerateShadowmapsEnable"] = s->m_GenerateShadowmapsEnable;
        j["ApplyShadowmapsEnable"] = s->m_ApplyShadowmapsEnable;
        j["SimpleShadowmapsEnable"] = s->m_SimpleShadowmapsEnable;
        j["TransparencyShadowmapsEnable"] = s->m_TransparencyShadowmapsEnable;
        j["CloudShadowEnable"] = s->m_CloudShadowEnable;
        j["ShadowmapResolution"] = s->m_ShadowmapResolution;
        j["ShadowmapQuality"] = s->m_ShadowmapQuality;
        j["ShadowmapSliceCount"] = s->m_ShadowmapSliceCount;
        j["ShadowmapViewDistance"] = s->m_ShadowmapViewDistance;
        j["ShadowmapViewDistanceScaleEnable"] = s->m_ShadowmapViewDistanceScaleEnable;
        j["ShadowMinScreenArea"] = s->m_ShadowMinScreenArea;
        j["ShadowmapMinScreenArea"] = s->m_ShadowmapMinScreenArea;
        j["ShadowViewportScale"] = s->m_ShadowViewportScale;
        j["ShadowmapSizeZScale"] = s->m_ShadowmapSizeZScale;
        j["ShadowmapSliceSchemeWeight"] = s->m_ShadowmapSliceSchemeWeight;
        j["ShadowmapFirstSliceScale"] = s->m_ShadowmapFirstSliceScale;
        j["ShadowmapExtrusionLength"] = s->m_ShadowmapExtrusionLength;
        j["ShadowmapMinFov"] = s->m_ShadowmapMinFov;
        j["ShadowmapPoissonFilterScale"] = s->m_ShadowmapPoissonFilterScale;
        j["ShadowmapFixedDepthEnable"] = s->m_ShadowmapFixedDepthEnable;
        j["ShadowmapFixedMovementEnable"] = s->m_ShadowmapFixedMovementEnable;
        j["ShadowmapCullVolumeEnable"] = s->m_ShadowmapCullVolumeEnable;
        j["ShadowmapAccumEnable"] = s->m_ShadowmapAccumEnable;
        j["ShadowmapAccumBilinearEnable"] = s->m_ShadowmapAccumBilinearEnable;
        j["ColoredShadowmapSlicesEnable"] = s->m_ColoredShadowmapSlicesEnable;
        j["ZBufferShadowTestEnable"] = s->m_ZBufferShadowTestEnable;
        j["DxShadowmap16BitEnable"] = s->m_DxShadowmap16BitEnable;
        j["OnlyShadowmapSlice"] = s->m_OnlyShadowmapSlice;

        j["MotionBlurEnable"] = s->m_MotionBlurEnable;
        j["MotionBlurGeometryPassEnable"] = s->m_MotionBlurGeometryPassEnable;
        j["MotionBlurStencilPassEnable"] = s->m_MotionBlurStencilPassEnable;
        j["MotionBlurScale"] = s->m_MotionBlurScale;
        j["MotionBlurMax"] = s->m_MotionBlurMax;
        j["MotionBlurNoiseScale"] = s->m_MotionBlurNoiseScale;
        j["MotionBlurQuality"] = s->m_MotionBlurQuality;
        j["MotionBlurMaxSampleCount"] = s->m_MotionBlurMaxSampleCount;
        j["MotionBlurFrameAverageCount"] = s->m_MotionBlurFrameAverageCount;
        j["MotionBlurMaxFrameTime"] = s->m_MotionBlurMaxFrameTime;
        j["ForceMotionBlurDepthCutoff"] = s->m_ForceMotionBlurDepthCutoff;
        j["ForceMotionBlurCutoffGradientScale"] = s->m_ForceMotionBlurCutoffGradientScale;

        j["OutdoorLightEnable"] = s->m_OutdoorLightEnable;
        j["OutdoorKeyLightEnable"] = s->m_OutdoorKeyLightEnable;
        j["OutdoorSkyLightEnable"] = s->m_OutdoorSkyLightEnable;
        j["OutdoorLightSpecularEnable"] = s->m_OutdoorLightSpecularEnable;
        j["OutdoorLightTilingEnable"] = s->m_OutdoorLightTilingEnable;
        j["OutdoorLightTileRenderEnable"] = s->m_OutdoorLightTileRenderEnable;
        j["OutdoorLightTileBlendEnable"] = s->m_OutdoorLightTileBlendEnable;
        j["OutdoorLightTileSimpleShaderEnable"] = s->m_OutdoorLightTileSimpleShaderEnable;
        j["OutdoorLightTileBatchCount"] = s->m_OutdoorLightTileBatchCount;
        j["LightIntensityScale"] = s->m_LightIntensityScale;
        j["LightForceIntensity"] = s->m_LightForceIntensity;
        j["LightRadiusScale"] = s->m_LightRadiusScale;
        j["LightAttenuationThreshold"] = s->m_LightAttenuationThreshold;
        j["LightAttenuationThresholdEnable"] = s->m_LightAttenuationThresholdEnable;
        j["LightWidthEnable"] = s->m_LightWidthEnable;
        j["LightIntensityNormalizationEnable"] = s->m_LightIntensityNormalizationEnable;
        j["SpecularLightingEnable"] = s->m_SpecularLightingEnable;
        j["SkinLightingEnable"] = s->m_SkinLightingEnable;
        j["TranslucencyLightingEnable"] = s->m_TranslucencyLightingEnable;
        j["EmissiveEnable"] = s->m_EmissiveEnable;
        j["UnlitEnable"] = s->m_UnlitEnable;
        j["DrawLightSources"] = s->m_DrawLightSources;

        j["LightLodSpecularFadeAreaStart"] = s->m_LightLodSpecularFadeAreaStart;
        j["LightLodSpecularFadeAreaEnd"] = s->m_LightLodSpecularFadeAreaEnd;
        j["LightLodRadiusFactor"] = s->m_LightLodRadiusFactor;
        j["LightLodFadeArea"] = s->m_LightLodFadeArea;
        j["LightLodMinArea"] = s->m_LightLodMinArea;

        j["LightCullEnable"] = s->m_LightCullEnable;
        j["LightDepthCullEnable"] = s->m_LightDepthCullEnable;
        j["LightNormalCullEnable"] = s->m_LightNormalCullEnable;
        j["LightConeCullEnable"] = s->m_LightConeCullEnable;
        j["LightStencilMinArea"] = s->m_LightStencilMinArea;
        j["LightStencilMethodEnable"] = s->m_LightStencilMethodEnable;
        j["LightVolumeMethodEnable"] = s->m_LightVolumeMethodEnable;
        j["LightVolumeDepthTestEnable"] = s->m_LightVolumeDepthTestEnable;
        j["LightTileMinArea"] = s->m_LightTileMinArea;
        j["LightTileOverlayEnable"] = s->m_LightTileOverlayEnable;
        j["LightOverdrawMaxLayerCount"] = s->m_LightOverdrawMaxLayerCount;

        j["PointLightsEnable"] = s->m_PointLightsEnable;
        j["MaxPointLightCount"] = s->m_MaxPointLightCount;
        j["SpotLightsEnable"] = s->m_SpotLightsEnable;
        j["MaxSpotLightCount"] = s->m_MaxSpotLightCount;
        j["MaxSpotLightShadowCount"] = s->m_MaxSpotLightShadowCount;
        j["SpotLightShadowmapResolution"] = s->m_SpotLightShadowmapResolution;
        j["SpotLightShadowmapQuality"] = s->m_SpotLightShadowmapQuality;
        j["SpotLightNearPlane"] = s->m_SpotLightNearPlane;
        j["SpotLightShadowmapPoissonFilterScale"] = s->m_SpotLightShadowmapPoissonFilterScale;
        j["SpotLightShadowmapEnable"] = s->m_SpotLightShadowmapEnable;
        j["DxSpotLightShadowmap16BitEnable"] = s->m_DxSpotLightShadowmap16BitEnable;
        j["SpotLightShadomapLevel"] = static_cast<int>(s->m_SpotLightShadomapLevel);
        j["LineLightsEnable"] = s->m_LineLightsEnable;
        j["ConeLightsEnable"] = s->m_ConeLightsEnable;
        j["LightProbesEnable"] = s->m_LightProbesEnable;

        j["SkyEnable"] = s->m_SkyEnable;
        j["SkyFogEnable"] = s->m_SkyFogEnable;
        j["SkyEnvmapEnable"] = s->m_SkyEnvmapEnable;
        j["SkyEnvmapUpdateEnable"] = s->m_SkyEnvmapUpdateEnable;
        j["SkyEnvmapForceUpdateEnable"] = s->m_SkyEnvmapForceUpdateEnable;
        j["SkyEnvmapMipmapGenEnable"] = s->m_SkyEnvmapMipmapGenEnable;
        j["SkyEnvmapUse8BitTexture"] = s->m_SkyEnvmapUse8BitTexture;
        j["SkyEnvmapResolution"] = s->m_SkyEnvmapResolution;
        j["SkyEnvmapSidesPerFrameCount"] = s->m_SkyEnvmapSidesPerFrameCount;
        j["SkyEnvmapFilterWidth"] = s->m_SkyEnvmapFilterWidth;
        j["SkyEnvmapFilterMode"] = static_cast<int>(s->m_SkyEnvmapFilterMode);
        j["SkyVisibilityEnvmapScalingEnable"] = s->m_SkyVisibilityEnvmapScalingEnable;
        j["DrawDebugSkyEnvmapMipLevel"] = s->m_DrawDebugSkyEnvmapMipLevel;

        j["DynamicEnvmapEnable"] = s->m_DynamicEnvmapEnable;
        j["DynamicEnvmapLightingEnable"] = s->m_DynamicEnvmapLightingEnable;
        j["DynamicEnvmapMipmapGenEnable"] = s->m_DynamicEnvmapMipmapGenEnable;
        j["DynamicEnvmapResolution"] = s->m_DynamicEnvmapResolution;
        j["DynamicEnvmapFilterWidth"] = s->m_DynamicEnvmapFilterWidth;
        j["DynamicEnvmapFilterMode"] = static_cast<int>(s->m_DynamicEnvmapFilterMode);
        j["DynamicEnvmapDefaultPosition"] = serialize::vec3ToJson(s->m_DynamicEnvmapDefaultPosition);
        j["DrawDebugDynamicEnvmapMipLevel"] = s->m_DrawDebugDynamicEnvmapMipLevel;
        j["StaticEnvmapEnable"] = s->m_StaticEnvmapEnable;
        j["CustomEnvmapEnable"] = s->m_CustomEnvmapEnable;

        j["PlanarReflectionEnable"] = s->m_PlanarReflectionEnable;
        j["PlanarReflectionBlur"] = s->m_PlanarReflectionBlur;
        j["PlanarReflectionWidth"] = s->m_PlanarReflectionWidth;
        j["PlanarReflectionHeight"] = s->m_PlanarReflectionHeight;
        j["PlanarReflectionCullFOV"] = s->m_PlanarReflectionCullFOV;
        j["TempPlanarReflectionY"] = s->m_TempPlanarReflectionY;
        j["PlanarReflectionDebugCullMode"] = s->m_PlanarReflectionDebugCullMode;
        j["FreezePlanarReflectionCullFrustum"] = s->m_FreezePlanarReflectionCullFrustum;
        j["ReflectionEnvmapSize"] = s->m_ReflectionEnvmapSize;

        j["SubSurfaceColor"] = serialize::vec3ToJson(s->m_SubSurfaceColor);
        j["SubSurfaceRolloffKeyLight"] = s->m_SubSurfaceRolloffKeyLight;
        j["SubSurfaceRolloffLocalLight"] = s->m_SubSurfaceRolloffLocalLight;

        j["DecalVolumeEnable"] = s->m_DecalVolumeEnable;
        j["MaxDecalVolumeCount"] = s->m_MaxDecalVolumeCount;
        j["DecalVolumeScale"] = s->m_DecalVolumeScale;
        j["MaxDestructionVolumeCount"] = s->m_MaxDestructionVolumeCount;

        j["ZPassEnable"] = s->m_ZPassEnable;
        j["MainOpaqueZPassEnable"] = s->m_MainOpaqueZPassEnable;
        j["OccluderMeshZPrepassEnable"] = s->m_OccluderMeshZPrepassEnable;
        j["OccluderMeshZPrepassDebugEnable"] = s->m_OccluderMeshZPrepassDebugEnable;
        j["ForegroundEnable"] = s->m_ForegroundEnable;
        j["ForegroundZPassEnable"] = s->m_ForegroundZPassEnable;
        j["ForegroundDepthClearEnable"] = s->m_ForegroundDepthClearEnable;
        j["ForegroundAsMainEnable"] = s->m_ForegroundAsMainEnable;
        j["ForegroundTransparentEnable"] = s->m_ForegroundTransparentEnable;
        j["DrawTransparent"] = s->m_DrawTransparent;
        j["DrawTransparentDecal"] = s->m_DrawTransparentDecal;
        j["ScreenEffectEnable"] = s->m_ScreenEffectEnable;
        j["FinalPostEnable"] = s->m_FinalPostEnable;
        j["ViewFxEnable"] = s->m_ViewFxEnable;

        j["LensFlaresEnable"] = s->m_LensFlaresEnable;
        j["HalfResLensFlaresEnable"] = s->m_HalfResLensFlaresEnable;
        j["LensFlareOcclusionEnable"] = s->m_LensFlareOcclusionEnable;

        j["GBufferClearEnable"] = s->m_GBufferClearEnable;
        j["GBufferAlphaTestSimpleEnable"] = s->m_GBufferAlphaTestSimpleEnable;
        j["DxGBufferLight16BitEnable"] = s->m_DxGBufferLight16BitEnable;
        j["DxGBufferNormal16BitEnable"] = s->m_DxGBufferNormal16BitEnable;
        j["DxLinearDepth32BitFormatEnable"] = s->m_DxLinearDepth32BitFormatEnable;
        j["DxDeferredCsPathEnable"] = s->m_DxDeferredCsPathEnable;
        j["GBufferTestCount"] = s->m_GBufferTestCount;
        j["GBufferLayout"] = static_cast<int>(s->m_GBufferLayout);

        j["OverrideDynamicAO"] = s->m_OverrideDynamicAO;

        j["WireframeEnable"] = s->m_WireframeEnable;
        j["OpaqueSortBySolutionEnable"] = s->m_OpaqueSortBySolutionEnable;
        j["GenericEntityRendererEnable"] = s->m_GenericEntityRendererEnable;
        j["GenericEntityMaxVisibleEntityCount"] = s->m_GenericEntityMaxVisibleEntityCount;
        j["SetupJobEnable"] = s->m_SetupJobEnable;
        j["DynamicCullZBufferTestEnable"] = s->m_DynamicCullZBufferTestEnable;
        j["DynamicCullDrawOccludedBoxesEnable"] = s->m_DynamicCullDrawOccludedBoxesEnable;
        j["ViewMode"] = static_cast<int>(s->m_ViewMode);
        j["OnlyTileIndex"] = s->m_OnlyTileIndex;
        j["OnlyLightTileX"] = s->m_OnlyLightTileX;
        j["OnlyLightTileY"] = s->m_OnlyLightTileY;
        j["OnlyLightTileIndex"] = s->m_OnlyLightTileIndex;

        bool hasDebug = s->m_DrawDebugGBuffer || s->m_DrawDebugZBufferEnable || s->m_DrawDebugShadowmaps;
        if (hasDebug)
        {
            j["DrawDebugGBuffer"] = s->m_DrawDebugGBuffer;
            j["DrawDebugZBufferEnable"] = s->m_DrawDebugZBufferEnable;
            j["DrawDebugVelocityBuffer"] = s->m_DrawDebugVelocityBuffer;
            j["DrawDebugMultisampleClassify"] = s->m_DrawDebugMultisampleClassify;
            j["DrawDebugHalfResEnvironment"] = s->m_DrawDebugHalfResEnvironment;
            j["DrawDebugShadowmaps"] = s->m_DrawDebugShadowmaps;
            j["DrawDebugQuarterShadowmaps"] = s->m_DrawDebugQuarterShadowmaps;
            j["DrawDebugTransShadowmap"] = s->m_DrawDebugTransShadowmap;
            j["DrawDebugSpotLightShadowmaps"] = s->m_DrawDebugSpotLightShadowmaps;
            j["DrawShadowFrustums"] = s->m_DrawShadowFrustums;
            j["DrawDebugLightSources"] = s->m_DrawDebugLightSources;
            j["DrawDebugLightShadowSources"] = s->m_DrawDebugLightShadowSources;
            j["DrawDebugLightStats"] = s->m_DrawDebugLightStats;
            j["DrawDebugLightTiles"] = s->m_DrawDebugLightTiles;
            j["DrawDebugLightTileVolumes"] = s->m_DrawDebugLightTileVolumes;
            j["DrawDebugLightTileFbGrid"] = s->m_DrawDebugLightTileFbGrid;
            j["DrawDebugLightTileGrid"] = s->m_DrawDebugLightTileGrid;
            j["DrawDebugLightTileSources"] = s->m_DrawDebugLightTileSources;
            j["DrawDebugDynamicAO"] = s->m_DrawDebugDynamicAO;
            j["DrawDebugDynamicEnvmap"] = s->m_DrawDebugDynamicEnvmap;
            j["DrawDebugSkyEnvmap"] = s->m_DrawDebugSkyEnvmap;
            j["DrawDebugPlanarReflection"] = s->m_DrawDebugPlanarReflection;
            j["DrawDebugPlanarReflectionCullFrustum"] = s->m_DrawDebugPlanarReflectionCullFrustum;
            j["DrawDebugDecalVolumes"] = s->m_DrawDebugDecalVolumes;
            j["DrawDebugDestructionVolumes"] = s->m_DrawDebugDestructionVolumes;
            j["DrawDebugLensFlares"] = s->m_DrawDebugLensFlares;
            j["DrawDebugLensFlareOccluders"] = s->m_DrawDebugLensFlareOccluders;
            j["DrawDebugWorldOcclusions"] = s->m_DrawDebugWorldOcclusions;
            j["DrawDebugVisibleEntityTypes"] = s->m_DrawDebugVisibleEntityTypes;
            j["DrawBoundingSpheres"] = s->m_DrawBoundingSpheres;
            j["DrawSolidBoundingBoxes"] = s->m_DrawSolidBoundingBoxes;
            j["DrawLineBoundingBoxes"] = s->m_DrawLineBoundingBoxes;
            j["DrawFrustums"] = s->m_DrawFrustums;
        }

        return j;

    }

    void deserialize(const json& j)
    {
        fb::WorldRenderSettings* e = getEdit();

        setOverrideEnabled(j.value("overrideEnabled", isOverrideEnabled()));

        JSON_GET(j, "ViewportScale", e->m_ViewportScale);
        JSON_GET(j, "FxaaQuality", e->m_FxaaQuality);
        JSON_GET(j, "FxaaForceVendor", e->m_FxaaForceVendor);
        JSON_GET_BOOL(j, "FxaaEnable", e->m_FxaaEnable);
        JSON_GET(j, "CullScreenAreaScale", e->m_CullScreenAreaScale);
        JSON_GET(j, "MultisampleCount", e->m_MultisampleCount);
        JSON_GET(j, "MultisampleThreshold", e->m_MultisampleThreshold);
        JSON_GET_BOOL(j, "HdrEnable", e->m_HdrEnable);
        JSON_GET_BOOL(j, "HalfResEnable", e->m_HalfResEnable);
        JSON_GET_BOOL(j, "BilateralHalfResCompositeEnable", e->m_BilateralHalfResCompositeEnable);
        JSON_GET_BOOL(j, "TiledHalfResCompositeEnable", e->m_TiledHalfResCompositeEnable);
        JSON_GET_BOOL(j, "TiledHalfResStencilOccludersEnable", e->m_TiledHalfResStencilOccludersEnable);
        JSON_GET_BOOL(j, "OutputGammaCorrectionEnable", e->m_OutputGammaCorrectionEnable);
        JSON_GET_BOOL(j, "Enable", e->m_Enable);

        JSON_GET_BOOL(j, "ShadowmapsEnable", e->m_ShadowmapsEnable);
        JSON_GET_BOOL(j, "GenerateShadowmapsEnable", e->m_GenerateShadowmapsEnable);
        JSON_GET_BOOL(j, "ApplyShadowmapsEnable", e->m_ApplyShadowmapsEnable);
        JSON_GET_BOOL(j, "SimpleShadowmapsEnable", e->m_SimpleShadowmapsEnable);
        JSON_GET_BOOL(j, "TransparencyShadowmapsEnable", e->m_TransparencyShadowmapsEnable);
        JSON_GET_BOOL(j, "CloudShadowEnable", e->m_CloudShadowEnable);
        JSON_GET(j, "ShadowmapResolution", e->m_ShadowmapResolution);
        JSON_GET(j, "ShadowmapQuality", e->m_ShadowmapQuality);
        JSON_GET(j, "ShadowmapSliceCount", e->m_ShadowmapSliceCount);
        JSON_GET(j, "ShadowmapViewDistance", e->m_ShadowmapViewDistance);
        JSON_GET_BOOL(j, "ShadowmapViewDistanceScaleEnable", e->m_ShadowmapViewDistanceScaleEnable);
        JSON_GET(j, "ShadowMinScreenArea", e->m_ShadowMinScreenArea);
        JSON_GET(j, "ShadowmapMinScreenArea", e->m_ShadowmapMinScreenArea);
        JSON_GET(j, "ShadowViewportScale", e->m_ShadowViewportScale);
        JSON_GET(j, "ShadowmapSizeZScale", e->m_ShadowmapSizeZScale);
        JSON_GET(j, "ShadowmapSliceSchemeWeight", e->m_ShadowmapSliceSchemeWeight);
        JSON_GET(j, "ShadowmapFirstSliceScale", e->m_ShadowmapFirstSliceScale);
        JSON_GET(j, "ShadowmapExtrusionLength", e->m_ShadowmapExtrusionLength);
        JSON_GET(j, "ShadowmapMinFov", e->m_ShadowmapMinFov);
        JSON_GET(j, "ShadowmapPoissonFilterScale", e->m_ShadowmapPoissonFilterScale);
        JSON_GET_BOOL(j, "ShadowmapFixedDepthEnable", e->m_ShadowmapFixedDepthEnable);
        JSON_GET_BOOL(j, "ShadowmapFixedMovementEnable", e->m_ShadowmapFixedMovementEnable);
        JSON_GET_BOOL(j, "ShadowmapCullVolumeEnable", e->m_ShadowmapCullVolumeEnable);
        JSON_GET_BOOL(j, "ShadowmapAccumEnable", e->m_ShadowmapAccumEnable);
        JSON_GET_BOOL(j, "ShadowmapAccumBilinearEnable", e->m_ShadowmapAccumBilinearEnable);
        JSON_GET_BOOL(j, "ColoredShadowmapSlicesEnable", e->m_ColoredShadowmapSlicesEnable);
        JSON_GET_BOOL(j, "ZBufferShadowTestEnable", e->m_ZBufferShadowTestEnable);
        JSON_GET_BOOL(j, "DxShadowmap16BitEnable", e->m_DxShadowmap16BitEnable);
        JSON_GET(j, "OnlyShadowmapSlice", e->m_OnlyShadowmapSlice);

        JSON_GET_BOOL(j, "MotionBlurEnable", e->m_MotionBlurEnable);
        JSON_GET_BOOL(j, "MotionBlurGeometryPassEnable", e->m_MotionBlurGeometryPassEnable);
        JSON_GET_BOOL(j, "MotionBlurStencilPassEnable", e->m_MotionBlurStencilPassEnable);
        JSON_GET(j, "MotionBlurScale", e->m_MotionBlurScale);
        JSON_GET(j, "MotionBlurMax", e->m_MotionBlurMax);
        JSON_GET(j, "MotionBlurNoiseScale", e->m_MotionBlurNoiseScale);
        JSON_GET(j, "MotionBlurQuality", e->m_MotionBlurQuality);
        JSON_GET(j, "MotionBlurMaxSampleCount", e->m_MotionBlurMaxSampleCount);
        JSON_GET(j, "MotionBlurFrameAverageCount", e->m_MotionBlurFrameAverageCount);
        JSON_GET(j, "MotionBlurMaxFrameTime", e->m_MotionBlurMaxFrameTime);
        JSON_GET(j, "ForceMotionBlurDepthCutoff", e->m_ForceMotionBlurDepthCutoff);
        JSON_GET(j, "ForceMotionBlurCutoffGradientScale", e->m_ForceMotionBlurCutoffGradientScale);

        JSON_GET_BOOL(j, "OutdoorLightEnable", e->m_OutdoorLightEnable);
        JSON_GET_BOOL(j, "OutdoorKeyLightEnable", e->m_OutdoorKeyLightEnable);
        JSON_GET_BOOL(j, "OutdoorSkyLightEnable", e->m_OutdoorSkyLightEnable);
        JSON_GET_BOOL(j, "OutdoorLightSpecularEnable", e->m_OutdoorLightSpecularEnable);
        JSON_GET_BOOL(j, "OutdoorLightTilingEnable", e->m_OutdoorLightTilingEnable);
        JSON_GET_BOOL(j, "OutdoorLightTileRenderEnable", e->m_OutdoorLightTileRenderEnable);
        JSON_GET_BOOL(j, "OutdoorLightTileBlendEnable", e->m_OutdoorLightTileBlendEnable);
        JSON_GET_BOOL(j, "OutdoorLightTileSimpleShaderEnable", e->m_OutdoorLightTileSimpleShaderEnable);
        JSON_GET(j, "OutdoorLightTileBatchCount", e->m_OutdoorLightTileBatchCount);
        JSON_GET(j, "LightIntensityScale", e->m_LightIntensityScale);
        JSON_GET(j, "LightForceIntensity", e->m_LightForceIntensity);
        JSON_GET(j, "LightRadiusScale", e->m_LightRadiusScale);
        JSON_GET(j, "LightAttenuationThreshold", e->m_LightAttenuationThreshold);
        JSON_GET_BOOL(j, "LightAttenuationThresholdEnable", e->m_LightAttenuationThresholdEnable);
        JSON_GET_BOOL(j, "LightWidthEnable", e->m_LightWidthEnable);
        JSON_GET_BOOL(j, "LightIntensityNormalizationEnable", e->m_LightIntensityNormalizationEnable);
        JSON_GET_BOOL(j, "SpecularLightingEnable", e->m_SpecularLightingEnable);
        JSON_GET_BOOL(j, "SkinLightingEnable", e->m_SkinLightingEnable);
        JSON_GET_BOOL(j, "TranslucencyLightingEnable", e->m_TranslucencyLightingEnable);
        JSON_GET_BOOL(j, "EmissiveEnable", e->m_EmissiveEnable);
        JSON_GET_BOOL(j, "UnlitEnable", e->m_UnlitEnable);
        JSON_GET_BOOL(j, "DrawLightSources", e->m_DrawLightSources);

        JSON_GET(j, "LightLodSpecularFadeAreaStart", e->m_LightLodSpecularFadeAreaStart);
        JSON_GET(j, "LightLodSpecularFadeAreaEnd", e->m_LightLodSpecularFadeAreaEnd);
        JSON_GET(j, "LightLodRadiusFactor", e->m_LightLodRadiusFactor);
        JSON_GET(j, "LightLodFadeArea", e->m_LightLodFadeArea);
        JSON_GET(j, "LightLodMinArea", e->m_LightLodMinArea);

        JSON_GET_BOOL(j, "LightCullEnable", e->m_LightCullEnable);
        JSON_GET_BOOL(j, "LightDepthCullEnable", e->m_LightDepthCullEnable);
        JSON_GET_BOOL(j, "LightNormalCullEnable", e->m_LightNormalCullEnable);
        JSON_GET_BOOL(j, "LightConeCullEnable", e->m_LightConeCullEnable);
        JSON_GET(j, "LightStencilMinArea", e->m_LightStencilMinArea);
        JSON_GET_BOOL(j, "LightStencilMethodEnable", e->m_LightStencilMethodEnable);
        JSON_GET_BOOL(j, "LightVolumeMethodEnable", e->m_LightVolumeMethodEnable);
        JSON_GET_BOOL(j, "LightVolumeDepthTestEnable", e->m_LightVolumeDepthTestEnable);
        JSON_GET(j, "LightTileMinArea", e->m_LightTileMinArea);
        JSON_GET_BOOL(j, "LightTileOverlayEnable", e->m_LightTileOverlayEnable);
        JSON_GET(j, "LightOverdrawMaxLayerCount", e->m_LightOverdrawMaxLayerCount);

        JSON_GET_BOOL(j, "PointLightsEnable", e->m_PointLightsEnable);
        JSON_GET(j, "MaxPointLightCount", e->m_MaxPointLightCount);
        JSON_GET_BOOL(j, "SpotLightsEnable", e->m_SpotLightsEnable);
        JSON_GET(j, "MaxSpotLightCount", e->m_MaxSpotLightCount);
        JSON_GET(j, "MaxSpotLightShadowCount", e->m_MaxSpotLightShadowCount);
        JSON_GET(j, "SpotLightShadowmapResolution", e->m_SpotLightShadowmapResolution);
        JSON_GET(j, "SpotLightShadowmapQuality", e->m_SpotLightShadowmapQuality);
        JSON_GET(j, "SpotLightNearPlane", e->m_SpotLightNearPlane);
        JSON_GET(j, "SpotLightShadowmapPoissonFilterScale", e->m_SpotLightShadowmapPoissonFilterScale);
        JSON_GET_BOOL(j, "SpotLightShadowmapEnable", e->m_SpotLightShadowmapEnable);
        JSON_GET_BOOL(j, "DxSpotLightShadowmap16BitEnable", e->m_DxSpotLightShadowmap16BitEnable);
        JSON_GET_ENUM(j, "SpotLightShadomapLevel", e->m_SpotLightShadomapLevel, fb::QualityLevel);
        JSON_GET_BOOL(j, "LineLightsEnable", e->m_LineLightsEnable);
        JSON_GET_BOOL(j, "ConeLightsEnable", e->m_ConeLightsEnable);
        JSON_GET_BOOL(j, "LightProbesEnable", e->m_LightProbesEnable);

        JSON_GET_BOOL(j, "SkyEnable", e->m_SkyEnable);
        JSON_GET_BOOL(j, "SkyFogEnable", e->m_SkyFogEnable);
        JSON_GET_BOOL(j, "SkyEnvmapEnable", e->m_SkyEnvmapEnable);
        JSON_GET_BOOL(j, "SkyEnvmapUpdateEnable", e->m_SkyEnvmapUpdateEnable);
        JSON_GET_BOOL(j, "SkyEnvmapForceUpdateEnable", e->m_SkyEnvmapForceUpdateEnable);
        JSON_GET_BOOL(j, "SkyEnvmapMipmapGenEnable", e->m_SkyEnvmapMipmapGenEnable);
        JSON_GET_BOOL(j, "SkyEnvmapUse8BitTexture", e->m_SkyEnvmapUse8BitTexture);
        JSON_GET(j, "SkyEnvmapResolution", e->m_SkyEnvmapResolution);
        JSON_GET(j, "SkyEnvmapSidesPerFrameCount", e->m_SkyEnvmapSidesPerFrameCount);
        JSON_GET(j, "SkyEnvmapFilterWidth", e->m_SkyEnvmapFilterWidth);
        JSON_GET_ENUM(j, "SkyEnvmapFilterMode", e->m_SkyEnvmapFilterMode, fb::MipmapFilterMode);
        JSON_GET_BOOL(j, "SkyVisibilityEnvmapScalingEnable", e->m_SkyVisibilityEnvmapScalingEnable);
        JSON_GET(j, "DrawDebugSkyEnvmapMipLevel", e->m_DrawDebugSkyEnvmapMipLevel);

        JSON_GET_BOOL(j, "DynamicEnvmapEnable", e->m_DynamicEnvmapEnable);
        JSON_GET_BOOL(j, "DynamicEnvmapLightingEnable", e->m_DynamicEnvmapLightingEnable);
        JSON_GET_BOOL(j, "DynamicEnvmapMipmapGenEnable", e->m_DynamicEnvmapMipmapGenEnable);
        JSON_GET(j, "DynamicEnvmapResolution", e->m_DynamicEnvmapResolution);
        JSON_GET(j, "DynamicEnvmapFilterWidth", e->m_DynamicEnvmapFilterWidth);
        JSON_GET_ENUM(j, "DynamicEnvmapFilterMode", e->m_DynamicEnvmapFilterMode, fb::MipmapFilterMode);
        JSON_GET_VEC3(j, "DynamicEnvmapDefaultPosition", e->m_DynamicEnvmapDefaultPosition);
        JSON_GET(j, "DrawDebugDynamicEnvmapMipLevel", e->m_DrawDebugDynamicEnvmapMipLevel);
        JSON_GET_BOOL(j, "StaticEnvmapEnable", e->m_StaticEnvmapEnable);
        JSON_GET_BOOL(j, "CustomEnvmapEnable", e->m_CustomEnvmapEnable);

        JSON_GET_BOOL(j, "PlanarReflectionEnable", e->m_PlanarReflectionEnable);
        JSON_GET_BOOL(j, "PlanarReflectionBlur", e->m_PlanarReflectionBlur);
        JSON_GET(j, "PlanarReflectionWidth", e->m_PlanarReflectionWidth);
        JSON_GET(j, "PlanarReflectionHeight", e->m_PlanarReflectionHeight);
        JSON_GET(j, "PlanarReflectionCullFOV", e->m_PlanarReflectionCullFOV);
        JSON_GET(j, "TempPlanarReflectionY", e->m_TempPlanarReflectionY);
        JSON_GET_BOOL(j, "PlanarReflectionDebugCullMode", e->m_PlanarReflectionDebugCullMode);
        JSON_GET_BOOL(j, "FreezePlanarReflectionCullFrustum", e->m_FreezePlanarReflectionCullFrustum);
        JSON_GET(j, "ReflectionEnvmapSize", e->m_ReflectionEnvmapSize);

        JSON_GET_VEC3(j, "SubSurfaceColor", e->m_SubSurfaceColor);
        JSON_GET(j, "SubSurfaceRolloffKeyLight", e->m_SubSurfaceRolloffKeyLight);
        JSON_GET(j, "SubSurfaceRolloffLocalLight", e->m_SubSurfaceRolloffLocalLight);

        JSON_GET_BOOL(j, "DecalVolumeEnable", e->m_DecalVolumeEnable);
        JSON_GET(j, "MaxDecalVolumeCount", e->m_MaxDecalVolumeCount);
        JSON_GET(j, "DecalVolumeScale", e->m_DecalVolumeScale);
        JSON_GET(j, "MaxDestructionVolumeCount", e->m_MaxDestructionVolumeCount);

        JSON_GET_BOOL(j, "ZPassEnable", e->m_ZPassEnable);
        JSON_GET_BOOL(j, "MainOpaqueZPassEnable", e->m_MainOpaqueZPassEnable);
        JSON_GET_BOOL(j, "OccluderMeshZPrepassEnable", e->m_OccluderMeshZPrepassEnable);
        JSON_GET_BOOL(j, "OccluderMeshZPrepassDebugEnable", e->m_OccluderMeshZPrepassDebugEnable);
        JSON_GET_BOOL(j, "ForegroundEnable", e->m_ForegroundEnable);
        JSON_GET_BOOL(j, "ForegroundZPassEnable", e->m_ForegroundZPassEnable);
        JSON_GET_BOOL(j, "ForegroundDepthClearEnable", e->m_ForegroundDepthClearEnable);
        JSON_GET_BOOL(j, "ForegroundAsMainEnable", e->m_ForegroundAsMainEnable);
        JSON_GET_BOOL(j, "ForegroundTransparentEnable", e->m_ForegroundTransparentEnable);
        JSON_GET_BOOL(j, "DrawTransparent", e->m_DrawTransparent);
        JSON_GET_BOOL(j, "DrawTransparentDecal", e->m_DrawTransparentDecal);
        JSON_GET_BOOL(j, "ScreenEffectEnable", e->m_ScreenEffectEnable);
        JSON_GET_BOOL(j, "FinalPostEnable", e->m_FinalPostEnable);
        JSON_GET_BOOL(j, "ViewFxEnable", e->m_ViewFxEnable);

        JSON_GET_BOOL(j, "LensFlaresEnable", e->m_LensFlaresEnable);
        JSON_GET_BOOL(j, "HalfResLensFlaresEnable", e->m_HalfResLensFlaresEnable);
        JSON_GET_BOOL(j, "LensFlareOcclusionEnable", e->m_LensFlareOcclusionEnable);

        JSON_GET_BOOL(j, "GBufferClearEnable", e->m_GBufferClearEnable);
        JSON_GET_BOOL(j, "GBufferAlphaTestSimpleEnable", e->m_GBufferAlphaTestSimpleEnable);
        JSON_GET_BOOL(j, "DxGBufferLight16BitEnable", e->m_DxGBufferLight16BitEnable);
        JSON_GET_BOOL(j, "DxGBufferNormal16BitEnable", e->m_DxGBufferNormal16BitEnable);
        JSON_GET_BOOL(j, "DxLinearDepth32BitFormatEnable", e->m_DxLinearDepth32BitFormatEnable);
        JSON_GET_BOOL(j, "DxDeferredCsPathEnable", e->m_DxDeferredCsPathEnable);
        JSON_GET(j, "GBufferTestCount", e->m_GBufferTestCount);
        JSON_GET_ENUM(j, "GBufferLayout", e->m_GBufferLayout, fb::ShaderGBufferLayout);

        JSON_GET_BOOL(j, "OverrideDynamicAO", e->m_OverrideDynamicAO);

        JSON_GET_BOOL(j, "WireframeEnable", e->m_WireframeEnable);
        JSON_GET_BOOL(j, "OpaqueSortBySolutionEnable", e->m_OpaqueSortBySolutionEnable);
        JSON_GET_BOOL(j, "GenericEntityRendererEnable", e->m_GenericEntityRendererEnable);
        JSON_GET(j, "GenericEntityMaxVisibleEntityCount", e->m_GenericEntityMaxVisibleEntityCount);
        JSON_GET_BOOL(j, "SetupJobEnable", e->m_SetupJobEnable);
        JSON_GET_BOOL(j, "DynamicCullZBufferTestEnable", e->m_DynamicCullZBufferTestEnable);
        JSON_GET_BOOL(j, "DynamicCullDrawOccludedBoxesEnable", e->m_DynamicCullDrawOccludedBoxesEnable);
        JSON_GET_ENUM(j, "ViewMode", e->m_ViewMode, fb::WorldViewMode);
        JSON_GET(j, "OnlyTileIndex", e->m_OnlyTileIndex);
        JSON_GET(j, "OnlyLightTileX", e->m_OnlyLightTileX);
        JSON_GET(j, "OnlyLightTileY", e->m_OnlyLightTileY);
        JSON_GET(j, "OnlyLightTileIndex", e->m_OnlyLightTileIndex);

        JSON_GET_BOOL(j, "DrawDebugGBuffer", e->m_DrawDebugGBuffer);
        JSON_GET_BOOL(j, "DrawDebugZBufferEnable", e->m_DrawDebugZBufferEnable);
        JSON_GET_BOOL(j, "DrawDebugVelocityBuffer", e->m_DrawDebugVelocityBuffer);
        JSON_GET_BOOL(j, "DrawDebugMultisampleClassify", e->m_DrawDebugMultisampleClassify);
        JSON_GET_BOOL(j, "DrawDebugHalfResEnvironment", e->m_DrawDebugHalfResEnvironment);
        JSON_GET_BOOL(j, "DrawDebugShadowmaps", e->m_DrawDebugShadowmaps);
        JSON_GET_BOOL(j, "DrawDebugQuarterShadowmaps", e->m_DrawDebugQuarterShadowmaps);
        JSON_GET_BOOL(j, "DrawDebugTransShadowmap", e->m_DrawDebugTransShadowmap);
        JSON_GET_BOOL(j, "DrawDebugSpotLightShadowmaps", e->m_DrawDebugSpotLightShadowmaps);
        JSON_GET_BOOL(j, "DrawShadowFrustums", e->m_DrawShadowFrustums);
        JSON_GET_BOOL(j, "DrawDebugLightSources", e->m_DrawDebugLightSources);
        JSON_GET_BOOL(j, "DrawDebugLightShadowSources", e->m_DrawDebugLightShadowSources);
        JSON_GET_BOOL(j, "DrawDebugLightStats", e->m_DrawDebugLightStats);
        JSON_GET_BOOL(j, "DrawDebugLightTiles", e->m_DrawDebugLightTiles);
        JSON_GET_BOOL(j, "DrawDebugLightTileVolumes", e->m_DrawDebugLightTileVolumes);
        JSON_GET_BOOL(j, "DrawDebugLightTileFbGrid", e->m_DrawDebugLightTileFbGrid);
        JSON_GET_BOOL(j, "DrawDebugLightTileGrid", e->m_DrawDebugLightTileGrid);
        JSON_GET_BOOL(j, "DrawDebugLightTileSources", e->m_DrawDebugLightTileSources);
        JSON_GET_BOOL(j, "DrawDebugDynamicAO", e->m_DrawDebugDynamicAO);
        JSON_GET_BOOL(j, "DrawDebugDynamicEnvmap", e->m_DrawDebugDynamicEnvmap);
        JSON_GET_BOOL(j, "DrawDebugSkyEnvmap", e->m_DrawDebugSkyEnvmap);
        JSON_GET_BOOL(j, "DrawDebugPlanarReflection", e->m_DrawDebugPlanarReflection);
        JSON_GET_BOOL(j, "DrawDebugPlanarReflectionCullFrustum", e->m_DrawDebugPlanarReflectionCullFrustum);
        JSON_GET_BOOL(j, "DrawDebugDecalVolumes", e->m_DrawDebugDecalVolumes);
        JSON_GET_BOOL(j, "DrawDebugDestructionVolumes", e->m_DrawDebugDestructionVolumes);
        JSON_GET_BOOL(j, "DrawDebugLensFlares", e->m_DrawDebugLensFlares);
        JSON_GET_BOOL(j, "DrawDebugLensFlareOccluders", e->m_DrawDebugLensFlareOccluders);
        JSON_GET_BOOL(j, "DrawDebugWorldOcclusions", e->m_DrawDebugWorldOcclusions);
        JSON_GET_BOOL(j, "DrawDebugVisibleEntityTypes", e->m_DrawDebugVisibleEntityTypes);
        JSON_GET_BOOL(j, "DrawBoundingSpheres", e->m_DrawBoundingSpheres);
        JSON_GET_BOOL(j, "DrawSolidBoundingBoxes", e->m_DrawSolidBoundingBoxes);
        JSON_GET_BOOL(j, "DrawLineBoundingBoxes", e->m_DrawLineBoundingBoxes);
        JSON_GET_BOOL(j, "DrawFrustums", e->m_DrawFrustums);
    }
}