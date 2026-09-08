#pragma once

#include <cstdint>
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

    // Tint/recolor what the texture already shows, via a GPU pass over its current view.
    bool tintExisting(void* dxTexture, const Params& p, std::string& err);

    // Replace the texture with a decoded image file (PNG/JPG/BMP - anything WIC reads).
    bool importImage(void* dxTexture, const std::string& path, std::string& err);

    inline bool pngOpaqueAlpha = false;

    // Decode the texture to RGBA and write it out as a PNG, for editing elsewhere. The
    // encoding of the source view is preserved, so the PNG matches what the game shows.
    bool exportPng(void* dxTexture, const std::string& path, std::string& err);

    // Raw block copy to/from DDS - keeps the original compressed format and mip chain.
    bool exportDds(void* dxTexture, const std::string& path, std::string& err);
    bool importDds(void* dxTexture, const std::string& path, std::string& err);

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

    // Restore the original views for one texture, or all of them.
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

    // The generated linear view, so the caller can refresh its cached preview.
    void* currentSrv(void* dxTexture);

    // Parameter panel. Returns true if the texture was re-generated this frame.
    // `suggestedName` seeds the export filename so nothing has to be typed.
    bool renderUI(void* dxTexture, const char* suggestedName = nullptr);

    // Where exports land when the typed path has no directory of its own.
    std::string defaultExportDir();

    // CPU half of a file preview, safe on any thread. DDS keeps the mips from the first one
    // that still covers maxSide (0 = all); other formats decode to RGBA8 scaled to maxSide.
    struct FileImage
    {
        uint32_t format = 0; // DXGI_FORMAT
        uint32_t width = 0, height = 0, mips = 0;
        int fullW = 0, fullH = 0;
        std::vector<uint8_t> data;
        std::vector<uint32_t> pitches; // per mip
        std::vector<uint32_t> offsets; // per mip, into data
    };
    bool loadFileImage(const std::string& path, int maxSide, FileImage& out, std::string& err);
    void* createFileView(const FileImage& img, std::string& err); // render thread; the SRV
    void releaseFileView(void* view);

    struct BatchItem
    {
        void* dxTexture = nullptr;
        std::string name; // relative path under the export folder, no extension
    };

    void beginBatch(std::vector<BatchItem> items, bool asDds, const std::string& subDir);
    void tickBatch(); // render thread, once per frame
    void cancelBatch();
    bool batchActive();
    size_t batchTotal();
    size_t batchDone();
    size_t batchFailed();
    std::string batchFolder();

    // Waits for any in-flight file writes. Call before the DLL goes away.
    void shutdown();
}
