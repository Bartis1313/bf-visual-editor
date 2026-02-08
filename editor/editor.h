#pragma once

#include "../SDK/sdk.h"

namespace editor
{
    void init();
    void shutdown();

    void render();
    void onManagerUpdateBegin(fb::VisualEnvironmentManager* manager);
    void onManagerUpdateEnd(fb::VisualEnvironmentManager* manager);
    void onVisualEnvironmentUpdated(fb::VisualEnvironment* ve);
    void onMessage(uint32_t category, uint32_t type);

    void onVisualEnvironmentEntityCreated(fb::VisualEnvironmentEntity* entity, fb::VisualEnvironmentEntityData* data);
    void onVisualEnvironmentEntityDestroyed(fb::VisualEnvironmentEntity* entity);
    void onLightEntityCreated(fb::LocalLightEntity* entity, fb::LocalLightEntityData* data);
    void onLightEntityDestroyed(fb::LocalLightEntity* entity);
    void onEmitterCreated(fb::EmitterTemplate* emitter, fb::EmitterTemplateData* data);

    bool& isEnabled();
}