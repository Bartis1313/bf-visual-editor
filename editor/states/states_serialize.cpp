#include "states.h"

#include "../comps/comps_serialize.h"

namespace editor::states
{
    json serializeStateData(const StateEditData& data)
    {
        json j;
        j["overrideEnabled"] = data.overrideEnabled;

        if (!data.captured)
            return j;

#define SERIALIZE_COMPONENT(jsonName, Name) \
    if (data.has##Name) { j[jsonName] = comps::serialize##Name(data.edit##Name); }

        SERIALIZE_COMPONENT("outdoorLight", OutdoorLight);
        SERIALIZE_COMPONENT("enlighten", Enlighten);
        SERIALIZE_COMPONENT("tonemap", Tonemap);
        SERIALIZE_COMPONENT("colorCorrection", ColorCorrection);
        SERIALIZE_COMPONENT("sky", Sky);
        SERIALIZE_COMPONENT("fog", Fog);
        SERIALIZE_COMPONENT("wind", Wind);
        SERIALIZE_COMPONENT("sunflare", SunFlare);
        SERIALIZE_COMPONENT("dynamicAO", DynamicAO);
        SERIALIZE_COMPONENT("dof", Dof);
        SERIALIZE_COMPONENT("vignette", Vignette);
        SERIALIZE_COMPONENT("filmGrain", FilmGrain);
        SERIALIZE_COMPONENT("lensScope", LensScope);
        SERIALIZE_COMPONENT("cameraParams", CameraParams);
        SERIALIZE_COMPONENT("screenEffect", ScreenEffect);
        SERIALIZE_COMPONENT("damageEffect", DamageEffect);
        SERIALIZE_COMPONENT("planarReflection", PlanarReflection);
        SERIALIZE_COMPONENT("dynamicEnvmap", DynamicEnvmap);
        SERIALIZE_COMPONENT("characterLighting", CharacterLighting);
        SERIALIZE_COMPONENT("motionBlur", MotionBlur);

#undef SERIALIZE_COMPONENT

        return j;
    }

    void deserializeStateData(const json& j, StateEditData& data)
    {
        data.overrideEnabled = j.value("overrideEnabled", true);
        data.captured = true;

#define DESERIALIZE_COMPONENT(jsonName, Name) \
    if (j.contains(jsonName)) { \
        data.has##Name = true; \
        comps::deserialize##Name(j[jsonName], data.edit##Name); \
    }

        DESERIALIZE_COMPONENT("outdoorLight", OutdoorLight);
        DESERIALIZE_COMPONENT("enlighten", Enlighten);
        DESERIALIZE_COMPONENT("tonemap", Tonemap);
        DESERIALIZE_COMPONENT("colorCorrection", ColorCorrection);
        DESERIALIZE_COMPONENT("sky", Sky);
        DESERIALIZE_COMPONENT("fog", Fog);
        DESERIALIZE_COMPONENT("wind", Wind);
        DESERIALIZE_COMPONENT("sunflare", SunFlare);
        DESERIALIZE_COMPONENT("dynamicAO", DynamicAO);
        DESERIALIZE_COMPONENT("dof", Dof);
        DESERIALIZE_COMPONENT("vignette", Vignette);
        DESERIALIZE_COMPONENT("filmGrain", FilmGrain);
        DESERIALIZE_COMPONENT("lensScope", LensScope);
        DESERIALIZE_COMPONENT("cameraParams", CameraParams);
        DESERIALIZE_COMPONENT("screenEffect", ScreenEffect);
        DESERIALIZE_COMPONENT("damageEffect", DamageEffect);
        DESERIALIZE_COMPONENT("planarReflection", PlanarReflection);
        DESERIALIZE_COMPONENT("dynamicEnvmap", DynamicEnvmap);
        DESERIALIZE_COMPONENT("characterLighting", CharacterLighting);
        DESERIALIZE_COMPONENT("motionBlur", MotionBlur);

#undef DESERIALIZE_COMPONENT
    }
}