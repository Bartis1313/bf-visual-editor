#include "global_ve.h"

#include "../comps/comps_serialize.h"

namespace editor::global_ve
{
    json serialize()
    {
        json j;
        auto& d = getData();
        j["globalOverrideEnabled"] = d.globalOverrideEnabled;

#define SAVE(Type, field) \
        if (d.field##OverrideEnabled) { \
            j[#Type]["enabled"] = true; \
            j[#Type]["data"] = comps::serialize##Type(d.edit##Type); \
        }
        VE_COMPONENTS(SAVE)
#undef SAVE
        return j;
    }

    void deserialize(const json& j)
    {
        auto& d = getData();
        d.globalOverrideEnabled = j.value("globalOverrideEnabled", false);

#define LOAD(Type, field) \
        if (j.contains(#Type) && j[#Type].value("enabled", false)) { \
            d.field##OverrideEnabled = true; \
            if (j[#Type].contains("data")) \
                comps::deserialize##Type(j[#Type]["data"], d.edit##Type); \
        }
        VE_COMPONENTS(LOAD)
#undef LOAD

#undef LOAD_COMPONENT
    }
}