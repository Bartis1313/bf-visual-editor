#include "comps.h"

#include "../ui/ui_helpers.h"

namespace editor::comps
{
    void renderOutdoorLightComponent(fb::OutdoorLightComponentData* e, const fb::OutdoorLightComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

        if (ImGui::TreeNode("Sun##OL"))
        {
            ui::Vec3Edit("Sun Color", &e->m_SunColor, &o->m_SunColor, true);
            // temp solution to normalize, engine will crash on ub values
            ui::FloatEdit("Sun Rotation X", &e->m_SunRotationX, &o->m_SunRotationX, -179.9f, 179.9f);
            ui::FloatEdit("Sun Rotation Y", &e->m_SunRotationY, &o->m_SunRotationY, -179.9f, 179.9f);
            ui::FloatEdit("Sun Specular Scale", &e->m_SunSpecularScale, &o->m_SunSpecularScale);
            ui::FloatEdit("Sun Shadow Height Scale", &e->m_SunShadowHeightScale, &o->m_SunShadowHeightScale);
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
            ui::Vec2Edit("Speed", &e->m_CloudShadowSpeed, &o->m_CloudShadowSpeed);
            ui::FloatEdit("Coverage", &e->m_CloudShadowCoverage, &o->m_CloudShadowCoverage, 0.0f, 1.0f);
            ui::FloatEdit("Size", &e->m_CloudShadowSize, &o->m_CloudShadowSize);
            ui::FloatEdit("Exponent", &e->m_CloudShadowExponent, &o->m_CloudShadowExponent);
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

    void renderEnlightenComponent(fb::EnlightenComponentData* e, const fb::EnlightenComponentData* o)
    {
        ImGui::Text("Disabling is not prefered!\nDisable if you want to set custom ground/sky lights");

        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("SkyBox Enable", &e->m_SkyBoxEnable, &o->m_SkyBoxEnable);

        if (ImGui::TreeNode("SkyBox Colors##EN"))
        {
            ImGui::Text("These only work with dynamic envmap or something, I don't know");

            ui::Vec3Edit("Sky Color", &e->m_SkyBoxSkyColor, &o->m_SkyBoxSkyColor, true);
            ui::Vec3Edit("Ground Color", &e->m_SkyBoxGroundColor, &o->m_SkyBoxGroundColor, true);
            ui::Vec3Edit("Sun Light Color", &e->m_SkyBoxSunLightColor, &o->m_SkyBoxSunLightColor, true);
            ui::Vec3Edit("Back Light Color", &e->m_SkyBoxBackLightColor, &o->m_SkyBoxBackLightColor, true);
            ui::Vec3Edit("Terrain Color", &e->m_TerrainColor, &o->m_TerrainColor, true);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Light Settings##EN"))
        {
            ui::FloatEdit("Bounce Scale", &e->m_BounceScale, &o->m_BounceScale);
            ui::FloatEdit("Sun Scale", &e->m_SunScale, &o->m_SunScale);
            ui::FloatEdit("Cull Distance", &e->m_CullDistance, &o->m_CullDistance);
            ui::FloatEdit("Sun Light Color Size", &e->m_SkyBoxSunLightColorSize, &o->m_SkyBoxSunLightColorSize);
            ui::FloatEdit("Back Light Color Size", &e->m_SkyBoxBackLightColorSize, &o->m_SkyBoxBackLightColorSize);
            ui::FloatEdit("Back Light Rotation X", &e->m_SkyBoxBackLightRotationX, &o->m_SkyBoxBackLightRotationX);
            ui::FloatEdit("Back Light Rotation Y", &e->m_SkyBoxBackLightRotationY, &o->m_SkyBoxBackLightRotationY);
            ImGui::TreePop();
        }
    }

    void renderTonemapComponent(fb::TonemapComponentData* e, const fb::TonemapComponentData* o)
    {
        int method = static_cast<int>(e->m_TonemapMethod);
        int origMethod = static_cast<int>(o->m_TonemapMethod);
        if (ui::EnumCombo<fb::TonemapMethod>("Tonemap Method", &method, &origMethod))
            e->m_TonemapMethod = static_cast<fb::TonemapMethod>(method);

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

    void renderColorCorrectionComponent(fb::ColorCorrectionComponentData* e, const fb::ColorCorrectionComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Color Grading Enable", &e->m_ColorGradingEnable, &o->m_ColorGradingEnable);
        ui::Vec3Edit("Brightness", &e->m_Brightness, &o->m_Brightness);
        ui::Vec3Edit("Contrast", &e->m_Contrast, &o->m_Contrast);
        ui::Vec3Edit("Saturation", &e->m_Saturation, &o->m_Saturation);
        ui::FloatEdit("Hue", &e->m_Hue, &o->m_Hue, -180.0f, 180.0f);
    }

    void renderSkyComponent(fb::SkyComponentData* e, const fb::SkyComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

        if (ImGui::TreeNode("General##Sky"))
        {
            ui::FloatEdit("Brightness Scale", &e->m_BrightnessScale, &o->m_BrightnessScale);
            ui::FloatEdit("Sun Size", &e->m_SunSize, &o->m_SunSize);
            ui::FloatEdit("Sun Scale", &e->m_SunScale, &o->m_SunScale);
            ui::FloatEdit("Sky Visibility Exponent", &e->m_SkyVisibilityExponent, &o->m_SkyVisibilityExponent);
            ui::FloatEdit("Sky Envmap 8Bit Tex Scale", &e->m_SkyEnvmap8BitTexScale, &o->m_SkyEnvmap8BitTexScale);
            ImGui::TreePop();
        }

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

        ui::Vec3Edit("Cloud Layer Sun Color", &e->m_CloudLayerSunColor, &o->m_CloudLayerSunColor, true);

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

    void renderFogComponent(fb::FogComponentData* e, const fb::FogComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

        if (ImGui::TreeNode("Basic Fog##Fog"))
        {
            ui::FloatEdit("Start", &e->m_Start, &o->m_Start);
            ui::FloatEdit("End", &e->m_End, &o->m_End);
            ui::FloatEdit("Distance Multiplier", &e->m_FogDistanceMultiplier, &o->m_FogDistanceMultiplier);
            ui::Vec4Edit("Curve", &e->m_Curve, &o->m_Curve);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Fog Color##Fog"))
        {
            ui::BoolEdit("Enable##FogColor", &e->m_FogColorEnable, &o->m_FogColorEnable);
            ui::Vec3Edit("Color", &e->m_FogColor, &o->m_FogColor, true);
            ui::FloatEdit("Color Start", &e->m_FogColorStart, &o->m_FogColorStart);
            ui::FloatEdit("Color End", &e->m_FogColorEnd, &o->m_FogColorEnd);
            ui::Vec4Edit("Color Curve", &e->m_FogColorCurve, &o->m_FogColorCurve);
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
            ImGui::TreePop();
        }
    }

    void renderWindComponent(fb::WindComponentData* e, const fb::WindComponentData* o)
    {
        ui::FloatEdit("Direction", &e->m_WindDirection, &o->m_WindDirection, 0.0f, 360.0f);
        ui::FloatEdit("Strength", &e->m_WindStrength, &o->m_WindStrength);
    }

    void renderSunFlareComponent(fb::SunFlareComponentData* e, const fb::SunFlareComponentData* o)
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
                fb::Vec2* size = nullptr;
                float* rayDistance = nullptr;
                fb::Vec4* alphaScreenPosCurve = nullptr;
                fb::Vec4* alphaOccluderCurve = nullptr;
                fb::Vec4* sizeScreenPosCurve = nullptr;
                fb::Vec4* sizeOccluderCurve = nullptr;

                const bool* origEnable = nullptr;
                const fb::Vec2* origSize = nullptr;
                const float* origRayDistance = nullptr;
                const fb::Vec4* origAlphaScreenPosCurve = nullptr;
                const fb::Vec4* origAlphaOccluderCurve = nullptr;
                const fb::Vec4* origSizeScreenPosCurve = nullptr;
                const fb::Vec4* origSizeOccluderCurve = nullptr;

                // should have edited sdk dump for this
                switch (i)
                {
                case 1:
                    enable = &e->m_Element1Enable;
                    size = &e->m_Element1Size;
                    rayDistance = &e->m_Element1RayDistance;
                    alphaScreenPosCurve = &e->m_Element1AlphaScreenPosCurve;
                    alphaOccluderCurve = &e->m_Element1AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element1SizeScreenPosCurve;
                    sizeOccluderCurve = &e->m_Element1SizeOccluderCurve;
                    origEnable = &o->m_Element1Enable;
                    origSize = &o->m_Element1Size;
                    origRayDistance = &o->m_Element1RayDistance;
                    origAlphaScreenPosCurve = &o->m_Element1AlphaScreenPosCurve;
                    origAlphaOccluderCurve = &o->m_Element1AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element1SizeScreenPosCurve;
                    origSizeOccluderCurve = &o->m_Element1SizeOccluderCurve;
                    break;
                case 2:
                    enable = &e->m_Element2Enable;
                    size = &e->m_Element2Size;
                    rayDistance = &e->m_Element2RayDistance;
                    alphaScreenPosCurve = &e->m_Element2AlphaScreenPosCurve;
                    alphaOccluderCurve = &e->m_Element2AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element2SizeScreenPosCurve;
                    sizeOccluderCurve = &e->m_Element2SizeOccluderCurve;
                    origEnable = &o->m_Element2Enable;
                    origSize = &o->m_Element2Size;
                    origRayDistance = &o->m_Element2RayDistance;
                    origAlphaScreenPosCurve = &o->m_Element2AlphaScreenPosCurve;
                    origAlphaOccluderCurve = &o->m_Element2AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element2SizeScreenPosCurve;
                    origSizeOccluderCurve = &o->m_Element2SizeOccluderCurve;
                    break;
                case 3:
                    enable = &e->m_Element3Enable;
                    size = &e->m_Element3Size;
                    rayDistance = &e->m_Element3RayDistance;
                    alphaScreenPosCurve = &e->m_Element3AlphaScreenPosCurve;
                    alphaOccluderCurve = &e->m_Element3AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element3SizeScreenPosCurve;
                    sizeOccluderCurve = &e->m_Element3SizeOccluderCurve;
                    origEnable = &o->m_Element3Enable;
                    origSize = &o->m_Element3Size;
                    origRayDistance = &o->m_Element3RayDistance;
                    origAlphaScreenPosCurve = &o->m_Element3AlphaScreenPosCurve;
                    origAlphaOccluderCurve = &o->m_Element3AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element3SizeScreenPosCurve;
                    origSizeOccluderCurve = &o->m_Element3SizeOccluderCurve;
                    break;
                case 4:
                    enable = &e->m_Element4Enable;
                    size = &e->m_Element4Size;
                    rayDistance = &e->m_Element4RayDistance;
                    alphaScreenPosCurve = &e->m_Element4AlphaScreenPosCurve;
                    alphaOccluderCurve = &e->m_Element4AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element4SizeScreenPosCurve;
                    sizeOccluderCurve = &e->m_Element4SizeOccluderCurve;
                    origEnable = &o->m_Element4Enable;
                    origSize = &o->m_Element4Size;
                    origRayDistance = &o->m_Element4RayDistance;
                    origAlphaScreenPosCurve = &o->m_Element4AlphaScreenPosCurve;
                    origAlphaOccluderCurve = &o->m_Element4AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element4SizeScreenPosCurve;
                    origSizeOccluderCurve = &o->m_Element4SizeOccluderCurve;
                    break;
                case 5:
                    enable = &e->m_Element5Enable;
                    size = &e->m_Element5Size;
                    rayDistance = &e->m_Element5RayDistance;
                    alphaScreenPosCurve = &e->m_Element5AlphaScreenPosCurve;
                    alphaOccluderCurve = &e->m_Element5AlphaOccluderCurve;
                    sizeScreenPosCurve = &e->m_Element5SizeScreenPosCurve;
                    sizeOccluderCurve = &e->m_Element5SizeOccluderCurve;
                    origEnable = &o->m_Element5Enable;
                    origSize = &o->m_Element5Size;
                    origRayDistance = &o->m_Element5RayDistance;
                    origAlphaScreenPosCurve = &o->m_Element5AlphaScreenPosCurve;
                    origAlphaOccluderCurve = &o->m_Element5AlphaOccluderCurve;
                    origSizeScreenPosCurve = &o->m_Element5SizeScreenPosCurve;
                    origSizeOccluderCurve = &o->m_Element5SizeOccluderCurve;
                    break;
                }

                ui::BoolEdit("Enable", enable, origEnable);
                ui::Vec2Edit("Size", size, origSize);
                ui::FloatEdit("Ray Distance", rayDistance, origRayDistance);
                ui::Vec4Edit("Alpha Screen Pos Curve", alphaScreenPosCurve, origAlphaScreenPosCurve);
                ui::Vec4Edit("Alpha Occluder Curve", alphaOccluderCurve, origAlphaOccluderCurve);
                ui::Vec4Edit("Size Screen Pos Curve", sizeScreenPosCurve, origSizeScreenPosCurve);
                ui::Vec4Edit("Size Occluder Curve", sizeOccluderCurve, origSizeOccluderCurve);

                ImGui::TreePop();
            }
        }

        ImGui::Unindent();
    }

    void renderDynamicAOComponent(fb::DynamicAOComponentData* e, const fb::DynamicAOComponentData* o)
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

    void renderDofComponent(fb::DofComponentData* e, const fb::DofComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);

        int filter = static_cast<int>(e->m_BlurFilter);
        int origFilter = static_cast<int>(o->m_BlurFilter);
        if (ui::EnumCombo<fb::BlurFilter>("Blur Filter", &filter, &origFilter))
            e->m_BlurFilter = static_cast<fb::BlurFilter>(filter);

        ui::FloatEdit("Focus Distance", &e->m_FocusDistance, &o->m_FocusDistance);
        ui::FloatEdit("Near Distance Scale", &e->m_NearDistanceScale, &o->m_NearDistanceScale);
        ui::FloatEdit("Far Distance Scale", &e->m_FarDistanceScale, &o->m_FarDistanceScale);
        ui::FloatEdit("Blur Filter Deviation", &e->m_BlurFilterDeviation, &o->m_BlurFilterDeviation);
        ui::FloatEdit("Blur Add", &e->m_BlurAdd, &o->m_BlurAdd);
        ui::FloatEdit("Scale", &e->m_Scale, &o->m_Scale);

        if (ImGui::TreeNode("Diffusion DoF##DoF"))
        {
            ui::BoolEdit("Enable##DiffDoF", &e->m_DiffusionDofEnable, &o->m_DiffusionDofEnable);
            ui::FloatEdit("Focal Length", &e->m_DiffusionDofFocalLength, &o->m_DiffusionDofFocalLength);
            ui::FloatEdit("Aperture", &e->m_DiffusionDofAperture, &o->m_DiffusionDofAperture);
            ImGui::TreePop();
        }
    }

    void renderVignetteComponent(fb::VignetteComponentData* e, const fb::VignetteComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::Vec3Edit("Color", &e->m_Color, &o->m_Color, true);
        ui::Vec2Edit("Scale", &e->m_Scale, &o->m_Scale);
        ui::FloatEdit("Opacity", &e->m_Opacity, &o->m_Opacity, 0.0f, 1.0f);
        ui::FloatEdit("Exponent", &e->m_Exponent, &o->m_Exponent);
    }

    void renderFilmGrainComponent(fb::FilmGrainComponentData* e, const fb::FilmGrainComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Linear Filtering", &e->m_LinearFilteringEnable, &o->m_LinearFilteringEnable);
        ui::BoolEdit("Random Enable", &e->m_RandomEnable, &o->m_RandomEnable);
        ui::Vec3Edit("Color Scale", &e->m_ColorScale, &o->m_ColorScale);
        ui::Vec2Edit("Texture Scale", &e->m_TextureScale, &o->m_TextureScale);
    }

    void renderLensScopeComponent(fb::LensScopeComponentData* e, const fb::LensScopeComponentData* o)
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

    void renderCameraParamsComponent(fb::CameraParamsComponentData* e, const fb::CameraParamsComponentData* o)
    {
        ui::FloatEdit("View Distance", &e->m_ViewDistance, &o->m_ViewDistance);
        ui::FloatEdit("Near Plane", &e->m_NearPlane, &o->m_NearPlane);
        ui::FloatEdit("Sun Shadowmap View Distance", &e->m_SunShadowmapViewDistance, &o->m_SunShadowmapViewDistance);
    }

    void renderScreenEffectComponent(fb::ScreenEffectComponentData* e, const fb::ScreenEffectComponentData* o)
    {
        int type = static_cast<int>(e->m_FrameType);
        int origType = static_cast<int>(o->m_FrameType);
        if (ui::EnumCombo<fb::ScreenEffectFrameType>("Frame Type", &type, &origType))
            e->m_FrameType = static_cast<fb::ScreenEffectFrameType>(type);

        ui::Vec4Edit("Screen Effect Params", &e->m_ScreenEffectParams, &o->m_ScreenEffectParams);
        ui::FloatEdit("Frame Width", &e->m_FrameWidth, &o->m_FrameWidth);
        ui::FloatEdit("Outer Frame Opacity", &e->m_OuterFrameOpacity, &o->m_OuterFrameOpacity, 0.0f, 1.0f);
        ui::FloatEdit("Inner Frame Opacity", &e->m_InnerFrameOpacity, &o->m_InnerFrameOpacity, 0.0f, 1.0f);
        ui::FloatEdit("Angle", &e->m_Angle, &o->m_Angle, 0.0f, 360.0f);
    }

    void renderDamageEffectComponent(fb::DamageEffectComponentData* e, const fb::DamageEffectComponentData* o)
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

    void renderPlanarReflectionComponent(fb::PlanarReflectionComponentData* e, const fb::PlanarReflectionComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::BoolEdit("Sky Render Enable", &e->m_SkyRenderEnable, &o->m_SkyRenderEnable);
        ui::FloatEdit("Ground Height", &e->m_GroundHeight, &o->m_GroundHeight);

        int hFilter = static_cast<int>(e->m_HorizontalBlurFilter);
        int origHFilter = static_cast<int>(o->m_HorizontalBlurFilter);
        if (ui::EnumCombo<fb::BlurFilter>("H Blur Filter", &hFilter, &origHFilter))
            e->m_HorizontalBlurFilter = static_cast<fb::BlurFilter>(hFilter);

        ui::FloatEdit("H Deviation", &e->m_HorizontalDeviation, &o->m_HorizontalDeviation);

        int vFilter = static_cast<int>(e->m_VerticalBlurFilter);
        int origVFilter = static_cast<int>(o->m_VerticalBlurFilter);
        if (ui::EnumCombo<fb::BlurFilter>("V Blur Filter", &vFilter, &origVFilter))
            e->m_VerticalBlurFilter = static_cast<fb::BlurFilter>(vFilter);

        ui::FloatEdit("V Deviation", &e->m_VerticalDeviation, &o->m_VerticalDeviation);
    }

    void renderDynamicEnvmapComponent(fb::DynamicEnvmapComponentData* e, const fb::DynamicEnvmapComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
        ui::Vec3Edit("Sky Color", &e->m_SkyColorEnvmap, &o->m_SkyColorEnvmap, true);
        ui::Vec3Edit("Ground Color", &e->m_GroundColorEnvmap, &o->m_GroundColorEnvmap, true);
        ui::Vec3Edit("Key Color", &e->m_KeyColorEnvmap, &o->m_KeyColorEnvmap, true);
    }

    void renderCharacterLightingComponent(fb::CharacterLightingComponentData* e, const fb::CharacterLightingComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_CharacterLightEnable, &o->m_CharacterLightEnable);
        ui::BoolEdit("First Person Enable", &e->m_FirstPersonEnable, &o->m_FirstPersonEnable);
        ui::BoolEdit("Lock to Camera Direction", &e->m_LockToCameraDirection, &o->m_LockToCameraDirection);

        int mode = static_cast<int>(e->m_CharacterLightingMode);
        int origMode = static_cast<int>(o->m_CharacterLightingMode);
        if (ui::EnumCombo<fb::CharacterLightingMode>("Lighting Mode", &mode, &origMode))
            e->m_CharacterLightingMode = static_cast<fb::CharacterLightingMode>(mode);

        ui::Vec3Edit("Top Light", &e->m_TopLight, &o->m_TopLight, true);
        ui::Vec3Edit("Bottom Light", &e->m_BottomLight, &o->m_BottomLight, true);
        ui::FloatEdit("Top Light Dir X", &e->m_TopLightDirX, &o->m_TopLightDirX);
        ui::FloatEdit("Top Light Dir Y", &e->m_TopLightDirY, &o->m_TopLightDirY);
        ui::FloatEdit("Camera Up Rotation", &e->m_CameraUpRotation, &o->m_CameraUpRotation);
        ui::FloatEdit("Blend Factor", &e->m_BlendFactor, &o->m_BlendFactor, 0.0f, 1.0f);
    }

    void renderMotionBlurComponent(fb::MotionBlurComponentData* e, const fb::MotionBlurComponentData* o)
    {
        ui::BoolEdit("Enable", &e->m_MotionBlurEnable, &o->m_MotionBlurEnable);
        ui::FloatEdit("Cutoff Radius", &e->m_MotionBlurCutoffRadius, &o->m_MotionBlurCutoffRadius);
        ui::FloatEdit("Cutoff Gradient Scale", &e->m_CutoffGradientScale, &o->m_CutoffGradientScale);
    }

    void renderWorldRenderSettingsComponent(fb::WorldRenderSettings* e, const fb::WorldRenderSettings* o)
    {
        if (ImGui::TreeNode("Viewport & Quality"))
        {
            ui::FloatEdit("Viewport Scale", &e->m_ViewportScale, &o->m_ViewportScale, 0.1f, 2.0f);
            ui::UIntEdit("FXAA Quality", &e->m_FxaaQuality, &o->m_FxaaQuality, 0, 5);
            ui::IntEdit("FXAA Force Vendor", &e->m_FxaaForceVendor, &o->m_FxaaForceVendor, -1, 3);
            ui::BoolEdit("FXAA Enable", &e->m_FxaaEnable, &o->m_FxaaEnable);
            ui::FloatEdit("Cull Screen Area Scale", &e->m_CullScreenAreaScale, &o->m_CullScreenAreaScale);
            ui::UIntEdit("Multisample Count", &e->m_MultisampleCount, &o->m_MultisampleCount, 1, 8);
            ui::FloatEdit("Multisample Threshold", &e->m_MultisampleThreshold, &o->m_MultisampleThreshold);
            ui::BoolEdit("HDR Enable", &e->m_HdrEnable, &o->m_HdrEnable);
            ui::BoolEdit("Half Res Enable", &e->m_HalfResEnable, &o->m_HalfResEnable);
            ui::BoolEdit("Bilateral Half Res Composite", &e->m_BilateralHalfResCompositeEnable, &o->m_BilateralHalfResCompositeEnable);
            ui::BoolEdit("Tiled Half Res Composite", &e->m_TiledHalfResCompositeEnable, &o->m_TiledHalfResCompositeEnable);
            ui::BoolEdit("Tiled Half Res Stencil Occluders", &e->m_TiledHalfResStencilOccludersEnable, &o->m_TiledHalfResStencilOccludersEnable);
            ui::BoolEdit("Output Gamma Correction", &e->m_OutputGammaCorrectionEnable, &o->m_OutputGammaCorrectionEnable);
            ui::BoolEdit("Enable", &e->m_Enable, &o->m_Enable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Shadows"))
        {
            ui::BoolEdit("Shadowmaps Enable", &e->m_ShadowmapsEnable, &o->m_ShadowmapsEnable);
            ui::BoolEdit("Generate Shadowmaps", &e->m_GenerateShadowmapsEnable, &o->m_GenerateShadowmapsEnable);
            ui::BoolEdit("Apply Shadowmaps", &e->m_ApplyShadowmapsEnable, &o->m_ApplyShadowmapsEnable);
            ui::BoolEdit("Simple Shadowmaps", &e->m_SimpleShadowmapsEnable, &o->m_SimpleShadowmapsEnable);
            ui::BoolEdit("Transparency Shadowmaps", &e->m_TransparencyShadowmapsEnable, &o->m_TransparencyShadowmapsEnable);
            ui::BoolEdit("Cloud Shadow", &e->m_CloudShadowEnable, &o->m_CloudShadowEnable);

            ImGui::Separator();
            ui::UIntEdit("Resolution", &e->m_ShadowmapResolution, &o->m_ShadowmapResolution, 256, 4096);
            ui::UIntEdit("Quality", &e->m_ShadowmapQuality, &o->m_ShadowmapQuality, 0, 4);
            ui::UIntEdit("Slice Count", &e->m_ShadowmapSliceCount, &o->m_ShadowmapSliceCount, 1, 8);
            ui::FloatEdit("View Distance", &e->m_ShadowmapViewDistance, &o->m_ShadowmapViewDistance);
            ui::BoolEdit("View Distance Scale Enable", &e->m_ShadowmapViewDistanceScaleEnable, &o->m_ShadowmapViewDistanceScaleEnable);
            ui::FloatEdit("Min Screen Area", &e->m_ShadowMinScreenArea, &o->m_ShadowMinScreenArea);
            ui::FloatEdit("Shadowmap Min Screen Area", &e->m_ShadowmapMinScreenArea, &o->m_ShadowmapMinScreenArea);
            ui::FloatEdit("Viewport Scale", &e->m_ShadowViewportScale, &o->m_ShadowViewportScale);
            ui::FloatEdit("Size Z Scale", &e->m_ShadowmapSizeZScale, &o->m_ShadowmapSizeZScale);
            ui::FloatEdit("Slice Scheme Weight", &e->m_ShadowmapSliceSchemeWeight, &o->m_ShadowmapSliceSchemeWeight);
            ui::FloatEdit("First Slice Scale", &e->m_ShadowmapFirstSliceScale, &o->m_ShadowmapFirstSliceScale);
            ui::FloatEdit("Extrusion Length", &e->m_ShadowmapExtrusionLength, &o->m_ShadowmapExtrusionLength);
            ui::FloatEdit("Min FOV", &e->m_ShadowmapMinFov, &o->m_ShadowmapMinFov);
            ui::FloatEdit("Poisson Filter Scale", &e->m_ShadowmapPoissonFilterScale, &o->m_ShadowmapPoissonFilterScale);

            ImGui::Separator();
            ui::BoolEdit("Fixed Depth", &e->m_ShadowmapFixedDepthEnable, &o->m_ShadowmapFixedDepthEnable);
            ui::BoolEdit("Fixed Movement", &e->m_ShadowmapFixedMovementEnable, &o->m_ShadowmapFixedMovementEnable);
            ui::BoolEdit("Cull Volume", &e->m_ShadowmapCullVolumeEnable, &o->m_ShadowmapCullVolumeEnable);
            ui::BoolEdit("Accum Enable", &e->m_ShadowmapAccumEnable, &o->m_ShadowmapAccumEnable);
            ui::BoolEdit("Accum Bilinear", &e->m_ShadowmapAccumBilinearEnable, &o->m_ShadowmapAccumBilinearEnable);
            ui::BoolEdit("Colored Slices", &e->m_ColoredShadowmapSlicesEnable, &o->m_ColoredShadowmapSlicesEnable);
            ui::BoolEdit("Z Buffer Test", &e->m_ZBufferShadowTestEnable, &o->m_ZBufferShadowTestEnable);
            ui::BoolEdit("DX 16-Bit", &e->m_DxShadowmap16BitEnable, &o->m_DxShadowmap16BitEnable);

            ui::IntEdit("Only Slice (debug)", &e->m_OnlyShadowmapSlice, &o->m_OnlyShadowmapSlice, -1, 8);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Motion Blur"))
        {
            ui::BoolEdit("Enable", &e->m_MotionBlurEnable, &o->m_MotionBlurEnable);
            ui::BoolEdit("Geometry Pass", &e->m_MotionBlurGeometryPassEnable, &o->m_MotionBlurGeometryPassEnable);
            ui::BoolEdit("Stencil Pass", &e->m_MotionBlurStencilPassEnable, &o->m_MotionBlurStencilPassEnable);
            ui::FloatEdit("Scale", &e->m_MotionBlurScale, &o->m_MotionBlurScale, 0.0f, 2.0f);
            ui::FloatEdit("Max", &e->m_MotionBlurMax, &o->m_MotionBlurMax);
            ui::FloatEdit("Noise Scale", &e->m_MotionBlurNoiseScale, &o->m_MotionBlurNoiseScale);
            ui::UIntEdit("Quality", &e->m_MotionBlurQuality, &o->m_MotionBlurQuality, 0, 4);
            ui::UIntEdit("Max Sample Count", &e->m_MotionBlurMaxSampleCount, &o->m_MotionBlurMaxSampleCount, 1, 64);
            ui::UIntEdit("Frame Average Count", &e->m_MotionBlurFrameAverageCount, &o->m_MotionBlurFrameAverageCount, 1, 16);
            ui::FloatEdit("Max Frame Time", &e->m_MotionBlurMaxFrameTime, &o->m_MotionBlurMaxFrameTime);
            ui::FloatEdit("Force Depth Cutoff", &e->m_ForceMotionBlurDepthCutoff, &o->m_ForceMotionBlurDepthCutoff);
            ui::FloatEdit("Cutoff Gradient Scale", &e->m_ForceMotionBlurCutoffGradientScale, &o->m_ForceMotionBlurCutoffGradientScale);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lighting - General"))
        {
            ui::BoolEdit("Outdoor Light Enable", &e->m_OutdoorLightEnable, &o->m_OutdoorLightEnable);
            ui::BoolEdit("Outdoor Key Light", &e->m_OutdoorKeyLightEnable, &o->m_OutdoorKeyLightEnable);
            ui::BoolEdit("Outdoor Sky Light", &e->m_OutdoorSkyLightEnable, &o->m_OutdoorSkyLightEnable);
            ui::BoolEdit("Outdoor Light Specular", &e->m_OutdoorLightSpecularEnable, &o->m_OutdoorLightSpecularEnable);
            ui::BoolEdit("Outdoor Light Tiling", &e->m_OutdoorLightTilingEnable, &o->m_OutdoorLightTilingEnable);
            ui::BoolEdit("Outdoor Light Tile Render", &e->m_OutdoorLightTileRenderEnable, &o->m_OutdoorLightTileRenderEnable);
            ui::BoolEdit("Outdoor Light Tile Blend", &e->m_OutdoorLightTileBlendEnable, &o->m_OutdoorLightTileBlendEnable);
            ui::BoolEdit("Outdoor Light Tile Simple Shader", &e->m_OutdoorLightTileSimpleShaderEnable, &o->m_OutdoorLightTileSimpleShaderEnable);
            ui::UIntEdit("Outdoor Light Tile Batch Count", &e->m_OutdoorLightTileBatchCount, &o->m_OutdoorLightTileBatchCount);

            ImGui::Separator();
            ui::FloatEdit("Intensity Scale", &e->m_LightIntensityScale, &o->m_LightIntensityScale);
            ui::FloatEdit("Force Intensity", &e->m_LightForceIntensity, &o->m_LightForceIntensity);
            ui::FloatEdit("Radius Scale", &e->m_LightRadiusScale, &o->m_LightRadiusScale);
            ui::FloatEdit("Attenuation Threshold", &e->m_LightAttenuationThreshold, &o->m_LightAttenuationThreshold);
            ui::BoolEdit("Attenuation Threshold Enable", &e->m_LightAttenuationThresholdEnable, &o->m_LightAttenuationThresholdEnable);
            ui::BoolEdit("Width Enable", &e->m_LightWidthEnable, &o->m_LightWidthEnable);
            ui::BoolEdit("Intensity Normalization", &e->m_LightIntensityNormalizationEnable, &o->m_LightIntensityNormalizationEnable);

            ImGui::Separator();
            ui::BoolEdit("Specular Lighting", &e->m_SpecularLightingEnable, &o->m_SpecularLightingEnable);
            ui::BoolEdit("Skin Lighting", &e->m_SkinLightingEnable, &o->m_SkinLightingEnable);
            ui::BoolEdit("Translucency Lighting", &e->m_TranslucencyLightingEnable, &o->m_TranslucencyLightingEnable);
            ui::BoolEdit("Emissive Enable", &e->m_EmissiveEnable, &o->m_EmissiveEnable);
            ui::BoolEdit("Unlit Enable", &e->m_UnlitEnable, &o->m_UnlitEnable);
            ui::BoolEdit("Draw Light Sources", &e->m_DrawLightSources, &o->m_DrawLightSources);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lighting - LOD"))
        {
            ui::FloatEdit("LOD Specular Fade Start", &e->m_LightLodSpecularFadeAreaStart, &o->m_LightLodSpecularFadeAreaStart);
            ui::FloatEdit("LOD Specular Fade End", &e->m_LightLodSpecularFadeAreaEnd, &o->m_LightLodSpecularFadeAreaEnd);
            ui::FloatEdit("LOD Radius Factor", &e->m_LightLodRadiusFactor, &o->m_LightLodRadiusFactor);
            ui::FloatEdit("LOD Fade Area", &e->m_LightLodFadeArea, &o->m_LightLodFadeArea);
            ui::FloatEdit("LOD Min Area", &e->m_LightLodMinArea, &o->m_LightLodMinArea);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lighting - Culling"))
        {
            ui::BoolEdit("Cull Enable", &e->m_LightCullEnable, &o->m_LightCullEnable);
            ui::BoolEdit("Depth Cull", &e->m_LightDepthCullEnable, &o->m_LightDepthCullEnable);
            ui::BoolEdit("Normal Cull", &e->m_LightNormalCullEnable, &o->m_LightNormalCullEnable);
            ui::BoolEdit("Cone Cull", &e->m_LightConeCullEnable, &o->m_LightConeCullEnable);
            ui::FloatEdit("Stencil Min Area", &e->m_LightStencilMinArea, &o->m_LightStencilMinArea);
            ui::BoolEdit("Stencil Method", &e->m_LightStencilMethodEnable, &o->m_LightStencilMethodEnable);
            ui::BoolEdit("Volume Method", &e->m_LightVolumeMethodEnable, &o->m_LightVolumeMethodEnable);
            ui::BoolEdit("Volume Depth Test", &e->m_LightVolumeDepthTestEnable, &o->m_LightVolumeDepthTestEnable);
            ui::FloatEdit("Tile Min Area", &e->m_LightTileMinArea, &o->m_LightTileMinArea);
            ui::BoolEdit("Tile Overlay", &e->m_LightTileOverlayEnable, &o->m_LightTileOverlayEnable);
            ui::UIntEdit("Overdraw Max Layer Count", &e->m_LightOverdrawMaxLayerCount, &o->m_LightOverdrawMaxLayerCount);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Light Types"))
        {
            ui::BoolEdit("Point Lights", &e->m_PointLightsEnable, &o->m_PointLightsEnable);
            ui::UIntEdit("Max Point Light Count", &e->m_MaxPointLightCount, &o->m_MaxPointLightCount, 0, 256);

            ImGui::Separator();
            ui::BoolEdit("Spot Lights", &e->m_SpotLightsEnable, &o->m_SpotLightsEnable);
            ui::UIntEdit("Max Spot Light Count", &e->m_MaxSpotLightCount, &o->m_MaxSpotLightCount, 0, 64);
            ui::UIntEdit("Max Spot Shadow Count", &e->m_MaxSpotLightShadowCount, &o->m_MaxSpotLightShadowCount, 0, 32);
            ui::UIntEdit("Spot Shadowmap Resolution", &e->m_SpotLightShadowmapResolution, &o->m_SpotLightShadowmapResolution, 128, 2048);
            ui::UIntEdit("Spot Shadowmap Quality", &e->m_SpotLightShadowmapQuality, &o->m_SpotLightShadowmapQuality, 0, 4);
            ui::FloatEdit("Spot Near Plane", &e->m_SpotLightNearPlane, &o->m_SpotLightNearPlane);
            ui::FloatEdit("Spot Poisson Filter Scale", &e->m_SpotLightShadowmapPoissonFilterScale, &o->m_SpotLightShadowmapPoissonFilterScale);
            ui::BoolEdit("Spot Shadowmap Enable", &e->m_SpotLightShadowmapEnable, &o->m_SpotLightShadowmapEnable);
            ui::BoolEdit("DX Spot Shadowmap 16-Bit", &e->m_DxSpotLightShadowmap16BitEnable, &o->m_DxSpotLightShadowmap16BitEnable);

            ImGui::Separator();
            ui::BoolEdit("Line Lights", &e->m_LineLightsEnable, &o->m_LineLightsEnable);
            ui::BoolEdit("Cone Lights", &e->m_ConeLightsEnable, &o->m_ConeLightsEnable);
            ui::BoolEdit("Light Probes", &e->m_LightProbesEnable, &o->m_LightProbesEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Sky"))
        {
            ui::BoolEdit("Sky Enable", &e->m_SkyEnable, &o->m_SkyEnable);
            ui::BoolEdit("Sky Fog Enable", &e->m_SkyFogEnable, &o->m_SkyFogEnable);

            ImGui::Separator();
            ui::BoolEdit("Sky Envmap Enable", &e->m_SkyEnvmapEnable, &o->m_SkyEnvmapEnable);
            ui::BoolEdit("Sky Envmap Update", &e->m_SkyEnvmapUpdateEnable, &o->m_SkyEnvmapUpdateEnable);
            ui::BoolEdit("Sky Envmap Force Update", &e->m_SkyEnvmapForceUpdateEnable, &o->m_SkyEnvmapForceUpdateEnable);
            ui::BoolEdit("Sky Envmap Mipmap Gen", &e->m_SkyEnvmapMipmapGenEnable, &o->m_SkyEnvmapMipmapGenEnable);
            ui::BoolEdit("Sky Envmap 8-Bit Texture", &e->m_SkyEnvmapUse8BitTexture, &o->m_SkyEnvmapUse8BitTexture);
            ui::UIntEdit("Sky Envmap Resolution", &e->m_SkyEnvmapResolution, &o->m_SkyEnvmapResolution, 16, 512);
            ui::UIntEdit("Sky Envmap Sides Per Frame", &e->m_SkyEnvmapSidesPerFrameCount, &o->m_SkyEnvmapSidesPerFrameCount, 1, 6);
            ui::FloatEdit("Sky Envmap Filter Width", &e->m_SkyEnvmapFilterWidth, &o->m_SkyEnvmapFilterWidth);
            ui::EnumCombo<fb::MipmapFilterMode>("Sky Envmap Filter Mode", reinterpret_cast<int*>(&e->m_SkyEnvmapFilterMode), reinterpret_cast<const int*>(&o->m_SkyEnvmapFilterMode));
            ui::BoolEdit("Sky Visibility Envmap Scaling", &e->m_SkyVisibilityEnvmapScalingEnable, &o->m_SkyVisibilityEnvmapScalingEnable);
            ui::IntEdit("Debug Sky Envmap Mip Level", &e->m_DrawDebugSkyEnvmapMipLevel, &o->m_DrawDebugSkyEnvmapMipLevel, -1, 10);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Dynamic Environment Map"))
        {
            ui::BoolEdit("Enable", &e->m_DynamicEnvmapEnable, &o->m_DynamicEnvmapEnable);
            ui::BoolEdit("Lighting Enable", &e->m_DynamicEnvmapLightingEnable, &o->m_DynamicEnvmapLightingEnable);
            ui::BoolEdit("Mipmap Gen", &e->m_DynamicEnvmapMipmapGenEnable, &o->m_DynamicEnvmapMipmapGenEnable);
            ui::UIntEdit("Resolution", &e->m_DynamicEnvmapResolution, &o->m_DynamicEnvmapResolution, 16, 512);
            ui::FloatEdit("Filter Width", &e->m_DynamicEnvmapFilterWidth, &o->m_DynamicEnvmapFilterWidth);
            ui::EnumCombo<fb::MipmapFilterMode>("Dynamic Mode", reinterpret_cast<int*>(&e->m_DynamicEnvmapFilterMode), reinterpret_cast<const int*>(&o->m_DynamicEnvmapFilterMode));
            ui::Vec3Edit("Default Position", &e->m_DynamicEnvmapDefaultPosition, &o->m_DynamicEnvmapDefaultPosition);
            ui::IntEdit("Debug Mip Level", &e->m_DrawDebugDynamicEnvmapMipLevel, &o->m_DrawDebugDynamicEnvmapMipLevel, -1, 10);

            ImGui::Separator();
            ui::BoolEdit("Static Envmap Enable", &e->m_StaticEnvmapEnable, &o->m_StaticEnvmapEnable);
            ui::BoolEdit("Custom Envmap Enable", &e->m_CustomEnvmapEnable, &o->m_CustomEnvmapEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Reflections"))
        {
            ui::BoolEdit("Planar Reflection Enable", &e->m_PlanarReflectionEnable, &o->m_PlanarReflectionEnable);
            ui::BoolEdit("Planar Reflection Blur", &e->m_PlanarReflectionBlur, &o->m_PlanarReflectionBlur);
            ui::UIntEdit("Planar Reflection Width", &e->m_PlanarReflectionWidth, &o->m_PlanarReflectionWidth, 128, 2048);
            ui::UIntEdit("Planar Reflection Height", &e->m_PlanarReflectionHeight, &o->m_PlanarReflectionHeight, 128, 2048);
            ui::FloatEdit("Planar Reflection Cull FOV", &e->m_PlanarReflectionCullFOV, &o->m_PlanarReflectionCullFOV);
            ui::FloatEdit("Temp Planar Reflection Y", &e->m_TempPlanarReflectionY, &o->m_TempPlanarReflectionY);
            ui::BoolEdit("Debug Cull Mode", &e->m_PlanarReflectionDebugCullMode, &o->m_PlanarReflectionDebugCullMode);
            ui::BoolEdit("Freeze Cull Frustum", &e->m_FreezePlanarReflectionCullFrustum, &o->m_FreezePlanarReflectionCullFrustum);
            ui::UIntEdit("Reflection Envmap Size", &e->m_ReflectionEnvmapSize, &o->m_ReflectionEnvmapSize, 16, 512);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Subsurface Scattering"))
        {
            ui::Vec3Edit("Color", &e->m_SubSurfaceColor, &o->m_SubSurfaceColor, true);
            ui::FloatEdit("Rolloff Key Light", &e->m_SubSurfaceRolloffKeyLight, &o->m_SubSurfaceRolloffKeyLight);
            ui::FloatEdit("Rolloff Local Light", &e->m_SubSurfaceRolloffLocalLight, &o->m_SubSurfaceRolloffLocalLight);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Decals & Volumes"))
        {
            ui::BoolEdit("Decal Volume Enable", &e->m_DecalVolumeEnable, &o->m_DecalVolumeEnable);
            ui::UIntEdit("Max Decal Volume Count", &e->m_MaxDecalVolumeCount, &o->m_MaxDecalVolumeCount, 0, 256);
            ui::FloatEdit("Decal Volume Scale", &e->m_DecalVolumeScale, &o->m_DecalVolumeScale);
            ui::UIntEdit("Max Destruction Volume Count", &e->m_MaxDestructionVolumeCount, &o->m_MaxDestructionVolumeCount, 0, 256);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Render Passes"))
        {
            ui::BoolEdit("Z Pass", &e->m_ZPassEnable, &o->m_ZPassEnable);
            ui::BoolEdit("Main Opaque Z Pass", &e->m_MainOpaqueZPassEnable, &o->m_MainOpaqueZPassEnable);
            ui::BoolEdit("Occluder Mesh Z Prepass", &e->m_OccluderMeshZPrepassEnable, &o->m_OccluderMeshZPrepassEnable);
            ui::BoolEdit("Occluder Mesh Z Prepass Debug", &e->m_OccluderMeshZPrepassDebugEnable, &o->m_OccluderMeshZPrepassDebugEnable);

            ImGui::Separator();
            ui::BoolEdit("Foreground Enable", &e->m_ForegroundEnable, &o->m_ForegroundEnable);
            ui::BoolEdit("Foreground Z Pass", &e->m_ForegroundZPassEnable, &o->m_ForegroundZPassEnable);
            ui::BoolEdit("Foreground Depth Clear", &e->m_ForegroundDepthClearEnable, &o->m_ForegroundDepthClearEnable);
            ui::BoolEdit("Foreground As Main", &e->m_ForegroundAsMainEnable, &o->m_ForegroundAsMainEnable);
            ui::BoolEdit("Foreground Transparent", &e->m_ForegroundTransparentEnable, &o->m_ForegroundTransparentEnable);

            ImGui::Separator();
            ui::BoolEdit("Draw Transparent", &e->m_DrawTransparent, &o->m_DrawTransparent);
            ui::BoolEdit("Draw Transparent Decal", &e->m_DrawTransparentDecal, &o->m_DrawTransparentDecal);
            ui::BoolEdit("Screen Effect Enable", &e->m_ScreenEffectEnable, &o->m_ScreenEffectEnable);
            ui::BoolEdit("Final Post Enable", &e->m_FinalPostEnable, &o->m_FinalPostEnable);
            ui::BoolEdit("View FX Enable", &e->m_ViewFxEnable, &o->m_ViewFxEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lens Flares"))
        {
            ui::BoolEdit("Enable", &e->m_LensFlaresEnable, &o->m_LensFlaresEnable);
            ui::BoolEdit("Half Res", &e->m_HalfResLensFlaresEnable, &o->m_HalfResLensFlaresEnable);
            ui::BoolEdit("Occlusion Enable", &e->m_LensFlareOcclusionEnable, &o->m_LensFlareOcclusionEnable);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("GBuffer"))
        {
            ui::BoolEdit("Clear Enable", &e->m_GBufferClearEnable, &o->m_GBufferClearEnable);
            ui::BoolEdit("Alpha Test Simple", &e->m_GBufferAlphaTestSimpleEnable, &o->m_GBufferAlphaTestSimpleEnable);
            ui::BoolEdit("DX Light 16-Bit", &e->m_DxGBufferLight16BitEnable, &o->m_DxGBufferLight16BitEnable);
            ui::BoolEdit("DX Normal 16-Bit", &e->m_DxGBufferNormal16BitEnable, &o->m_DxGBufferNormal16BitEnable);
            ui::BoolEdit("DX Linear Depth 32-Bit", &e->m_DxLinearDepth32BitFormatEnable, &o->m_DxLinearDepth32BitFormatEnable);
            ui::BoolEdit("DX Deferred CS Path", &e->m_DxDeferredCsPathEnable, &o->m_DxDeferredCsPathEnable);
            ui::UIntEdit("Test Count", &e->m_GBufferTestCount, &o->m_GBufferTestCount);
            ui::EnumCombo<fb::ShaderGBufferLayout>("Layout", reinterpret_cast<int*>(&e->m_GBufferLayout), reinterpret_cast<const int*>(&o->m_GBufferLayout));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Dynamic AO"))
        {
            ui::BoolEdit("Override Dynamic AO", &e->m_OverrideDynamicAO, &o->m_OverrideDynamicAO);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Misc Rendering"))
        {
            ui::BoolEdit("Wireframe", &e->m_WireframeEnable, &o->m_WireframeEnable);
            ui::BoolEdit("Opaque Sort By Solution", &e->m_OpaqueSortBySolutionEnable, &o->m_OpaqueSortBySolutionEnable);
            ui::BoolEdit("Generic Entity Renderer", &e->m_GenericEntityRendererEnable, &o->m_GenericEntityRendererEnable);
            ui::UIntEdit("Generic Entity Max Visible", &e->m_GenericEntityMaxVisibleEntityCount, &o->m_GenericEntityMaxVisibleEntityCount);
            ui::BoolEdit("Setup Job Enable", &e->m_SetupJobEnable, &o->m_SetupJobEnable);
            ui::BoolEdit("Dynamic Cull Z Buffer Test", &e->m_DynamicCullZBufferTestEnable, &o->m_DynamicCullZBufferTestEnable);
            ui::BoolEdit("Dynamic Cull Draw Occluded Boxes", &e->m_DynamicCullDrawOccludedBoxesEnable, &o->m_DynamicCullDrawOccludedBoxesEnable);
            ui::EnumCombo<fb::WorldViewMode>("View Mode", reinterpret_cast<int*>(&e->m_ViewMode), reinterpret_cast<const int*>(&o->m_ViewMode));
            ui::IntEdit("Only Tile Index", &e->m_OnlyTileIndex, &o->m_OnlyTileIndex, -1, 64);
            ui::IntEdit("Only Light Tile X", &e->m_OnlyLightTileX, &o->m_OnlyLightTileX, -1, 64);
            ui::IntEdit("Only Light Tile Y", &e->m_OnlyLightTileY, &o->m_OnlyLightTileY, -1, 64);
            ui::IntEdit("Only Light Tile Index", &e->m_OnlyLightTileIndex, &o->m_OnlyLightTileIndex, -1, 256);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Debug Visualization"))
        {
            if (ImGui::TreeNode("Buffers"))
            {
                ui::BoolEdit("GBuffer", &e->m_DrawDebugGBuffer, &o->m_DrawDebugGBuffer);
                ui::BoolEdit("Z Buffer", &e->m_DrawDebugZBufferEnable, &o->m_DrawDebugZBufferEnable);
                ui::BoolEdit("Velocity Buffer", &e->m_DrawDebugVelocityBuffer, &o->m_DrawDebugVelocityBuffer);
                ui::BoolEdit("Multisample Classify", &e->m_DrawDebugMultisampleClassify, &o->m_DrawDebugMultisampleClassify);
                ui::BoolEdit("Half Res Environment", &e->m_DrawDebugHalfResEnvironment, &o->m_DrawDebugHalfResEnvironment);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Shadows"))
            {
                ui::BoolEdit("Shadowmaps", &e->m_DrawDebugShadowmaps, &o->m_DrawDebugShadowmaps);
                ui::BoolEdit("Quarter Shadowmaps", &e->m_DrawDebugQuarterShadowmaps, &o->m_DrawDebugQuarterShadowmaps);
                ui::BoolEdit("Trans Shadowmap", &e->m_DrawDebugTransShadowmap, &o->m_DrawDebugTransShadowmap);
                ui::BoolEdit("Spot Light Shadowmaps", &e->m_DrawDebugSpotLightShadowmaps, &o->m_DrawDebugSpotLightShadowmaps);
                ui::BoolEdit("Shadow Frustums", &e->m_DrawShadowFrustums, &o->m_DrawShadowFrustums);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Lights"))
            {
                ui::BoolEdit("Light Sources", &e->m_DrawDebugLightSources, &o->m_DrawDebugLightSources);
                ui::BoolEdit("Light Shadow Sources", &e->m_DrawDebugLightShadowSources, &o->m_DrawDebugLightShadowSources);
                ui::BoolEdit("Light Stats", &e->m_DrawDebugLightStats, &o->m_DrawDebugLightStats);
                ui::BoolEdit("Light Tiles", &e->m_DrawDebugLightTiles, &o->m_DrawDebugLightTiles);
                ui::BoolEdit("Light Tile Volumes", &e->m_DrawDebugLightTileVolumes, &o->m_DrawDebugLightTileVolumes);
                ui::BoolEdit("Light Tile FB Grid", &e->m_DrawDebugLightTileFbGrid, &o->m_DrawDebugLightTileFbGrid);
                ui::BoolEdit("Light Tile Grid", &e->m_DrawDebugLightTileGrid, &o->m_DrawDebugLightTileGrid);
                ui::BoolEdit("Light Tile Sources", &e->m_DrawDebugLightTileSources, &o->m_DrawDebugLightTileSources);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Environment"))
            {
                ui::BoolEdit("Dynamic AO", &e->m_DrawDebugDynamicAO, &o->m_DrawDebugDynamicAO);
                ui::BoolEdit("Dynamic Envmap", &e->m_DrawDebugDynamicEnvmap, &o->m_DrawDebugDynamicEnvmap);
                ui::BoolEdit("Sky Envmap", &e->m_DrawDebugSkyEnvmap, &o->m_DrawDebugSkyEnvmap);
                ui::BoolEdit("Planar Reflection", &e->m_DrawDebugPlanarReflection, &o->m_DrawDebugPlanarReflection);
                ui::BoolEdit("Planar Reflection Cull Frustum", &e->m_DrawDebugPlanarReflectionCullFrustum, &o->m_DrawDebugPlanarReflectionCullFrustum);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Volumes & Misc"))
            {
                ui::BoolEdit("Decal Volumes", &e->m_DrawDebugDecalVolumes, &o->m_DrawDebugDecalVolumes);
                ui::BoolEdit("Destruction Volumes", &e->m_DrawDebugDestructionVolumes, &o->m_DrawDebugDestructionVolumes);
                ui::BoolEdit("Lens Flares", &e->m_DrawDebugLensFlares, &o->m_DrawDebugLensFlares);
                ui::BoolEdit("Lens Flare Occluders", &e->m_DrawDebugLensFlareOccluders, &o->m_DrawDebugLensFlareOccluders);
                ui::BoolEdit("World Occlusions", &e->m_DrawDebugWorldOcclusions, &o->m_DrawDebugWorldOcclusions);
                ui::BoolEdit("Visible Entity Types", &e->m_DrawDebugVisibleEntityTypes, &o->m_DrawDebugVisibleEntityTypes);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Bounding Geometry"))
            {
                ui::BoolEdit("Bounding Spheres", &e->m_DrawBoundingSpheres, &o->m_DrawBoundingSpheres);
                ui::BoolEdit("Solid Bounding Boxes", &e->m_DrawSolidBoundingBoxes, &o->m_DrawSolidBoundingBoxes);
                ui::BoolEdit("Line Bounding Boxes", &e->m_DrawLineBoundingBoxes, &o->m_DrawLineBoundingBoxes);
                ui::BoolEdit("Frustums", &e->m_DrawFrustums, &o->m_DrawFrustums);
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }
}