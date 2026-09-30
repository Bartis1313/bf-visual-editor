#pragma once

namespace editor::enlighten::inspect
{
    void tick(); // render thread
    void renderOverlay(); // update or render thread, per backend
    void renderOverlayUI();
    void renderUI();
    void clear(); // level unload
}
