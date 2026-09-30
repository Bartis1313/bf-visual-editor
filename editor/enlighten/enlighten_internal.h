#pragma once

#include "enlighten.h"
#include "../../SDK/fb.h"
#include <d3d11.h>
#include <string>
#include <vector>

namespace editor::enlighten::detail
{
    constexpr int SLOT_COUNT = 3; // luma, chroma, direction

    struct Original
    {
        fb::DxTexture* tex = nullptr;
        uint32_t width = 0, height = 0;
        uint32_t dxgi = 0;
        uint32_t bpp = 0;
        std::vector<uint8_t> pixels; // mip 0, tightly packed
        bool read = false;
    };

    // bx blue share = shader x, by red share = shader y
    void chromaBytes(uint32_t dxgi, int& bx, int& by);
    uint32_t typedFormat(uint32_t dxgi);
    uint32_t bytesPerPixel(uint32_t dxgi); // 0 = unsupported format

    extern void* g_staticAsset;
    extern std::string g_staticName;
    extern bool g_dynamicEnable;
    extern bool g_scanned;
    extern Original g_orig[SLOT_COUNT];
    extern fb::DxTexture* g_skyVisTex; // EnlightenDataAsset::SkyVisibilityTexture, BC4 atlas
    fb::EnlightenRuntimeSettings* runtimeSettings();

    // BF4 0x1426724C0, header bytes untouched
#if defined(BFVE_GAME_BF4)
    constexpr size_t RUNTIME_BEGIN = 0x20;
    constexpr size_t RUNTIME_SIZE = 0xA0;
#else
    constexpr size_t RUNTIME_BEGIN = 0x0C; // after m_Name
    constexpr size_t RUNTIME_SIZE = 0x60;
#endif
    struct RuntimeEdit
    {
        bool captured = false;
        bool enabled = false;
        alignas(16) uint8_t orig[RUNTIME_SIZE] = {};
        alignas(16) uint8_t edit[RUNTIME_SIZE] = {};
        int kickFrames = 0;
        int kickTotal = 0;
        int kickLength = 60;
        int probeForceFrames = 0;
        int refreshFrames = 0;
    };
    extern RuntimeEdit g_rt;
    // VE update thread
    struct VeEnlighten { fb::Vec3 terrain{}, albedo{}; float bounce = 0.0f, sun = 0.0f; bool valid = false; };
    extern VeEnlighten g_veEnl;
    void runtimeKick(int frames);
    void runtimeRefresh(int frames);
    inline bool g_measuringReference = false;
    inline uint32_t g_veUpdates = 0;
    void runtimeRevert();
    extern json g_pendingRuntime;
    void applyRuntimeJson(const json& r);

#if defined(BFVE_GAME_BF4)
#define ENL_RT_FLOATS(X) \
    X(m_TemporalCoherenceThreshold) \
    X(m_SkyBoxScale) \
    X(m_MaxPerFrameSolveTime) \
    X(m_CubeMapForceGlobalScale) \
    X(m_LocalLightForceRadius) \
    X(m_DrawDebugLightProbeSize)
#define ENL_RT_UINTS(X) \
    X(m_MinSystemUpdateCount) \
    X(m_JobCount) \
    X(m_CubeMapMaxUpdateCount) \
    X(m_CubeMapConvolutionSampleCount) \
    X(m_LightProbeMaxSourceSolveCount) \
    X(m_LightProbeMaxInstanceUpdateCount) \
    X(m_LightProbeLookupTableGridRes)
#define ENL_RT_INTS(X) \
    X(m_DrawDebugSystemDependenciesEnable) \
    X(m_DrawDebugSystemBoundingBoxEnable)
#define ENL_RT_BOOLS_SOLVER(X) \
    X(m_Enable, "Enable") \
    X(m_ForceDynamic, "Force dynamic") \
    X(m_JobsEnable, "Jobs") \
    X(m_ForceUpdateStaticLightingBuffersEnable, "Force update static lighting buffers")
#define ENL_RT_BOOLS_INPUT(X) \
    X(m_ShadowsEnable, "Shadows") \
    X(m_SpotLightShadowsEnable, "Spot light shadows") \
    X(m_CompensateSunShadowHeightScale, "Compensate sun shadow height scale") \
    X(m_AlbedoForceUpdateEnable, "Albedo force update") \
    X(m_AlbedoForceColorEnable, "Albedo force color") \
    X(m_TerrainMapEnable, "Terrain map") \
    X(m_EmissiveEnable, "Emissive")
#define ENL_RT_BOOLS_OUTPUT(X) \
    X(m_LightMapsEnable, "Light maps") \
    X(m_LightProbeEnable, "Light probes") \
    X(m_LightProbeNewSamplingEnable, "Light probe new sampling") \
    X(m_LightProbeForceUpdate, "Light probe force update") \
    X(m_LightProbeJobsEnable, "Light probe jobs") \
    X(m_CubeMapsEnable, "Cube maps") \
    X(m_CubeMapMip0OnlyEnable, "Cube map mip0 only") \
    X(m_CubeMapCpuMipMapGenerationEnable, "Cube map CPU mips") \
    X(m_CubeMapConvolutionEnable, "Cube map convolution") \
    X(m_LocalLightsEnable, "Local lights") \
    X(m_LocalLightCullingEnable, "Local light culling") \
    X(m_LocalLightCustumFalloff, "Local light custom falloff")
#define ENL_RT_BOOLS_DEBUG(X) \
    X(m_DrawDebugCubeMaps, "Cube maps") \
    X(m_DrawDebugEntities, "Entities") \
    X(m_DrawDebugSystemsEnable, "Systems") \
    X(m_DrawDebugLightProbes, "Light probes") \
    X(m_DrawDebugLightProbeGrid, "Light probe grid") \
    X(m_DrawDebugLightProbeOcclusion, "Light probe occlusion") \
    X(m_DrawDebugLightProbeStats, "Light probe stats") \
    X(m_DrawDebugLightProbeBoundingBoxes, "Light probe bounds") \
    X(m_DrawSolveTaskPerformance, "Solve task performance") \
    X(m_DrawDebugColoringEnable, "Debug coloring") \
    X(m_DrawDebugTextures, "Textures") \
    X(m_DrawDebugBackFaces, "Back faces") \
    X(m_DrawDebugTargetMeshes, "Target meshes") \
    X(m_DrawWarningsEnable, "Warnings")
#else
#define ENL_RT_FLOATS(X) \
    X(m_TemporalCoherenceThreshold) \
    X(m_SkyBoxScale) \
    X(m_LocalLightForceRadius) \
    X(m_DrawDebugLightProbeSize)
#define ENL_RT_UINTS(X) \
    X(m_MinSystemUpdateCount) \
    X(m_JobCount) \
    X(m_LightProbeMaxUpdateSolveCount)
#define ENL_RT_INTS(X) \
    X(m_DrawDebugSystemDependenciesEnable) \
    X(m_DrawDebugSystemBoundingBoxEnable)
#define ENL_RT_BOOLS_SOLVER(X) \
    X(m_Enable, "Enable") \
    X(m_ForceDynamic, "Force dynamic")
#define ENL_RT_BOOLS_INPUT(X) \
    X(m_ShadowsEnable, "Shadows") \
    X(m_CompensateSunShadowHeightScale, "Compensate sun shadow height scale") \
    X(m_AlbedoForceUpdateEnable, "Albedo force update") \
    X(m_AlbedoForceColorEnable, "Albedo force color") \
    X(m_TerrainMapEnable, "Terrain map") \
    X(m_EmissiveEnable, "Emissive")
#define ENL_RT_BOOLS_OUTPUT(X) \
    X(m_LightMapsEnable, "Light maps") \
    X(m_LightProbeEnable, "Light probes") \
    X(m_LightProbeForceUpdate, "Light probe force update") \
    X(m_LightProbeJobsEnable, "Light probe jobs") \
    X(m_LocalLightsEnable, "Local lights") \
    X(m_LocalLightCullingEnable, "Local light culling") \
    X(m_LocalLightCustumFalloff, "Local light custom falloff")
#define ENL_RT_BOOLS_DEBUG(X) \
    X(m_DrawDebugEntities, "Entities") \
    X(m_DrawDebugSystemsEnable, "Systems") \
    X(m_DrawDebugLightProbes, "Light probes") \
    X(m_DrawDebugLightProbeOcclusion, "Light probe occlusion") \
    X(m_DrawDebugLightProbeStats, "Light probe stats") \
    X(m_DrawDebugLightProbeBoundingBoxes, "Light probe bounds") \
    X(m_DrawSolveTaskPerformance, "Solve task performance") \
    X(m_DrawDebugColoringEnable, "Debug coloring") \
    X(m_DrawDebugTextures, "Textures") \
    X(m_DrawDebugBackFaces, "Back faces") \
    X(m_DrawDebugTargetMeshes, "Target meshes") \
    X(m_DrawWarningsEnable, "Warnings")
#endif
#define ENL_RT_BOOLS(X) ENL_RT_BOOLS_SOLVER(X) ENL_RT_BOOLS_INPUT(X) ENL_RT_BOOLS_OUTPUT(X) ENL_RT_BOOLS_DEBUG(X)

    bool readback(Original& o, std::string& err); // render thread
#if defined(BFVE_GAME_BF4)
    fb::DxTexture* resolveAsset(const void* textureAsset);
#endif

    bool writeAtlasDds(const std::string& path, uint32_t w, uint32_t h, uint32_t dxgi, uint32_t bpp,
                       const std::vector<uint8_t>& pixels, std::string& err);
}
