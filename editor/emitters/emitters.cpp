#include "emitters.h"
#include "../editor_context.h"
#include "../../utils/log.h"
#include <sstream>

namespace editor::emitters
{
    std::unordered_map<fb::EmitterTemplateData*, EmitterEditData> emitterMap;
    EmitterTreeNode emitterTree;
    fb::EmitterTemplateData* selectedEmitter = nullptr;
    char searchBuf[64] = { };
    bool scanned = false;
    std::vector<PendingEmitterEdit> pendingEdits;

    std::unordered_map<fb::EmitterTemplateData*, EmitterEditData>& getMap() { return emitterMap; }
    EmitterTreeNode& getTree() { return emitterTree; }
    fb::EmitterTemplateData* getSelected() { return selectedEmitter; }
    void setSelected(fb::EmitterTemplateData* data) { selectedEmitter = data; }
    std::vector<PendingEmitterEdit>& getPendingEdits() { return pendingEdits; }

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
        emitterMap.clear();
        emitterTree.Clear();
        selectedEmitter = nullptr;
        searchBuf[0] = '\0';
        scanned = false;
        pendingEdits.clear();
    }

    static void InsertIntoTree(fb::EmitterTemplateData* data, const std::string& fullPath)
    {
        std::vector<std::string> parts;
        std::stringstream ss(fullPath);
        std::string part;

        while (std::getline(ss, part, '/'))
        {
            if (!part.empty())
                parts.push_back(part);
        }

        if (parts.empty())
        {
            emitterTree.emitters.push_back(data);
            return;
        }

        EmitterTreeNode* node = &emitterTree;
        for (size_t i = 0; i < parts.size() - 1; i++)
        {
            node = &node->children[parts[i]];
            node->name = parts[i];
        }

        node->emitters.push_back(data);
    }

    void scan()
    {
        emitterMap.clear();
        emitterTree.Clear();

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
                if (classInfo->m_ClassId != fb::EmitterTemplateData::ClassId())
                    continue;

                fb::EmitterTemplateData* emitterData = static_cast<fb::EmitterTemplateData*>(obj);

                EmitterEditData editData;
                editData.data = emitterData;
                editData.name = emitterData->m_Name ? emitterData->m_Name : "Unknown";
                editData.original.captureFrom(emitterData);
                editData.captured = true;

                for (fb::ProcessorData* proc = emitterData->m_RootProcessor; proc; proc = proc->m_NextProcessor)
                {
                    fb::TypeInfo* type = proc->GetType();
                    if (!type || type->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                        continue;

                    fb::ClassInfo* procClassInfo = static_cast<fb::ClassInfo*>(type);
                    if (procClassInfo->m_ClassId == fb::UpdateColorData::ClassId())
                    {
                        editData.colorProcessor = static_cast<fb::UpdateColorData*>(proc);
                        editData.originalColor.captureFrom(editData.colorProcessor);
                    }
                }

                emitterMap[emitterData] = editData;
                InsertIntoTree(emitterData, editData.name);
            }
        }

        applyPendingEdits();

        scanned = true;
        logger::info("Found {} emitters", emitterMap.size());
    }

    void onCreated(fb::EmitterTemplate* emitter, fb::EmitterTemplateData* data)
    {
        if (!emitter || !data)
            return;

        auto it = emitterMap.find(data);
        if (it == emitterMap.end())
            return;

        it->second.lastTemplate = emitter;
    }

    void captureCurrentState(const EmitterEditData& edit, EmitterSnapshot& outTemplate, EmitterColorSnapshot& outColor)
    {
        outTemplate.captureFrom(edit.data);

        outColor.exists = false;
        if (edit.colorProcessor)
        {
            outColor.captureFrom(edit.colorProcessor);
        }
    }

    void applyPendingEdits()
    {
        if (pendingEdits.empty())
            return;

        int appliedCount = 0;

        for (auto it = pendingEdits.begin(); it != pendingEdits.end(); )
        {
            bool applied = false;

            for (auto& [dataPtr, edit] : emitterMap)
            {
                if (edit.name == it->name)
                {
                    it->templateData.restoreTo(edit.data);

                    if (it->hasColorData && edit.colorProcessor)
                        it->colorData.restoreTo(edit.colorProcessor);

                    edit.modified = true;
                    logger::info("ApplyPendingEmitterEdits: Applied '{}'", it->name.c_str());
                    appliedCount++;
                    applied = true;
                    break;
                }
            }

            if (applied)
                it = pendingEdits.erase(it);
            else
                ++it;
        }

        if (appliedCount > 0)
            logger::info("ApplyPendingEmitterEdits: Applied {} pending edits, {} remaining", appliedCount, pendingEdits.size());
    }
}

void EmitterSnapshot::captureFrom(const fb::EmitterTemplateData* d)
{
    using T = fb::EmitterTemplateData;

    memcpy(&pointLightIntensity, &d->m_PointLightIntensity, offsetof(T, m_Name) - offsetof(T, m_PointLightIntensity));
    lifetimeFrameCount = d->m_LifetimeFrameCount;
    timeScale = d->m_TimeScale;
    lifetime = d->m_Lifetime;
    visibleAfterDistance = d->m_VisibleAfterDistance;
    emittableType = d->m_EmittableType;
    memcpy(&distanceScaleNearValue, &d->m_DistanceScaleNearValue, offsetof(T, m_FollowSpawnSource) - offsetof(T, m_DistanceScaleNearValue));
    memcpy(&followSpawnSource, &d->m_FollowSpawnSource, offsetof(T, _0x00B7) - offsetof(T, m_FollowSpawnSource));
}

void EmitterSnapshot::restoreTo(fb::EmitterTemplateData* d) const
{
    using T = fb::EmitterTemplateData;

    memcpy(&d->m_PointLightIntensity, &pointLightIntensity, offsetof(T, m_Name) - offsetof(T, m_PointLightIntensity));
    d->m_LifetimeFrameCount = lifetimeFrameCount;
    d->m_TimeScale = timeScale;
    d->m_Lifetime = lifetime;
    d->m_VisibleAfterDistance = visibleAfterDistance;
    d->m_EmittableType = emittableType;
    memcpy(&d->m_DistanceScaleNearValue, &distanceScaleNearValue, offsetof(T, m_FollowSpawnSource) - offsetof(T, m_DistanceScaleNearValue));
    memcpy(&d->m_FollowSpawnSource, &followSpawnSource, offsetof(T, _0x00B7) - offsetof(T, m_FollowSpawnSource));
}

void EmitterColorSnapshot::captureFrom(fb::UpdateColorData* colorProc)
{
    if (!colorProc)
    {
        exists = false;
        return;
    }

    exists = true;
    color = colorProc->m_Color;
    hasPolynomial = false;

    if (colorProc->m_Pre)
    {
        fb::TypeInfo* type = colorProc->m_Pre->GetType();
        if (type && type->GetTypeCode() == fb::BasicTypesEnum::kTypeCode_Class)
        {
            fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(type);
            if (classInfo->m_ClassId == fb::PolynomialColorInterpData::ClassId())
            {
                hasPolynomial = true;
                fb::PolynomialColorInterpData* poly = static_cast<fb::PolynomialColorInterpData*>(colorProc->m_Pre);
                color0 = poly->m_Color0;
                color1 = poly->m_Color1;
                coefficients = poly->m_Coefficients;
            }
        }
    }
}

void EmitterColorSnapshot::restoreTo(fb::UpdateColorData* colorProc) const
{
    if (!exists || !colorProc)
        return;

    colorProc->m_Color = color;

    if (hasPolynomial && colorProc->m_Pre)
    {
        fb::TypeInfo* type = colorProc->m_Pre->GetType();
        if (type && type->GetTypeCode() == fb::BasicTypesEnum::kTypeCode_Class)
        {
            fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(type);
            if (classInfo->m_ClassId == fb::PolynomialColorInterpData::ClassId())
            {
                fb::PolynomialColorInterpData* poly = static_cast<fb::PolynomialColorInterpData*>(colorProc->m_Pre);
                poly->m_Color0 = color0;
                poly->m_Color1 = color1;
                poly->m_Coefficients = coefficients;
            }
        }
    }
}