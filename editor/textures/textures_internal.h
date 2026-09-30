#pragma once
#include "textures.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <unordered_set>
#include <functional>

namespace editor::textures::detail
{
    uint8_t* liveInstance(uint64_t setKey, uint32_t materialIndex);
    bool materialHasColor(const MaterialEntry& m);
    std::string materialLabel(const MaterialEntry& m);

    uint32_t shaderParamHandle(const char* s);
    bool paramInfo(const void* block, uint32_t slot, uint32_t& handle, uint16_t& offset);
    uint8_t* paramValue(void* block, uint16_t offset);
    void* readTex(void* block, uint16_t offset);
    void writeVec(void* block, uint16_t offset, const float* v);
    void writeTex(void* block, uint16_t offset, void* t);
    bool blockIsSane(const void* block, uint8_t& vecOut, uint8_t& texOut, uint8_t& boolOut);
    uint32_t setMaterialCount(const uint8_t* set);
    uint8_t* setMaterials(const uint8_t* set);
    uint8_t* setBlock(const uint8_t* materials, uint32_t index);
    std::string assetName(const void* asset);
    const char* nameForHandle(uint32_t handle);
    bool paramIsColor(uint32_t handle, const char* name);
    bool containsCI(const char* haystack, const char* needle);
    const TextureEntry* findTextureEntry(void* texture);
    const char* textureLabel(const TextureEntry& e);
    std::string shortLabel(const TextureEntry& e);
    void ensureDrawable(TextureEntry& e, bool markShown = true);
    void selectTextureIndex(int index);
    void holdVecOverride(const MaterialEntry& m, uint32_t handle, const float* value);
    void holdTexOverride(const MaterialEntry& m, uint32_t handle, void* texture);
    void renderVectorParams(const MaterialEntry& m, bool colorsOnly, const std::vector<uint32_t>* only);
    void renderMaterialTextureSlots(const MaterialEntry& m);
    void renderEbxTextures(const MaterialEntry& m);


    struct SkySlotDesc { const char* label; uint32_t offset; };

// fb::CapturedSkyComponentData offsets = dumped SkyComponentData offsets - 0x80
#if defined(BFVE_GAME_BF4)
    inline constexpr SkySlotDesc SKY_SLOTS[] = {
        { "SkyGradientTexture", 0x38 },
        { "PanoramicTexture", 0x60 },
        { "PanoramicAlphaTexture", 0x68 },
        { "CloudLayerMaskTexture", 0x70 },
        { "CloudLayer1Texture", 0x98 },
        { "CloudLayer2Texture", 0xC0 },
        { "StaticEnvmapTexture", 0xC8 },
        { "CustomEnvmapTexture", 0xD8 },
    };
#else
    inline constexpr SkySlotDesc SKY_SLOTS[] = {
        { "SkyGradientTexture", 0x30 },
        { "PanoramicTexture", 0x54 },
        { "PanoramicAlphaTexture", 0x58 },
        { "CloudLayerMaskTexture", 0x60 },
        { "CloudLayer1Texture", 0x88 },
        { "CloudLayer2Texture", 0xB0 },
        { "StaticEnvmapTexture", 0xB4 },
        { "CustomEnvmapTexture", 0xC0 },
    };
#endif
    inline constexpr int SKY_SLOT_COUNT = int(sizeof(SKY_SLOTS) / sizeof(SKY_SLOTS[0]));

    struct SlotPick
    {
        void* block = nullptr;
        uint32_t slot = 0;
        uint16_t offset = 0;
        uint32_t handle = 0;
        uint64_t setKey = 0;
        uint32_t material = 0;
        void* original = nullptr;
        bool open = false;
    };

    extern std::atomic<bool> budgetHit;
    extern uint32_t frameCounter;
    extern uint32_t g_liveCount;
    extern std::vector<int> g_visible;
    extern int skyResPicker;
    extern char skyPickSearch[128];
    extern SlotPick slotPick;
    extern char slotPickSearch[128];

    const char* srvDimensionName(void* srv, uint32_t& format);
    void releaseSrv(void* srv);
    bool streamingTarget(void* dxTexture, fb::TextureStreamingManager*& mgr, uint16_t& handle);
    int onDemandStatus(void* dxTexture);
    bool releaseHandle(uint16_t handle);
    bool isPinned(uint16_t handle);
    void queueLoad(uint16_t handle);
    void addRefTexture(void* tex);
    void forEachVeState(const std::function<void(uint8_t*)>& fn);
    void*& skySlotRef(uint8_t* state, int slot);
    void* skyCurrent(int slot);
    void skyCaptureOriginal(int slot);
    void skyRevertSlot(int slot);
    bool looksSkyRelevant(const std::string& path);
    const char* typeName(uint32_t t);
    void harvestTextureAssets();
    void* resolveAssetTexture(void* textureAsset);
    void noteUncatalogued(void* dxTexture);
    int catalogueClone(void* clone, const TextureEntry& src);
    std::string exportNameFor(const TextureEntry& e);
    bool matchesSearch(const std::string& haystack, const char* needleRaw);
    void rebuildUsages();
    void writeTextureSlot(const Usage& u, void* newTexture);
    void refreshVisible();
    TexBackup* findTexBackup(void* block, uint32_t slot);
    void assignSlot(void* block, uint32_t slot, uint16_t offset, uint32_t handle, uint64_t setKey, uint32_t material, void* current, void* replacement);
    void revertSlot(void* block, uint32_t slot, uint16_t offset, uint32_t handle, uint64_t setKey, uint32_t material);

    inline fb::DxTexture* asTex(void* p) { return static_cast<fb::DxTexture*>(p); }
    extern std::mutex pinMutex;
    extern std::unordered_set<uint16_t> pinnedHandles;
    extern uint32_t originalBudgetCap;
    extern uint32_t residentTextures;
    void addRefSrv(void* srv);
    bool srvIsDrawable2D(void* srv);
    bool liveTexture(const void* texture);
    bool readBudget(uint32_t& used, uint32_t& cap);
    bool setBudgetCap(uint32_t bytes);
    void restoreBudgetCap();
    void cancelLoads();
    void releaseAllPins();
    void readVec(void* block, uint16_t offset, float* v);
    void scan();
    VecBackup* findVecBackup(void* block, uint32_t index);
    void dropOverride(uint64_t setKey, uint32_t material, uint32_t handle, void* block);
    ParamOverride* findOverride(uint64_t setKey, uint32_t material, uint32_t handle, void* block);
    bool slotForHandle(const MaterialEntry& m, uint32_t handle, bool wantTexture, uint16_t& offset);
    void revertAll();
    fb::SurfaceShaderInstance* instanceOf(const MaterialEntry& m);
    void* growBlockWithVector(fb::SurfaceShaderInstance* inst, uint32_t handle, const float* value);
    void restoreGrownBlock(fb::SurfaceShaderInstance* inst);
    void releaseGrownBlocks();
    void queueGrow(uint64_t setKey, uint32_t material, uint32_t handle, const float* value);
    void freeRetiredBlocks();
    int effectiveMode(uint32_t handle, const char* name, const float* v);
    void* redirectEdit(void* block);
    void mirrorWrite(void* edited, uint32_t handle, bool isTexture, const float* value, void* texture);
    void exportAllLoaded(bool asDds);

    void renderSlotPicker();

    void holdShaderPatches();
    void applyPendingShader();
    void clearShaderPatches();
    void renderShadersTab();
    void renderShaderNamedTextures(const MaterialEntry& m);

    struct ShaderParam { std::string name; uint32_t bytes; bool vertexOnly; uint32_t cbSlot; uint32_t offset; uint32_t handle; };
    const std::vector<ShaderParam>& shaderParamNames(const MaterialEntry& m);
    bool pickedConstant(const MaterialEntry& m, const ShaderParam& p, float out[4]);
}

namespace editor::textures
{
    void collectMeshTextures(const std::vector<MeshVariationRef>& meshes, std::vector<std::pair<uint32_t, std::string>>& found, bool& byName);
    extern void* g_lampEditTex;
    extern uint32_t g_lastScanElements;
}
