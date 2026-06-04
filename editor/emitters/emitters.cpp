#include "emitters.h"
#include "../editor_context.h"
#include "../../utils/log.h"
#include "../render/render.h"
#include <sstream>
#include <imgui.h>
#include <cstdio>
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

    static void RegisterEmitter(fb::EmitterTemplateData* emitterData, const std::string& displayName)
    {
        if (!emitterData) return;
        if (emitterMap.count(emitterData)) return;

        std::string baseName = displayName.empty() ? std::string{ "Unknown" } : displayName;
        int& useCount = nameUseCounts[baseName];
        std::string finalName = (useCount == 0) ? baseName
            : baseName + " (" + std::to_string(useCount) + ")";
        ++useCount;

        EmitterEditData editData;
        editData.data = emitterData;
        editData.name = finalName;
        editData.key = finalName;
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

        if (classId == fb::ScalableEmitterDocument::ClassId())
        {
            fb::ScalableEmitterDocument* doc = reinterpret_cast<fb::ScalableEmitterDocument*>(asset);
            auto regWithTag = [&](fb::EmitterTemplateData* td, const char* tag)
                {
                    if (!td) return;
                    if (emitterMap.count(td)) return;
                    std::string name = emitterPath;
                    if (tag && *tag)
                    {
                        name += " ["; name += tag; name += "]";
                    }
                    RegisterEmitter(td, name);
                };
            regWithTag(doc->m_TemplateDataUltra, "Ultra");
            regWithTag(doc->m_TemplateDataHigh, "High");
            regWithTag(doc->m_TemplateDataMedium, "Medium");
            regWithTag(doc->m_TemplateDataLow, "Low");
        }
        else if (classId == fb::FlatEmitterDocument::ClassId())
        {
            auto* doc = reinterpret_cast<fb::FlatEmitterDocument*>(asset);
            RegisterEmitter(doc->m_TemplateData, emitterPath);
        }
    }
#endif // BFVE_GAME_BF4

    void scan()
    {
        emitterMap.clear();
        emitterTree.Clear();
        nameUseCounts.clear();

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
                    RegisterEmitter(emitterData, name);
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
                else if (classId == fb::ScalableEmitterDocument::ClassId()
                      || classId == fb::FlatEmitterDocument::ClassId())
                {
                    pendingDocs.push_back(reinterpret_cast<fb::EmitterAsset*>(obj));
                }
#endif
            }
        }

#if defined(BFVE_GAME_BF4)
        for (fb::EmitterAsset* asset : pendingDocs)
        {
            if (!asset || asset->tryGetDebugName().empty())
                continue;
            BF4_processEmitterAsset(asset, std::string{});
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

void editor::emitters::renderOverlay()
{
    if (!showOverlay)
        return;

    std::vector<void*> ents;
    ents.assign(g_emitterEntities.begin(), g_emitterEntities.end());

    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    ImFont* font = ImGui::GetFont();
    const float fontSize = ImGui::GetFontSize() * 1.2f;

    std::vector<void*> dead;
    for (void* e : ents)
    {
        fb::Vec3 pos{ };
        if (!readEmitterPos(e, pos))
        {
            dead.push_back(e);
            continue;
        }

        ImVec2 sp{ };
        float depth = 0.0f;
        if (!render::worldToScreen(pos, sp, depth))
            continue;
        if (depth > overlayMaxDistance)
            continue;

        const ImColor col = render::Colors::Orange;

        float r = 700.0f / depth;
        if (r < 3.0f)  r = 3.0f;
        if (r > 24.0f) r = 24.0f;
        render::circle(sp, r, col);

        auto nameIt = g_emitterNames.find(e);
        if (nameIt == g_emitterNames.end())
        {
            if (auto dataIt = g_emitterData.find(e); dataIt != g_emitterData.end())
            {
                std::string resolved = resolveEmitterName(dataIt->second);
                if (!resolved.empty())
                    nameIt = g_emitterNames.emplace(e, std::move(resolved)).first;
            }
        }

        char buf[256];
        if (nameIt != g_emitterNames.end())
            std::snprintf(buf, sizeof(buf), "%s  %.0fm", nameIt->second.c_str(), depth);
        else
            std::snprintf(buf, sizeof(buf), "emitter_%p  %.0fm", e, depth);

        const ImVec2 ts = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, buf);
        const ImVec2 tpos{ sp.x - ts.x * 0.5f, sp.y + r + 3.0f };
        dl->AddText(font, fontSize, ImVec2{ tpos.x + 1.0f, tpos.y + 1.0f }, IM_COL32(0, 0, 0, 200), buf);
        dl->AddText(font, fontSize, tpos, col, buf);
    }

    if (!dead.empty())
    {
        for (void* d : dead)
        {
            g_emitterEntities.erase(d);
            g_emitterData.erase(d);
            g_emitterNames.erase(d);
        }
    }
}
#else
void editor::emitters::renderOverlay() {}
#endif