#include "emitters.h"
#include "../serialize/serialize.h"

namespace editor::emitters
{
    void serializeSnapshot(json& j, const EmitterSnapshot& s)
    {
        JSON_SET_VEC4(j, "pointLightIntensity", s.pointLightIntensity);
        JSON_SET_VEC3(j, "pointLightPivot", s.pointLightPivot);
        JSON_SET_VEC3(j, "pointLightColor", s.pointLightColor);

        JSON_SET(j, "maxCount", s.maxCount);
        JSON_SET(j, "lifetimeFrameCount", s.lifetimeFrameCount);

        JSON_SET(j, "timeScale", s.timeScale);
        JSON_SET(j, "lifetime", s.lifetime);
        JSON_SET(j, "visibleAfterDistance", s.visibleAfterDistance);
        JSON_SET(j, "distanceScaleNearValue", s.distanceScaleNearValue);
        JSON_SET(j, "pointLightRadius", s.pointLightRadius);
        JSON_SET(j, "vertexPixelLightingBlendFactor", s.vertexPixelLightingBlendFactor);
        JSON_SET(j, "globalLocalNormalBlendFactor", s.globalLocalNormalBlendFactor);
        JSON_SET(j, "softParticlesFadeDistanceMultiplier", s.softParticlesFadeDistanceMultiplier);
        JSON_SET(j, "lightWrapAroundFactor", s.lightWrapAroundFactor);
        JSON_SET(j, "lightMultiplier", s.lightMultiplier);
        JSON_SET(j, "distanceScaleFarValue", s.distanceScaleFarValue);
        JSON_SET(j, "pointLightRandomIntensityMin", s.pointLightRandomIntensityMin);
        JSON_SET(j, "meshCullingDistance", s.meshCullingDistance);
        JSON_SET(j, "pointLightRandomIntensityMax", s.pointLightRandomIntensityMax);
        JSON_SET(j, "maxSpawnDistance", s.maxSpawnDistance);
        JSON_SET(j, "minScreenArea", s.minScreenArea);
        JSON_SET(j, "distanceScaleLength", s.distanceScaleLength);
        JSON_SET(j, "pointLightMaxClamp", s.pointLightMaxClamp);
        JSON_SET(j, "particleCullingFactor", s.particleCullingFactor);
        JSON_SET(j, "pointLightMinClamp", s.pointLightMinClamp);

        JSON_SET(j, "emittableType", static_cast<int>(s.emittableType));
        JSON_SET(j, "followSpawnSource", s.followSpawnSource);
        JSON_SET(j, "repeatParticleSpawning", s.repeatParticleSpawning);
        JSON_SET(j, "emissive", s.emissive);
        JSON_SET(j, "exclusionVolumeCullEnable", s.exclusionVolumeCullEnable);
        JSON_SET(j, "transparencySunShadowEnable", s.transparencySunShadowEnable);
        JSON_SET(j, "forceFullRes", s.forceFullRes);
        JSON_SET(j, "localSpace", s.localSpace);
        JSON_SET(j, "opaque", s.opaque);
        JSON_SET(j, "actAsPointLight", s.actAsPointLight);
        JSON_SET(j, "killParticlesWithEmitter", s.killParticlesWithEmitter);
        JSON_SET(j, "forceNiceSorting", s.forceNiceSorting);
    }

    void deserializeSnapshot(const json& j, EmitterSnapshot& s)
    {
        JSON_GET_VEC4(j, "pointLightIntensity", s.pointLightIntensity);
        JSON_GET_VEC3(j, "pointLightPivot", s.pointLightPivot);
        JSON_GET_VEC3(j, "pointLightColor", s.pointLightColor);

        JSON_GET_INT(j, "maxCount", s.maxCount);
        JSON_GET_INT(j, "lifetimeFrameCount", s.lifetimeFrameCount);

        JSON_GET(j, "timeScale", s.timeScale);
        JSON_GET(j, "lifetime", s.lifetime);
        JSON_GET(j, "visibleAfterDistance", s.visibleAfterDistance);
        JSON_GET(j, "distanceScaleNearValue", s.distanceScaleNearValue);
        JSON_GET(j, "pointLightRadius", s.pointLightRadius);
        JSON_GET(j, "vertexPixelLightingBlendFactor", s.vertexPixelLightingBlendFactor);
        JSON_GET(j, "globalLocalNormalBlendFactor", s.globalLocalNormalBlendFactor);
        JSON_GET(j, "softParticlesFadeDistanceMultiplier", s.softParticlesFadeDistanceMultiplier);
        JSON_GET(j, "lightWrapAroundFactor", s.lightWrapAroundFactor);
        JSON_GET(j, "lightMultiplier", s.lightMultiplier);
        JSON_GET(j, "distanceScaleFarValue", s.distanceScaleFarValue);
        JSON_GET(j, "pointLightRandomIntensityMin", s.pointLightRandomIntensityMin);
        JSON_GET(j, "meshCullingDistance", s.meshCullingDistance);
        JSON_GET(j, "pointLightRandomIntensityMax", s.pointLightRandomIntensityMax);
        JSON_GET(j, "maxSpawnDistance", s.maxSpawnDistance);
        JSON_GET(j, "minScreenArea", s.minScreenArea);
        JSON_GET(j, "distanceScaleLength", s.distanceScaleLength);
        JSON_GET(j, "pointLightMaxClamp", s.pointLightMaxClamp);
        JSON_GET(j, "particleCullingFactor", s.particleCullingFactor);
        JSON_GET(j, "pointLightMinClamp", s.pointLightMinClamp);

        JSON_GET_ENUM(j, "emittableType", s.emittableType, fb::EmittableType);

        JSON_GET_BOOL(j, "followSpawnSource", s.followSpawnSource);
        JSON_GET_BOOL(j, "repeatParticleSpawning", s.repeatParticleSpawning);
        JSON_GET_BOOL(j, "emissive", s.emissive);
        JSON_GET_BOOL(j, "exclusionVolumeCullEnable", s.exclusionVolumeCullEnable);
        JSON_GET_BOOL(j, "transparencySunShadowEnable", s.transparencySunShadowEnable);
        JSON_GET_BOOL(j, "forceFullRes", s.forceFullRes);
        JSON_GET_BOOL(j, "localSpace", s.localSpace);
        JSON_GET_BOOL(j, "opaque", s.opaque);
        JSON_GET_BOOL(j, "actAsPointLight", s.actAsPointLight);
        JSON_GET_BOOL(j, "killParticlesWithEmitter", s.killParticlesWithEmitter);
        JSON_GET_BOOL(j, "forceNiceSorting", s.forceNiceSorting);
    }

    void serializeColorSnapshot(json& j, const EmitterColorSnapshot& s)
    {
        JSON_SET(j, "exists", s.exists);
        if (!s.exists)
            return;

        JSON_SET_VEC3(j, "color", s.color);
        JSON_SET(j, "hasPolynomial", s.hasPolynomial);

        if (s.hasPolynomial)
        {
            JSON_SET_VEC3(j, "color0", s.color0);
            JSON_SET_VEC3(j, "color1", s.color1);
            JSON_SET_VEC4(j, "coefficients", s.coefficients);
        }
    }

    void deserializeColorSnapshot(const json& j, EmitterColorSnapshot& s)
    {
        JSON_GET_BOOL(j, "exists", s.exists);
        if (!s.exists)
            return;

        JSON_GET_VEC3(j, "color", s.color);
        JSON_GET_BOOL(j, "hasPolynomial", s.hasPolynomial);

        if (s.hasPolynomial)
        {
            JSON_GET_VEC3(j, "color0", s.color0);
            JSON_GET_VEC3(j, "color1", s.color1);
            JSON_GET_VEC4(j, "coefficients", s.coefficients);
        }
    }
}