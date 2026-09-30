#pragma once

#include <cstdint>
#include <d3d11.h>
#include <string>
#include <vector>

namespace editor::textures::gen
{
    struct Params
    {
        float colorA[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // rgb multiply, a scales alpha
        float brightness = 1.0f;
        int upscale = 1;
        float sharpness = 0.5f;
        float detail = 0.25f;
    };
    const char* formatName(void* dxTexture);

    bool tintExisting(void* dxTexture, const Params& p, std::string& err);

    bool importImage(void* dxTexture, const std::string& path, std::string& err);

    inline bool pngOpaqueAlpha = false;

    bool exportPng(void* dxTexture, const std::string& path, std::string& err);

    bool exportDds(void* dxTexture, const std::string& path, std::string& err);
    bool importDds(void* dxTexture, const std::string& path, std::string& err);

    // takes ownership of tex/srv
    bool installCubeOverride(void* dxTexture, ID3D11Texture2D* tex, ID3D11ShaderResourceView* srv, std::string& err);

    void* cloneTexture(void* dxTexture);

    bool isClonedTexture(void* p);

    void* cloneSource(void* clone);

    void releaseClones();

    void onEngineReleasingViews(void* dxTexture);
    void onEngineCreatedViews(void* dxTexture);

    void reassertOverrides();
    void* originalView(void* dxTexture);

    using PathOfTexture = std::string (*)(void* dxTexture);
    using TextureOfPath = void* (*)(const std::string& path);
    void setAssetResolvers(PathOfTexture pathOf, TextureOfPath textureOf);
    void replayPending();

    bool revert(void* dxTexture);
    void revertAll();

    bool hasOverride(void* dxTexture);
    void* installedView(void* dxTexture);
    size_t overrideCount();

    struct EditInfo
    {
        void* dxTexture = nullptr;
        std::string sourceFile;
        bool tinted = false;
        Params params;
    };

    std::vector<EditInfo> listEdits();

    bool replayEdit(const EditInfo& edit, std::string& err);

    void* currentSrv(void* dxTexture);

    bool renderUI(void* dxTexture, const char* suggestedName = nullptr);

    std::string defaultExportDir();

    // any thread, maxSide 0 = all
    struct FileImage
    {
        uint32_t format = 0; // DXGI_FORMAT
        uint32_t width = 0, height = 0, mips = 0;
        int fullW = 0, fullH = 0;
        std::vector<uint8_t> data;
        std::vector<uint32_t> pitches; // per mip
        std::vector<uint32_t> offsets; // per mip
    };
    bool loadFileImage(const std::string& path, int maxSide, FileImage& out, std::string& err);
    void* createFileView(const FileImage& img, std::string& err); // render thread
    void releaseFileView(void* view);

    struct BatchItem
    {
        void* dxTexture = nullptr;
        std::string name; // no extension
    };

    void beginBatch(std::vector<BatchItem> items, bool asDds, const std::string& subDir);
    void tickBatch(); // render thread
    void cancelBatch();
    bool batchActive();
    size_t batchTotal();
    size_t batchDone();
    size_t batchFailed();
    std::string batchFolder();

    void shutdown();
}
