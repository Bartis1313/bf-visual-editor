#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <nlohmann/json.hpp>

#include "../../SDK/fb.h"
#include "../types.h"

using json = nlohmann::json;

namespace editor::textures
{
    void init();
    void shutdown();
    void clear();
    void renderTab();

    void tick();

    void requestRescan(int delayFrames = 120);
    void tickGameThread();

    void renderSkyTextureSlots();

    int renderMeshMaterials(const std::vector<MeshVariationRef>& meshes);
    int renderMeshTextures(const std::vector<MeshVariationRef>& meshes);

    struct SurfaceInfo
    {
        uint64_t setKey = 0;
        uint32_t material = 0;
        std::string meshName, shaderName, variationName;
        std::vector<std::string> textures;
        size_t own = 0;
        std::vector<std::pair<std::string, std::string>> slots;
        uint32_t frame = 0;
    };
    bool surfaceUnderCrosshair(SurfaceInfo& out);

    struct ShaderEdit
    {
        std::string graph;
        float original[4] = { };
        float value[4] = { };
    };
    std::vector<ShaderEdit> listShaderEdits();
    void queueShaderEdit(const ShaderEdit& e);

    size_t pendingShaderWork();
    std::vector<std::string> materialTextures(uint32_t meshHash, uint32_t material, size_t max);
    std::vector<std::string> texturesOfViews(void* const* srvs, size_t count, size_t max);
    std::vector<std::string> meshTextureNames(const MeshVariationRef& ref, size_t max);

    bool renderAssetTextureRow(void* textureAsset, float thumb = 48.0f);

    bool renderAssetTexturePicker(void** slot, const char* id);

    inline bool focusTab = false;
    inline bool focusMeshTab = false;

    void applySkyOverrides(fb::VisualEnvironment* ve);

    int skySlotCount();
    const char* skySlotLabel(int slot);
    int skySlotByLabel(const char* label);
    void setSkySlot(int slot, void* texture);
    void revertSkySlot(int slot);

    std::string texturePath(void* dxTexture);
    void* textureByPath(const std::string& path);
    const char* paramName(uint32_t handle);
    void catalogueCloneOf(void* clone, void* source);

    void applyPendingConfig();
    void clearPendingConfig();

    json serialize();
    void deserialize(const json& root);

    struct SkyOverride { bool enabled = false; void* asset = nullptr; };
    inline bool skyOverridesDirty = false;
    inline bool logicVeReloadRequested = false; // destroy+recreate the VE entity
    inline int veReloadPhase = 0; // 0 idle, 1 = apply Disable, 2 = apply Enable

    inline SkyOverride skyOverride[8];

    inline void* skyOriginal[8] = {};
    inline bool skyOriginalCaptured[8] = {};
    inline std::vector<std::pair<std::string, void*>> textureAssets;

    struct Layout
    {
#if defined(BFVE_GAME_BF4)
        uint32_t materialsOffset = 0x10;
        uint32_t countOffset = 0x18;
        uint32_t instanceStride = 0x50;
        uint32_t blockOffset = 0x08;
#else
        uint32_t materialsOffset = 0x08;
        uint32_t countOffset = 0x0C;
        uint32_t instanceStride = 0x0C;
        uint32_t blockOffset = 0x04;
#endif
    };

    struct TextureEntry
    {
        void* texture = nullptr; // fb::DxTexture*
        void* asset = nullptr; // fb::TextureAsset*
        void* vtable = nullptr;
        void* srvLinear = nullptr; // m_shaderViews[0]
        void* srvGamma = nullptr; // m_shaderViews[1]
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 0;
        uint32_t mips = 0;
        uint32_t type = 0; // 0=2d 1=cube 2=3d 3=array 5=1d
        uint32_t format = 0; // fb::TextureFormat
        uint32_t resourceFormat = 0; // DXGI_FORMAT
        uint32_t shaderFormat = 0; // DXGI_FORMAT
        bool srgb = false;
        bool drawable = false;
        bool viewChecked = false;
        bool nameCached = false;
        bool ownsSrv = false;
        void* resource = nullptr; // +0x98
        std::string shortName;
        std::string lowerPath;
        bool loaded = false;
        uint16_t handle = 0xFFFF;
        uint32_t lastDrawn = 0;
        uint32_t refs = 0;
    };

    struct MaterialEntry
    {
        void* set = nullptr;
        uint64_t setKey = 0;
        uint32_t index = 0;
        void* block = nullptr;
        uint8_t vecCount = 0;
        uint8_t texCount = 0;
        uint8_t boolCount = 0;
        std::string meshName;
        std::string variationName;
        std::vector<uint32_t> variationHandles;
        bool variationOverride = false;

        std::vector<std::pair<uint32_t, std::string>> ebxTextures;
        std::string shaderName;
        void* meshMaterial = nullptr;
        void* dbTexParams = nullptr;
        uint32_t dbTexCount = 0;
        std::string lowerName;
        std::string searchKey;
        std::string label;
        bool hasColor = false;
    };

    struct VecBackup { void* block; uint32_t index; float value[4]; };
    struct TexBackup { void* block; uint32_t index; void* texture; };

    struct ParamOverride
    {
        uint64_t setKey = 0;
        uint32_t material = 0;
        uint32_t handle = 0;
        bool isTexture = false;
        float value[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        void* texture = nullptr;

        void* block = nullptr;
        bool added = false;
    };

    inline std::vector<ParamOverride> paramOverrides;
    inline bool holdEdits = true;

    struct ScanStats
    {
        uint32_t sets = 0;
        uint32_t setsOk = 0;
        uint32_t setsNoMat = 0;
        uint32_t setsBadCnt = 0;
        uint32_t materials = 0;
        uint32_t blocksOk = 0;
        uint32_t texSlots = 0;
        uint32_t texUnique = 0;
        uint32_t vtableMatch = 0;
        uint32_t elementCount = 0;
        uint32_t namesFound = 0;
        uint32_t handlesTotal = 0;
        uint32_t handlesNamed = 0;
        uint32_t ebxEntries = 0;
        uint32_t matsNamed = 0;
        uint32_t texNamed = 0;
        uint32_t assetTextures = 0; // via +0x18
    };

    inline Layout layout;
    inline std::vector<TextureEntry> entries;
    inline std::vector<MaterialEntry> materials;
    inline std::unordered_map<uint32_t, std::string> handleNames;
    inline std::unordered_set<uint32_t> colorHandles;
    inline std::unordered_map<uint32_t, std::string> variationNames;
    inline std::unordered_map<void*, std::string> textureNames;
    inline std::vector<VecBackup> vecBackups;
    inline std::vector<TexBackup> texBackups;
    inline ScanStats stats;
    inline int selected = -1;
    inline int selectedMaterial = -1;
    inline int pickerSlot = -1;
    inline int minSize = 0;
    inline bool only2D = true;
    inline bool asColor = true;
    inline float thumbSize = 96.0f;
    struct Usage { int material; uint32_t slot; uint32_t handle; };

    inline std::vector<Usage> usages;
    inline int usagesFor = -1;
    inline char search[128] = {};
    inline float zoom = 1.0f;
    inline float panX = 0.0f;
    inline float panY = 0.0f;
    inline bool previewGamma = false;
    inline int replaceSlot = -1; // -1 = all
    enum ParamMode { ParamAuto = 0, ParamColor = 1, ParamNumeric = 2 };
    inline std::unordered_map<uint32_t, int> paramMode;

    struct MaterialFilter
    {
        char search[128] = {};
        bool colorOnly = true;
        int limit = 200;
        std::vector<int> matches;
        std::string cachedNeedle;
        bool cachedColorOnly = false;
        uint32_t cachedGeneration = ~0u;
    };

    inline MaterialFilter materialFilter;
    inline MaterialFilter lampFilter{ {}, true, 200 };

    inline bool lampColorOnly = true;
    inline bool lampThisVariationOnly = true;
    inline bool lampOverridesOnly = true;
    inline float bulkTint[3] = { 1.0f, 1.0f, 1.0f };
    inline float bulkTintScale = 1.0f;

    inline bool skyRelevantOnly = true;
    inline int skyEditSlot = -1;
    inline bool galleryThumbs = true;
    inline int maxThumbs = 96;
    inline uint32_t catalogGeneration = 0;
    inline bool includeAllAssets = true;
    inline bool keepUnloaded = true;
    inline bool showUnloaded = true;

    inline bool autoLoadMissing = false;

    inline int budgetTargetMb = 2048;
    inline bool autoRaiseBudget = false;
    inline int maxPins = 512;

    inline bool keepAllLoaded = true;

    inline bool autoResolveViews = false;
    inline int resolvePerFrame = 64;

    fb::TextureStreamingSettings* streamingSettings();

    // via fb::SettingsManager::applySettings
    void applyStreamingSettings();

    void loadAllMissing();
    size_t loadsPending();
    size_t loadsFinished();
    size_t loadsRequested();

    size_t texturesResident();
    size_t texturesCatalogued();

    int countTexturesMatching(const char* substr, bool residentOnly);
    int loadTexturesMatching(const char* substr);
    int tintTexturesMatching(const char* substr, const float rgb[3], float brightness);
    int revertTexturesMatching(const char* substr);

    void renderMaterialBrowser(MaterialFilter& filter);

    bool heldVecParam(const void* block, uint32_t handle, float (&out)[4]);

    void* meshSetByName(const char* name); // bf3

    void rescanNow();

    bool looksLikeParamBlock(void* block, uint8_t& vec, uint8_t& tex, uint8_t& bol);

    void* cloneParamBlockInto(void** slot);
    bool isClonedParamBlock(void* p);

    uint32_t paramHandle(const char* name);

    void registerParamName(const char* name);

    bool addVectorParam(const MaterialEntry& m, const char* name, const float* value);
    bool addVectorParamHandle(const MaterialEntry& m, uint32_t handle, const float* value);
    void removeAddedParam(const MaterialEntry& m, uint32_t handle);

    bool blockHasHandle(void* block, uint32_t handle);

    void setMirrorBlocks(const std::vector<void*>* blocks);

    using EditRedirect = void* (*)(void* block, void* user);
    void setEditRedirect(EditRedirect fn, void* user);

    bool selectTextureNamed(const char* path);

    bool renderTextureRowByPath(const char* path, const char* label);

    std::string lowerCopy(const std::string& s);
    void cacheName(TextureEntry& e);
    void renderLampTextureRow(TextureEntry& te, const char* label,
                               const std::string& path);

    uint32_t searchTextures(const char* needle);

    const char* nameForParamHandle(uint32_t handle);

    void* registerAssetTextureName(void* textureAsset, const char* path);

    int privatiseBlockTextures(void* block);
    void releaseClonedParamBlocks();

    void renderBlockParams(void* block, bool colorsOnly);

    inline bool autoScan = true;

    inline bool autoLoadAfterScan = false;
}
