#pragma once

#include <string>
#include <unordered_set>
#include <cmath>
#include <type_traits>
#include "../SDK/sdk.h"

template<typename T>
concept enumable = std::is_enum_v<T>;

struct StateHash
{
    int priority = 0;
    float visibility = 0.0f;
    uint32_t componentMask = 0;

    bool operator==(const StateHash& other) const
    {
        return priority == other.priority &&
            componentMask == other.componentMask &&
            std::fabs(visibility - other.visibility) < 0.001f;
    }
};

struct StateEntityInfo
{
    fb::VisualEnvironmentEntity* entity = nullptr;
    fb::VisualEnvironmentEntityData* data = nullptr;
    std::string name;
    int priority = 0;
    bool excluded = false;
};

enum ComponentBit : uint32_t
{
    Comp_OutdoorLight = 1 << 0,
    Comp_Enlighten = 1 << 1,
    Comp_Tonemap = 1 << 2,
    Comp_ColorCorrection = 1 << 3,
    Comp_Sky = 1 << 4,
    Comp_Fog = 1 << 5,
    Comp_Wind = 1 << 6,
    Comp_Sunflare = 1 << 7,
    Comp_DynamicAO = 1 << 8,
    Comp_Dof = 1 << 9,
    Comp_Vignette = 1 << 10,
    Comp_FilmGrain = 1 << 11,
    Comp_LensScope = 1 << 12,
    Comp_CameraParams = 1 << 13,
    Comp_ScreenEffect = 1 << 14,
    Comp_DamageEffect = 1 << 15,
    Comp_PlanarReflection = 1 << 16,
    Comp_DynamicEnvmap = 1 << 17,
    Comp_CharacterLighting = 1 << 18,
    Comp_MotionBlur = 1 << 19,
};

struct LightDataEntry
{
    fb::LocalLightEntityData* dataPtr = nullptr;
    std::string assetName = "(unknown)";
    std::string lightType = "LocalLight";
    std::string containerType;

    bool isSpotLight = false;
    bool isPointLight = false;

    // basic light
    bool origCaptured = false;
    fb::Vec3 origColor{};
    fb::Vec3 origParticleColorScale{};
    fb::Vec3 origEnlightenColorScale{};
    float origRadius = 0.f;
    float origIntensity = 0.f;
    float origAttenuationOffset = 0.f;
    int origEnlightenColorMode = 0;
    bool origVisible = true;
    bool origSpecularEnable = true;
    bool origEnlightenEnable = true;

    // spotlight
    int origSpotShape = 0;
    float origConeInnerAngle = 0.f;
    float origConeOuterAngle = 0.f;
    float origFrustumFov = 0.f;
    float origFrustumAspect = 0.f;
    float origOrthoWidth = 0.f;
    float origOrthoHeight = 0.f;
    bool origCastShadowsEnable = false;
    int origCastShadowsMinLevel = 0;

    // pointlight
    float origPointWidth = 0.f;
    float origTranslucencyAmbient = 0.f;
    float origTranslucencyScale = 0.f;
    unsigned int origTranslucencyPower = 0;
    float origTranslucencyDistortion = 0.f;

    bool hasOverride = false;

    // Edited values
    fb::Vec3 color{ };
    fb::Vec3 particleColorScale{ };
    fb::Vec3 enlightenColorScale{ };
    float radius = 0.f;
    float intensity = 0.f;
    float attenuationOffset = 0.f;
    int enlightenColorMode = 0;
    bool visible = true;
    bool specularEnable = true;
    bool enlightenEnable = true;
    int spotShape = 0;
    float coneInnerAngle = 0.f;
    float coneOuterAngle = 0.f;
    float frustumFov = 0.f;
    float frustumAspect = 0.f;
    float orthoWidth = 0.f;
    float orthoHeight = 0.f;
    bool castShadowsEnable = false;
    int castShadowsMinLevel = 0;
    float pointWidth = 0.f;
    float translucencyAmbient = 0.f;
    float translucencyScale = 0.f;
    unsigned int translucencyPower = 0;
    float translucencyDistortion = 0.f;

    std::unordered_set<fb::LocalLightEntity*> activeEntities;

    size_t ActiveCount() const { return activeEntities.size(); }
    void ResetToOriginal()
    {
        if (!origCaptured) return;
        color = origColor;
        particleColorScale = origParticleColorScale;
        enlightenColorScale = origEnlightenColorScale;
        radius = origRadius;
        intensity = origIntensity;
        attenuationOffset = origAttenuationOffset;
        enlightenColorMode = origEnlightenColorMode;
        visible = origVisible;
        specularEnable = origSpecularEnable;
        enlightenEnable = origEnlightenEnable;
        spotShape = origSpotShape;
        coneInnerAngle = origConeInnerAngle;
        coneOuterAngle = origConeOuterAngle;
        frustumFov = origFrustumFov;
        frustumAspect = origFrustumAspect;
        orthoWidth = origOrthoWidth;
        orthoHeight = origOrthoHeight;
        castShadowsEnable = origCastShadowsEnable;
        castShadowsMinLevel = origCastShadowsMinLevel;
        pointWidth = origPointWidth;
        translucencyAmbient = origTranslucencyAmbient;
        translucencyScale = origTranslucencyScale;
        translucencyPower = origTranslucencyPower;
        translucencyDistortion = origTranslucencyDistortion;
    }
};

struct StateEditData
{
#define DECLARE_COMPONENT(name) \
        bool has##name = false; \
        void* captured##name = nullptr; \
        fb::name##ComponentData orig##name{}; \
        fb::name##ComponentData edit##name{}

    DECLARE_COMPONENT(OutdoorLight);
    DECLARE_COMPONENT(Enlighten);
    DECLARE_COMPONENT(Tonemap);
    DECLARE_COMPONENT(ColorCorrection);
    DECLARE_COMPONENT(Sky);
    DECLARE_COMPONENT(Fog);
    DECLARE_COMPONENT(Wind);
    DECLARE_COMPONENT(SunFlare);
    DECLARE_COMPONENT(DynamicAO);
    DECLARE_COMPONENT(Dof);
    DECLARE_COMPONENT(Vignette);
    DECLARE_COMPONENT(FilmGrain);
    DECLARE_COMPONENT(LensScope);
    DECLARE_COMPONENT(CameraParams);
    DECLARE_COMPONENT(ScreenEffect);
    DECLARE_COMPONENT(DamageEffect);
    DECLARE_COMPONENT(PlanarReflection);
    DECLARE_COMPONENT(DynamicEnvmap);
    DECLARE_COMPONENT(CharacterLighting);
    DECLARE_COMPONENT(MotionBlur);

#undef DECLARE_COMPONENT

    bool overrideEnabled = true;
    bool captured = false;

    fb::VisualEnvironmentState* statePtr = nullptr;
    int stateIndex = -1;
    std::string stateName;

    void Reset() { *this = StateEditData{}; }
    void ResetCaptures()
    {
        hasOutdoorLight = false;
        hasEnlighten = false;
        hasTonemap = false;
        hasColorCorrection = false;
        hasSky = false;
        hasFog = false;
        hasWind = false;
        hasSunFlare = false;
        hasDynamicAO = false;
        hasDof = false;
        hasVignette = false;
        hasFilmGrain = false;
        hasLensScope = false;
        hasCameraParams = false;
        hasScreenEffect = false;
        hasDamageEffect = false;
        hasPlanarReflection = false;
        hasDynamicEnvmap = false;
        hasCharacterLighting = false;
        hasMotionBlur = false;

        capturedOutdoorLight = nullptr;
        capturedEnlighten = nullptr;
        capturedTonemap = nullptr;
        capturedColorCorrection = nullptr;
        capturedSky = nullptr;
        capturedFog = nullptr;
        capturedWind = nullptr;
        capturedSunFlare = nullptr;
        capturedDynamicAO = nullptr;
        capturedDof = nullptr;
        capturedVignette = nullptr;
        capturedFilmGrain = nullptr;
        capturedLensScope = nullptr;
        capturedCameraParams = nullptr;
        capturedScreenEffect = nullptr;
        capturedDamageEffect = nullptr;
        capturedPlanarReflection = nullptr;
        capturedDynamicEnvmap = nullptr;
        capturedCharacterLighting = nullptr;
        capturedMotionBlur = nullptr;
    }
    int ComponentCount() const
    {
        int c = 0;
        if (hasOutdoorLight) c++;
        if (hasEnlighten) c++;
        if (hasTonemap) c++;
        if (hasColorCorrection) c++;
        if (hasSky) c++;
        if (hasFog) c++;
        if (hasWind) c++;
        if (hasSunFlare) c++;
        if (hasDynamicAO) c++;
        if (hasDof) c++;
        if (hasVignette) c++;
        if (hasFilmGrain) c++;
        if (hasLensScope) c++;
        if (hasCameraParams) c++;
        if (hasScreenEffect) c++;
        if (hasDamageEffect) c++;
        if (hasPlanarReflection) c++;
        if (hasDynamicEnvmap) c++;
        if (hasCharacterLighting) c++;
        if (hasMotionBlur) c++;
        return c;
    }
    static int CountLiveComponents(fb::VisualEnvironmentState* state)
    {
        if (!state) return 0;
        int c = 0;
        if (state->outdoorLight) c++;
        if (state->enlighten) c++;
        if (state->tonemap) c++;
        if (state->colorCorrection) c++;
        if (state->sky) c++;
        if (state->fog) c++;
        if (state->wind) c++;
        if (state->sunFlare) c++;
        if (state->dynamicAO) c++;
        if (state->dof) c++;
        if (state->vignette) c++;
        if (state->filmGrain) c++;
        if (state->lensScope) c++;
        if (state->cameraParams) c++;
        if (state->screenEffect) c++;
        if (state->damageEffect) c++;
        if (state->planarReflection) c++;
        if (state->dynamicEnvmap) c++;
        if (state->characterLighting) c++;
        if (state->motionBlur) c++;
        return c;
    }
};

struct StateEditEntry
{
    StateHash hash;
    StateEditData editData;
};

struct VisualEnvironmentGlobalData
{
    fb::OutdoorLightComponentData origOutdoorLight{};
    fb::OutdoorLightComponentData editOutdoorLight{};
    bool outdoorLightOverrideEnabled = false;

    fb::EnlightenComponentData origEnlighten{};
    fb::EnlightenComponentData editEnlighten{};
    bool enlightenOverrideEnabled = false;

    fb::TonemapComponentData origTonemap{};
    fb::TonemapComponentData editTonemap{};
    bool tonemapOverrideEnabled = false;

    fb::ColorCorrectionComponentData origColorCorrection{};
    fb::ColorCorrectionComponentData editColorCorrection{};
    bool colorCorrectionOverrideEnabled = false;

    fb::SkyComponentData origSky{};
    fb::SkyComponentData editSky{};
    bool skyOverrideEnabled = false;

    fb::FogComponentData origFog{};
    fb::FogComponentData editFog{};
    bool fogOverrideEnabled = false;

    fb::WindComponentData origWind{};
    fb::WindComponentData editWind{};
    bool windOverrideEnabled = false;

    fb::SunFlareComponentData origSunFlare{};
    fb::SunFlareComponentData editSunFlare{};
    bool sunFlareOverrideEnabled = false;

    fb::DynamicAOComponentData origDynamicAO{};
    fb::DynamicAOComponentData editDynamicAO{};
    bool dynamicAOOverrideEnabled = false;

    fb::DofComponentData origDof{};
    fb::DofComponentData editDof{};
    bool dofOverrideEnabled = false;

    fb::VignetteComponentData origVignette{};
    fb::VignetteComponentData editVignette{};
    bool vignetteOverrideEnabled = false;

    fb::FilmGrainComponentData origFilmGrain{};
    fb::FilmGrainComponentData editFilmGrain{};
    bool filmGrainOverrideEnabled = false;

    fb::LensScopeComponentData origLensScope{};
    fb::LensScopeComponentData editLensScope{};
    bool lensScopeOverrideEnabled = false;

    fb::CameraParamsComponentData origCameraParams{};
    fb::CameraParamsComponentData editCameraParams{};
    bool cameraParamsOverrideEnabled = false;

    fb::ScreenEffectComponentData origScreenEffect{};
    fb::ScreenEffectComponentData editScreenEffect{};
    bool screenEffectOverrideEnabled = false;

    fb::DamageEffectComponentData origDamageEffect{};
    fb::DamageEffectComponentData editDamageEffect{};
    bool damageEffectOverrideEnabled = false;

    fb::PlanarReflectionComponentData origPlanarReflection{};
    fb::PlanarReflectionComponentData editPlanarReflection{};
    bool planarReflectionOverrideEnabled = false;

    fb::DynamicEnvmapComponentData origDynamicEnvmap{};
    fb::DynamicEnvmapComponentData editDynamicEnvmap{};
    bool dynamicEnvmapOverrideEnabled = false;

    fb::CharacterLightingComponentData origCharacterLighting{};
    fb::CharacterLightingComponentData editCharacterLighting{};
    bool characterLightingOverrideEnabled = false;

    fb::MotionBlurComponentData origMotionBlur{};
    fb::MotionBlurComponentData editMotionBlur{};
    bool motionBlurOverrideEnabled = false;

    bool captured = false;
    bool globalOverrideEnabled = false;

    void Reset() { *this = VisualEnvironmentGlobalData{ }; }
    int EnabledOverrideCount() const
    {
        int c = 0;
        if (outdoorLightOverrideEnabled) c++;
        if (enlightenOverrideEnabled) c++;
        if (tonemapOverrideEnabled) c++;
        if (colorCorrectionOverrideEnabled) c++;
        if (skyOverrideEnabled) c++;
        if (fogOverrideEnabled) c++;
        if (windOverrideEnabled) c++;
        if (sunFlareOverrideEnabled) c++;
        if (dynamicAOOverrideEnabled) c++;
        if (dofOverrideEnabled) c++;
        if (vignetteOverrideEnabled) c++;
        if (filmGrainOverrideEnabled) c++;
        if (lensScopeOverrideEnabled) c++;
        if (cameraParamsOverrideEnabled) c++;
        if (screenEffectOverrideEnabled) c++;
        if (damageEffectOverrideEnabled) c++;
        if (planarReflectionOverrideEnabled) c++;
        if (dynamicEnvmapOverrideEnabled) c++;
        if (characterLightingOverrideEnabled) c++;
        if (motionBlurOverrideEnabled) c++;
        return c;
    }
};

struct EmitterEditData
{
    bool modified = false;
    fb::EmitterTemplateData* data = nullptr;
    fb::EmitterTemplate* lastTemplate = nullptr;
    std::string name;
    std::string category;

    struct Snapshot
    {
        fb::Vec4 pointLightIntensity;
        fb::Vec3 pointLightPivot;
        fb::Vec3 pointLightColor;
        unsigned int maxCount;
        unsigned int lifetimeFrameCount;
        float timeScale;
        float lifetime;
        float visibleAfterDistance;
        float distanceScaleNearValue;
        float pointLightRadius;
        float vertexPixelLightingBlendFactor;
        float globalLocalNormalBlendFactor;
        float softParticlesFadeDistanceMultiplier;
        float lightWrapAroundFactor;
        float lightMultiplier;
        float distanceScaleFarValue;
        float pointLightRandomIntensityMin;
        float meshCullingDistance;
        float pointLightRandomIntensityMax;
        float maxSpawnDistance;
        float minScreenArea;
        float distanceScaleLength;
        float pointLightMaxClamp;
        float particleCullingFactor;
        float pointLightMinClamp;
        fb::EmittableType emittableType;
        bool followSpawnSource;
        bool repeatParticleSpawning;
        bool emissive;
        bool exclusionVolumeCullEnable;
        bool transparencySunShadowEnable;
        bool forceFullRes;
        bool localSpace;
        bool opaque;
        bool actAsPointLight;
        bool killParticlesWithEmitter;
        bool forceNiceSorting;

        void CaptureFrom(const fb::EmitterTemplateData* d)
        {
            pointLightIntensity = d->m_PointLightIntensity;
            pointLightPivot = d->m_PointLightPivot;
            pointLightColor = d->m_PointLightColor;
            maxCount = d->m_MaxCount;
            lifetimeFrameCount = d->m_LifetimeFrameCount;
            timeScale = d->m_TimeScale;
            lifetime = d->m_Lifetime;
            visibleAfterDistance = d->m_VisibleAfterDistance;
            distanceScaleNearValue = d->m_DistanceScaleNearValue;
            pointLightRadius = d->m_PointLightRadius;
            vertexPixelLightingBlendFactor = d->m_VertexPixelLightingBlendFactor;
            globalLocalNormalBlendFactor = d->m_GlobalLocalNormalBlendFactor;
            softParticlesFadeDistanceMultiplier = d->m_SoftParticlesFadeDistanceMultiplier;
            lightWrapAroundFactor = d->m_LightWrapAroundFactor;
            lightMultiplier = d->m_LightMultiplier;
            distanceScaleFarValue = d->m_DistanceScaleFarValue;
            pointLightRandomIntensityMin = d->m_PointLightRandomIntensityMin;
            meshCullingDistance = d->m_MeshCullingDistance;
            pointLightRandomIntensityMax = d->m_PointLightRandomIntensityMax;
            maxSpawnDistance = d->m_MaxSpawnDistance;
            minScreenArea = d->m_MinScreenArea;
            distanceScaleLength = d->m_DistanceScaleLength;
            pointLightMaxClamp = d->m_PointLightMaxClamp;
            particleCullingFactor = d->m_ParticleCullingFactor;
            pointLightMinClamp = d->m_PointLightMinClamp;
            emittableType = d->m_EmittableType;
            followSpawnSource = d->m_FollowSpawnSource;
            repeatParticleSpawning = d->m_RepeatParticleSpawning;
            emissive = d->m_Emissive;
            exclusionVolumeCullEnable = d->m_ExclusionVolumeCullEnable;
            transparencySunShadowEnable = d->m_TransparencySunShadowEnable;
            forceFullRes = d->m_ForceFullRes;
            localSpace = d->m_LocalSpace;
            opaque = d->m_Opaque;
            actAsPointLight = d->m_ActAsPointLight;
            killParticlesWithEmitter = d->m_KillParticlesWithEmitter;
            forceNiceSorting = d->m_ForceNiceSorting;
        }
        void RestoreTo(fb::EmitterTemplateData* d) const
        {
            d->m_PointLightIntensity = pointLightIntensity;
            d->m_PointLightPivot = pointLightPivot;
            d->m_PointLightColor = pointLightColor;
            d->m_MaxCount = maxCount;
            d->m_LifetimeFrameCount = lifetimeFrameCount;
            d->m_TimeScale = timeScale;
            d->m_Lifetime = lifetime;
            d->m_VisibleAfterDistance = visibleAfterDistance;
            d->m_DistanceScaleNearValue = distanceScaleNearValue;
            d->m_PointLightRadius = pointLightRadius;
            d->m_VertexPixelLightingBlendFactor = vertexPixelLightingBlendFactor;
            d->m_GlobalLocalNormalBlendFactor = globalLocalNormalBlendFactor;
            d->m_SoftParticlesFadeDistanceMultiplier = softParticlesFadeDistanceMultiplier;
            d->m_LightWrapAroundFactor = lightWrapAroundFactor;
            d->m_LightMultiplier = lightMultiplier;
            d->m_DistanceScaleFarValue = distanceScaleFarValue;
            d->m_PointLightRandomIntensityMin = pointLightRandomIntensityMin;
            d->m_MeshCullingDistance = meshCullingDistance;
            d->m_PointLightRandomIntensityMax = pointLightRandomIntensityMax;
            d->m_MaxSpawnDistance = maxSpawnDistance;
            d->m_MinScreenArea = minScreenArea;
            d->m_DistanceScaleLength = distanceScaleLength;
            d->m_PointLightMaxClamp = pointLightMaxClamp;
            d->m_ParticleCullingFactor = particleCullingFactor;
            d->m_PointLightMinClamp = pointLightMinClamp;
            d->m_EmittableType = emittableType;
            d->m_FollowSpawnSource = followSpawnSource;
            d->m_RepeatParticleSpawning = repeatParticleSpawning;
            d->m_Emissive = emissive;
            d->m_ExclusionVolumeCullEnable = exclusionVolumeCullEnable;
            d->m_TransparencySunShadowEnable = transparencySunShadowEnable;
            d->m_ForceFullRes = forceFullRes;
            d->m_LocalSpace = localSpace;
            d->m_Opaque = opaque;
            d->m_ActAsPointLight = actAsPointLight;
            d->m_KillParticlesWithEmitter = killParticlesWithEmitter;
            d->m_ForceNiceSorting = forceNiceSorting;
        }
    };

    // this should be probably vec snapshot, since access will be easier, not just for colors
    struct ColorSnapshot
    {
        bool exists = false;
        bool hasPolynomial = false;

        fb::Vec3 color;
        fb::Vec3 color0;
        fb::Vec3 color1;
        fb::Vec4 coefficients;

        void CaptureFrom(fb::UpdateColorData* colorProc)
        {
            if (!colorProc)
            {
                exists = false;
                return;
            }

            exists = true;
            color = colorProc->m_Color;

            if (colorProc->m_Pre)
            {
                fb::TypeInfo* type = colorProc->m_Pre->GetType();
                if (type && type->GetTypeCode() == fb::BasicTypesEnum::kTypeCode_Class)
                {
                    fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(type);
                    if (classInfo->m_ClassId == fb::PolynomialColorInterpData::ClassId())
                    {
                        hasPolynomial = true;
                        fb::PolynomialColorInterpData* poly = static_cast<fb::PolynomialColorInterpData*>(colorProc->m_Pre);
                        color0 = poly->m_Color0;
                        color1 = poly->m_Color1;
                        coefficients = poly->m_Coefficients;
                    }
                }
            }
        }

        void RestoreTo(fb::UpdateColorData* colorProc) const
        {
            if (!exists || !colorProc)
                return;

            colorProc->m_Color = color;

            if (hasPolynomial && colorProc->m_Pre)
            {
                fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(colorProc->m_Pre->GetType());
                if (classInfo->m_ClassId == fb::PolynomialColorInterpData::ClassId())
                {
                    fb::PolynomialColorInterpData* poly = static_cast<fb::PolynomialColorInterpData*>(colorProc->m_Pre);
                    poly->m_Color0 = color0;
                    poly->m_Color1 = color1;
                    poly->m_Coefficients = coefficients;
                }
            }
        }

    };

    fb::UpdateColorData* colorProcessor = nullptr;
    Snapshot original;
    ColorSnapshot originalColor;
    bool captured = false;
};

struct PendingEmitterEdit
{
    std::string name;
    EmitterEditData::Snapshot templateData;
    EmitterEditData::ColorSnapshot colorData;
    bool hasColorData = false;
};

struct EmitterTreeNode
{
    std::string name;
    std::map<std::string, EmitterTreeNode> children;
    std::vector<fb::EmitterTemplateData*> emitters;

    void Clear()
    {
        children.clear();
        emitters.clear();
    }
};

struct SpawnedEffect
{
    uint32_t handle;
    fb::LinearTransform transform;
    std::string effectName;
};