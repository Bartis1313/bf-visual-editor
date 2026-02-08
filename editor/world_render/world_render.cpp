#include "world_render.h"
#include "../editor_context.h"
#include "../copy/copy.h"
#include "../../utils/log.h"
#include "../editor.h"

namespace editor::world_render
{
    fb::WorldRenderSettings original{ };
    fb::WorldRenderSettings edit{ };
    bool captured = false;
    bool overrideEnabled = false;

    void init() {}
    void shutdown() { clear(); }

    void clear()
    {
        if (captured)
        {
            fb::WorldRenderSettings* live = getWorldRenderSettings();
            if (live)
                copy::worldRenderSettings(live, &original);
        }

        captured = false;
        overrideEnabled = false;
        memset(&original, 0, sizeof(original));
        memset(&edit, 0, sizeof(edit));
    }

    bool hasCaptured() { return captured; }
    bool isOverrideEnabled() { return overrideEnabled; }
    void setOverrideEnabled(bool enabled) { overrideEnabled = enabled; }

    fb::WorldRenderSettings* getOriginal() { return &original; }
    fb::WorldRenderSettings* getEdit() { return &edit; }

    void capture()
    {
        if (captured)
            return;

        fb::WorldRenderSettings* live = getWorldRenderSettings();
        if (!live)
        {
            logger::debug("WorldRender::Capture - No live settings");
            return;
        }

        copy::worldRenderSettings(&original, live);
        copy::worldRenderSettings(&edit, live);
        captured = true;

        logger::debug("WorldRender: Captured");
    }

    void apply()
    {
        if (!captured || !overrideEnabled || !isSafeToOperate())
            return;

        fb::WorldRenderSettings* live = getWorldRenderSettings();
        if (live)
            copy::worldRenderSettings(live, &edit);
    }

    void reset()
    {
        if (!captured)
            return;

        copy::worldRenderSettings(&edit, &original);

        fb::WorldRenderSettings* live = getWorldRenderSettings();
        if (live)
            copy::worldRenderSettings(live, &original);

        logger::debug("WorldRender: Reset");
    }
}