#include "world_render.h"
#include "../serialize/serialize.h"

#if defined(BFVE_GAME_BF4)

namespace editor::world_render
{
    json serialize()
    {
        json j;
        fb::WorldRenderSettings* s = getEdit();

        j["overrideEnabled"] = isOverrideEnabled();

        // Viewport & quality
        j["Enable"] = s->m_Enable;
        j["ViewportScale"] = s->m_ViewportScale;
        j["CullScreenAreaScale"] = s->m_CullScreenAreaScale;
        j["MultisampleCount"] = s->m_MultisampleCount;
        j["MultisampleThreshold"] = s->m_MultisampleThreshold;
        j["HudTextureWidth"] = s->m_HudTextureWidth;
        j["HudTextureHeight"] = s->m_HudTextureHeight;
        j["ViewMode"] = static_cast<int>(s->m_ViewMode);
        j["HdrEnable"] = s->m_HdrEnable;
        j["FastHdrEnable"] = s->m_FastHdrEnable;
        j["HalfResEnable"] = s->m_HalfResEnable;
        j["ForceFullResEnable"] = s->m_ForceFullResEnable;
        j["HalfResLensFlaresEnable"] = s->m_HalfResLensFlaresEnable;
        j["BilateralHalfResCompositeEnable"] = s->m_BilateralHalfResCompositeEnable;
        j["TiledHalfResCompositeEnable"] = s->m_TiledHalfResCompositeEnable;
        j["TiledHalfResStencilOccludersEnable"] = s->m_TiledHalfResStencilOccludersEnable;
        j["ReadOnlyDepthEnable"] = s->m_ReadOnlyDepthEnable;
        j["OutputGammaCorrectionEnable"] = s->m_OutputGammaCorrectionEnable;
        j["DxLinearDepth32BitFormatEnable"] = s->m_DxLinearDepth32BitFormatEnable;
        j["DxDepthBuffer32BitEnable"] = s->m_DxDepthBuffer32BitEnable;
        j["PostProcessAntialiasingMode"] = static_cast<int>(s->m_PostProcessAntialiasingMode);
        j["FxaaForceVendor"] = s->m_FxaaForceVendor;
        j["RenderScaleResampleMode"] = static_cast<int>(s->m_RenderScaleResampleMode);
        j["RenderScaleResampleEnable"] = s->m_RenderScaleResampleEnable;
        j["SimpleRenderingEnable"] = s->m_SimpleRenderingEnable;
        j["ViewFxEnable"] = s->m_ViewFxEnable;
        j["GenericEntityRendererEnable"] = s->m_GenericEntityRendererEnable;
        j["GenericEntityMaxVisibleEntityCount"] = s->m_GenericEntityMaxVisibleEntityCount;

        // Overlay
        j["OverlayEnable"] = s->m_OverlayEnable;
        JSON_SET_VEC4(j, "OverlayColor", s->m_OverlayColor);

        // SMAA
        j["SmaaVelocityReprojectionEnable"] = s->m_SmaaVelocityReprojectionEnable;
        j["SmaaUseStencil"] = s->m_SmaaUseStencil;
        j["SmaaPredicatedThresholdingEnable"] = s->m_SmaaPredicatedThresholdingEnable;

        // GBuffer
        j["GBufferLayout"] = s->m_GBufferLayout;
        j["GBufferTestCount"] = s->m_GBufferTestCount;
        j["GBufferAlphaTestSimpleSmoothness"] = s->m_GBufferAlphaTestSimpleSmoothness;
        j["GBufferForceSmoothness"] = s->m_GBufferForceSmoothness;
        j["GBufferForceSpecularAlbedo"] = s->m_GBufferForceSpecularAlbedo;
        j["GBufferClearEnable"] = s->m_GBufferClearEnable;
        j["GBufferAlphaTestSimpleEnable"] = s->m_GBufferAlphaTestSimpleEnable;
        j["DxGBufferLight16BitEnable"] = s->m_DxGBufferLight16BitEnable;
        j["DxGBufferNormal16BitEnable"] = s->m_DxGBufferNormal16BitEnable;
        j["Gen4aEsramEnable"] = s->m_Gen4aEsramEnable;

        // Shadows
        j["ShadowmapsEnable"] = s->m_ShadowmapsEnable;
        j["GenerateShadowmapsEnable"] = s->m_GenerateShadowmapsEnable;
        j["ApplyShadowmapsEnable"] = s->m_ApplyShadowmapsEnable;
        j["SimpleShadowmapsEnable"] = s->m_SimpleShadowmapsEnable;
        j["TransparencyShadowmapsEnable"] = s->m_TransparencyShadowmapsEnable;
        j["TransparencyShadowmapsHalfRes"] = s->m_TransparencyShadowmapsHalfRes;
        j["CloudShadowEnable"] = s->m_CloudShadowEnable;
        j["ShadowmapResolution"] = s->m_ShadowmapResolution;
        j["ShadowmapQuality"] = s->m_ShadowmapQuality;
        j["ShadowmapSliceCount"] = s->m_ShadowmapSliceCount;
        j["ShadowmapViewDistance"] = s->m_ShadowmapViewDistance;
        j["ShadowmapViewDistanceScaleEnable"] = s->m_ShadowmapViewDistanceScaleEnable;
        j["ShadowViewportScale"] = s->m_ShadowViewportScale;
        j["ShadowmapSizeZScale"] = s->m_ShadowmapSizeZScale;
        j["ShadowmapSliceSchemeWeight"] = s->m_ShadowmapSliceSchemeWeight;
        j["ShadowmapFirstSliceScale"] = s->m_ShadowmapFirstSliceScale;
        j["ShadowmapExtrusionLength"] = s->m_ShadowmapExtrusionLength;
        j["ShadowmapMinFov"] = s->m_ShadowmapMinFov;
        j["ShadowmapPoissonFilterScale"] = s->m_ShadowmapPoissonFilterScale;
        j["ShadowmapTransitionBlendAmount"] = s->m_ShadowmapTransitionBlendAmount;
        j["ShadowmapTransitionBlendEnable"] = s->m_ShadowmapTransitionBlendEnable;
        j["ShadowmapFixedDepthEnable"] = s->m_ShadowmapFixedDepthEnable;
        j["ShadowmapFixedMovementEnable"] = s->m_ShadowmapFixedMovementEnable;
        j["ShadowmapCullVolumeEnable"] = s->m_ShadowmapCullVolumeEnable;
        j["ShadowmapAccumEnable"] = s->m_ShadowmapAccumEnable;
        j["ShadowmapAccumBilinearEnable"] = s->m_ShadowmapAccumBilinearEnable;
        j["ZBufferShadowTestEnable"] = s->m_ZBufferShadowTestEnable;
        j["DxShadowmap16BitEnable"] = s->m_DxShadowmap16BitEnable;
        j["OnlyShadowmapSlice"] = s->m_OnlyShadowmapSlice;

        // Motion blur
        j["MotionBlurEnable"] = s->m_MotionBlurEnable;
        j["MotionBlurForceOn"] = s->m_MotionBlurForceOn;
        j["MotionBlurOptimalStableVelocityFormula"] = s->m_MotionBlurOptimalStableVelocityFormula;
        j["MotionBlurGeometryPassEnable"] = s->m_MotionBlurGeometryPassEnable;
        j["MotionBlurStencilPassEnable"] = s->m_MotionBlurStencilPassEnable;
        j["MotionBlurBackgroundPassEnable"] = s->m_MotionBlurBackgroundPassEnable;
        j["MotionBlurCenteredEnable"] = s->m_MotionBlurCenteredEnable;
        JSON_SET_VEC3(j, "MotionBlurClearColor", s->m_MotionBlurClearColor);
        j["MotionBlurScale"] = s->m_MotionBlurScale;
        j["MotionBlurFixedShutterTime"] = s->m_MotionBlurFixedShutterTime;
        j["MotionBlurMax"] = s->m_MotionBlurMax;
        j["MotionBlurRadialBlurMax"] = s->m_MotionBlurRadialBlurMax;
        j["MotionBlurNoiseScale"] = s->m_MotionBlurNoiseScale;
        j["MotionBlurQuality"] = s->m_MotionBlurQuality;
        j["MotionBlurDebugMode"] = s->m_MotionBlurDebugMode;
        j["MotionBlurMaxSampleCount"] = s->m_MotionBlurMaxSampleCount;
        j["ForceMotionBlurDepthCutoff"] = s->m_ForceMotionBlurDepthCutoff;
        j["ForceMotionBlurCutoffGradientScale"] = s->m_ForceMotionBlurCutoffGradientScale;
        j["MotionBlurDepthCheckThreshold"] = s->m_MotionBlurDepthCheckThreshold;
        j["MotionBlurDepthCheckMaxDistance"] = s->m_MotionBlurDepthCheckMaxDistance;

        // Lighting - general
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
        j["SubSurfaceScatteringEnable"] = s->m_SubSurfaceScatteringEnable;
        j["EmissiveEnable"] = s->m_EmissiveEnable;
        j["UnlitEnable"] = s->m_UnlitEnable;
        j["DynamicEnvmapLightingEnable"] = s->m_DynamicEnvmapLightingEnable;
        j["LightProbesEnable"] = s->m_LightProbesEnable;
        j["DrawLightSources"] = s->m_DrawLightSources;
        j["HairCoverageEnable"] = s->m_HairCoverageEnable;

        // Lighting - LOD
        j["LightLodSpecularFadeAreaStart"] = s->m_LightLodSpecularFadeAreaStart;
        j["LightLodSpecularFadeAreaEnd"] = s->m_LightLodSpecularFadeAreaEnd;
        j["LightLodRadiusFactor"] = s->m_LightLodRadiusFactor;
        j["LightLodFadeArea"] = s->m_LightLodFadeArea;
        j["LightLodMinArea"] = s->m_LightLodMinArea;

        // Lighting - culling
        j["LightCullEnable"] = s->m_LightCullEnable;
        j["LightOcclusionCullEnable"] = s->m_LightOcclusionCullEnable;
        j["LightDepthCullEnable"] = s->m_LightDepthCullEnable;
        j["LightNormalCullEnable"] = s->m_LightNormalCullEnable;
        j["LightConeCullEnable"] = s->m_LightConeCullEnable;
        j["LightNarrowAngleThreshold"] = s->m_LightNarrowAngleThreshold;
        j["LightStencilMinArea"] = s->m_LightStencilMinArea;
        j["LightStencilMethodEnable"] = s->m_LightStencilMethodEnable;
        j["LightVolumeMethodEnable"] = s->m_LightVolumeMethodEnable;
        j["LightVolumeDepthTestEnable"] = s->m_LightVolumeDepthTestEnable;
        j["LightTileMinArea"] = s->m_LightTileMinArea;
        j["LightTileOverlayEnable"] = s->m_LightTileOverlayEnable;

        // Light tiling
        j["LightTileCsAvgLightCountPerTile"] = s->m_LightTileCsAvgLightCountPerTile;
        j["LightTileCsPathEnable"] = s->m_LightTileCsPathEnable;
        j["LightTileMinMaxUseHTile"] = s->m_LightTileMinMaxUseHTile;
        j["LightTileSplitCullingAndLighting"] = s->m_LightTileSplitCullingAndLighting;
        j["LightCubeMapStencilEnable"] = s->m_LightCubeMapStencilEnable;
        j["LightCubeMapStencilMinArea"] = s->m_LightCubeMapStencilMinArea;

        // Outdoor light
        j["OutdoorLightEnable"] = s->m_OutdoorLightEnable;
        j["OutdoorKeyLightEnable"] = s->m_OutdoorKeyLightEnable;
        j["OutdoorSkyLightEnable"] = s->m_OutdoorSkyLightEnable;
        j["OutdoorLightTilingEnable"] = s->m_OutdoorLightTilingEnable;
        j["OutdoorLightTileRenderEnable"] = s->m_OutdoorLightTileRenderEnable;
        j["OutdoorLightTileBlendEnable"] = s->m_OutdoorLightTileBlendEnable;
        j["OutdoorLightTileSimpleShaderEnable"] = s->m_OutdoorLightTileSimpleShaderEnable;
        j["OutdoorLightTileBatchCount"] = s->m_OutdoorLightTileBatchCount;

        // Light types
        j["PointLightsEnable"] = s->m_PointLightsEnable;
        j["MaxPointLightCount"] = s->m_MaxPointLightCount;
        j["SpotLightsEnable"] = s->m_SpotLightsEnable;
        j["MaxSpotLightCount"] = s->m_MaxSpotLightCount;
        j["MaxSpotLightShadowCount"] = s->m_MaxSpotLightShadowCount;
        j["SpotLightsAsConeLightsLevel"] = static_cast<int>(s->m_SpotLightsAsConeLightsLevel);
        j["LineLightsEnable"] = s->m_LineLightsEnable;
        j["ConeLightsEnable"] = s->m_ConeLightsEnable;

        // Spot light shadows
        j["SpotLightShadowmapEnable"] = s->m_SpotLightShadowmapEnable;
        j["SpotLightShadowmapResolution"] = s->m_SpotLightShadowmapResolution;
        j["SpotLightShadowmapQuality"] = s->m_SpotLightShadowmapQuality;
        j["SpotLightNearPlane"] = s->m_SpotLightNearPlane;
        j["SpotLightShadowmapPoissonFilterScale"] = s->m_SpotLightShadowmapPoissonFilterScale;
        j["SpotLightShadowmapLevel"] = static_cast<int>(s->m_SpotLightShadowmapLevel);
        j["RadiositySpotLightShadowCullingEnable"] = s->m_RadiositySpotLightShadowCullingEnable;
        j["DxSpotLightShadowmap16BitEnable"] = s->m_DxSpotLightShadowmap16BitEnable;

        // Shadow occlusion & frustum culling
        j["ShadowOcclusionCullingEnable"] = s->m_ShadowOcclusionCullingEnable;
        j["ShadowOcclusionCullingWidth"] = s->m_ShadowOcclusionCullingWidth;
        j["ShadowOcclusionCullingHeight"] = s->m_ShadowOcclusionCullingHeight;
        j["ShadowOcclusionTriangleCount"] = s->m_ShadowOcclusionTriangleCount;
        j["FrustumSilhouetteCullingEnable"] = s->m_FrustumSilhouetteCullingEnable;
        j["FrustumSilhouetteCullingPadding"] = s->m_FrustumSilhouetteCullingPadding;

        // Sky
        j["SkyEnable"] = s->m_SkyEnable;
        j["SkyFogEnable"] = s->m_SkyFogEnable;
        j["SkyHeightFogEnable"] = s->m_SkyHeightFogEnable;
        j["SkyForwardScatteringEnable"] = s->m_SkyForwardScatteringEnable;
        j["SkyVisibilityEnvmapScalingEnable"] = s->m_SkyVisibilityEnvmapScalingEnable;
        j["SkyEnvmapEnable"] = s->m_SkyEnvmapEnable;
        j["SkyEnvmapMipmapGenEnable"] = s->m_SkyEnvmapMipmapGenEnable;
        j["SkyEnvmapUpdateEnable"] = s->m_SkyEnvmapUpdateEnable;
        j["SkyEnvmapForceUpdateEnable"] = s->m_SkyEnvmapForceUpdateEnable;
        j["SkyEnvmapUse8BitTexture"] = s->m_SkyEnvmapUse8BitTexture;
        j["SkyEnvmapResolution"] = s->m_SkyEnvmapResolution;
        j["SkyEnvmapSidesPerFrameCount"] = s->m_SkyEnvmapSidesPerFrameCount;
        j["SkyEnvmapFilterWidth"] = s->m_SkyEnvmapFilterWidth;
        j["SkyEnvmapFilterMode"] = static_cast<int>(s->m_SkyEnvmapFilterMode);
        j["DrawDebugSkyEnvmapMipLevel"] = s->m_DrawDebugSkyEnvmapMipLevel;

        // Dynamic envmap
        j["DynamicEnvmapEnable"] = s->m_DynamicEnvmapEnable;
        j["DynamicEnvmapMipmapGenEnable"] = s->m_DynamicEnvmapMipmapGenEnable;
        j["DynamicEnvmapResolution"] = s->m_DynamicEnvmapResolution;
        j["DynamicEnvmapFilterWidth"] = s->m_DynamicEnvmapFilterWidth;
        j["DynamicEnvmapFilterMode"] = static_cast<int>(s->m_DynamicEnvmapFilterMode);
        JSON_SET_VEC3(j, "DynamicEnvmapDefaultPosition", s->m_DynamicEnvmapDefaultPosition);
        j["DrawDebugDynamicEnvmapMipLevel"] = s->m_DrawDebugDynamicEnvmapMipLevel;
        j["StaticEnvmapEnable"] = s->m_StaticEnvmapEnable;
        j["CustomEnvmapEnable"] = s->m_CustomEnvmapEnable;
        j["CustomEnvmapMipmapClampEnable"] = s->m_CustomEnvmapMipmapClampEnable;

        // Distortion
        j["DistortionEnable"] = s->m_DistortionEnable;
        j["DistortionHalfResEnable"] = s->m_DistortionHalfResEnable;
        j["Distortion8BitEnable"] = s->m_Distortion8BitEnable;
        j["DistortionTilingEnable"] = s->m_DistortionTilingEnable;

        // Reflections
        j["PlanarReflectionEnable"] = s->m_PlanarReflectionEnable;
        j["PlanarReflectionFastHdrEnable"] = s->m_PlanarReflectionFastHdrEnable;
        j["PlanarReflectionBlur"] = s->m_PlanarReflectionBlur;
        j["PlanarReflectionClippingEnable"] = s->m_PlanarReflectionClippingEnable;
        j["PlanarReflectionViewScale"] = s->m_PlanarReflectionViewScale;
        j["PlanarReflectionCullFOV"] = s->m_PlanarReflectionCullFOV;
        j["PlanarReflectionFarPlane"] = s->m_PlanarReflectionFarPlane;
        j["ReflectionLodScale"] = s->m_ReflectionLodScale;

        // Decals & volumes
        j["DecalVolumeEnable"] = s->m_DecalVolumeEnable;
        j["DecalVolumeScale"] = s->m_DecalVolumeScale;
        j["MaxDecalVolumeCount"] = s->m_MaxDecalVolumeCount;
        j["MaxDestructionVolumeCount"] = s->m_MaxDestructionVolumeCount;

        // Lens flares
        j["LensFlaresEnable"] = s->m_LensFlaresEnable;
        j["LensFlareOcclusionEnable"] = s->m_LensFlareOcclusionEnable;

        // Render passes
        j["ZPassEnable"] = s->m_ZPassEnable;
        j["MainOpaqueZPassEnable"] = s->m_MainOpaqueZPassEnable;
        j["OccluderMeshZPrepassEnable"] = s->m_OccluderMeshZPrepassEnable;
        j["OccluderMeshZPrepassDebugEnable"] = s->m_OccluderMeshZPrepassDebugEnable;
        j["ForegroundEnable"] = s->m_ForegroundEnable;
        j["ForegroundZPassEnable"] = s->m_ForegroundZPassEnable;
        j["ForegroundDepthClearEnable"] = s->m_ForegroundDepthClearEnable;
        j["ForegroundTransparentEnable"] = s->m_ForegroundTransparentEnable;
        j["DrawTransparent"] = s->m_DrawTransparent;
        j["DrawTransparentDecal"] = s->m_DrawTransparentDecal;
        j["FinalPostEnable"] = s->m_FinalPostEnable;
        j["ScreenEffectEnable"] = s->m_ScreenEffectEnable;
        j["WireframeEnable"] = s->m_WireframeEnable;
        j["OpaqueSortBySolutionEnable"] = s->m_OpaqueSortBySolutionEnable;
        j["OnlyTileIndex"] = s->m_OnlyTileIndex;
        j["OnlyLightTileIndex"] = s->m_OnlyLightTileIndex;

        // Job scheduling / misc
        j["SetupJobEnable"] = s->m_SetupJobEnable;
        j["FinishSyncJobsFirstEnable"] = s->m_FinishSyncJobsFirstEnable;
        j["PrepareDispatchListJobEnable"] = s->m_PrepareDispatchListJobEnable;
        j["ConsoleRenderTargetPoolSharingEnable"] = s->m_ConsoleRenderTargetPoolSharingEnable;
        j["EmitterShadowingBlendToggle"] = s->m_EmitterShadowingBlendToggle;
        j["EmitterShadowingManySamplesToggle"] = s->m_EmitterShadowingManySamplesToggle;

        // Dynamic AO
        j["OverrideDynamicAO"] = s->m_OverrideDynamicAO;

        // Debug
        j["DrawDebugGBuffer"] = s->m_DrawDebugGBuffer;
        j["DrawDebugZBufferEnable"] = s->m_DrawDebugZBufferEnable;
        j["DrawDebugVelocityBuffer"] = s->m_DrawDebugVelocityBuffer;
        j["DrawDebugMultisampleClassify"] = s->m_DrawDebugMultisampleClassify;
        j["DrawDebugHalfResEnvironment"] = s->m_DrawDebugHalfResEnvironment;
        j["DrawDebugHalfResHdrTargets"] = s->m_DrawDebugHalfResHdrTargets;
        j["DrawDebugGroundHeight"] = s->m_DrawDebugGroundHeight;
        j["DrawDebugShadowmaps"] = s->m_DrawDebugShadowmaps;
        j["DrawDebugSpotLightShadowmaps"] = s->m_DrawDebugSpotLightShadowmaps;
        j["DrawDebugLightSources"] = s->m_DrawDebugLightSources;
        j["DrawDebugLightShadowSources"] = s->m_DrawDebugLightShadowSources;
        j["DrawDebugLightTextureSources"] = s->m_DrawDebugLightTextureSources;
        j["DrawDebugLightStats"] = s->m_DrawDebugLightStats;
        j["DrawDebugLightTiles"] = s->m_DrawDebugLightTiles;
        j["DrawDebugLightTileVolumes"] = s->m_DrawDebugLightTileVolumes;
        j["DrawDebugLightTileFbGrid"] = s->m_DrawDebugLightTileFbGrid;
        j["DrawDebugLightTileGrid"] = s->m_DrawDebugLightTileGrid;
        j["DrawDebugLightTileSources"] = s->m_DrawDebugLightTileSources;
        j["DrawDebugSkyEnvmap"] = s->m_DrawDebugSkyEnvmap;
        j["DrawDebugSkyTextures"] = s->m_DrawDebugSkyTextures;
        j["DrawDebugDynamicEnvmap"] = s->m_DrawDebugDynamicEnvmap;
        j["DrawDebugPlanarReflection"] = s->m_DrawDebugPlanarReflection;
        j["DrawDebugPlanarReflectionCullFrustum"] = s->m_DrawDebugPlanarReflectionCullFrustum;
        j["DrawDebugPlanarReflectionMode"] = s->m_DrawDebugPlanarReflectionMode;
        j["DrawDebugIndirectCubeMapEnable"] = s->m_DrawDebugIndirectCubeMapEnable;
        j["DrawDebugIndirectCubeMapIndex"] = s->m_DrawDebugIndirectCubeMapIndex;
        j["DrawDebugIndirectCubeMapMipLevel"] = s->m_DrawDebugIndirectCubeMapMipLevel;
        j["DrawDebugDecalVolumes"] = s->m_DrawDebugDecalVolumes;
        j["DrawDebugDestructionVolumes"] = s->m_DrawDebugDestructionVolumes;
        j["DrawDebugLensFlares"] = s->m_DrawDebugLensFlares;
        j["DrawDebugLensFlareOccluders"] = s->m_DrawDebugLensFlareOccluders;
        j["DrawDebugDynamicAO"] = s->m_DrawDebugDynamicAO;
        j["DrawDebugDistortion"] = s->m_DrawDebugDistortion;
        j["DrawDebugVisibleEntityTypes"] = s->m_DrawDebugVisibleEntityTypes;
        j["DrawDebugDof"] = s->m_DrawDebugDof;
        j["DrawDebugDofFullscreen"] = s->m_DrawDebugDofFullscreen;
        j["DrawDebugMarschnerTextures"] = s->m_DrawDebugMarschnerTextures;
        j["DrawBoundingSpheres"] = s->m_DrawBoundingSpheres;
        j["DrawSolidBoundingBoxes"] = s->m_DrawSolidBoundingBoxes;
        j["DrawLineBoundingBoxes"] = s->m_DrawLineBoundingBoxes;
        j["DrawFrustums"] = s->m_DrawFrustums;

        return j;
    }

    void deserialize(const json& j)
    {
        fb::WorldRenderSettings* e = getEdit();

        setOverrideEnabled(j.value("overrideEnabled", isOverrideEnabled()));

        // Viewport & quality
        JSON_GET_BOOL(j, "Enable", e->m_Enable);
        JSON_GET(j, "ViewportScale", e->m_ViewportScale);
        JSON_GET(j, "CullScreenAreaScale", e->m_CullScreenAreaScale);
        JSON_GET(j, "MultisampleCount", e->m_MultisampleCount);
        JSON_GET(j, "MultisampleThreshold", e->m_MultisampleThreshold);
        JSON_GET(j, "HudTextureWidth", e->m_HudTextureWidth);
        JSON_GET(j, "HudTextureHeight", e->m_HudTextureHeight);
        JSON_GET_ENUM(j, "ViewMode", e->m_ViewMode, fb::WorldViewMode);
        JSON_GET_BOOL(j, "HdrEnable", e->m_HdrEnable);
        JSON_GET_BOOL(j, "FastHdrEnable", e->m_FastHdrEnable);
        JSON_GET_BOOL(j, "HalfResEnable", e->m_HalfResEnable);
        JSON_GET_BOOL(j, "ForceFullResEnable", e->m_ForceFullResEnable);
        JSON_GET_BOOL(j, "HalfResLensFlaresEnable", e->m_HalfResLensFlaresEnable);
        JSON_GET_BOOL(j, "BilateralHalfResCompositeEnable", e->m_BilateralHalfResCompositeEnable);
        JSON_GET_BOOL(j, "TiledHalfResCompositeEnable", e->m_TiledHalfResCompositeEnable);
        JSON_GET_BOOL(j, "TiledHalfResStencilOccludersEnable", e->m_TiledHalfResStencilOccludersEnable);
        JSON_GET_BOOL(j, "ReadOnlyDepthEnable", e->m_ReadOnlyDepthEnable);
        JSON_GET_BOOL(j, "OutputGammaCorrectionEnable", e->m_OutputGammaCorrectionEnable);
        JSON_GET_BOOL(j, "DxLinearDepth32BitFormatEnable", e->m_DxLinearDepth32BitFormatEnable);
        JSON_GET_BOOL(j, "DxDepthBuffer32BitEnable", e->m_DxDepthBuffer32BitEnable);
        JSON_GET_ENUM(j, "PostProcessAntialiasingMode", e->m_PostProcessAntialiasingMode, fb::PostProcessAAMode);
        JSON_GET(j, "FxaaForceVendor", e->m_FxaaForceVendor);
        JSON_GET_ENUM(j, "RenderScaleResampleMode", e->m_RenderScaleResampleMode, fb::ScaleResampleMode);
        JSON_GET_BOOL(j, "RenderScaleResampleEnable", e->m_RenderScaleResampleEnable);
        JSON_GET_BOOL(j, "SimpleRenderingEnable", e->m_SimpleRenderingEnable);
        JSON_GET_BOOL(j, "ViewFxEnable", e->m_ViewFxEnable);
        JSON_GET_BOOL(j, "GenericEntityRendererEnable", e->m_GenericEntityRendererEnable);
        JSON_GET(j, "GenericEntityMaxVisibleEntityCount", e->m_GenericEntityMaxVisibleEntityCount);

        // Overlay
        JSON_GET_BOOL(j, "OverlayEnable", e->m_OverlayEnable);
        JSON_GET_VEC4(j, "OverlayColor", e->m_OverlayColor);

        // SMAA
        JSON_GET_BOOL(j, "SmaaVelocityReprojectionEnable", e->m_SmaaVelocityReprojectionEnable);
        JSON_GET_BOOL(j, "SmaaUseStencil", e->m_SmaaUseStencil);
        JSON_GET_BOOL(j, "SmaaPredicatedThresholdingEnable", e->m_SmaaPredicatedThresholdingEnable);

        // GBuffer
        JSON_GET(j, "GBufferLayout", e->m_GBufferLayout);
        JSON_GET(j, "GBufferTestCount", e->m_GBufferTestCount);
        JSON_GET(j, "GBufferAlphaTestSimpleSmoothness", e->m_GBufferAlphaTestSimpleSmoothness);
        JSON_GET(j, "GBufferForceSmoothness", e->m_GBufferForceSmoothness);
        JSON_GET(j, "GBufferForceSpecularAlbedo", e->m_GBufferForceSpecularAlbedo);
        JSON_GET_BOOL(j, "GBufferClearEnable", e->m_GBufferClearEnable);
        JSON_GET_BOOL(j, "GBufferAlphaTestSimpleEnable", e->m_GBufferAlphaTestSimpleEnable);
        JSON_GET_BOOL(j, "DxGBufferLight16BitEnable", e->m_DxGBufferLight16BitEnable);
        JSON_GET_BOOL(j, "DxGBufferNormal16BitEnable", e->m_DxGBufferNormal16BitEnable);
        JSON_GET_BOOL(j, "Gen4aEsramEnable", e->m_Gen4aEsramEnable);

        // Shadows
        JSON_GET_BOOL(j, "ShadowmapsEnable", e->m_ShadowmapsEnable);
        JSON_GET_BOOL(j, "GenerateShadowmapsEnable", e->m_GenerateShadowmapsEnable);
        JSON_GET_BOOL(j, "ApplyShadowmapsEnable", e->m_ApplyShadowmapsEnable);
        JSON_GET_BOOL(j, "SimpleShadowmapsEnable", e->m_SimpleShadowmapsEnable);
        JSON_GET_BOOL(j, "TransparencyShadowmapsEnable", e->m_TransparencyShadowmapsEnable);
        JSON_GET_BOOL(j, "TransparencyShadowmapsHalfRes", e->m_TransparencyShadowmapsHalfRes);
        JSON_GET_BOOL(j, "CloudShadowEnable", e->m_CloudShadowEnable);
        JSON_GET(j, "ShadowmapResolution", e->m_ShadowmapResolution);
        JSON_GET(j, "ShadowmapQuality", e->m_ShadowmapQuality);
        JSON_GET(j, "ShadowmapSliceCount", e->m_ShadowmapSliceCount);
        JSON_GET(j, "ShadowmapViewDistance", e->m_ShadowmapViewDistance);
        JSON_GET_BOOL(j, "ShadowmapViewDistanceScaleEnable", e->m_ShadowmapViewDistanceScaleEnable);
        JSON_GET(j, "ShadowViewportScale", e->m_ShadowViewportScale);
        JSON_GET(j, "ShadowmapSizeZScale", e->m_ShadowmapSizeZScale);
        JSON_GET(j, "ShadowmapSliceSchemeWeight", e->m_ShadowmapSliceSchemeWeight);
        JSON_GET(j, "ShadowmapFirstSliceScale", e->m_ShadowmapFirstSliceScale);
        JSON_GET(j, "ShadowmapExtrusionLength", e->m_ShadowmapExtrusionLength);
        JSON_GET(j, "ShadowmapMinFov", e->m_ShadowmapMinFov);
        JSON_GET(j, "ShadowmapPoissonFilterScale", e->m_ShadowmapPoissonFilterScale);
        JSON_GET(j, "ShadowmapTransitionBlendAmount", e->m_ShadowmapTransitionBlendAmount);
        JSON_GET_BOOL(j, "ShadowmapTransitionBlendEnable", e->m_ShadowmapTransitionBlendEnable);
        JSON_GET_BOOL(j, "ShadowmapFixedDepthEnable", e->m_ShadowmapFixedDepthEnable);
        JSON_GET_BOOL(j, "ShadowmapFixedMovementEnable", e->m_ShadowmapFixedMovementEnable);
        JSON_GET_BOOL(j, "ShadowmapCullVolumeEnable", e->m_ShadowmapCullVolumeEnable);
        JSON_GET_BOOL(j, "ShadowmapAccumEnable", e->m_ShadowmapAccumEnable);
        JSON_GET_BOOL(j, "ShadowmapAccumBilinearEnable", e->m_ShadowmapAccumBilinearEnable);
        JSON_GET_BOOL(j, "ZBufferShadowTestEnable", e->m_ZBufferShadowTestEnable);
        JSON_GET_BOOL(j, "DxShadowmap16BitEnable", e->m_DxShadowmap16BitEnable);
        JSON_GET(j, "OnlyShadowmapSlice", e->m_OnlyShadowmapSlice);

        // Motion blur
        JSON_GET_BOOL(j, "MotionBlurEnable", e->m_MotionBlurEnable);
        JSON_GET_BOOL(j, "MotionBlurForceOn", e->m_MotionBlurForceOn);
        JSON_GET_BOOL(j, "MotionBlurOptimalStableVelocityFormula", e->m_MotionBlurOptimalStableVelocityFormula);
        JSON_GET_BOOL(j, "MotionBlurGeometryPassEnable", e->m_MotionBlurGeometryPassEnable);
        JSON_GET_BOOL(j, "MotionBlurStencilPassEnable", e->m_MotionBlurStencilPassEnable);
        JSON_GET_BOOL(j, "MotionBlurBackgroundPassEnable", e->m_MotionBlurBackgroundPassEnable);
        JSON_GET_BOOL(j, "MotionBlurCenteredEnable", e->m_MotionBlurCenteredEnable);
        JSON_GET_VEC3(j, "MotionBlurClearColor", e->m_MotionBlurClearColor);
        JSON_GET(j, "MotionBlurScale", e->m_MotionBlurScale);
        JSON_GET(j, "MotionBlurFixedShutterTime", e->m_MotionBlurFixedShutterTime);
        JSON_GET(j, "MotionBlurMax", e->m_MotionBlurMax);
        JSON_GET(j, "MotionBlurRadialBlurMax", e->m_MotionBlurRadialBlurMax);
        JSON_GET(j, "MotionBlurNoiseScale", e->m_MotionBlurNoiseScale);
        JSON_GET(j, "MotionBlurQuality", e->m_MotionBlurQuality);
        JSON_GET(j, "MotionBlurDebugMode", e->m_MotionBlurDebugMode);
        JSON_GET(j, "MotionBlurMaxSampleCount", e->m_MotionBlurMaxSampleCount);
        JSON_GET(j, "ForceMotionBlurDepthCutoff", e->m_ForceMotionBlurDepthCutoff);
        JSON_GET(j, "ForceMotionBlurCutoffGradientScale", e->m_ForceMotionBlurCutoffGradientScale);
        JSON_GET(j, "MotionBlurDepthCheckThreshold", e->m_MotionBlurDepthCheckThreshold);
        JSON_GET(j, "MotionBlurDepthCheckMaxDistance", e->m_MotionBlurDepthCheckMaxDistance);

        // Lighting - general
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
        JSON_GET_BOOL(j, "SubSurfaceScatteringEnable", e->m_SubSurfaceScatteringEnable);
        JSON_GET_BOOL(j, "EmissiveEnable", e->m_EmissiveEnable);
        JSON_GET_BOOL(j, "UnlitEnable", e->m_UnlitEnable);
        JSON_GET_BOOL(j, "DynamicEnvmapLightingEnable", e->m_DynamicEnvmapLightingEnable);
        JSON_GET_BOOL(j, "LightProbesEnable", e->m_LightProbesEnable);
        JSON_GET_BOOL(j, "DrawLightSources", e->m_DrawLightSources);
        JSON_GET_BOOL(j, "HairCoverageEnable", e->m_HairCoverageEnable);

        // Lighting - LOD
        JSON_GET(j, "LightLodSpecularFadeAreaStart", e->m_LightLodSpecularFadeAreaStart);
        JSON_GET(j, "LightLodSpecularFadeAreaEnd", e->m_LightLodSpecularFadeAreaEnd);
        JSON_GET(j, "LightLodRadiusFactor", e->m_LightLodRadiusFactor);
        JSON_GET(j, "LightLodFadeArea", e->m_LightLodFadeArea);
        JSON_GET(j, "LightLodMinArea", e->m_LightLodMinArea);

        // Lighting - culling
        JSON_GET_BOOL(j, "LightCullEnable", e->m_LightCullEnable);
        JSON_GET_BOOL(j, "LightOcclusionCullEnable", e->m_LightOcclusionCullEnable);
        JSON_GET_BOOL(j, "LightDepthCullEnable", e->m_LightDepthCullEnable);
        JSON_GET_BOOL(j, "LightNormalCullEnable", e->m_LightNormalCullEnable);
        JSON_GET_BOOL(j, "LightConeCullEnable", e->m_LightConeCullEnable);
        JSON_GET(j, "LightNarrowAngleThreshold", e->m_LightNarrowAngleThreshold);
        JSON_GET(j, "LightStencilMinArea", e->m_LightStencilMinArea);
        JSON_GET_BOOL(j, "LightStencilMethodEnable", e->m_LightStencilMethodEnable);
        JSON_GET_BOOL(j, "LightVolumeMethodEnable", e->m_LightVolumeMethodEnable);
        JSON_GET_BOOL(j, "LightVolumeDepthTestEnable", e->m_LightVolumeDepthTestEnable);
        JSON_GET(j, "LightTileMinArea", e->m_LightTileMinArea);
        JSON_GET_BOOL(j, "LightTileOverlayEnable", e->m_LightTileOverlayEnable);

        // Light tiling
        JSON_GET(j, "LightTileCsAvgLightCountPerTile", e->m_LightTileCsAvgLightCountPerTile);
        JSON_GET_BOOL(j, "LightTileCsPathEnable", e->m_LightTileCsPathEnable);
        JSON_GET_BOOL(j, "LightTileMinMaxUseHTile", e->m_LightTileMinMaxUseHTile);
        JSON_GET_BOOL(j, "LightTileSplitCullingAndLighting", e->m_LightTileSplitCullingAndLighting);
        JSON_GET_BOOL(j, "LightCubeMapStencilEnable", e->m_LightCubeMapStencilEnable);
        JSON_GET(j, "LightCubeMapStencilMinArea", e->m_LightCubeMapStencilMinArea);

        // Outdoor light
        JSON_GET_BOOL(j, "OutdoorLightEnable", e->m_OutdoorLightEnable);
        JSON_GET_BOOL(j, "OutdoorKeyLightEnable", e->m_OutdoorKeyLightEnable);
        JSON_GET_BOOL(j, "OutdoorSkyLightEnable", e->m_OutdoorSkyLightEnable);
        JSON_GET_BOOL(j, "OutdoorLightTilingEnable", e->m_OutdoorLightTilingEnable);
        JSON_GET_BOOL(j, "OutdoorLightTileRenderEnable", e->m_OutdoorLightTileRenderEnable);
        JSON_GET_BOOL(j, "OutdoorLightTileBlendEnable", e->m_OutdoorLightTileBlendEnable);
        JSON_GET_BOOL(j, "OutdoorLightTileSimpleShaderEnable", e->m_OutdoorLightTileSimpleShaderEnable);
        JSON_GET(j, "OutdoorLightTileBatchCount", e->m_OutdoorLightTileBatchCount);

        // Light types
        JSON_GET_BOOL(j, "PointLightsEnable", e->m_PointLightsEnable);
        JSON_GET(j, "MaxPointLightCount", e->m_MaxPointLightCount);
        JSON_GET_BOOL(j, "SpotLightsEnable", e->m_SpotLightsEnable);
        JSON_GET(j, "MaxSpotLightCount", e->m_MaxSpotLightCount);
        JSON_GET(j, "MaxSpotLightShadowCount", e->m_MaxSpotLightShadowCount);
        JSON_GET_ENUM(j, "SpotLightsAsConeLightsLevel", e->m_SpotLightsAsConeLightsLevel, fb::QualityLevel);
        JSON_GET_BOOL(j, "LineLightsEnable", e->m_LineLightsEnable);
        JSON_GET_BOOL(j, "ConeLightsEnable", e->m_ConeLightsEnable);

        // Spot light shadows
        JSON_GET_BOOL(j, "SpotLightShadowmapEnable", e->m_SpotLightShadowmapEnable);
        JSON_GET(j, "SpotLightShadowmapResolution", e->m_SpotLightShadowmapResolution);
        JSON_GET(j, "SpotLightShadowmapQuality", e->m_SpotLightShadowmapQuality);
        JSON_GET(j, "SpotLightNearPlane", e->m_SpotLightNearPlane);
        JSON_GET(j, "SpotLightShadowmapPoissonFilterScale", e->m_SpotLightShadowmapPoissonFilterScale);
        JSON_GET_ENUM(j, "SpotLightShadowmapLevel", e->m_SpotLightShadowmapLevel, fb::QualityLevel);
        JSON_GET_BOOL(j, "RadiositySpotLightShadowCullingEnable", e->m_RadiositySpotLightShadowCullingEnable);
        JSON_GET_BOOL(j, "DxSpotLightShadowmap16BitEnable", e->m_DxSpotLightShadowmap16BitEnable);

        // Shadow occlusion & frustum culling
        JSON_GET_BOOL(j, "ShadowOcclusionCullingEnable", e->m_ShadowOcclusionCullingEnable);
        JSON_GET(j, "ShadowOcclusionCullingWidth", e->m_ShadowOcclusionCullingWidth);
        JSON_GET(j, "ShadowOcclusionCullingHeight", e->m_ShadowOcclusionCullingHeight);
        JSON_GET(j, "ShadowOcclusionTriangleCount", e->m_ShadowOcclusionTriangleCount);
        JSON_GET_BOOL(j, "FrustumSilhouetteCullingEnable", e->m_FrustumSilhouetteCullingEnable);
        JSON_GET(j, "FrustumSilhouetteCullingPadding", e->m_FrustumSilhouetteCullingPadding);

        // Sky
        JSON_GET_BOOL(j, "SkyEnable", e->m_SkyEnable);
        JSON_GET_BOOL(j, "SkyFogEnable", e->m_SkyFogEnable);
        JSON_GET_BOOL(j, "SkyHeightFogEnable", e->m_SkyHeightFogEnable);
        JSON_GET_BOOL(j, "SkyForwardScatteringEnable", e->m_SkyForwardScatteringEnable);
        JSON_GET_BOOL(j, "SkyVisibilityEnvmapScalingEnable", e->m_SkyVisibilityEnvmapScalingEnable);
        JSON_GET_BOOL(j, "SkyEnvmapEnable", e->m_SkyEnvmapEnable);
        JSON_GET_BOOL(j, "SkyEnvmapMipmapGenEnable", e->m_SkyEnvmapMipmapGenEnable);
        JSON_GET_BOOL(j, "SkyEnvmapUpdateEnable", e->m_SkyEnvmapUpdateEnable);
        JSON_GET_BOOL(j, "SkyEnvmapForceUpdateEnable", e->m_SkyEnvmapForceUpdateEnable);
        JSON_GET_BOOL(j, "SkyEnvmapUse8BitTexture", e->m_SkyEnvmapUse8BitTexture);
        JSON_GET(j, "SkyEnvmapResolution", e->m_SkyEnvmapResolution);
        JSON_GET(j, "SkyEnvmapSidesPerFrameCount", e->m_SkyEnvmapSidesPerFrameCount);
        JSON_GET(j, "SkyEnvmapFilterWidth", e->m_SkyEnvmapFilterWidth);
        JSON_GET_ENUM(j, "SkyEnvmapFilterMode", e->m_SkyEnvmapFilterMode, fb::MipmapFilterMode);
        JSON_GET(j, "DrawDebugSkyEnvmapMipLevel", e->m_DrawDebugSkyEnvmapMipLevel);

        // Dynamic envmap
        JSON_GET_BOOL(j, "DynamicEnvmapEnable", e->m_DynamicEnvmapEnable);
        JSON_GET_BOOL(j, "DynamicEnvmapMipmapGenEnable", e->m_DynamicEnvmapMipmapGenEnable);
        JSON_GET(j, "DynamicEnvmapResolution", e->m_DynamicEnvmapResolution);
        JSON_GET(j, "DynamicEnvmapFilterWidth", e->m_DynamicEnvmapFilterWidth);
        JSON_GET_ENUM(j, "DynamicEnvmapFilterMode", e->m_DynamicEnvmapFilterMode, fb::MipmapFilterMode);
        JSON_GET_VEC3(j, "DynamicEnvmapDefaultPosition", e->m_DynamicEnvmapDefaultPosition);
        JSON_GET(j, "DrawDebugDynamicEnvmapMipLevel", e->m_DrawDebugDynamicEnvmapMipLevel);
        JSON_GET_BOOL(j, "StaticEnvmapEnable", e->m_StaticEnvmapEnable);
        JSON_GET_BOOL(j, "CustomEnvmapEnable", e->m_CustomEnvmapEnable);
        JSON_GET_BOOL(j, "CustomEnvmapMipmapClampEnable", e->m_CustomEnvmapMipmapClampEnable);

        // Distortion
        JSON_GET_BOOL(j, "DistortionEnable", e->m_DistortionEnable);
        JSON_GET_BOOL(j, "DistortionHalfResEnable", e->m_DistortionHalfResEnable);
        JSON_GET_BOOL(j, "Distortion8BitEnable", e->m_Distortion8BitEnable);
        JSON_GET_BOOL(j, "DistortionTilingEnable", e->m_DistortionTilingEnable);

        // Reflections
        JSON_GET_BOOL(j, "PlanarReflectionEnable", e->m_PlanarReflectionEnable);
        JSON_GET_BOOL(j, "PlanarReflectionFastHdrEnable", e->m_PlanarReflectionFastHdrEnable);
        JSON_GET_BOOL(j, "PlanarReflectionBlur", e->m_PlanarReflectionBlur);
        JSON_GET_BOOL(j, "PlanarReflectionClippingEnable", e->m_PlanarReflectionClippingEnable);
        JSON_GET(j, "PlanarReflectionViewScale", e->m_PlanarReflectionViewScale);
        JSON_GET(j, "PlanarReflectionCullFOV", e->m_PlanarReflectionCullFOV);
        JSON_GET(j, "PlanarReflectionFarPlane", e->m_PlanarReflectionFarPlane);
        JSON_GET(j, "ReflectionLodScale", e->m_ReflectionLodScale);

        // Decals & volumes
        JSON_GET_BOOL(j, "DecalVolumeEnable", e->m_DecalVolumeEnable);
        JSON_GET(j, "DecalVolumeScale", e->m_DecalVolumeScale);
        JSON_GET(j, "MaxDecalVolumeCount", e->m_MaxDecalVolumeCount);
        JSON_GET(j, "MaxDestructionVolumeCount", e->m_MaxDestructionVolumeCount);

        // Lens flares
        JSON_GET_BOOL(j, "LensFlaresEnable", e->m_LensFlaresEnable);
        JSON_GET_BOOL(j, "LensFlareOcclusionEnable", e->m_LensFlareOcclusionEnable);

        // Render passes
        JSON_GET_BOOL(j, "ZPassEnable", e->m_ZPassEnable);
        JSON_GET_BOOL(j, "MainOpaqueZPassEnable", e->m_MainOpaqueZPassEnable);
        JSON_GET_BOOL(j, "OccluderMeshZPrepassEnable", e->m_OccluderMeshZPrepassEnable);
        JSON_GET_BOOL(j, "OccluderMeshZPrepassDebugEnable", e->m_OccluderMeshZPrepassDebugEnable);
        JSON_GET_BOOL(j, "ForegroundEnable", e->m_ForegroundEnable);
        JSON_GET_BOOL(j, "ForegroundZPassEnable", e->m_ForegroundZPassEnable);
        JSON_GET_BOOL(j, "ForegroundDepthClearEnable", e->m_ForegroundDepthClearEnable);
        JSON_GET_BOOL(j, "ForegroundTransparentEnable", e->m_ForegroundTransparentEnable);
        JSON_GET_BOOL(j, "DrawTransparent", e->m_DrawTransparent);
        JSON_GET_BOOL(j, "DrawTransparentDecal", e->m_DrawTransparentDecal);
        JSON_GET_BOOL(j, "FinalPostEnable", e->m_FinalPostEnable);
        JSON_GET_BOOL(j, "ScreenEffectEnable", e->m_ScreenEffectEnable);
        JSON_GET_BOOL(j, "WireframeEnable", e->m_WireframeEnable);
        JSON_GET_BOOL(j, "OpaqueSortBySolutionEnable", e->m_OpaqueSortBySolutionEnable);
        JSON_GET(j, "OnlyTileIndex", e->m_OnlyTileIndex);
        JSON_GET(j, "OnlyLightTileIndex", e->m_OnlyLightTileIndex);

        // Job scheduling / misc
        JSON_GET_BOOL(j, "SetupJobEnable", e->m_SetupJobEnable);
        JSON_GET_BOOL(j, "FinishSyncJobsFirstEnable", e->m_FinishSyncJobsFirstEnable);
        JSON_GET_BOOL(j, "PrepareDispatchListJobEnable", e->m_PrepareDispatchListJobEnable);
        JSON_GET_BOOL(j, "ConsoleRenderTargetPoolSharingEnable", e->m_ConsoleRenderTargetPoolSharingEnable);
        JSON_GET_BOOL(j, "EmitterShadowingBlendToggle", e->m_EmitterShadowingBlendToggle);
        JSON_GET_BOOL(j, "EmitterShadowingManySamplesToggle", e->m_EmitterShadowingManySamplesToggle);

        // Dynamic AO
        JSON_GET_BOOL(j, "OverrideDynamicAO", e->m_OverrideDynamicAO);

        // Debug
        JSON_GET_BOOL(j, "DrawDebugGBuffer", e->m_DrawDebugGBuffer);
        JSON_GET_BOOL(j, "DrawDebugZBufferEnable", e->m_DrawDebugZBufferEnable);
        JSON_GET_BOOL(j, "DrawDebugVelocityBuffer", e->m_DrawDebugVelocityBuffer);
        JSON_GET_BOOL(j, "DrawDebugMultisampleClassify", e->m_DrawDebugMultisampleClassify);
        JSON_GET_BOOL(j, "DrawDebugHalfResEnvironment", e->m_DrawDebugHalfResEnvironment);
        JSON_GET_BOOL(j, "DrawDebugHalfResHdrTargets", e->m_DrawDebugHalfResHdrTargets);
        JSON_GET(j, "DrawDebugGroundHeight", e->m_DrawDebugGroundHeight);
        JSON_GET_BOOL(j, "DrawDebugShadowmaps", e->m_DrawDebugShadowmaps);
        JSON_GET_BOOL(j, "DrawDebugSpotLightShadowmaps", e->m_DrawDebugSpotLightShadowmaps);
        JSON_GET_BOOL(j, "DrawDebugLightSources", e->m_DrawDebugLightSources);
        JSON_GET_BOOL(j, "DrawDebugLightShadowSources", e->m_DrawDebugLightShadowSources);
        JSON_GET_BOOL(j, "DrawDebugLightTextureSources", e->m_DrawDebugLightTextureSources);
        JSON_GET_BOOL(j, "DrawDebugLightStats", e->m_DrawDebugLightStats);
        JSON_GET_BOOL(j, "DrawDebugLightTiles", e->m_DrawDebugLightTiles);
        JSON_GET_BOOL(j, "DrawDebugLightTileVolumes", e->m_DrawDebugLightTileVolumes);
        JSON_GET_BOOL(j, "DrawDebugLightTileFbGrid", e->m_DrawDebugLightTileFbGrid);
        JSON_GET_BOOL(j, "DrawDebugLightTileGrid", e->m_DrawDebugLightTileGrid);
        JSON_GET_BOOL(j, "DrawDebugLightTileSources", e->m_DrawDebugLightTileSources);
        JSON_GET_BOOL(j, "DrawDebugSkyEnvmap", e->m_DrawDebugSkyEnvmap);
        JSON_GET_BOOL(j, "DrawDebugSkyTextures", e->m_DrawDebugSkyTextures);
        JSON_GET_BOOL(j, "DrawDebugDynamicEnvmap", e->m_DrawDebugDynamicEnvmap);
        JSON_GET_BOOL(j, "DrawDebugPlanarReflection", e->m_DrawDebugPlanarReflection);
        JSON_GET_BOOL(j, "DrawDebugPlanarReflectionCullFrustum", e->m_DrawDebugPlanarReflectionCullFrustum);
        JSON_GET(j, "DrawDebugPlanarReflectionMode", e->m_DrawDebugPlanarReflectionMode);
        JSON_GET_BOOL(j, "DrawDebugIndirectCubeMapEnable", e->m_DrawDebugIndirectCubeMapEnable);
        JSON_GET(j, "DrawDebugIndirectCubeMapIndex", e->m_DrawDebugIndirectCubeMapIndex);
        JSON_GET(j, "DrawDebugIndirectCubeMapMipLevel", e->m_DrawDebugIndirectCubeMapMipLevel);
        JSON_GET_BOOL(j, "DrawDebugDecalVolumes", e->m_DrawDebugDecalVolumes);
        JSON_GET_BOOL(j, "DrawDebugDestructionVolumes", e->m_DrawDebugDestructionVolumes);
        JSON_GET_BOOL(j, "DrawDebugLensFlares", e->m_DrawDebugLensFlares);
        JSON_GET_BOOL(j, "DrawDebugLensFlareOccluders", e->m_DrawDebugLensFlareOccluders);
        JSON_GET_BOOL(j, "DrawDebugDynamicAO", e->m_DrawDebugDynamicAO);
        JSON_GET_BOOL(j, "DrawDebugDistortion", e->m_DrawDebugDistortion);
        JSON_GET_BOOL(j, "DrawDebugVisibleEntityTypes", e->m_DrawDebugVisibleEntityTypes);
        JSON_GET_BOOL(j, "DrawDebugDof", e->m_DrawDebugDof);
        JSON_GET_BOOL(j, "DrawDebugDofFullscreen", e->m_DrawDebugDofFullscreen);
        JSON_GET_BOOL(j, "DrawDebugMarschnerTextures", e->m_DrawDebugMarschnerTextures);
        JSON_GET_BOOL(j, "DrawBoundingSpheres", e->m_DrawBoundingSpheres);
        JSON_GET_BOOL(j, "DrawSolidBoundingBoxes", e->m_DrawSolidBoundingBoxes);
        JSON_GET_BOOL(j, "DrawLineBoundingBoxes", e->m_DrawLineBoundingBoxes);
        JSON_GET_BOOL(j, "DrawFrustums", e->m_DrawFrustums);
    }
}

#endif // BFVE_GAME_BF4
