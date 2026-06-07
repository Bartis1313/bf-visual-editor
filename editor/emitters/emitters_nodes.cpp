#include "emitters.h"
#include "../ui/ui_helpers.h"

#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <string>

namespace editor::emitters
{
#if defined(BFVE_GAME_BF4)

    namespace
    {
        bool eF(const char* l, float* v, float sp = 0.01f) { return ImGui::DragFloat(l, v, sp); }
        bool eF2(const char* l, fb::Vec2* v) { return ImGui::DragFloat2(l, &v->m_x, 0.01f); }
        bool eF3(const char* l, fb::Vec3* v) { return ImGui::DragFloat3(l, &v->m_x, 0.01f); }
        bool eF4(const char* l, fb::Vec4* v) { return ImGui::DragFloat4(l, &v->m_x, 0.01f); }
        bool eB(const char* l, bool* v) { return ImGui::Checkbox(l, v); }
        bool eI(const char* l, int* v) { return ImGui::DragInt(l, v); }
        bool eU(const char* l, uint32_t* v) { return ImGui::DragScalar(l, ImGuiDataType_U32, v); }
        bool eCol(const char* l, fb::Vec3* v)
        {
            return ui::HdrColor3Edit(l, v, v);
        }

        template <typename E>
        bool eEnum(const char* l, E* v)
        {
            return ui::EnumCombo<E>(l, reinterpret_cast<int*>(v));
        }

        bool startsWith(const char* s, const char* p)
        {
            return std::strncmp(s, p, std::strlen(p)) == 0;
        }

        const char* rawClassName(fb::TypeInfo* ti)
        {
            if (!ti || ti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                return "?";
            auto ci = static_cast<fb::ClassInfo*>(ti);
            return (ci->m_InfoData && ci->m_InfoData->m_Name) ? ci->m_InfoData->m_Name : "?";
        }

        std::string friendlyName(const char* raw)
        {
            std::string s = raw ? raw : "?";
            if (s.size() > 4 && s.compare(s.size() - 4, 4, "Data") == 0)
                s.erase(s.size() - 4);
            return s;
        }

        struct Category { const char* tag; ImVec4 color; };

        Category categoryOf(const char* name)
        {
            if (startsWith(name, "Spawn"))  return { "Spawn",  ImVec4{ 0.55f, 0.85f, 0.45f, 1.0f } };
            if (startsWith(name, "Update")) return { "Update", ImVec4{ 0.45f, 0.70f, 0.95f, 1.0f } };
            if (startsWith(name, "Gravity") || startsWith(name, "LocalForce") ||
                startsWith(name, "WorldForces") || startsWith(name, "WorldWind") ||
                startsWith(name, "AirResistance") || startsWith(name, "Turbulance"))
                return { "Force", ImVec4{ 0.95f, 0.70f, 0.40f, 1.0f } };
            if (startsWith(name, "EmitterData") || startsWith(name, "BaseEmitter"))
                return { "Sub", ImVec4{ 0.85f, 0.55f, 0.90f, 1.0f } };
            if (startsWith(name, "PreRoll")) return { "Sim", ImVec4{ 0.80f, 0.80f, 0.55f, 1.0f } };
            return { "Node", ImVec4{ 0.75f, 0.75f, 0.75f, 1.0f } };
        }

        bool renderEvaluatorFields(fb::EvaluatorData* ev, uint32_t classId)
        {
            bool ch = false;
            if (classId == fb::ConstantEvaluatorData::ClassId())
            {
                auto* d = static_cast<fb::ConstantEvaluatorData*>(ev);
                ch |= eF("Scale", &d->m_Scale);
            }
            else if (classId == fb::PolynomialData::ClassId())
            {
                auto* d = static_cast<fb::PolynomialData*>(ev);
                ch |= eF("Scale", &d->m_ScaleValue);
                ch |= eF4("Coefficients", &d->m_Coefficients);
                ch |= eF("Min Clamp", &d->m_MinClamp);
                ch |= eF("Max Clamp", &d->m_MaxClamp);
            }
            else if (classId == fb::PolynomialColorInterpData::ClassId())
            {
                auto* d = static_cast<fb::PolynomialColorInterpData*>(ev);
                ch |= eCol("Color 0", &d->m_Color0);
                ch |= eCol("Color 1", &d->m_Color1);
                ch |= eF4("Coefficients", &d->m_Coefficients);
            }
            else if (classId == fb::SphereEvaluatorData::ClassId())
            {
                auto* d = static_cast<fb::SphereEvaluatorData*>(ev);
                ch |= eF("Radius", &d->m_Radius);
                ch |= eF3("Scale", &d->m_Scale);
                ch |= eF3("Pivot", &d->m_Pivot);
            }
            else if (classId == fb::SuperSphereEvaluatorData::ClassId())
            {
                auto* d = static_cast<fb::SuperSphereEvaluatorData*>(ev);
                ch |= eF("Inner Radius", &d->m_InnerRadius);
                ch |= eF3("Scale", &d->m_Scale);
                ch |= eF3("Pivot", &d->m_Pivot);
                ch |= eF("Outer Radius", &d->m_OuterRadius);
                ch |= eF("Start Zenith", &d->m_StartZenithAngle);
                ch |= eF("End Zenith", &d->m_EndZenithAngle);
                ch |= eF("Start Azimuth", &d->m_StartAzimuthAngle);
                ch |= eF("End Azimuth", &d->m_EndAzimuthAngle);
            }
            else if (classId == fb::BoxEvaluatorData::ClassId())
            {
                auto* d = static_cast<fb::BoxEvaluatorData*>(ev);
                ch |= eF3("Dimensions", &d->m_Dimensions);
                ch |= eF3("Pivot", &d->m_Pivot);
            }
            else if (classId == fb::RandomEvaluatorData::ClassId())
            {
                auto* d = static_cast<fb::RandomEvaluatorData*>(ev);
                ch |= eF("Min", &d->m_Min);
                ch |= eF("Max", &d->m_Max);
            }
            else if (classId == fb::RandomXYZEvaluatorData::ClassId())
            {
                auto* d = static_cast<fb::RandomXYZEvaluatorData*>(ev);
                ch |= eF("Min X", &d->m_MinX); ImGui::SameLine(); ch |= eF("Max X", &d->m_MaxX);
                ch |= eF("Min Y", &d->m_MinY); ImGui::SameLine(); ch |= eF("Max Y", &d->m_MaxY);
                ch |= eF("Min Z", &d->m_MinZ); ImGui::SameLine(); ch |= eF("Max Z", &d->m_MaxZ);
            }
            else if (classId == fb::RotateVectorData::ClassId())
            {
                auto* d = static_cast<fb::RotateVectorData*>(ev);
                ch |= eF("Angle", &d->m_Angle);
                ch |= eB("Input Affects Phi", &d->m_InputAffectsPhi);
                ch |= eB("Rotate Within Plane", &d->m_RotateWithinPlane);
            }
            else if (classId == fb::CameraProximityEvaluatorData::ClassId())
            {
                auto* d = static_cast<fb::CameraProximityEvaluatorData*>(ev);
                ch |= eF("Forward Offset", &d->m_ForwardOffset);
                ch |= eF3("Size", &d->m_Size);
                ch |= eF3("Offset", &d->m_Offset);
                ch |= eF3("Inner Radius Dir", &d->m_InnerRadiusDirection);
                ch |= eF("Inner Radius", &d->m_InnerRadius);
            }
            else if (classId == fb::DefaultEvaluatorData::ClassId())
            {
                auto* d = static_cast<fb::DefaultEvaluatorData*>(ev);
                ch |= eF4("Values", &d->m_Values);
            }
            else if (classId == fb::SampleTextureData::ClassId())
            {
                auto* d = static_cast<fb::SampleTextureData*>(ev);
                ch |= eF3("Color Intensity Min", &d->m_ColorIntensityMin);
                ch |= eF3("Color Intensity Max", &d->m_ColorIntensityMax);
                ch |= eF2("Texture Dimensions", &d->m_TextureDimensions);
                ch |= eF("Origin U", &d->m_TextureOriginU);
                ch |= eF("Origin V", &d->m_TextureOriginV);
                ImGui::TextDisabled("Gradient samples: %d", static_cast<int>(d->m_GradientData.size()));
            }
            else if (classId == fb::PolynomialOperatorData::ClassId())
            {
                auto* d = static_cast<fb::PolynomialOperatorData*>(ev);
                ch |= eEnum("Operation", &d->m_Operation);
                if (ImGui::TreeNode("First Operand"))
                {
                    ch |= eF4("Coefficients##op1", &d->m_FirstOperand.m_Coefficients);
                    ch |= eF("Scale##op1", &d->m_FirstOperand.m_ScaleValue);
                    ch |= eF("Min Clamp##op1", &d->m_FirstOperand.m_MinClamp);
                    ch |= eF("Max Clamp##op1", &d->m_FirstOperand.m_MaxClamp);
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Second Operand"))
                {
                    ch |= eF4("Coefficients##op2", &d->m_SecondOperand.m_Coefficients);
                    ch |= eF("Scale##op2", &d->m_SecondOperand.m_ScaleValue);
                    ch |= eF("Min Clamp##op2", &d->m_SecondOperand.m_MinClamp);
                    ch |= eF("Max Clamp##op2", &d->m_SecondOperand.m_MaxClamp);
                    ImGui::TreePop();
                }
                ch |= eF("Result Min Clamp", &d->m_MinClampResult);
                ch |= eF("Result Max Clamp", &d->m_MaxClampResult);
            }
            else if (classId == fb::SplineData::ClassId())
            {
                auto* d = static_cast<fb::SplineData*>(ev);
                ch |= eEnum("Spline Type", &d->m_SplineCurve.m_SplineType);
                ImGui::TextDisabled("Full spline curve editing not yet supported");
            }
            else
            {
                ImGui::TextDisabled("(no editor for this evaluator)");
            }
            return ch;
        }

        bool renderEvaluator(fb::EvaluatorData* ev)
        {
            fb::TypeInfo* ti = ev->GetType();
            const char* raw = rawClassName(ti);
            const uint32_t classId = (ti && ti->GetTypeCode() == fb::BasicTypesEnum::kTypeCode_Class)
                ? static_cast<fb::ClassInfo*>(ti)->m_ClassId : 0u;

            ImGui::PushID(ev);
            char label[160];
            std::snprintf(label, sizeof(label), "Pre: %s###eval", friendlyName(raw).c_str());

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.85f, 0.80f, 0.55f, 1.0f });
            const bool open = ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen);
            ImGui::PopStyleColor();

            bool ch = false;
            if (open)
            {
                ch = renderEvaluatorFields(ev, classId);
                ImGui::TreePop();
            }
            ImGui::PopID();
            return ch;
        }

        bool renderProcessorFields(fb::ProcessorData* proc, fb::ClassInfo* ci, uint32_t classId)
        {
            // thse are not really helpful, nor present
            if (ci->isSubclassOf((fb::ClassInfo*)fb::UpdateShaderParam01Data::ClassInfoPtr()))
            {
                auto* d = static_cast<fb::UpdateShaderParam01Data*>(proc);
                bool ch = false;
                ch |= eF("Spawn Value Min", &d->m_SpawnValueMin);
                ch |= eF("Spawn Value Max", &d->m_SpawnValueMax);
                ch |= eB("Enable Random Spawn", &d->m_EnableRandomSpawnValue);
                ch |= eF4("Default Values", &d->m_DefaultValues);
                return ch;
            }

            bool ch = false;
            if (classId == fb::UpdateColorData::ClassId())
            {
                ch |= eCol("Color", &static_cast<fb::UpdateColorData*>(proc)->m_Color);
            }
            else if (classId == fb::UpdateSizeData::ClassId())
            {
                ch |= eF2("Pivot", &static_cast<fb::UpdateSizeData*>(proc)->m_Pivot);
            }
            else if (classId == fb::UpdateAlphaLevelScaleData::ClassId())
            {
                ch |= eF("Exponent", &static_cast<fb::UpdateAlphaLevelScaleData*>(proc)->m_Exponent);
            }
            else if (classId == fb::UpdateAlphaLevelMaxData::ClassId())
            {
                ch |= eF("Max Level", &static_cast<fb::UpdateAlphaLevelMaxData*>(proc)->m_MaxLevel);
            }
            else if (classId == fb::UpdateAlphaLevelMinData::ClassId())
            {
                ch |= eF("Min Level", &static_cast<fb::UpdateAlphaLevelMinData*>(proc)->m_MinLevel);
            }
            else if (classId == fb::UpdateTransparencyData::ClassId())
            {
                ch |= eF("Cull Threshold", &static_cast<fb::UpdateTransparencyData*>(proc)->m_CullThreshold);
            }
            else if (classId == fb::UpdateCollisionData::ClassId())
            {
                auto* d = static_cast<fb::UpdateCollisionData*>(proc);
                ch |= eF("Restitution", &d->m_Restitution);
                ch |= eF("Reflection Bias", &d->m_ReflectionBias);
                ch |= eF("Rest Speed Threshold", &d->m_RestSpeedThreshold);
                ch |= eF("Randomness", &d->m_Randomness);
                ch |= eEnum("Method", &d->m_Method);
                ch |= eEnum("Priority", &d->m_Priority);
            }
            else if (classId == fb::UpdateCameraProximityData::ClassId())
            {
                auto* d = static_cast<fb::UpdateCameraProximityData*>(proc);
                ch |= eF("Forward Offset", &d->m_ForwardOffset);
                ch |= eF3("Size", &d->m_Size);
            }
            else if (classId == fb::UpdateRibbonTextureData::ClassId())
            {
                auto* d = static_cast<fb::UpdateRibbonTextureData*>(proc);
                ch |= eI("Texture Particle Count", reinterpret_cast<int*>(&d->m_TextureParticleCount));
                ch |= eB("Mirror Texture", &d->m_MirrorTexture);
            }
            else if (classId == fb::UpdateRibbonFadeData::ClassId())
            {
                auto* d = static_cast<fb::UpdateRibbonFadeData*>(proc);
                ch |= eI("Fade In Particles", reinterpret_cast<int*>(&d->m_FadeInParticleCount));
                ch |= eI("Fade Out Particles", reinterpret_cast<int*>(&d->m_FadeOutParticleCount));
            }
            else if (classId == fb::UpdateAgeData::ClassId())
            {
                auto* d = static_cast<fb::UpdateAgeData*>(proc);
                ch |= eF("Lifetime", &d->m_Lifetime);
                ch |= eF("Random Lifetime Scale", &d->m_RandomLifetimeScale);
                ch |= eF("Max Factor", &d->m_MaxFactor);
            }
            else if (classId == fb::TurbulanceData::ClassId())
            {
                auto* d = static_cast<fb::TurbulanceData*>(proc);
                ch |= eF("Intensity", &d->m_Intensity);
                ch |= eEnum("Noise Type", &d->m_NoiseType);
                ch |= eF("Period Space", &d->m_PeriodSpace);
                ch |= eF("Force As Instant Velocity", &d->m_TurbulenceForceAsInstantVelocity);
                ch |= eI("Octaves", &d->m_Octaves);
                ch |= eF("Octave Persistence", &d->m_OctavePersistence);
                ch |= eF("Per Particle Randomness", &d->m_PerParticleRandomness);
            }
            else if (classId == fb::AirResistanceData::ClassId())
            {
                ch |= eF("Drag Factor", &static_cast<fb::AirResistanceData*>(proc)->m_DragFactor);
            }
            else if (classId == fb::WorldForcesData::ClassId())
            {
                auto* d = static_cast<fb::WorldForcesData*>(proc);
                ch |= eF("Forces Multiplier", &d->m_ForcesMultiplier);
                ch |= eF("Per Particle Randomness", &d->m_PerParticleRandomness);
            }
            else if (classId == fb::WorldWindData::ClassId())
            {
                auto* d = static_cast<fb::WorldWindData*>(proc);
                ch |= eF("Wind Multiplier", &d->m_WindMultiplier);
                ch |= eF("Per Particle Randomness", &d->m_PerParticleRandomness);
            }
            else if (classId == fb::LocalForceData::ClassId())
            {
                auto* d = static_cast<fb::LocalForceData*>(proc);
                ch |= eF("Per Particle Randomness", &d->m_PerParticleRandomness);
                ch |= eB("Emitter Local Space", &d->m_EmitterLocalSpaceForce);
                ch |= eF3("Local Force", &d->m_LocalForce);
            }
            else if (classId == fb::GravityData::ClassId())
            {
                auto* d = static_cast<fb::GravityData*>(proc);
                ch |= eF("Gravity", &d->m_Gravity);
                ch |= eF("Per Particle Randomness", &d->m_PerParticleRandomness);
            }
            else if (classId == fb::SpawnColorRandomData::ClassId())
            {
                auto* d = static_cast<fb::SpawnColorRandomData*>(proc);
                ch |= eCol("Color 0", &d->m_Color0);
                ch |= eCol("Color 1", &d->m_Color1);
            }
            else if (classId == fb::SpawnRotationSpeedData::ClassId())
            {
                ch |= eF("Rotation Speed", &static_cast<fb::SpawnRotationSpeedData*>(proc)->m_RotationSpeed);
            }
            else if (classId == fb::SpawnRotationData::ClassId())
            {
                ch |= eF("Rotation", &static_cast<fb::SpawnRotationData*>(proc)->m_Rotation);
            }
            else if (classId == fb::SpawnAnimationFrameData::ClassId())
            {
                ch |= eU("Animation Frame", &static_cast<fb::SpawnAnimationFrameData*>(proc)->m_AnimationFrame);
            }
            else if (classId == fb::SpawnAnimationData::ClassId())
            {
                auto* d = static_cast<fb::SpawnAnimationData*>(proc);
                ch |= eF("Animation Speed", &d->m_AnimationSpeed);
                ch |= eB("Based On Lifetime", &d->m_BasedOnLifetime);
            }
            else if (classId == fb::SpawnSizeData::ClassId())
            {
                ch |= eF("Size", &static_cast<fb::SpawnSizeData*>(proc)->m_Size);
            }
            else if (classId == fb::SpawnSpeedData::ClassId())
            {
                ch |= eF("Speed", &static_cast<fb::SpawnSpeedData*>(proc)->m_Speed);
            }
            else if (classId == fb::SpawnDirectionData::ClassId())
            {
                auto* d = static_cast<fb::SpawnDirectionData*>(proc);
                ch |= eF("Direction From Origin", &d->m_DirectionFromEmitterOrigin);
                ch |= eF("Inherit Speed Amount", &d->m_InheritSpeedAmount);
                ch |= eF("Inherit Speed Smoothing", &d->m_InheritSpeedSmoothingFactor);
                ch |= eF("Inherit Speed Randomness", &d->m_InheritSpeedRandomness);
                ch |= eB("Inherit Speed & Dir", &d->m_InheritSpeedAndDirectionFromEmitter);
            }
            else if (classId == fb::PreRollData::ClassId())
            {
                auto* d = static_cast<fb::PreRollData*>(proc);
                ch |= eF("Pre Roll", &d->m_PreRoll);
                ch |= eF("Updates Per Second", &d->m_UpdatesPerSecond);
            }
            else if (classId == fb::SpawnRibbonRateData::ClassId())
            {
                auto* d = static_cast<fb::SpawnRibbonRateData*>(proc);
                ch |= eF("Spawn Rate", &d->m_SpawnRate);
                ch |= eF("Angle Deviation", &d->m_AngleDeviation);
                ch |= eB("Distribute Over Distance", &d->m_DistributeOverDistance);
            }
            else if (classId == fb::SpawnRateData::ClassId())
            {
                auto* d = static_cast<fb::SpawnRateData*>(proc);
                ch |= eF("Spawn Rate", &d->m_SpawnRate);
                ch |= eB("Distribute Over Time", &d->m_DistributeOverTime);
                ch |= eB("Distribute Over Distance", &d->m_DistributeOverDistance);
            }
            else if (classId == fb::EmitterData::ClassId())
            {
                ImGui::TextDisabled("Sub-emitters: %d", static_cast<int>(static_cast<fb::EmitterData*>(proc)->m_EmitterAssets.size()));
            }
            else if (classId == fb::BaseEmitterData::ClassId())
            {
                ImGui::TextDisabled("Base sub-emitter document");
            }
            else
            {
                ImGui::TextDisabled("(no own fields)");
            }
            return ch;
        }

        void renderProcessorSection(EmitterEditData& edit, fb::ProcessorData* proc)
        {
            fb::TypeInfo* ti = proc->GetType();
            fb::ClassInfo* ci = (ti && ti->GetTypeCode() == fb::BasicTypesEnum::kTypeCode_Class)
                ? static_cast<fb::ClassInfo*>(ti) : nullptr;
            const char* raw = rawClassName(ti);
            const uint32_t classId = ci ? ci->m_ClassId : 0u;

            const Category cat = categoryOf(raw);

            ImGui::PushID(proc);

            char label[256];
            std::snprintf(label, sizeof(label), "[%s] %s###proc", cat.tag, friendlyName(raw).c_str());

            ImGui::PushStyleColor(ImGuiCol_Text, cat.color);
            const bool open = ImGui::CollapsingHeader(label);
            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Class: %s  (ClassId %u)", raw, classId);
                ImGui::EndTooltip();
            }

            if (open)
            {
                ImGui::Indent();
                bool ch = false;

                ch |= eEnum("Input", &proc->m_EvaluatorInput);

                if (ci)
                    ch |= renderProcessorFields(proc, ci, classId);

                if (proc->m_Pre)
                    ch |= renderEvaluator(proc->m_Pre);

                if (ch)
                    edit.modified = true;

                ImGui::Unindent();
            }

            ImGui::PopID();
        }

        bool isShaderParam(fb::ProcessorData* proc)
        {
            fb::TypeInfo* ti = proc ? proc->GetType() : nullptr;
            if (!ti || ti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                return false;

            return static_cast<fb::ClassInfo*>(ti)->isSubclassOf((fb::ClassInfo*)fb::UpdateShaderParam01Data::ClassInfoPtr());
        }
    }

    void renderProcessorGraph(EmitterEditData& edit)
    {
        fb::EmitterTemplateData* d = edit.data;
        if (!d)
        {
            ImGui::TextDisabled("No template data");
            return;
        }

        for (fb::ProcessorData* proc = d->m_RootProcessor; proc; proc = proc->m_NextProcessor)
            if (!isShaderParam(proc))
                renderProcessorSection(edit, proc);
    }

#else // !BFVE_GAME_BF4

    void renderProcessorGraph(EmitterEditData&)
    {
        ImGui::TextDisabled("Processor graph is BF4-only for now.");
    }

#endif
}
