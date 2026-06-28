#include "emitters.h"
#include "../editor_context.h"
#include "../../utils/log.h"
#include "../render/render.h"
#include <sstream>
#include <imgui.h>
#include <cstdio>
#include <cstring>
#include <cfloat>
#include <unordered_set>
#include <mutex>
#include <vector>

namespace editor::emitters
{
    std::unordered_map<fb::EmitterTemplateData*, EmitterEditData> emitterMap;
    EmitterTreeNode emitterTree;
    fb::EmitterTemplateData* selectedEmitter = nullptr;
    char searchBuf[64] = { };
    bool scanned = false;
    std::vector<PendingEmitterEdit> pendingEdits;

    static std::unordered_set<void*> g_emitterEntities;
    static std::unordered_map<void*, fb::EmitterEntityData*> g_emitterData;
    static std::unordered_map<void*, std::string> g_emitterNames;

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
        g_emitterEntities.clear();
        g_emitterData.clear();
        g_emitterNames.clear();
    }

    static std::string normalizePath(const char* m)
    {
        std::string s = m ? m : "";
        for (auto& c : s)
            if (c == '\\') c = '/';
        return s;
    }

    static void InsertIntoTree(fb::EmitterTemplateData* data, const std::string& fullPath)
    {
        std::vector<std::string> parts;
        {
            std::stringstream ss(fullPath);
            std::string part;
            while (std::getline(ss, part, '/'))
                if (!part.empty()) parts.push_back(part);
        }

        if (parts.empty())
        {
            emitterTree.emitters.push_back(data);
            return;
        }

        EmitterTreeNode* node = &emitterTree;
        for (size_t i = 0; i + 1 < parts.size(); ++i)
        {
            node = &node->children[parts[i]];
            node->name = parts[i];
        }
        node->emitters.push_back(data);
    }

#if defined(BFVE_GAME_BF4)
    static std::unordered_set<fb::EmitterAsset*> g_placedDocs;
#endif

    static std::unordered_map<std::string, int> nameUseCounts;
    static std::unordered_map<std::string, std::vector<fb::EmitterAsset*>> nameToDocs;

    static void RegisterEmitter(fb::EmitterTemplateData* emitterData, const std::string& displayName, fb::EmitterAsset* sourceDoc = nullptr)
    {
        if (!emitterData) return;
        if (emitterMap.count(emitterData)) return;

        std::string baseName = displayName.empty() ? std::string{ "Unknown" } : displayName;
        int& useCount = nameUseCounts[baseName];
        std::string finalName = (useCount == 0) ? baseName
            : baseName + " (" + std::to_string(useCount) + ")";
        ++useCount;

        if (sourceDoc)
            nameToDocs[baseName].push_back(sourceDoc);

        EmitterEditData editData;
        editData.data = emitterData;
        editData.name = finalName;
        editData.key = finalName;
        editData.original.captureFrom(emitterData);
        editData.captured = true;

        std::string summary{ };
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
            else if (procClassInfo->m_ClassId == fb::SpawnColorRandomData::ClassId())
            {
                editData.spawnColorProcessor = static_cast<fb::SpawnColorRandomData*>(proc);
                editData.originalSpawnColor.captureFrom(editData.spawnColorProcessor);
            }

            const char* cname = (procClassInfo->m_InfoData && procClassInfo->m_InfoData->m_Name)
                ? procClassInfo->m_InfoData->m_Name : nullptr;
            if (cname)
            {
                std::string f = cname;
                if (f.size() > 4 && f.compare(f.size() - 4, 4, "Data") == 0)
                    f.erase(f.size() - 4);
                if (f.rfind("UpdateShaderParam", 0) != 0) // shaderparams aren't meaningful
                {
                    if (!summary.empty()) summary += ", ";
                    summary += f;
                }
            }
        }
        editData.typeSummary = summary;
        editData.origProcBytes = captureProcOriginals(emitterData);

        emitterMap[emitterData] = editData;
        InsertIntoTree(emitterData, editData.name);
    }

#if defined(BFVE_GAME_BF4)
    static void BF4_processEmitterAsset(fb::EmitterAsset* asset, const std::string& parentPath, int& emCounter);

    static void BF4_processEffectEntity(fb::EffectEntityData* effect, const std::string& parentPath, int& emCounter)
    {
        if (!effect)
            return;

        for (auto comp : effect->m_Components)
        {
            if (!comp)
                continue;

            fb::TypeInfo* ti = comp->GetType();
            if (!ti || ti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                continue;

            fb::ClassInfo* ci = static_cast<fb::ClassInfo*>(ti);

            if (ci->isSubclassOf((fb::ClassInfo*)fb::EmitterEntityData::ClassInfoPtr()))
            {
                fb::EmitterEntityData* emEntity = reinterpret_cast<fb::EmitterEntityData*>(comp);
                BF4_processEmitterAsset(emEntity->m_Emitter, parentPath, emCounter);
            }
            else if (ci->isSubclassOf((fb::ClassInfo*)fb::EffectEntityData::ClassInfoPtr()))
            {
                BF4_processEffectEntity(reinterpret_cast<fb::EffectEntityData*>(comp), parentPath, emCounter);
            }
        }
    }

    static void BF4_processEmitterAsset(fb::EmitterAsset* asset, const std::string& parentPath, int& emCounter)
    {
        if (!asset)
            return;

        fb::TypeInfo* ti = asset->GetType();
        if (!ti || ti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
            return;
        const uint32_t classId = static_cast<fb::ClassInfo*>(ti)->m_ClassId;

        const bool alreadyPlaced = g_placedDocs.count(asset) != 0;
        std::string leafName = asset->tryGetDebugName();
        if (leafName.empty())
            leafName = "emitter " + std::to_string(alreadyPlaced ? 0 : emCounter++);

        std::string emitterPath = parentPath;
        if (!emitterPath.empty()) emitterPath += '/';
        emitterPath += leafName;

        const bool firstVisit = g_placedDocs.insert(asset).second;

        std::vector<fb::EmitterTemplateData*> templates;

        if (classId == fb::ScalableEmitterDocument::ClassId())
        {
            auto* doc = reinterpret_cast<fb::ScalableEmitterDocument*>(asset);
            struct Q { fb::EmitterTemplateData* td; const char* tag; };
            const Q qs[] = {
                { doc->m_TemplateDataUltra,  "Ultra"  },
                { doc->m_TemplateDataHigh,   "High"   },
                { doc->m_TemplateDataMedium, "Medium" },
                { doc->m_TemplateDataLow,    "Low"    },
            };
            for (const auto& q : qs)
            {
                if (!q.td) continue;
                RegisterEmitter(q.td, emitterPath + " [" + q.tag + "]", asset);
                templates.push_back(q.td);
            }
        }
        else if (classId == fb::FlatEmitterDocument::ClassId())
        {
            auto* doc = reinterpret_cast<fb::FlatEmitterDocument*>(asset);
            if (doc->m_TemplateData)
            {
                RegisterEmitter(doc->m_TemplateData, emitterPath, asset);
                templates.push_back(doc->m_TemplateData);
            }
        }

        // sub emitters
        if (firstVisit)
        {
            for (fb::EmitterTemplateData* td : templates)
            {
                for (fb::ProcessorData* proc = td->m_RootProcessor; proc; proc = proc->m_NextProcessor)
                {
                    fb::TypeInfo* pti = proc->GetType();
                    if (!pti || pti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                        continue;
                    if (static_cast<fb::ClassInfo*>(pti)->m_ClassId != fb::EmitterData::ClassId())
                        continue;
                    for (fb::EmitterDocument* child : static_cast<fb::EmitterData*>(proc)->m_EmitterAssets)
                        if (child)
                            BF4_processEmitterAsset(child, parentPath, emCounter);
                }
            }
        }
    }
#endif // BFVE_GAME_BF4

    void scan()
    {
        emitterMap.clear();
        emitterTree.Clear();
        nameUseCounts.clear();
        nameToDocs.clear();
#if defined(BFVE_GAME_BF4)
        g_placedDocs.clear();
#endif

        fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
        if (!rm)
        {
            logger::info("[emitters::scan] no ResourceManager");
            return;
        }

        [[maybe_unused]] size_t bpSeen = 0;
        [[maybe_unused]] size_t bpSkipNullObject = 0;
        [[maybe_unused]] size_t bpSkipNoInnerType = 0;
        [[maybe_unused]] size_t bpSkipNotEffectEntity = 0;

#if defined(BFVE_GAME_BF4)
        std::unordered_map<std::string, size_t> classHistogram;
        size_t totalCompartments = 0;
        size_t totalObjects = 0;
        std::vector<fb::EmitterAsset*> pendingDocs;
#endif

        for (const auto& comp : rm->m_compartments)
        {
            if (!comp)
                continue;

#if defined(BFVE_GAME_BF4)
            ++totalCompartments;
#endif

            for (const auto& obj : comp->m_objects)
            {
                if (!obj) continue;

                fb::TypeInfo* typeInfo = obj->GetType();
                if (!typeInfo || typeInfo->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                    continue;

                fb::ClassInfo* classInfo = static_cast<fb::ClassInfo*>(typeInfo);
                const uint32_t classId = classInfo->m_ClassId;
#if defined(BFVE_GAME_BF4)
                ++totalObjects;
                const char* clsName = (classInfo->m_InfoData && classInfo->m_InfoData->m_Name)
                    ? classInfo->m_InfoData->m_Name : "?";
                ++classHistogram[clsName];
#endif

#if defined(BFVE_GAME_BF3)
                if (classId == fb::EmitterTemplateData::ClassId())
                {
                    fb::EmitterTemplateData* emitterData = reinterpret_cast<fb::EmitterTemplateData*>(obj);
                    const std::string name = emitterData->m_Name ? emitterData->m_Name : "Unknown";
                    RegisterEmitter(emitterData, name); // InsertIntoTree happens inside
                }
#else
                if (classInfo->isSubclassOf((fb::ClassInfo*)fb::EffectBlueprint::ClassInfoPtr()))
                {
                    bpSeen++;
                    fb::EffectBlueprint* bp = reinterpret_cast<fb::EffectBlueprint*>(obj);
                    if (!bp->m_Object)
                    {
                        bpSkipNullObject++;
                        continue;
                    }

                    fb::TypeInfo* innerType = bp->m_Object->GetType();
                    if (!innerType || innerType->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                    {
                        bpSkipNoInnerType++;
                        continue;
                    }
                    fb::ClassInfo* innerCi = static_cast<fb::ClassInfo*>(innerType);
                    if (!innerCi->isSubclassOf((fb::ClassInfo*)fb::EffectEntityData::ClassInfoPtr()))
                    {
                        bpSkipNotEffectEntity++;
                        continue;
                    }

                    fb::EffectEntityData* effect = reinterpret_cast<fb::EffectEntityData*>(bp->m_Object);
                    int emCounter = 0;
                    BF4_processEffectEntity(effect, normalizePath(bp->m_Name), emCounter);
                }
                else if (classId == fb::ScalableEmitterDocument::ClassId()
                      || classId == fb::FlatEmitterDocument::ClassId())
                {
                    pendingDocs.push_back(reinterpret_cast<fb::EmitterAsset*>(obj));
                }
#endif
            }
        }

#if defined(BFVE_GAME_BF4)
        // Loose docs not referenced by any effect go under one folder. Anonymous
        // ones (no debug name) are skipped as noise.
        int orphanCounter = 0;
        for (fb::EmitterAsset* asset : pendingDocs)
        {
            if (!asset || asset->tryGetDebugName().empty())
                continue;
            if (g_placedDocs.count(asset))
                continue; // already listed under the effect that uses it
            BF4_processEmitterAsset(asset, "(unreferenced documents)", orphanCounter);
        }
#endif

        applyPendingEdits();

        scanned = true;
#if defined(BFVE_GAME_BF4)
        logger::info("Found {} emitters (compartments={} objects={} EffectBlueprints seen={} skip-null={} skip-notype={} skip-notEffect={})",
            emitterMap.size(), totalCompartments, totalObjects,
            bpSeen, bpSkipNullObject, bpSkipNoInnerType, bpSkipNotEffectEntity);

        std::vector<std::pair<std::string, size_t>> sorted(classHistogram.begin(), classHistogram.end());
        std::sort(sorted.begin(), sorted.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
        const size_t topN = std::min<size_t>(30, sorted.size());
        for (size_t i = 0; i < topN; ++i)
            logger::info("  [scan-class] {}: {}", sorted[i].first, sorted[i].second);

        size_t repeatedNames = 0;
        for (const auto& [name, docs] : nameToDocs)
        {
            if (docs.size() < 2)
                continue;

            ++repeatedNames;
            bool allSameName = true;
            const std::string firstDocName = docs.front() ? docs.front()->tryGetDebugName() : std::string{ };
            for (fb::EmitterAsset* d : docs)
            {
                if (!d || d->tryGetDebugName() != firstDocName)
                    allSameName = false;
            }

            logger::info("  [repeat] '{}' x{} - docName='{}' sameAssetName={}",
                name, docs.size(), firstDocName, allSameName ? "yes" : "no");
            for (fb::EmitterAsset* d : docs)
                logger::info("      doc={} name='{}'",
                    static_cast<void*>(d), d ? d->tryGetDebugName() : std::string{});
        }
        logger::info("Repeated names: {}", repeatedNames);
#else
        logger::info("Found {} emitters", emitterMap.size());
#endif
    }

#if defined(BFVE_GAME_BF4)
    void onEmitterEntityCreatedBF4(fb::EmitterEntityData* data, void* entity)
    {
        if (entity)
        {
            g_emitterEntities.insert(entity);
            if (data)
                g_emitterData[entity] = data;
        }

        if (!data || !data->m_Emitter)
            return;

        std::string foundName;
        auto stamp = [&](fb::EmitterTemplateData* td)
            {
                if (!td) return;
                auto it = emitterMap.find(td);
                if (it != emitterMap.end())
                {
                    it->second.lastTemplate = reinterpret_cast<fb::EmitterTemplate*>(entity);
                    if (foundName.empty())
                        foundName = it->second.name;
                }
            };

        auto* asset = data->m_Emitter;
        fb::TypeInfo* ti = asset->GetType();
        if (!ti || ti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
            return;

        const uint32_t classId = static_cast<fb::ClassInfo*>(ti)->m_ClassId;

        if (classId == fb::ScalableEmitterDocument::ClassId())
        {
            auto* doc = reinterpret_cast<fb::ScalableEmitterDocument*>(asset);
            stamp(doc->m_TemplateDataUltra);
            stamp(doc->m_TemplateDataHigh);
            stamp(doc->m_TemplateDataMedium);
            stamp(doc->m_TemplateDataLow);
        }
        else if (classId == fb::FlatEmitterDocument::ClassId())
        {
            auto* doc = reinterpret_cast<fb::FlatEmitterDocument*>(asset);
            stamp(doc->m_TemplateData);
        }

        if (entity)
        {
            if (foundName.empty())
                foundName = asset->tryGetDebugName();
            if (!foundName.empty())
                g_emitterNames[entity] = foundName;
        }
    }

    static std::string resolveEmitterName(fb::EmitterEntityData* data)
    {
        if (!data || !data->m_Emitter)
            return {};

        auto* asset = data->m_Emitter;
        fb::TypeInfo* ti = asset->GetType();
        if (!ti || ti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
            return {};

        std::string foundName;
        auto pick = [&](fb::EmitterTemplateData* td)
            {
                if (!td || !foundName.empty()) return;
                auto it = emitterMap.find(td);
                if (it != emitterMap.end())
                    foundName = it->second.name;
            };

        const uint32_t classId = static_cast<fb::ClassInfo*>(ti)->m_ClassId;
        if (classId == fb::ScalableEmitterDocument::ClassId())
        {
            auto* doc = reinterpret_cast<fb::ScalableEmitterDocument*>(asset);
            pick(doc->m_TemplateDataUltra);
            pick(doc->m_TemplateDataHigh);
            pick(doc->m_TemplateDataMedium);
            pick(doc->m_TemplateDataLow);
        }
        else if (classId == fb::FlatEmitterDocument::ClassId())
        {
            auto* doc = reinterpret_cast<fb::FlatEmitterDocument*>(asset);
            pick(doc->m_TemplateData);
        }

        if (foundName.empty())
            foundName = asset->tryGetDebugName();

        return foundName;
    }

#endif

    void onCreated(fb::EmitterTemplate* emitter, fb::EmitterTemplateData* data)
    {
        if (!emitter || !data)
        {
            return;
        }

        auto it = emitterMap.find(data);
        if (it == emitterMap.end())
        {
            return;
        }

        it->second.lastTemplate = emitter;
    }

    void captureCurrentState(const EmitterEditData& edit, EmitterSnapshot& outTemplate, EmitterColorSnapshot& outColor, EmitterSpawnColorSnapshot& outSpawnColor, EmitterProcSnapshot& outProc)
    {
        outTemplate.captureFrom(edit.data);

        outColor.exists = false;
        if (edit.colorProcessor)
        {
            outColor.captureFrom(edit.colorProcessor);
        }

        outSpawnColor.exists = false;
        if (edit.spawnColorProcessor)
        {
            outSpawnColor.captureFrom(edit.spawnColorProcessor);
        }

        outProc.captureFrom(edit.data);
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
                if (edit.key == it->key)
                {
                    it->templateData.restoreTo(edit.data);

                    if (it->hasColorData && edit.colorProcessor)
                        it->colorData.restoreTo(edit.colorProcessor);

                    if (it->hasSpawnColorData && edit.spawnColorProcessor)
                        it->spawnColorData.restoreTo(edit.spawnColorProcessor);

                    if (it->hasProcData)
                        it->procData.restoreTo(edit.data);

                    edit.modified = true;
                    logger::info("ApplyPendingEmitterEdits: Applied '{}'", it->key.c_str());
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
    if (!d)
        return;

    maxCount = d->m_MaxCount;
    lifetimeFrameCount = d->m_LifetimeFrameCount;
    timeScale = d->m_TimeScale;
    lifetime = d->m_Lifetime;
    emittableType = d->m_EmittableType;
    distanceScaleNearValue = d->m_DistanceScaleNearValue;
    distanceScaleFarValue = d->m_DistanceScaleFarValue;
    distanceScaleLength = d->m_DistanceScaleLength;
    vertexPixelLightingBlendFactor = d->m_VertexPixelLightingBlendFactor;
    globalLocalNormalBlendFactor = d->m_GlobalLocalNormalBlendFactor;
    softParticlesFadeDistanceMultiplier = d->m_SoftParticlesFadeDistanceMultiplier;
    lightWrapAroundFactor = d->m_LightWrapAroundFactor;
    lightMultiplier = d->m_LightMultiplier;
    meshCullingDistance = d->m_MeshCullingDistance;
    maxSpawnDistance = d->m_MaxSpawnDistance;
    minScreenArea = d->m_MinScreenArea;
    particleCullingFactor = d->m_ParticleCullingFactor;
    followSpawnSource = d->m_FollowSpawnSource;
    repeatParticleSpawning = d->m_RepeatParticleSpawning;
    emissive = d->m_Emissive;
    exclusionVolumeCullEnable = d->m_ExclusionVolumeCullEnable;
    transparencySunShadowEnable = d->m_TransparencySunShadowEnable;
    forceFullRes = d->m_ForceFullRes;
    localSpace = d->m_LocalSpace;
    opaque = d->m_Opaque;
    killParticlesWithEmitter = d->m_KillParticlesWithEmitter;
    forceNiceSorting = d->m_ForceNiceSorting;

#if defined(BFVE_GAME_BF3)
    pointLightIntensity = d->m_PointLightIntensity;
    pointLightPivot = d->m_PointLightPivot;
    pointLightColor = d->m_PointLightColor;
    pointLightRadius = d->m_PointLightRadius;
    pointLightRandomIntensityMin = d->m_PointLightRandomIntensityMin;
    pointLightRandomIntensityMax = d->m_PointLightRandomIntensityMax;
    pointLightMaxClamp = d->m_PointLightMaxClamp;
    pointLightMinClamp = d->m_PointLightMinClamp;
    visibleAfterDistance = d->m_VisibleAfterDistance;
    actAsPointLight = d->m_ActAsPointLight;
#endif
}

void EmitterSnapshot::restoreTo(fb::EmitterTemplateData* d) const
{
    if (!d) return;

    d->m_MaxCount = maxCount;
    d->m_LifetimeFrameCount = lifetimeFrameCount;
    d->m_TimeScale = timeScale;
    d->m_Lifetime = lifetime;
    d->m_EmittableType = emittableType;
    d->m_DistanceScaleNearValue = distanceScaleNearValue;
    d->m_DistanceScaleFarValue = distanceScaleFarValue;
    d->m_DistanceScaleLength = distanceScaleLength;
    d->m_VertexPixelLightingBlendFactor = vertexPixelLightingBlendFactor;
    d->m_GlobalLocalNormalBlendFactor = globalLocalNormalBlendFactor;
    d->m_SoftParticlesFadeDistanceMultiplier = softParticlesFadeDistanceMultiplier;
    d->m_LightWrapAroundFactor = lightWrapAroundFactor;
    d->m_LightMultiplier = lightMultiplier;
    d->m_MeshCullingDistance = meshCullingDistance;
    d->m_MaxSpawnDistance = maxSpawnDistance;
    d->m_MinScreenArea = minScreenArea;
    d->m_ParticleCullingFactor = particleCullingFactor;
    d->m_FollowSpawnSource = followSpawnSource;
    d->m_RepeatParticleSpawning = repeatParticleSpawning;
    d->m_Emissive = emissive;
    d->m_ExclusionVolumeCullEnable = exclusionVolumeCullEnable;
    d->m_TransparencySunShadowEnable = transparencySunShadowEnable;
    d->m_ForceFullRes = forceFullRes;
    d->m_LocalSpace = localSpace;
    d->m_Opaque = opaque;
    d->m_KillParticlesWithEmitter = killParticlesWithEmitter;
    d->m_ForceNiceSorting = forceNiceSorting;

#if defined(BFVE_GAME_BF3)
    d->m_PointLightIntensity = pointLightIntensity;
    d->m_PointLightPivot = pointLightPivot;
    d->m_PointLightColor = pointLightColor;
    d->m_PointLightRadius = pointLightRadius;
    d->m_PointLightRandomIntensityMin = pointLightRandomIntensityMin;
    d->m_PointLightRandomIntensityMax = pointLightRandomIntensityMax;
    d->m_PointLightMaxClamp = pointLightMaxClamp;
    d->m_PointLightMinClamp = pointLightMinClamp;
    d->m_VisibleAfterDistance = visibleAfterDistance;
    d->m_ActAsPointLight = actAsPointLight;
#endif
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

void EmitterSpawnColorSnapshot::captureFrom(fb::SpawnColorRandomData* proc)
{
    if (!proc)
    {
        exists = false;
        return;
    }

    exists = true;
    color0 = proc->m_Color0;
    color1 = proc->m_Color1;
}

void EmitterSpawnColorSnapshot::restoreTo(fb::SpawnColorRandomData* proc) const
{
    if (!exists || !proc)
        return;

    proc->m_Color0 = color0;
    proc->m_Color1 = color1;
}

#if defined(BFVE_GAME_BF4)
namespace
{
    // typeinfo dump
    constexpr size_t kProcValueStart = 0x20;
    constexpr size_t kEvalValueStart = 0x18;

    uint32_t classIdOf(fb::DataContainer* o)
    {
        fb::TypeInfo* ti = o ? o->GetType() : nullptr;
        if (!ti || ti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class) return 0;
        return static_cast<fb::ClassInfo*>(ti)->m_ClassId;
    }
    size_t totalSizeOf(fb::DataContainer* o)
    {
        fb::TypeInfo* ti = o ? o->GetType() : nullptr;
        auto* tid = ti ? ti->GetTypeInfoData() : nullptr;
        return tid ? tid->m_TotalSize : 0;
    }

    bool procHasPointers(uint32_t cid)
    {
        return cid == fb::EmitterData::ClassId() || cid == fb::BaseEmitterData::ClassId()
            || cid == fb::UpdateTextureCoordsData::ClassId() || cid == fb::UpdateClipScaleData::ClassId();
    }
    bool evalHasPointers(uint32_t cid)
    {
        return cid == fb::SampleTextureData::ClassId();
    }
}
#endif

namespace editor::emitters
{
    std::unordered_map<void*, std::vector<uint8_t>> captureProcOriginals(fb::EmitterTemplateData* d)
    {
        std::unordered_map<void*, std::vector<uint8_t>> m;
#if defined(BFVE_GAME_BF4)
        if (!d) return m;
        for (fb::ProcessorData* proc = d->m_RootProcessor; proc; proc = proc->m_NextProcessor)
        {
            const size_t sz = totalSizeOf(proc);
            if (sz)
            {
                auto* b = reinterpret_cast<const uint8_t*>(proc);
                m[proc].assign(b, b + sz);
            }
            if (proc->m_Pre)
            {
                const size_t psz = totalSizeOf(proc->m_Pre);
                if (psz)
                {
                    auto* pb = reinterpret_cast<const uint8_t*>(proc->m_Pre);
                    m[proc->m_Pre].assign(pb, pb + psz);
                }
            }
        }
#else
        (void)d;
#endif
        return m;
    }

    void resetProcessorEdits(EmitterEditData& edit)
    {
#if defined(BFVE_GAME_BF4)
        fb::EmitterTemplateData* d = edit.data;
        if (!d) return;
        for (fb::ProcessorData* proc = d->m_RootProcessor; proc; proc = proc->m_NextProcessor)
        {
            auto it = edit.origProcBytes.find(proc);
            if (it != edit.origProcBytes.end() && !procHasPointers(classIdOf(proc)))
            {
                const size_t sz = totalSizeOf(proc);
                if (sz >= kProcValueStart && it->second.size() == sz)
                    std::memcpy(reinterpret_cast<uint8_t*>(proc) + kProcValueStart,
                        it->second.data() + kProcValueStart, sz - kProcValueStart);
            }
            if (proc->m_Pre)
            {
                auto pit = edit.origProcBytes.find(proc->m_Pre);
                if (pit != edit.origProcBytes.end() && !evalHasPointers(classIdOf(proc->m_Pre)))
                {
                    const size_t psz = totalSizeOf(proc->m_Pre);
                    if (psz >= kEvalValueStart && pit->second.size() == psz)
                        std::memcpy(reinterpret_cast<uint8_t*>(proc->m_Pre) + kEvalValueStart,
                            pit->second.data() + kEvalValueStart, psz - kEvalValueStart);
                }
            }
        }
#else
        (void)edit;
#endif
    }
}

void EmitterProcSnapshot::captureFrom(fb::EmitterTemplateData* d)
{
    exists = false;
    nodes.clear();
#if defined(BFVE_GAME_BF4)
    if (!d)
        return;

    for (fb::ProcessorData* proc = d->m_RootProcessor; proc; proc = proc->m_NextProcessor)
    {
        EmitterProcNode n;
        n.classId = classIdOf(proc);
        if (n.classId && !procHasPointers(n.classId))
        {
            const size_t sz = totalSizeOf(proc);
            if (sz > kProcValueStart)
            {
                auto* base = reinterpret_cast<const uint8_t*>(proc);
                n.procBytes.assign(base + kProcValueStart, base + sz);
            }
        }
        if (proc->m_Pre)
        {
            const uint32_t pcid = classIdOf(proc->m_Pre);
            if (pcid && !evalHasPointers(pcid))
            {
                const size_t psz = totalSizeOf(proc->m_Pre);
                if (psz > kEvalValueStart)
                {
                    auto* pbase = reinterpret_cast<const uint8_t*>(proc->m_Pre);
                    n.hasPre = true;
                    n.preClassId = pcid;
                    n.preBytes.assign(pbase + kEvalValueStart, pbase + psz);
                }
            }
        }
        nodes.push_back(std::move(n));
    }
    exists = !nodes.empty();
#else
    (void)d;
#endif
}

void EmitterProcSnapshot::restoreTo(fb::EmitterTemplateData* d) const
{
#if defined(BFVE_GAME_BF4)
    if (!exists || !d) return;
    size_t i = 0;
    for (fb::ProcessorData* proc = d->m_RootProcessor; proc && i < nodes.size(); proc = proc->m_NextProcessor, ++i)
    {
        const EmitterProcNode& n = nodes[i];
        if (!n.procBytes.empty() && classIdOf(proc) == n.classId)
        {
            const size_t sz = totalSizeOf(proc);
            if (sz >= kProcValueStart && n.procBytes.size() == sz - kProcValueStart)
                std::memcpy(reinterpret_cast<uint8_t*>(proc) + kProcValueStart, n.procBytes.data(), n.procBytes.size());
        }
        if (n.hasPre && proc->m_Pre && !n.preBytes.empty() && classIdOf(proc->m_Pre) == n.preClassId)
        {
            const size_t psz = totalSizeOf(proc->m_Pre);
            if (psz >= kEvalValueStart && n.preBytes.size() == psz - kEvalValueStart)
                std::memcpy(reinterpret_cast<uint8_t*>(proc->m_Pre) + kEvalValueStart, n.preBytes.data(), n.preBytes.size());
        }
    }
#else
    (void)d;
#endif
}

static bool entityIsSubclassOf(void* e, fb::ClassInfo* base)
{
    fb::TypeInfo* ti = reinterpret_cast<fb::ITypedObject*>(e)->GetType();
    if (!ti || ti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
        return false;
    return static_cast<fb::ClassInfo*>(ti)->isSubclassOf(base);
}

static fb::EmitterEntityData* emitterData(void* emitterEntity)
{
#if defined(BFVE_GAME_BF4)
    return *reinterpret_cast<fb::EmitterEntityData**>(reinterpret_cast<char*>(emitterEntity) + 0x80);
#else
    return *reinterpret_cast<fb::EmitterEntityData**>(reinterpret_cast<char*>(emitterEntity) + 0x74);
#endif
}

#if defined(BFVE_GAME_BF4)
static bool readEmitterPos(void* e, fb::Vec3& out)
{
    /*movaps  xmm0, xmmword ptr[rcx + 40h]
        .text:0000000140A73464                 movaps  xmmword ptr[rdx], xmm0
        .text : 0000000140A73467                 movaps  xmm1, xmmword ptr[rcx + 50h]
        .text : 0000000140A7346B                 movaps  xmmword ptr[rdx + 10h], xmm1
        .text : 0000000140A7346F                 movaps  xmm0, xmmword ptr[rcx + 60h]
        .text : 0000000140A73473                 movaps  xmmword ptr[rdx + 20h], xmm0
        .text : 0000000140A73477                 movaps  xmm1, xmmword ptr[rcx + 70h]
        .text : 0000000140A7347B                 movaps  xmmword ptr[rdx + 30h], xmm1*/
    __try
    {
        if (!e || *reinterpret_cast<std::uintptr_t*>(e) != 0x141CF2EC0)
            return false;
        void* cur = e;

        // EffectEntity : public SpatialEntity
        void* parent = *reinterpret_cast<void**>(reinterpret_cast<char*>(cur) + 0x90);
        const auto pv = reinterpret_cast<std::uintptr_t>(parent);
        const std::uintptr_t pvt = *reinterpret_cast<std::uintptr_t*>(parent);
        if (pvt < 0x140000000 || pvt >= 0x143000000)
            return false;

        cur = parent;

        out = *reinterpret_cast<fb::Vec3*>(reinterpret_cast<char*>(cur) + 0x70);
        return true;
    }
    __except (1)
    {
        return false;
    }
}

static int bf4GatherChildren(void* e, void** out, int maxN)
{
    __try
    {
        int n = 0;
        for (void* c = *reinterpret_cast<void**>(reinterpret_cast<char*>(e) + 0xA0);
             c && n < maxN;
             c = *reinterpret_cast<void**>(reinterpret_cast<char*>(c) + 0x98))
        {
            out[n++] = c;
        }
        return n;
    }
    __except (1) { return -1; }
}

static bool drawEmitterMarkerBf4(void* emitter, fb::EmitterEntityData* data, ImDrawList* dl, ImFont* font, float fontSize)
{
    fb::Vec3 pos{ };
    if (!readEmitterPos(emitter, pos))
        return false;

    ImVec2 sp{ };
    float depth = 0.0f;
    if (!render::worldToScreen(pos, sp, depth))
        return true;

    if (depth > editor::emitters::overlayMaxDistance)
        return true;

    const ImColor col = render::Colors::Orange;

    float r = 700.0f / depth;
    if (r < 3.0f)  r = 3.0f;
    if (r > 24.0f) r = 24.0f;
    render::circle(sp, r, col);

    const std::string name = editor::emitters::resolveEmitterName(data ? data : emitterData(emitter));

    char buf[256];
    if (!name.empty())
        std::snprintf(buf, sizeof(buf), "%s  %.0fm", name.c_str(), depth);
    else
        std::snprintf(buf, sizeof(buf), "emitter  %.0fm", depth);

    const ImVec2 ts = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, buf);
    const ImVec2 tpos{ sp.x - ts.x * 0.5f, sp.y + r + 3.0f };
    dl->AddText(font, fontSize, ImVec2{ tpos.x + 1.0f, tpos.y + 1.0f }, IM_COL32(0, 0, 0, 200), buf);
    dl->AddText(font, fontSize, tpos, col, buf);
    return true;
}

void editor::emitters::renderOverlay()
{
    if (!showOverlay)
        return;

    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    ImFont* font = ImGui::GetFont();
    const float fontSize = ImGui::GetFontSize() * 1.2f;

    fb::ClassInfo* const emitterClass = reinterpret_cast<fb::ClassInfo*>(fb::EmitterEntity::ClassInfoPtr());

    std::unordered_set<void*> drawn;
    fb::EntityList<void> fx{ reinterpret_cast<void*>(fb::EffectEntity::ClassInfoPtr()), 0x28 };
    void* e;
    while ((e = fx.nextOfKind()) != nullptr)
    {
        void* kids[20];
        const int n = bf4GatherChildren(e, kids, sizeof(kids));
        for (int i = 0; i < n; ++i)
        {
            void* emitter = kids[i];
            if (!entityIsSubclassOf(emitter, emitterClass))
                continue;
            if (!drawn.insert(emitter).second)
                continue;
            drawEmitterMarkerBf4(emitter, emitterData(emitter), dl, font, fontSize);
        }
    }

    std::vector<void*> dead;
    for (void* emitter : g_emitterEntities)
    {
        if (drawn.count(emitter))
            continue;

        fb::EmitterEntityData* data = nullptr;
        if (auto it = g_emitterData.find(emitter); it != g_emitterData.end())
            data = it->second;

        if (!drawEmitterMarkerBf4(emitter, data, dl, font, fontSize))
            dead.push_back(emitter);
    }

    for (void* d : dead)
    {
        g_emitterEntities.erase(d);
        g_emitterData.erase(d);
        g_emitterNames.erase(d);
    }
}
#else // BFVE_GAME_BF3

using getWorldTransform_t = void(__thiscall*)(void* thisptr, fb::LinearTransform* out);
static const getWorldTransform_t getWorldTransform = reinterpret_cast<getWorldTransform_t>(0x0054B770);

static bool readEmitterWorldPos(void* e, fb::Vec3& out)
{
    __try
    {
        alignas(16) fb::LinearTransform xf{ };
        getWorldTransform(e, &xf);
        out = xf.m_trans;
        return true;
    }
    __except (1)
    {
        return false;
    }
}

// didnt bother to understand this layour, ghetto and works
static int sehGatherChildren(void* e, void** out, int maxN)
{
    __try
    {
        int n = 0;
        for (void* c = *reinterpret_cast<void**>(reinterpret_cast<char*>(e) + 0x60);
             c && n < maxN;
             c = *reinterpret_cast<void**>(reinterpret_cast<char*>(c) + 0x5C))
        {
            out[n++] = c;
        }
		//printf("Child count for %p: %d\n", e, n);

        return n;
    }
    __except (1) { return -1; }
}

static std::string resolveEmitterName(fb::EmitterEntityData* data)
{
    if (!data || !data->m_Emitter)
        return {};

    return data->m_Emitter->tryGetDebugName();
}

static bool drawEmitterMarker(void* emitter, fb::EmitterEntityData* data, ImDrawList* dl, ImFont* font, float fontSize)
{
    fb::Vec3 pos{ };
    if (!readEmitterWorldPos(emitter, pos))
        return false;

    ImVec2 sp{ };
    float depth = 0.0f;
    if (!render::worldToScreen(pos, sp, depth))
        return true;

    if (depth > editor::emitters::overlayMaxDistance)
        return true;

    const ImColor col = render::Colors::Orange;

    float r = 700.0f / depth;
    if (r < 3.0f)  r = 3.0f;
    if (r > 24.0f) r = 24.0f;
    render::circle(sp, r, col);

    const std::string name = resolveEmitterName(data ? data : emitterData(emitter));

    char buf[256];
    if (!name.empty())
        std::snprintf(buf, sizeof(buf), "%s  %.0fm", name.c_str(), depth);
    else
        std::snprintf(buf, sizeof(buf), "emitter  %.0fm", depth);

    const ImVec2 ts = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, buf);
    const ImVec2 tpos{ sp.x - ts.x * 0.5f, sp.y + r + 3.0f };
    dl->AddText(font, fontSize, ImVec2{ tpos.x + 1.0f, tpos.y + 1.0f }, IM_COL32(0, 0, 0, 200), buf);
    dl->AddText(font, fontSize, tpos, col, buf);
    return true;
}

void editor::emitters::onEmitterEntityCreated(fb::EmitterEntityData* data, void* entity)
{
    if (!entity)
        return;

    g_emitterEntities.insert(entity);
    if (data)
        g_emitterData[entity] = data;
}

void editor::emitters::renderOverlay()
{
    if (!showOverlay)
        return;

    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    ImFont* font = ImGui::GetFont();
    const float fontSize = ImGui::GetFontSize() * 1.2f;

    fb::ClassInfo* const emitterClass = reinterpret_cast<fb::ClassInfo*>(fb::ClientEmitterEntity::ClassInfoPtr());

    std::unordered_set<void*> drawn;
    fb::EntityList<void> fx{ reinterpret_cast<fb::ClassInfo*>(fb::EffectEntity::ClassInfoPtr()) };
    void* e;
    while ((e = fx.nextOfKind()) != nullptr)
    {
        void* kids[20];
        const int n = sehGatherChildren(e, kids, sizeof(kids));
        for (int i = 0; i < n; ++i)
        {
            void* emitter = kids[i];
            if (!entityIsSubclassOf(emitter, emitterClass))
                continue;
            if (!drawn.insert(emitter).second)
                continue;
            drawEmitterMarker(emitter, emitterData(emitter), dl, font, fontSize);
        }
    }

    std::vector<void*> dead;
    for (void* emitter : g_emitterEntities)
    {
        if (drawn.count(emitter))
            continue;

        fb::EmitterEntityData* data = nullptr;
        if (auto it = g_emitterData.find(emitter); it != g_emitterData.end())
            data = it->second;

        if (!drawEmitterMarker(emitter, data, dl, font, fontSize))
            dead.push_back(emitter);
    }

    for (void* d : dead)
    {
        g_emitterEntities.erase(d);
        g_emitterData.erase(d);
    }
}

#endif