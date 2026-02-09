#pragma once

#include <string>
#include <unordered_set>
#include <map>
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

    int origSpotShape = 0;
    float origConeInnerAngle = 0.f;
    float origConeOuterAngle = 0.f;
    float origFrustumFov = 0.f;
    float origFrustumAspect = 0.f;
    float origOrthoWidth = 0.f;
    float origOrthoHeight = 0.f;
    bool origCastShadowsEnable = false;
    int origCastShadowsMinLevel = 0;

    float origPointWidth = 0.f;
    float origTranslucencyAmbient = 0.f;
    float origTranslucencyScale = 0.f;
    unsigned int origTranslucencyPower = 0;
    float origTranslucencyDistortion = 0.f;

    bool hasOverride = false;

    fb::Vec3 color{};
    fb::Vec3 particleColorScale{};
    fb::Vec3 enlightenColorScale{};
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
        if (!origCaptured)
            return;

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

// X-macro for all Visual Environment components. Define once, use everywhere.
//
// Each entry is X(Type, field) where:
//   Type  - PascalCase, used for type names and suffixes (fb::TypeComponentData, has##Type, orig##Type, edit##Type)
//   field - camelCase, used for struct members and function names (state->field, copy::field())
//
// To use, define a macro taking (Type, field), invoke VE_COMPONENTS with it, then undef:
//
//   #define COPY_ALL(Type, field) copy::field(&data.orig##Type, &ve->field);
//   VE_COMPONENTS(COPY_ALL)
//   #undef COPY_ALL
//
// This expands to copy::outdoorLight(&data.origOutdoorLight, &ve->outdoorLight); for each component.
// Adding a new component here automatically updates all places that use the macro.
#define VE_COMPONENTS(X) \
    X(OutdoorLight,      outdoorLight) \
    X(Enlighten,         enlighten) \
    X(Tonemap,           tonemap) \
    X(ColorCorrection,   colorCorrection) \
    X(Sky,               sky) \
    X(Fog,               fog) \
    X(Wind,              wind) \
    X(SunFlare,          sunFlare) \
    X(DynamicAO,         dynamicAO) \
    X(Dof,               dof) \
    X(Vignette,          vignette) \
    X(FilmGrain,         filmGrain) \
    X(LensScope,         lensScope) \
    X(CameraParams,      cameraParams) \
    X(ScreenEffect,      screenEffect) \
    X(DamageEffect,      damageEffect) \
    X(PlanarReflection,  planarReflection) \
    X(DynamicEnvmap,     dynamicEnvmap) \
    X(CharacterLighting, characterLighting) \
    X(MotionBlur,        motionBlur)

struct StateEditData
{
#define DECLARE_COMPONENT(Type, field) \
        bool has##Type = false; \
        void* captured##Type = nullptr; \
        fb::Type##ComponentData orig##Type{}; \
        fb::Type##ComponentData edit##Type{};
    VE_COMPONENTS(DECLARE_COMPONENT)
#undef DECLARE_COMPONENT

    bool overrideEnabled = true;
    bool captured = false;

    fb::VisualEnvironmentState* statePtr = nullptr;
    int stateIndex = -1;
    std::string stateName;

    void Reset() { *this = StateEditData{}; }

    void ResetCaptures()
    {
#define RESET(Type, field) has##Type = false; captured##Type = nullptr;
        VE_COMPONENTS(RESET)
#undef RESET
    }

    int ComponentCount() const
    {
        int c = 0;
#define COUNT(Type, field) c += has##Type;
        VE_COMPONENTS(COUNT)
#undef COUNT
        return c;
    }

    static int CountLiveComponents(fb::VisualEnvironmentState* state)
    {
        if (!state)
            return 0;

        int c = 0;
#define COUNT(Type, field) c += (state->field != nullptr);
        VE_COMPONENTS(COUNT)
#undef COUNT
        return c;
    }
};

struct StateEditEntry
{
    StateHash hash;
    StateEditData editData;
};

struct GlobalVEData
{
#define DECLARE_COMPONENT(Type, field) \
        fb::Type##ComponentData orig##Type{}; \
        fb::Type##ComponentData edit##Type{}; \
        bool field##OverrideEnabled = false;

    VE_COMPONENTS(DECLARE_COMPONENT)
#undef DECLARE_COMPONENT

        bool captured = false;
    bool globalOverrideEnabled = false;

    void Reset() { *this = GlobalVEData{}; }

    int EnabledOverrideCount() const
    {
        int c = 0;
#define COUNT(Type, field) c += field##OverrideEnabled;
        VE_COMPONENTS(COUNT)
#undef COUNT
            return c;
    }
};

// EmitterTemplateData wrapper
struct EmitterSnapshot
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

    void captureFrom(const fb::EmitterTemplateData* d);
    void restoreTo(fb::EmitterTemplateData* d) const;
};

struct EmitterColorSnapshot
{
    bool exists = false;
    bool hasPolynomial = false;
    fb::Vec3 color;
    fb::Vec3 color0;
    fb::Vec3 color1;
    fb::Vec4 coefficients;

    void captureFrom(fb::UpdateColorData* colorProc);
    void restoreTo(fb::UpdateColorData* colorProc) const;
};

struct EmitterEditData
{
    bool modified = false;
    fb::EmitterTemplateData* data = nullptr;
    fb::EmitterTemplate* lastTemplate = nullptr;
    std::string name;
    std::string category;
    fb::UpdateColorData* colorProcessor = nullptr;
    EmitterSnapshot original;
    EmitterColorSnapshot originalColor;
    bool captured = false;
};

struct PendingEmitterEdit
{
    std::string name;
    EmitterSnapshot templateData;
    EmitterColorSnapshot colorData;
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

// should be moved from this file, dunno where
template<size_t N>
struct PrettyName
{
    char buf[N * 2]{};

    constexpr PrettyName(const char* s)
    {
        size_t j = 0;
        for (size_t i = 0; s[i]; ++i)
        {
            char c = s[i];
            if (i > 0 && c >= 'A' && c <= 'Z')
            {
                bool prevLower = s[i - 1] >= 'a' && s[i - 1] <= 'z';
                bool prevUpper = s[i - 1] >= 'A' && s[i - 1] <= 'Z';
                bool nextLower = s[i + 1] && s[i + 1] >= 'a' && s[i + 1] <= 'z';

                if (prevLower || (prevUpper && nextLower))
                    buf[j++] = ' ';
            }
            buf[j++] = c;
        }
    }
};

#define PRETTY_CASE_NAME(str) (PrettyName<sizeof(str)>(str).buf)