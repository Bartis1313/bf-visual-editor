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

    static std::unordered_map<std::string, int> nameUseCounts;
    static std::unordered_map<std::string, int> keyUseCounts; // fallback

    static void RegisterEmitter(fb::EmitterTemplateData* emitterData, const std::string& displayName, const std::string& stableKey)
    {
        if (!emitterData) return;
        if (emitterMap.count(emitterData)) return;

        std::string baseName = displayName.empty() ? std::string{"Unknown"} : displayName;
        int& useCount = nameUseCounts[baseName];
        std::string finalName = (useCount == 0)
            ? baseName
            : baseName + " (" + std::to_string(useCount) + ")";
        ++useCount;

        std::string finalKey = stableKey.empty() ? finalName : stableKey;
        if (int& keyCount = keyUseCounts[finalKey]; keyCount++ > 0)
        {
            logger::warning("[emitters] duplicate emitter key '{}' (#{}), disambiguating", finalKey.c_str(), keyCount - 1);
            finalKey += " #" + std::to_string(keyCount - 1);
        }

        EmitterEditData editData;
        editData.data = emitterData;
        editData.name = finalName;
        editData.key = finalKey;
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

#if defined(BFVE_GAME_BF4)
    static void BF4_processEmitterAsset(fb::EmitterAsset* asset, const std::string& parentPath);

    static void BF4_processEffectEntity(fb::EffectEntityData* effect, const std::string& parentPath)
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
                BF4_processEmitterAsset(emEntity->m_Emitter, parentPath);
            }
            else if (ci->isSubclassOf((fb::ClassInfo*)fb::EffectEntityData::ClassInfoPtr()))
            {
                BF4_processEffectEntity(reinterpret_cast<fb::EffectEntityData*>(comp), parentPath);
            }
        }
    }

    static void BF4_processEmitterAsset(fb::EmitterAsset* asset, const std::string& parentPath)
    {
        if (!asset)
            return;

        std::string emitterPath = parentPath;
        std::string assetName = asset->tryGetDebugName();
        if (!assetName.empty())
        {
            if (!emitterPath.empty()) emitterPath += '/';
            emitterPath += assetName;
        }

        fb::TypeInfo* ti = asset->GetType();
        if (!ti || ti->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
            return;

        fb::ClassInfo* ci = static_cast<fb::ClassInfo*>(ti);
        const uint32_t classId = ci->m_ClassId;

        std::string docPath = asset->m_Name ? std::string{asset->m_Name} : std::string{ };

        if (classId == fb::ScalableEmitterDocument::ClassId())
        {
            fb::ScalableEmitterDocument* doc = reinterpret_cast<fb::ScalableEmitterDocument*>(asset);
            auto regWithTag = [&](fb::EmitterTemplateData* td, const char* tag)
            {
                if (!td) return;
                if (emitterMap.count(td)) return;
                std::string name = emitterPath;
                std::string key = docPath;
                if (tag && *tag)
                {
                    name += " ["; name += tag; name += "]";
                    key  += " ["; key  += tag; key  += "]";
                }
                RegisterEmitter(td, name, key);
            };
            regWithTag(doc->m_TemplateDataUltra, "Ultra");
            regWithTag(doc->m_TemplateDataHigh, "High");
            regWithTag(doc->m_TemplateDataMedium, "Medium");
            regWithTag(doc->m_TemplateDataLow, "Low");
        }
        else if (classId == fb::FlatEmitterDocument::ClassId())
        {
            auto* doc = reinterpret_cast<fb::FlatEmitterDocument*>(asset);
            RegisterEmitter(doc->m_TemplateData, emitterPath, docPath);
        }
    }
#endif // BFVE_GAME_BF4

    void scan()
    {
        emitterMap.clear();
        emitterTree.Clear();
        nameUseCounts.clear();
        keyUseCounts.clear();

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
                    RegisterEmitter(emitterData, name, name);
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
                    BF4_processEffectEntity(effect, bp->tryGetDebugName());
                }
#endif
            }
        }

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
#else
        logger::info("Found {} emitters", emitterMap.size());
#endif
    }

#if defined(BFVE_GAME_BF4)
    void onEmitterEntityCreatedBF4(fb::EmitterEntityData* data, void* entity)
    {
        if (!data || !data->m_Emitter)
            return;

        auto stamp = [&](fb::EmitterTemplateData* td)
        {
            if (!td) return;
            auto it = emitterMap.find(td);
            if (it != emitterMap.end())
                it->second.lastTemplate = reinterpret_cast<fb::EmitterTemplate*>(entity);
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
                if (edit.key == it->key)
                {
                    it->templateData.restoreTo(edit.data);

                    if (it->hasColorData && edit.colorProcessor)
                        it->colorData.restoreTo(edit.colorProcessor);

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

    maxCount                            = d->m_MaxCount;
    lifetimeFrameCount                  = d->m_LifetimeFrameCount;
    timeScale                           = d->m_TimeScale;
    lifetime                            = d->m_Lifetime;
    emittableType                       = d->m_EmittableType;
    distanceScaleNearValue              = d->m_DistanceScaleNearValue;
    distanceScaleFarValue               = d->m_DistanceScaleFarValue;
    distanceScaleLength                 = d->m_DistanceScaleLength;
    vertexPixelLightingBlendFactor      = d->m_VertexPixelLightingBlendFactor;
    globalLocalNormalBlendFactor        = d->m_GlobalLocalNormalBlendFactor;
    softParticlesFadeDistanceMultiplier = d->m_SoftParticlesFadeDistanceMultiplier;
    lightWrapAroundFactor               = d->m_LightWrapAroundFactor;
    lightMultiplier                     = d->m_LightMultiplier;
    meshCullingDistance                 = d->m_MeshCullingDistance;
    maxSpawnDistance                    = d->m_MaxSpawnDistance;
    minScreenArea                       = d->m_MinScreenArea;
    particleCullingFactor               = d->m_ParticleCullingFactor;
    followSpawnSource                   = d->m_FollowSpawnSource;
    repeatParticleSpawning              = d->m_RepeatParticleSpawning;
    emissive                            = d->m_Emissive;
    exclusionVolumeCullEnable           = d->m_ExclusionVolumeCullEnable;
    transparencySunShadowEnable         = d->m_TransparencySunShadowEnable;
    forceFullRes                        = d->m_ForceFullRes;
    localSpace                          = d->m_LocalSpace;
    opaque                              = d->m_Opaque;
    killParticlesWithEmitter            = d->m_KillParticlesWithEmitter;
    forceNiceSorting                    = d->m_ForceNiceSorting;

#if defined(BFVE_GAME_BF3)
    pointLightIntensity          = d->m_PointLightIntensity;
    pointLightPivot              = d->m_PointLightPivot;
    pointLightColor              = d->m_PointLightColor;
    pointLightRadius             = d->m_PointLightRadius;
    pointLightRandomIntensityMin = d->m_PointLightRandomIntensityMin;
    pointLightRandomIntensityMax = d->m_PointLightRandomIntensityMax;
    pointLightMaxClamp           = d->m_PointLightMaxClamp;
    pointLightMinClamp           = d->m_PointLightMinClamp;
    visibleAfterDistance         = d->m_VisibleAfterDistance;
    actAsPointLight              = d->m_ActAsPointLight;
#endif
}

void EmitterSnapshot::restoreTo(fb::EmitterTemplateData* d) const
{
    if (!d) return;

    d->m_MaxCount                            = maxCount;
    d->m_LifetimeFrameCount                  = lifetimeFrameCount;
    d->m_TimeScale                           = timeScale;
    d->m_Lifetime                            = lifetime;
    d->m_EmittableType                       = emittableType;
    d->m_DistanceScaleNearValue              = distanceScaleNearValue;
    d->m_DistanceScaleFarValue               = distanceScaleFarValue;
    d->m_DistanceScaleLength                 = distanceScaleLength;
    d->m_VertexPixelLightingBlendFactor      = vertexPixelLightingBlendFactor;
    d->m_GlobalLocalNormalBlendFactor        = globalLocalNormalBlendFactor;
    d->m_SoftParticlesFadeDistanceMultiplier = softParticlesFadeDistanceMultiplier;
    d->m_LightWrapAroundFactor               = lightWrapAroundFactor;
    d->m_LightMultiplier                     = lightMultiplier;
    d->m_MeshCullingDistance                 = meshCullingDistance;
    d->m_MaxSpawnDistance                    = maxSpawnDistance;
    d->m_MinScreenArea                       = minScreenArea;
    d->m_ParticleCullingFactor               = particleCullingFactor;
    d->m_FollowSpawnSource                   = followSpawnSource;
    d->m_RepeatParticleSpawning              = repeatParticleSpawning;
    d->m_Emissive                            = emissive;
    d->m_ExclusionVolumeCullEnable           = exclusionVolumeCullEnable;
    d->m_TransparencySunShadowEnable         = transparencySunShadowEnable;
    d->m_ForceFullRes                        = forceFullRes;
    d->m_LocalSpace                          = localSpace;
    d->m_Opaque                              = opaque;
    d->m_KillParticlesWithEmitter            = killParticlesWithEmitter;
    d->m_ForceNiceSorting                    = forceNiceSorting;

#if defined(BFVE_GAME_BF3)
    d->m_PointLightIntensity            = pointLightIntensity;
    d->m_PointLightPivot                = pointLightPivot;
    d->m_PointLightColor                = pointLightColor;
    d->m_PointLightRadius               = pointLightRadius;
    d->m_PointLightRandomIntensityMin   = pointLightRandomIntensityMin;
    d->m_PointLightRandomIntensityMax   = pointLightRandomIntensityMax;
    d->m_PointLightMaxClamp             = pointLightMaxClamp;
    d->m_PointLightMinClamp             = pointLightMinClamp;
    d->m_VisibleAfterDistance           = visibleAfterDistance;
    d->m_ActAsPointLight                = actAsPointLight;
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