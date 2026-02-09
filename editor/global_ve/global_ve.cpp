#include "global_ve.h"
#include "../editor_context.h"
#include "../copy/copy.h"

namespace editor::global_ve
{
    GlobalVEData data;

    GlobalVEData& getData() { return data; }

    void init()
    {
        clear();
    }

    void shutdown()
    {
        clear();
    }

    void clear()
    {
        data.Reset();
    }

    bool hasCaptured()
    {
        return data.captured;
    }

    bool isEnabled()
    {
        return data.globalOverrideEnabled;
    }

    int enabledCount()
    {
        return data.EnabledOverrideCount();
    }

    void capture(fb::VisualEnvironment* ve)
    {
        if (!ve || data.captured)
            return;

        if (levelUnloadingSignaled || !isSafeToOperate())
            return;

#define CAPTURE(Type, field) \
        copy::field(&data.orig##Type, &ve->field); \
        copy::field(&data.edit##Type, &ve->field);
        VE_COMPONENTS(CAPTURE)
#undef CAPTURE

            data.captured = true;
    }

    void apply(fb::VisualEnvironment* ve)
    {
        if (!ve || !data.captured || !data.globalOverrideEnabled)
            return;

#define APPLY(Type, field) \
        if (data.field##OverrideEnabled) \
            copy::field(&ve->field, &data.edit##Type);
        VE_COMPONENTS(APPLY)
#undef APPLY
    }

    void onUpdated(fb::VisualEnvironment* ve)
    {
        if (!ve)
            return;

        if (!data.captured)
            capture(ve);

        if (data.captured && data.globalOverrideEnabled)
            apply(ve);
    }

    void resetToOriginal()
    {
        if (!data.captured)
            return;

#define RESET(Type, field) copy::field(&data.edit##Type, &data.orig##Type);
        VE_COMPONENTS(RESET)
#undef RESET
    }

    void resetComponent(int index)
    {
        if (!data.captured)
            return;

        int i = 0;
#define RESET_IDX(Type, field) \
        if (index == i++) { copy::field(&data.edit##Type, &data.orig##Type); return; }
        VE_COMPONENTS(RESET_IDX)
#undef RESET_IDX
    }
}