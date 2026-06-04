#include "effects.h"
#include "../editor_context.h"
#include "../ui/ui_helpers.h"
#include "../../utils/log.h"
#include <imgui.h>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

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
        selectedName.clear();
    }

    void scanAssets()
    {
        assets.clear();

        fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
        if (!rm)
            return;

        std::unordered_set<fb::EffectBlueprint*> seen;
        std::unordered_map<std::string, int> nameUseCounts;

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

                fb::EffectBlueprint* eB = reinterpret_cast<fb::EffectBlueprint*>(obj);
                if (!seen.insert(eB).second)
                    continue;

                std::string baseName = eB->m_Name ? eB->m_Name : "Unknown";
                int& useCount = nameUseCounts[baseName];
                std::string finalName = (useCount == 0)
                    ? baseName
                    : baseName + " (" + std::to_string(useCount) + ")";
                ++useCount;

                assets.emplace_back(std::move(finalName), eB);
            }
        }

        std::sort(assets.begin(), assets.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });

        logger::info("Scanned {} effect assets", assets.size());
    }

    // crash due to that
#pragma optimize("", off)
    uint32_t spawnAtTransform(fb::Asset* effect, const fb::LinearTransform& transform)
    {
        if (!effect)
            return 0;

        fb::EffectManager* effectManager = fb::EffectManager::GetInstance();
        if (!effectManager)
            return 0;

        fb::EffectParams params = {};
        params.m_paramCount = 0;

        uint32_t handle = 0;
        __try
        {
            handle = effectManager->playEffect(effect, const_cast<fb::LinearTransform*>(&transform), &params, false);
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            return 0;
		}
        return handle;
    }
#pragma optimize("", on)

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