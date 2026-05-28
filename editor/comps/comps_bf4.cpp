#include "comps.h"

#include "../ui/ui_helpers.h"
#include <imgui.h>

#if defined(BFVE_GAME_BF4)

namespace editor::comps
{
    void renderOutdoorLightComponent(fb::CapturedOutdoorLightComponentData* e, const fb::CapturedOutdoorLightComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

        if (ImGui::TreeNode("Sun##OL"))
        {
            ui::Vec3Edit("Sun Color", &e->m_SunColor, &o->m_SunColor, true);
            ui::FloatEdit("Sun Rotation X", &e->m_SunRotationX, &o->m_SunRotationX, -179.9f, 179.9f);
            ui::FloatEdit("Sun Rotation Y", &e->m_SunRotationY, &o->m_SunRotationY, -179.9f, 179.9f);
            ui::FloatEdit("Sun Specular Scale", &e->m_SunSpecularScale, &o->m_SunSpecularScale);
            ui::FloatEdit("Sun Shadow Height Scale", &e->m_SunShadowHeightScale, &o->m_SunShadowHeightScale);

            ImGui::Separator();
            ui::BoolEdit("Shadow Sun Rotation Enable", &e->m_ShadowSunRotationEnable, &o->m_ShadowSunRotationEnable);
            ui::FloatEdit("Shadow Sun Rotation X", &e->m_ShadowSunRotationX, &o->m_ShadowSunRotationX, -179.9f, 179.9f);
            ui::FloatEdit("Shadow Sun Rotation Y", &e->m_ShadowSunRotationY, &o->m_ShadowSunRotationY, -179.9f, 179.9f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Sky##OL"))
        {
            ui::Vec3Edit("Sky Color", &e->m_SkyColor, &o->m_SkyColor, true);
            ui::Vec3Edit("Ground Color", &e->m_GroundColor, &o->m_GroundColor, true);
            ui::FloatEdit("Sky Light Angle Factor", &e->m_SkyLightAngleFactor, &o->m_SkyLightAngleFactor);
            ui::FloatEdit("Sky Envmap Shadow Scale", &e->m_SkyEnvmapShadowScale, &o->m_SkyEnvmapShadowScale);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Cloud Shadow##OL"))
        {
            ui::BoolEdit("Enable##CS", &e->m_CloudShadowEnable, &o->m_CloudShadowEnable);
            ui::BoolEdit("Top Down", &e->m_CloudShadowIsTopDown, &o->m_CloudShadowIsTopDown);
            ui::Vec2Edit("Speed", &e->m_CloudShadowSpeed, &o->m_CloudShadowSpeed);
            ui::FloatEdit("Coverage", &e->m_CloudShadowCoverage, &o->m_CloudShadowCoverage, 0.0f, 1.0f);
            ui::FloatEdit("Size", &e->m_CloudShadowSize, &o->m_CloudShadowSize);
            ui::FloatEdit("Exponent", &e->m_CloudShadowExponent, &o->m_CloudShadowExponent);
            ui::FloatEdit("Start Fade", &e->m_CloudShadowStartFade, &o->m_CloudShadowStartFade);
            ui::FloatEdit("Fade Distance", &e->m_CloudShadowsFadeDistance, &o->m_CloudShadowsFadeDistance);
            ui::Vec2Edit("XZ Translation", &e->m_CloudXZTranslation, &o->m_CloudXZTranslation);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Translucency##OL"))
        {
            ui::FloatEdit("Distortion", &e->m_TranslucencyDistortion, &o->m_TranslucencyDistortion);
            ui::FloatEdit("Ambient", &e->m_TranslucencyAmbient, &o->m_TranslucencyAmbient);
            ui::FloatEdit("Scale", &e->m_TranslucencyScale, &o->m_TranslucencyScale);
            ui::FloatEdit("Power", &e->m_TranslucencyPower, &o->m_TranslucencyPower);
            ImGui::TreePop();
        }
    }

    void renderEnlightenComponent(fb::CapturedEnlightenComponentData* e, const fb::CapturedEnlightenComponentData* o)
    {
        ui::BoolEdit("SkyBox Enable", &e->m_SkyBoxEnable, &o->m_SkyBoxEnable);

        if (ImGui::TreeNode("SkyBox Colors##EN"))
        {
            ui::Vec3Edit("Sky Color", &e->m_SkyBoxSkyColor, &o->m_SkyBoxSkyColor, true);
            ui::Vec3Edit("Ground Color", &e->m_SkyBoxGroundColor, &o->m_SkyBoxGroundColor, true);
            ui::Vec3Edit("Sun Light Color", &e->m_SkyBoxSunLightColor, &o->m_SkyBoxSunLightColor, true);
            ui::Vec3Edit("Back Light Color", &e->m_SkyBoxBackLightColor, &o->m_SkyBoxBackLightColor, true);
            ui::Vec3Edit("Terrain Color", &e->m_TerrainColor, &o->m_TerrainColor, true);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Overrides##EN"))
        {
            ui::Vec3Edit("Opaque AlphaTest Simple Scale", &e->m_OpaqueAlphaTestSimpleScale, &o->m_OpaqueAlphaTestSimpleScale);
            ui::Vec3Edit("Override Light Probe", &e->m_OverrideLightProbe, &o->m_OverrideLightProbe, true);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Light Settings##EN"))
        {
            ui::FloatEdit("Bounce Scale", &e->m_BounceScale, &o->m_BounceScale);
            ui::FloatEdit("Sun Scale", &e->m_SunScale, &o->m_SunScale);
            ui::FloatEdit("Cull Distance", &e->m_CullDistance, &o->m_CullDistance);
            ui::FloatEdit("Cull Radius", &e->m_CullRadius, &o->m_CullRadius);
            ui::FloatEdit("Sun Light Color Size", &e->m_SkyBoxSunLightColorSize, &o->m_SkyBoxSunLightColorSize);
            ui::FloatEdit("Back Light Color Size", &e->m_SkyBoxBackLightColorSize, &o->m_SkyBoxBackLightColorSize);
            ui::FloatEdit("Back Light Rotation X", &e->m_SkyBoxBackLightRotationX, &o->m_SkyBoxBackLightRotationX);
            ui::FloatEdit("Back Light Rotation Y", &e->m_SkyBoxBackLightRotationY, &o->m_SkyBoxBackLightRotationY);
            ImGui::TreePop();
        }
    }

    void renderTonemapComponent(fb::CapturedTonemapComponentData* e, const fb::CapturedTonemapComponentData* o)
    {
        ui::EnumCombo<fb::TonemapMethod>("Tonemap Method",
            reinterpret_cast<int*>(&e->m_TonemapMethod),
            reinterpret_cast<const int*>(&o->m_TonemapMethod));

        ui::Vec3Edit("Bloom Scale", &e->m_BloomScale, &o->m_BloomScale);
        ui::FloatEdit("Min Exposure", &e->m_MinExposure, &o->m_MinExposure);
        ui::FloatEdit("Max Exposure", &e->m_MaxExposure, &o->m_MaxExposure);
        ui::FloatEdit("Middle Gray", &e->m_MiddleGray, &o->m_MiddleGray);
        ui::FloatEdit("Exposure Adjust Time", &e->m_ExposureAdjustTime, &o->m_ExposureAdjustTime);

        if (ImGui::TreeNode("Chromostereopsis##TM"))
        {
            ui::BoolEdit("Enable##Chromo", &e->m_ChromostereopsisEnable, &o->m_ChromostereopsisEnable);
            ui::FloatEdit("Offset", &e->m_ChromostereopsisOffset, &o->m_ChromostereopsisOffset);
            ui::FloatEdit("Scale##Chromo", &e->m_ChromostereopsisScale, &o->m_ChromostereopsisScale);
            ImGui::TreePop();
        }
    }

    void renderColorCorrectionComponent(fb::CapturedColorCorrectionComponentData* e, const fb::CapturedColorCorrectionComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Color Grading Enable", &e->m_ColorGradingEnable, &o->m_ColorGradingEnable);
        ui::Vec3Edit("Brightness", &e->m_Brightness, &o->m_Brightness);
        ui::Vec3Edit("Contrast", &e->m_Contrast, &o->m_Contrast);
        ui::Vec3Edit("Saturation", &e->m_Saturation, &o->m_Saturation);
        ui::FloatEdit("Hue", &e->m_Hue, &o->m_Hue, -180.0f, 180.0f);
    }

    void renderSkyComponent(fb::CapturedSkyComponentData* e, const fb::CapturedSkyComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Indirect CubeMap Override", &e->m_IndirectCubeMapOverride, &o->m_IndirectCubeMapOverride);

        if (ImGui::TreeNode("General##Sky"))
        {
            ui::FloatEdit("Brightness Scale", &e->m_BrightnessScale, &o->m_BrightnessScale);
            ui::FloatEdit("Sun Size", &e->m_SunSize, &o->m_SunSize);
            ui::FloatEdit("Sun Scale", &e->m_SunScale, &o->m_SunScale);
            ui::FloatEdit("Sky Visibility Exponent", &e->m_SkyVisibilityExponent, &o->m_SkyVisibilityExponent);
            ui::FloatEdit("Sky Envmap 8Bit Tex Scale", &e->m_SkyEnvmap8BitTexScale, &o->m_SkyEnvmap8BitTexScale);
            ImGui::TreePop();
        }

        ui::Vec3Edit("Cloud Layer Sun Color", &e->m_CloudLayerSunColor, &o->m_CloudLayerSunColor, true);

        if (ImGui::TreeNode("Cloud Layer 1##Sky"))
        {
            ui::Vec3Edit("Color##CL1", &e->m_CloudLayer1Color, &o->m_CloudLayer1Color, true);
            ui::FloatEdit("Altitude##CL1", &e->m_CloudLayer1Altitude, &o->m_CloudLayer1Altitude);
            ui::FloatEdit("Tile Factor##CL1", &e->m_CloudLayer1TileFactor, &o->m_CloudLayer1TileFactor);
            ui::FloatEdit("Rotation##CL1", &e->m_CloudLayer1Rotation, &o->m_CloudLayer1Rotation, 0.0f, 360.0f);
            ui::FloatEdit("Speed##CL1", &e->m_CloudLayer1Speed, &o->m_CloudLayer1Speed);
            ui::FloatEdit("Sun Light Intensity##CL1", &e->m_CloudLayer1SunLightIntensity, &o->m_CloudLayer1SunLightIntensity);
            ui::FloatEdit("Sun Light Power##CL1", &e->m_CloudLayer1SunLightPower, &o->m_CloudLayer1SunLightPower);
            ui::FloatEdit("Ambient Light Intensity##CL1", &e->m_CloudLayer1AmbientLightIntensity, &o->m_CloudLayer1AmbientLightIntensity);
            ui::FloatEdit("Alpha Mul##CL1", &e->m_CloudLayer1AlphaMul, &o->m_CloudLayer1AlphaMul, 0.0f, 1.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Cloud Layer 2##Sky"))
        {
            ui::Vec3Edit("Color##CL2", &e->m_CloudLayer2Color, &o->m_CloudLayer2Color, true);
            ui::FloatEdit("Altitude##CL2", &e->m_CloudLayer2Altitude, &o->m_CloudLayer2Altitude);
            ui::FloatEdit("Tile Factor##CL2", &e->m_CloudLayer2TileFactor, &o->m_CloudLayer2TileFactor);
            ui::FloatEdit("Rotation##CL2", &e->m_CloudLayer2Rotation, &o->m_CloudLayer2Rotation, 0.0f, 360.0f);
            ui::FloatEdit("Speed##CL2", &e->m_CloudLayer2Speed, &o->m_CloudLayer2Speed);
            ui::FloatEdit("Sun Light Intensity##CL2", &e->m_CloudLayer2SunLightIntensity, &o->m_CloudLayer2SunLightIntensity);
            ui::FloatEdit("Sun Light Power##CL2", &e->m_CloudLayer2SunLightPower, &o->m_CloudLayer2SunLightPower);
            ui::FloatEdit("Ambient Light Intensity##CL2", &e->m_CloudLayer2AmbientLightIntensity, &o->m_CloudLayer2AmbientLightIntensity);
            ui::FloatEdit("Alpha Mul##CL2", &e->m_CloudLayer2AlphaMul, &o->m_CloudLayer2AlphaMul, 0.0f, 1.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Panoramic##Sky"))
        {
            ui::FloatEdit("Rotation##Pan", &e->m_PanoramicRotation, &o->m_PanoramicRotation, 0.0f, 360.0f);
            ui::FloatEdit("Tile Factor##Pan", &e->m_PanoramicTileFactor, &o->m_PanoramicTileFactor);
            ui::FloatEdit("UV Min X", &e->m_PanoramicUVMinX, &o->m_PanoramicUVMinX, 0.0f, 1.0f);
            ui::FloatEdit("UV Max X", &e->m_PanoramicUVMaxX, &o->m_PanoramicUVMaxX, 0.0f, 1.0f);
            ui::FloatEdit("UV Min Y", &e->m_PanoramicUVMinY, &o->m_PanoramicUVMinY, 0.0f, 1.0f);
            ui::FloatEdit("UV Max Y", &e->m_PanoramicUVMaxY, &o->m_PanoramicUVMaxY, 0.0f, 1.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Envmap##Sky"))
        {
            ui::FloatEdit("Custom Envmap Scale", &e->m_CustomEnvmapScale, &o->m_CustomEnvmapScale);
            ui::FloatEdit("Custom Envmap Ambient", &e->m_CustomEnvmapAmbient, &o->m_CustomEnvmapAmbient);
            ui::FloatEdit("Static Envmap Scale", &e->m_StaticEnvmapScale, &o->m_StaticEnvmapScale);
            ImGui::TreePop();
        }
    }

    void renderFogComponent(fb::CapturedFogComponentData* e, const fb::CapturedFogComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

        if (ImGui::TreeNode("Basic Fog##Fog"))
        {
            ui::FloatEdit("Start", &e->m_Start, &o->m_Start);
            ui::FloatEdit("End", &e->m_End, &o->m_End);
            ui::FloatEdit("Distance Multiplier", &e->m_FogDistanceMultiplier, &o->m_FogDistanceMultiplier);
            ui::CurveVec4Edit("Curve", &e->m_Curve, &o->m_Curve);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Fog Color##Fog"))
        {
            ui::BoolEdit("Enable##FogColor", &e->m_FogColorEnable, &o->m_FogColorEnable);
            ui::Vec3Edit("Color", &e->m_FogColor, &o->m_FogColor, true);
            ui::FloatEdit("Color Start", &e->m_FogColorStart, &o->m_FogColorStart);
            ui::FloatEdit("Color End", &e->m_FogColorEnd, &o->m_FogColorEnd);
            ui::CurveVec4Edit("Color Curve", &e->m_FogColorCurve, &o->m_FogColorCurve);
            ImGui::TreePop();
        }

        ui::BoolEdit("Gradient Enable", &e->m_FogGradientEnable, &o->m_FogGradientEnable);

        if (ImGui::TreeNode("Height Fog##Fog"))
        {
            ui::BoolEdit("Enable##HeightFog", &e->m_HeightFogEnable, &o->m_HeightFogEnable);
            ui::FloatEdit("Altitude", &e->m_HeightFogAltitude, &o->m_HeightFogAltitude);
            ui::FloatEdit("Depth", &e->m_HeightFogDepth, &o->m_HeightFogDepth);
            ui::FloatEdit("Visibility Range", &e->m_HeightFogVisibilityRange, &o->m_HeightFogVisibilityRange);
            ui::FloatEdit("Follow Camera", &e->m_HeightFogFollowCamera, &o->m_HeightFogFollowCamera, 0.0f, 1.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Transparency Fade##Fog"))
        {
            ui::FloatEdit("Start##TransFade", &e->m_TransparencyFadeStart, &o->m_TransparencyFadeStart);
            ui::FloatEdit("End##TransFade", &e->m_TransparencyFadeEnd, &o->m_TransparencyFadeEnd);
            ui::FloatEdit("Clamp", &e->m_TransparencyFadeClamp, &o->m_TransparencyFadeClamp);
            ui::CurveVec4Edit("Curve##TransFade", &e->m_TransparencyFadeCurve, &o->m_TransparencyFadeCurve);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Forward Light Scattering##Fog"))
        {
            ui::BoolEdit("Enabled##FLS", &e->m_ForwardLightScatteringEnabled, &o->m_ForwardLightScatteringEnabled);
            ui::Vec3Edit("Color##FLS", &e->m_ForwardLightScatteringColor, &o->m_ForwardLightScatteringColor, true);
            ui::FloatEdit("Phase G", &e->m_ForwardLightScatteringPhaseG, &o->m_ForwardLightScatteringPhaseG);
            ui::FloatEdit("Strength", &e->m_ForwardLightScatteringStrength, &o->m_ForwardLightScatteringStrength);
            ui::FloatEdit("Presence", &e->m_ForwardLightScatteringPresence, &o->m_ForwardLightScatteringPresence);
            ui::FloatEdit("Max Blur Length", &e->m_ForwardLightScatteringMaxBlurLength, &o->m_ForwardLightScatteringMaxBlurLength);
            ui::FloatEdit("Extinction", &e->m_ForwardLightScatteringExtinction, &o->m_ForwardLightScatteringExtinction);
            ui::FloatEdit("Smoothness", &e->m_ForwardLightScatteringSmoothness, &o->m_ForwardLightScatteringSmoothness);
            ImGui::TreePop();
        }
    }

    void renderWindComponent(fb::CapturedWindComponentData* e, const fb::CapturedWindComponentData* o)
    {
        ui::FloatEdit("Direction", &e->m_WindDirection, &o->m_WindDirection, 0.0f, 360.0f);
        ui::FloatEdit("Strength", &e->m_WindStrength, &o->m_WindStrength);

        ImGui::Separator();
        ui::FloatEdit("Variation Multiplier", &e->m_WindVariationMultiplier, &o->m_WindVariationMultiplier);
        ui::FloatEdit("Variation Rate Multiplier", &e->m_WindVariationRateMultiplier, &o->m_WindVariationRateMultiplier);
        ui::FloatEdit("Micro Variation Multiplier", &e->m_WindMicroVariationMultiplier, &o->m_WindMicroVariationMultiplier);

        ImGui::Separator();
        ui::FloatEdit("Turbulence Multiplier", &e->m_TurbulenceMultiplier, &o->m_TurbulenceMultiplier);
        ui::FloatEdit("Turbulence Scale", &e->m_TurbulenceScale, &o->m_TurbulenceScale);
    }

    void renderSunFlareComponent(fb::CapturedSunFlareComponentData* e, const fb::CapturedSunFlareComponentData* o)
    {
        ImGui::Indent();

        ui::BoolEdit("Enable##SunFlare", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Debug Draw Occluder", &e->m_DebugDrawOccluder, &o->m_DebugDrawOccluder);
        ui::FloatEdit("Occluder Size", &e->m_OccluderSize, &o->m_OccluderSize);

        for (int i = 1; i <= 5; i++)
        {
            char label[64];
            sprintf_s(label, "Element %d", i);

            if (ImGui::TreeNode(label))
            {
                bool* enable = nullptr;
                bool* alignedToRay = nullptr;
                fb::Vec2* size = nullptr;
                float* rayDistance = nullptr;
                float* rotationLocal = nullptr;
                float* rotationDistMul = nullptr;
                fb::Vec4* alphaScreenPosCurve = nullptr;
                fb::Vec4* alphaOccluderCurve = nullptr;
                fb::Vec4* sizeScreenPosCurve = nullptr;
                fb::Vec4* sizeOccluderCurve = nullptr;
                fb::Vec4* rotationDistCurve = nullptr;

                const bool* origEnable = nullptr;
                const bool* origAlignedToRay = nullptr;
                const fb::Vec2* origSize = nullptr;
                const float* origRayDistance = nullptr;
                const float* origRotationLocal = nullptr;
                const float* origRotationDistMul = nullptr;
                const fb::Vec4* origAlphaScreenPosCurve = nullptr;
                const fb::Vec4* origAlphaOccluderCurve = nullptr;
                const fb::Vec4* origSizeScreenPosCurve = nullptr;
                const fb::Vec4* origSizeOccluderCurve = nullptr;
                const fb::Vec4* origRotationDistCurve = nullptr;

                switch (i)
                {
                case 1:
                    enable = &e->m_Element1Enable; alignedToRay = &e->m_Element1RotationAlignedToRay;
                    size = &e->m_Element1Size; rayDistance = &e->m_Element1RayDistance;
                    rotationLocal = &e->m_Element1RotationLocal; rotationDistMul = &e->m_Element1RotationDistMultiplier;
                    alphaScreenPosCurve = &e->m_Element1AlphaScreenPosCurve; alphaOccluderCurve = &e->m_Element1AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element1SizeScreenPosCurve; sizeOccluderCurve = &e->m_Element1SizeOccluderCurve;
                    rotationDistCurve = &e->m_Element1RotationDistCurve;
                    origEnable = &o->m_Element1Enable; origAlignedToRay = &o->m_Element1RotationAlignedToRay;
                    origSize = &o->m_Element1Size; origRayDistance = &o->m_Element1RayDistance;
                    origRotationLocal = &o->m_Element1RotationLocal; origRotationDistMul = &o->m_Element1RotationDistMultiplier;
                    origAlphaScreenPosCurve = &o->m_Element1AlphaScreenPosCurve; origAlphaOccluderCurve = &o->m_Element1AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element1SizeScreenPosCurve; origSizeOccluderCurve = &o->m_Element1SizeOccluderCurve;
                    origRotationDistCurve = &o->m_Element1RotationDistCurve;
                    break;
                case 2:
                    enable = &e->m_Element2Enable; alignedToRay = &e->m_Element2RotationAlignedToRay;
                    size = &e->m_Element2Size; rayDistance = &e->m_Element2RayDistance;
                    rotationLocal = &e->m_Element2RotationLocal; rotationDistMul = &e->m_Element2RotationDistMultiplier;
                    alphaScreenPosCurve = &e->m_Element2AlphaScreenPosCurve; alphaOccluderCurve = &e->m_Element2AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element2SizeScreenPosCurve; sizeOccluderCurve = &e->m_Element2SizeOccluderCurve;
                    rotationDistCurve = &e->m_Element2RotationDistCurve;
                    origEnable = &o->m_Element2Enable; origAlignedToRay = &o->m_Element2RotationAlignedToRay;
                    origSize = &o->m_Element2Size; origRayDistance = &o->m_Element2RayDistance;
                    origRotationLocal = &o->m_Element2RotationLocal; origRotationDistMul = &o->m_Element2RotationDistMultiplier;
                    origAlphaScreenPosCurve = &o->m_Element2AlphaScreenPosCurve; origAlphaOccluderCurve = &o->m_Element2AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element2SizeScreenPosCurve; origSizeOccluderCurve = &o->m_Element2SizeOccluderCurve;
                    origRotationDistCurve = &o->m_Element2RotationDistCurve;
                    break;
                case 3:
                    enable = &e->m_Element3Enable; alignedToRay = &e->m_Element3RotationAlignedToRay;
                    size = &e->m_Element3Size; rayDistance = &e->m_Element3RayDistance;
                    rotationLocal = &e->m_Element3RotationLocal; rotationDistMul = &e->m_Element3RotationDistMultiplier;
                    alphaScreenPosCurve = &e->m_Element3AlphaScreenPosCurve; alphaOccluderCurve = &e->m_Element3AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element3SizeScreenPosCurve; sizeOccluderCurve = &e->m_Element3SizeOccluderCurve;
                    rotationDistCurve = &e->m_Element3RotationDistCurve;
                    origEnable = &o->m_Element3Enable; origAlignedToRay = &o->m_Element3RotationAlignedToRay;
                    origSize = &o->m_Element3Size; origRayDistance = &o->m_Element3RayDistance;
                    origRotationLocal = &o->m_Element3RotationLocal; origRotationDistMul = &o->m_Element3RotationDistMultiplier;
                    origAlphaScreenPosCurve = &o->m_Element3AlphaScreenPosCurve; origAlphaOccluderCurve = &o->m_Element3AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element3SizeScreenPosCurve; origSizeOccluderCurve = &o->m_Element3SizeOccluderCurve;
                    origRotationDistCurve = &o->m_Element3RotationDistCurve;
                    break;
                case 4:
                    enable = &e->m_Element4Enable; alignedToRay = &e->m_Element4RotationAlignedToRay;
                    size = &e->m_Element4Size; rayDistance = &e->m_Element4RayDistance;
                    rotationLocal = &e->m_Element4RotationLocal; rotationDistMul = &e->m_Element4RotationDistMultiplier;
                    alphaScreenPosCurve = &e->m_Element4AlphaScreenPosCurve; alphaOccluderCurve = &e->m_Element4AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element4SizeScreenPosCurve; sizeOccluderCurve = &e->m_Element4SizeOccluderCurve;
                    rotationDistCurve = &e->m_Element4RotationDistCurve;
                    origEnable = &o->m_Element4Enable; origAlignedToRay = &o->m_Element4RotationAlignedToRay;
                    origSize = &o->m_Element4Size; origRayDistance = &o->m_Element4RayDistance;
                    origRotationLocal = &o->m_Element4RotationLocal; origRotationDistMul = &o->m_Element4RotationDistMultiplier;
                    origAlphaScreenPosCurve = &o->m_Element4AlphaScreenPosCurve; origAlphaOccluderCurve = &o->m_Element4AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element4SizeScreenPosCurve; origSizeOccluderCurve = &o->m_Element4SizeOccluderCurve;
                    origRotationDistCurve = &o->m_Element4RotationDistCurve;
                    break;
                case 5:
                    enable = &e->m_Element5Enable; alignedToRay = &e->m_Element5RotationAlignedToRay;
                    size = &e->m_Element5Size; rayDistance = &e->m_Element5RayDistance;
                    rotationLocal = &e->m_Element5RotationLocal; rotationDistMul = &e->m_Element5RotationDistMultiplier;
                    alphaScreenPosCurve = &e->m_Element5AlphaScreenPosCurve; alphaOccluderCurve = &e->m_Element5AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element5SizeScreenPosCurve; sizeOccluderCurve = &e->m_Element5SizeOccluderCurve;
                    rotationDistCurve = &e->m_Element5RotationDistCurve;
                    origEnable = &o->m_Element5Enable; origAlignedToRay = &o->m_Element5RotationAlignedToRay;
                    origSize = &o->m_Element5Size; origRayDistance = &o->m_Element5RayDistance;
                    origRotationLocal = &o->m_Element5RotationLocal; origRotationDistMul = &o->m_Element5RotationDistMultiplier;
                    origAlphaScreenPosCurve = &o->m_Element5AlphaScreenPosCurve; origAlphaOccluderCurve = &o->m_Element5AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element5SizeScreenPosCurve; origSizeOccluderCurve = &o->m_Element5SizeOccluderCurve;
                    origRotationDistCurve = &o->m_Element5RotationDistCurve;
                    break;
                }

                ui::BoolEdit("Enable", enable, origEnable);
                ui::BoolEdit("Rotation Aligned To Ray", alignedToRay, origAlignedToRay);
                ui::Vec2Edit("Size", size, origSize);
                ui::FloatEdit("Ray Distance", rayDistance, origRayDistance);
                ui::FloatEdit("Rotation Local", rotationLocal, origRotationLocal);
                ui::FloatEdit("Rotation Dist Multiplier", rotationDistMul, origRotationDistMul);
                ui::CurveVec4Edit("Alpha Screen Pos Curve", alphaScreenPosCurve, origAlphaScreenPosCurve);
                ui::CurveVec4Edit("Alpha Occluder Curve", alphaOccluderCurve, origAlphaOccluderCurve);
                ui::CurveVec4Edit("Size Screen Pos Curve", sizeScreenPosCurve, origSizeScreenPosCurve);
                ui::CurveVec4Edit("Size Occluder Curve", sizeOccluderCurve, origSizeOccluderCurve);
                ui::CurveVec4Edit("Rotation Dist Curve", rotationDistCurve, origRotationDistCurve);

                ImGui::TreePop();
            }
        }

        ImGui::Unindent();
    }

    void renderDynamicAOComponent(fb::CapturedDynamicAOComponentData* e, const fb::CapturedDynamicAOComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

        if (ImGui::TreeNode("SSAO##DAO"))
        {
            ui::FloatEdit("Radius", &e->m_SsaoRadius, &o->m_SsaoRadius);
            ui::FloatEdit("Fade", &e->m_SsaoFade, &o->m_SsaoFade);
            ui::FloatEdit("Max Distance Inner", &e->m_SsaoMaxDistanceInner, &o->m_SsaoMaxDistanceInner);
            ui::FloatEdit("Max Distance Outer", &e->m_SsaoMaxDistanceOuter, &o->m_SsaoMaxDistanceOuter);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("HBAO##DAO"))
        {
            ui::FloatEdit("Radius", &e->m_HbaoRadius, &o->m_HbaoRadius);
            ui::FloatEdit("Attenuation", &e->m_HbaoAttenuation, &o->m_HbaoAttenuation);
            ui::FloatEdit("Angle Bias", &e->m_HbaoAngleBias, &o->m_HbaoAngleBias);
            ui::FloatEdit("Power Exponent", &e->m_HbaoPowerExponent, &o->m_HbaoPowerExponent);
            ui::FloatEdit("Contrast", &e->m_HbaoContrast, &o->m_HbaoContrast);
            ui::FloatEdit("Max Footprint Radius", &e->m_HbaoMaxFootprintRadius, &o->m_HbaoMaxFootprintRadius);
            ImGui::TreePop();
        }
    }

    void renderDofComponent(fb::CapturedDofComponentData* e, const fb::CapturedDofComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Debug Draw Focus Plane", &e->m_DebugDrawFocusPlane, &o->m_DebugDrawFocusPlane);
        ui::FloatEdit("Focus Distance", &e->m_FocusDistance, &o->m_FocusDistance);
        ui::FloatEdit("Blur Factor", &e->m_BlurFactor, &o->m_BlurFactor);
        ui::FloatEdit("Blur Add", &e->m_BlurAdd, &o->m_BlurAdd);

        if (ImGui::TreeNode("Simple DoF##DoF"))
        {
            ui::EnumCombo<fb::BlurFilter>("Blur Filter##Simple",
                reinterpret_cast<int*>(&e->m_SimpleDofBlurFilter),
                reinterpret_cast<const int*>(&o->m_SimpleDofBlurFilter));
            ui::FloatEdit("Max Blur##Simple", &e->m_SimpleDofMaxBlur, &o->m_SimpleDofMaxBlur);
            ui::FloatEdit("Near Start##Simple", &e->m_SimpleDofNearStart, &o->m_SimpleDofNearStart);
            ui::FloatEdit("Near End##Simple", &e->m_SimpleDofNearEnd, &o->m_SimpleDofNearEnd);
            ui::FloatEdit("Far Start##Simple", &e->m_SimpleDofFarStart, &o->m_SimpleDofFarStart);
            ui::FloatEdit("Far End##Simple", &e->m_SimpleDofFarEnd, &o->m_SimpleDofFarEnd);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Ironsights DoF##DoF"))
        {
            ui::BoolEdit("Active##Iron", &e->m_IronsightsDofActive, &o->m_IronsightsDofActive);
            ui::BoolEdit("Circle Blur##Iron", &e->m_IronsightsDofCircleBlur, &o->m_IronsightsDofCircleBlur);
            ui::FloatEdit("Hip-to-Ironsights Fade", &e->m_HipToIronsightsFade, &o->m_HipToIronsightsFade, 0.0f, 1.0f);
            ui::FloatEdit("Start Fade##Iron", &e->m_IronsightsDofStartFade, &o->m_IronsightsDofStartFade);
            ui::FloatEdit("Focal Distance##Iron", &e->m_IronsightsFocalDistance, &o->m_IronsightsFocalDistance);
            ui::FloatEdit("Circle Distance##Iron", &e->m_IronsightsDofCircleDistance, &o->m_IronsightsDofCircleDistance);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Sprite DoF##DoF"))
        {
            ui::FloatEdit("Near Start##Sprite", &e->m_SpriteDofNearStart, &o->m_SpriteDofNearStart);
            ui::FloatEdit("Near End##Sprite", &e->m_SpriteDofNearEnd, &o->m_SpriteDofNearEnd);
            ui::FloatEdit("Far Start##Sprite", &e->m_SpriteDofFarStart, &o->m_SpriteDofFarStart);
            ui::FloatEdit("Far End##Sprite", &e->m_SpriteDofFarEnd, &o->m_SpriteDofFarEnd);
            ui::FloatEdit("Max Blur##Sprite", &e->m_SpriteDofMaxBlur, &o->m_SpriteDofMaxBlur);
            ImGui::TreePop();
        }
    }

    void renderVignetteComponent(fb::CapturedVignetteComponentData* e, const fb::CapturedVignetteComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::Vec3Edit("Color", &e->m_Color, &o->m_Color, true);
        ui::Vec2Edit("Scale", &e->m_Scale, &o->m_Scale);
        ui::FloatEdit("Opacity", &e->m_Opacity, &o->m_Opacity, 0.0f, 1.0f);
        ui::FloatEdit("Exponent", &e->m_Exponent, &o->m_Exponent);
    }

    void renderFilmGrainComponent(fb::CapturedFilmGrainComponentData* e, const fb::CapturedFilmGrainComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Linear Filtering", &e->m_LinearFilteringEnable, &o->m_LinearFilteringEnable);
        ui::BoolEdit("Random Enable", &e->m_RandomEnable, &o->m_RandomEnable);
        ui::Vec3Edit("Color Scale", &e->m_ColorScale, &o->m_ColorScale);
        ui::Vec2Edit("Texture Scale", &e->m_TextureScale, &o->m_TextureScale);
    }

    void renderLensScopeComponent(fb::CapturedLensScopeComponentData* e, const fb::CapturedLensScopeComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::Vec2Edit("Blur Center", &e->m_BlurCenter, &o->m_BlurCenter);
        ui::FloatEdit("Blur Scale", &e->m_BlurScale, &o->m_BlurScale);
        ui::Vec2Edit("Radial Blend Coeffs", &e->m_RadialBlendDistanceCoefficients, &o->m_RadialBlendDistanceCoefficients);

        if (ImGui::TreeNode("Chromatic Aberration##LS"))
        {
            ui::Vec3Edit("Color 1", &e->m_ChromaticAberrationColor1, &o->m_ChromaticAberrationColor1, true);
            ui::Vec3Edit("Color 2", &e->m_ChromaticAberrationColor2, &o->m_ChromaticAberrationColor2, true);
            ui::Vec2Edit("Displacement 1", &e->m_ChromaticAberrationDisplacement1, &o->m_ChromaticAberrationDisplacement1);
            ui::Vec2Edit("Displacement 2", &e->m_ChromaticAberrationDisplacement2, &o->m_ChromaticAberrationDisplacement2);
            ui::Vec2Edit("Strengths", &e->m_ChromaticAberrationStrengths, &o->m_ChromaticAberrationStrengths);
            ImGui::TreePop();
        }
    }

    void renderCameraParamsComponent(fb::CapturedCameraParamsComponentData* e, const fb::CapturedCameraParamsComponentData* o)
    {
        ui::FloatEdit("View Distance", &e->m_ViewDistance, &o->m_ViewDistance);
        ui::FloatEdit("Near Plane", &e->m_NearPlane, &o->m_NearPlane);
        ui::FloatEdit("Sun Shadowmap View Distance", &e->m_SunShadowmapViewDistance, &o->m_SunShadowmapViewDistance);
    }

    void renderScreenEffectComponent(fb::CapturedScreenEffectComponentData* e, const fb::CapturedScreenEffectComponentData* o)
    {
        ui::EnumCombo<fb::ScreenEffectFrameType>("Frame Type",
            reinterpret_cast<int*>(&e->m_FrameType),
            reinterpret_cast<const int*>(&o->m_FrameType));

        ui::Vec4Edit("Screen Effect Params", &e->m_ScreenEffectParams, &o->m_ScreenEffectParams);
        ui::FloatEdit("Frame Width", &e->m_FrameWidth, &o->m_FrameWidth);
        ui::FloatEdit("Outer Frame Opacity", &e->m_OuterFrameOpacity, &o->m_OuterFrameOpacity, 0.0f, 1.0f);
        ui::FloatEdit("Inner Frame Opacity", &e->m_InnerFrameOpacity, &o->m_InnerFrameOpacity, 0.0f, 1.0f);
        ui::FloatEdit("Angle", &e->m_Angle, &o->m_Angle, 0.0f, 360.0f);
    }

    void renderDamageEffectComponent(fb::CapturedDamageEffectComponentData* e, const fb::CapturedDamageEffectComponentData* o)
    {
        ui::BoolEdit("Debug Damage", &e->m_DebugDamage, &o->m_DebugDamage);

        if (ImGui::TreeNode("Damage Direction##DE"))
        {
            ui::Vec4Edit("Top", &e->m_TopDamage, &o->m_TopDamage);
            ui::Vec4Edit("Bottom", &e->m_BottomDamage, &o->m_BottomDamage);
            ui::Vec4Edit("Left", &e->m_LeftDamage, &o->m_LeftDamage);
            ui::Vec4Edit("Right", &e->m_RightDamage, &o->m_RightDamage);
            ImGui::TreePop();
        }

        ui::FloatEdit("Frame Width", &e->m_FrameWidth, &o->m_FrameWidth);
        ui::FloatEdit("Outer Frame Opacity", &e->m_OuterFrameOpacity, &o->m_OuterFrameOpacity, 0.0f, 1.0f);
        ui::FloatEdit("Inner Frame Opacity", &e->m_InnerFrameOpacity, &o->m_InnerFrameOpacity, 0.0f, 1.0f);
        ui::FloatEdit("Start Critical Threshold", &e->m_StartCriticalEffectHealthThreshold, &o->m_StartCriticalEffectHealthThreshold, 0.0f, 1.0f);
        ui::FloatEdit("End Critical Threshold", &e->m_EndCriticalEffectHealthThreshold, &o->m_EndCriticalEffectHealthThreshold, 0.0f, 1.0f);
        ui::FloatEdit("Min Damage % Threshold", &e->m_MinDamagePercentageThreshold, &o->m_MinDamagePercentageThreshold);
        ui::FloatEdit("Falloff Time", &e->m_FallofTime, &o->m_FallofTime);
        ui::FloatEdit("Max Opacity Damage %", &e->m_MaxOpacityDamagePercentage, &o->m_MaxOpacityDamagePercentage);
    }

    void renderPlanarReflectionComponent(fb::CapturedPlanarReflectionComponentData* e, const fb::CapturedPlanarReflectionComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Sky Render Enable", &e->m_SkyRenderEnable, &o->m_SkyRenderEnable);
        ui::BoolEdit("Terrain Reflections", &e->m_TerrainReflectionsEnable, &o->m_TerrainReflectionsEnable);
        ui::BoolEdit("Override Outdoor Light Colors", &e->m_OverideOutdoorLightColors, &o->m_OverideOutdoorLightColors);
        ui::FloatEdit("Ground Height", &e->m_GroundHeight, &o->m_GroundHeight);
        ui::FloatEdit("View Distance", &e->m_ViewDistance, &o->m_ViewDistance);

        if (ImGui::TreeNode("Outdoor Light Override Colors##PR"))
        {
            ui::Vec3Edit("Key Color", &e->m_KeyColorReflection, &o->m_KeyColorReflection, true);
            ui::Vec3Edit("Sky Color", &e->m_SkyColorReflection, &o->m_SkyColorReflection, true);
            ui::Vec3Edit("Ground Color", &e->m_GroundColorReflection, &o->m_GroundColorReflection, true);
            ImGui::TreePop();
        }

        ui::EnumCombo<fb::BlurFilter>("H Blur Filter",
            reinterpret_cast<int*>(&e->m_HorizontalBlurFilter),
            reinterpret_cast<const int*>(&o->m_HorizontalBlurFilter));
        ui::FloatEdit("H Deviation", &e->m_HorizontalDeviation, &o->m_HorizontalDeviation);

        ui::EnumCombo<fb::BlurFilter>("V Blur Filter",
            reinterpret_cast<int*>(&e->m_VerticalBlurFilter),
            reinterpret_cast<const int*>(&o->m_VerticalBlurFilter));
        ui::FloatEdit("V Deviation", &e->m_VerticalDeviation, &o->m_VerticalDeviation);
    }

    void renderDynamicEnvmapComponent(fb::CapturedDynamicEnvmapComponentData* e, const fb::CapturedDynamicEnvmapComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Terrain Reflections", &e->m_TerrainReflectionsEnable, &o->m_TerrainReflectionsEnable);
        ui::Vec3Edit("Sky Color", &e->m_SkyColorEnvmap, &o->m_SkyColorEnvmap, true);
        ui::Vec3Edit("Ground Color", &e->m_GroundColorEnvmap, &o->m_GroundColorEnvmap, true);
        ui::Vec3Edit("Key Color", &e->m_KeyColorEnvmap, &o->m_KeyColorEnvmap, true);
    }

    void renderCharacterLightingComponent(fb::CapturedCharacterLightingComponentData* e, const fb::CapturedCharacterLightingComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_CharacterLightEnable, &o->m_CharacterLightEnable);
        ui::BoolEdit("First Person Enable", &e->m_FirstPersonEnable, &o->m_FirstPersonEnable);
        ui::BoolEdit("Lock to Camera Direction", &e->m_LockToCameraDirection, &o->m_LockToCameraDirection);

        ui::EnumCombo<fb::CharacterLightingMode>("Lighting Mode",
            reinterpret_cast<int*>(&e->m_CharacterLightingMode),
            reinterpret_cast<const int*>(&o->m_CharacterLightingMode));

        ui::Vec3Edit("Top Light", &e->m_TopLight, &o->m_TopLight, true);
        ui::Vec3Edit("Bottom Light", &e->m_BottomLight, &o->m_BottomLight, true);
        ui::FloatEdit("Top Light Dir X", &e->m_TopLightDirX, &o->m_TopLightDirX);
        ui::FloatEdit("Top Light Dir Y", &e->m_TopLightDirY, &o->m_TopLightDirY);
        ui::FloatEdit("Camera Up Rotation", &e->m_CameraUpRotation, &o->m_CameraUpRotation);
        ui::FloatEdit("Blend Factor", &e->m_BlendFactor, &o->m_BlendFactor, 0.0f, 1.0f);

        if (ImGui::TreeNode("Fade Distances##CL"))
        {
            ui::FloatEdit("Start Fade Distance", &e->m_StartFadeDistance, &o->m_StartFadeDistance);
            ui::FloatEdit("End Fade Distance", &e->m_EndFadeDistance, &o->m_EndFadeDistance);
            ui::FloatEdit("Start Fade Out Distance", &e->m_StartFadeOutDistance, &o->m_StartFadeOutDistance);
            ui::FloatEdit("End Fade Out Distance", &e->m_EndFadeOutDistance, &o->m_EndFadeOutDistance);
            ImGui::TreePop();
        }
    }

    void renderMotionBlurComponent(fb::CapturedMotionBlurComponentData* e, const fb::CapturedMotionBlurComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_MotionBlurEnable, &o->m_MotionBlurEnable);
        ui::BoolEdit("Centered", &e->m_MotionBlurCentered, &o->m_MotionBlurCentered);
        ui::FloatEdit("Scale", &e->m_MotionBlurScale, &o->m_MotionBlurScale, 0.0f, 2.0f);
        ui::FloatEdit("Cutoff Radius", &e->m_MotionBlurCutoffRadius, &o->m_MotionBlurCutoffRadius);
        ui::FloatEdit("Cutoff Gradient Scale", &e->m_CutoffGradientScale, &o->m_CutoffGradientScale);

        if (ImGui::TreeNode("Radial Blur##MB"))
        {
            ui::BoolEdit("Enable##Radial", &e->m_RadialBlurEnable, &o->m_RadialBlurEnable);
            ui::Vec2Edit("Center", &e->m_RadialBlurCenter, &o->m_RadialBlurCenter);
            ui::FloatEdit("Offset", &e->m_RadialBlurOffset, &o->m_RadialBlurOffset);
            ui::FloatEdit("Scale##Radial", &e->m_RadialBlurScale, &o->m_RadialBlurScale);
            ImGui::TreePop();
        }
    }

    void renderVehicleLightingComponent(fb::CapturedVehicleLightingComponentData* e, const fb::CapturedVehicleLightingComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_VehicleLightEnable, &o->m_VehicleLightEnable);
        ui::BoolEdit("First Person Enable", &e->m_FirstPersonEnable, &o->m_FirstPersonEnable);
        ui::BoolEdit("Lock to Camera Direction", &e->m_LockToCameraDirection, &o->m_LockToCameraDirection);

        ui::EnumCombo<fb::VehicleLightingMode>("Lighting Mode",
            reinterpret_cast<int*>(&e->m_VehicleLightingMode),
            reinterpret_cast<const int*>(&o->m_VehicleLightingMode));

        ui::Vec3Edit("Top Light", &e->m_TopLight, &o->m_TopLight, true);
        ui::Vec3Edit("Bottom Light", &e->m_BottomLight, &o->m_BottomLight, true);
        ui::FloatEdit("Top Light Dir X", &e->m_TopLightDirX, &o->m_TopLightDirX);
        ui::FloatEdit("Top Light Dir Y", &e->m_TopLightDirY, &o->m_TopLightDirY);
        ui::FloatEdit("Camera Up Rotation", &e->m_CameraUpRotation, &o->m_CameraUpRotation);
        ui::FloatEdit("Blend Factor", &e->m_BlendFactor, &o->m_BlendFactor, 0.0f, 1.0f);

        if (ImGui::TreeNode("Fade Distances##VL"))
        {
            ui::FloatEdit("Start Fade Distance", &e->m_StartFadeDistance, &o->m_StartFadeDistance);
            ui::FloatEdit("End Fade Distance", &e->m_EndFadeDistance, &o->m_EndFadeDistance);
            ui::FloatEdit("Start Fade Out Distance", &e->m_StartFadeOutDistance, &o->m_StartFadeOutDistance);
            ui::FloatEdit("End Fade Out Distance", &e->m_EndFadeOutDistance, &o->m_EndFadeOutDistance);
            ImGui::TreePop();
        }
    }

    void renderSubSurfaceScatteringComponent(fb::CapturedSubSurfaceScatteringComponentData* e, const fb::CapturedSubSurfaceScatteringComponentData* o)
    {
        ui::BoolEdit("Advanced SSS Enable", &e->m_AdvancedSssEnable, &o->m_AdvancedSssEnable);

        if (ImGui::TreeNode("Simple SSS##SSS"))
        {
            ui::Vec3Edit("Color", &e->m_SimpleSssColor, &o->m_SimpleSssColor, true);
            ui::FloatEdit("Rolloff Key Light", &e->m_SimpleSssRolloffKeyLight, &o->m_SimpleSssRolloffKeyLight);
            ui::FloatEdit("Rolloff Local Light", &e->m_SimpleSssRolloffLocalLight, &o->m_SimpleSssRolloffLocalLight);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Advanced SSS Material Widths##SSS"))
        {
            ui::Vec3Edit("Material 0", &e->m_AdvancedSssMat0Width, &o->m_AdvancedSssMat0Width);
            ui::Vec3Edit("Material 1", &e->m_AdvancedSssMat1Width, &o->m_AdvancedSssMat1Width);
            ui::Vec3Edit("Material 2", &e->m_AdvancedSssMat2Width, &o->m_AdvancedSssMat2Width);
            ui::Vec3Edit("Material 3", &e->m_AdvancedSssMat3Width, &o->m_AdvancedSssMat3Width);
            ui::Vec3Edit("Material 4", &e->m_AdvancedSssMat4Width, &o->m_AdvancedSssMat4Width);
            ImGui::TreePop();
        }
    }

    void renderWorldRenderSettingsComponent(fb::WorldRenderSettings* e, const fb::WorldRenderSettings* o)
    {
        ImGui::TextDisabled("WorldRenderSettings — use the World Render tab for full control");
        ImGui::Separator();
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("HDR", &e->m_HdrEnable, &o->m_HdrEnable);
        ui::BoolEdit("Sky Enable", &e->m_SkyEnable, &o->m_SkyEnable);
        ui::BoolEdit("Sky Fog Enable", &e->m_SkyFogEnable, &o->m_SkyFogEnable);
        ui::BoolEdit("Motion Blur Enable", &e->m_MotionBlurEnable, &o->m_MotionBlurEnable);
        ui::BoolEdit("Shadowmaps Enable", &e->m_ShadowmapsEnable, &o->m_ShadowmapsEnable);
    }
}

#endif // BFVE_GAME_BF4
