#include "lights.h"
#include "lights_internal.h"
#include "../editor_context.h"

#include <Windows.h>

#include <cstring>
#include <mutex>
#include "../textures/textures.h"
#include "../emitters/emitters.h"
#include "../textures/surfacepick.h"
#include "../../utils/log.h"
#include "../render/render.h"

#include "../../hooks/functions.h" // g_pDevice / g_pContext
#include <d3d11.h>

#include <imgui.h>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <vector>
#include <format>
#include <algorithm>
#include <unordered_set>

namespace editor::lights
{
    using namespace detail;

    static std::unordered_map<fb::LocalLightEntityData*, LightDataEntry> entries;
    static bool scanned = false;

    static void linkLampContents(fb::LocalLightEntityData* data, LightDataEntry& entry);
    static void clearLampLinks();
    static void clearLevelPointers();
    static void clearOverlayState();

    static fb::Array<fb::GameObjectData*>* componentsOf(fb::ClassInfo* classInfo, void* data)
    {
        if (!classInfo || !data)
            return nullptr;
#if defined(BFVE_GAME_BF4)
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::ComponentEntityData::ClassInfoPtr()))
            return &static_cast<fb::ComponentEntityData*>(data)->m_Components;
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::ComponentData::ClassInfoPtr()))
            return &static_cast<fb::ComponentData*>(data)->m_Components;
#else
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::ComponentData::ClassInfoPtr()))
            return &static_cast<fb::ComponentData*>(data)->m_Components;
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::GameEntityData::ClassInfoPtr()))
            return &static_cast<fb::GameEntityData*>(data)->m_Components;
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::EffectEntityData::ClassInfoPtr()))
            return &static_cast<fb::EffectEntityData*>(data)->m_Components;
#endif
        return nullptr;
    }

    static bool isShaderParamData(fb::ClassInfo* classInfo)
    {
#if defined(BFVE_GAME_BF4)
        return classInfo && classInfo->isSubclassOf((fb::ClassInfo*)fb::ShaderParameterEntityData::ClassInfoPtr());
#else
        return classInfo && classInfo->isSubclassOf((fb::ClassInfo*)fb::ShaderParameterComponentData::ClassInfoPtr());
#endif
    }

    static fb::LocalLightEntityData* entityData(fb::LocalLightEntity* e)
    {
        if (!e)
            return nullptr;
        return fb::isValidPtr(e->m_data) ? e->m_data : nullptr;
    }

    static bool liveLightEntity(fb::LocalLightEntity* e, fb::LocalLightEntityData* data)
    {
        return data != nullptr && entityData(e) == data;
    }

    // clones
    static bool liveLightEntity(fb::LocalLightEntity* e, const LightDataEntry& entry)
    {
        fb::LocalLightEntityData* d = entityData(e);
        if (!d)
            return false;
        return d == entry.dataPtr;
    }

    // nan check, suually dead stuff is here
    static bool entityPos(fb::LocalLightEntity* e, fb::Vec3& out)
    {
        if (!e)
            return false;

        const fb::Vec3 p = e->transform().m_trans;
        if (!std::isfinite(p.m_x) || !std::isfinite(p.m_y) || !std::isfinite(p.m_z))
            return false;

        out = p;
        return true;
    }

    static void pruneDeadEntities()
    {
        size_t dropped = 0;

        for (auto& [dataPtr, entry] : entries)
        {
            for (auto it = entry.activeEntities.begin(); it != entry.activeEntities.end(); )
            {
                if (liveLightEntity(*it, entry))
                {
                    ++it;
                    continue;
                }

                it = entry.activeEntities.erase(it);
                ++dropped;
            }
        }

        if (dropped)
            logger::info("[lights] dropped {} stale light entity/entities", dropped);
    }

    static char filterBuffer[256] = { };
    static int typeFilter = 0;

    std::unordered_map<fb::LocalLightEntityData*, LightDataEntry>& getEntries() { return entries; }

    bool isUnresolvedName(const std::string& name)
    {
        return name.empty() || name == "(dynamic)" || name == "(unknown)" || name == "(unnamed)";
    }

    bool entryWorldPos(const LightDataEntry& entry, fb::Vec3& out)
    {
        for (fb::LocalLightEntity* e : entry.activeEntities)
        {
            if (!liveLightEntity(e, entry.dataPtr))
                continue;
            if (entityPos(e, out))
                return true;
        }
        return false;
    }

    std::unordered_map<fb::LocalLightEntityData*, std::string> buildDisplayNames()
    {
        std::unordered_map<fb::LocalLightEntityData*, std::string> result;
        result.reserve(entries.size());

        std::unordered_map<std::string, std::vector<fb::LocalLightEntityData*>> byName;
        for (auto& [ptr, entry] : entries)
        {
            if (isUnresolvedName(entry.assetName))
            {
                char buf[40];
                std::snprintf(buf, sizeof(buf), "light_%p", static_cast<void*>(ptr));
                result[ptr] = buf;
            }
            else
            {
                byName[entry.assetName].push_back(ptr);
            }
        }

        for (auto& [name, ptrs] : byName)
        {
            if (ptrs.size() == 1)
            {
                result[ptrs[0]] = name;
                continue;
            }

            std::sort(ptrs.begin(), ptrs.end(),
                [](fb::LocalLightEntityData* a, fb::LocalLightEntityData* b)
                {
                    fb::Vec3 pa{}, pb{};
                    const bool ha = entryWorldPos(entries[a], pa);
                    const bool hb = entryWorldPos(entries[b], pb);
                    if (ha != hb) return ha;
                    if (ha && hb)
                    {
                        if (pa.m_x != pb.m_x) return pa.m_x < pb.m_x;
                        if (pa.m_y != pb.m_y) return pa.m_y < pb.m_y;
                        if (pa.m_z != pb.m_z) return pa.m_z < pb.m_z;
                    }
                    return a < b;
                });

            for (size_t i = 0; i < ptrs.size(); ++i)
            {
                char buf[256];
                std::snprintf(buf, sizeof(buf), "%s (%zu)", name.c_str(), i + 1);
                result[ptrs[i]] = buf;
            }
        }

        return result;
    }

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
        forgetSunFlares();
        entries.clear();
        clearLampLinks();
        clearLevelPointers();
        scanned = false;
        filterBuffer[0] = '\0';
        typeFilter = 0;
    }

    static void RegisterLightData(fb::LocalLightEntityData* data, const std::string& assetName, const std::string& containerType, void* container)
    {
        if (!data)
            return;

        if (auto it = entries.find(data); it != entries.end())
        {
            auto& e = it->second;
            const bool placeholderName =
                e.assetName.empty() || e.assetName == "(dynamic)" || e.assetName == "(unknown)";
            const bool placeholderContainer =
                e.containerType.empty() || e.containerType == "Runtime";
            if (placeholderName) e.assetName = assetName;
            if (placeholderContainer) e.containerType = containerType;
            return;
        }

        LightDataEntry& entry = entries[data];
        entry.dataPtr = data;
        entry.assetName = assetName;
        entry.containerType = containerType;

        if (fb::ClassInfo* classInfo = fb::classOf(data))
        {
            const uint32_t classId = classInfo->m_ClassId;
            if (classId == fb::SpotLightEntityData::ClassId())
            {
                entry.isSpotLight = true;
                entry.lightType = "SpotLight";
            }
            else if (classId == fb::PointLightEntityData::ClassId())
            {
                entry.isPointLight = true;
                entry.lightType = "PointLight";
            }
            else
            {
                entry.lightType = "LocalLight";
            }
        }
    }

    struct Placement
    {
        void* data = nullptr;
        fb::Vec3 pos{ };
        bool hasPos = false;
    };

    static Placement placementOf(void* data, const fb::Vec3& origin)
    {
        Placement p;
        p.data = data;

        fb::Vec3 local{ };
        if (!data)
            return p;

        local = static_cast<fb::SpatialEntityData*>(data)->m_Transform.m_trans;

        if (!std::isfinite(local.m_x) || !std::isfinite(local.m_y) || !std::isfinite(local.m_z))
            return p;

        p.pos.m_x = local.m_x + origin.m_x;
        p.pos.m_y = local.m_y + origin.m_y;
        p.pos.m_z = local.m_z + origin.m_z;
        p.hasPos = true;
        return p;
    }

    struct ContainerContents
    {
        std::vector<Placement> lights;
        std::vector<MeshVariationRef> meshes;
        std::vector<Placement> meshObjects; // the entity data behind them, not always results in true data, but enough...
        std::vector<void*> shaderParams; // ShaderParameterEntityData
        std::vector<Placement> flares;
        std::vector<std::string> effects;
        void* blueprint = nullptr; // what was walked
    };

    // A flare this far from a light, in the prefab's own units, just a guess after all
    constexpr float FLARE_PAIR_RADIUS = 2.5f;

    static std::unordered_map<void*, std::vector<uint32_t>> placementVariations;
    constexpr size_t MAX_PLACEMENT_VARIATIONS = 8;

    static void CollectObject(fb::GameObjectData* el, uint32_t varHash, int refHops,
                              ContainerContents& out, const fb::Vec3& origin);

    static bool readComponentArray(fb::GameObjectData* el, fb::GameObjectData**& first,
                                   uint32_t& count)
    {
        fb::Array<fb::GameObjectData*>* comps = componentsOf(fb::classOf(el), el);
        if (!comps)
            return false;
        count = comps->size();
        first = comps->m_firstElement;
        return true;
    }

    static void CollectComponents(fb::GameObjectData* el, uint32_t varHash, int refHops,
                                  ContainerContents& out, const fb::Vec3& origin)
    {
        static int depth = 0;
        if (depth >= 2)
            return;

        uint32_t count = 0;
        fb::GameObjectData** first = nullptr;
        if (!readComponentArray(el, first, count))
            return;

        if (!first || count > 64)
            return;

        ++depth;
        for (uint32_t i = 0; i < count; ++i)
            if (first[i])
                CollectObject(first[i], varHash, refHops, out, origin);
        --depth;
    }

    static void AddMesh(fb::MeshAsset* mesh, uint32_t varHash, ContainerContents& out)
    {
        if (!mesh || !mesh->m_NameHash)
            return;
        const uint32_t nameHash = mesh->m_NameHash;

        char text[256];
        copyEngineString(mesh->m_Name, text, sizeof(text));

        MeshVariationRef ref;
        ref.key = uint64_t(varHash) | (uint64_t(nameHash) << 32);
        ref.name = text;
        ref.asset = mesh;

        for (const MeshVariationRef& existing : out.meshes)
            if (existing == ref)
                return;

        out.meshes.push_back(std::move(ref));
    }

    static void CollectBlueprint(fb::Blueprint* bp, uint32_t varHash, int refHops,
                                 ContainerContents& out, const fb::Vec3& origin)
    {
        if (!bp || refHops > 1)
            return;

        fb::ClassInfo* classInfo = fb::classOf(bp);
        if (!classInfo)
            return;

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::ObjectBlueprint::ClassInfoPtr()))
        {
            CollectObject(reinterpret_cast<fb::ObjectBlueprint*>(bp)->m_Object,
                          varHash, refHops, out, origin);
            return;
        }

        // SpatialPrefabBlueprint / WorldPartData / SubWorldData / LogicPrefabBlueprint all
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::PrefabBlueprint::ClassInfoPtr()))
        {
            for (auto el : reinterpret_cast<fb::PrefabBlueprint*>(bp)->m_Objects)
                CollectObject(el, varHash, refHops, out, origin);
        }
    }

    static void CollectObject(fb::GameObjectData* el, uint32_t varHash, int refHops,
                              ContainerContents& out, const fb::Vec3& origin)
    {
        if (!el || refHops > 1)
            return;

        fb::ClassInfo* classInfo = fb::classOf(el);
        if (!classInfo)
            return;

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::LocalLightEntityData::ClassInfoPtr()))
        {
            out.lights.push_back(placementOf(el, origin));
            return;
        }

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::StaticModelEntityData::ClassInfoPtr()))
        {
            AddMesh(static_cast<fb::StaticModelEntityData*>(el)->m_Mesh, varHash, out);
            out.meshObjects.push_back(placementOf(el, origin));

            CollectComponents(el, varHash, refHops, out, origin);
            return;
        }

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::MeshProxyEntityData::ClassInfoPtr()))
        {
            AddMesh(static_cast<fb::MeshProxyEntityData*>(el)->m_Mesh, varHash, out);
            out.meshObjects.push_back(placementOf(el, origin));
            CollectComponents(el, varHash, refHops, out, origin);
            return;
        }

        if (isShaderParamData(classInfo))
        {
            for (void* existing : out.shaderParams)
                if (existing == el)
                    return;
            if (out.shaderParams.size() < 16)
                out.shaderParams.push_back(el);
            return;
        }

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::LensFlareEntityData::ClassInfoPtr()))
        {
            for (const Placement& existing : out.flares)
                if (existing.data == el)
                    return;
            out.flares.push_back(placementOf(el, origin));
            return;
        }

        // effect placed to light, usually cone. Emitter stuff
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::EffectReferenceObjectData::ClassInfoPtr()))
        {
            auto* ref = static_cast<fb::EffectReferenceObjectData*>(el);
            if (!ref->m_Blueprint)
                return;

            char text[256];
            if (!copyEngineString(ref->m_Blueprint->m_Name, text, sizeof(text)))
                return;

            std::string name = text;
            for (const std::string& existing : out.effects)
                if (existing == name)
                    return;
            out.effects.push_back(std::move(name));
            return;
        }

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::ReferenceObjectData::ClassInfoPtr()))
        {
            if (refHops >= 1)
                return; // below the lamp blueprint is debris

            auto* ref = static_cast<fb::ReferenceObjectData*>(el);

            uint32_t nested = varHash;
            if (ref->m_ObjectVariation && ref->m_ObjectVariation->m_NameHash)
                nested = ref->m_ObjectVariation->m_NameHash;

            fb::Vec3 nestedOrigin = origin;
            const fb::Vec3& t = ref->m_BlueprintTransform.m_trans;
            if (std::isfinite(t.m_x) && std::isfinite(t.m_y) && std::isfinite(t.m_z))
            {
                nestedOrigin.m_x += t.m_x;
                nestedOrigin.m_y += t.m_y;
                nestedOrigin.m_z += t.m_z;
            }

            CollectBlueprint(ref->m_Blueprint, nested, refHops + 1, out, nestedOrigin);
        }
    }

    static std::vector<ContainerContents> pendingLinks;

    static bool containerIsProp(const char* typeName)
    {
        return typeName &&
               (std::strcmp(typeName, "SpatialPrefabBlueprint") == 0 ||
                std::strcmp(typeName, "PrefabBlueprint") == 0 ||
                std::strcmp(typeName, "LogicPrefabBlueprint") == 0);
    }

    constexpr size_t MAX_PROP_LIGHTS = 64;
    constexpr size_t MAX_PROP_MESHES = 8;

    template<typename ContainerType>
    static void RecordPlacementVariations(ContainerType* container)
    {
        if (!container)
            return;

        for (auto el : container->m_Objects)
        {
            fb::ClassInfo* classInfo = fb::classOf(el);
            if (!classInfo ||
                !classInfo->isSubclassOf((fb::ClassInfo*)fb::ReferenceObjectData::ClassInfoPtr()))
                continue;

            auto* ref = static_cast<fb::ReferenceObjectData*>(el);
            if (!ref->m_Blueprint || !ref->m_ObjectVariation || !ref->m_ObjectVariation->m_NameHash)
                continue;

            std::vector<uint32_t>& seen = placementVariations[ref->m_Blueprint];
            if (seen.size() >= MAX_PLACEMENT_VARIATIONS)
                continue;

            const uint32_t hash = ref->m_ObjectVariation->m_NameHash;
            if (std::find(seen.begin(), seen.end(), hash) == seen.end())
                seen.push_back(hash);
        }
    }

    template<typename ContainerType>
    static void CollectLampMeshes(ContainerType* container, const char* containerTypeName)
    {
        if (!container || !containerIsProp(containerTypeName))
            return;

        ContainerContents cc;
        cc.blueprint = container;
        const fb::Vec3 origin{ };
        for (auto el : container->m_Objects)
            CollectObject(el, 0, 0, cc, origin);

        if (cc.lights.empty() ||
            (cc.meshes.empty() && cc.flares.empty() && cc.effects.empty() &&
             cc.meshObjects.empty() && cc.shaderParams.empty()))
            return;

        if (cc.lights.size() > MAX_PROP_LIGHTS || cc.meshes.size() > MAX_PROP_MESHES)
        {
            logger::debug("[lights] skipping {} - {} light(s), {} mesh(es): too broad to link",
                containerTypeName, cc.lights.size(), cc.meshes.size());
            return;
        }

        pendingLinks.push_back(std::move(cc));
    }

    struct WorldMesh
    {
        fb::MeshAsset* mesh;
        uint32_t varHash;
        fb::Vec3 pos;
        fb::LinearTransform frame; // world space
        bool hasBox;
        fb::Vec3 boxMin, boxMax;
        const void* member; // StaticModelGroupMemberData, EBX-placed only
        uint8_t radiosityOverride; // ReferenceObjectData / per-instance RadiosityTypeOverride
    };
    struct WorldLight { fb::LocalLightEntityData* data; fb::Vec3 pos; };
    static std::vector<WorldMesh> worldMeshes;
    static std::vector<WorldLight> worldLights;
    static std::unordered_map<fb::LocalLightEntityData*, std::vector<MeshVariationRef>> nearbyMeshes;
    constexpr float MESH_PAIR_RADIUS = 2.5f;
    constexpr float MESH_BOX_SLACK = 0.5f;
    constexpr size_t MAX_WORLD_MESHES = 150000; // to not overtank the overlay

    static std::unordered_map<fb::MeshAsset*, std::pair<bool, std::pair<fb::Vec3, fb::Vec3>>> g_meshBoxes;
    struct SubsetGeo
    {
        uint32_t material;
        bool hasBox;
        fb::Vec3 mn, mx;
    };
    struct MeshGeo
    {
        fb::MeshSet* set = nullptr;
        std::vector<SubsetGeo> subsets;
        std::vector<uint8_t> vertices, indices;
        std::vector<uint8_t> lodVertices[6], lodIndices[6]; // LOD 1+, [0] unused
        uint32_t vertexDataSize = 0, indexDataSize = 0;
        int copyState = 0;

        // ERRORS STUFF
        std::string reason; // error reason, shouldn't happen? please test if ever reports
        uint32_t stride0 = 0, fmt0 = 0, triangles = 0;
    };
    static std::mutex g_geoCopyMutex;
    static std::vector<fb::MeshAsset*> g_geoCopyQueue;
    static std::unordered_map<fb::MeshAsset*, MeshGeo> g_meshGeo;

    // field packing is different between bf3 and bf4
    static fb::MeshLayout* lodAt(const fb::MeshSet* set, uint32_t l)
    {
        if (!set || !set->m_layout || l >= set->m_layout->m_lodCount)
            return nullptr;
#if defined(BFVE_GAME_BF4)
        return static_cast<fb::MeshLayout*>(set->m_layout->m_lods[l]);
#else
        return set->m_layout->m_lods[l].as<fb::MeshLayout>();
#endif
    }
    static fb::MeshLayout* lod0(const fb::MeshSet* set) { return lodAt(set, 0); }
    static fb::MeshSubset* subsetsOf(const fb::MeshLayout* lod)
    {
#if defined(BFVE_GAME_BF4)
        return lod->subsets();
#else
        return lod->m_subsets.as<fb::MeshSubset>();
#endif
    }
    static fb::MeshData* dataOf(const fb::MeshLayout* lod)
    {
#if defined(BFVE_GAME_BF4)
        return lod->m_data;
#else
        return lod->m_data.as<fb::MeshData>();
#endif
    }
    // {format, offset, stream}
    static void element(const fb::MeshSubset& sub, uint32_t e, uint32_t& format, uint32_t& offset, uint32_t& stream)
    {
#if defined(BFVE_GAME_BF4)
        const uint32_t el = sub.m_declElements[e];
        format = (el >> 8) & 0xFF; offset = (el >> 16) & 0xFF; stream = (el >> 24) & 0xFF;
#else
        format = sub.m_elements[e][1]; offset = sub.m_elements[e][2]; stream = sub.m_elements[e][3];
#endif
    }
    static uint32_t elementFormat(const fb::MeshSubset& sub, uint32_t e)
    {
        uint32_t f = 0, o = 0, st = 0;
        element(sub, e, f, o, st);
        return f;
    }
    static uint32_t stride0(const fb::MeshSubset& sub)
    {
#if defined(BFVE_GAME_BF4)
        return sub.m_streamStrides[0];
#else
        return sub.m_streams[0][0] ? sub.m_streams[0][0] : sub.m_vertexStride;
#endif
    }
    // ID3D11Buffer*: BF4 DxBuffer +0x10 (sub_140BEBA30), BF3 +0x08
    static ID3D11Buffer* d3dBuffer(void* renderBuffer)
    {
        if (!renderBuffer)
            return nullptr;
#if defined(BFVE_GAME_BF4)
        return *reinterpret_cast<ID3D11Buffer**>(static_cast<uint8_t*>(renderBuffer) + 0x10);
#else
        return static_cast<fb::DxRenderBuffer*>(renderBuffer)->m_buffer;
#endif
    }

    static fb::MeshSet* findMeshSet(fb::MeshAsset* mesh)
    {
        if (!mesh)
            return nullptr;
        const uint32_t hash = mesh->m_NameHash;
#if defined(BFVE_GAME_BF4)
        void* registry = fb::MeshSetRegistry::Instance();
        if (!registry)
            return nullptr;
        using find_t = fb::MeshSet* (__fastcall*)(void*, int);
        fb::MeshSet* set = reinterpret_cast<find_t>(fb::MeshSetRegistry::FIND)(registry, int(hash));
        const uint32_t maxLods = 6;
#else
        auto* set = static_cast<fb::MeshSet*>(textures::meshSetByName(mesh->m_Name));
        const uint32_t maxLods = 5;
#endif
        if (!set || !set->m_layout || set->m_layout->m_nameHash != hash ||
            set->m_layout->m_lodCount < 1 || set->m_layout->m_lodCount > maxLods)
            return nullptr;
        return set;
    }

    static bool meshLocalBox(fb::MeshAsset* mesh, fb::Vec3& mn, fb::Vec3& mx)
    {
        if (!mesh)
            return false;
        auto cached = g_meshBoxes.find(mesh);
        if (cached != g_meshBoxes.end())
        {
            mn = cached->second.second.first;
            mx = cached->second.second.second;
            return cached->second.first;
        }

        bool ok = false;
#if defined(BFVE_GAME_BF4)
        if (void* registry = fb::MeshSetRegistry::Instance())
        {
            using find_t = fb::MeshSet* (__fastcall*)(void*, int);
            const uint32_t hash = mesh->m_NameHash;
            fb::MeshSet* set = reinterpret_cast<find_t>(fb::MeshSetRegistry::FIND)(registry, int(hash));
            if (set && set->m_layout && set->m_layout->m_nameHash == hash &&
                set->m_layout->m_lodCount <= 6)
            {
                mn = set->m_layout->m_boundingMin;
                mx = set->m_layout->m_boundingMax;
                ok = true;
            }
        }
#else
        if (auto* set = static_cast<fb::MeshSet*>(textures::meshSetByName(mesh->m_Name)))
        {
            if (set->m_layout && set->m_layout->m_nameHash == mesh->m_NameHash &&
                set->m_layout->m_lodCount <= 5)
            {
                mn = set->m_layout->m_boundingMin;
                mx = set->m_layout->m_boundingMax;
                ok = true;
            }
        }
#endif
        g_meshBoxes[mesh] = { ok, { mn, mx } };
        return ok;
    }

    static WorldMesh makeWorldMesh(fb::MeshAsset* mesh, uint32_t varHash, const fb::LinearTransform& frame)
    {
        WorldMesh m{ };
        m.mesh = mesh;
        m.varHash = varHash;
        m.pos = frame.m_trans;
        m.frame = frame;
        m.hasBox = meshLocalBox(mesh, m.boxMin, m.boxMax);
        return m;
    }

    static bool insideWorldMesh(const WorldMesh& m, const fb::Vec3& p, float& volumeOut)
    {
        if (!m.hasBox)
            return false;

        const fb::Vec3 d{ p.m_x - m.frame.m_trans.m_x, p.m_y - m.frame.m_trans.m_y, p.m_z - m.frame.m_trans.m_z };
        const auto axis = [&](const fb::Vec3& r) -> float
        {
            const float len2 = r.m_x * r.m_x + r.m_y * r.m_y + r.m_z * r.m_z;
            return len2 > 1e-8f ? (d.m_x * r.m_x + d.m_y * r.m_y + d.m_z * r.m_z) / len2 : 0.0f;
        };
        const float lx = axis(m.frame.m_right), ly = axis(m.frame.m_up), lz = axis(m.frame.m_forward);

        const float s = MESH_BOX_SLACK;
        if (lx < m.boxMin.m_x - s || lx > m.boxMax.m_x + s ||
            ly < m.boxMin.m_y - s || ly > m.boxMax.m_y + s ||
            lz < m.boxMin.m_z - s || lz > m.boxMax.m_z + s)
            return false;

        volumeOut = (m.boxMax.m_x - m.boxMin.m_x) * (m.boxMax.m_y - m.boxMin.m_y) * (m.boxMax.m_z - m.boxMin.m_z);
        return true;
    }

    // p' = trans + x*right + y*up + z*forward
    static fb::Vec3 applyTransform(const fb::LinearTransform& t, const fb::Vec3& p)
    {
        fb::Vec3 r{ };
        r.m_x = t.m_trans.m_x + p.m_x * t.m_right.m_x + p.m_y * t.m_up.m_x + p.m_z * t.m_forward.m_x;
        r.m_y = t.m_trans.m_y + p.m_x * t.m_right.m_y + p.m_y * t.m_up.m_y + p.m_z * t.m_forward.m_y;
        r.m_z = t.m_trans.m_z + p.m_x * t.m_right.m_z + p.m_y * t.m_up.m_z + p.m_z * t.m_forward.m_z;
        return r;
    }

    static fb::LinearTransform composeTransform(const fb::LinearTransform& outer,
                                                const fb::LinearTransform& inner)
    {
        const auto rot = [&](const fb::Vec3& v)
        {
            fb::Vec3 r{ };
            r.m_x = v.m_x * outer.m_right.m_x + v.m_y * outer.m_up.m_x + v.m_z * outer.m_forward.m_x;
            r.m_y = v.m_x * outer.m_right.m_y + v.m_y * outer.m_up.m_y + v.m_z * outer.m_forward.m_y;
            r.m_z = v.m_x * outer.m_right.m_z + v.m_y * outer.m_up.m_z + v.m_z * outer.m_forward.m_z;
            return r;
        };
        fb::LinearTransform r = outer;
        r.m_right = rot(inner.m_right);
        r.m_up = rot(inner.m_up);
        r.m_forward = rot(inner.m_forward);
        r.m_trans = applyTransform(outer, inner.m_trans);
        return r;
    }

    static fb::LinearTransform invertTransform(const fb::LinearTransform& t)
    {
        const float m[3][3] = {
            { t.m_right.m_x, t.m_up.m_x, t.m_forward.m_x },
            { t.m_right.m_y, t.m_up.m_y, t.m_forward.m_y },
            { t.m_right.m_z, t.m_up.m_z, t.m_forward.m_z } };
        const float det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
                        - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
                        + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
        fb::LinearTransform r{ };
        if (std::fabs(det) < 1e-12f)
            return r;
        const float id = 1.0f / det;
        float inv[3][3];
        inv[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * id;
        inv[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) * id;
        inv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * id;
        inv[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * id;
        inv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * id;
        inv[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * id;
        inv[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * id;
        inv[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * id;
        inv[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * id;
        r.m_right = { inv[0][0], inv[1][0], inv[2][0] };
        r.m_up = { inv[0][1], inv[1][1], inv[2][1] };
        r.m_forward = { inv[0][2], inv[1][2], inv[2][2] };
        const fb::Vec3& p = t.m_trans;
        r.m_trans = { -(inv[0][0] * p.m_x + inv[0][1] * p.m_y + inv[0][2] * p.m_z),
                      -(inv[1][0] * p.m_x + inv[1][1] * p.m_y + inv[1][2] * p.m_z),
                      -(inv[2][0] * p.m_x + inv[2][1] * p.m_y + inv[2][2] * p.m_z) };
        return r;
    }

#if defined(BFVE_GAME_BF3)
    // Rigid/CompositeMeshEntityData (m_Mesh +0x50), sometimes StaticModelEntityData
    static fb::MeshAsset* groupMemberMesh(const fb::StaticModelGroupMemberData& mem)
    {
        fb::ClassInfo* mci = fb::classOf(mem.m_MeshEntityType);
        if (!mci)
            return nullptr;
        if (mci->isSubclassOf((fb::ClassInfo*)fb::RigidMeshEntityData::ClassInfoPtr()))
            return static_cast<fb::RigidMeshEntityData*>(mem.m_MeshEntityType)->m_Mesh;
        if (mci->isSubclassOf((fb::ClassInfo*)fb::CompositeMeshEntityData::ClassInfoPtr()))
            return static_cast<fb::CompositeMeshEntityData*>(mem.m_MeshEntityType)->m_Mesh;
        if (mci->isSubclassOf((fb::ClassInfo*)fb::StaticModelEntityData::ClassInfoPtr()))
            return static_cast<fb::StaticModelEntityData*>(mem.m_MeshEntityType)->m_Mesh;
        return nullptr;
    }
#endif

    template <typename Register>
    static void ForEachLightIn(fb::GameObjectData* el, int depth, Register&& reg);
    static void RegisterLightData(fb::LocalLightEntityData* data, const std::string& assetName, const std::string& containerType, void* container);

    static std::unordered_set<void*> g_registeredBlueprints;
    static void RegisterBlueprintLights(fb::Blueprint* bp, fb::ClassInfo* bci, const fb::LinearTransform& frame)
    {
        if (!bp || !bci)
            return;
        const bool first = g_registeredBlueprints.insert(bp).second;
        const std::string name = bp->m_Name ? bp->m_Name : "(unnamed)";
        const auto visit = [&](fb::GameObjectData* root, const char* type)
        {
            ForEachLightIn(root, 0, [&](fb::LocalLightEntityData* light)
            {
                if (first)
                    RegisterLightData(light, name, type, bp);
                if (worldLights.size() < MAX_WORLD_MESHES)
                    worldLights.push_back({ light,
                        applyTransform(frame, static_cast<fb::SpatialEntityData*>(light)->m_Transform.m_trans) });
            });
        };
        if (bci->isSubclassOf((fb::ClassInfo*)fb::ObjectBlueprint::ClassInfoPtr()))
            visit(reinterpret_cast<fb::ObjectBlueprint*>(bp)->m_Object, "ObjectBlueprint");
        else if (bci->isSubclassOf((fb::ClassInfo*)fb::PrefabBlueprint::ClassInfoPtr()))
            for (auto child : reinterpret_cast<fb::PrefabBlueprint*>(bp)->m_Objects)
                if (!fb::classOf(child) || !fb::classOf(child)->isSubclassOf((fb::ClassInfo*)fb::LocalLightEntityData::ClassInfoPtr()))
                    visit(child, "PrefabBlueprint"); // the walk places direct lights
    }

    static void CollectWorldPlacements(fb::GameObjectData* el, const fb::LinearTransform& frame,
                                       uint32_t varHash, int hops, uint8_t radOverride = 0)
    {
        if (!el || hops > 4 || worldMeshes.size() >= MAX_WORLD_MESHES)
            return;

        fb::ClassInfo* classInfo = fb::classOf(el);
        if (!classInfo)
            return;

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::LocalLightEntityData::ClassInfoPtr()))
        {
            const fb::Vec3 pos = applyTransform(frame, static_cast<fb::SpatialEntityData*>(el)->m_Transform.m_trans);
            worldLights.push_back({ static_cast<fb::LocalLightEntityData*>(el), pos });
            return;
        }

        fb::MeshAsset* mesh = nullptr;
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::StaticModelEntityData::ClassInfoPtr()))
            mesh = static_cast<fb::StaticModelEntityData*>(el)->m_Mesh;
        else if (classInfo->isSubclassOf((fb::ClassInfo*)fb::MeshProxyEntityData::ClassInfoPtr()))
            mesh = static_cast<fb::MeshProxyEntityData*>(el)->m_Mesh;
#if defined(BFVE_GAME_BF3)
        else if (classInfo->isSubclassOf((fb::ClassInfo*)fb::RigidMeshEntityData::ClassInfoPtr()))
            mesh = static_cast<fb::RigidMeshEntityData*>(el)->m_Mesh;
        else if (classInfo->isSubclassOf((fb::ClassInfo*)fb::CompositeMeshEntityData::ClassInfoPtr()))
            mesh = static_cast<fb::CompositeMeshEntityData*>(el)->m_Mesh;
#endif
        if (mesh)
        {
            worldMeshes.push_back(makeWorldMesh(mesh, varHash,
                composeTransform(frame, static_cast<fb::SpatialEntityData*>(el)->m_Transform)));
            worldMeshes.back().radiosityOverride = radOverride;
            return;
        }

        // one member per mesh, one transform per instance
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::StaticModelGroupEntityData::ClassInfoPtr()))
        {
            auto* group = static_cast<fb::StaticModelGroupEntityData*>(el);
            const fb::LinearTransform gframe =
                composeTransform(frame, static_cast<fb::SpatialEntityData*>(el)->m_Transform);

            for (fb::StaticModelGroupMemberData& mem : group->m_MemberDatas)
            {
#if defined(BFVE_GAME_BF4)
                fb::MeshAsset* member = mem.m_MeshAsset;
#else
                fb::MeshAsset* member = groupMemberMesh(mem);
#endif
                if (!member || mem.m_InstanceTransforms.size() == 0)
                    continue;

                const uint32_t n = mem.m_InstanceTransforms.size();
                const uint32_t nv = mem.m_InstanceObjectVariation.m_firstElement
                    ? mem.m_InstanceObjectVariation.size() : 0;
                for (uint32_t i = 0; i < n && worldMeshes.size() < MAX_WORLD_MESHES; ++i)
                {
                    const uint32_t vh = i < nv ? mem.m_InstanceObjectVariation.At(int(i)) : varHash;
                    worldMeshes.push_back(makeWorldMesh(member, vh,
                        composeTransform(gframe, mem.m_InstanceTransforms.At(int(i)))));
                    worldMeshes.back().member = &mem;
                    worldMeshes.back().radiosityOverride = radOverride;
#if defined(BFVE_GAME_BF4)
                    if (mem.m_InstanceRadiosityTypeOverride.m_firstElement && i < mem.m_InstanceRadiosityTypeOverride.size() && mem.m_InstanceRadiosityTypeOverride.At(int(i)))
                        worldMeshes.back().radiosityOverride = uint8_t(mem.m_InstanceRadiosityTypeOverride.At(int(i)));
#endif
                }
            }
            return;
        }

        if (!classInfo->isSubclassOf((fb::ClassInfo*)fb::ReferenceObjectData::ClassInfoPtr()))
            return;

        auto* ref = static_cast<fb::ReferenceObjectData*>(el);
        fb::Blueprint* bp = ref->m_Blueprint;
        fb::ClassInfo* bci = fb::classOf(bp);
        if (!bci)
            return;

        const fb::LinearTransform inner = composeTransform(frame, ref->m_BlueprintTransform);
        const uint32_t vh = ref->m_ObjectVariation ? ref->m_ObjectVariation->m_NameHash : varHash;
        RegisterBlueprintLights(bp, bci, inner);
        uint8_t ro = radOverride;
#if defined(BFVE_GAME_BF4)
        if (ref->m_RadiosityTypeOverride) ro = uint8_t(ref->m_RadiosityTypeOverride);
#endif

        if (bci->isSubclassOf((fb::ClassInfo*)fb::ObjectBlueprint::ClassInfoPtr()))
            CollectWorldPlacements(reinterpret_cast<fb::ObjectBlueprint*>(bp)->m_Object, inner, vh, hops + 1, ro);
        else if (bci->isSubclassOf((fb::ClassInfo*)fb::PrefabBlueprint::ClassInfoPtr()))
            for (auto child : reinterpret_cast<fb::PrefabBlueprint*>(bp)->m_Objects)
                CollectWorldPlacements(child, inner, vh, hops + 1, ro);
    }

#if defined(BFVE_GAME_BF4)
    static std::unordered_map<void*, fb::LinearTransform> g_groupFrames;

    static uint8_t instanceRadiosityOverride(fb::StaticModelGroupMemberData* mem, uint32_t i)
    {
        auto& a = mem->m_InstanceRadiosityTypeOverride;
        return a.m_firstElement && i < a.size() ? uint8_t(a.At(int(i))) : 0;
    }

    static fb::LinearTransform memberLocal(fb::StaticModelGroupMember* rec, uint32_t i)
    {
        alignas(16) fb::LinearTransform local{ };
        rec->localTransform(i, &local);
        return local;
    }

    static void CollectGroupInstances()
    {
        std::vector<void*> heads;
        collectHeads(fb::ClientStaticModelGroupEntity::ClassInfoPtr(),
                     uint16_t(fb::ClientStaticModelGroupEntity::ClassId()), heads);
        g_groupFrames.clear();

        size_t groups = 0, added = 0, viaPhysics = 0;
        for (void* head : heads)
        {
            uint32_t seen = 0;
            for (void* node = head; node && ++seen <= 65536; )
            {
                auto* e = reinterpret_cast<fb::ClientStaticModelGroupEntity*>(
                    static_cast<uint8_t*>(node) - fb::STATIC_MODEL_GROUP_LINK_OFFSET);
                ++groups;

                const size_t memberCount = e->m_data ? e->m_data->m_MemberDatas.size() : 0;
                fb::StaticModelGroupMember* end = e->m_membersEnd;
                if (e->m_membersBegin && end > e->m_membersBegin + memberCount)
                    end = e->m_membersBegin + memberCount;

                for (fb::StaticModelGroupMember* rec = e->m_membersBegin; rec && rec < end; ++rec)
                {
                    fb::StaticModelGroupMemberData* mem = rec->m_data;
                    if (!mem || !mem->m_MeshAsset || !rec->m_instances)
                        continue;
                    if (mem->m_InstanceTransforms.size() != 0)
                        worldMeshes.erase(std::remove_if(worldMeshes.begin(), worldMeshes.end(),
                            [mem](const WorldMesh& w) { return w.member == mem; }), worldMeshes.end());

                    const uintptr_t vt = reinterpret_cast<uintptr_t>(rec->m_instances->m_vtable);
                    size_t stride = 0;
                    if (vt == fb::StaticModelGroupMeshInstance::RIGID_VTABLE)
                        stride = fb::StaticModelGroupMeshInstance::RIGID_SIZE;
                    else if (vt == fb::StaticModelGroupMeshInstance::COMPOSITE_VTABLE)
                        stride = fb::StaticModelGroupMeshInstance::COMPOSITE_SIZE;

                    const uint32_t nv = mem->m_InstanceObjectVariation.m_firstElement
                        ? mem->m_InstanceObjectVariation.size() : 0;

                    if (!stride)
                    {
                        // group frame from a sibling, local from the engine
                        auto frameIt = g_groupFrames.find(e);
                        if (frameIt == g_groupFrames.end())
                            continue; // no sibling seen yet
                        for (uint32_t i = 0; i < mem->m_InstanceCount && worldMeshes.size() < MAX_WORLD_MESHES; ++i)
                        {
                            const uint32_t vh = i < nv ? mem->m_InstanceObjectVariation.At(int(i)) : 0;
                            worldMeshes.push_back(makeWorldMesh(mem->m_MeshAsset, vh, composeTransform(frameIt->second, memberLocal(rec, i))));
                            worldMeshes.back().radiosityOverride = instanceRadiosityOverride(mem, i);
                            ++added;
                            ++viaPhysics;
                        }
                        continue;
                    }

                    for (uint32_t i = 0; i < mem->m_InstanceCount && worldMeshes.size() < MAX_WORLD_MESHES; ++i)
                    {
                        auto* inst = reinterpret_cast<fb::StaticModelGroupMeshInstance*>(
                            reinterpret_cast<uint8_t*>(rec->m_instances) + i * stride);
                        if (reinterpret_cast<uintptr_t>(inst->m_vtable) != vt)
                            continue;
                        if (i == 0 && !g_groupFrames.count(e))
                        {
                            g_groupFrames[e] = composeTransform(inst->m_transform, invertTransform(memberLocal(rec, 0)));
                        }
                        const uint32_t vh = i < nv ? mem->m_InstanceObjectVariation.At(int(i)) : 0;
                        worldMeshes.push_back(makeWorldMesh(mem->m_MeshAsset, vh, inst->m_transform));
                        worldMeshes.back().radiosityOverride = instanceRadiosityOverride(mem, i);
                        ++added;
                    }
                }

                void* next = e->m_link.next;
                if (!next || next == head)
                    break;
                node = next;
            }
        }
        logger::info("[lights] {} group entities, {} instances placed from their Havok transforms ({} via physics parts)",
                     groups, added, viaPhysics);
    }
#else
    struct GroupPhysicsRef { fb::StaticModelGroupMemberData* member; fb::MeshAsset* mesh; };
    static std::unordered_map<void*, GroupPhysicsRef> g_groupPhysics;
    static std::unordered_set<const void*> g_liveGroupData;

    static void CollectGroupInstances()
    {
        g_groupPhysics.clear();
        // group entities are not iterable
        // g_liveGroupData filters the freed ones
        fb::EntityList<fb::ClientStaticModelGroupEntity> list{
            reinterpret_cast<fb::ClassInfo*>(fb::ClientStaticModelGroupEntity::ClassInfoPtr()), false };

        size_t groups = 0, added = 0, members = 0, noPhysics = 0, noMesh = 0, noInstance = 0;
        size_t outOfRange = 0, badInstances = 0;
        uintptr_t firstBadVt = 0;
        const auto containsNoCase = [](const char* hay, const char* needle)
        {
            if (!hay || !needle || !*needle)
                return false;
            const size_t n = std::strlen(needle);
            for (const char* p = hay; *p; ++p)
                if (_strnicmp(p, needle, n) == 0)
                    return true;
            return false;
        };
        fb::ClientStaticModelGroupEntity* e = nullptr;
        size_t stale = 0;
        while ((e = list.nextOfKind()) != nullptr)
        {
            ++groups;
            if (!g_liveGroupData.count(e->m_data))
            {
                ++stale;
                continue;
            }
            const size_t memberCount = e->m_data->m_MemberDatas.size();
            fb::ClientStaticModelGroupMember* end = e->m_membersEnd;
            if (e->m_membersBegin && end > e->m_membersBegin + memberCount)
                end = e->m_membersBegin + memberCount;

            for (fb::ClientStaticModelGroupMember* rec = e->m_membersBegin; rec && rec < end; ++rec)
            {
                if (reinterpret_cast<uintptr_t>(rec->m_vtable) != fb::ClientStaticModelGroupMember::VTABLE)
                    continue;
                ++members;
                fb::StaticModelGroupMemberData* mem = rec->m_data;
                if (!mem)
                    continue;
                fb::MeshAsset* mesh = groupMemberMesh(*mem);
                const bool watched = mesh && containsNoCase(mesh->m_Name, meshOverlayFilter);
                if (!mesh)
                {
                    ++noMesh;
                    continue;
                }
                const bool ebxTransforms = mem->m_InstanceTransforms.m_firstElement && mem->m_InstanceTransforms.size() != 0;
                fb::StaticModelGroupPhysics* phys = rec->m_physics;
                const uint32_t nv = mem->m_InstanceObjectVariation.m_firstElement
                    ? mem->m_InstanceObjectVariation.size() : 0;
                size_t placedHere = 0, skipped = 0;
                uintptr_t skippedVt = 0;
                const auto place = [&](fb::StaticModelGroupMeshInstance* inst, uint32_t i)
                {
                    if (!inst)
                    {
                        ++noInstance;
                        return;
                    }
                    const uintptr_t vt = reinterpret_cast<uintptr_t>(inst->m_vtable);
                    if (vt != fb::StaticModelGroupMeshInstance::RIGID_VTABLE &&
                        vt != fb::StaticModelGroupMeshInstance::COMPOSITE_VTABLE)
                    {
                        ++skipped;
                        skippedVt = vt;
                        return;
                    }
                    if (worldMeshes.size() >= MAX_WORLD_MESHES)
                        return;
                    const uint32_t vh = i < nv ? mem->m_InstanceObjectVariation.At(int(i)) : 0;
                    worldMeshes.push_back(makeWorldMesh(mesh, vh, inst->m_transform));
                    ++added;
                    ++placedHere;
                };

                if (ebxTransforms)
                {
                    if (rec->m_entitiesWithoutPhysics)
                    {
                        // realized transforms replace the ebx composed ones
                        worldMeshes.erase(std::remove_if(worldMeshes.begin(), worldMeshes.end(),
                            [mem](const WorldMesh& w) { return w.member == mem; }), worldMeshes.end());
                        for (uint32_t i = 0; i < mem->m_InstanceCount; ++i)
                            place(rec->m_entitiesWithoutPhysics[i], i);
                    }
                    if (phys)
                        g_groupPhysics[phys] = { mem, mesh };
                }
                else if (!phys || !phys->m_partTable)
                {
                    ++noPhysics;
                }
                else
                {
                    g_groupPhysics[phys] = { mem, mesh };
                    const bool singleEntry = (phys->m_flags & 0x40000000u) != 0;
                    const uint32_t partCount = uint32_t(phys->m_partTableEnd - phys->m_partTable);
                    for (uint32_t i = 0; i < mem->m_InstanceCount; ++i)
                    {
                        const uint32_t part = singleEntry ? 0u
                            : mem->m_PhysicsPartRange.m_First + i * mem->m_PhysicsPartCountPerInstance;
                        if (part >= partCount)
                        {
                            ++outOfRange;
                            continue;
                        }
                        place(phys->m_partTable[part].m_instance, i);
                    }
                }
                badInstances += skipped;
                if (skipped && !firstBadVt)
                    firstBadVt = skippedVt;
                if (watched)
                    logger::info("[lights] group member {}: {} instances, EBX transforms {}, physics {}, part table {} entries, parts {}+{}/instance: {} placed, {} skipped (vtable {:#x})",
                                 mesh->m_Name ? mesh->m_Name : "?", mem->m_InstanceCount, ebxTransforms ? "yes" : "no",
                                 phys ? "yes" : "no", phys && phys->m_partTable ? uint32_t(phys->m_partTableEnd - phys->m_partTable) : 0u,
                                 mem->m_PhysicsPartRange.m_First, mem->m_PhysicsPartCountPerInstance, placedHere, skipped, skippedVt);
            }
        }
        logger::info("[lights] {} group entities ({} with data no longer loaded), {} members: {} instances placed, "
                     "{} members without physics, {} without a mesh, {} parts without an instance, {} parts out of range, "
                     "{} instances of another class (vtable {:#x})",
                     groups, stale, members, added, noPhysics, noMesh, noInstance, outOfRange, badInstances, firstBadVt);
    }
#endif

    static void BuildNearbyMeshes()
    {
        nearbyMeshes.clear();
        if (worldMeshes.empty() || worldLights.empty())
            return;

        const float limit = MESH_PAIR_RADIUS * MESH_PAIR_RADIUS;
        uint32_t byBox = 0;
        for (const WorldLight& l : worldLights)
        {
            const WorldMesh* best = nullptr;
            float bestVolume = FLT_MAX;
            for (const WorldMesh& m : worldMeshes)
            {
                float volume = 0.0f;
                if (insideWorldMesh(m, l.pos, volume) && volume < bestVolume)
                {
                    bestVolume = volume;
                    best = &m;
                }
            }
            if (best)
                ++byBox;

            float bestSqr = limit;
            for (const WorldMesh& m : worldMeshes)
            {
                if (best)
                    break;
                const float d = distanceSqr(l.pos, m.pos);
                if (d < bestSqr) { bestSqr = d; best = &m; }
            }
            if (!best)
                continue;

            ContainerContents cc;
            AddMesh(best->mesh, best->varHash, cc);
            std::vector<MeshVariationRef>& list = nearbyMeshes[l.data];
            for (const MeshVariationRef& m : cc.meshes)
                if (std::find(list.begin(), list.end(), m) == list.end())
                    list.push_back(m);
        }

        logger::info("[lights] world placements: {} light(s), {} mesh(es), {} light data linked ({} by box)",
            worldLights.size(), worldMeshes.size(), nearbyMeshes.size(), byBox);
    }

    static std::unordered_map<fb::LocalLightEntityData*, std::vector<uint32_t>> linkIndex;

    static void clearLampLinks()
    {
        pendingLinks.clear();
        pendingLinks.shrink_to_fit();
        linkIndex.clear();
        placementVariations.clear();
        worldMeshes.clear();
        worldLights.clear();
        nearbyMeshes.clear();
        g_meshBoxes.clear();
        g_meshGeo.clear();
    }

    static void linkFromContainers(fb::LocalLightEntityData* data, LightDataEntry& entry)
    {
        auto at = linkIndex.find(data);
        if (at == linkIndex.end())
            return;

        for (uint32_t i : at->second)
        {
            if (i >= pendingLinks.size())
                continue;

            const ContainerContents& cc = pendingLinks[i];

            for (const MeshVariationRef& mesh : cc.meshes)
            {
                bool have = false;
                for (const MeshVariationRef& existing : entry.lampMeshes)
                    if (existing == mesh) { have = true; break; }
                if (!have)
                    entry.lampMeshes.push_back(mesh);
            }

            std::vector<const Placement*> selves;
            for (const Placement& l : cc.lights)
                if (l.data == data && l.hasPos)
                    selves.push_back(&l);

            const bool measurable = !selves.empty();

            const Placement* nearest = nullptr;
            float nearestSqr = FLT_MAX;

            for (const Placement& flare : cc.flares)
            {
                if (!flare.data)
                    continue;

                bool keep = true;
                if (measurable && flare.hasPos)
                {
                    float best = FLT_MAX;
                    for (const Placement* self : selves)
                        best = (std::min)(best, distanceSqr(self->pos, flare.pos));

                    if (best < nearestSqr) { nearestSqr = best; nearest = &flare; }
                    keep = best <= FLARE_PAIR_RADIUS * FLARE_PAIR_RADIUS;
                }
                else if (cc.flares.size() > 1)
                {
                    keep = false;
                }

                if (!keep)
                    continue;

                bool have = false;
                for (void* existing : entry.lampFlares)
                    if (existing == flare.data) { have = true; break; }
                if (!have)
                    entry.lampFlares.push_back(flare.data);
            }

            if (entry.lampFlares.empty() && nearest)
                entry.lampFlares.push_back(nearest->data);
            else if (entry.lampFlares.empty() && !cc.flares.empty() && cc.flares.size() == 1)
                entry.lampFlares.push_back(cc.flares.front().data);

            for (void* sp : cc.shaderParams)
            {
                bool have = false;
                for (void* existing : entry.lampShaderParams)
                    if (existing == sp) { have = true; break; }
                if (!have)
                    entry.lampShaderParams.push_back(sp);
            }

            for (const Placement& obj : cc.meshObjects)
            {
                if (!obj.data)
                    continue;

                bool have = false;
                for (void* existing : entry.lampMeshObjects)
                    if (existing == obj.data) { have = true; break; }
                if (!have)
                    entry.lampMeshObjects.push_back(obj.data);
            }

            for (const std::string& fx : cc.effects)
            {
                bool have = false;
                for (const std::string& existing : entry.lampEffects)
                    if (existing == fx) { have = true; break; }
                if (!have)
                    entry.lampEffects.push_back(fx);
            }
        }
    }

    static void linkLampContents(fb::LocalLightEntityData* data, LightDataEntry& entry)
    {
        linkFromContainers(data, entry);

        if (entry.lampMeshes.empty())
            if (auto nb = nearbyMeshes.find(data); nb != nearbyMeshes.end())
                for (const MeshVariationRef& mesh : nb->second)
                    if (std::find(entry.lampMeshes.begin(), entry.lampMeshes.end(), mesh) == entry.lampMeshes.end())
                        entry.lampMeshes.push_back(mesh);
    }

    static void ApplyPlacementVariations()
    {
        size_t added = 0;

        for (ContainerContents& cc : pendingLinks)
        {
            auto at = placementVariations.find(cc.blueprint);
            if (at == placementVariations.end())
                continue;

            std::vector<MeshVariationRef> extra;
            for (const MeshVariationRef& mesh : cc.meshes)
            {
                // leave it alone.if applied itself
                if (uint32_t(mesh.key & 0xFFFFFFFFull) != 0)
                    continue;

                for (uint32_t varHash : at->second)
                {
                    MeshVariationRef ref;
                    ref.key = uint64_t(varHash) | (mesh.key & 0xFFFFFFFF00000000ull);
                    ref.name = mesh.name;

                    bool have = false;
                    for (const MeshVariationRef& e : cc.meshes)
                        if (e == ref) { have = true; break; }
                    for (const MeshVariationRef& e : extra)
                        if (e == ref) { have = true; break; }

                    if (!have)
                        extra.push_back(std::move(ref));
                }
            }

            added += extra.size();
            cc.meshes.insert(cc.meshes.end(),
                std::make_move_iterator(extra.begin()), std::make_move_iterator(extra.end()));
        }

        logger::info("[lights] {} blueprint placement(s) carry a variation; {} extra mesh key(s)",
            placementVariations.size(), added);
    }

    static void ResolvePendingLinks()
    {
        ApplyPlacementVariations();

        linkIndex.clear();
        for (uint32_t i = 0; i < pendingLinks.size(); ++i)
            for (const Placement& light : pendingLinks[i].lights)
                if (light.data)
                    linkIndex[static_cast<fb::LocalLightEntityData*>(light.data)].push_back(i);

        size_t linked = 0;
        for (auto& [data, entry] : entries)
        {
            if (linkIndex.count(data))
            {
                linkLampContents(data, entry);
                ++linked;
            }
        }

        logger::info("[lights] {} container(s) index {} light(s); linked {} existing entry/entries",
            pendingLinks.size(), linkIndex.size(), linked);
    }
    template<typename Register>
    static void ForEachLightIn(fb::GameObjectData* el, int depth, Register&& reg)
    {
        if (!el || depth > 4)
            return;

        fb::ClassInfo* classInfo = fb::classOf(el);
        if (!classInfo)
            return;

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::LocalLightEntityData::ClassInfoPtr()))
        {
            reg(static_cast<fb::LocalLightEntityData*>(el));
            return;
        }

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::LightComponentData::ClassInfoPtr()))
        {
            if (auto* light = static_cast<fb::LightComponentData*>(el)->m_Light)
                reg(light);
        // components can nest
        }

#if defined(BFVE_GAME_BF4)
        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::LightEffectEntityData::ClassInfoPtr()))
        {
            if (auto* light = static_cast<fb::LightEffectEntityData*>(el)->m_Light)
                reg(light);
        // falls through, it is an EffectEntityData too
        }
#endif

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::EffectEntityData::ClassInfoPtr()))
        {
            for (auto* child : static_cast<fb::EffectEntityData*>(el)->m_Components)
                ForEachLightIn(child, depth + 1, reg);
            return;
        }

        if (classInfo->isSubclassOf((fb::ClassInfo*)fb::PartComponentData::ClassInfoPtr()))
        {
            for (fb::HealthStateData* state : static_cast<fb::PartComponentData*>(el)->m_HealthStates)
                if (state)
                    for (auto* child : state->m_Objects)
                        ForEachLightIn(child, depth + 1, reg);
        }

        if (fb::Array<fb::GameObjectData*>* comps = componentsOf(classInfo, el))
        {
            for (auto* child : *comps)
                ForEachLightIn(child, depth + 1, reg);
        }
    }

    template<typename ContainerType>
    static void ProcessLightContainer(ContainerType* container, const char* containerTypeName)
    {
        if (!container) return;

        std::string assetName = container->m_Name ? container->m_Name : "(unnamed)";
        for (auto el : container->m_Objects)
        {
            fb::ClassInfo* elClass = fb::classOf(el);
            if (!elClass)
                continue;
#if defined(BFVE_GAME_BF3)
            // this happens only in bf3
            if (elClass->isSubclassOf((fb::ClassInfo*)fb::StaticModelGroupEntityData::ClassInfoPtr()))
                g_liveGroupData.insert(el);
#endif

            ForEachLightIn(el, 0, [&](fb::LocalLightEntityData* light)
            {
                RegisterLightData(light, assetName, containerTypeName, container);
            });
        }


        RecordPlacementVariations(container);
        CollectLampMeshes(container, containerTypeName);

        if (!containerIsProp(containerTypeName))
        {
            fb::LinearTransform identity{ };
            identity.m_right.m_x = 1.0f;
            identity.m_up.m_y = 1.0f;
            identity.m_forward.m_z = 1.0f;
            for (auto el : container->m_Objects)
                CollectWorldPlacements(el, identity, 0, 0);
        }
    }

    static void ProcessObjectBlueprint(fb::ObjectBlueprint* objBp, const char* containerTypeName = "ObjectBlueprint")
    {
        if (!objBp || !objBp->m_Object)
            return;

        const std::string assetName = objBp->m_Name ? objBp->m_Name : "(unnamed)";

        ForEachLightIn(objBp->m_Object, 0, [&](fb::LocalLightEntityData* light)
        {
            RegisterLightData(light, assetName, containerTypeName, objBp);
        });
    }

#if defined(BFVE_GAME_BF3)
    void onPartitionLoaded(void* partition)
    {
        auto* part = static_cast<fb::InternalDatabasePartition*>(partition);
        const char* name = part->m_name ? part->m_name : "(null name)";
        for (fb::DataContainer* obj : part->m_instances)
        {
            fb::ClassInfo* ci = fb::classOf(obj);
            if (!ci || !ci->isSubclassOf((fb::ClassInfo*)fb::LocalLightEntityData::ClassInfoPtr()))
                continue;
            const char* type = ci->m_InfoData && ci->m_InfoData->m_Name ? ci->m_InfoData->m_Name : "LocalLightEntityData";
            logger::info("[lights] partition {}: {} {}", name, type, static_cast<const void*>(obj));
        }
    }

    static void ScanPartitions(fb::ResourceManager* rm)
    {
        size_t domains = 0, partitions = 0, instances = 0, lights = 0, named = 0;
        for (fb::ResourceManager::Compartment* comp : rm->m_compartments)
        {
            if (!comp || !comp->m_domain)
                continue;
            ++domains;
            for (fb::InternalDatabasePartition* part : comp->m_domain->m_partitions)
            {
                if (!part)
                    continue;
                ++partitions;
                const char* name = part->m_name ? part->m_name : "(unnamed partition)";
                for (fb::DataContainer* obj : part->m_instances)
                {
                    if (!obj)
                        continue;
                    ++instances;
                    fb::ClassInfo* ci = fb::classOf(obj);
                    if (!ci)
                        continue;
                    if (ci->isSubclassOf((fb::ClassInfo*)fb::StaticModelGroupEntityData::ClassInfoPtr()))
                        g_liveGroupData.insert(obj);
                    if (!ci->isSubclassOf((fb::ClassInfo*)fb::LocalLightEntityData::ClassInfoPtr()))
                        continue;
                    ++lights;
                    auto* light = static_cast<fb::LocalLightEntityData*>(obj);
                    const auto it = entries.find(light);
                    const bool had = it != entries.end() && !isUnresolvedName(it->second.assetName);
                    RegisterLightData(light, name, "Partition", part);
                    if (!had)
                        ++named;
                }
            }
        }
        logger::info("[lights] partition walk: {} domains, {} partitions, {} instances, {} light data, {} named by it",
                     domains, partitions, instances, lights, named);
    }
#endif

    static uint64_t resourceFingerprint()
    {
        fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
        if (!rm)
            return 0;
        uint64_t f = 0;
        for (const auto comp : rm->m_compartments)
            if (comp)
                f = f * 1315423911ull + 1 + comp->m_objects.size();
        return f;
    }
    static uint64_t lastScanFingerprint = 0;

    void rescanIfResourcesGrew()
    {
        static uint32_t calls = 0;
        if (!scanned || (++calls % 120) != 0)
            return;

        const uint64_t f = resourceFingerprint();
        if (f == lastScanFingerprint)
            return;
        static uint32_t lastAuto = 0; // just dont tank performance...
        if (calls - lastAuto < 1800)
            return;
        lastAuto = calls;

        logger::info("[lights] resources changed since the last scan - rescanning");
        scanAll();
        scanExistingEntities();
    }

    void scanAll()
    {
        fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
        if (!rm)
            return;

        clearLampLinks();
        g_registeredBlueprints.clear();
#if defined(BFVE_GAME_BF3)
        g_liveGroupData.clear();
#endif

        logger::debug("Scanning ResourceManager for light data...");

        for (const auto comp : rm->m_compartments)
        {
            if (!comp) continue;

            for (const auto obj : comp->m_objects)
            {
                fb::ClassInfo* classInfo = fb::classOf(obj);
                if (!classInfo)
                    continue;
                const char* typeName = classInfo->m_InfoData && classInfo->m_InfoData->m_Name
                    ? classInfo->m_InfoData->m_Name : "Blueprint";
#if defined(BFVE_GAME_BF3)
                if (classInfo->isSubclassOf((fb::ClassInfo*)fb::StaticModelGroupEntityData::ClassInfoPtr()))
                    g_liveGroupData.insert(obj);
#endif

                if (classInfo->isSubclassOf((fb::ClassInfo*)fb::PrefabBlueprint::ClassInfoPtr()))
                    ProcessLightContainer(reinterpret_cast<fb::PrefabBlueprint*>(obj), typeName);
                else if (classInfo->isSubclassOf((fb::ClassInfo*)fb::ObjectBlueprint::ClassInfoPtr()))
                    ProcessObjectBlueprint(reinterpret_cast<fb::ObjectBlueprint*>(obj), typeName);
            }
        }

#if defined(BFVE_GAME_BF3)
        ScanPartitions(rm);
#endif
        CollectGroupInstances();
        BuildNearbyMeshes();
        ResolvePendingLinks();


        scanned = true;
        lastScanFingerprint = resourceFingerprint();
        size_t unresolved = 0;
        for (const auto& [ptr, entry] : entries)
            if (isUnresolvedName(entry.assetName))
                ++unresolved;

        logger::info("Scan complete: {} light data entries, {} unresolved", entries.size(), unresolved);
    }

    void scanExistingEntities()
    {
#if defined(BFVE_GAME_BF4)
        // sub_140CC7EE0 ctor uses sub_1407DAFC0(this+0x30, this, 1)
        fb::EntityList<fb::LocalLightEntity> lights{
            (fb::ClassInfo*)fb::LocalLightEntity::ClassInfoPtr(), offsetof(fb::LocalLightEntity, m_link) };

        size_t dispatched = 0;
        fb::LocalLightEntity* light;
        while ((light = lights.nextOfKind()) != nullptr)
        {
            fb::LocalLightEntityData* data = entityData(light);
            if (!data)
                continue;

            onEntityCreated(light, data);
            ++dispatched;
        }

        size_t totalActive = 0;
        for (const auto& [ptr, entry] : entries)
            totalActive += entry.activeEntities.size();

        logger::info("[lights::scanExistingEntities] dispatched={} active={}",
            dispatched, totalActive);

#elif defined(BFVE_GAME_BF3)
        fb::EntityList<fb::LocalLightEntity> lights{ (fb::ClassInfo*)fb::LocalLightEntity::ClassInfoPtr() };
        size_t dispatched = 0;
        fb::LocalLightEntity* light;
        while ((light = lights.nextOfKind()) != nullptr)
        {
            fb::LocalLightEntityData* data = entityData(light);
            if (!data)
                continue;

            onEntityCreated(light, data);
            ++dispatched;
        }
        size_t totalActive = 0;
        for (const auto& [ptr, entry] : entries)
            totalActive += entry.activeEntities.size();

        logger::debug("[lights::scanExistingEntities] dispatched={} active={}",
			dispatched, totalActive);

        linkFlaresByProximity();
#endif
    }

    void refreshEntities()
    {
#if defined(BFVE_GAME_BF3)
        size_t iterable = 0, walked = 0, unknown = 0;
        {
            fb::EntityList<fb::LocalLightEntity> lights{ (fb::ClassInfo*)fb::LocalLightEntity::ClassInfoPtr() };
            while (lights.nextOfKind() != nullptr)
                ++iterable;
        }
        {
            fb::EntityList<fb::LocalLightEntity> lights{ (fb::ClassInfo*)fb::LocalLightEntity::ClassInfoPtr(), false };
            fb::LocalLightEntity* light;
            while ((light = lights.nextOfKind()) != nullptr)
            {
                ++walked;
                fb::LocalLightEntityData* data = entityData(light);
                if (!data)
                    continue;
                if (!entries.count(data))
                {
                    ++unknown;
                    continue;
                }
                onEntityCreated(light, data);
            }
        }
        static size_t lastWalked = SIZE_MAX, lastIterable = SIZE_MAX;
        if (walked != lastWalked || iterable != lastIterable)
        {
            size_t active = 0;
            for (const auto& [ptr, entry] : entries)
                active += entry.activeEntities.size();
            logger::info("[lights] entity walk: {} light entities ({} iterable, {} with data the scan does not know), {} active in {} entries",
                         walked, iterable, unknown, active, entries.size());
            lastWalked = walked;
            lastIterable = iterable;
        }
#endif
    }

    void nameDynamicLightsByEmitters()
    {
#if defined(BFVE_GAME_BF3)
        std::unordered_map<void*, std::string> owners;
        emitters::lightOwners(owners);
#endif
        for (auto& [dataPtr, entry] : entries)
        {
            if (!isUnresolvedName(entry.assetName) || entry.activeEntities.empty())
                continue;
#if defined(BFVE_GAME_BF3)
            {
                const std::string* owner = nullptr;
                for (fb::LocalLightEntity* e : entry.activeEntities)
                    if (auto it = owners.find(e); it != owners.end())
                    {
                        owner = &it->second;
                        break;
                    }
                if (owner)
                {
                    entry.assetName = *owner;
                    entry.containerType = "Emitter";
                    logger::info("[lights] runtime light {} is the point light of emitter {}", static_cast<const void*>(dataPtr), *owner);
                    continue;
                }
            }
#endif
            fb::Vec3 pos{ };
            if (!entryWorldPos(entry, pos))
                continue;

            const WorldLight* placed = nullptr;
            float best = 0.75f * 0.75f;
            for (const WorldLight& wl : worldLights)
            {
                if (wl.data == dataPtr)
                    continue;
                const float dx = wl.pos.m_x - pos.m_x, dy = wl.pos.m_y - pos.m_y, dz = wl.pos.m_z - pos.m_z;
                const float d = dx * dx + dy * dy + dz * dz;
                if (d < best)
                {
                    best = d;
                    placed = &wl;
                }
            }
            if (placed)
            {
                auto src = entries.find(placed->data);
                if (src != entries.end() && !isUnresolvedName(src->second.assetName))
                {
                    entry.assetName = src->second.assetName;
                    entry.containerType = src->second.containerType;
                    logger::info("[lights] runtime light {} named after the placement at its position: {}",
                                 static_cast<const void*>(dataPtr), entry.assetName);
                    continue;
                }
            }

            static std::unordered_set<const void*> reported;
            if (reported.insert(dataPtr).second)
                logger::info("[lights] unresolved light {} at ({:.1f} {:.1f} {:.1f}): no placement and no emitter owns it",
                             static_cast<const void*>(dataPtr), pos.m_x, pos.m_y, pos.m_z);
        }
    }

    void scanAndApplyOverrides()
    {
        scanAll();
        scanExistingEntities();
        {
            for (auto& [dataPtr, entry] : entries)
            {
                if (entry.hasOverride && !entry.activeEntities.empty())
                    applyOverride(entry);
            }
        }
        logger::debug("ScanAndApplyOverrides complete");
    }

    void captureOriginal(LightDataEntry& entry)
    {
        if (!entry.dataPtr || entry.origCaptured)
            return;

        auto data = entry.dataPtr;

        entry.origColor = data->m_Color;
        entry.origParticleColorScale = data->m_ParticleColorScale;
        entry.origEnlightenColorScale = data->m_EnlightenColorScale;
        entry.origRadius = data->m_Radius;
        entry.origIntensity = data->m_Intensity;
        entry.origAttenuationOffset = data->m_AttenuationOffset;
        entry.origEnlightenColorMode = static_cast<int>(data->m_EnlightenColorMode);
        entry.origVisible = data->m_Visible;
        entry.origSpecularEnable = data->m_SpecularEnable;
        entry.origEnlightenEnable = data->m_EnlightenEnable;

        entry.color = entry.origColor;
        entry.particleColorScale = entry.origParticleColorScale;
        entry.enlightenColorScale = entry.origEnlightenColorScale;
        entry.radius = entry.origRadius;
        entry.intensity = entry.origIntensity;
        entry.attenuationOffset = entry.origAttenuationOffset;
        entry.enlightenColorMode = entry.origEnlightenColorMode;
        entry.visible = entry.origVisible;
        entry.specularEnable = entry.origSpecularEnable;
        entry.enlightenEnable = entry.origEnlightenEnable;

        if (entry.isSpotLight)
        {
            auto spotData = static_cast<fb::SpotLightEntityData*>(data);
            entry.origSpotShape = static_cast<int>(spotData->m_Shape);
            entry.origConeInnerAngle = spotData->m_ConeInnerAngle;
            entry.origConeOuterAngle = spotData->m_ConeOuterAngle;
            entry.origFrustumFov = spotData->m_FrustumFov;
            entry.origFrustumAspect = spotData->m_FrustumAspect;
            entry.origOrthoWidth = spotData->m_OrthoWidth;
            entry.origOrthoHeight = spotData->m_OrthoHeight;
            entry.origCastShadowsEnable = spotData->m_CastShadowsEnable;
            entry.origCastShadowsMinLevel = static_cast<int>(spotData->m_CastShadowsMinLevel);

            entry.spotShape = entry.origSpotShape;
            entry.coneInnerAngle = entry.origConeInnerAngle;
            entry.coneOuterAngle = entry.origConeOuterAngle;
            entry.frustumFov = entry.origFrustumFov;
            entry.frustumAspect = entry.origFrustumAspect;
            entry.orthoWidth = entry.origOrthoWidth;
            entry.orthoHeight = entry.origOrthoHeight;
            entry.castShadowsEnable = entry.origCastShadowsEnable;
            entry.castShadowsMinLevel = entry.origCastShadowsMinLevel;
        }

        if (entry.isPointLight)
        {
            auto pointData = static_cast<fb::PointLightEntityData*>(data);
            entry.origPointWidth = pointData->m_Width;
            entry.origTranslucencyAmbient = pointData->m_TranslucencyAmbient;
            entry.origTranslucencyScale = pointData->m_TranslucencyScale;
            entry.origTranslucencyPower = pointData->m_TranslucencyPower;
            entry.origTranslucencyDistortion = pointData->m_TranslucencyDistortion;

            entry.pointWidth = entry.origPointWidth;
            entry.translucencyAmbient = entry.origTranslucencyAmbient;
            entry.translucencyScale = entry.origTranslucencyScale;
            entry.translucencyPower = entry.origTranslucencyPower;
            entry.translucencyDistortion = entry.origTranslucencyDistortion;
        }

        entry.origCaptured = true;
    }

    void applyToData(fb::LocalLightEntityData* data, const LightDataEntry& entry)
    {
        if (!data || !entry.hasOverride)
            return;

        data->m_Color = entry.color;
        data->m_ParticleColorScale = entry.particleColorScale;
        data->m_EnlightenColorScale = entry.enlightenColorScale;
        data->m_Radius = entry.radius;
        data->m_Intensity = entry.intensity;
        data->m_AttenuationOffset = entry.attenuationOffset;
        data->m_EnlightenColorMode = static_cast<fb::EnlightenColorMode>(entry.enlightenColorMode);
        data->m_Visible = entry.visible;
        data->m_SpecularEnable = entry.specularEnable;
        data->m_EnlightenEnable = entry.enlightenEnable;

        if (entry.isSpotLight)
        {
            auto spotData = static_cast<fb::SpotLightEntityData*>(data);
            spotData->m_Shape = static_cast<fb::SpotLightShape>(entry.spotShape);
            spotData->m_ConeInnerAngle = entry.coneInnerAngle;
            spotData->m_ConeOuterAngle = entry.coneOuterAngle;
            spotData->m_FrustumFov = entry.frustumFov;
            spotData->m_FrustumAspect = entry.frustumAspect;
            spotData->m_OrthoWidth = entry.orthoWidth;
            spotData->m_OrthoHeight = entry.orthoHeight;
            spotData->m_CastShadowsEnable = entry.castShadowsEnable;
            spotData->m_CastShadowsMinLevel = static_cast<fb::QualityLevel>(entry.castShadowsMinLevel);
        }
        else if (entry.isPointLight)
        {
            auto pointData = static_cast<fb::PointLightEntityData*>(data);
            pointData->m_Width = entry.pointWidth;
            pointData->m_TranslucencyAmbient = entry.translucencyAmbient;
            pointData->m_TranslucencyScale = entry.translucencyScale;
            pointData->m_TranslucencyPower = entry.translucencyPower;
            pointData->m_TranslucencyDistortion = entry.translucencyDistortion;
        }
    }

    std::vector<MeshMaterialInfo> meshAssetMaterials(void* meshAsset)
    {
        std::vector<MeshMaterialInfo> out;
        auto* asset = static_cast<fb::MeshAsset*>(meshAsset);
        if (!asset || !asset->m_NameHash || !asset->m_Materials.m_firstElement || asset->m_Materials.size() > 64)
            return out;

        for (fb::MeshMaterial* mm : asset->m_Materials)
        {
            if (!mm)
                continue;

            MeshMaterialInfo info;
            char text[256];

            if (mm->m_Shader.m_Shader && copyEngineString(mm->m_Shader.m_Shader->m_Name, text, sizeof(text)))
                info.shader = text;

            if (mm->m_Shader.m_VectorParameters.m_firstElement && mm->m_Shader.m_VectorParameters.size() <= 64)
                for (const fb::VectorShaderParameter& p : mm->m_Shader.m_VectorParameters)
                    if (copyEngineString(p.m_ParameterName, text, sizeof(text)))
                        info.parameters.push_back(text);

            if (mm->m_Shader.m_TextureParameters.m_firstElement && mm->m_Shader.m_TextureParameters.size() <= 64)
            {
                for (const fb::TextureShaderParameter& p : mm->m_Shader.m_TextureParameters)
                {
                    MeshMaterialInfo::TextureBinding tb;
                    tb.asset = p.m_Value;
                    if (copyEngineString(p.m_ParameterName, text, sizeof(text)))
                        tb.parameter = text;
                    if (p.m_Value && copyEngineString(p.m_Value->m_Name, text, sizeof(text)))
                        tb.texture = text;
                    info.textures.push_back(std::move(tb));
                }
            }

            out.push_back(std::move(info));
        }

        return out;
    }

    namespace
    {
        struct ClaimedParam { uint32_t handle; float value[4]; };
        std::mutex g_claimMutex;
        std::vector<ClaimedParam> g_claimed;
        std::atomic<uint32_t> g_claimCount{ 0 };
    }

    bool shaderParamOverride(uint32_t handle, float (&out)[4])
    {
        if (g_claimCount.load(std::memory_order_relaxed) == 0)
            return false;

        std::lock_guard<std::mutex> lock(g_claimMutex);
        for (const ClaimedParam& c : g_claimed)
            if (c.handle == handle)
            {
                out[0] = c.value[0]; out[1] = c.value[1];
                out[2] = c.value[2]; out[3] = c.value[3];
                return true;
            }
        return false;
    }

    void setShaderParamOverride(uint32_t handle, const float (&value)[4])
    {
        std::lock_guard<std::mutex> lock(g_claimMutex);
        for (ClaimedParam& c : g_claimed)
            if (c.handle == handle)
            {
                c.value[0] = value[0]; c.value[1] = value[1];
                c.value[2] = value[2]; c.value[3] = value[3];
                return;
            }

        g_claimed.push_back({ handle, { value[0], value[1], value[2], value[3] } });
        g_claimCount.store(uint32_t(g_claimed.size()), std::memory_order_relaxed);
    }

    void clearShaderParamOverride(uint32_t handle)
    {
        std::lock_guard<std::mutex> lock(g_claimMutex);
        for (size_t i = 0; i < g_claimed.size(); ++i)
            if (g_claimed[i].handle == handle)
            {
                g_claimed.erase(g_claimed.begin() + ptrdiff_t(i));
                g_claimCount.store(uint32_t(g_claimed.size()), std::memory_order_relaxed);
                return;
            }
    }

    bool hasShaderParamOverride(uint32_t handle)
    {
        float ignored[4];
        return shaderParamOverride(handle, ignored);
    }

    bool readShaderDriver(void* data, uint32_t& handle, float (&value)[4])
    {
        if (!data)
            return false;
#if defined(BFVE_GAME_BF4)
        auto* driver = static_cast<fb::ShaderParameterEntityData*>(data);
        handle = driver->m_ParameterHandle;
        const fb::Vec4& v = driver->m_VecParam;
#else
        auto* driver = static_cast<fb::ShaderParameterComponentData*>(data);
        if (driver->m_ShaderParameterVectors.size() == 0)
            return false;
        fb::ShaderParameterVector& sv = driver->m_ShaderParameterVectors.At(0);
        handle = textures::paramHandle(sv.m_ParameterName);
        const fb::Vec4& v = sv.m_Value;
#endif
        value[0] = v.m_x; value[1] = v.m_y; value[2] = v.m_z; value[3] = v.m_w;
        return true;
    }

    bool writeShaderDriver(void* data, const float (&value)[4])
    {
        if (!data)
            return false;
#if defined(BFVE_GAME_BF4)
        fb::Vec4& v = static_cast<fb::ShaderParameterEntityData*>(data)->m_VecParam;
#else
        fb::ShaderParameterComponentData* driver = static_cast<fb::ShaderParameterComponentData*>(data);
        if (driver->m_ShaderParameterVectors.size() == 0)
            return false;
        fb::Vec4& v = driver->m_ShaderParameterVectors.At(0).m_Value;
#endif
        v.m_x = value[0]; v.m_y = value[1]; v.m_z = value[2]; v.m_w = value[3];
        return true;
    }

    uint32_t applyShaderDrivers(LightDataEntry& entry)
    {
        if (entry.shaderDrivers.empty() || entry.lampShaderParams.empty())
            return 0;

        uint32_t written = 0;

        for (const LightDataEntry::ShaderDriverEdit& drv : entry.shaderDrivers)
        {
            for (void* data : entry.lampShaderParams)
            {
                uint32_t handle = 0;
                float current[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                if (!readShaderDriver(data, handle, current) || handle != drv.handle)
                    continue;

                if (writeShaderDriver(data, drv.value))
                    ++written;
            }

            if (written || !entry.lampShaderParams.empty())
                setShaderParamOverride(drv.handle, drv.value);
        }

        return written;
    }

    void setShaderDriverFor(LightDataEntry& entry, uint32_t handle, const float (&value)[4])
    {
        auto it = std::find_if(entry.shaderDrivers.begin(), entry.shaderDrivers.end(),
            [handle](const LightDataEntry::ShaderDriverEdit& e) { return e.handle == handle; });

        if (it == entry.shaderDrivers.end())
        {
            entry.shaderDrivers.push_back({ handle, { value[0], value[1], value[2], value[3] } });
        }
        else
        {
            for (int i = 0; i < 4; ++i)
                it->value[i] = value[i];
        }

        applyShaderDrivers(entry);
        entry.shaderDriversApplied = true;
    }

    static void clearLevelPointers()
    {
        {
            std::lock_guard<std::mutex> lock(g_claimMutex);
            g_claimed.clear();
            g_claimCount.store(0, std::memory_order_relaxed);
        }

        clearFlarePointers();
        clearOverlayState();
    }

    bool entityPosOf(fb::LocalLightEntity* e, fb::Vec3& out) { return entityPos(e, out); }

    void applyOverride(LightDataEntry& entry)
    {
        if (!entry.dataPtr)
            return;

        applyToData(entry.dataPtr, entry);

        for (auto entity : entry.activeEntities)
        {
            if (!liveLightEntity(entity, entry))
                continue;

            entity->setDirty();
        }
    }

    void resetAll()
    {
        for (auto& [dataPtr, entry] : entries)
        {
            if (entry.origCaptured && entry.hasOverride)
            {
                entry.ResetToOriginal();
                applyOverride(entry);
            }
        }
    }

    void onEntityCreated(fb::LocalLightEntity* entity, fb::LocalLightEntityData* data)
    {
        if (!entity || !data)
            return;

        auto it = entries.find(data);
        if (it == entries.end())
        {
            LightDataEntry& entry = entries[data];
            entry.dataPtr = data;
            entry.assetName = "(dynamic)";
            entry.containerType = "Runtime";

            if (fb::ClassInfo* classInfo = fb::classOf(entity))
            {
                if (classInfo->m_ClassId == fb::SpotLightEntity::ClassId())
                {
                    entry.isSpotLight = true;
                    entry.lightType = "SpotLight";
                }
                else if (classInfo->m_ClassId == fb::PointLightEntity::ClassId())
                {
                    entry.isPointLight = true;
                    entry.lightType = "PointLight";
                }
                else
                {
                    entry.lightType = "LocalLight";
                }
            }
            it = entries.find(data);

            linkLampContents(data, it->second);
        }

        LightDataEntry& entry = it->second;
        entry.activeEntities.insert(entity);

        if (!entry.origCaptured)
            captureOriginal(entry);

        if (entry.hasOverride)
        {
            applyToData(data, entry);

            if (liveLightEntity(entity, entry))
                entity->setDirty();
        }
    }

    void onEntityDestroyed(fb::LocalLightEntity* entity)
    {
        if (!entity)
            return;

        for (auto& [dataPtr, entry] : entries)
        {
            if (entry.activeEntities.erase(entity))
                break;
        }
    }

    void forgetData(fb::LocalLightEntityData* data)
    {
        entries.erase(data);
        nearbyMeshes.erase(data);
        linkIndex.erase(data);
    }

    fb::LocalLightEntity* closestLightToCrosshair(float* outScreenDist)
    {
        const ImVec2 disp = ImGui::GetIO().DisplaySize;
        const ImVec2 center{ disp.x * 0.5f, disp.y * 0.5f };

        fb::LocalLightEntity* best = nullptr;
        float bestSqr = FLT_MAX;

        for (auto& [dataPtr, entry] : entries)
        {
            for (fb::LocalLightEntity* e : entry.activeEntities)
            {
                fb::Vec3 pos{ };
                if (!liveLightEntity(e, entry) || !entityPos(e, pos))
                    continue;

                ImVec2 sp;
                if (!render::worldToScreen(pos, sp))
                    continue;

                const float dx = sp.x - center.x;
                const float dy = sp.y - center.y;
                const float sqr = dx * dx + dy * dy;
                if (sqr < bestSqr)
                {
                    bestSqr = sqr;
                    best = e;
                }
            }
        }

        if (best && outScreenDist)
            *outScreenDist = std::sqrt(bestSqr);
        return best;
    }

    namespace
    {
        PlacedMesh g_aimedMesh;
        bool g_haveAimedMesh = false;
    }

    bool aimedPlacedMesh(PlacedMesh& out)
    {
        out = g_aimedMesh;
        return g_haveAimedMesh;
    }

    size_t placedMeshCount() { return worldMeshes.size(); }

    static float boxDistance(const WorldMesh& m, const fb::Vec3& p)
    {
        const fb::Vec3 d{ p.m_x - m.frame.m_trans.m_x, p.m_y - m.frame.m_trans.m_y, p.m_z - m.frame.m_trans.m_z };
        const auto axis = [&](const fb::Vec3& r) -> float
        {
            const float len2 = r.m_x * r.m_x + r.m_y * r.m_y + r.m_z * r.m_z;
            return len2 > 1e-8f ? (d.m_x * r.m_x + d.m_y * r.m_y + d.m_z * r.m_z) / len2 : 0.0f;
        };
        const float l[3] = { axis(m.frame.m_right), axis(m.frame.m_up), axis(m.frame.m_forward) };
        const float mn[3] = { m.boxMin.m_x, m.boxMin.m_y, m.boxMin.m_z };
        const float mx[3] = { m.boxMax.m_x, m.boxMax.m_y, m.boxMax.m_z };
        float sum = 0.0f;
        for (int i = 0; i < 3; ++i)
        {
            const float o = l[i] < mn[i] ? mn[i] - l[i] : (l[i] > mx[i] ? l[i] - mx[i] : 0.0f);
            sum += o * o;
        }
        return std::sqrt(sum);
    }

    static bool rayHitsWorldMesh(const WorldMesh& m, const fb::Vec3& from, const fb::Vec3& dir,
                                 float maxT, float& tOut)
    {
        if (!m.hasBox)
            return false;
        const fb::Vec3 d{ from.m_x - m.frame.m_trans.m_x, from.m_y - m.frame.m_trans.m_y, from.m_z - m.frame.m_trans.m_z };
        const auto axis = [&](const fb::Vec3& r, const fb::Vec3& v) -> float
        {
            const float len2 = r.m_x * r.m_x + r.m_y * r.m_y + r.m_z * r.m_z;
            return len2 > 1e-8f ? (v.m_x * r.m_x + v.m_y * r.m_y + v.m_z * r.m_z) / len2 : 0.0f;
        };
        const float o[3] = { axis(m.frame.m_right, d), axis(m.frame.m_up, d), axis(m.frame.m_forward, d) };
        const float v[3] = { axis(m.frame.m_right, dir), axis(m.frame.m_up, dir), axis(m.frame.m_forward, dir) };
        const float mn[3] = { m.boxMin.m_x, m.boxMin.m_y, m.boxMin.m_z };
        const float mx[3] = { m.boxMax.m_x, m.boxMax.m_y, m.boxMax.m_z };
        float t0 = -FLT_MAX, t1 = maxT;
        for (int i = 0; i < 3; ++i)
        {
            if (std::fabs(v[i]) < 1e-8f)
            {
                if (o[i] < mn[i] || o[i] > mx[i])
                    return false;
                continue;
            }
            float a = (mn[i] - o[i]) / v[i], b = (mx[i] - o[i]) / v[i];
            if (a > b) std::swap(a, b);
            if (a > t0) t0 = a;
            if (b < t1) t1 = b;
            if (t0 > t1)
                return false;
        }
        if (t1 <= 0.0f)
            return false; // entirely behind
        tOut = t0 > 0.0f ? t0 : t1;
        return true;
    }

    bool placedMeshAt(const fb::Vec3& point, float slack, PlacedMesh& out)
    {
        const WorldMesh* best = nullptr;
        float bestVolume = FLT_MAX;
        for (const WorldMesh& m : worldMeshes)
        {
            float volume = 0.0f;
            if (insideWorldMesh(m, point, volume) && volume < bestVolume)
            {
                bestVolume = volume;
                best = &m;
            }
        }
        if (!best)
        {
            float bestD = slack;
            for (const WorldMesh& m : worldMeshes)
            {
                if (!m.hasBox)
                    continue;
                const float d = boxDistance(m, point);
                if (d < bestD)
                {
                    bestD = d;
                    best = &m;
                }
            }
        }
        if (!best)
            return false;
        out = { best->mesh, best->varHash, best->pos };
        return true;
    }

    namespace
    {
        std::mutex g_aimMutex;
        AimRay g_aim;

        struct VisEntry { fb::Vec3 pos; bool visible; uint32_t tick; bool pending; };
        std::mutex g_visMutex;
        std::unordered_map<uint64_t, VisEntry> g_vis;
        uint32_t g_visTick = 0;

        uint64_t visKey(const fb::Vec3& p)
        {
            const int32_t x = int32_t(std::floor(p.m_x * 4.0f)), y = int32_t(std::floor(p.m_y * 4.0f)), z = int32_t(std::floor(p.m_z * 4.0f));
            return (uint64_t(uint32_t(x)) << 42) ^ (uint64_t(uint32_t(y)) << 21) ^ uint64_t(uint32_t(z)) ^ (uint64_t(uint32_t(x)) * 0x9E3779B97F4A7C15ull);
        }
    }

    bool overlayVisible(const fb::Vec3& pos)
    {
        if (!overlayOcclusion)
            return true;
        std::lock_guard<std::mutex> lock(g_visMutex);
        const uint64_t key = visKey(pos);
        auto it = g_vis.find(key);
        if (it == g_vis.end())
        {
            if (g_vis.size() < 4096)
                g_vis.emplace(key, VisEntry{ pos, true, 0, true });
            return true;
        }
        return it->second.visible;
    }

    // again dont tank the performance...
    static void tickOcclusion(const fb::Vec3& cam, const fb::Vec3& dir, void* me)
    {
        std::vector<std::pair<uint64_t, fb::Vec3>> batch;
        {
            std::lock_guard<std::mutex> lock(g_visMutex);
            ++g_visTick;
            if (!overlayOcclusion || g_vis.empty())
            {
                if (!overlayOcclusion)
                    g_vis.clear();
                return;
            }
            for (auto& [k, e] : g_vis)
                if (e.pending && batch.size() < 48)
                    batch.emplace_back(k, e.pos);
            if (batch.size() < 48)
            {
                std::vector<std::pair<uint32_t, uint64_t>> stale;
                for (auto& [k, e] : g_vis)
                    if (!e.pending && g_visTick - e.tick > 20)
                        stale.emplace_back(e.tick, k);
                std::sort(stale.begin(), stale.end());
                for (auto& [t, k] : stale)
                {
                    if (batch.size() >= 48) break;
                    batch.emplace_back(k, g_vis[k].pos);
                }
            }
            if (g_vis.size() > 2048)
                for (auto it = g_vis.begin(); it != g_vis.end();)
                    it = (g_visTick - it->second.tick > 600) ? g_vis.erase(it) : std::next(it);
        }

        const fb::Vec3 start{ cam.m_x + dir.m_x * 1.0f, cam.m_y + dir.m_y * 1.0f, cam.m_z + dir.m_z * 1.0f };
        for (auto& [k, p] : batch)
        {
            fb::RayCastHit hit{ };
            const bool blocked = fb::physicsRayQuery(start, p, hit, fb::RAY_CAST_WORLD_ONLY, me);

            const bool visible = !blocked || hit.m_lambda > 0.97f;
            std::lock_guard<std::mutex> lock(g_visMutex);
            if (auto it = g_vis.find(k); it != g_vis.end())
            {
                it->second.visible = visible;
                it->second.tick = g_visTick;
                it->second.pending = false;
            }
        }
    }

    void staticMeshBodies(std::vector<void*>& out)
    {
        out.clear();
#if defined(BFVE_GAME_BF4)
        std::vector<void*> heads;
        collectHeads(fb::ClientStaticModelGroupEntity::ClassInfoPtr(), uint16_t(fb::ClientStaticModelGroupEntity::ClassId()), heads);
        for (void* head : heads)
        {
            size_t seen = 0;
            for (void* node = head; node && ++seen <= 65536; )
            {
                auto* e = reinterpret_cast<fb::ClientStaticModelGroupEntity*>(static_cast<uint8_t*>(node) - fb::STATIC_MODEL_GROUP_LINK_OFFSET);
                const size_t memberCount = e->m_data ? e->m_data->m_MemberDatas.size() : 0;
                fb::StaticModelGroupMember* end = e->m_membersEnd;
                if (e->m_membersBegin && end > e->m_membersBegin + memberCount)
                    end = e->m_membersBegin + memberCount;
                for (fb::StaticModelGroupMember* rec = e->m_membersBegin; rec && rec < end; ++rec)
                    if (rec->m_physics) out.push_back(rec->m_physics);
                void* next = e->m_link.next;
                if (!next || next == head)
                    break;
                node = next;
            }
        }
#endif
        std::sort(out.begin(), out.end());
        out.erase(std::unique(out.begin(), out.end()), out.end());
    }

#if defined(BFVE_GAME_BF4)
    // hit = GroupPhysicsEntity + part, stored at member+0x20
    static bool resolveGroupHit(void* body, uint32_t part, const fb::Vec3& hitPos, PlacedMesh& out)
    {
        if (!body)
            return false;
        std::vector<void*> heads;
        collectHeads(fb::ClientStaticModelGroupEntity::ClassInfoPtr(),
                     uint16_t(fb::ClientStaticModelGroupEntity::ClassId()), heads);
        for (void* head : heads)
        {
            size_t seen = 0;
            for (void* node = head; node && ++seen <= 65536; )
            {
                auto* e = reinterpret_cast<fb::ClientStaticModelGroupEntity*>(
                    static_cast<uint8_t*>(node) - fb::STATIC_MODEL_GROUP_LINK_OFFSET);
                const size_t memberCount = e->m_data ? e->m_data->m_MemberDatas.size() : 0;
                fb::StaticModelGroupMember* end = e->m_membersEnd;
                if (e->m_membersBegin && end > e->m_membersBegin + memberCount)
                    end = e->m_membersBegin + memberCount;

                for (fb::StaticModelGroupMember* rec = e->m_membersBegin; rec && rec < end; ++rec)
                {
                    if (rec->m_physics != body)
                        continue;
                    fb::StaticModelGroupMemberData* mem = rec->m_data;
                    if (!mem || !mem->m_MeshAsset || !rec->m_instances || !mem->m_PhysicsPartCountPerInstance)
                        continue;
                    const uint32_t first = mem->m_PhysicsPartRange.m_First;
                    if (part < first)
                        continue;
                    const uint32_t i = (part - first) / mem->m_PhysicsPartCountPerInstance;
                    if (i >= mem->m_InstanceCount)
                        continue;

                    const uintptr_t vt = reinterpret_cast<uintptr_t>(rec->m_instances->m_vtable);
                    size_t stride = 0;
                    if (vt == fb::StaticModelGroupMeshInstance::RIGID_VTABLE)
                        stride = fb::StaticModelGroupMeshInstance::RIGID_SIZE;
                    else if (vt == fb::StaticModelGroupMeshInstance::COMPOSITE_VTABLE)
                        stride = fb::StaticModelGroupMeshInstance::COMPOSITE_SIZE;
                    const uint32_t nv = mem->m_InstanceObjectVariation.m_firstElement
                        ? mem->m_InstanceObjectVariation.size() : 0;
                    out.mesh = mem->m_MeshAsset;
                    out.varHash = i < nv ? mem->m_InstanceObjectVariation.At(int(i)) : 0;
                    out.pos = hitPos;
                    if (stride)
                    {
                        auto* inst = reinterpret_cast<fb::StaticModelGroupMeshInstance*>(
                            reinterpret_cast<uint8_t*>(rec->m_instances) + i * stride);
                        out.pos = inst->m_transform.m_trans;
                    }
                    else if (auto frameIt = g_groupFrames.find(e); frameIt != g_groupFrames.end())
                    {
                        out.pos = composeTransform(frameIt->second, memberLocal(rec, i)).m_trans;
                    }
                    return true;
                }

                void* next = e->m_link.next;
                if (!next || next == head)
                    break;
                node = next;
            }
        }
        return false;
    }
#else
    // BF3 hit body = member's StaticModelGroupPhysics
    static bool resolveGroupHit(void* body, uint32_t part, const fb::Vec3& hitPos, PlacedMesh& out)
    {
        auto it = body ? g_groupPhysics.find(body) : g_groupPhysics.end();
        if (it == g_groupPhysics.end())
            return false;
        auto* phys = static_cast<fb::StaticModelGroupPhysics*>(body);
        fb::StaticModelGroupMemberData* mem = it->second.member;
        if (!phys->m_partTable || !phys->m_partTableEnd)
            return false;
        const bool singleEntry = (phys->m_flags & 0x40000000u) != 0;
        const uint32_t entry = singleEntry ? 0u : part;
        if (phys->m_partTable + entry >= phys->m_partTableEnd)
            return false;
        fb::StaticModelGroupMeshInstance* inst = phys->m_partTable[entry].m_instance;
        if (!inst)
            return false;
        uint32_t i = 0;
        if (!singleEntry && mem->m_PhysicsPartCountPerInstance && part >= mem->m_PhysicsPartRange.m_First)
            i = (part - mem->m_PhysicsPartRange.m_First) / mem->m_PhysicsPartCountPerInstance;
        const uint32_t nv = mem->m_InstanceObjectVariation.m_firstElement
            ? mem->m_InstanceObjectVariation.size() : 0;
        const uintptr_t vt = reinterpret_cast<uintptr_t>(inst->m_vtable);
        out.mesh = it->second.mesh;
        out.varHash = i < nv ? mem->m_InstanceObjectVariation.At(int(i)) : 0;
        out.pos = (vt == fb::StaticModelGroupMeshInstance::RIGID_VTABLE ||
                   vt == fb::StaticModelGroupMeshInstance::COMPOSITE_VTABLE)
            ? inst->m_transform.m_trans : hitPos;
        return true;
    }
#endif

    static void clearOverlayState()
    {
        worldMeshes.clear();
        worldLights.clear();
        nearbyMeshes.clear();
        g_meshBoxes.clear();
        g_meshGeo.clear();
#if defined(BFVE_GAME_BF4)
        g_groupFrames.clear();
#else
        g_groupPhysics.clear();
#endif
        {
            std::lock_guard<std::mutex> lock(g_geoCopyMutex);
            g_geoCopyQueue.clear();
        }
        {
            std::lock_guard<std::mutex> lock(g_visMutex);
            g_vis.clear();
        }
        {
            std::lock_guard<std::mutex> lock(g_aimMutex);
            g_aim = AimRay{ };
        }
        g_haveAimedMesh = false;
    }

    bool lastAimRay(AimRay& out)
    {
        std::lock_guard<std::mutex> lock(g_aimMutex);
        out = g_aim;
        return out.valid;
    }

    void tickAimRay()
    {
        if (!aimRayEnabled && !aimRayRequested)
            return;
        AimRay r;
        if (!render::cameraPosition(r.from) || !render::cameraForward(r.dir))
        {
            std::lock_guard<std::mutex> lock(g_aimMutex);
            g_aim = AimRay{ };
            return;
        }
        // 1 ahead of the camera
        const fb::Vec3 start{ r.from.m_x + r.dir.m_x * 1.0f, r.from.m_y + r.dir.m_y * 1.0f, r.from.m_z + r.dir.m_z * 1.0f };
        // 120 is range
        const fb::Vec3 to{ r.from.m_x + r.dir.m_x * 120.0f, r.from.m_y + r.dir.m_y * 120.0f, r.from.m_z + r.dir.m_z * 120.0f };

        void* me = nullptr; // fixed_vector of skips, nullptr = none
        void* caster = fb::physicsRayCaster();
#if defined(BFVE_GAME_BF4)
        me = fb::localSoldierPhysics();
#endif
        fb::RayCastHit hit{ };
        const bool ok = caster && fb::physicsRayQuery(start, to, hit, fb::RAY_CAST_WORLD_ONLY, me);
        if (ok)
        {
            r.valid = true;
            r.engine = true;
            r.hit = hit.m_position;
            r.normal = hit.m_normal;
            r.body = hit.m_rigidBody;
            r.part = hit.m_part;
            r.exactValid = resolveGroupHit(hit.m_rigidBody, hit.m_part, hit.m_position, r.exact);
            const float dx = r.hit.m_x - r.from.m_x, dy = r.hit.m_y - r.from.m_y, dz = r.hit.m_z - r.from.m_z;
            r.t = std::sqrt(dx * dx + dy * dy + dz * dz);
        }
        if (!r.valid)
        {
            PlacedMesh pm;
            float t = 0.0f;
            if (rayPlacedMesh(r.from, r.dir, 120.0f, pm, t))
            {
                r.valid = true;
                r.engine = false;
                r.t = t;
                r.hit = { r.from.m_x + r.dir.m_x * t, r.from.m_y + r.dir.m_y * t, r.from.m_z + r.dir.m_z * t };
            }
        }
        tickOcclusion(r.from, r.dir, me);
        std::lock_guard<std::mutex> lock(g_aimMutex);
        g_aim = r;
    }

    bool rayPlacedMesh(const fb::Vec3& from, const fb::Vec3& dir, float maxDistance,
                       PlacedMesh& out, float& t)
    {
        const WorldMesh* best = nullptr;
        float bestT = maxDistance;
        for (const WorldMesh& m : worldMeshes)
        {
            float hit = 0.0f;
            if (rayHitsWorldMesh(m, from, dir, bestT, hit) && hit < bestT)
            {
                // t0 clamped to 0 -> inside the box counts
                bestT = hit;
                best = &m;
            }
        }
        if (!best)
            return false;
        out = { best->mesh, best->varHash, best->pos };
        t = bestT;
        return true;
    }

    std::vector<NearMesh> placedMeshesMatching(const char* text, const fb::Vec3& from, size_t count)
    {
        std::vector<NearMesh> out;
        if (!text || !*text)
            return out;
        std::string needle = text;
        std::transform(needle.begin(), needle.end(), needle.begin(),
            [](unsigned char c) { return char(std::tolower(c)); });

        for (const WorldMesh& m : worldMeshes)
        {
            const char* name = m.mesh ? m.mesh->m_Name : nullptr;
            if (!name)
                continue;
            std::string lower = name;
            std::transform(lower.begin(), lower.end(), lower.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
            if (lower.find(needle) == std::string::npos)
                continue;
            const float d = m.hasBox ? boxDistance(m, from) : std::sqrt(distanceSqr(from, m.pos));
            out.push_back({ { m.mesh, m.varHash, m.pos }, d, m.hasBox });
        }
        std::sort(out.begin(), out.end(), [](const NearMesh& a, const NearMesh& b) { return a.distance < b.distance; });
        if (out.size() > count)
            out.resize(count);
        return out;
    }

    std::vector<NearMesh> placedMeshesNear(const fb::Vec3& pos, size_t count, float maxDistance)
    {
        std::vector<NearMesh> out;
        for (const WorldMesh& m : worldMeshes)
        {
            const float d = m.hasBox ? boxDistance(m, pos) : std::sqrt(distanceSqr(pos, m.pos));
            if (d > maxDistance)
                continue;
            out.push_back({ { m.mesh, m.varHash, m.pos }, d, m.hasBox });
        }
        std::sort(out.begin(), out.end(), [](const NearMesh& a, const NearMesh& b) { return a.distance < b.distance; });
        if (out.size() > count)
            out.resize(count);
        return out;
    }

    bool meshRefFor(const PlacedMesh& m, MeshVariationRef& out)
    {
        ContainerContents cc;
        AddMesh(static_cast<fb::MeshAsset*>(m.mesh), m.varHash, cc);
        if (cc.meshes.empty())
            return false;
        out = cc.meshes.front();
        return true;
    }

    static bool boxSane(const WorldMesh& m)
    {
        if (!m.hasBox)
            return false;
        const float dx = m.boxMax.m_x - m.boxMin.m_x, dy = m.boxMax.m_y - m.boxMin.m_y, dz = m.boxMax.m_z - m.boxMin.m_z;
        return dx >= 0.0f && dy >= 0.0f && dz >= 0.0f && dx < 400.0f && dy < 400.0f && dz < 400.0f &&
               std::fabs(m.boxMin.m_x) < 2000.0f && std::fabs(m.boxMin.m_y) < 2000.0f && std::fabs(m.boxMin.m_z) < 2000.0f;
    }

    static fb::Vec3 meshAnchor(const WorldMesh& m)
    {
        if (!boxSane(m))
            return m.pos;
        const fb::Vec3 c{ (m.boxMin.m_x + m.boxMax.m_x) * 0.5f, (m.boxMin.m_y + m.boxMax.m_y) * 0.5f, (m.boxMin.m_z + m.boxMax.m_z) * 0.5f };
        return applyTransform(m.frame, c);
    }

    static float meshExtent(const WorldMesh& m)
    {
        if (!boxSane(m))
            return 0.0f;
        const float dx = m.boxMax.m_x - m.boxMin.m_x, dy = m.boxMax.m_y - m.boxMin.m_y, dz = m.boxMax.m_z - m.boxMin.m_z;
        return dx > dy ? (dx > dz ? dx : dz) : (dy > dz ? dy : dz);
    }

    // classic 3d box
    static void drawMeshBox(const WorldMesh& m, const ImColor& col)
    {
        if (!boxSane(m))
            return;
        fb::Vec3 c[8];
        for (int i = 0; i < 8; ++i)
        {
            const fb::Vec3 local{ (i & 1) ? m.boxMax.m_x : m.boxMin.m_x,
                                  (i & 2) ? m.boxMax.m_y : m.boxMin.m_y,
                                  (i & 4) ? m.boxMax.m_z : m.boxMin.m_z };
            c[i] = applyTransform(m.frame, local);
        }
        static const int edges[12][2] = { {0,1},{2,3},{4,5},{6,7}, {0,2},{1,3},{4,6},{5,7}, {0,4},{1,5},{2,6},{3,7} };
        for (const auto& e : edges)
            render::line3(c[e[0]], c[e[1]], col, 1.0f);
    }

    // LOD0 subsets of a mesh set
    // material index and local box each
    static const MeshGeo& meshGeo(fb::MeshAsset* mesh)
    {
        auto it = g_meshGeo.find(mesh);
        if (it != g_meshGeo.end())
            return it->second;
        MeshGeo g;
        if (fb::MeshSet* set = findMeshSet(mesh))
        {
            fb::MeshLayout* lod = lod0(set);
            fb::MeshSubset* subs = lod ? subsetsOf(lod) : nullptr;
            if (subs && lod->m_subsetCount <= 256)
            {
                g.set = set;
                for (uint32_t i = 0; i < lod->m_subsetCount; ++i)
                    g.subsets.push_back({ subs[i].m_materialIndex, false, {}, {} });
            }
        }
        return g_meshGeo.emplace(mesh, std::move(g)).first->second;
    }

    static float halfToFloat(uint16_t h)
    {
        const uint32_t sign = (h >> 15) & 1, exp = (h >> 10) & 0x1F, man = h & 0x3FF;
        uint32_t bits;
        if (exp == 0)
            bits = sign << 31; // zero / denormal
        else if (exp == 31)
            bits = (sign << 31) | 0x7F800000;
        else
            bits = (sign << 31) | ((exp + 112) << 23) | (man << 13);
        float f;
        std::memcpy(&f, &bits, 4);
        return f;
    }

    // VertexElementFormat 3/4 float3/4, 7/8 half3/4, BF4 adds packed
    static bool positionFormat(const fb::MeshSubset& sub, bool& halves)
    {
        for (uint32_t e = 0; e < sub.m_elementCount && e < 16; ++e)
        {
            uint32_t fmt = 0, offset = 0, stream = 0;
            element(sub, e, fmt, offset, stream);
            if (offset != 0 || stream != 0)
                continue;
            const bool floats = fmt == 3 || fmt == 4 || fmt == 28 || fmt == 31 || fmt == 34 || fmt == 37;
            halves = fmt == 7 || fmt == 8 || fmt == 16 || fmt == 17 || fmt == 20 || fmt == 21 || fmt == 25 || fmt == 27 || fmt == 30 || fmt == 33 || fmt == 36;
            return floats || halves;
        }
        return false;
    }

    static fb::Vec3 decodePosition(const uint8_t* p, bool halves)
    {
        fb::Vec3 local{ };
        if (halves)
        {
            uint16_t h[3];
            std::memcpy(h, p, 6);
            local = { halfToFloat(h[0]), halfToFloat(h[1]), halfToFloat(h[2]) };
        }
        else
            std::memcpy(&local, p, 12);
        return local;
    }

    // asks for the GPU copy once
    // true when it is there.
    static bool geometryReady(fb::MeshAsset* mesh)
    {
        MeshGeo& g = const_cast<MeshGeo&>(meshGeo(mesh));
        if (g.copyState == 2)
            return true;
        if (g.copyState == 0 && g.set)
        {
            g.copyState = 1;
            std::lock_guard<std::mutex> lock(g_geoCopyMutex);
            g_geoCopyQueue.push_back(mesh);
        }
        return false;
    }

    static const ImColor kMaterialPalette[8] =
    {
        ImColor(255, 90, 90, 220), ImColor(90, 255, 120, 220), ImColor(90, 160, 255, 220), ImColor(255, 220, 80, 220),
        ImColor(255, 120, 255, 220), ImColor(80, 240, 240, 220), ImColor(255, 160, 60, 220), ImColor(200, 200, 200, 220)
    };

    // this is reversed with help of 2020 leak
    static size_t drawMeshWireframe(const WorldMesh& m, int onlyMaterial, size_t maxLines)
    {
        MeshGeo& g = const_cast<MeshGeo&>(meshGeo(m.mesh));
        const auto why = [&](const char* reason)
        {
            g.reason = reason;
            return 0;
        };
        g.reason.clear();
        if (!g.set || !g.set->m_layout)
            return why("mesh set not registered");
        fb::MeshLayout* lod = lod0(g.set);
        if (!lod)
            return why("no LOD0");
        if (g.copyState == 0)
        {
            g.copyState = 1;
            std::lock_guard<std::mutex> lock(g_geoCopyMutex);
            g_geoCopyQueue.push_back(m.mesh);
            return 0;
        }
        if (g.copyState != 2)
            return g.copyState == 3 ? why("buffer copy failed") : why("copying the buffers...");
        fb::MeshSubset* subs = subsetsOf(lod);
        const uint32_t n = lod->m_subsetCount;
        if (!subs || n > 256)
            return why("no subsets");

        uint32_t indexCount = 0, triangles = 0;
        for (uint32_t i = 0; i < n; ++i)
        {
            const uint32_t end = subs[i].m_startIndex + subs[i].m_primitiveCount * 3;
            if (end > indexCount) indexCount = end;
            if (onlyMaterial < 0 || subs[i].m_materialIndex == uint32_t(onlyMaterial))
                triangles += subs[i].m_primitiveCount;
        }
        if (!indexCount || !triangles)
            return why("subsets carry no triangles - the range fields may sit elsewhere");
        // IndexBufferFormat: 0 = 16-bit, 1 = 32-bit (both games)
        uint32_t bpi = g.indices.size() / indexCount >= 4 ? 4 : 2;
        if (lod->m_indexBufferFormat == 0) bpi = 2;
        else if (lod->m_indexBufferFormat == 1) bpi = 4;
        if (size_t(bpi) * indexCount > g.indices.size())
        {
            char r[128];
            std::snprintf(r, sizeof(r), "index range %u x %u B exceeds the %zu B index buffer (record dumped to the log)", indexCount, bpi, g.indices.size());
            return why(r);
        }
        const size_t step = triangles * 3 > maxLines ? (triangles * 3 + maxLines - 1) / maxLines : 1;

        size_t lines = 0;
        for (uint32_t i = 0; i < n; ++i)
        {
            const fb::MeshSubset& sub = subs[i];
            if (onlyMaterial >= 0 && sub.m_materialIndex != uint32_t(onlyMaterial))
                continue;
            bool halves = false;
            g.fmt0 = sub.m_elementCount ? elementFormat(sub, 0) : 0;
            g.stride0 = stride0(sub);
            g.triangles = sub.m_primitiveCount;
            if (!positionFormat(sub, halves))
            {
                char r[96];
                std::snprintf(r, sizeof(r), "position format %u unknown (elements %u, stride %u)", g.fmt0, sub.m_elementCount, stride0(sub));
                why(r);
                continue;
            }
            const uint32_t stride = stride0(sub);
            if (!stride || sub.m_vertexOffset + size_t(sub.m_vertexCount) * stride > g.vertices.size())
            {
                why("vertex range exceeds the vertex buffer");
                continue;
            }
            const uint8_t* vb = g.vertices.data() + sub.m_vertexOffset;
            const ImColor col = meshOverlayWirePalette ? kMaterialPalette[sub.m_materialIndex & 7]
                : ImColor(meshOverlayWireColor[0], meshOverlayWireColor[1], meshOverlayWireColor[2], meshOverlayWireColor[3]);
            const auto position = [&](uint32_t vi, fb::Vec3& out)
            {
                if (vi >= sub.m_vertexCount)
                    return false;
                out = applyTransform(m.frame, decodePosition(vb + size_t(vi) * stride, halves));
                return true;
            };
            for (uint32_t t = 0; t < sub.m_primitiveCount; t += uint32_t(step))
            {
                const size_t base = size_t(sub.m_startIndex) + size_t(t) * 3;
                uint32_t idx[3];
                if (bpi == 2)
                {
                    uint16_t s16[3];
                    std::memcpy(s16, g.indices.data() + base * 2, 6);
                    idx[0] = s16[0]; idx[1] = s16[1]; idx[2] = s16[2];
                }
                else
                    std::memcpy(idx, g.indices.data() + base * 4, 12);
                fb::Vec3 a, b, c;
                if (!position(idx[0], a) || !position(idx[1], b) || !position(idx[2], c))
                    continue;
                render::line3(a, b, col);
                render::line3(b, c, col);
                render::line3(c, a, col);
                lines += 3;
                if (lines >= maxLines)
                    return lines;
            }
        }
        return lines;
    }

    // copied through a staging buffer
    // the Map stalls the GPU
    // so one mesh per frame
    static bool copyGpuBuffer(void* renderBuffer, std::vector<uint8_t>& out)
    {
        if (!renderBuffer || !g_pDevice || !g_pContext)
            return false;
        ID3D11Buffer* buf = d3dBuffer(renderBuffer);
        if (!buf)
            return false;
        D3D11_BUFFER_DESC d{ };
        buf->GetDesc(&d);
        if (!d.ByteWidth || d.ByteWidth > (64u << 20))
            return false;
        D3D11_BUFFER_DESC sd{ };
        sd.ByteWidth = d.ByteWidth;
        sd.Usage = D3D11_USAGE_STAGING;
        sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        ID3D11Buffer* staging = nullptr;
        if (FAILED(g_pDevice->CreateBuffer(&sd, nullptr, &staging)) || !staging)
            return false;
        g_pContext->CopyResource(staging, buf);
        D3D11_MAPPED_SUBRESOURCE map{ };
        const bool ok = SUCCEEDED(g_pContext->Map(staging, 0, D3D11_MAP_READ, 0, &map));
        if (ok)
        {
            out.assign(static_cast<const uint8_t*>(map.pData), static_cast<const uint8_t*>(map.pData) + d.ByteWidth);
            g_pContext->Unmap(staging, 0);
        }
        staging->Release();
        return ok;
    }

    void tickGeometryCopies()
    {
        fb::MeshAsset* mesh = nullptr;
        {
            std::lock_guard<std::mutex> lock(g_geoCopyMutex);
            if (g_geoCopyQueue.empty())
                return;
            mesh = g_geoCopyQueue.back();
            g_geoCopyQueue.pop_back();
        }
        auto it = g_meshGeo.find(mesh);
        if (it == g_meshGeo.end())
            return;
        MeshGeo& g = it->second;
        g.copyState = 3;
        if (!g.set || !g.set->m_layout)
            return;
        fb::MeshLayout* lod = lod0(g.set);
        fb::MeshData* md = lod ? dataOf(lod) : nullptr;
        if (!md || !md->m_vertexBuffer || !md->m_indexBuffer)
        {
            return;
        }
        if (copyGpuBuffer(md->m_vertexBuffer, g.vertices) && copyGpuBuffer(md->m_indexBuffer, g.indices))
        {
            g.vertexDataSize = lod->m_vertexDataSize;
            g.indexDataSize = lod->m_indexDataSize;
            g.copyState = 2;
            // per-material bounds from vertices, record has none
            fb::MeshSubset* subs = subsetsOf(lod);
            for (uint32_t i = 0; subs && i < lod->m_subsetCount && i < g.subsets.size(); ++i)
            {
                const fb::MeshSubset& sub = subs[i];
                bool halves = false;
                const uint32_t stride = stride0(sub);
                if (!positionFormat(sub, halves) || !stride ||
                    sub.m_vertexOffset + size_t(sub.m_vertexCount) * stride > g.vertices.size() || !sub.m_vertexCount)
                    continue;
                fb::Vec3 mn{ FLT_MAX, FLT_MAX, FLT_MAX }, mx{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
                for (uint32_t v = 0; v < sub.m_vertexCount; ++v)
                {
                    const fb::Vec3 p = decodePosition(g.vertices.data() + sub.m_vertexOffset + size_t(v) * stride, halves);
                    mn = { p.m_x < mn.m_x ? p.m_x : mn.m_x, p.m_y < mn.m_y ? p.m_y : mn.m_y, p.m_z < mn.m_z ? p.m_z : mn.m_z };
                    mx = { p.m_x > mx.m_x ? p.m_x : mx.m_x, p.m_y > mx.m_y ? p.m_y : mx.m_y, p.m_z > mx.m_z ? p.m_z : mx.m_z };
                }
                g.subsets[i].mn = mn;
                g.subsets[i].mx = mx;
                g.subsets[i].hasBox = true;
            }
            // lower LODs carry their own radiosity charts
            for (uint32_t l = 1; l < g.set->m_layout->m_lodCount && l < 6; ++l)
            {
                fb::MeshLayout* ll = lodAt(g.set, l);
                fb::MeshData* ld = ll ? dataOf(ll) : nullptr;
                if (!ld || !ld->m_vertexBuffer || !ld->m_indexBuffer ||
                    !copyGpuBuffer(ld->m_vertexBuffer, g.lodVertices[l]) || !copyGpuBuffer(ld->m_indexBuffer, g.lodIndices[l]))
                {
                    g.lodVertices[l].clear();
                    g.lodIndices[l].clear();
                }
            }
        }
    }

    void drainGeometryCopies(int maxCount)
    {
        for (int i = 0; i < maxCount; ++i)
        {
            {
                std::lock_guard<std::mutex> lock(g_geoCopyMutex);
                if (g_geoCopyQueue.empty())
                    return;
            }
            tickGeometryCopies();
        }
    }

    static uint32_t elementUsage(const fb::MeshSubset& sub, uint32_t e)
    {
#if defined(BFVE_GAME_BF4)
        return sub.m_declElements[e] & 0xFF;
#else
        return sub.m_elements[e][0];
#endif
    }

    uint32_t meshLodCount(fb::MeshAsset* mesh)
    {
        const MeshGeo& g = meshGeo(mesh);
        return g.set && g.set->m_layout ? (std::min)(uint32_t(g.set->m_layout->m_lodCount), 6u) : 0;
    }

    int meshGeometry(fb::MeshAsset* mesh, GeoView& out, uint32_t l)
    {
        out = GeoView{};
        if (!mesh || l >= 6)
            return -1;
        MeshGeo& g = const_cast<MeshGeo&>(meshGeo(mesh));
        if (!g.set || !g.set->m_layout)
            return -1;
        if (!geometryReady(mesh))
            return g.copyState == 3 ? -1 : 0;
        const std::vector<uint8_t>& vertices = l ? g.lodVertices[l] : g.vertices;
        const std::vector<uint8_t>& indices = l ? g.lodIndices[l] : g.indices;
        if (vertices.empty() || indices.empty())
            return -1;
        fb::MeshLayout* lod = lodAt(g.set, l);
        fb::MeshSubset* subs = lod ? subsetsOf(lod) : nullptr;
        const uint32_t n = lod ? lod->m_subsetCount : 0;
        if (!subs || !n || n > 256)
            return -1;

        uint32_t indexCount = 0;
        for (uint32_t i = 0; i < n; ++i)
            indexCount = (std::max)(indexCount, subs[i].m_startIndex + subs[i].m_primitiveCount * 3);
        uint32_t bpi = indexCount && indices.size() / indexCount >= 4 ? 4 : 2;
        if (lod->m_indexBufferFormat == 0) bpi = 2;
        else if (lod->m_indexBufferFormat == 1) bpi = 4;

        out.vertices = vertices.data();
        out.vertexBytes = vertices.size();
        out.indices = indices.data();
        out.indexBytes = indices.size();
        out.bytesPerIndex = bpi;
        for (uint32_t i = 0; i < n; ++i)
        {
            const fb::MeshSubset& sub = subs[i];
            GeoSubset gs;
            gs.materialIndex = sub.m_materialIndex;
            gs.primitiveCount = sub.m_primitiveCount;
            gs.startIndex = sub.m_startIndex;
            gs.vertexOffset = sub.m_vertexOffset;
            gs.vertexCount = sub.m_vertexCount;
            gs.stride = stride0(sub);
            bool halves = false;
            if (!positionFormat(sub, halves))
                gs.stride = 0; // unusable
            gs.posHalves = halves;
            for (uint32_t e = 0; e < sub.m_elementCount && e < 16; ++e)
            {
                // decl usage = VertexElementUsage + 10: TexCoord0 0x21, RadiosityTexCoord 0x2A
                const uint32_t usage = elementUsage(sub, e);
#if defined(BFVE_GAME_BF3)
                // BF3 RadiosityTexCoord 0x1F, decl byte unverified
                if (usage != 0x1F && usage != 0x29)
#else
                if (usage != 0x2A && usage != 0x20)
#endif
                    continue;
                uint32_t fmt = 0, off = 0, st = 0;
                element(sub, e, fmt, off, st);
                gs.uvFormat = fmt;
                gs.uvOffset = off;
                gs.uvStream = int(st);
                break;
            }
            out.subsets.push_back(gs);
#if defined(BFVE_GAME_BF3)
            static int s_logged = 0;
            if (s_logged < 6)
            {
                ++s_logged;
                std::string us;
                for (uint32_t e = 0; e < sub.m_elementCount && e < 16; ++e) us += std::format("{:02x} ", elementUsage(sub, e));
                logger::info("[lights] BF3 decl usages of {}: {}(radiosity uv {})", mesh->m_Name ? mesh->m_Name : "?", us, gs.uvFormat ? "found" : "none");
            }
#endif
        }
        return 1;
    }

    bool meshBox(fb::MeshAsset* mesh, fb::Vec3& mn, fb::Vec3& mx) { return meshLocalBox(mesh, mn, mx); }

    void forEachWorldMesh(const std::function<void(const WorldMeshRef&)>& fn)
    {
        for (const WorldMesh& m : worldMeshes)
            if (m.mesh)
                fn(WorldMeshRef{ m.mesh, &m.frame, m.hasBox, m.boxMin, m.boxMax, m.radiosityOverride });
    }

    static const std::vector<std::string>& overlayTextures(const WorldMesh& m)
    {
        static std::unordered_map<uint64_t, std::vector<std::string>> cache;
        static uint32_t generation = ~0u;
        if (generation != textures::catalogGeneration)
        {
            cache.clear();
            generation = textures::catalogGeneration;
        }
        const uint64_t key = reinterpret_cast<uintptr_t>(m.mesh) ^ (uint64_t(m.varHash) << 40);
        auto it = cache.find(key);
        if (it != cache.end())
            return it->second;
        std::vector<std::string> names;
        MeshVariationRef ref;
        if (meshRefFor(PlacedMesh{ m.mesh, m.varHash, m.pos }, ref))
            for (const std::string& t : textures::meshTextureNames(ref, 6))
            {
                const size_t k = t.rfind('/');
                names.push_back(k == std::string::npos ? t : t.substr(k + 1));
            }
        if (cache.size() > 4096)
            cache.clear();
        return cache.emplace(key, std::move(names)).first->second;
    }

    static void renderMeshOverlay()
    {
        g_haveAimedMesh = false;
        if (!showMeshOverlay || worldMeshes.empty())
            return;

        const ImVec2 disp = render::displaySize();
        const ImVec2 center{ disp.x * 0.5f, disp.y * 0.5f };
        const float fontSize = render::textHeight();

        struct Hit { const WorldMesh* m; ImVec2 sp; float depth; float centerSqr; };
        std::vector<Hit> hits;
        hits.reserve(256);

        std::string filter;
        for (const char* c = meshOverlayFilter; *c; ++c)
            filter.push_back(char(std::tolower(static_cast<unsigned char>(*c))));
        const auto matchesFilter = [&filter](const WorldMesh& m)
        {
            if (filter.empty())
                return true;
            if (!m.mesh || !m.mesh->m_Name)
                return false;
            std::string name = m.mesh->m_Name;
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return char(std::tolower(c)); });
            return name.find(filter) != std::string::npos;
        };

        size_t best = SIZE_MAX;
        float bestSqr = FLT_MAX;
        for (const WorldMesh& m : worldMeshes)
        {
            if (!matchesFilter(m))
                continue;
            ImVec2 sp{ };
            float depth = 0.0f;
            if (!render::worldToScreen(meshAnchor(m), sp, depth) || (filter.empty() && depth > meshOverlayMaxDistance))
                continue;

            const float dx = sp.x - center.x, dy = sp.y - center.y;
            const float csqr = dx * dx + dy * dy;
            if (csqr < bestSqr)
            {
                bestSqr = csqr;
                best = hits.size();
            }
            hits.push_back({ &m, sp, depth, csqr });
        }
        if (hits.size() > 400)
        {
            std::nth_element(hits.begin(), hits.begin() + 400, hits.end(),
                             [](const Hit& a, const Hit& b) { return a.centerSqr < b.centerSqr; });
            hits.resize(400);
            best = 0;
            for (size_t i = 1; i < hits.size(); ++i)
                if (hits[i].centerSqr < hits[best].centerSqr)
                    best = i;
        }

        for (size_t i = 0; i < hits.size(); ++i)
        {
            const Hit& h = hits[i];
            const bool aimed = i == best;
            if (meshOverlayOnlyAimed && !aimed && filter.empty())
                continue;
            ImColor col = aimed ? render::Colors::Yellow : filter.empty()
                ? ImColor(meshOverlayColor[0], meshOverlayColor[1], meshOverlayColor[2], meshOverlayColor[3]) : render::Colors::Cyan;
            if (!overlayVisible(meshAnchor(*h.m)))
                col.Value.w *= 0.25f;
            const float r = aimed ? 6.0f : 4.0f;
            render::rect2(ImVec2{ h.sp.x - r, h.sp.y - r }, ImVec2{ h.sp.x + r, h.sp.y + r }, col, false, aimed ? 2.0f : 1.0f);
            // "big pieces" get a box, or if the user is filtering for a name
            if (aimed || !filter.empty() || meshExtent(*h.m) > 8.0f)
                drawMeshBox(*h.m, col);

            if (!meshOverlayLabels && !aimed)
                continue;
            if (!aimed && filter.empty() && h.depth > meshOverlayMaxDistance * 0.5f)
                continue;

            const char* name = h.m->mesh ? h.m->mesh->m_Name : nullptr;
            if (!name)
                continue;
            const char* slash = std::strrchr(name, '/');
            const char* leaf = slash ? slash + 1 : name;
            const ImVec2 tpos{ h.sp.x + r + 2.0f, h.sp.y - fontSize * 0.5f };
            if (filter.empty())
                render::label(tpos, leaf, col);
            else
            {
                char buf[160];
                std::snprintf(buf, sizeof(buf), "%s  %.0f m", leaf, h.depth);
                render::label(tpos, buf, col);
            }

            if (meshOverlayTextures && (aimed || h.centerSqr < 300.0f * 300.0f))
            {
                const MeshGeo& geo = meshGeo(h.m->mesh);
                if (geometryReady(h.m->mesh) && !geo.subsets.empty())
                {
                    const auto leafOf = [](const std::string& p) { const size_t k = p.rfind('/'); return k == std::string::npos ? p : p.substr(k + 1); };
                    for (const SubsetGeo& sg : geo.subsets)
                    {
                        if (!sg.hasBox)
                            continue;
                        const fb::Vec3 c = applyTransform(h.m->frame, fb::Vec3{ (sg.mn.m_x + sg.mx.m_x) * 0.5f, (sg.mn.m_y + sg.mx.m_y) * 0.5f, (sg.mn.m_z + sg.mx.m_z) * 0.5f });
                        ImVec2 csp{ };
                        float cdepth = 0.0f;
                        if (!render::worldToScreen(c, csp, cdepth))
                            continue;
                        const ImColor mc = kMaterialPalette[sg.material & 7];
                        render::rect2(ImVec2{ csp.x - 3.0f, csp.y - 3.0f }, ImVec2{ csp.x + 3.0f, csp.y + 3.0f }, mc, true);
                        char mb[200];
                        std::string texts;
                        for (const std::string& t : textures::materialTextures(h.m->mesh->m_NameHash, sg.material, 3))
                            texts += (texts.empty() ? "" : ", ") + leafOf(t);
                        std::snprintf(mb, sizeof(mb), "[%u] %s", sg.material, texts.empty() ? "(no texture named)" : texts.c_str());
                        render::label(ImVec2{ csp.x + 6.0f, csp.y - fontSize * 0.5f }, mb, mc);
                    }
                }
                else
                {
                    float ty = tpos.y + fontSize;
                    for (const std::string& t : overlayTextures(*h.m))
                    {
                        render::label(ImVec2{ tpos.x + 8.0f, ty }, t.c_str(), ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w * 0.85f));
                        ty += fontSize;
                    }
                }
            }
            if (meshOverlayWireframe && (aimed || !filter.empty()))
            {
                const size_t drawn = drawMeshWireframe(*h.m, -1, aimed ? 40000 : 6000);
                if (aimed)
                {
                    const MeshGeo& wg = meshGeo(h.m->mesh);
                    char wb[200];
                    if (drawn)
                        std::snprintf(wb, sizeof(wb), "wireframe: %zu lines (fmt %u, stride %u)", drawn, wg.fmt0, wg.stride0);
                    else
                        std::snprintf(wb, sizeof(wb), "wireframe: %s", wg.reason.empty() ? "nothing to draw" : wg.reason.c_str());
                    render::label(ImVec2{ h.sp.x + r + 2.0f, h.sp.y + fontSize * 0.6f }, wb, ImColor(255, 180, 120, 255));
                }
            }
        }

        if (best != SIZE_MAX)
        {
            g_aimedMesh = { hits[best].m->mesh, hits[best].m->varHash, hits[best].m->pos };
            g_haveAimedMesh = true;
        }
    }

    static float renderSurfaceLines(float x, float y)
    {
        if (!textures::pick::enabled)
            return y;
        textures::SurfaceInfo si;
        if (!textures::surfaceUnderCrosshair(si))
            return y;
        const auto leafOf = [](const std::string& p) { const size_t k = p.rfind('/'); return k == std::string::npos ? p : p.substr(k + 1); };
        char buf[256];
        if (si.meshName.empty())
            std::snprintf(buf, sizeof(buf), "surface: material not catalogued - textures bound to it:");
        else
            std::snprintf(buf, sizeof(buf), "%s  [%u] %s%s%s", leafOf(si.meshName).c_str(), si.material, leafOf(si.shaderName).c_str(),
                          si.variationName.empty() ? "" : "  var ", si.variationName.empty() ? "" : leafOf(si.variationName).c_str());
        render::label(ImVec2{ x, y }, buf, render::Colors::Cyan);
        y += render::textHeight();
        if (!si.slots.empty())
        {
            for (const auto& [name, path] : si.slots)
            {
                const std::string line = name + " = " + (path.empty() ? "(not catalogued)" : leafOf(path));
                render::label(ImVec2{ x, y }, line.c_str(), path.empty() ? ImColor(190, 190, 190, 170) : render::Colors::White);
                y += render::textHeight();
            }
            return y;
        }
        for (size_t i = 0; i < si.textures.size(); ++i)
        {
            const bool own = i < si.own;
            const std::string line = own ? leafOf(si.textures[i]) : leafOf(si.textures[i]) + "  (also bound)";
            render::label(ImVec2{ x, y }, line.c_str(), own ? render::Colors::White : ImColor(190, 190, 190, 170));
            y += render::textHeight();
        }
        return y;
    }

    static void renderAimRay()
    {
        AimRay r;
        if (!aimRayEnabled || !lastAimRay(r))
        {
            const ImVec2 disp = render::displaySize();
            renderSurfaceLines(disp.x * 0.5f + 12.0f, disp.y * 0.5f + 8.0f);
            return;
        }

        const fb::Vec3 tail{ r.from.m_x + r.dir.m_x * 1.5f, r.from.m_y + r.dir.m_y * 1.5f - 0.25f, r.from.m_z + r.dir.m_z * 1.5f };
        render::line3(tail, r.hit, r.engine ? ImColor(255, 200, 60, 200) : ImColor(160, 160, 160, 160), 1.5f);
        ImVec2 sp{ };
        if (!render::worldToScreen(r.hit, sp))
            return;
        render::circle(sp, 6.0f, ImColor(255, 200, 60, 255), 12, 2.0f);
        if (r.engine)
        {
            const fb::Vec3 n{ r.hit.m_x + r.normal.m_x * 0.5f, r.hit.m_y + r.normal.m_y * 0.5f, r.hit.m_z + r.normal.m_z * 0.5f };
            render::line3(r.hit, n, ImColor(80, 220, 255, 255), 2.0f);
        }

        PlacedMesh pm;
        bool have = r.exactValid;
        if (have)
            pm = r.exact;
        else
            have = placedMeshAt(r.hit, 0.75f, pm);

        static PlacedMesh cachedFor;
        static std::vector<std::string> lines;
        if (!have)
            lines.clear();
        else if (cachedFor.mesh != pm.mesh || cachedFor.varHash != pm.varHash || lines.empty())
        {
            cachedFor = pm;
            lines.clear();
            const auto leafOf = [](const std::string& p) { const size_t k = p.rfind('/'); return k == std::string::npos ? p : p.substr(k + 1); };
            MeshVariationRef ref;
            if (meshRefFor(pm, ref))
            {
                lines.push_back(leafOf(ref.name) + (r.exactValid ? "" : " (nearest box)"));
                for (const std::string& t : textures::meshTextureNames(ref, 8))
                    lines.push_back(leafOf(t));
            }
        }

        char head[64];
        std::snprintf(head, sizeof(head), "%.1f m", r.t);
        float y = sp.y - 8.0f;
        const float x = sp.x + 12.0f;
        render::label(ImVec2{ x, y }, head, ImColor(255, 200, 60, 255));
        y += render::textHeight();
        y = renderSurfaceLines(x, y);
        if (!have)
        {
            char why[96];
            if (r.engine)
                std::snprintf(why, sizeof(why), "body %p part %u - not a group member, no box here", r.body, r.part);
            else
                std::snprintf(why, sizeof(why), "engine ray missed - box ray only");
            render::label(ImVec2{ x, y }, why, ImColor(255, 120, 120, 255));
        }
        for (size_t i = 0; i < lines.size(); ++i, y += render::textHeight())
            render::label(ImVec2{ x, y }, lines[i].c_str(), i == 0 ? render::Colors::Yellow : render::Colors::White);
    }

    void renderOverlay()
    {
        renderMeshOverlay();
        renderAimRay();

        if (!showOverlay)
            return;

        pruneDeadEntities();

        fb::LocalLightEntity* closest = closestLightToCrosshair();

        const auto displayNames = buildDisplayNames();

        const ImVec2 disp = render::displaySize();
        const ImVec2 center{ disp.x * 0.5f, disp.y * 0.5f };
        constexpr float LABEL_SCALE = 1.25f;

        for (auto& [dataPtr, entry] : entries)
        {
            bool haveLabel = false;
            float bestCenterSqr = FLT_MAX;
            ImVec2 labelSp{};
            float labelMarkerR = 0.0f;
            float labelDepth = 0.0f;
            ImColor labelCol = render::Colors::LightBlue;

            for (fb::LocalLightEntity* e : entry.activeEntities)
            {
                const bool isClosest = (e == closest);
                if (showOnlyClosest && !isClosest)
                    continue;

                fb::Vec3 pos{ };
                if (!liveLightEntity(e, entry) || !entityPos(e, pos))
                    continue;

                ImVec2 sp{ };
                float depth = 0.0f;
                if (!render::worldToScreen(pos, sp, depth))
                    continue;

                if (depth > overlayMaxDistance)
                    continue;

                const bool visible = overlayVisible(pos);
                ImColor col = isClosest ? render::Colors::Yellow : render::Colors::LightBlue;
                if (!visible)
                    col.Value.w *= 0.25f;

                float markerR = 0.0f;
                if (isClosest)
                {
                    render::drawSphere(pos, 0.35f, 16, 8, col);
                    markerR = 20.0f;
                }
                else
                {
                    markerR = 700.0f / depth;
                    if (markerR < 3.0f) markerR = 3.0f;
                    if (markerR > 26.0f) markerR = 26.0f;
                    render::circle(sp, markerR, col);
                }

                const float dx = sp.x - center.x, dy = sp.y - center.y;
                const float csqr = isClosest ? -1.0f : dx * dx + dy * dy;
                if (csqr < bestCenterSqr)
                {
                    bestCenterSqr = csqr;
                    labelSp = sp;
                    labelMarkerR = markerR;
                    labelDepth = depth;
                    labelCol = col;
                    haveLabel = true;
                }
            }

            if (!haveLabel)
                continue;

            const auto nameIt = displayNames.find(dataPtr);
            const char* dispName = nameIt != displayNames.end()
                ? nameIt->second.c_str() : entry.assetName.c_str();

            const size_t count = entry.activeEntities.size();
            char buf[256];
            if (count > 1)
                std::snprintf(buf, sizeof(buf), "%s [%s] x%zu  %.0fm",
                    dispName, entry.lightType.c_str(), count, labelDepth);
            else
                std::snprintf(buf, sizeof(buf), "%s [%s]  %.0fm",
                    dispName, entry.lightType.c_str(), labelDepth);

            const float tw = render::textWidth(buf, LABEL_SCALE);
            const ImVec2 tpos{ labelSp.x - tw * 0.5f, labelSp.y + labelMarkerR + 3.0f };
            render::label(tpos, buf, labelCol, LABEL_SCALE);
        }
    }
}