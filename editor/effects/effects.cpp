#include "effects.h"
#include "../editor_context.h"
#include "../ui/ui_helpers.h"
#include "../../utils/log.h"
#include <imgui.h>
#include <algorithm>

namespace editor::effects
{
    void init()
    {
        clear();
        transform.m_right = { 1.0f, 0.0f, 0.0f };
        transform.m_up = { 0.0f, 1.0f, 0.0f };
        transform.m_forward = { 0.0f, 0.0f, 1.0f };
        transform.m_trans = { 0.0f, 0.0f, 0.0f };
    }

    void shutdown()
    {
        stopAll();
        clear();
    }

    void clear()
    {
        assets.clear();
        spawned.clear();
        selected = nullptr;
    }

    void scanAssets()
    {
        assets.clear();

        fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
        if (!rm)
            return;

        for (const auto& comp : rm->m_compartments)
        {
            if (!comp) continue;

            for (const auto& obj : comp->m_objects)
            {
                if (!obj) continue;

                fb::TypeInfo* typeInfo = obj->GetType();
                if (!typeInfo || typeInfo->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                    continue;

                fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(typeInfo);
                if (classInfo->m_ClassId != fb::EffectBlueprint::ClassId())
                    continue;

                fb::EffectBlueprint* eB = static_cast<fb::EffectBlueprint*>(obj);
                assets.emplace_back(eB->m_Name, eB);
            }
        }

        std::sort(assets.begin(), assets.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });

        logger::info("Scanned {} effect assets", assets.size());
    }

    uint32_t spawnAtTransform(fb::Asset* effect, const fb::LinearTransform& transform)
    {
        if (!effect)
            return 0;

        fb::EffectManager* effectManager = fb::EffectManager::GetInstance();
        if (!effectManager)
            return 0;

        fb::EffectParams params = {};
        params.m_paramCount = 0;

        return effectManager->playEffect(effect, const_cast<fb::LinearTransform*>(&transform), &params, false);
    }

    void stop(uint32_t handle)
    {
        if (handle == 0)
            return;

        fb::EffectManager* effectManager = fb::EffectManager::GetInstance();
        if (!effectManager)
            return;

        effectManager->stopEffect(handle);
    }

    void stopAll()
    {
        for (const auto& effect : spawned)
            stop(effect.handle);
        spawned.clear();
    }
}