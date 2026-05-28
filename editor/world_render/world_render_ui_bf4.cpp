#include "world_render.h"
#include "../ui/ui_helpers.h"
#include <imgui.h>

#if defined(BFVE_GAME_BF4)

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

        ImGui::BeginChild("WorldRenderSettingsBF4", ImVec2{ 0, 0 }, false);

        bool changed = false;

        if (ImGui::TreeNode("Viewport & Quality"))
        {
            changed |= ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
            changed |= ui::FloatEdit("Viewport Scale", &e->m_ViewportScale, &o->m_ViewportScale, 0.1f, 2.0f);
            changed |= ui::FloatEdit("Cull Screen Area Scale", &e->m_CullScreenAreaScale, &o->m_CullScreenAreaScale);
            changed |= ui::UIntEdit("Multisample Count", &e->m_MultisampleCount, &o->m_MultisampleCount, 1, 8);
            changed |= ui::FloatEdit("Multisample Threshold", &e->m_MultisampleThreshold, &o->m_MultisampleThreshold);
            changed |= ui::UIntEdit("HUD Texture Width", &e->m_HudTextureWidth, &o->m_HudTextureWidth);
            changed |= ui::UIntEdit("HUD Texture Height", &e->m_HudTextureHeight, &o->m_HudTextureHeight);
            changed |= ui::EnumCombo<fb::WorldViewMode>("View Mode",
                reinterpret_cast<int*>(&e->m_ViewMode),
                reinterpret_cast<const int*>(&o->m_ViewMode));

            ImGui::Separator();
            changed |= ui::BoolEdit("HDR", &e->m_HdrEnable, &o->m_HdrEnable);
            changed |= ui::BoolEdit("Fast HDR", &e->m_FastHdrEnable, &o->m_FastHdrEnable);
            changed |= ui::BoolEdit("Half Res", &e->m_HalfResEnable, &o->m_HalfResEnable);
            changed |= ui::BoolEdit("Force Full Res", &e->m_ForceFullResEnable, &o->m_ForceFullResEnable);
            changed |= ui::BoolEdit("Half Res Lens Flares", &e->m_HalfResLensFlaresEnable, &o->m_HalfResLensFlaresEnable);
            changed |= ui::BoolEdit("Bilateral Half Res Composite", &e->m_BilateralHalfResCompositeEnable, &o->m_BilateralHalfResCompositeEnable);
            changed |= ui::BoolEdit("Tiled Half Res Composite", &e->m_TiledHalfResCompositeEnable, &o->m_TiledHalfResCompositeEnable);
            changed |= ui::BoolEdit("Tiled Half Res Stencil Occluders", &e->m_TiledHalfResStencilOccludersEnable, &o->m_TiledHalfResStencilOccludersEnable);
            changed |= ui::BoolEdit("Read Only Depth", &e->m_ReadOnlyDepthEnable, &o->m_ReadOnlyDepthEnable);
            changed |= ui::BoolEdit("Output Gamma Correction", &e->m_OutputGammaCorrectionEnable, &o->m_OutputGammaCorrectionEnable);
            changed |= ui::BoolEdit("DX Linear Depth 32-Bit", &e->m_DxLinearDepth32BitFormatEnable, &o->m_DxLinearDepth32BitFormatEnable);
            changed |= ui::BoolEdit("DX Depth Buffer 32-Bit", &e->m_DxDepthBuffer32BitEnable, &o->m_DxDepthBuffer32BitEnable);

            ImGui::Separator();
            changed |= ui::EnumCombo<fb::PostProcessAAMode>("Post-Process AA Mode",
                reinterpret_cast<int*>(&e->m_PostProcessAntialiasingMode),
                reinterpret_cast<const int*>(&o->m_PostProcessAntialiasingMode));
            changed |= ui::IntEdit("FXAA Force Vendor", &e->m_FxaaForceVendor, &o->m_FxaaForceVendor, -1, 3);
            changed |= ui::EnumCombo<fb::ScaleResampleMode>("Render Scale Resample Mode",
                reinterpret_cast<int*>(&e->m_RenderScaleResampleMode),
                reinterpret_cast<const int*>(&o->m_RenderScaleResampleMode));
            changed |= ui::BoolEdit("Render Scale Resample Enable", &e->m_RenderScaleResampleEnable, &o->m_RenderScaleResampleEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Simple Rendering", &e->m_SimpleRenderingEnable, &o->m_SimpleRenderingEnable);
            changed |= ui::BoolEdit("View FX", &e->m_ViewFxEnable, &o->m_ViewFxEnable);
            changed |= ui::BoolEdit("Generic Entity Renderer", &e->m_GenericEntityRendererEnable, &o->m_GenericEntityRendererEnable);
            changed |= ui::UIntEdit("Generic Entity Max Visible", &e->m_GenericEntityMaxVisibleEntityCount, &o->m_GenericEntityMaxVisibleEntityCount);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Overlay"))
        {
            changed |= ui::BoolEdit("Overlay Enable", &e->m_OverlayEnable, &o->m_OverlayEnable);
            changed |= ui::Vec4Edit("Overlay Color", &e->m_OverlayColor, &o->m_OverlayColor, true);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("SMAA"))
        {
            changed |= ui::BoolEdit("Velocity Reprojection", &e->m_SmaaVelocityReprojectionEnable, &o->m_SmaaVelocityReprojectionEnable);
            changed |= ui::BoolEdit("Use Stencil", &e->m_SmaaUseStencil, &o->m_SmaaUseStencil);
            changed |= ui::BoolEdit("Predicated Thresholding", &e->m_SmaaPredicatedThresholdingEnable, &o->m_SmaaPredicatedThresholdingEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("GBuffer"))
        {
            changed |= ui::UIntEdit("Layout", &e->m_GBufferLayout, &o->m_GBufferLayout, 0, 4);
            changed |= ui::UIntEdit("Test Count", &e->m_GBufferTestCount, &o->m_GBufferTestCount);
            changed |= ui::FloatEdit("Alpha Test Simple Smoothness", &e->m_GBufferAlphaTestSimpleSmoothness, &o->m_GBufferAlphaTestSimpleSmoothness);
            changed |= ui::FloatEdit("Force Smoothness", &e->m_GBufferForceSmoothness, &o->m_GBufferForceSmoothness);
            changed |= ui::FloatEdit("Force Specular Albedo", &e->m_GBufferForceSpecularAlbedo, &o->m_GBufferForceSpecularAlbedo);
            changed |= ui::BoolEdit("Clear Enable", &e->m_GBufferClearEnable, &o->m_GBufferClearEnable);
            changed |= ui::BoolEdit("Alpha Test Simple", &e->m_GBufferAlphaTestSimpleEnable, &o->m_GBufferAlphaTestSimpleEnable);
            changed |= ui::BoolEdit("DX Light 16-Bit", &e->m_DxGBufferLight16BitEnable, &o->m_DxGBufferLight16BitEnable);
            changed |= ui::BoolEdit("DX Normal 16-Bit", &e->m_DxGBufferNormal16BitEnable, &o->m_DxGBufferNormal16BitEnable);
            changed |= ui::BoolEdit("Gen4a ESRAM", &e->m_Gen4aEsramEnable, &o->m_Gen4aEsramEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Shadows"))
        {
            changed |= ui::BoolEdit("Shadowmaps Enable", &e->m_ShadowmapsEnable, &o->m_ShadowmapsEnable);
            changed |= ui::BoolEdit("Generate Shadowmaps", &e->m_GenerateShadowmapsEnable, &o->m_GenerateShadowmapsEnable);
            changed |= ui::BoolEdit("Apply Shadowmaps", &e->m_ApplyShadowmapsEnable, &o->m_ApplyShadowmapsEnable);
            changed |= ui::BoolEdit("Simple Shadowmaps", &e->m_SimpleShadowmapsEnable, &o->m_SimpleShadowmapsEnable);
            changed |= ui::BoolEdit("Transparency Shadowmaps", &e->m_TransparencyShadowmapsEnable, &o->m_TransparencyShadowmapsEnable);
            changed |= ui::BoolEdit("Transparency Shadowmaps Half-Res", &e->m_TransparencyShadowmapsHalfRes, &o->m_TransparencyShadowmapsHalfRes);
            changed |= ui::BoolEdit("Cloud Shadow", &e->m_CloudShadowEnable, &o->m_CloudShadowEnable);

            ImGui::Separator();
            changed |= ui::UIntEdit("Resolution", &e->m_ShadowmapResolution, &o->m_ShadowmapResolution, 256, 4096);
            changed |= ui::UIntEdit("Quality", &e->m_ShadowmapQuality, &o->m_ShadowmapQuality, 0, 4);
            changed |= ui::UIntEdit("Slice Count", &e->m_ShadowmapSliceCount, &o->m_ShadowmapSliceCount, 1, 8);
            changed |= ui::FloatEdit("View Distance", &e->m_ShadowmapViewDistance, &o->m_ShadowmapViewDistance);
            changed |= ui::BoolEdit("View Distance Scale Enable", &e->m_ShadowmapViewDistanceScaleEnable, &o->m_ShadowmapViewDistanceScaleEnable);
            changed |= ui::FloatEdit("Viewport Scale", &e->m_ShadowViewportScale, &o->m_ShadowViewportScale);
            changed |= ui::FloatEdit("Size Z Scale", &e->m_ShadowmapSizeZScale, &o->m_ShadowmapSizeZScale);
            changed |= ui::FloatEdit("Slice Scheme Weight", &e->m_ShadowmapSliceSchemeWeight, &o->m_ShadowmapSliceSchemeWeight);
            changed |= ui::FloatEdit("First Slice Scale", &e->m_ShadowmapFirstSliceScale, &o->m_ShadowmapFirstSliceScale);
            changed |= ui::FloatEdit("Extrusion Length", &e->m_ShadowmapExtrusionLength, &o->m_ShadowmapExtrusionLength);
            changed |= ui::FloatEdit("Min FOV", &e->m_ShadowmapMinFov, &o->m_ShadowmapMinFov);
            changed |= ui::FloatEdit("Poisson Filter Scale", &e->m_ShadowmapPoissonFilterScale, &o->m_ShadowmapPoissonFilterScale);
            changed |= ui::FloatEdit("Transition Blend Amount", &e->m_ShadowmapTransitionBlendAmount, &o->m_ShadowmapTransitionBlendAmount);
            changed |= ui::BoolEdit("Transition Blend Enable", &e->m_ShadowmapTransitionBlendEnable, &o->m_ShadowmapTransitionBlendEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Fixed Depth", &e->m_ShadowmapFixedDepthEnable, &o->m_ShadowmapFixedDepthEnable);
            changed |= ui::BoolEdit("Fixed Movement", &e->m_ShadowmapFixedMovementEnable, &o->m_ShadowmapFixedMovementEnable);
            changed |= ui::BoolEdit("Cull Volume", &e->m_ShadowmapCullVolumeEnable, &o->m_ShadowmapCullVolumeEnable);
            changed |= ui::BoolEdit("Accum Enable", &e->m_ShadowmapAccumEnable, &o->m_ShadowmapAccumEnable);
            changed |= ui::BoolEdit("Accum Bilinear", &e->m_ShadowmapAccumBilinearEnable, &o->m_ShadowmapAccumBilinearEnable);
            changed |= ui::BoolEdit("Z-Buffer Shadow Test", &e->m_ZBufferShadowTestEnable, &o->m_ZBufferShadowTestEnable);
            changed |= ui::BoolEdit("DX 16-Bit", &e->m_DxShadowmap16BitEnable, &o->m_DxShadowmap16BitEnable);
            changed |= ui::IntEdit("Only Slice (debug)", &e->m_OnlyShadowmapSlice, &o->m_OnlyShadowmapSlice, -1, 8);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Motion Blur"))
        {
            changed |= ui::BoolEdit("Enable", &e->m_MotionBlurEnable, &o->m_MotionBlurEnable);
            changed |= ui::BoolEdit("Force On", &e->m_MotionBlurForceOn, &o->m_MotionBlurForceOn);
            changed |= ui::BoolEdit("Optimal Stable Velocity Formula", &e->m_MotionBlurOptimalStableVelocityFormula, &o->m_MotionBlurOptimalStableVelocityFormula);
            changed |= ui::BoolEdit("Geometry Pass", &e->m_MotionBlurGeometryPassEnable, &o->m_MotionBlurGeometryPassEnable);
            changed |= ui::BoolEdit("Stencil Pass", &e->m_MotionBlurStencilPassEnable, &o->m_MotionBlurStencilPassEnable);
            changed |= ui::BoolEdit("Background Pass", &e->m_MotionBlurBackgroundPassEnable, &o->m_MotionBlurBackgroundPassEnable);
            changed |= ui::BoolEdit("Centered", &e->m_MotionBlurCenteredEnable, &o->m_MotionBlurCenteredEnable);

            ImGui::Separator();
            changed |= ui::HdrColor3Edit("Clear Color", &e->m_MotionBlurClearColor, &o->m_MotionBlurClearColor);
            changed |= ui::FloatEdit("Scale", &e->m_MotionBlurScale, &o->m_MotionBlurScale, 0.0f, 2.0f);
            changed |= ui::FloatEdit("Fixed Shutter Time", &e->m_MotionBlurFixedShutterTime, &o->m_MotionBlurFixedShutterTime);
            changed |= ui::FloatEdit("Max", &e->m_MotionBlurMax, &o->m_MotionBlurMax);
            changed |= ui::FloatEdit("Radial Blur Max", &e->m_MotionBlurRadialBlurMax, &o->m_MotionBlurRadialBlurMax);
            changed |= ui::FloatEdit("Noise Scale", &e->m_MotionBlurNoiseScale, &o->m_MotionBlurNoiseScale);
            changed |= ui::UIntEdit("Quality", &e->m_MotionBlurQuality, &o->m_MotionBlurQuality, 0, 4);
            changed |= ui::UIntEdit("Debug Mode", &e->m_MotionBlurDebugMode, &o->m_MotionBlurDebugMode);
            changed |= ui::UIntEdit("Max Sample Count", &e->m_MotionBlurMaxSampleCount, &o->m_MotionBlurMaxSampleCount, 1, 64);
            changed |= ui::FloatEdit("Force Depth Cutoff", &e->m_ForceMotionBlurDepthCutoff, &o->m_ForceMotionBlurDepthCutoff);
            changed |= ui::FloatEdit("Cutoff Gradient Scale", &e->m_ForceMotionBlurCutoffGradientScale, &o->m_ForceMotionBlurCutoffGradientScale);
            changed |= ui::FloatEdit("Depth Check Threshold", &e->m_MotionBlurDepthCheckThreshold, &o->m_MotionBlurDepthCheckThreshold);
            changed |= ui::FloatEdit("Depth Check Max Distance", &e->m_MotionBlurDepthCheckMaxDistance, &o->m_MotionBlurDepthCheckMaxDistance);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lighting - General"))
        {
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
            changed |= ui::BoolEdit("Subsurface Scattering", &e->m_SubSurfaceScatteringEnable, &o->m_SubSurfaceScatteringEnable);
            changed |= ui::BoolEdit("Emissive", &e->m_EmissiveEnable, &o->m_EmissiveEnable);
            changed |= ui::BoolEdit("Unlit", &e->m_UnlitEnable, &o->m_UnlitEnable);
            changed |= ui::BoolEdit("Dynamic Envmap Lighting", &e->m_DynamicEnvmapLightingEnable, &o->m_DynamicEnvmapLightingEnable);
            changed |= ui::BoolEdit("Light Probes", &e->m_LightProbesEnable, &o->m_LightProbesEnable);
            changed |= ui::BoolEdit("Draw Light Sources", &e->m_DrawLightSources, &o->m_DrawLightSources);
            changed |= ui::BoolEdit("Hair Coverage", &e->m_HairCoverageEnable, &o->m_HairCoverageEnable);
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
            changed |= ui::BoolEdit("Occlusion Cull", &e->m_LightOcclusionCullEnable, &o->m_LightOcclusionCullEnable);
            changed |= ui::BoolEdit("Depth Cull", &e->m_LightDepthCullEnable, &o->m_LightDepthCullEnable);
            changed |= ui::BoolEdit("Normal Cull", &e->m_LightNormalCullEnable, &o->m_LightNormalCullEnable);
            changed |= ui::BoolEdit("Cone Cull", &e->m_LightConeCullEnable, &o->m_LightConeCullEnable);
            changed |= ui::FloatEdit("Narrow Angle Threshold", &e->m_LightNarrowAngleThreshold, &o->m_LightNarrowAngleThreshold);

            ImGui::Separator();
            changed |= ui::FloatEdit("Stencil Min Area", &e->m_LightStencilMinArea, &o->m_LightStencilMinArea);
            changed |= ui::BoolEdit("Stencil Method", &e->m_LightStencilMethodEnable, &o->m_LightStencilMethodEnable);
            changed |= ui::BoolEdit("Volume Method", &e->m_LightVolumeMethodEnable, &o->m_LightVolumeMethodEnable);
            changed |= ui::BoolEdit("Volume Depth Test", &e->m_LightVolumeDepthTestEnable, &o->m_LightVolumeDepthTestEnable);
            changed |= ui::FloatEdit("Tile Min Area", &e->m_LightTileMinArea, &o->m_LightTileMinArea);
            changed |= ui::BoolEdit("Tile Overlay", &e->m_LightTileOverlayEnable, &o->m_LightTileOverlayEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Light Tiling"))
        {
            changed |= ui::UIntEdit("CS Avg Light Count Per Tile", &e->m_LightTileCsAvgLightCountPerTile, &o->m_LightTileCsAvgLightCountPerTile);
            changed |= ui::BoolEdit("CS Path Enable", &e->m_LightTileCsPathEnable, &o->m_LightTileCsPathEnable);
            changed |= ui::BoolEdit("Min/Max Use HTile", &e->m_LightTileMinMaxUseHTile, &o->m_LightTileMinMaxUseHTile);
            changed |= ui::BoolEdit("Split Culling And Lighting", &e->m_LightTileSplitCullingAndLighting, &o->m_LightTileSplitCullingAndLighting);

            ImGui::Separator();
            changed |= ui::BoolEdit("Cube Map Stencil Enable", &e->m_LightCubeMapStencilEnable, &o->m_LightCubeMapStencilEnable);
            changed |= ui::FloatEdit("Cube Map Stencil Min Area", &e->m_LightCubeMapStencilMinArea, &o->m_LightCubeMapStencilMinArea);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Outdoor Light"))
        {
            changed |= ui::BoolEdit("Outdoor Light Enable", &e->m_OutdoorLightEnable, &o->m_OutdoorLightEnable);
            changed |= ui::BoolEdit("Outdoor Key Light", &e->m_OutdoorKeyLightEnable, &o->m_OutdoorKeyLightEnable);
            changed |= ui::BoolEdit("Outdoor Sky Light", &e->m_OutdoorSkyLightEnable, &o->m_OutdoorSkyLightEnable);
            changed |= ui::BoolEdit("Outdoor Light Tiling", &e->m_OutdoorLightTilingEnable, &o->m_OutdoorLightTilingEnable);
            changed |= ui::BoolEdit("Tile Render", &e->m_OutdoorLightTileRenderEnable, &o->m_OutdoorLightTileRenderEnable);
            changed |= ui::BoolEdit("Tile Blend", &e->m_OutdoorLightTileBlendEnable, &o->m_OutdoorLightTileBlendEnable);
            changed |= ui::BoolEdit("Tile Simple Shader", &e->m_OutdoorLightTileSimpleShaderEnable, &o->m_OutdoorLightTileSimpleShaderEnable);
            changed |= ui::UIntEdit("Tile Batch Count", &e->m_OutdoorLightTileBatchCount, &o->m_OutdoorLightTileBatchCount);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Light Types"))
        {
            changed |= ui::BoolEdit("Point Lights", &e->m_PointLightsEnable, &o->m_PointLightsEnable);
            changed |= ui::UIntEdit("Max Point Light Count", &e->m_MaxPointLightCount, &o->m_MaxPointLightCount, 0, 1024);

            ImGui::Separator();
            changed |= ui::BoolEdit("Spot Lights", &e->m_SpotLightsEnable, &o->m_SpotLightsEnable);
            changed |= ui::UIntEdit("Max Spot Light Count", &e->m_MaxSpotLightCount, &o->m_MaxSpotLightCount, 0, 256);
            changed |= ui::UIntEdit("Max Spot Shadow Count", &e->m_MaxSpotLightShadowCount, &o->m_MaxSpotLightShadowCount, 0, 64);
            changed |= ui::EnumCombo<fb::QualityLevel>("Spots As Cone Lights Level",
                reinterpret_cast<int*>(&e->m_SpotLightsAsConeLightsLevel),
                reinterpret_cast<const int*>(&o->m_SpotLightsAsConeLightsLevel));

            ImGui::Separator();
            changed |= ui::BoolEdit("Line Lights", &e->m_LineLightsEnable, &o->m_LineLightsEnable);
            changed |= ui::BoolEdit("Cone Lights", &e->m_ConeLightsEnable, &o->m_ConeLightsEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Spot Light Shadows"))
        {
            changed |= ui::BoolEdit("Enable", &e->m_SpotLightShadowmapEnable, &o->m_SpotLightShadowmapEnable);
            changed |= ui::UIntEdit("Resolution", &e->m_SpotLightShadowmapResolution, &o->m_SpotLightShadowmapResolution, 128, 4096);
            changed |= ui::UIntEdit("Quality", &e->m_SpotLightShadowmapQuality, &o->m_SpotLightShadowmapQuality, 0, 4);
            changed |= ui::FloatEdit("Near Plane", &e->m_SpotLightNearPlane, &o->m_SpotLightNearPlane);
            changed |= ui::FloatEdit("Poisson Filter Scale", &e->m_SpotLightShadowmapPoissonFilterScale, &o->m_SpotLightShadowmapPoissonFilterScale);
            changed |= ui::EnumCombo<fb::QualityLevel>("Shadowmap Level",
                reinterpret_cast<int*>(&e->m_SpotLightShadowmapLevel),
                reinterpret_cast<const int*>(&o->m_SpotLightShadowmapLevel));
            changed |= ui::BoolEdit("Radiosity Spot Shadow Culling", &e->m_RadiositySpotLightShadowCullingEnable, &o->m_RadiositySpotLightShadowCullingEnable);
            changed |= ui::BoolEdit("DX 16-Bit Spot Shadowmap", &e->m_DxSpotLightShadowmap16BitEnable, &o->m_DxSpotLightShadowmap16BitEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Shadow Occlusion"))
        {
            changed |= ui::BoolEdit("Occlusion Culling Enable", &e->m_ShadowOcclusionCullingEnable, &o->m_ShadowOcclusionCullingEnable);
            changed |= ui::UIntEdit("Culling Width", &e->m_ShadowOcclusionCullingWidth, &o->m_ShadowOcclusionCullingWidth);
            changed |= ui::UIntEdit("Culling Height", &e->m_ShadowOcclusionCullingHeight, &o->m_ShadowOcclusionCullingHeight);
            changed |= ui::UIntEdit("Triangle Count", &e->m_ShadowOcclusionTriangleCount, &o->m_ShadowOcclusionTriangleCount);

            ImGui::Separator();
            changed |= ui::BoolEdit("Frustum Silhouette Culling", &e->m_FrustumSilhouetteCullingEnable, &o->m_FrustumSilhouetteCullingEnable);
            changed |= ui::FloatEdit("Frustum Silhouette Padding", &e->m_FrustumSilhouetteCullingPadding, &o->m_FrustumSilhouetteCullingPadding);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Sky"))
        {
            changed |= ui::BoolEdit("Sky Enable", &e->m_SkyEnable, &o->m_SkyEnable);
            changed |= ui::BoolEdit("Sky Fog", &e->m_SkyFogEnable, &o->m_SkyFogEnable);
            changed |= ui::BoolEdit("Sky Height Fog", &e->m_SkyHeightFogEnable, &o->m_SkyHeightFogEnable);
            changed |= ui::BoolEdit("Sky Forward Scattering", &e->m_SkyForwardScatteringEnable, &o->m_SkyForwardScatteringEnable);
            changed |= ui::BoolEdit("Sky Visibility Envmap Scaling", &e->m_SkyVisibilityEnvmapScalingEnable, &o->m_SkyVisibilityEnvmapScalingEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Sky Envmap", &e->m_SkyEnvmapEnable, &o->m_SkyEnvmapEnable);
            changed |= ui::BoolEdit("Sky Envmap Mipmap Gen", &e->m_SkyEnvmapMipmapGenEnable, &o->m_SkyEnvmapMipmapGenEnable);
            changed |= ui::BoolEdit("Sky Envmap Update", &e->m_SkyEnvmapUpdateEnable, &o->m_SkyEnvmapUpdateEnable);
            changed |= ui::BoolEdit("Sky Envmap Force Update", &e->m_SkyEnvmapForceUpdateEnable, &o->m_SkyEnvmapForceUpdateEnable);
            changed |= ui::BoolEdit("Sky Envmap 8-Bit Texture", &e->m_SkyEnvmapUse8BitTexture, &o->m_SkyEnvmapUse8BitTexture);
            changed |= ui::UIntEdit("Sky Envmap Resolution", &e->m_SkyEnvmapResolution, &o->m_SkyEnvmapResolution, 16, 512);
            changed |= ui::UIntEdit("Sky Envmap Sides Per Frame", &e->m_SkyEnvmapSidesPerFrameCount, &o->m_SkyEnvmapSidesPerFrameCount, 1, 6);
            changed |= ui::FloatEdit("Sky Envmap Filter Width", &e->m_SkyEnvmapFilterWidth, &o->m_SkyEnvmapFilterWidth);
            changed |= ui::EnumCombo<fb::MipmapFilterMode>("Sky Envmap Filter Mode",
                reinterpret_cast<int*>(&e->m_SkyEnvmapFilterMode),
                reinterpret_cast<const int*>(&o->m_SkyEnvmapFilterMode));
            changed |= ui::IntEdit("Debug Sky Envmap Mip Level", &e->m_DrawDebugSkyEnvmapMipLevel, &o->m_DrawDebugSkyEnvmapMipLevel, -1, 10);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Dynamic Environment Map"))
        {
            changed |= ui::BoolEdit("Enable", &e->m_DynamicEnvmapEnable, &o->m_DynamicEnvmapEnable);
            changed |= ui::BoolEdit("Mipmap Gen", &e->m_DynamicEnvmapMipmapGenEnable, &o->m_DynamicEnvmapMipmapGenEnable);
            changed |= ui::UIntEdit("Resolution", &e->m_DynamicEnvmapResolution, &o->m_DynamicEnvmapResolution, 16, 512);
            changed |= ui::FloatEdit("Filter Width", &e->m_DynamicEnvmapFilterWidth, &o->m_DynamicEnvmapFilterWidth);
            changed |= ui::EnumCombo<fb::MipmapFilterMode>("Filter Mode",
                reinterpret_cast<int*>(&e->m_DynamicEnvmapFilterMode),
                reinterpret_cast<const int*>(&o->m_DynamicEnvmapFilterMode));
            changed |= ui::Vec3Edit("Default Position", &e->m_DynamicEnvmapDefaultPosition, &o->m_DynamicEnvmapDefaultPosition);
            changed |= ui::IntEdit("Debug Mip Level", &e->m_DrawDebugDynamicEnvmapMipLevel, &o->m_DrawDebugDynamicEnvmapMipLevel, -1, 10);

            ImGui::Separator();
            changed |= ui::BoolEdit("Static Envmap", &e->m_StaticEnvmapEnable, &o->m_StaticEnvmapEnable);
            changed |= ui::BoolEdit("Custom Envmap", &e->m_CustomEnvmapEnable, &o->m_CustomEnvmapEnable);
            changed |= ui::BoolEdit("Custom Envmap Mipmap Clamp", &e->m_CustomEnvmapMipmapClampEnable, &o->m_CustomEnvmapMipmapClampEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Distortion"))
        {
            changed |= ui::BoolEdit("Enable", &e->m_DistortionEnable, &o->m_DistortionEnable);
            changed |= ui::BoolEdit("Half Res", &e->m_DistortionHalfResEnable, &o->m_DistortionHalfResEnable);
            changed |= ui::BoolEdit("8-Bit", &e->m_Distortion8BitEnable, &o->m_Distortion8BitEnable);
            changed |= ui::BoolEdit("Tiling", &e->m_DistortionTilingEnable, &o->m_DistortionTilingEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Reflections"))
        {
            changed |= ui::BoolEdit("Planar Reflection Enable", &e->m_PlanarReflectionEnable, &o->m_PlanarReflectionEnable);
            changed |= ui::BoolEdit("Fast HDR", &e->m_PlanarReflectionFastHdrEnable, &o->m_PlanarReflectionFastHdrEnable);
            changed |= ui::BoolEdit("Blur", &e->m_PlanarReflectionBlur, &o->m_PlanarReflectionBlur);
            changed |= ui::BoolEdit("Clipping", &e->m_PlanarReflectionClippingEnable, &o->m_PlanarReflectionClippingEnable);
            changed |= ui::FloatEdit("View Scale", &e->m_PlanarReflectionViewScale, &o->m_PlanarReflectionViewScale);
            changed |= ui::FloatEdit("Cull FOV", &e->m_PlanarReflectionCullFOV, &o->m_PlanarReflectionCullFOV);
            changed |= ui::FloatEdit("Far Plane", &e->m_PlanarReflectionFarPlane, &o->m_PlanarReflectionFarPlane);
            changed |= ui::FloatEdit("Reflection LOD Scale", &e->m_ReflectionLodScale, &o->m_ReflectionLodScale);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Decals & Volumes"))
        {
            changed |= ui::BoolEdit("Decal Volume Enable", &e->m_DecalVolumeEnable, &o->m_DecalVolumeEnable);
            changed |= ui::FloatEdit("Decal Volume Scale", &e->m_DecalVolumeScale, &o->m_DecalVolumeScale);
            changed |= ui::UIntEdit("Max Decal Volume Count", &e->m_MaxDecalVolumeCount, &o->m_MaxDecalVolumeCount, 0, 1024);
            changed |= ui::UIntEdit("Max Destruction Volume Count", &e->m_MaxDestructionVolumeCount, &o->m_MaxDestructionVolumeCount, 0, 1024);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lens Flares"))
        {
            changed |= ui::BoolEdit("Enable", &e->m_LensFlaresEnable, &o->m_LensFlaresEnable);
            changed |= ui::BoolEdit("Occlusion Enable", &e->m_LensFlareOcclusionEnable, &o->m_LensFlareOcclusionEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Render Passes"))
        {
            changed |= ui::BoolEdit("Z Pass", &e->m_ZPassEnable, &o->m_ZPassEnable);
            changed |= ui::BoolEdit("Main Opaque Z Pass", &e->m_MainOpaqueZPassEnable, &o->m_MainOpaqueZPassEnable);
            changed |= ui::BoolEdit("Occluder Mesh Z Prepass", &e->m_OccluderMeshZPrepassEnable, &o->m_OccluderMeshZPrepassEnable);
            changed |= ui::BoolEdit("Occluder Mesh Z Prepass Debug", &e->m_OccluderMeshZPrepassDebugEnable, &o->m_OccluderMeshZPrepassDebugEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Foreground", &e->m_ForegroundEnable, &o->m_ForegroundEnable);
            changed |= ui::BoolEdit("Foreground Z Pass", &e->m_ForegroundZPassEnable, &o->m_ForegroundZPassEnable);
            changed |= ui::BoolEdit("Foreground Depth Clear", &e->m_ForegroundDepthClearEnable, &o->m_ForegroundDepthClearEnable);
            changed |= ui::BoolEdit("Foreground Transparent", &e->m_ForegroundTransparentEnable, &o->m_ForegroundTransparentEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Draw Transparent", &e->m_DrawTransparent, &o->m_DrawTransparent);
            changed |= ui::BoolEdit("Draw Transparent Decal", &e->m_DrawTransparentDecal, &o->m_DrawTransparentDecal);
            changed |= ui::BoolEdit("Final Post", &e->m_FinalPostEnable, &o->m_FinalPostEnable);
            changed |= ui::BoolEdit("Screen Effect", &e->m_ScreenEffectEnable, &o->m_ScreenEffectEnable);
            changed |= ui::BoolEdit("Wireframe", &e->m_WireframeEnable, &o->m_WireframeEnable);

            ImGui::Separator();
            changed |= ui::BoolEdit("Opaque Sort By Solution", &e->m_OpaqueSortBySolutionEnable, &o->m_OpaqueSortBySolutionEnable);
            changed |= ui::IntEdit("Only Tile Index", &e->m_OnlyTileIndex, &o->m_OnlyTileIndex, -1, 256);
            changed |= ui::IntEdit("Only Light Tile Index", &e->m_OnlyLightTileIndex, &o->m_OnlyLightTileIndex, -1, 1024);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Job Scheduling"))
        {
            changed |= ui::BoolEdit("Setup Job", &e->m_SetupJobEnable, &o->m_SetupJobEnable);
            changed |= ui::BoolEdit("Finish Sync Jobs First", &e->m_FinishSyncJobsFirstEnable, &o->m_FinishSyncJobsFirstEnable);
            changed |= ui::BoolEdit("Prepare Dispatch List Job", &e->m_PrepareDispatchListJobEnable, &o->m_PrepareDispatchListJobEnable);
            changed |= ui::BoolEdit("Console RT Pool Sharing", &e->m_ConsoleRenderTargetPoolSharingEnable, &o->m_ConsoleRenderTargetPoolSharingEnable);
            changed |= ui::BoolEdit("Emitter Shadowing Blend Toggle", &e->m_EmitterShadowingBlendToggle, &o->m_EmitterShadowingBlendToggle);
            changed |= ui::BoolEdit("Emitter Shadowing Many Samples", &e->m_EmitterShadowingManySamplesToggle, &o->m_EmitterShadowingManySamplesToggle);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Dynamic AO"))
        {
            changed |= ui::BoolEdit("Override Dynamic AO", &e->m_OverrideDynamicAO, &o->m_OverrideDynamicAO);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Debug Visualization"))
        {
            if (ImGui::TreeNode("Buffers"))
            {
                changed |= ui::BoolEdit("GBuffer", &e->m_DrawDebugGBuffer, &o->m_DrawDebugGBuffer);
                changed |= ui::BoolEdit("Z-Buffer", &e->m_DrawDebugZBufferEnable, &o->m_DrawDebugZBufferEnable);
                changed |= ui::BoolEdit("Velocity Buffer", &e->m_DrawDebugVelocityBuffer, &o->m_DrawDebugVelocityBuffer);
                changed |= ui::BoolEdit("Multisample Classify", &e->m_DrawDebugMultisampleClassify, &o->m_DrawDebugMultisampleClassify);
                changed |= ui::BoolEdit("Half Res Environment", &e->m_DrawDebugHalfResEnvironment, &o->m_DrawDebugHalfResEnvironment);
                changed |= ui::BoolEdit("Half Res HDR Targets", &e->m_DrawDebugHalfResHdrTargets, &o->m_DrawDebugHalfResHdrTargets);
                changed |= ui::UIntEdit("Ground Height", &e->m_DrawDebugGroundHeight, &o->m_DrawDebugGroundHeight);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Shadows"))
            {
                changed |= ui::BoolEdit("Shadowmaps", &e->m_DrawDebugShadowmaps, &o->m_DrawDebugShadowmaps);
                changed |= ui::BoolEdit("Spot Light Shadowmaps", &e->m_DrawDebugSpotLightShadowmaps, &o->m_DrawDebugSpotLightShadowmaps);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Lights"))
            {
                changed |= ui::BoolEdit("Light Sources", &e->m_DrawDebugLightSources, &o->m_DrawDebugLightSources);
                changed |= ui::BoolEdit("Light Shadow Sources", &e->m_DrawDebugLightShadowSources, &o->m_DrawDebugLightShadowSources);
                changed |= ui::BoolEdit("Light Texture Sources", &e->m_DrawDebugLightTextureSources, &o->m_DrawDebugLightTextureSources);
                changed |= ui::BoolEdit("Light Stats", &e->m_DrawDebugLightStats, &o->m_DrawDebugLightStats);
                changed |= ui::BoolEdit("Light Tiles", &e->m_DrawDebugLightTiles, &o->m_DrawDebugLightTiles);
                changed |= ui::BoolEdit("Light Tile Volumes", &e->m_DrawDebugLightTileVolumes, &o->m_DrawDebugLightTileVolumes);
                changed |= ui::BoolEdit("Light Tile FB Grid", &e->m_DrawDebugLightTileFbGrid, &o->m_DrawDebugLightTileFbGrid);
                changed |= ui::BoolEdit("Light Tile Grid", &e->m_DrawDebugLightTileGrid, &o->m_DrawDebugLightTileGrid);
                changed |= ui::BoolEdit("Light Tile Sources", &e->m_DrawDebugLightTileSources, &o->m_DrawDebugLightTileSources);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Environment / Reflections"))
            {
                changed |= ui::BoolEdit("Sky Envmap", &e->m_DrawDebugSkyEnvmap, &o->m_DrawDebugSkyEnvmap);
                changed |= ui::BoolEdit("Sky Textures", &e->m_DrawDebugSkyTextures, &o->m_DrawDebugSkyTextures);
                changed |= ui::BoolEdit("Dynamic Envmap", &e->m_DrawDebugDynamicEnvmap, &o->m_DrawDebugDynamicEnvmap);
                changed |= ui::BoolEdit("Planar Reflection", &e->m_DrawDebugPlanarReflection, &o->m_DrawDebugPlanarReflection);
                changed |= ui::BoolEdit("Planar Reflection Cull Frustum", &e->m_DrawDebugPlanarReflectionCullFrustum, &o->m_DrawDebugPlanarReflectionCullFrustum);
                changed |= ui::UIntEdit("Planar Reflection Mode", &e->m_DrawDebugPlanarReflectionMode, &o->m_DrawDebugPlanarReflectionMode);

                ImGui::Separator();
                changed |= ui::BoolEdit("Indirect CubeMap", &e->m_DrawDebugIndirectCubeMapEnable, &o->m_DrawDebugIndirectCubeMapEnable);
                changed |= ui::UIntEdit("Indirect CubeMap Index", &e->m_DrawDebugIndirectCubeMapIndex, &o->m_DrawDebugIndirectCubeMapIndex);
                changed |= ui::UIntEdit("Indirect CubeMap Mip Level", &e->m_DrawDebugIndirectCubeMapMipLevel, &o->m_DrawDebugIndirectCubeMapMipLevel);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Volumes & Misc"))
            {
                changed |= ui::BoolEdit("Decal Volumes", &e->m_DrawDebugDecalVolumes, &o->m_DrawDebugDecalVolumes);
                changed |= ui::BoolEdit("Destruction Volumes", &e->m_DrawDebugDestructionVolumes, &o->m_DrawDebugDestructionVolumes);
                changed |= ui::BoolEdit("Lens Flares", &e->m_DrawDebugLensFlares, &o->m_DrawDebugLensFlares);
                changed |= ui::BoolEdit("Lens Flare Occluders", &e->m_DrawDebugLensFlareOccluders, &o->m_DrawDebugLensFlareOccluders);
                changed |= ui::BoolEdit("Dynamic AO", &e->m_DrawDebugDynamicAO, &o->m_DrawDebugDynamicAO);
                changed |= ui::BoolEdit("Distortion", &e->m_DrawDebugDistortion, &o->m_DrawDebugDistortion);
                changed |= ui::BoolEdit("Visible Entity Types", &e->m_DrawDebugVisibleEntityTypes, &o->m_DrawDebugVisibleEntityTypes);
                changed |= ui::BoolEdit("DoF", &e->m_DrawDebugDof, &o->m_DrawDebugDof);
                changed |= ui::BoolEdit("DoF Fullscreen", &e->m_DrawDebugDofFullscreen, &o->m_DrawDebugDofFullscreen);
                changed |= ui::BoolEdit("Marschner Textures", &e->m_DrawDebugMarschnerTextures, &o->m_DrawDebugMarschnerTextures);
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

#endif // BFVE_GAME_BF4
