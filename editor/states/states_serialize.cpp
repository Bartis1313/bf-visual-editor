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

#define SERIALIZE(Type, field) \
        if (data.has##Type) j[#field] = comps::serialize##Type(data.edit##Type);
        VE_COMPONENTS(SERIALIZE)
#undef SERIALIZE

        return j;
    }

    void deserializeStateData(const json& j, StateEditData& data)
    {
        data.overrideEnabled = j.value("overrideEnabled", true);
        data.captured = true;

#define DESERIALIZE(Type, field) \
        if (j.contains(#field)) { \
            data.has##Type = true; \
            comps::deserialize##Type(j[#field], data.edit##Type); \
        }
        VE_COMPONENTS(DESERIALIZE)
#undef DESERIALIZE
    }
}