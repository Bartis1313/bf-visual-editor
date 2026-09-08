#pragma once

#include "../types.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>

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

    void onPartitionLoaded(void* partition);

    void applyOverride(LightDataEntry& entry);
    void applyToData(fb::LocalLightEntityData* data, const LightDataEntry& entry);
    void captureOriginal(LightDataEntry& entry);
    void resetAll();

    std::unordered_map<fb::LocalLightEntityData*, LightDataEntry>& getEntries();

    void renderTab();
    void renderEditor(LightDataEntry& entry);
    void renderMeshesPanel();

    // A realized mesh entity beside a light and the ShaderParameterBlocks reachable from it.
    struct MeshInstance
    {
        void* entity = nullptr;
        void* data = nullptr;
        std::string name;
        std::vector<void*> blocks;
        std::vector<void**> blockSlots;
    };

    // MeshAsset::m_Materials: shader name, vector parameter names, texture bindings.
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

    // LocalLightEntity::m_transform.m_trans, false when the entity is not live.
    bool entityPosOf(fb::LocalLightEntity* e, fb::Vec3& out);

    // ShaderParameterEntityData::m_VecParam - what the engine pushes into the mesh block.
    bool readShaderDriver(void* data, uint32_t& handle, float (&value)[4]);
    bool writeShaderDriver(void* data, const float (&value)[4]);

    // Claim a shader parameter by handle; substituted in the hook on sub_140C26490, which
    // wins over both the data and a property-connection override.
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
    inline uint32_t flareListGeneration = 0; // bumped whenever the list is rebuilt

    uint32_t flareElementCount(void* flareData);
    fb::LensFlareElement* flareElement(void* flareData, uint32_t index);
    std::string flareShaderName(void* element); // LensFlareElement::m_Shader->m_Name

    std::string flareTextureKey(const std::string& shaderName);

    // Shader objects by name. Color is which shader an element uses (they carry no
    // parameters), so the palette is the color vocabulary.
    struct FlareShaderChoice { std::string name; void* object = nullptr; };
    const std::vector<FlareShaderChoice>& flareShaderPalette();
    void harvestFlareShaders(); // level + authored list, sorted by leaf name
    void* fetchFlareShader(const char* name);

    // Slot swap on one entity, re-applied from the hook on sub_140CCCDA0 after each rebuild.
    void* flareShaderSlot(void* entity, uint32_t elementIndex);
    bool setFlareShader(void* entity, uint32_t elementIndex, void* shaderObject);
    void applyFlareShaderClaims(void* entity);
    void clearFlareShaderClaim(void* entity, uint32_t elementIndex);
    bool hasFlareShaderClaims();

    // Recorded on the light so a config can name it; re-applied to whatever flares the
    // light has. element == kAllFlareElements points every element at the shader.
    constexpr uint32_t kAllFlareElements = 0xFFFFFFFFu;
    void setFlareShaderFor(LightDataEntry& entry, uint32_t element, const std::string& shaderName);
    void clearFlareShadersFor(LightDataEntry& entry);
    uint32_t applyFlareShaders(LightDataEntry& entry); // 0 until the flares are realized

    // Element numbers (offset into LensFlareElement), written to every flare data of the
    // light - shared by all placements - and recorded for the config.
    void setFlareFieldFor(LightDataEntry& entry, uint32_t element, uint32_t offset,
                              const float* value, uint8_t count);
    uint32_t applyFlareFields(LightDataEntry& entry);

    void setShaderDriverFor(LightDataEntry& entry, uint32_t handle, const float (&value)[4]);
    uint32_t applyShaderDrivers(LightDataEntry& entry);

    void tick(); // retries saved halo edits until their flares exist

    fb::LocalLightEntity* closestLightToCrosshair(float* outScreenDist = nullptr);
    void renderOverlay();

    struct PlacedMesh
    {
        void* mesh = nullptr;
        uint32_t varHash = 0;
        fb::Vec3 pos{ };
    };
    bool aimedPlacedMesh(PlacedMesh& out);
    bool currentMesh(PlacedMesh& out); // what the Meshes panel is locked on or aiming at
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
    inline bool aimRayEnabled = false; // physics ray each update tick; the Meshes panel turns it on
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
