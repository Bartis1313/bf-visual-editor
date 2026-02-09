#include "states.h"
#include "../editor_context.h"
#include "../copy/copy.h"

namespace editor::states
{
    void captureStateData(fb::VisualEnvironmentState* state, StateEditData& data)
    {
        if (!state || levelUnloadingSignaled || !isSafeToOperate())
            return;

        data.captured = true;
        data.statePtr = state;

#define CAPTURE(Type, field) \
            if (!levelUnloadingSignaled && state->field) { \
                if (!data.has##Type || data.captured##Type != static_cast<void*>(state->field)) { \
                    data.has##Type = true; \
                    data.captured##Type = static_cast<void*>(state->field); \
                    copy::field(&data.orig##Type, state->field); \
                    copy::field(&data.edit##Type, state->field); \
                } \
            }
        VE_COMPONENTS(CAPTURE)
#undef CAPTURE
    }

    void applyEdits(fb::VisualEnvironmentState* state, const StateEditData& data)
    {
        if (!state || !data.captured || levelUnloadingSignaled || !isSafeToOperate())
            return;

#define APPLY(Type, field) \
            if (!levelUnloadingSignaled && data.has##Type && state->field) \
                copy::field(state->field, &data.edit##Type);
        VE_COMPONENTS(APPLY)
#undef APPLY
    }

    void restoreState(fb::VisualEnvironmentState* state, const StateEditData& data)
    {
        if (!state || !data.captured)
            return;

#define RESTORE(Type, field) \
            if (data.has##Type && state->field) \
                copy::field(state->field, &data.orig##Type);
        VE_COMPONENTS(RESTORE)
#undef RESTORE
    }

    void resetToOriginal(StateEditData& data)
    {
        if (!data.captured)
            return;

#define RESET(Type, field) \
            if (data.has##Type) copy::field(&data.edit##Type, &data.orig##Type);
        VE_COMPONENTS(RESET)
#undef RESET
    }

    void restoreEditValuesFromEntry(StateEditData& dst, const StateEditData& src)
    {
        dst.overrideEnabled = src.overrideEnabled;

#define RESTORE_EDIT(Type, field) \
            if (dst.has##Type && src.has##Type) \
                copy::field(&dst.edit##Type, &src.edit##Type);
        VE_COMPONENTS(RESTORE_EDIT)
#undef RESTORE_EDIT
    }

    void copyEditDataDirectly(StateEditData& dst, const StateEditData& src)
    {
        dst.overrideEnabled = src.overrideEnabled;

#define COPY_EDIT(Type, field) \
            if (dst.has##Type) copy::field(&dst.edit##Type, &src.edit##Type);
        VE_COMPONENTS(COPY_EDIT)
#undef COPY_EDIT
    }
}