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

    // Per-frame housekeeping: drains the on-demand load queue and runs a deferred
    // rescan. Cheap when there is nothing to do, so it is safe to call unconditionally.
    void tick();

    // Ask for a rescan a few frames from now. A level is not fully realized at the
    // moment it reports loaded, so scanning immediately finds almost nothing.
    void requestRescan(int delayFrames = 120);
    void tickGameThread();

    // The eight sky texture slots, for embedding in the VE component editor.
    void renderSkyTextureSlots();

    // Editors for every realized material of the given meshes. Returns how many matched.
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

    // One compact row for a fb::TextureAsset*: thumbnail, resolved path, and a button that
    // selects it in the Textures tab. False when the asset is not catalogued.
    bool renderAssetTextureRow(void* textureAsset, float thumb = 48.0f);

    bool renderAssetTexturePicker(void** slot, const char* id);

    // Set when something asks to be shown in the Textures tab; the tab bars honour it once.
    inline bool focusTab = false;
    inline bool focusMeshTab = false;

    // Called every frame after the VE blend, to re-assert sky texture overrides.
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
    inline std::vector<std::pair<std::string, void*>> textureAssets; // loaded TextureAssets

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
        void* texture = nullptr; // fb::DxTexture*, null until the asset resolves
        void* asset = nullptr; // fb::TextureAsset*, so we can resolve later
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
        bool drawable = false; // srvLinear is a plain TEXTURE2D view AND we hold a ref
        bool viewChecked = false; // GetDesc/AddRef done lazily, on first display
        bool nameCached = false; // shortName/lowerPath filled in
        bool ownsSrv = false; // srvLinear is a view we created ourselves
        void* resource = nullptr; // DxTexture::m_resource (+0x98)
        std::string shortName; // last path component, for display
        std::string lowerPath; // lowercased full path, for searching
        bool loaded = false; // seen in the most recent scan
        uint16_t handle = 0xFFFF; // DxTexture::m_handle, for streaming calls
        uint32_t lastDrawn = 0; // frame this was last displayed, for eviction
        uint32_t refs = 0; // material slots pointing at it
    };

    struct MaterialEntry
    {
        void* set = nullptr;
        uint64_t setKey = 0; // MeshVariationManager key of the owning set
        uint32_t index = 0; // index within the variation set
        void* block = nullptr; // ShaderParameterBlock*
        uint8_t vecCount = 0;
        uint8_t texCount = 0;
        uint8_t boolCount = 0;
        std::string meshName; // from the EBX pairing - the material's identity
        std::string variationName; // ObjectVariation this set was realized for
        std::vector<uint32_t> variationHandles;
        bool variationOverride = false;

        std::vector<std::pair<uint32_t, std::string>> ebxTextures;
        std::string shaderName;
        void* meshMaterial = nullptr;
        void* dbTexParams = nullptr;
        uint32_t dbTexCount = 0;
        std::string lowerName; // lowercased meshName, for exact matching
        std::string searchKey; // lowercased mesh + variation, for the search box
        std::string label;
        bool hasColor = false;
    };

    // Originals are captured on first write so every edit is revertible - these are live
    // engine structures, not our own copies.
    struct VecBackup { void* block; uint32_t index; float value[4]; };
    struct TexBackup { void* block; uint32_t index; void* texture; };

    struct ParamOverride
    {
        uint64_t setKey = 0;
        uint32_t material = 0;
        uint32_t handle = 0; // shader parameter handle
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
        uint32_t setsNoMat = 0; // materials pointer null/unreadable
        uint32_t setsBadCnt = 0; // count out of range
        uint32_t materials = 0;
        uint32_t blocksOk = 0;
        uint32_t texSlots = 0;
        uint32_t texUnique = 0;
        uint32_t vtableMatch = 0;
        uint32_t elementCount = 0; // manager's own count, for cross-check
        uint32_t namesFound = 0; // distinct parameter names harvested from EBX
        uint32_t handlesTotal = 0;
        uint32_t handlesNamed = 0; // how many resolved - also validates the hash
        uint32_t ebxEntries = 0; // MeshVariationDatabase entries harvested
        uint32_t matsNamed = 0; // materials that got a mesh name
        uint32_t texNamed = 0; // textures that got an asset path
        uint32_t assetTextures = 0; // added straight from TextureAssets via +0x18
    };

    inline Layout layout;
    inline std::vector<TextureEntry> entries;
    inline std::vector<MaterialEntry> materials;
    inline std::unordered_map<uint32_t, std::string> handleNames; // param handle -> param name
    // Parameters the EBX side declares as ShaderParameterType_Color. Authoritative, so no
    // guessing from the name is needed to know a Vec4 is a color.
    inline std::unordered_set<uint32_t> colorHandles;
    inline std::unordered_map<uint32_t, std::string> variationNames;
    inline std::unordered_map<void*, std::string> textureNames; // DxTexture*  -> asset path
    inline std::vector<VecBackup> vecBackups;
    inline std::vector<TexBackup> texBackups;
    inline ScanStats stats;
    inline int selected = -1;
    inline int selectedMaterial = -1;
    inline int pickerSlot = -1; // texture param awaiting a pick
    inline int minSize = 0;
    inline bool only2D = true;
    inline bool asColor = true;
    inline float thumbSize = 96.0f;
    // Where a given texture is referenced: material index + its texture slot.
    struct Usage { int material; uint32_t slot; uint32_t handle; };

    inline std::vector<Usage> usages; // for the selected texture
    inline int usagesFor = -1; // which texture `usages` describes
    inline char search[128] = {};
    inline float zoom = 1.0f; // texture preview zoom
    inline float panX = 0.0f;
    inline float panY = 0.0f;
    inline bool previewGamma = false;
    inline int replaceSlot = -1; // index into `usages`, -1 = all
    // Per-parameter display mode, keyed by handle. Vec4 params are not always colors -
    // plenty are UV offsets, tiling factors or sizes, where a color picker is useless.
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

    inline MaterialFilter materialFilter; // Textures -> Materials
    inline MaterialFilter lampFilter{ {}, true, 200 }; // Lights -> Lamp materials

    inline bool lampColorOnly = true;
    inline bool lampThisVariationOnly = true;
    // Show only the materials the ObjectVariation overrides. StreetLight_02_Destruction_ON
    // carries exactly one MeshMaterialVariation, setting exactly one parameter.
    inline bool lampOverridesOnly = true;
    inline float bulkTint[3] = { 1.0f, 1.0f, 1.0f };
    inline float bulkTintScale = 1.0f;

    inline bool skyRelevantOnly = true; // filter the sky picker
    inline int skyEditSlot = -1; // slot whose texture editor is open
    inline bool galleryThumbs = true; // grid of previews vs plain list
    inline int maxThumbs = 96; // images drawn per frame
    inline uint32_t catalogGeneration = 0; // bumped whenever entries change
    inline bool includeAllAssets = true; // catalogue every loaded TextureAsset
    inline bool keepUnloaded = true; // accumulate across scans
    inline bool showUnloaded = true;

    inline bool autoLoadMissing = false;

    inline int budgetTargetMb = 2048;
    inline bool autoRaiseBudget = false;
    inline int maxPins = 512;

    inline bool keepAllLoaded = true;

    inline bool autoResolveViews = false;
    inline int resolvePerFrame = 64;

    fb::TextureStreamingSettings* streamingSettings();

    // Push settings changes through fb::SettingsManager::applySettings, so the engine
    // reacts to them instead of finding them changed underneath it.
    void applyStreamingSettings();

    // Queue every texture that has no GPU data. Drained on the existing worker thread, a
    // batch at a time, so the frame never blocks on it.
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

    void* resourceByName(const char* name);

    // Re-catalogue now. Public so a tab that only borrows the browser can still fill it.
    void rescanNow();

    bool looksLikeParamBlock(void* block, uint8_t& vec, uint8_t& tex, uint8_t& bol);

    void* cloneParamBlockInto(void** slot);
    bool isClonedParamBlock(void* p);

    uint32_t paramHandle(const char* name);

    // Teach the editor a parameter name, so a block that uses it shows "Color" instead of a
    // hash. Names harvested from EBX go in the same table.
    void registerParamName(const char* name);

    bool addVectorParam(const MaterialEntry& m, const char* name, const float* value);
    bool addVectorParamHandle(const MaterialEntry& m, uint32_t handle, const float* value);
    void removeAddedParam(const MaterialEntry& m, uint32_t handle);

    // Does this block carry that parameter? Used to tell the block that belongs to a mesh's
    // material apart from the others hanging off the same entity.
    bool blockHasHandle(void* block, uint32_t handle);

    void setMirrorBlocks(const std::vector<void*>* blocks);

    using EditRedirect = void* (*)(void* block, void* user);
    void setEditRedirect(EditRedirect fn, void* user);

    // Jump to a texture by its asset path, the way a shader's texture binding names it.
    // Returns false when nothing in the catalogue carries that name.
    bool selectTextureNamed(const char* path);

    bool renderTextureRowByPath(const char* path, const char* label);

    std::string lowerCopy(const std::string& s);
    void renderLampTextureRow(TextureEntry& te, const char* label,
                               const std::string& path);

    uint32_t searchTextures(const char* needle);

    const char* nameForParamHandle(uint32_t handle);

    void* registerAssetTextureName(void* textureAsset, const char* path);

    int privatiseBlockTextures(void* block);
    void releaseClonedParamBlocks();

    // Parameter editor for such a block. Same backups, holds and reverts as a material.
    void renderBlockParams(void* block, bool colorsOnly);

    // Re-catalogue by itself once a level has settled, so a new map needs no Scan press.
    inline bool autoScan = true;

    inline bool autoLoadAfterScan = false;
}
