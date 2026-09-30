#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace fb { class EnlightenRuntimeSettings; class EnlightenRuntimeDatabase; }

namespace editor::enlighten::db
{
    struct Loaded
    {
        std::string file;
        uint32_t width = 0, height = 0, instances = 0, systems = 0;
        uint32_t flags = 0;
        bool replaced = false, forcedDynamic = false;
    };

    inline bool edbOverrides = true;
    inline bool forceDynamic = false;
    inline bool engineSolver = false;
    inline bool liveProbes = true; // <map>_probes.bin
    inline float calibrationStrength = 1.0f; // 0 raw solve, 1 matched to shipped atlas
    inline float outputScale = 1.0f;

    // ctor hook, engine thread
    void* onConstruct(void* arena, int* resourceFlags, void* blob);
    const std::vector<Loaded>& loaded();
    std::string directory();
    std::string mapFileName();

    struct Captured
    {
        uint32_t flags = 0;
        std::vector<uint8_t> blob;
        std::vector<uint32_t> relocs;
    };
    void onLoaderLoad(void* request, void* buffers); // loader thread, before relocation
    bool capturedFor(uint32_t width, uint32_t height, Captured& out);
    bool writeEdb(const std::string& path, const Captured& c);

    // VE update thread, no level reload
    void applyLive(const std::string& path);

    void tickGameThread(); // VE update thread
    void tickRender(); // render thread
    void onLevelUnload();

    fb::EnlightenRuntimeDatabase* liveDatabase();

    // static probe sets and cubemaps held at budget 0
    void clampRuntime(fb::EnlightenRuntimeSettings* rs);

    // <map>_gain.bin, display output only, bounce unscaled
    std::string gainPath();
    void reloadGains(); // render thread
    void scaleOutput(float* irradiance, const unsigned short* luma); // solver job threads
    bool uploadSystemOutput(void* system); // BF3 copyIrradianceTexturesToGpu hook
    bool calibrate(std::string& note); // render thread, gain = shipped / raw
    // render thread
    void requestCalibration();
    void measureNow();
    // render thread, restore = shipped SH
    void scaleLevelProbes(float r, float g, float b);
    void restoreLevelProbes();
    bool calibrationPending();
    const std::string& calibrationNote();
    struct TexelGain { float gain = 1.0f, tint[3] = { 1.0f, 1.0f, 1.0f }, add[3] = {}, floorScale = 1.0f, raw[3] = {}; };
    bool gainAt(uint32_t x, uint32_t y, TexelGain& out);

    void renderUI();
}
