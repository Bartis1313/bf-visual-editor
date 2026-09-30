#pragma once

#include "../types.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>
#include <functional>

using json = nlohmann::json;

namespace editor::lights
{
    void init();
    void shutdown();
    void clear();

    void scanAll();
    void scanExistingEntities();

    void refreshEntities();

    void nameDynamicLightsByEmitters();
    void scanAndApplyOverrides();

    void rescanIfResourcesGrew();

    void onEntityCreated(fb::LocalLightEntity* entity, fb::LocalLightEntityData* data);
    void onEntityDestroyed(fb::LocalLightEntity* entity);
    void forgetData(fb::LocalLightEntityData* data); // before a spawned light's data is freed

    void onPartitionLoaded(void* partition);

    void applyOverride(LightDataEntry& entry);
    void applyToData(fb::LocalLightEntityData* data, const LightDataEntry& entry);
    void captureOriginal(LightDataEntry& entry);
    void resetAll();

    std::unordered_map<fb::LocalLightEntityData*, LightDataEntry>& getEntries();

    void renderTab();
    void renderEditor(LightDataEntry& entry);
    void renderMeshesPanel();

    // realized mesh entity near a light + its param blocks
    struct MeshInstance
    {
        void* entity = nullptr;
        void* data = nullptr;
        std::string name;
        std::vector<void*> blocks;
        std::vector<void**> blockSlots;
    };

    // from MeshAsset::m_Materials
    struct MeshMaterialInfo
    {
        std::string shader;
        std::vector<std::string> parameters;

        struct TextureBinding
        {
            std::string parameter, texture;
            void* asset = nullptr;
        };
        std::vector<TextureBinding> textures;
    };

    std::vector<MeshMaterialInfo> meshAssetMaterials(void* meshAsset);

    // LocalLightEntity::m_transform.m_trans
    bool entityPosOf(fb::LocalLightEntity* e, fb::Vec3& out);

    // ShaderParameterEntityData::m_VecParam
    bool readShaderDriver(void* data, uint32_t& handle, float (&value)[4]);
    bool writeShaderDriver(void* data, const float (&value)[4]);

    // claimed in the sub_140C26490 hook, beats connections
    bool shaderParamOverride(uint32_t handle, float (&out)[4]);
    void setShaderParamOverride(uint32_t handle, const float (&value)[4]);
    void clearShaderParamOverride(uint32_t handle);
    bool hasShaderParamOverride(uint32_t handle);

    struct FlareInstance
    {
        void* entity = nullptr;
        void* data = nullptr;
    };

    void scanLensFlares();
    void scanLensFlaresFor(const std::vector<void*>& wantedData); // empty = all
    const std::vector<FlareInstance>& lensFlares();
    inline uint32_t flareListGeneration = 0;

    uint32_t flareElementCount(void* flareData);
    fb::LensFlareElement* flareElement(void* flareData, uint32_t index);
    std::string flareShaderName(void* element); // LensFlareElement::m_Shader->m_Name

    std::string flareTextureKey(const std::string& shaderName);

    // flare color = which shader, no parameters
    struct FlareShaderChoice { std::string name; void* object = nullptr; };
    const std::vector<FlareShaderChoice>& flareShaderPalette();
    void harvestFlareShaders(); // level + authored, sorted by leaf name
    void* fetchFlareShader(const char* name);

    // re-applied by the sub_140CCCDA0 hook after rebuilds
    void* flareShaderSlot(void* entity, uint32_t elementIndex);
    bool setFlareShader(void* entity, uint32_t elementIndex, void* shaderObject);
    void applyFlareShaderClaims(void* entity);
    void clearFlareShaderClaim(void* entity, uint32_t elementIndex);
    bool hasFlareShaderClaims();

    constexpr uint32_t ALL_FLARE_ELEMENTS = 0xFFFFFFFFu;
    void setFlareShaderFor(LightDataEntry& entry, uint32_t element, const std::string& shaderName);
    void clearFlareShadersFor(LightDataEntry& entry);
    uint32_t applyFlareShaders(LightDataEntry& entry); // 0 until the flares are realized

    // offset into LensFlareElement, all flare data of the light
    void setFlareFieldFor(LightDataEntry& entry, uint32_t element, uint32_t offset,
                              const float* value, uint8_t count);
    uint32_t applyFlareFields(LightDataEntry& entry);

    void setShaderDriverFor(LightDataEntry& entry, uint32_t handle, const float (&value)[4]);
    uint32_t applyShaderDrivers(LightDataEntry& entry);

    void tick(); // retries halo edits until flares exist
    // far lens flares follow dir like the sun flare, null restores
    size_t followSunFlares(const fb::Vec3* dir);
    void forgetSunFlares(); // level unload

    fb::LocalLightEntity* closestLightToCrosshair(float* outScreenDist = nullptr);
    void renderOverlay();

    struct PlacedMesh
    {
        void* mesh = nullptr;
        uint32_t varHash = 0;
        fb::Vec3 pos{ };
    };
    bool aimedPlacedMesh(PlacedMesh& out);
    bool currentMesh(PlacedMesh& out); // locked or aimed mesh
    bool meshRefFor(const PlacedMesh& m, MeshVariationRef& out);
    size_t placedMeshCount();

    struct NearMesh { PlacedMesh mesh; float distance; bool hasBox; };
    std::vector<NearMesh> placedMeshesNear(const fb::Vec3& pos, size_t count, float maxDistance);

    bool placedMeshAt(const fb::Vec3& point, float slack, PlacedMesh& out);

    struct AimRay
    {
        bool valid = false;
        bool engine = false;
        fb::Vec3 from{ }, dir{ }, hit{ }, normal{ };
        float t = 0.0f;
        void* body = nullptr;
        uint32_t part = 0;
        bool exactValid = false;
        PlacedMesh exact;
    };
    inline bool aimRayEnabled = false; // physics ray per update tick + overlay
    inline bool aimRayRequested = false; // ray without the overlay
    void tickAimRay();
    bool lastAimRay(AimRay& out);

    inline bool overlayOcclusion = false; // one engine ray per overlay item per frame
    bool overlayVisible(const fb::Vec3& pos);

    bool rayPlacedMesh(const fb::Vec3& from, const fb::Vec3& dir, float maxDistance,
                       PlacedMesh& out, float& t);

    std::vector<NearMesh> placedMeshesMatching(const char* text, const fb::Vec3& from, size_t count);

    inline PlacedMesh meshPick;
    inline bool meshPickPending = false;

    inline bool showMeshOverlay = false;
    inline char meshOverlayFilter[64] = { };
    inline bool meshOverlayTextures = false;
    inline bool meshOverlayWireframe = false;
    inline float meshOverlayColor[4] = { 0.70f, 0.90f, 0.70f, 0.80f };
    inline float meshOverlayWireColor[4] = { 1.00f, 0.60f, 0.20f, 0.85f };
    inline bool meshOverlayWirePalette = true;
    inline bool meshOverlayOnlyAimed = false;
    inline bool meshOverlayLabels = true;
    void tickGeometryCopies();
    void drainGeometryCopies(int maxCount); // several per frame while a bake waits

    // meshGeometry: 0 pending, 1 filled, -1 no usable set
    struct GeoSubset
    {
        uint32_t materialIndex = 0, primitiveCount = 0, startIndex = 0;
        uint32_t vertexOffset = 0, vertexCount = 0, stride = 0;
        uint32_t posOffset = 0;
        bool posHalves = false;
        uint32_t uvFormat = 0, uvOffset = 0; // radiosity UV (decl usage 0x2A), 0 = none
        int uvStream = 0;
    };
    struct GeoView
    {
        const uint8_t* vertices = nullptr;
        size_t vertexBytes = 0;
        const uint8_t* indices = nullptr;
        size_t indexBytes = 0;
        uint32_t bytesPerIndex = 2;
        std::vector<GeoSubset> subsets;
    };
    int meshGeometry(fb::MeshAsset* mesh, GeoView& out, uint32_t lod = 0);
    void staticMeshBodies(std::vector<void*>& out); // RayCastHit::m_rigidBody of static model groups
    uint32_t meshLodCount(fb::MeshAsset* mesh);
    bool meshBox(fb::MeshAsset* mesh, fb::Vec3& mn, fb::Vec3& mx); // mesh-local LOD0 box
    struct WorldMeshRef
    {
        fb::MeshAsset* mesh;
        const fb::LinearTransform* frame;
        bool hasBox;
        fb::Vec3 boxMin, boxMax; // mesh-local
        int radiosityOverride; // 0 none, 1 dynamic, 2 light probe, 3 terrain projected
    };
    void forEachWorldMesh(const std::function<void(const WorldMeshRef&)>& fn);
    inline float meshOverlayMaxDistance = 40.0f;

    inline bool showOverlay = false;
    inline bool showOnlyClosest = false;
    inline float overlayMaxDistance = 100.0f;

    json serialize(const LightDataEntry& entry);
    void deserialize(const json& j, LightDataEntry& entry);

    bool isUnresolvedName(const std::string& name);
    bool entryWorldPos(const LightDataEntry& entry, fb::Vec3& out);

    std::unordered_map<fb::LocalLightEntityData*, std::string> buildDisplayNames();
    enum class KeyMatch { None, Name, Exact };

    std::string makeLightKey(const LightDataEntry& entry, fb::LocalLightEntityData* dataPtr, const std::string& displayName);
    KeyMatch matchLightKey(const std::string& key, const LightDataEntry& entry, fb::LocalLightEntityData* dataPtr, const std::string& displayName);
}
