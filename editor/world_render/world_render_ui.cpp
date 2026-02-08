#include "world_render.h"
#include "../ui/ui_helpers.h"
#include <imgui.h>

namespace editor::world_render
{
    void renderTab()
    {
        if (!hasCaptured())
        {
            ImGui::TextColored(ImVec4{ 0.8f, 0.8f, 0.2f, 1.0f }, "World render settings not captured. Spawn in-game.");
            return;
        }

        fb::WorldRenderSettings* e = getEdit();
        fb::WorldRenderSettings* o = getOriginal();

        bool enabled = isOverrideEnabled();
        if (ImGui::Checkbox("Enable World Render Overrides", &enabled))
            setOverrideEnabled(enabled);

        ImGui::SameLine();
        if (ImGui::Button("Reset to Original"))
            reset();

        if (!enabled)
        {
            ImGui::TextColored(ImVec4{ 0.7f, 0.7f, 0.3f, 1.0f },
                "Overrides disabled. Enable to apply changes.");
        }

        ImGui::Separator();

        ImGui::BeginChild("WorldRenderSettings", ImVec2{ 0, 0 }, false);

        bool changed = false;

        if (ImGui::TreeNode("Viewport & Quality"))
        {
            changed |= ui::FloatEdit("Viewport Scale", &e->m_ViewportScale, &o->m_ViewportScale, 0.1f, 2.0f);
            changed |= ui::UIntEdit("FXAA Quality", &e->m_FxaaQuality, &o->m_FxaaQuality, 0, 5);
            changed |= ui::IntEdit("FXAA Force Vendor", &e->m_FxaaForceVendor, &o->m_FxaaForceVendor, -1, 3);
            changed |= ui::BoolEdit("FXAA Enable", &e->m_FxaaEnable, &o->m_FxaaEnable);
            changed |= ui::FloatEdit("Cull Screen Area Scale", &e->m_CullScreenAreaScale, &o->m_CullScreenAreaScale);
            changed |= ui::UIntEdit("Multisample Count", &e->m_MultisampleCount, &o->m_MultisampleCount, 1, 8);
            changed |= ui::FloatEdit("Multisample Threshold", &e->m_MultisampleThreshold, &o->m_MultisampleThreshold);
            changed |= ui::BoolEdit("HDR Enable", &e->m_HdrEnable, &o->m_HdrEnable);
            changed |= ui::BoolEdit("Half Res Enable", &e->m_HalfResEnable, &o->m_HalfResEnable);
            changed |= ui::BoolEdit("Bilateral Half Res Composite", &e->m_BilateralHalfResCompositeEnable, &o->m_BilateralHalfResCompositeEnable);
            changed |= ui::BoolEdit("Tiled Half Res Composite", &e->m_TiledHalfResCompositeEnable, &o->m_TiledHalfResCompositeEnable);
            changed |= ui::BoolEdit("Tiled Half Res Stencil Occluders", &e->m_TiledHalfResStencilOccludersEnable, &o->m_TiledHalfResStencilOccludersEnable);
            changed |= ui::BoolEdit("Output Gamma Correction", &e->m_OutputGammaCorrectionEnable, &o->m_OutputGammaCorrectionEnable);
            changed |= ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Shadows"))
        {
            changed |= ui::BoolEdit("Shadowmaps Enable", &e->m_ShadowmapsEnable, &o->m_ShadowmapsEnable);
            changed |= ui::BoolEdit("Generate Shadowmaps", &e->m_GenerateShadowmapsEnable, &o->m_GenerateShadowmapsEnable);
            changed |= ui::BoolEdit("Apply Shadowmaps", &e->m_ApplyShadowmapsEnable, &o->m_ApplyShadowmapsEnable);
            changed |= ui::BoolEdit("Simple Shadowmaps", &e->m_SimpleShadowmapsEnable, &o->m_SimpleShadowmapsEnable);
            changed |= ui::BoolEdit("Transparency Shadowmaps", &e->m_TransparencyShadowmapsEnable, &o->m_TransparencyShadowmapsEnable);
            changed |= ui::BoolEdit("Cloud Shadow", &e->m_CloudShadowEnable, &o->m_CloudShadowEnable);

            ImGui::Separator();
            changed |= ui::UIntEdit("Resolution", &e->m_ShadowmapResolution, &o->m_ShadowmapResolution, 256, 4096);
            changed |= ui::UIntEdit("Quality", &e->m_ShadowmapQuality, &o->m_ShadowmapQuality, 0, 4);
            changed |= ui::UIntEdit("Slice Count", &e->m_ShadowmapSliceCount, &o->m_ShadowmapSliceCount, 1, 8);
            changed |= ui::FloatEdit("View Distance", &e->m_ShadowmapViewDistance, &o->m_ShadowmapViewDistance);
            changed |= ui::BoolEdit("View Distance Scale Enable", &e->m_ShadowmapViewDistanceScaleEnable, &o->m_ShadowmapViewDistanceScaleEnable);
            changed |= ui::FloatEdit("Min Screen Area", &e->m_ShadowMinScreenArea, &o->m_ShadowMinScreenArea);
            changed |= ui::FloatEdit("Shadowmap Min Screen Area", &e->m_ShadowmapMinScreenArea, &o->m_ShadowmapMinScreenArea);
            changed |= ui::FloatEdit("Viewport Scale", &e->m_ShadowViewportScale, &o->m_ShadowViewportScale);
            changed |= ui::FloatEdit("Size Z Scale", &e->m_ShadowmapSizeZScale, &o->m_ShadowmapSizeZScale);
            changed |= ui::FloatEdit("Slice Scheme Weight", &e->m_ShadowmapSliceSchemeWeight, &o->m_ShadowmapSliceSchemeWeight);
            changed |= ui::FloatEdit("First Slice Scale", &e->m_ShadowmapFirstSliceScale, &o->m_ShadowmapFirstSliceScale);
            changed |= ui::FloatEdit("Extrusion Length", &e->m_ShadowmapExtrusionLength, &o->m_ShadowmapExtrusionLength);
            changed |= ui::FloatEdit("Min FOV", &e->m_ShadowmapMinFov, &o->m_ShadowmapMinFov);
            changed |= ui::FloatEdit("Poisson Filter Scale", &e->m_ShadowmapPoissonFilterScale, &o->m_ShadowmapPoissonFilterScale);

            ImGui::Separator();
            changed |= ui::BoolEdit("Fixed Depth", &e->m_ShadowmapFixedDepthEnable, &o->m_ShadowmapFixedDepthEnable);
            changed |= ui::BoolEdit("Fixed Movement", &e->m_ShadowmapFixedMovementEnable, &o->m_ShadowmapFixedMovementEnable);
            changed |= ui::BoolEdit("Cull Volume", &e->m_ShadowmapCullVolumeEnable, &o->m_ShadowmapCullVolumeEnable);
            changed |= ui::BoolEdit("Accum Enable", &e->m_ShadowmapAccumEnable, &o->m_ShadowmapAccumEnable);
            changed |= ui::BoolEdit("Accum Bilinear", &e->m_ShadowmapAccumBilinearEnable, &o->m_ShadowmapAccumBilinearEnable);
            changed |= ui::BoolEdit("Colored Slices", &e->m_ColoredShadowmapSlicesEnable, &o->m_ColoredShadowmapSlicesEnable);
            changed |= ui::BoolEdit("Z Buffer Test", &e->m_ZBufferShadowTestEnable, &o->m_ZBufferShadowTestEnable);
            changed |= ui::BoolEdit("DX 16-Bit", &e->m_DxShadowmap16BitEnable, &o->m_DxShadowmap16BitEnable);

            changed |= ui::IntEdit("Only Slice (debug)", &e->m_OnlyShadowmapSlice, &o->m_OnlyShadowmapSlice, -1, 8);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Motion Blur"))
        {
            changed |= ui::BoolEdit("Enable", &e->m_MotionBlurEnable, &o->m_MotionBlurEnable);
            changed |= ui::BoolEdit("Geometry Pass", &e->m_MotionBlurGeometryPassEnable, &o->m_MotionBlurGeometryPassEnable);
            changed |= ui::BoolEdit("Stencil Pass", &e->m_MotionBlurStencilPassEnable, &o->m_MotionBlurStencilPassEnable);
            changed |= ui::FloatEdit("Scale", &e->m_MotionBlurScale, &o->m_MotionBlurScale, 0.0f, 2.0f);
            changed |= ui::FloatEdit("Max", &e->m_MotionBlurMax, &o->m_MotionBlurMax);
            changed |= ui::FloatEdit("Noise Scale", &e->m_MotionBlurNoiseScale, &o->m_MotionBlurNoiseScale);
            changed |= ui::UIntEdit("Quality", &e->m_MotionBlurQuality, &o->m_MotionBlurQuality, 0, 4);
            changed |= ui::UIntEdit("Max Sample Count", &e->m_MotionBlurMaxSampleCount, &o->m_MotionBlurMaxSampleCount, 1, 64);
            changed |= ui::UIntEdit("Frame Average Count", &e->m_MotionBlurFrameAverageCount, &o->m_MotionBlurFrameAverageCount, 1, 16);
            changed |= ui::FloatEdit("Max Frame Time", &e->m_MotionBlurMaxFrameTime, &o->m_MotionBlurMaxFrameTime);
            changed |= ui::FloatEdit("Force Depth Cutoff", &e->m_ForceMotionBlurDepthCutoff, &o->m_ForceMotionBlurDepthCutoff);
            changed |= ui::FloatEdit("Cutoff Gradient Scale", &e->m_ForceMotionBlurCutoffGradientScale, &o->m_ForceMotionBlurCutoffGradientScale);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lighting - General"))
        {
            changed |= ui::BoolEdit("Outdoor Light Enable", &e->m_OutdoorLightEnable, &o->m_OutdoorLightEnable);
            changed |= ui::BoolEdit("Outdoor Key Light", &e->m_OutdoorKeyLightEnable, &o->m_OutdoorKeyLightEnable);
            changed |= ui::BoolEdit("Outdoor Sky Light", &e->m_OutdoorSkyLightEnable, &o->m_OutdoorSkyLightEnable);
            changed |= ui::BoolEdit("Outdoor Light Specular", &e->m_OutdoorLightSpecularEnable, &o->m_OutdoorLightSpecularEnable);
            changed |= ui::BoolEdit("Outdoor Light Tiling", &e->m_OutdoorLightTilingEnable, &o->m_OutdoorLightTilingEnable);
            changed |= ui::BoolEdit("Outdoor Light Tile Render", &e->m_OutdoorLightTileRenderEnable, &o->m_OutdoorLightTileRenderEnable);
            changed |= ui::BoolEdit("Outdoor Light Tile Blend", &e->m_OutdoorLightTileBlendEnable, &o->m_OutdoorLightTileBlendEnable);
            changed |= ui::BoolEdit("Outdoor Light Tile Simple Shader", &e->m_OutdoorLightTileSimpleShaderEnable, &o->m_OutdoorLightTileSimpleShaderEnable);
            changed |= ui::UIntEdit("Outdoor Light Tile Batch Count", &e->m_OutdoorLightTileBatchCount, &o->m_OutdoorLightTileBatchCount);

            ImGui::Separator();
            changed |= ui::FloatEdit("Intensity Scale", &e->m_LightIntensityScale, &o->m_LightIntensityScale);
            changed |= ui::FloatEdit("Force Intensity", &e->m_LightForceIntensity, &o->m_LightForceIntensity);
            changed |= ui::FloatEdit("Radius Scale", &e->m_LightRadiusScale, &o->m_LightRadiusScale);
            changed |= ui::FloatEdit("Attenuation Threshold", &e->m_LightAttenuationThreshold, &o->m_LightAttenuationThreshold);
            changed |= ui::BoolEdit("Attenuation Threshold Enable", &e->m_LightAttenuationThresholdEnable, &o->m_LightAttenuationThresholdEnable);
            changed |= ui::BoolEdit("Width Enable", &e->m_LightWidthEnable, &o->m_LightWidthEnable);
            changed |= ui::BoolEdit("Intensity Normalization", &e->m_LightIntensityNormalizationEnable, &o->m_LightIntensityNormalizationEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Specular Lighting", &e->m_SpecularLightingEnable, &o->m_SpecularLightingEnable);
            changed |= ui::BoolEdit("Skin Lighting", &e->m_SkinLightingEnable, &o->m_SkinLightingEnable);
            changed |= ui::BoolEdit("Translucency Lighting", &e->m_TranslucencyLightingEnable, &o->m_TranslucencyLightingEnable);
            changed |= ui::BoolEdit("Emissive Enable", &e->m_EmissiveEnable, &o->m_EmissiveEnable);
            changed |= ui::BoolEdit("Unlit Enable", &e->m_UnlitEnable, &o->m_UnlitEnable);
            changed |= ui::BoolEdit("Draw Light Sources", &e->m_DrawLightSources, &o->m_DrawLightSources);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lighting - LOD"))
        {
            changed |= ui::FloatEdit("LOD Specular Fade Start", &e->m_LightLodSpecularFadeAreaStart, &o->m_LightLodSpecularFadeAreaStart);
            changed |= ui::FloatEdit("LOD Specular Fade End", &e->m_LightLodSpecularFadeAreaEnd, &o->m_LightLodSpecularFadeAreaEnd);
            changed |= ui::FloatEdit("LOD Radius Factor", &e->m_LightLodRadiusFactor, &o->m_LightLodRadiusFactor);
            changed |= ui::FloatEdit("LOD Fade Area", &e->m_LightLodFadeArea, &o->m_LightLodFadeArea);
            changed |= ui::FloatEdit("LOD Min Area", &e->m_LightLodMinArea, &o->m_LightLodMinArea);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lighting - Culling"))
        {
            changed |= ui::BoolEdit("Cull Enable", &e->m_LightCullEnable, &o->m_LightCullEnable);
            changed |= ui::BoolEdit("Depth Cull", &e->m_LightDepthCullEnable, &o->m_LightDepthCullEnable);
            changed |= ui::BoolEdit("Normal Cull", &e->m_LightNormalCullEnable, &o->m_LightNormalCullEnable);
            changed |= ui::BoolEdit("Cone Cull", &e->m_LightConeCullEnable, &o->m_LightConeCullEnable);
            changed |= ui::FloatEdit("Stencil Min Area", &e->m_LightStencilMinArea, &o->m_LightStencilMinArea);
            changed |= ui::BoolEdit("Stencil Method", &e->m_LightStencilMethodEnable, &o->m_LightStencilMethodEnable);
            changed |= ui::BoolEdit("Volume Method", &e->m_LightVolumeMethodEnable, &o->m_LightVolumeMethodEnable);
            changed |= ui::BoolEdit("Volume Depth Test", &e->m_LightVolumeDepthTestEnable, &o->m_LightVolumeDepthTestEnable);
            changed |= ui::FloatEdit("Tile Min Area", &e->m_LightTileMinArea, &o->m_LightTileMinArea);
            changed |= ui::BoolEdit("Tile Overlay", &e->m_LightTileOverlayEnable, &o->m_LightTileOverlayEnable);
            changed |= ui::UIntEdit("Overdraw Max Layer Count", &e->m_LightOverdrawMaxLayerCount, &o->m_LightOverdrawMaxLayerCount);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Light Types"))
        {
            changed |= ui::BoolEdit("Point Lights", &e->m_PointLightsEnable, &o->m_PointLightsEnable);
            changed |= ui::UIntEdit("Max Point Light Count", &e->m_MaxPointLightCount, &o->m_MaxPointLightCount, 0, 256);

            ImGui::Separator();
            changed |= ui::BoolEdit("Spot Lights", &e->m_SpotLightsEnable, &o->m_SpotLightsEnable);
            changed |= ui::UIntEdit("Max Spot Light Count", &e->m_MaxSpotLightCount, &o->m_MaxSpotLightCount, 0, 64);
            changed |= ui::UIntEdit("Max Spot Shadow Count", &e->m_MaxSpotLightShadowCount, &o->m_MaxSpotLightShadowCount, 0, 32);
            changed |= ui::UIntEdit("Spot Shadowmap Resolution", &e->m_SpotLightShadowmapResolution, &o->m_SpotLightShadowmapResolution, 128, 2048);
            changed |= ui::UIntEdit("Spot Shadowmap Quality", &e->m_SpotLightShadowmapQuality, &o->m_SpotLightShadowmapQuality, 0, 4);
            changed |= ui::FloatEdit("Spot Near Plane", &e->m_SpotLightNearPlane, &o->m_SpotLightNearPlane);
            changed |= ui::FloatEdit("Spot Poisson Filter Scale", &e->m_SpotLightShadowmapPoissonFilterScale, &o->m_SpotLightShadowmapPoissonFilterScale);
            changed |= ui::BoolEdit("Spot Shadowmap Enable", &e->m_SpotLightShadowmapEnable, &o->m_SpotLightShadowmapEnable);
            changed |= ui::BoolEdit("DX Spot Shadowmap 16-Bit", &e->m_DxSpotLightShadowmap16BitEnable, &o->m_DxSpotLightShadowmap16BitEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Line Lights", &e->m_LineLightsEnable, &o->m_LineLightsEnable);
            changed |= ui::BoolEdit("Cone Lights", &e->m_ConeLightsEnable, &o->m_ConeLightsEnable);
            changed |= ui::BoolEdit("Light Probes", &e->m_LightProbesEnable, &o->m_LightProbesEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Sky"))
        {
            changed |= ui::BoolEdit("Sky Enable", &e->m_SkyEnable, &o->m_SkyEnable);
            changed |= ui::BoolEdit("Sky Fog Enable", &e->m_SkyFogEnable, &o->m_SkyFogEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Sky Envmap Enable", &e->m_SkyEnvmapEnable, &o->m_SkyEnvmapEnable);
            changed |= ui::BoolEdit("Sky Envmap Update", &e->m_SkyEnvmapUpdateEnable, &o->m_SkyEnvmapUpdateEnable);
            changed |= ui::BoolEdit("Sky Envmap Force Update", &e->m_SkyEnvmapForceUpdateEnable, &o->m_SkyEnvmapForceUpdateEnable);
            changed |= ui::BoolEdit("Sky Envmap Mipmap Gen", &e->m_SkyEnvmapMipmapGenEnable, &o->m_SkyEnvmapMipmapGenEnable);
            changed |= ui::BoolEdit("Sky Envmap 8-Bit Texture", &e->m_SkyEnvmapUse8BitTexture, &o->m_SkyEnvmapUse8BitTexture);
            changed |= ui::UIntEdit("Sky Envmap Resolution", &e->m_SkyEnvmapResolution, &o->m_SkyEnvmapResolution, 16, 512);
            changed |= ui::UIntEdit("Sky Envmap Sides Per Frame", &e->m_SkyEnvmapSidesPerFrameCount, &o->m_SkyEnvmapSidesPerFrameCount, 1, 6);
            changed |= ui::FloatEdit("Sky Envmap Filter Width", &e->m_SkyEnvmapFilterWidth, &o->m_SkyEnvmapFilterWidth);
            changed |= ui::EnumCombo<fb::MipmapFilterMode>("Sky Envmap Filter Mode", reinterpret_cast<int*>(&e->m_SkyEnvmapFilterMode), reinterpret_cast<const int*>(&o->m_SkyEnvmapFilterMode));
            changed |= ui::BoolEdit("Sky Visibility Envmap Scaling", &e->m_SkyVisibilityEnvmapScalingEnable, &o->m_SkyVisibilityEnvmapScalingEnable);
            changed |= ui::IntEdit("Debug Sky Envmap Mip Level", &e->m_DrawDebugSkyEnvmapMipLevel, &o->m_DrawDebugSkyEnvmapMipLevel, -1, 10);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Dynamic Environment Map"))
        {
            changed |= ui::BoolEdit("Enable", &e->m_DynamicEnvmapEnable, &o->m_DynamicEnvmapEnable);
            changed |= ui::BoolEdit("Lighting Enable", &e->m_DynamicEnvmapLightingEnable, &o->m_DynamicEnvmapLightingEnable);
            changed |= ui::BoolEdit("Mipmap Gen", &e->m_DynamicEnvmapMipmapGenEnable, &o->m_DynamicEnvmapMipmapGenEnable);
            changed |= ui::UIntEdit("Resolution", &e->m_DynamicEnvmapResolution, &o->m_DynamicEnvmapResolution, 16, 512);
            changed |= ui::FloatEdit("Filter Width", &e->m_DynamicEnvmapFilterWidth, &o->m_DynamicEnvmapFilterWidth);
            changed |= ui::EnumCombo<fb::MipmapFilterMode>("Dynamic Mode", reinterpret_cast<int*>(&e->m_DynamicEnvmapFilterMode), reinterpret_cast<const int*>(&o->m_DynamicEnvmapFilterMode));
            changed |= ui::Vec3Edit("Default Position", &e->m_DynamicEnvmapDefaultPosition, &o->m_DynamicEnvmapDefaultPosition);
            changed |= ui::IntEdit("Debug Mip Level", &e->m_DrawDebugDynamicEnvmapMipLevel, &o->m_DrawDebugDynamicEnvmapMipLevel, -1, 10);

            ImGui::Separator();
            changed |= ui::BoolEdit("Static Envmap Enable", &e->m_StaticEnvmapEnable, &o->m_StaticEnvmapEnable);
            changed |= ui::BoolEdit("Custom Envmap Enable", &e->m_CustomEnvmapEnable, &o->m_CustomEnvmapEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Reflections"))
        {
            changed |= ui::BoolEdit("Planar Reflection Enable", &e->m_PlanarReflectionEnable, &o->m_PlanarReflectionEnable);
            changed |= ui::BoolEdit("Planar Reflection Blur", &e->m_PlanarReflectionBlur, &o->m_PlanarReflectionBlur);
            changed |= ui::UIntEdit("Planar Reflection Width", &e->m_PlanarReflectionWidth, &o->m_PlanarReflectionWidth, 128, 2048);
            changed |= ui::UIntEdit("Planar Reflection Height", &e->m_PlanarReflectionHeight, &o->m_PlanarReflectionHeight, 128, 2048);
            changed |= ui::FloatEdit("Planar Reflection Cull FOV", &e->m_PlanarReflectionCullFOV, &o->m_PlanarReflectionCullFOV);
            changed |= ui::FloatEdit("Temp Planar Reflection Y", &e->m_TempPlanarReflectionY, &o->m_TempPlanarReflectionY);
            changed |= ui::BoolEdit("Debug Cull Mode", &e->m_PlanarReflectionDebugCullMode, &o->m_PlanarReflectionDebugCullMode);
            changed |= ui::BoolEdit("Freeze Cull Frustum", &e->m_FreezePlanarReflectionCullFrustum, &o->m_FreezePlanarReflectionCullFrustum);
            changed |= ui::UIntEdit("Reflection Envmap Size", &e->m_ReflectionEnvmapSize, &o->m_ReflectionEnvmapSize, 16, 512);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Subsurface Scattering"))
        {
            changed |= ui::Vec3Edit("Color", &e->m_SubSurfaceColor, &o->m_SubSurfaceColor, true);
            changed |= ui::FloatEdit("Rolloff Key Light", &e->m_SubSurfaceRolloffKeyLight, &o->m_SubSurfaceRolloffKeyLight);
            changed |= ui::FloatEdit("Rolloff Local Light", &e->m_SubSurfaceRolloffLocalLight, &o->m_SubSurfaceRolloffLocalLight);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Decals & Volumes"))
        {
            changed |= ui::BoolEdit("Decal Volume Enable", &e->m_DecalVolumeEnable, &o->m_DecalVolumeEnable);
            changed |= ui::UIntEdit("Max Decal Volume Count", &e->m_MaxDecalVolumeCount, &o->m_MaxDecalVolumeCount, 0, 256);
            changed |= ui::FloatEdit("Decal Volume Scale", &e->m_DecalVolumeScale, &o->m_DecalVolumeScale);
            changed |= ui::UIntEdit("Max Destruction Volume Count", &e->m_MaxDestructionVolumeCount, &o->m_MaxDestructionVolumeCount, 0, 256);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Render Passes"))
        {
            changed |= ui::BoolEdit("Z Pass", &e->m_ZPassEnable, &o->m_ZPassEnable);
            changed |= ui::BoolEdit("Main Opaque Z Pass", &e->m_MainOpaqueZPassEnable, &o->m_MainOpaqueZPassEnable);
            changed |= ui::BoolEdit("Occluder Mesh Z Prepass", &e->m_OccluderMeshZPrepassEnable, &o->m_OccluderMeshZPrepassEnable);
            changed |= ui::BoolEdit("Occluder Mesh Z Prepass Debug", &e->m_OccluderMeshZPrepassDebugEnable, &o->m_OccluderMeshZPrepassDebugEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Foreground Enable", &e->m_ForegroundEnable, &o->m_ForegroundEnable);
            changed |= ui::BoolEdit("Foreground Z Pass", &e->m_ForegroundZPassEnable, &o->m_ForegroundZPassEnable);
            changed |= ui::BoolEdit("Foreground Depth Clear", &e->m_ForegroundDepthClearEnable, &o->m_ForegroundDepthClearEnable);
            changed |= ui::BoolEdit("Foreground As Main", &e->m_ForegroundAsMainEnable, &o->m_ForegroundAsMainEnable);
            changed |= ui::BoolEdit("Foreground Transparent", &e->m_ForegroundTransparentEnable, &o->m_ForegroundTransparentEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Draw Transparent", &e->m_DrawTransparent, &o->m_DrawTransparent);
            changed |= ui::BoolEdit("Draw Transparent Decal", &e->m_DrawTransparentDecal, &o->m_DrawTransparentDecal);
            changed |= ui::BoolEdit("Screen Effect Enable", &e->m_ScreenEffectEnable, &o->m_ScreenEffectEnable);
            changed |= ui::BoolEdit("Final Post Enable", &e->m_FinalPostEnable, &o->m_FinalPostEnable);
            changed |= ui::BoolEdit("View FX Enable", &e->m_ViewFxEnable, &o->m_ViewFxEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lens Flares"))
        {
            changed |= ui::BoolEdit("Enable", &e->m_LensFlaresEnable, &o->m_LensFlaresEnable);
            changed |= ui::BoolEdit("Half Res", &e->m_HalfResLensFlaresEnable, &o->m_HalfResLensFlaresEnable);
            changed |= ui::BoolEdit("Occlusion Enable", &e->m_LensFlareOcclusionEnable, &o->m_LensFlareOcclusionEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("GBuffer"))
        {
            changed |= ui::BoolEdit("Clear Enable", &e->m_GBufferClearEnable, &o->m_GBufferClearEnable);
            changed |= ui::BoolEdit("Alpha Test Simple", &e->m_GBufferAlphaTestSimpleEnable, &o->m_GBufferAlphaTestSimpleEnable);
            changed |= ui::BoolEdit("DX Light 16-Bit", &e->m_DxGBufferLight16BitEnable, &o->m_DxGBufferLight16BitEnable);
            changed |= ui::BoolEdit("DX Normal 16-Bit", &e->m_DxGBufferNormal16BitEnable, &o->m_DxGBufferNormal16BitEnable);
            changed |= ui::BoolEdit("DX Linear Depth 32-Bit", &e->m_DxLinearDepth32BitFormatEnable, &o->m_DxLinearDepth32BitFormatEnable);
            changed |= ui::BoolEdit("DX Deferred CS Path", &e->m_DxDeferredCsPathEnable, &o->m_DxDeferredCsPathEnable);
            changed |= ui::UIntEdit("Test Count", &e->m_GBufferTestCount, &o->m_GBufferTestCount);
            changed |= ui::EnumCombo<fb::ShaderGBufferLayout>("Layout", reinterpret_cast<int*>(&e->m_GBufferLayout), reinterpret_cast<const int*>(&o->m_GBufferLayout));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Dynamic AO"))
        {
            changed |= ui::BoolEdit("Override Dynamic AO", &e->m_OverrideDynamicAO, &o->m_OverrideDynamicAO);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Misc Rendering"))
        {
            changed |= ui::BoolEdit("Wireframe", &e->m_WireframeEnable, &o->m_WireframeEnable);
            changed |= ui::BoolEdit("Opaque Sort By Solution", &e->m_OpaqueSortBySolutionEnable, &o->m_OpaqueSortBySolutionEnable);
            changed |= ui::BoolEdit("Generic Entity Renderer", &e->m_GenericEntityRendererEnable, &o->m_GenericEntityRendererEnable);
            changed |= ui::UIntEdit("Generic Entity Max Visible", &e->m_GenericEntityMaxVisibleEntityCount, &o->m_GenericEntityMaxVisibleEntityCount);
            changed |= ui::BoolEdit("Setup Job Enable", &e->m_SetupJobEnable, &o->m_SetupJobEnable);
            changed |= ui::BoolEdit("Dynamic Cull Z Buffer Test", &e->m_DynamicCullZBufferTestEnable, &o->m_DynamicCullZBufferTestEnable);
            changed |= ui::BoolEdit("Dynamic Cull Draw Occluded Boxes", &e->m_DynamicCullDrawOccludedBoxesEnable, &o->m_DynamicCullDrawOccludedBoxesEnable);
            changed |= ui::EnumCombo<fb::WorldViewMode>("View Mode", reinterpret_cast<int*>(&e->m_ViewMode), reinterpret_cast<const int*>(&o->m_ViewMode));
            changed |= ui::IntEdit("Only Tile Index", &e->m_OnlyTileIndex, &o->m_OnlyTileIndex, -1, 64);
            changed |= ui::IntEdit("Only Light Tile X", &e->m_OnlyLightTileX, &o->m_OnlyLightTileX, -1, 64);
            changed |= ui::IntEdit("Only Light Tile Y", &e->m_OnlyLightTileY, &o->m_OnlyLightTileY, -1, 64);
            changed |= ui::IntEdit("Only Light Tile Index", &e->m_OnlyLightTileIndex, &o->m_OnlyLightTileIndex, -1, 256);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Debug Visualization"))
        {
            if (ImGui::TreeNode("Buffers"))
            {
                changed |= ui::BoolEdit("GBuffer", &e->m_DrawDebugGBuffer, &o->m_DrawDebugGBuffer);
                changed |= ui::BoolEdit("Z Buffer", &e->m_DrawDebugZBufferEnable, &o->m_DrawDebugZBufferEnable);
                changed |= ui::BoolEdit("Velocity Buffer", &e->m_DrawDebugVelocityBuffer, &o->m_DrawDebugVelocityBuffer);
                changed |= ui::BoolEdit("Multisample Classify", &e->m_DrawDebugMultisampleClassify, &o->m_DrawDebugMultisampleClassify);
                changed |= ui::BoolEdit("Half Res Environment", &e->m_DrawDebugHalfResEnvironment, &o->m_DrawDebugHalfResEnvironment);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Shadows"))
            {
                changed |= ui::BoolEdit("Shadowmaps", &e->m_DrawDebugShadowmaps, &o->m_DrawDebugShadowmaps);
                changed |= ui::BoolEdit("Quarter Shadowmaps", &e->m_DrawDebugQuarterShadowmaps, &o->m_DrawDebugQuarterShadowmaps);
                changed |= ui::BoolEdit("Trans Shadowmap", &e->m_DrawDebugTransShadowmap, &o->m_DrawDebugTransShadowmap);
                changed |= ui::BoolEdit("Spot Light Shadowmaps", &e->m_DrawDebugSpotLightShadowmaps, &o->m_DrawDebugSpotLightShadowmaps);
                changed |= ui::BoolEdit("Shadow Frustums", &e->m_DrawShadowFrustums, &o->m_DrawShadowFrustums);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Lights"))
            {
                changed |= ui::BoolEdit("Light Sources", &e->m_DrawDebugLightSources, &o->m_DrawDebugLightSources);
                changed |= ui::BoolEdit("Light Shadow Sources", &e->m_DrawDebugLightShadowSources, &o->m_DrawDebugLightShadowSources);
                changed |= ui::BoolEdit("Light Stats", &e->m_DrawDebugLightStats, &o->m_DrawDebugLightStats);
                changed |= ui::BoolEdit("Light Tiles", &e->m_DrawDebugLightTiles, &o->m_DrawDebugLightTiles);
                changed |= ui::BoolEdit("Light Tile Volumes", &e->m_DrawDebugLightTileVolumes, &o->m_DrawDebugLightTileVolumes);
                changed |= ui::BoolEdit("Light Tile FB Grid", &e->m_DrawDebugLightTileFbGrid, &o->m_DrawDebugLightTileFbGrid);
                changed |= ui::BoolEdit("Light Tile Grid", &e->m_DrawDebugLightTileGrid, &o->m_DrawDebugLightTileGrid);
                changed |= ui::BoolEdit("Light Tile Sources", &e->m_DrawDebugLightTileSources, &o->m_DrawDebugLightTileSources);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Environment"))
            {
                changed |= ui::BoolEdit("Dynamic AO", &e->m_DrawDebugDynamicAO, &o->m_DrawDebugDynamicAO);
                changed |= ui::BoolEdit("Dynamic Envmap", &e->m_DrawDebugDynamicEnvmap, &o->m_DrawDebugDynamicEnvmap);
                changed |= ui::BoolEdit("Sky Envmap", &e->m_DrawDebugSkyEnvmap, &o->m_DrawDebugSkyEnvmap);
                changed |= ui::BoolEdit("Planar Reflection", &e->m_DrawDebugPlanarReflection, &o->m_DrawDebugPlanarReflection);
                changed |= ui::BoolEdit("Planar Reflection Cull Frustum", &e->m_DrawDebugPlanarReflectionCullFrustum, &o->m_DrawDebugPlanarReflectionCullFrustum);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Volumes & Misc"))
            {
                changed |= ui::BoolEdit("Decal Volumes", &e->m_DrawDebugDecalVolumes, &o->m_DrawDebugDecalVolumes);
                changed |= ui::BoolEdit("Destruction Volumes", &e->m_DrawDebugDestructionVolumes, &o->m_DrawDebugDestructionVolumes);
                changed |= ui::BoolEdit("Lens Flares", &e->m_DrawDebugLensFlares, &o->m_DrawDebugLensFlares);
                changed |= ui::BoolEdit("Lens Flare Occluders", &e->m_DrawDebugLensFlareOccluders, &o->m_DrawDebugLensFlareOccluders);
                changed |= ui::BoolEdit("World Occlusions", &e->m_DrawDebugWorldOcclusions, &o->m_DrawDebugWorldOcclusions);
                changed |= ui::BoolEdit("Visible Entity Types", &e->m_DrawDebugVisibleEntityTypes, &o->m_DrawDebugVisibleEntityTypes);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Bounding Geometry"))
            {
                changed |= ui::BoolEdit("Bounding Spheres", &e->m_DrawBoundingSpheres, &o->m_DrawBoundingSpheres);
                changed |= ui::BoolEdit("Solid Bounding Boxes", &e->m_DrawSolidBoundingBoxes, &o->m_DrawSolidBoundingBoxes);
                changed |= ui::BoolEdit("Line Bounding Boxes", &e->m_DrawLineBoundingBoxes, &o->m_DrawLineBoundingBoxes);
                changed |= ui::BoolEdit("Frustums", &e->m_DrawFrustums, &o->m_DrawFrustums);
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        if (changed)
            world_render::apply();

        ImGui::EndChild();
    }
}