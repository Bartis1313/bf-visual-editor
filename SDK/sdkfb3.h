#pragma once

// this sdk uses dumped typeinfo because I was lazy to put everything here

#include <Windows.h>
#include <d3d11.h>
#include <cstdint>
#include <array>
#include <string>
#include <cstring>

#include "general.h"
#include "offsets.h"
#include "typeinfo.h"
#include "eastl.h"
#include "vfunc.h"
#include "typeinfo.h"

namespace fb
{
#include "../SDK_bf4/enums.h"

// guess since in bf4 it went as uint32, in bf3 it was enum
enum ShaderGBufferLayout
{
    ShaderGBufferLayout_Default,
    ShaderGBufferLayout_Test
};

}
#include "../SDK_bf4/declarations.h"
#include "../SDK_bf4/structs.h"

#define _DataContainer_
#define _Asset_
#define _LocalLightEntity_
#define _PointLightEntity_
#define _SpotLightEntity_
#define _VisualEnvironmentEntity_
#define _WorldRenderSettingsBase_

namespace fb
{
    class DataContainer : public ITypedObject
    {
    public:
        unsigned __int16 m_refCnt; //0x0008
        unsigned __int16 m_flags; //0x000A
        char _pad[4]; //0x000C

        fb::Guid* getInstanceGuid()
        {
            if ((m_flags & 0x100) == 0)
                return nullptr;
            return reinterpret_cast<fb::Guid*>(this) - 1;
        }

        bool is(const char* name)
        {
            fb::TypeInfo* typeinf = this->GetType();
            if (!typeinf) return false;
            return std::strcmp(typeinf->m_InfoData->m_Name, name) == 0;
        }

        bool isClassId(unsigned int classId)
        {
            fb::TypeInfo* typeinf = this->GetType();
            if (!typeinf || typeinf->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
                return false;
            return static_cast<fb::ClassInfo*>(typeinf)->m_ClassId == classId;
        }
    };

    class Asset : public DataContainer
    {
    public:
        static __inline Asset* DefaultInstance()
        {
            return (Asset*)0x0000000068A21D40;
        }
        static __inline unsigned int ClassId()
        {
            return 1491;
        }
        static __inline uintptr_t ClassInfoPtr()
        {
            return 0x0000000142BFF590;
        }

        char* m_Name; //0x0010

        std::string tryGetDebugName() const
        {
            if (!m_Name) return {};
            std::string name{ m_Name };
            const size_t pos = name.find_last_of("/\\");
            if (pos != std::string::npos) name = name.substr(pos + 1);
            return name;
        }
    };

#pragma pack(push, 16)
    class WorldRenderSettingsBase
    {
    public:
        static __inline WorldRenderSettingsBase* DefaultInstance()
        {
            return (WorldRenderSettingsBase*)0x0000000068A5F0C0;
        }
        static __inline unsigned int ClassId()
        {
            return 3465;
        }
        static __inline uintptr_t ClassInfoPtr()
        {
            return 0x0000000142C79070;
        }

        char padInherit[sizeof(DataContainer)]; // 0x0000
        float m_CullScreenAreaScale; //0x0010
        float m_ViewportScale; //0x0014
        char _0x0018[8];
        Vec3 m_MotionBlurClearColor; //0x0020
        Vec3 m_DynamicEnvmapDefaultPosition; //0x0030
        float m_ShadowViewportScale; //0x0040
        float m_ShadowmapMinFov; //0x0044
        float m_ShadowmapSizeZScale; //0x0048
        unsigned __int32 m_ShadowmapResolution; //0x004C
        unsigned __int32 m_ShadowmapQuality; //0x0050
        float m_ShadowmapPoissonFilterScale; //0x0054
        unsigned __int32 m_ShadowmapSliceCount; //0x0058
        float m_ShadowmapSliceSchemeWeight; //0x005C
        float m_ShadowmapFirstSliceScale; //0x0060
        float m_ShadowmapViewDistance; //0x0064
        float m_ShadowmapExtrusionLength; //0x0068
        float m_ShadowmapTransitionBlendAmount; //0x006C
        float m_MotionBlurScale; //0x0070
        float m_MotionBlurFixedShutterTime; //0x0074
        float m_MotionBlurMax; //0x0078
        float m_MotionBlurRadialBlurMax; //0x007C
        float m_MotionBlurNoiseScale; //0x0080
        unsigned __int32 m_MotionBlurQuality; //0x0084
        unsigned __int32 m_MotionBlurDebugMode; //0x0088
        unsigned __int32 m_MotionBlurMaxSampleCount; //0x008C
        float m_ForceMotionBlurDepthCutoff; //0x0090
        float m_ForceMotionBlurCutoffGradientScale; //0x0094
        float m_MotionBlurDepthCheckThreshold; //0x0098
        float m_MotionBlurDepthCheckMaxDistance; //0x009C
        unsigned __int32 m_MultisampleCount; //0x00A0
        float m_MultisampleThreshold; //0x00A4
        __int32 m_OnlyShadowmapSlice; //0x00A8
        WorldViewMode m_ViewMode; //0x00AC
        float m_XenonHdrColorScale; //0x00B0
        float m_XenonHdrColorScaleFactor; //0x00B4
        float m_SkyEnvmapFilterWidth; //0x00B8
        unsigned __int32 m_SkyEnvmapResolution; //0x00BC
        __int32 m_DrawDebugSkyEnvmapMipLevel; //0x00C0
        MipmapFilterMode m_SkyEnvmapFilterMode; //0x00C4
        unsigned __int32 m_SkyEnvmapSidesPerFrameCount; //0x00C8
        float m_DynamicEnvmapFilterWidth; //0x00CC
        unsigned __int32 m_DynamicEnvmapResolution; //0x00D0
        __int32 m_DrawDebugDynamicEnvmapMipLevel; //0x00D4
        MipmapFilterMode m_DynamicEnvmapFilterMode; //0x00D8
        unsigned __int32 m_HudTextureWidth; //0x00DC
        unsigned __int32 m_HudTextureHeight; //0x00E0
        bool m_ShadowmapsEnable; //0x00E4
        bool m_TransparencyShadowmapsEnable; //0x00E5
        bool m_TransparencyShadowmapsHalfRes; //0x00E6
        bool m_ShadowmapFixedMovementEnable; //0x00E7
        bool m_ShadowmapFixedDepthEnable; //0x00E8
        bool m_ShadowmapViewDistanceScaleEnable; //0x00E9
        bool m_ShadowmapCullVolumeEnable; //0x00EA
        bool m_ShadowmapAccumEnable; //0x00EB
        bool m_ShadowmapAccumBilinearEnable; //0x00EC
        bool m_ShadowmapTransitionBlendEnable; //0x00ED
        bool m_Ps3ShadowmapTilingEnable; //0x00EE
        bool m_Ps3Shadowmap16BitEnable; //0x00EF
        bool m_Ps3SpotLightShadowmap16BitEnable; //0x00F0
        bool m_DxShadowmap16BitEnable; //0x00F1
        bool m_DxSpotLightShadowmap16BitEnable; //0x00F2
        bool m_ApplyShadowmapsEnable; //0x00F3
        bool m_GenerateShadowmapsEnable; //0x00F4
        bool m_SimpleShadowmapsEnable; //0x00F5
        bool m_EmitterShadowingBlendToggle; //0x00F6
        bool m_EmitterShadowingManySamplesToggle; //0x00F7
        bool m_DxLinearDepth32BitFormatEnable; //0x00F8
        bool m_DxDepthBuffer32BitEnable; //0x00F9
        bool m_MotionBlurEnable; //0x00FA
        bool m_MotionBlurForceOn; //0x00FB
        bool m_MotionBlurOptimalStableVelocityFormula; //0x00FC
        bool m_MotionBlurStencilPassEnable; //0x00FD
        bool m_MotionBlurGeometryPassEnable; //0x00FE
        bool m_MotionBlurBackgroundPassEnable; //0x00FF
        bool m_MotionBlurCenteredEnable; //0x0100
        bool m_DrawDebugMultisampleClassify; //0x0101
        bool m_DrawTransparent; //0x0102
        bool m_DrawTransparentDecal; //0x0103
        bool m_Enable; //0x0104
        bool m_HdrEnable; //0x0105
        bool m_ReadOnlyDepthEnable; //0x0106
        bool m_ConsoleRenderTargetPoolSharingEnable; //0x0107
        bool m_Ps3RenderTargetPoolGBufferOffsetEnable; //0x0108
        bool m_FastHdrEnable; //0x0109
        bool m_XenonFloatDepthBufferEnable; //0x010A
        bool m_Ps3HdrClearEnable; //0x010B
        bool m_Ps3TilingEnable; //0x010C
        bool m_Ps3ZCullEnable; //0x010D
        bool m_Ps3SCullEnable; //0x010E
        bool m_Ps3ColorCompressionEnable; //0x010F
        bool m_Ps3TiledHalfTargetEnable; //0x0110
        bool m_Ps3TiledSSAOTargetsEnable; //0x0111
        bool m_FinalPostEnable; //0x0112
        bool m_OutputGammaCorrectionEnable; //0x0113
        bool m_ScreenEffectEnable; //0x0114
        bool m_DrawLightSources; //0x0115
        bool m_DrawSolidBoundingBoxes; //0x0116
        bool m_DrawLineBoundingBoxes; //0x0117
        bool m_DrawBoundingSpheres; //0x0118
        bool m_DrawFrustums; //0x0119
        bool m_DrawDebugShadowmaps; //0x011A
        bool m_DrawDebugSpotLightShadowmaps; //0x011B
        bool m_DrawDebugSkyEnvmap; //0x011C
        bool m_DrawDebugVelocityBuffer; //0x011D
        bool m_DrawDebugZBufferEnable; //0x011E
        bool m_DrawDebugHalfResEnvironment; //0x011F
        bool m_DrawDebugDistortion; //0x0120
        bool m_DrawDebugVisibleEntityTypes; //0x0121
        bool m_DrawDebugSkyTextures; //0x0122
        bool m_DrawDebugMarschnerTextures; //0x0123
        bool m_DrawDebugDof; //0x0124
        bool m_DrawDebugDofFullscreen; //0x0125
        bool m_DrawDebugHalfResHdrTargets; //0x0126
        bool m_WireframeEnable; //0x0127
        bool m_ZPassEnable; //0x0128
        bool m_OccluderMeshZPrepassEnable; //0x0129
        bool m_OccluderMeshZPrepassDebugEnable; //0x012A
        bool m_HalfResEnable; //0x012B
        bool m_ForceFullResEnable; //0x012C
        bool m_HalfResLensFlaresEnable; //0x012D
        bool m_ForegroundEnable; //0x012E
        bool m_ForegroundDepthClearEnable; //0x012F
        bool m_ForegroundZPassEnable; //0x0130
        bool m_ForegroundTransparentEnable; //0x0131
        bool m_BilateralHalfResCompositeEnable; //0x0132
        bool m_TiledHalfResCompositeEnable; //0x0133
        bool m_TiledHalfResStencilOccludersEnable; //0x0134
        bool m_SkyEnable; //0x0135
        bool m_SkyFogEnable; //0x0136
        bool m_SkyHeightFogEnable; //0x0137
        bool m_SkyForwardScatteringEnable; //0x0138
        bool m_DistortionEnable; //0x0139
        bool m_DistortionHalfResEnable; //0x013A
        bool m_Distortion8BitEnable; //0x013B
        bool m_DistortionTilingEnable; //0x013C
        bool m_StaticEnvmapEnable; //0x013D
        bool m_CustomEnvmapEnable; //0x013E
        bool m_CustomEnvmapMipmapClampEnable; //0x013F
        bool m_SkyEnvmapEnable; //0x0140
        bool m_SkyEnvmapMipmapGenEnable; //0x0141
        bool m_SkyEnvmapUpdateEnable; //0x0142
        bool m_SkyEnvmapForceUpdateEnable; //0x0143
        bool m_SkyEnvmapUse8BitTexture; //0x0144
        bool m_DynamicEnvmapEnable; //0x0145
        bool m_DynamicEnvmapMipmapGenEnable; //0x0146
        bool m_DrawDebugDynamicEnvmap; //0x0147
        bool m_HairCoverageEnable; //0x0148
        bool m_SetupJobEnable; //0x0149
        bool m_FinishSyncJobsFirstEnable; //0x014A
        bool m_PrepareDispatchListJobEnable; //0x014B
        char _0x014C[4];
    };//Size=0x0150
#pragma pack(pop)

}

namespace fb
{
    struct EntityIterableLink
    {
        EntityIterableLink* next; //0x0000
        EntityIterableLink* prev; //0x0008
    };//Size=0x0010

    // sub_1407DAFC0: no back-pointer, entity = node - offset
    constexpr intptr_t LENS_FLARE_LINK_OFFSET = 0x30;
    constexpr intptr_t STATIC_MODEL_LINK_OFFSET = 0x40;

    // head = ClassInfo + 16 * (realm + base) (sub_1407CD5D0, sub_1407DAFC0)
    constexpr uint32_t ITERABLE_BASES[] = { 6u, 8u };
    constexpr uint32_t ITERABLE_REALMS[] = { 0u, 1u };
    constexpr size_t GAME_WORLD_REALM = 40;

    inline void** iterableListHead(uintptr_t classInfo, uint32_t realm, uint32_t base)
    {
        return reinterpret_cast<void**>(classInfo + 16ull * (realm + base));
    }
}

#define _DxTexture_
namespace fb
{
    class DxTexture
    {
    public:
        static __inline unsigned int ClassId() { return 138; }
        static __inline uintptr_t ClassInfoPtr() { return 0x0000000142C6DEE0; }
        static __inline void* VTable() { return reinterpret_cast<void*>(OFF_vt_DxTexture); }

        void* m_vtable; //0x0000
        unsigned __int32 m_refCount; //0x0008
        char _0x000C[4];
        void* m_arena; //0x0010
        char _0x0018[12];
        unsigned __int32 m_memoryType; //0x0024 4 = system memory, pixels in m_data
        unsigned __int32 m_flags; //0x0028
        char _0x002C[4];
        unsigned __int32 m_width; //0x0030
        unsigned __int32 m_height; //0x0034
        unsigned __int32 m_depth; //0x0038
        unsigned __int32 m_sliceCount; //0x003C
        unsigned __int32 m_mipmapCount; //0x0040
        unsigned __int32 m_type; //0x0044 0 2D, 1 cube, 2 3D, 3 array, 5 1D
        unsigned __int32 m_format; //0x0048 TextureFormat
        bool m_srgb; //0x004C
        char _0x004D[7];
        unsigned __int32 m_bindFlags; //0x0054
        unsigned __int32 m_subResourceCount; //0x0058
        char _0x005C[4];
        void* m_subResourceData; //0x0060
        void* m_streamingHeader; //0x0068
        DxTexture* m_reuse; //0x0070
        char _0x0078[8];
        unsigned __int16 m_handle; //0x0080 0xFFFF = none
        char _0x0082[2];
        unsigned __int32 m_bindFlags2; //0x0084
        unsigned __int32 m_resFormat; //0x0088 DXGI_FORMAT
        unsigned __int32 m_shaderFormat; //0x008C DXGI_FORMAT
        void* m_streamingInfo; //0x0090 128 bytes
        ID3D11Resource* m_resource; //0x0098
        void* m_data; //0x00A0
        ID3D11ShaderResourceView* m_shaderViews[2]; //0x00A8 [0] linear, [1] sRGB
        bool m_ownsResource; //0x00B8
        char _0x00B9[7];
    };//Size=0x00C0
}

namespace fb
{
    // unloadOnDemand sub_140B8B140: entry = mgr + ((handle + 1) << 6)
    struct TextureStreamingEntry
    {
        DxTexture* m_texture; //0x0000
        void* m_streamData; //0x0008
        char _0x0010[10];
        unsigned __int16 m_mipRefCount[15]; //0x001A one per compartment
        unsigned __int8 m_mipCount; //0x0038
        char _0x0039[2];
        unsigned __int8 m_loadedMips; //0x003B
        unsigned __int16 m_residentMip; //0x003C
        unsigned __int8 m_state; //0x003E 2 = loaded
        unsigned __int8 m_flags; //0x003F
    };//Size=0x0040

    class TextureStreamingManager
    {
    public:
        char _0x0000[0x10];
        char m_lock[0x30]; //0x0010 sub_140670B20 / sub_1406724D0
        TextureStreamingEntry m_entries[0x2B00];//0x0040
        char _0xAC040[0x24];
        unsigned __int32 m_onDemandBudgetCap; //0xAC064 bytes
        char _0xAC068[0x20];
        unsigned __int32 m_onDemandBudgetUsed; //0xAC088 bytes

        static TextureStreamingManager* Singleton()
        {
            return *reinterpret_cast<TextureStreamingManager**>(OFF_g_textureStreamingManager);
        }

        TextureStreamingEntry* entry(unsigned __int16 handle)
        {
            return (handle & 0x7FFF) == handle ? &m_entries[handle] : nullptr;
        }

        // 0 not loaded, 1 loading, 2 loaded, 3 not on-demand
        int getOnDemandStatus(unsigned __int16 handle)
        {
            using fn_t = int(__fastcall*)(TextureStreamingManager*, unsigned __int16);
            return reinterpret_cast<fn_t>(OFF_TextureStreamingManager_getOnDemandStatus)(this, handle);
        }
        bool loadOnDemand(unsigned __int16 handle, bool highPriority)
        {
            using fn_t = bool(__fastcall*)(TextureStreamingManager*, unsigned __int16, unsigned __int8);
            return reinterpret_cast<fn_t>(OFF_TextureStreamingManager_loadOnDemand)(this, handle, highPriority ? 1 : 0);
        }
        bool unloadOnDemand(unsigned __int16 handle, bool defer)
        {
            using fn_t = bool(__fastcall*)(TextureStreamingManager*, unsigned __int16, unsigned __int8);
            return reinterpret_cast<fn_t>(OFF_TextureStreamingManager_unloadOnDemand)(this, handle, defer ? 1 : 0);
        }
    };
}

namespace fb
{
    // g_meshVariationManager (qword_1427382C8) hash node
    struct MeshVariationNode
    {
        unsigned __int64 m_key; //0x0000 variationHash | (meshNameHash << 32)
        void* m_set; //0x0008 MeshVariationSet*
        char _0x0010[8];
        MeshVariationNode* m_next; //0x0018
    };//Size=0x0020

    class MeshVariationManager
    {
    public:
        char _0x0000[0x30];
        void* m_arena; //0x0030 SurfaceShaderInstance::create arena (sub_140BA4600)
        char _0x0038[8];
        MeshVariationNode** m_buckets; //0x0040 m_bucketCount + 1, last is a sentinel
        unsigned __int32 m_bucketCount; //0x0048
        unsigned __int32 m_elementCount; //0x004C

        static MeshVariationManager* Singleton()
        {
            return *reinterpret_cast<MeshVariationManager**>(OFF_g_meshVariationManager);
        }
    };

    class SurfaceShaderInstance
    {
    public:
        static constexpr uintptr_t CREATE = OFF_SurfaceShaderInstance_create;

        void* m_shader; //0x0000 ISurfaceShader*, refcounted
        void* m_block; //0x0008 ShaderParameterBlock*
        unsigned __int16 m_blockSize; //0x0010
        char _0x0012[0x3E];
    };//Size=0x0050

    class SurfaceShader
    {
    public:
        struct ParamDesc // unverified, dword 0 is not the handle
        {
            char _0x0000[0x14];
        };//Size=0x0014

        void* m_vtable; //0x0000 0x141DCEA68
        unsigned __int32 m_refCount; //0x0008
        char _0x000C[4];
        unsigned __int32 m_nameHash; //0x0010 djb2 of lowercase graph name
        bool m_resolved; //0x0014
        char _0x0015[0x2B];
        unsigned __int16 m_vectorCount; //0x0040
        unsigned __int16 m_textureCount; //0x0042
        char _0x0044[4];
        ParamDesc* m_vectorDescs; //0x0048
        ParamDesc* m_textureDescs; //0x0050
        void* m_programs; //0x0058 ShaderProgramRef[m_programCount]
        unsigned __int32 m_programCount; //0x0060
        char _0x0064[0xC];
    };//Size=0x0070

    // ShaderDb vtable 0x141DCEDF0, loader sub_140BC9BB0
    // pixel count +0x204, array +0x228 stride 0x48
    class PixelShaderPermutation
    {
    public:
        char _0x0000[0x28];
        ID3D11PixelShader* m_shader; //0x0028 SmartRef
        char _0x0030[8];
        const unsigned char* m_data; //0x0038 DXBC
        unsigned __int32 m_dataSize; //0x0040
        char _0x0044[4];
    };//Size=0x0048

    // sub_140BB8210: data +0x40, size +0x48, stride 0x50
    class VertexShaderPermutation
    {
    public:
        char _0x0000[0x28];
        ID3D11VertexShader* m_shader; //0x0028
        char _0x0030[0x10];
        const unsigned char* m_data; //0x0040 DXBC
        unsigned __int32 m_dataSize; //0x0048
        char _0x004C[4];
    };//Size=0x0050

    class ShaderProgramEntry
    {
    public:
        char _0x0000[8];
        unsigned __int8 m_flags; //0x0008 >>4 = stream count
        unsigned __int8 m_pass; //0x0009
        unsigned __int8 m_subPass; //0x000A
        char _0x000B[5];
        void* m_desc; //0x0010
        VertexShaderPermutation* m_vertex; //0x0018
        PixelShaderPermutation* m_pixel; //0x0020 null for depth-only passes
        void* m_geometry; //0x0028
        void* m_hull; //0x0030
        void* m_domain; //0x0038
        char _0x0040[0x10];
    };//Size=0x0050

    struct ShaderProgramRef
    {
        void* m_key; //0x0000
        ShaderProgramEntry* m_entry; //0x0008
    };//Size=0x0010

    // setParameter sub_140C26490: value at m_values + m_offset
    class ShaderParameterBlock
    {
    public:
        struct Entry
        {
            unsigned __int32 m_handle; //0x0000
            unsigned __int16 m_offset; //0x0004 from m_values
            unsigned __int16 m_arraySize; //0x0006 1
        };//Size=0x0008

        unsigned __int16 m_sizeFlags; //0x0000 size = & 0x1FFF
        unsigned __int8 m_vectorCount; //0x0002
        unsigned __int8 m_textureCount; //0x0003
        unsigned __int8 m_boolCount; //0x0004
        char _0x0005[11];
        Entry m_entries[1]; //0x0010 values start here

        unsigned __int16 size() const { return m_sizeFlags & 0x1FFF; }
        char* values() { return reinterpret_cast<char*>(this) + 0x10; }
    };

}

#define _ShaderDatabase_
namespace fb
{
    // qword_14273A618, vtable[4](this, allocator, name) (sub_140CCCDA0)
    // cached by name, refcounted via vtable[0]/[1]
    class ShaderDatabase
    {
    public:
        static __inline unsigned int ClassId() { return 142; }
        static __inline uintptr_t ClassInfoPtr() { return 0x0000000142C6DC20; }

        void* m_vtable; //0x0000
        char _0x0008[504];

        static ShaderDatabase* Singleton()
        {
            return *reinterpret_cast<ShaderDatabase**>(OFF_g_shaderDatabase);
        }
        static void* Allocator()
        {
            return reinterpret_cast<void*>(OFF_g_shaderSystemAllocator);
        }
        static constexpr size_t FIND_SLOT = 4;
    };
}

#define _MeshSet_
namespace fb
{
    // FB2014, m_nameHash = djb2-lower of the asset name
    struct MeshSetLayout
    {
        Vec3 m_boundingMin; //0x0000
        Vec3 m_boundingMax; //0x0010
        void* m_lods[6]; //0x0020
        const char* m_name; //0x0050
        const char* m_shortName; //0x0058
        unsigned __int32 m_nameHash; //0x0060
        unsigned __int32 m_meshType; //0x0064
        unsigned __int32 m_flags; //0x0068
        unsigned __int16 m_lodCount; //0x006C
        unsigned __int16 m_subsetCount; //0x006E
    };//Size=0x0070

    struct MeshData
    {
        void* m_arena; //0x0000
        void* m_vertexBuffer; //0x0008 DxBuffer
        void* m_indexBuffer; //0x0010 DxBuffer
        const unsigned char* m_vertexData; //0x0018
        const unsigned char* m_indexData; //0x0020 m_vertexData + MeshLayout::m_vertexDataSize
        char _0x0028[8];
    };//Size=0x0030

    struct MeshSubset
    {
        void* m_geometryDecls; //0x0000
        const char* m_materialName; //0x0008
        unsigned __int32 m_materialIndex; //0x0010
        unsigned __int32 m_primitiveCount; //0x0014
        unsigned __int32 m_startIndex; //0x0018
        unsigned __int32 m_vertexOffset; //0x001C bytes into the LOD vertex block
        unsigned __int32 m_vertexCount; //0x0020
        unsigned __int32 _0x0024; //0x0024 not a size
        char _0x0028[8];
        unsigned __int32 m_declElements[16]; //0x0030
        unsigned __int8 m_streamStrides[8]; //0x0070
        unsigned __int8 m_elementCount; //0x0078
        unsigned __int8 m_streamCount; //0x0079
        char _0x007A[2];
        float m_texCoordsPerMeter[6]; //0x007C
        char _0x0094[0x2C]; //0x0094 zeros, then -1 words (bones)
    };//Size=0x00C0

    struct MeshLayout
    {
        unsigned __int32 m_type; //0x0000
        unsigned __int32 m_maxStreamInstanceCount; //0x0004
        unsigned __int32 m_subsetCount; //0x0008
        char _0x000C[8]; //0x000C MeshSubset*
        char _0x0014[0x30];
        unsigned __int32 m_flags; //0x0044
        unsigned __int32 m_indexBufferFormat; //0x0048
        unsigned __int32 m_indexDataSize; //0x004C
        unsigned __int32 m_vertexDataSize; //0x0050
        char _0x0054[0x3C];
        MeshData* m_data; //0x0090

        MeshSubset* subsets() const
        {
            MeshSubset* p = nullptr;
            std::memcpy(&p, _0x000C, sizeof(p));
            return p;
        }
    };

    // ctor sub_140B925B0, vtable off_141DC48D0, init sub_140B979E0
    class MeshSet
    {
    public:
        void* m_vtable; //0x0000
        unsigned __int32 m_refCount; //0x0008
        char _0x000C[4];
        void* m_arena; //0x0010
        char _0x0018[8];
        MeshSetLayout* m_layout; //0x0020
        const void* m_vertexIndexData; //0x0028
        char _0x0030[0x20];
        MeshData m_meshData[6]; //0x0050
        char _0x0170[0x50];
    };//Size=0x01C0
    static_assert(sizeof(MeshSubset) == 0xC0, "MeshSubset");
    static_assert(offsetof(MeshSubset, m_primitiveCount) == 0x14, "MeshSubset ranges");
    static_assert(offsetof(MeshSubset, m_declElements) == 0x30, "MeshSubset decl");
    static_assert(offsetof(MeshLayout, m_data) == 0x90, "MeshLayout data");
    static_assert(offsetof(MeshSet, m_meshData) == 0x50 && sizeof(MeshSet) == 0x1C0, "MeshSet");

    // registered by sub_140B9ECA0, found by sub_140691EC0
    class MeshSetRegistry
    {
    public:
        static void* Instance()
        {
            void* const owner = *reinterpret_cast<void**>(OFF_g_meshSetRegistryOwner);
            return owner ? *reinterpret_cast<void**>(static_cast<char*>(owner) + 0x10) : nullptr;
        }
        static constexpr uintptr_t FIND = OFF_MeshSetRegistry_find; // MeshSet* (__fastcall*)(void* registry, int hash)
    };
}

#define _LensFlareEntity_
namespace fb
{
    class LensFlareEntity
    {
    public:
        static __inline unsigned int ClassId() { return 896; }
        static __inline uintptr_t ClassInfoPtr() { return 0x0000000142C77330; }

        void* m_vtable; //0x0000
        char _0x0008[0x10];
        unsigned __int32 m_flags; //0x0018
        char _0x001C[4];
        void* m_world; //0x0020
        LensFlareEntityData* m_data; //0x0028
        EntityIterableLink m_link; //0x0030
        LinearTransform m_transform; //0x0040
        Vec3 m_direction; //0x0080 { 1, 0, 0 } at ctor
        Array<void*> m_elementShaders; //0x0090 refcounted, one per element
        char _0x0098[0x10];
        void* m_flareSystem; //0x00A8
        bool m_enabled; //0x00B0
        bool m_halfRes; //0x00B1
        bool m_directionEnable; //0x00B2
        char _0x00B3[13];
    };//Size=0x00C0
}

#define _ClientStaticModelEntity_
namespace fb
{
    class ClientStaticModelEntity
    {
    public:
        static __inline unsigned int ClassId() { return 686; }
        static __inline uintptr_t ClassInfoPtr() { return 0x0000000142C2FF20; }

        void* m_vtable; //0x0000
        char _0x0008[0x28];
        StaticModelEntityData* m_data; //0x0030
        char _0x0038[8];
        EntityIterableLink m_link; //0x0040
        char _0x0050[0x1F0];
        Vec3 m_position; //0x0240
    };//Size=0x0250
}

#define _ClientStaticModelGroupEntity_
namespace fb
{
    class StaticModelGroupMeshInstance
    {
    public:
        static constexpr uintptr_t RIGID_VTABLE = OFF_vt_RigidMeshEntity;
        static constexpr uintptr_t COMPOSITE_VTABLE = OFF_vt_CompositeMeshEntity;
        static constexpr size_t RIGID_SIZE = 0x80;
        static constexpr size_t COMPOSITE_SIZE = 0x90;

        void* m_vtable; //0x0000
        void* m_meshSet; //0x0008
        void* m_variation; //0x0010 MeshVariationSet*, refcounted
        void* m_next; //0x0018
        LinearTransform m_transform; //0x0020
        char _0x0060[0x20];
    };//Size=0x0080 rigid, 0x0090 composite

    // ctor sub_1408E9230, base sub_14099A090
    class StaticModelGroupMember
    {
    public:
        void* m_vtable; //0x0000
        char _0x0008[0x10];
        StaticModelGroupMemberData* m_data; //0x0018
        void* m_physics; //0x0020
        char _0x0028[0x98];
        void* m_meshSet; //0x00C0
        StaticModelGroupMeshInstance* m_instances; //0x00C8 m_data->m_InstanceCount, stride by vtable

        void localTransform(unsigned int instance, LinearTransform* out)
        {
            using fn_t = void (*)(StaticModelGroupMember*, unsigned int, LinearTransform*);
            reinterpret_cast<fn_t>(OFF_StaticModelGroupMember_getInstanceTransform)(this, instance, out);
        }
    };//Size=0x00D0

    // ctor sub_1408E90D0, size 0x108, link +0x40 (sub_1407CE570)
    // m_members filled by sub_1408F2B60, one per MemberData
    class ClientStaticModelGroupEntity
    {
    public:
        static __inline unsigned int ClassId() { return 682; }
        static __inline uintptr_t ClassInfoPtr() { return 0x0000000142C301A0; }

        void* m_vtable; //0x0000
        char _0x0008[0x28];
        StaticModelGroupEntityData* m_data; //0x0030
        char _0x0038[8];
        EntityIterableLink m_link; //0x0040
        char _0x0050[0x88];
        StaticModelGroupMember* m_membersBegin; //0x00D8
        StaticModelGroupMember* m_membersEnd; //0x00E0
        StaticModelGroupMember* m_membersCap; //0x00E8
        char _0x00F0[0x18];
    };//Size=0x0108
    constexpr intptr_t STATIC_MODEL_GROUP_LINK_OFFSET = 0x40;

    static_assert(sizeof(StaticModelGroupMeshInstance) == 0x80, "StaticModelGroupMeshInstance");
    static_assert(offsetof(StaticModelGroupMeshInstance, m_transform) == 0x20, "instance transform");
    static_assert(sizeof(StaticModelGroupMember) == 0xD0, "StaticModelGroupMember");
    static_assert(offsetof(StaticModelGroupMember, m_instances) == 0xC8, "member instances");
    static_assert(sizeof(ClientStaticModelGroupEntity) == 0x108, "ClientStaticModelGroupEntity");
    static_assert(offsetof(ClientStaticModelGroupEntity, m_membersBegin) == 0xD8, "group members");
}

#include "../SDK_bf4/classes.h"

namespace fb
{
    class EntityBus;

    class CapturedOutdoorLightComponentData
    {
    public:
        Vec3 m_SunColor; //0x0080
        Vec3 m_SkyColor; //0x0090
        Vec3 m_GroundColor; //0x00A0
        Realm m_Realm; //0x00B0
        float m_SunRotationX; //0x00B4
        float m_SunRotationY; //0x00B8
        float m_ShadowSunRotationX; //0x00BC
        float m_ShadowSunRotationY; //0x00C0
        float m_SkyLightAngleFactor; //0x00C4
        float m_SunSpecularScale; //0x00C8
        float m_SkyEnvmapShadowScale; //0x00CC
        float m_SunShadowHeightScale; //0x00D0
        char _0x00D4[4];
        TextureAsset* m_CloudShadowTexture; //0x00D8
        Vec2 m_CloudShadowSpeed; //0x00E0
        float m_CloudShadowSize; //0x00E8
        float m_CloudShadowCoverage; //0x00EC
        float m_CloudShadowExponent; //0x00F0
        float m_CloudShadowStartFade; //0x00F4
        float m_CloudShadowsFadeDistance; //0x00F8
        Vec2 m_CloudXZTranslation; //0x00FC
        TextureAddress m_CloudShadowAddressingMode; //0x0104
        float m_TranslucencyAmbient; //0x0108
        float m_TranslucencyScale; //0x010C
        float m_TranslucencyPower; //0x0110
        float m_TranslucencyDistortion; //0x0114
        bool m_Enable; //0x0118
        bool m_ShadowSunRotationEnable; //0x0119
        bool m_CloudShadowEnable; //0x011A
        bool m_CloudShadowIsTopDown; //0x011B
        char _0x011C[4];
    };

    class CapturedEnlightenComponentData
    {
    public:
        Vec3 m_TerrainColor; //0x0080
        Vec3 m_SkyBoxSkyColor; //0x0090
        Vec3 m_SkyBoxGroundColor; //0x00A0
        Vec3 m_SkyBoxSunLightColor; //0x00B0
        Vec3 m_SkyBoxBackLightColor; //0x00C0
        Vec3 m_OpaqueAlphaTestSimpleScale; //0x00D0
        Vec3 m_OverrideLightProbe; //0x00E0
        Realm m_Realm; //0x00F0
        float m_BounceScale; //0x00F4
        float m_SunScale; //0x00F8
        float m_CullDistance; //0x00FC
        float m_CullRadius; //0x0100
        float m_SkyBoxSunLightColorSize; //0x0104
        float m_SkyBoxBackLightColorSize; //0x0108
        float m_SkyBoxBackLightRotationX; //0x010C
        float m_SkyBoxBackLightRotationY; //0x0110
        char _0x0114[4];
        TextureAsset* m_OverrideIrradianceChromaTexture; //0x0118
        TextureAsset* m_OverrideIrradianceLumaTexture; //0x0120
        bool m_SkyBoxEnable; //0x0128
        char _0x0129[7];
    };

    class CapturedTonemapComponentData
    {
    public:
        Vec3 m_BloomScale; //0x0080
        Realm m_Realm; //0x0090
        TonemapMethod m_TonemapMethod; //0x0094
        float m_MiddleGray; //0x0098
        float m_MinExposure; //0x009C
        float m_MaxExposure; //0x00A0
        float m_ExposureAdjustTime; //0x00A4
        float m_ChromostereopsisScale; //0x00A8
        float m_ChromostereopsisOffset; //0x00AC
        bool m_ChromostereopsisEnable; //0x00B0
        char _0x00B1[15];
    };

    class CapturedColorCorrectionComponentData
    {
    public:
        Vec3 m_Brightness; //0x0080
        Vec3 m_Contrast; //0x0090
        Vec3 m_Saturation; //0x00A0
        Realm m_Realm; //0x00B0
        float m_Hue; //0x00B4
        TextureAsset* m_ColorGradingTexture; //0x00B8
        bool m_Enable; //0x00C0
        bool m_ColorGradingEnable; //0x00C1
        char _0x00C2[14];
    };

    class CapturedSkyComponentData
    {
    public:
        Vec3 m_CloudLayerSunColor; //0x0080
        Vec3 m_CloudLayer1Color; //0x0090
        Vec3 m_CloudLayer2Color; //0x00A0
        Realm m_Realm; //0x00B0
        float m_BrightnessScale; //0x00B4
        TextureAsset* m_SkyGradientTexture; //0x00B8
        float m_SunSize; //0x00C0
        float m_SunScale; //0x00C4
        float m_PanoramicUVMinX; //0x00C8
        float m_PanoramicUVMaxX; //0x00CC
        float m_PanoramicUVMinY; //0x00D0
        float m_PanoramicUVMaxY; //0x00D4
        float m_PanoramicTileFactor; //0x00D8
        float m_PanoramicRotation; //0x00DC
        TextureAsset* m_PanoramicTexture; //0x00E0
        TextureAsset* m_PanoramicAlphaTexture; //0x00E8
        TextureAsset* m_CloudLayerMaskTexture; //0x00F0
        float m_CloudLayer1Altitude; //0x00F8
        float m_CloudLayer1TileFactor; //0x00FC
        float m_CloudLayer1Rotation; //0x0100
        float m_CloudLayer1Speed; //0x0104
        float m_CloudLayer1SunLightIntensity; //0x0108
        float m_CloudLayer1SunLightPower; //0x010C
        float m_CloudLayer1AmbientLightIntensity; //0x0110
        float m_CloudLayer1AlphaMul; //0x0114
        TextureAsset* m_CloudLayer1Texture; //0x0118
        float m_CloudLayer2Altitude; //0x0120
        float m_CloudLayer2TileFactor; //0x0124
        float m_CloudLayer2Rotation; //0x0128
        float m_CloudLayer2Speed; //0x012C
        float m_CloudLayer2SunLightIntensity; //0x0130
        float m_CloudLayer2SunLightPower; //0x0134
        float m_CloudLayer2AmbientLightIntensity; //0x0138
        float m_CloudLayer2AlphaMul; //0x013C
        TextureAsset* m_CloudLayer2Texture; //0x0140
        TextureAsset* m_StaticEnvmapTexture; //0x0148
        float m_StaticEnvmapScale; //0x0150
        float m_SkyEnvmap8BitTexScale; //0x0154
        TextureAsset* m_CustomEnvmapTexture; //0x0158
        float m_CustomEnvmapScale; //0x0160
        float m_CustomEnvmapAmbient; //0x0164
        float m_SkyVisibilityExponent; //0x0168
        bool m_Enable; //0x016C
        bool m_IndirectCubeMapOverride; //0x016D
        char _0x016E[2];
    };

    class CapturedFogComponentData
    {
    public:
        Vec4 m_Curve; //0x0080
        Vec3 m_FogColor; //0x0090
        Vec4 m_FogColorCurve; //0x00A0
        Vec4 m_TransparencyFadeCurve; //0x00B0
        Vec3 m_ForwardLightScatteringColor; //0x00C0
        Realm m_Realm; //0x00D0
        float m_FogDistanceMultiplier; //0x00D4
        float m_Start; //0x00D8
        float m_End; //0x00DC
        float m_FogColorStart; //0x00E0
        float m_FogColorEnd; //0x00E4
        float m_TransparencyFadeStart; //0x00E8
        float m_TransparencyFadeEnd; //0x00EC
        float m_TransparencyFadeClamp; //0x00F0
        float m_ForwardLightScatteringPhaseG; //0x00F4
        float m_ForwardLightScatteringStrength; //0x00F8
        float m_ForwardLightScatteringPresence; //0x00FC
        float m_ForwardLightScatteringMaxBlurLength; //0x0100
        float m_ForwardLightScatteringExtinction; //0x0104
        float m_ForwardLightScatteringSmoothness; //0x0108
        float m_HeightFogFollowCamera; //0x010C
        float m_HeightFogAltitude; //0x0110
        float m_HeightFogDepth; //0x0114
        float m_HeightFogVisibilityRange; //0x0118
        bool m_Enable; //0x011C
        bool m_FogGradientEnable; //0x011D
        bool m_FogColorEnable; //0x011E
        bool m_ForwardLightScatteringEnabled; //0x011F
        bool m_HeightFogEnable; //0x0120
        char _0x0121[15];
    };

    class CapturedWindComponentData
    {
    public:
        Realm m_Realm; //0x0080
        float m_WindDirection; //0x0084
        float m_WindStrength; //0x0088
        float m_WindVariationMultiplier; //0x008C
        float m_WindVariationRateMultiplier; //0x0090
        float m_WindMicroVariationMultiplier; //0x0094
        float m_TurbulenceMultiplier; //0x0098
        float m_TurbulenceScale; //0x009C
    };

    class CapturedSunFlareComponentData
    {
    public:
        Vec4 m_Element1SizeOccluderCurve; //0x0080
        Vec4 m_Element1SizeScreenPosCurve; //0x0090
        Vec4 m_Element1AlphaOccluderCurve; //0x00A0
        Vec4 m_Element1AlphaScreenPosCurve; //0x00B0
        Vec4 m_Element1RotationDistCurve; //0x00C0
        Vec4 m_Element2SizeOccluderCurve; //0x00D0
        Vec4 m_Element2SizeScreenPosCurve; //0x00E0
        Vec4 m_Element2AlphaOccluderCurve; //0x00F0
        Vec4 m_Element2AlphaScreenPosCurve; //0x0100
        Vec4 m_Element2RotationDistCurve; //0x0110
        Vec4 m_Element3SizeOccluderCurve; //0x0120
        Vec4 m_Element3SizeScreenPosCurve; //0x0130
        Vec4 m_Element3AlphaOccluderCurve; //0x0140
        Vec4 m_Element3AlphaScreenPosCurve; //0x0150
        Vec4 m_Element3RotationDistCurve; //0x0160
        Vec4 m_Element4SizeOccluderCurve; //0x0170
        Vec4 m_Element4SizeScreenPosCurve; //0x0180
        Vec4 m_Element4AlphaOccluderCurve; //0x0190
        Vec4 m_Element4AlphaScreenPosCurve; //0x01A0
        Vec4 m_Element4RotationDistCurve; //0x01B0
        Vec4 m_Element5SizeOccluderCurve; //0x01C0
        Vec4 m_Element5SizeScreenPosCurve; //0x01D0
        Vec4 m_Element5AlphaOccluderCurve; //0x01E0
        Vec4 m_Element5AlphaScreenPosCurve; //0x01F0
        Vec4 m_Element5RotationDistCurve; //0x0200
        Realm m_Realm; //0x0210
        float m_OccluderSize; //0x0214
        SurfaceShaderBaseAsset* m_Element1Shader; //0x0218
        float m_Element1RayDistance; //0x0220
        Vec2 m_Element1Size; //0x0224
        float m_Element1RotationLocal; //0x022C
        float m_Element1RotationDistMultiplier; //0x0230
        char _0x0234[4];
        SurfaceShaderBaseAsset* m_Element2Shader; //0x0238
        float m_Element2RayDistance; //0x0240
        Vec2 m_Element2Size; //0x0244
        float m_Element2RotationLocal; //0x024C
        float m_Element2RotationDistMultiplier; //0x0250
        char _0x0254[4];
        SurfaceShaderBaseAsset* m_Element3Shader; //0x0258
        float m_Element3RayDistance; //0x0260
        Vec2 m_Element3Size; //0x0264
        float m_Element3RotationLocal; //0x026C
        float m_Element3RotationDistMultiplier; //0x0270
        char _0x0274[4];
        SurfaceShaderBaseAsset* m_Element4Shader; //0x0278
        float m_Element4RayDistance; //0x0280
        Vec2 m_Element4Size; //0x0284
        float m_Element4RotationLocal; //0x028C
        float m_Element4RotationDistMultiplier; //0x0290
        char _0x0294[4];
        SurfaceShaderBaseAsset* m_Element5Shader; //0x0298
        float m_Element5RayDistance; //0x02A0
        Vec2 m_Element5Size; //0x02A4
        float m_Element5RotationLocal; //0x02AC
        float m_Element5RotationDistMultiplier; //0x02B0
        bool m_Enable; //0x02B4
        bool m_DebugDrawOccluder; //0x02B5
        bool m_Element1Enable; //0x02B6
        bool m_Element1RotationAlignedToRay; //0x02B7
        bool m_Element2Enable; //0x02B8
        bool m_Element2RotationAlignedToRay; //0x02B9
        bool m_Element3Enable; //0x02BA
        bool m_Element3RotationAlignedToRay; //0x02BB
        bool m_Element4Enable; //0x02BC
        bool m_Element4RotationAlignedToRay; //0x02BD
        bool m_Element5Enable; //0x02BE
        bool m_Element5RotationAlignedToRay; //0x02BF
    };

    class CapturedDynamicAOComponentData
    {
    public:
        Realm m_Realm; //0x0080
        float m_SsaoFade; //0x0084
        float m_SsaoRadius; //0x0088
        float m_SsaoMaxDistanceInner; //0x008C
        float m_SsaoMaxDistanceOuter; //0x0090
        float m_HbaoRadius; //0x0094
        float m_HbaoAngleBias; //0x0098
        float m_HbaoAttenuation; //0x009C
        float m_HbaoContrast; //0x00A0
        float m_HbaoMaxFootprintRadius; //0x00A4
        float m_HbaoPowerExponent; //0x00A8
        bool m_Enable; //0x00AC
        char _0x00AD[3];
    };

    class CapturedDofComponentData
    {
    public:
        Realm m_Realm; //0x0080
        float m_FocusDistance; //0x0084
        float m_BlurFactor; //0x0088
        float m_BlurAdd; //0x008C
        BlurFilter m_SimpleDofBlurFilter; //0x0090
        float m_SimpleDofMaxBlur; //0x0094
        float m_SimpleDofNearStart; //0x0098
        float m_SimpleDofNearEnd; //0x009C
        float m_SimpleDofFarStart; //0x00A0
        float m_SimpleDofFarEnd; //0x00A4
        float m_HipToIronsightsFade; //0x00A8
        float m_IronsightsDofStartFade; //0x00AC
        float m_IronsightsFocalDistance; //0x00B0
        float m_IronsightsDofCircleDistance; //0x00B4
        float m_SpriteDofNearStart; //0x00B8
        float m_SpriteDofNearEnd; //0x00BC
        float m_SpriteDofFarStart; //0x00C0
        float m_SpriteDofFarEnd; //0x00C4
        float m_SpriteDofMaxBlur; //0x00C8
        char _0x00CC[4];
        TextureAsset* m_SpriteDofBokehTexture; //0x00D0
        bool m_Enable; //0x00D8
        bool m_DebugDrawFocusPlane; //0x00D9
        bool m_IronsightsDofActive; //0x00DA
        bool m_IronsightsDofCircleBlur; //0x00DB
        char _0x00DC[4];
    };

    class CapturedVignetteComponentData
    {
    public:
        Vec3 m_Color; //0x0080
        Realm m_Realm; //0x0090
        Vec2 m_Scale; //0x0094
        float m_Exponent; //0x009C
        float m_Opacity; //0x00A0
        bool m_Enable; //0x00A4
        char _0x00A5[11];
    };

    class CapturedFilmGrainComponentData
    {
    public:
        Vec3 m_ColorScale; //0x0080
        Realm m_Realm; //0x0090
        Vec2 m_TextureScale; //0x0094
        char _0x009C[4];
        TextureAsset* m_Texture; //0x00A0
        bool m_Enable; //0x00A8
        bool m_LinearFilteringEnable; //0x00A9
        bool m_RandomEnable; //0x00AA
        char _0x00AB[5];
    };

    class CapturedLensScopeComponentData
    {
    public:
        Vec3 m_ChromaticAberrationColor1; //0x0080
        Vec3 m_ChromaticAberrationColor2; //0x0090
        Realm m_Realm; //0x00A0
        float m_BlurScale; //0x00A4
        Vec2 m_BlurCenter; //0x00A8
        Vec2 m_ChromaticAberrationStrengths; //0x00B0
        Vec2 m_ChromaticAberrationDisplacement1; //0x00B8
        Vec2 m_ChromaticAberrationDisplacement2; //0x00C0
        Vec2 m_RadialBlendDistanceCoefficients; //0x00C8
        bool m_Enable; //0x00D0
        char _0x00D1[15];
    };

    class CapturedCameraParamsComponentData
    {
    public:
        Realm m_Realm; //0x0080
        float m_ViewDistance; //0x0084
        float m_NearPlane; //0x0088
        float m_SunShadowmapViewDistance; //0x008C
    };

    class CapturedScreenEffectComponentData
    {
    public:
        Vec4 m_ScreenEffectParams; //0x0080
        Realm m_Realm; //0x0090
        ScreenEffectFrameType m_FrameType; //0x0094
        SurfaceShaderBaseAsset* m_Shader; //0x0098
        float m_FrameWidth; //0x00A0
        float m_OuterFrameOpacity; //0x00A4
        float m_InnerFrameOpacity; //0x00A8
        float m_Angle; //0x00AC
    };

    class CapturedDamageEffectComponentData
    {
    public:
        Vec4 m_TopDamage; //0x0080
        Vec4 m_LeftDamage; //0x0090
        Vec4 m_BottomDamage; //0x00A0
        Vec4 m_RightDamage; //0x00B0
        Realm m_Realm; //0x00C0
        char _0x00C4[4];
        SurfaceShaderBaseAsset* m_Shader; //0x00C8
        float m_FrameWidth; //0x00D0
        float m_OuterFrameOpacity; //0x00D4
        float m_InnerFrameOpacity; //0x00D8
        float m_FallofTime; //0x00DC
        float m_MinDamagePercentageThreshold; //0x00E0
        float m_MaxOpacityDamagePercentage; //0x00E4
        float m_StartCriticalEffectHealthThreshold; //0x00E8
        float m_EndCriticalEffectHealthThreshold; //0x00EC
        bool m_DebugDamage; //0x00F0
        char _0x00F1[15];
    };

    class CapturedShaderParamsComponentData
    {
    public:
        Vec4 m_Value; //0x0080
        Realm m_Realm; //0x0090
        char _0x0094[4];
        char* m_ParameterName; //0x0098
    };

    class CapturedPlanarReflectionComponentData
    {
    public:
        Vec3 m_KeyColorReflection; //0x0080
        Vec3 m_SkyColorReflection; //0x0090
        Vec3 m_GroundColorReflection; //0x00A0
        Realm m_Realm; //0x00B0
        float m_GroundHeight; //0x00B4
        float m_ViewDistance; //0x00B8
        BlurFilter m_VerticalBlurFilter; //0x00BC
        float m_VerticalDeviation; //0x00C0
        BlurFilter m_HorizontalBlurFilter; //0x00C4
        float m_HorizontalDeviation; //0x00C8
        bool m_Enable; //0x00CC
        bool m_TerrainReflectionsEnable; //0x00CD
        bool m_SkyRenderEnable; //0x00CE
        bool m_OverideOutdoorLightColors; //0x00CF
    };

    class CapturedDynamicEnvmapComponentData
    {
    public:
        Vec3 m_KeyColorEnvmap; //0x0080
        Vec3 m_SkyColorEnvmap; //0x0090
        Vec3 m_GroundColorEnvmap; //0x00A0
        Realm m_Realm; //0x00B0
        bool m_Enable; //0x00B4
        bool m_TerrainReflectionsEnable; //0x00B5
        char _0x00B6[10];
    };

    class CapturedCharacterLightingComponentData
    {
    public:
        Vec3 m_TopLight; //0x0080
        Vec3 m_BottomLight; //0x0090
        Realm m_Realm; //0x00A0
        float m_CameraUpRotation; //0x00A4
        CharacterLightingMode m_CharacterLightingMode; //0x00A8
        float m_BlendFactor; //0x00AC
        float m_TopLightDirX; //0x00B0
        float m_TopLightDirY; //0x00B4
        float m_StartFadeDistance; //0x00B8
        float m_EndFadeDistance; //0x00BC
        float m_StartFadeOutDistance; //0x00C0
        float m_EndFadeOutDistance; //0x00C4
        bool m_CharacterLightEnable; //0x00C8
        bool m_FirstPersonEnable; //0x00C9
        bool m_LockToCameraDirection; //0x00CA
        char _0x00CB[5];
    };

    class CapturedVehicleLightingComponentData
    {
    public:
        Vec3 m_TopLight; //0x0080
        Vec3 m_BottomLight; //0x0090
        Realm m_Realm; //0x00A0
        float m_CameraUpRotation; //0x00A4
        VehicleLightingMode m_VehicleLightingMode; //0x00A8
        float m_BlendFactor; //0x00AC
        float m_TopLightDirX; //0x00B0
        float m_TopLightDirY; //0x00B4
        float m_StartFadeDistance; //0x00B8
        float m_EndFadeDistance; //0x00BC
        float m_StartFadeOutDistance; //0x00C0
        float m_EndFadeOutDistance; //0x00C4
        bool m_VehicleLightEnable; //0x00C8
        bool m_FirstPersonEnable; //0x00C9
        bool m_LockToCameraDirection; //0x00CA
        char _0x00CB[5];
    };

    class CapturedSubSurfaceScatteringComponentData
    {
    public:
        Vec3 m_SimpleSssColor; //0x0080
        Vec3 m_AdvancedSssMat0Width; //0x0090
        Vec3 m_AdvancedSssMat1Width; //0x00A0
        Vec3 m_AdvancedSssMat2Width; //0x00B0
        Vec3 m_AdvancedSssMat3Width; //0x00C0
        Vec3 m_AdvancedSssMat4Width; //0x00D0
        Realm m_Realm; //0x00E0
        float m_SimpleSssRolloffKeyLight; //0x00E4
        float m_SimpleSssRolloffLocalLight; //0x00E8
        bool m_AdvancedSssEnable; //0x00EC
        char _0x00ED[3];
    };

    class CapturedMotionBlurComponentData
    {
    public:
        Realm m_Realm; //0x0080
        float m_MotionBlurScale; //0x0084
        float m_MotionBlurCutoffRadius; //0x0088
        float m_CutoffGradientScale; //0x008C
        Vec2 m_RadialBlurCenter; //0x0090
        float m_RadialBlurOffset; //0x0098
        float m_RadialBlurScale; //0x009C
        bool m_MotionBlurEnable; //0x00A0
        bool m_MotionBlurCentered; //0x00A1
        bool m_RadialBlurEnable; //0x00A2
        char _0x00A3[13];
    };

    // built by sub_14066B600, read by sub_140665140
    class PropertyPair
    {
    public:
        void* m_override; //0x0000 bit 0 = override live
        void* m_value; //0x0008 &data->field
    };

    // ctor sub_140CC7EE0, base sub_1407C2510
    class LocalLightEntity : public ITypedObject
    {
    public:
        static __inline unsigned int ClassId()
        {
            return 879;
        }
        static __inline uintptr_t ClassInfoPtr()
        {
            return 0x0000000142C76D90;
        }

        char _0x0008[0x18];
        void* m_world; //0x0020
        LocalLightEntityData* m_data; //0x0028
        EntityIterableLink m_link; //0x0030
        PropertyPair m_color; //0x0040
        LinearTransform m_transform; //0x0050
        PropertyPair m_intensity; //0x0090
        PropertyPair m_enlightenColorScale; //0x00A0
        PropertyPair m_radius; //0x00B0
        PropertyPair m_particleColorScale; //0x00C0
        char _0x00D0[0x10];

        LocalLightEntityData* data() { return m_data; }

        LinearTransform& transform() { return m_transform; }
        const Vec3& position() { return m_transform.m_trans; }

        void setDirty()
        {
            using fn_t = void(__fastcall*)(LocalLightEntity*);
            reinterpret_cast<fn_t>(OFF_LocalLightEntity_setDirty)(this);
        }
    };//Size=0x00E0

    class PointLightEntity : public LocalLightEntity
    {
    public:
        static __inline unsigned int ClassId()
        {
            return 881;
        }
        static __inline uintptr_t ClassInfoPtr()
        {
            return 0x0000000142C76A70;
        }
    };

    class SpotLightEntity : public LocalLightEntity
    {
    public:
        static __inline unsigned int ClassId()
        {
            return 880;
        }
        static __inline uintptr_t ClassInfoPtr()
        {
            return 0x0000000142C769D0;
        }
        char spottail[32];
    };

    struct VisualEnvironmentState
    {
        float visibility;
        int priority;
        bool overrideVisibility;
        bool excluded;
        char pad_0x0A[0x2];
        int stateId;
        Vec4 activeEffectPosition;
        CapturedOutdoorLightComponentData* outdoorLight; // 0x20 (slot 4)
        CapturedEnlightenComponentData* enlighten; // 0x28 (slot 5)
        CapturedTonemapComponentData* tonemap; // 0x30 (slot 6)
        CapturedColorCorrectionComponentData* colorCorrection; // 0x38 (slot 7)
        CapturedSkyComponentData* sky; // 0x40 (slot 8)
        CapturedFogComponentData* fog; // 0x48 (slot 9)
        CapturedWindComponentData* wind; // 0x50 (slot 10)
        CapturedSunFlareComponentData* sunFlare; // 0x58 (slot 11)
        CapturedDynamicAOComponentData* dynamicAO; // 0x60 (slot 12)
        CapturedDofComponentData* dof; // 0x68 (slot 13)
        CapturedVignetteComponentData* vignette; // 0x70 (slot 14)
        CapturedFilmGrainComponentData* filmGrain; // 0x78 (slot 15)
        CapturedLensScopeComponentData* lensScope; // 0x80 (slot 16)
        CapturedCameraParamsComponentData* cameraParams; // 0x88 (slot 17)
        CapturedScreenEffectComponentData* screenEffect; // 0x90 (slot 18) not copied
        CapturedDamageEffectComponentData* damageEffect; // 0x98 (slot 19) not copied
        CapturedPlanarReflectionComponentData* planarReflection; // 0xA0 (slot 20)
        CapturedDynamicEnvmapComponentData* dynamicEnvmap; // 0xA8 (slot 21)
        CapturedCharacterLightingComponentData* characterLighting; // 0xB0 (slot 22)
        CapturedVehicleLightingComponentData* vehicleLighting; // 0xB8 (slot 23)
        CapturedSubSurfaceScatteringComponentData* subSurfaceScattering; // 0xC0 (slot 24)
        CapturedMotionBlurComponentData* motionBlur; // 0xC8 (slot 25)
        char _pad_0xD0[128]; // 0xD0 slots 26..44
        CapturedShaderParamsComponentData* shaderparams[3]; // 0x150
        char _pad_0x168[16]; // 0x168
        unsigned __int32 shaderparamsCount; // 0x170
        char _tail[528 - 0x174]; // pad to 0x210
    };

    class VisualEnvironmentEntity : public ClientGameComponentEntity
    {
    public:
        static __inline unsigned int ClassId()
        {
            return 714;
        }
        static __inline uintptr_t ClassInfoPtr()
        {
            return 0x0000000142C2FFC0;
        }

        VisualEnvironmentReferenceObjectData* m_referenceObjectData; // 0x60
        char pad[0x28]; // 0x68
        fb::VisualEnvironmentState m_state; // 0x90...0x210
    };

    struct ShProbe
    {
        __m128 shR;
        __m128 shG;
        __m128 shB;
        __m128 shA;
    };

    // update at OFF_VisualEnvironmentManager_update confirms paddings
    class VisualEnvironment
    {
    public:
        CapturedOutdoorLightComponentData outdoorLight; // 0
        char _pad_oL[176 - sizeof(CapturedOutdoorLightComponentData)];
        CapturedEnlightenComponentData enlighten; // 176
        char _pad_en[192 - sizeof(CapturedEnlightenComponentData)];
        CapturedTonemapComponentData tonemap; // 368
        char _pad_tm[80 - sizeof(CapturedTonemapComponentData)];
        CapturedColorCorrectionComponentData colorCorrection; // 448
        char _pad_cc[96 - sizeof(CapturedColorCorrectionComponentData)];
        CapturedSkyComponentData sky; // 544
        char _pad_sk[256 - sizeof(CapturedSkyComponentData)];
        CapturedFogComponentData fog; // 800
        char _pad_fg[192 - sizeof(CapturedFogComponentData)];
        CapturedWindComponentData wind; // 992
        char _pad_wd[48 - sizeof(CapturedWindComponentData)];
        CapturedSunFlareComponentData sunFlare; // 1040
        char _pad_sf[592 - sizeof(CapturedSunFlareComponentData)];
        CapturedDynamicAOComponentData dynamicAO; // 1632
        char _pad_ao[64 - sizeof(CapturedDynamicAOComponentData)];
        CapturedDofComponentData dof; // 1696
        char _pad_df[112 - sizeof(CapturedDofComponentData)];
        CapturedVignetteComponentData vignette; // 1808
        char _pad_vg[64 - sizeof(CapturedVignetteComponentData)];
        CapturedFilmGrainComponentData filmGrain; // 1872
        char _pad_fl[64 - sizeof(CapturedFilmGrainComponentData)];
        CapturedLensScopeComponentData lensScope; // 1936
        char _pad_ls[112 - sizeof(CapturedLensScopeComponentData)];
        CapturedCameraParamsComponentData cameraParams; // 2048
        char _pad_cp[32 - sizeof(CapturedCameraParamsComponentData)];
        CapturedScreenEffectComponentData screenEffect; // 2080
        char _pad_se[80 - sizeof(CapturedScreenEffectComponentData)];
        CapturedDamageEffectComponentData damageEffect; // 2160
        char _pad_de[144 - sizeof(CapturedDamageEffectComponentData)];
        char _pad_gap[752]; // 2304...3056 some scratchpad or whatever it is
        CapturedPlanarReflectionComponentData planarReflection; // 3056
        char _pad_pr[96 - sizeof(CapturedPlanarReflectionComponentData)];
        CapturedDynamicEnvmapComponentData dynamicEnvmap; // 3152
        char _pad_dm[80 - sizeof(CapturedDynamicEnvmapComponentData)];
        CapturedCharacterLightingComponentData characterLighting;// 3232
        char _pad_cl[96 - sizeof(CapturedCharacterLightingComponentData)];
        CapturedVehicleLightingComponentData vehicleLighting; // 3328
        char _pad_vl[96 - sizeof(CapturedVehicleLightingComponentData)];
        CapturedSubSurfaceScatteringComponentData subSurfaceScattering; // 3424
        char _pad_ss[128 - sizeof(CapturedSubSurfaceScatteringComponentData)];
        CapturedMotionBlurComponentData motionBlur; // 3552
        char _pad_mb[52 - sizeof(CapturedMotionBlurComponentData)];
        // unverified rest...
    };

    class VisualEnvironmentManager
    {
    public:
        static VisualEnvironmentManager* GetInstance()
        {
            return *(VisualEnvironmentManager**)OFF_g_visualEnvironmentManager;
        }

        char pad[0x30];
        eastl::vector<VisualEnvironmentState*> m_states;

        void setDirty(bool dirty)
        {
            *(bool*)((char*)this + 0x1FC8) = dirty;
        }

        // this is extra for states, it loops thru all then
        void forceStatesDirty()
        {
            *(bool*)((char*)this + 0x2560) = true;
        }

        // this is more deep renderer update
        void forceLutUpload()
        {
            *(bool*)((char*)this + 0x1FD0) = true;
        }

        // found in update, lights seem to call it as extra
        void rebakeCharacterLightingSH()
        {
            using Fn = void (__fastcall*)(VisualEnvironmentManager*);
            reinterpret_cast<Fn>(OFF_VisualEnvironmentManager_rebakeCharacterLightingSH)(this);
        }
        // found in update, lights seem to call it as extra
        void rebakeVehicleLightingSH()
        {
            using Fn = void (__fastcall*)(VisualEnvironmentManager*);
            reinterpret_cast<Fn>(OFF_VisualEnvironmentManager_rebakeVehicleLightingSH)(this);
        }

        fb::VisualEnvironment& getEnv()
        {
            return *(VisualEnvironment*)((char*)this + 0x1010);
        }
    };

    inline WorldRenderSettings* getWorldRenderSettingsGlobal()
    {
        return *(WorldRenderSettings**)OFF_g_worldRenderSettings;
    }

    class Screen
    {
    public:
        char pad[0x58];
        unsigned int m_Width;
        unsigned int m_Height;
        unsigned int m_WindowWidth;
        unsigned int m_WindowHeight;
        char pad2[0x88];
        IDXGISwapChain* m_pSwapChain;
    };

    class DxRenderer
    {
    public:
        char pad[0x38];
        Screen* m_pScreen;
        char pad2[0xC0];
        ID3D11Device* m_pDevice;
        ID3D11DeviceContext* m_pContext;

        static DxRenderer* GetInstance()
        {
            return *(DxRenderer**)OFF_g_dxRenderer;
        }
    };

    class IMouse
    {
    public:
        virtual void init();
        virtual void release();
        virtual void preFrameUpdate(float);
        virtual void update(float);
        virtual void updateBetweenFrames(float);
        virtual bool isDown(void*, bool);
        virtual bool wentDown(void*);
        virtual bool wentUp(void*);
        virtual float getLevel(void*, bool);
        virtual float getLevel(void*);
        virtual bool getBufferedAction(void*);
        virtual void enableCursorMode(bool, int);
        virtual void setCursorPosition(int, int);
        virtual void setCursorRect(bool, unsigned int, unsigned int, unsigned int, unsigned int);
        virtual void setMouseSensitivity(float);
        virtual void setUIOwnsInput(bool);

        enum class InputButtons : BYTE
        {
            IDB_Button_0 = 0,
            IDB_Button_1 = 1,
            IDB_Button_2 = 2,
            IDB_Button_3 = 3,
            IDB_Button_4 = 4,
            IDB_Button_5 = 5,
            IDB_Button_6 = 6,
            IDB_Button_7 = 7,
            IDB_Button_Undefined = 8
        };

    public:
        class MouseDevice
        {
        public:
            class MouseState
            {
            public:
                long x; //0x0000
                long y; //0x0004
                long z; //0x0008
                unsigned char buttons[0x8]; //0x000C
            };

            virtual void Function0(); // 0x0000
            virtual void Read(float SampleTime, bool VisualFrame); //0x0008
            virtual const char* Name(void); //0x0010
            virtual const char* Name(unsigned int); // 0x0018
            virtual unsigned int ControlCount(); // 0x0020
            virtual bool IsConnected(); // 0x0028
            virtual bool IsAnalogue(unsigned int); // 0x0030
            virtual float GetValue(unsigned int); // 0x0038

            char _0x0008[144];
            __int64 m_HWND; //0x0098
            __int64 m_pWindowProcedure; //0x00A0
            char _0x00A8[40];
            BYTE m_CursorMode; //0x00D0
            BYTE m_UIOwnsInput; //0x00D1
            BYTE m_ShowCursor; //0x00D2
            BYTE m_CursorConfined; //0x00D3
            BYTE m_HasOverflowed; //0x00D4
            BYTE m_UseRawMouseInput; //0x00D5
            BYTE m_WindowActive; //0x00D6
            char _0x00D7[25];
            MouseState m_Current; //0x00F0
            MouseState m_Buffer; //0x0104
            char _0x0118[24];

        }; //Size = 0x0130

        char _0x0000[16 - 8];
        MouseDevice* m_pDevice; // 0x0010

    };//Size=0x0040

    class IKeyboard
    {
    public:
        virtual void init();
        virtual void release();
        virtual void preFrameUpdate(float);
        virtual void update(float);
        virtual bool isDown(int);
        virtual bool wentDown(int);
        virtual bool wentUp(int);
        virtual void enableTypingMode(bool);
    };

    class InputCache;
    class Pad;
    class MotionController;

    class BorderInputNode
    {
    public:
        class InputCache
        {
        public:
            char _0x0000[4];
            float m_conceptCache[253]; //0x0004 indexed by InputConceptIdentifiers
        };

        virtual void Init();
        virtual void Exit();

        InputCache* m_inputCache; //0x0008
        char _0x0010[48];
        /*InputNode**/ void* m_pInputNode; //0x0040
        char _0x0048[8];
        IKeyboard* m_keyboard; //0x0050
        IMouse* m_mouse; //0x0058

        static BorderInputNode* GetInstance()
        {
            return *(BorderInputNode**)OFF_g_borderInputNode;
        }
    };//Size=0x0088

    class PlayerManager;
    class MaterialGridManager;
    class WorldRenderModule;
    class GameWorld;
    class ClientLevel;

    class ClientLevel
    {
    public:
        char pad[0x38];
        LevelData* m_levelData;
    };

    class ClientPlayer
    {
    public:
        char pad[0x14D0];
        void* m_controlledControllable;
        bool isAlive() { return m_controlledControllable != nullptr; }
    };

    class ClientPlayerManager
    {
    public:
        virtual void function();
        virtual eastl::vector<ClientPlayer*>* getPlayers();
        virtual eastl::vector<ClientPlayer*>* getSpectators();

        char pad[0x538];
        ClientPlayer* m_pLocalPlayer;
    };

    class ClientGameContext
    {
    public:
        char pad[0x28];
        class Level* m_pLevel;
        char pad2[0x30];
        ClientPlayerManager* m_pPlayerManager;

        static ClientGameContext* GetInstance()
        {
            return *(ClientGameContext**)OFF_g_clientGameContext;
        }
    };

    class ResourceManager
    {
    public:
        class Compartment
        {
        public:
            char pad[0x108];
            eastl::vector<fb::ITypedObject*> m_objects;
        };

        volatile uint32_t m_bundleLoadInProgress;
        std::array<ResourceManager::Compartment*, 3005> m_compartments;

        static ResourceManager* GetInstance()
        {
            return *(ResourceManager**)OFF_g_resourceManager;
        }
    };

    class Message : public ITypedObject
    {
    public:
        virtual void* dtor(void*);
        DWORD m_Category;
        DWORD m_Type;
        char pad_000C[20];
        bool m_ownedByMessageManager;
        char pad_0021[7];
    };

    struct EffectParams
    {
        union Value
        {
            __m128 v;
            float f;
            int i;
            bool b;
        };

        enum ParameterType : __int32
        {
            PTInvalid = 0x0,
            PTFloat = 0x1,
            PTInt = 0x2,
            PTBool = 0x3,
            PTVector = 0x4,
        };

        Value m_parameterValue[4];
        const char* m_parameterName[4];
        ParameterType m_parameterType[4];
        unsigned int m_paramCount;
        char pad[8];
    };

    inline void* g_lastEffectLevel = nullptr;

    class EffectManager
    {
    public:
        static EffectManager* GetInstance()
        {
            return *(EffectManager**)OFF_g_effectManager;
        }

        uint32_t stopEffect(uint32_t handle)
        {
            using fn_t = void (__fastcall*)(EffectManager*, uint32_t);
            auto fn = reinterpret_cast<fn_t>(OFF_EffectManager_stopEffect);
            fn(this, handle);
            return 0;
        }

        static void* entityGetLevel(void* entity)
        {
            if (!entity)
                return nullptr;

            using fn_t = void* (__fastcall*)(void*);
            return reinterpret_cast<fn_t>(OFF_Entity_getLevel)(entity);
        }

        void* getLevelForPlayEffect()
        {
            if (g_lastEffectLevel)
                return g_lastEffectLevel;

            return nullptr; // better, or get it somehow from some "good" effect ent
        }

#pragma optimize("", off)
        [[nodiscard]] uint32_t playEffect(fb::Asset* asset, fb::LinearTransform* tr, fb::EffectParams* /*params*/, bool isFirstPerson)
        {
            using fn_t = uint32_t(__fastcall*)(EffectManager*, Asset*, LinearTransform*,
                void* /*level*/, char /*flags*/, void* /*extraOword*/,
                const __m128* /*velVec*/, char /*trailingFlag*/);

            if (!asset)
                return 0;

            void* level = getLevelForPlayEffect();
            if (!level)
                return 0;

            __m128i extra = _mm_setzero_si128(); // a6: position override
            __m128 vel = _mm_setzero_ps(); // a7: velocity vector

            auto fn = reinterpret_cast<fn_t>(OFF_EffectManager_playEffect);
            return fn(this, asset, tr, level, static_cast<char>(isFirstPerson ? 0x8 : 0), &extra, &vel, static_cast<char>(0));
        }
#pragma optimize("", on)
    };

    class EmitterTemplate
    {
    public:
        char pad[0x14];
        eastl::vector<fb::ProcessorData*> m_processors;
        unsigned __int8 m_processorIndices[ProcessorTypeCount];
        char pad3[0x2];
        fb::Vec2 m_uvPos;
        fb::Vec2 m_uvSize;
        fb::Vec2 m_uvPosNm;
        fb::Vec2 m_uvSizeNm;

        template<typename T, typename E>
            requires std::is_enum_v<E>
        T* getProcessor(E e)
        {
            unsigned __int8 v1 = m_processorIndices[e];
            if (v1 == 0xFF) return nullptr;
            return (T*)m_processors[v1];
        }
    };

    class RenderView
    {
    public:
        char _0x0000[64];
        LinearTransform m_Transform;
        char _0x0080[52];
        float m_FovY;
        float m_DefaultFovY;
        float m_NearPlane;
        float m_FarPlane;
        float m_AspectRatio;
        float m_OrthoWidth;
        float m_OrthoHeight;
        char _0x00D0[16];
        uint32_t m_DirtyFlags; // 1 matrices, 2 transposes, 4 depth ratio, 8 frustum
        char _0x00E4[364];
        float m_FovX;
        float m_DepthHeightRatio;
        float m_FovScale;
        float m_FovScaleSquared;
        LinearTransform m_View;
        LinearTransform m_ViewTranspose;
        LinearTransform m_ViewInverse;
        LinearTransform m_Projection;
        LinearTransform m_ViewAtOrigin;
        LinearTransform m_ProjectionTranspose;
        LinearTransform m_ProjectionInverse;
        LinearTransform m_ViewProjection;
        LinearTransform m_ViewProjectionTranspose;
        LinearTransform m_ViewProjectionInverse;
    };//Size=0x04E0
    static_assert(sizeof(RenderView) == 0x4E0);

    // GameRenderer::createUpdateJob input, one per view
    class GameRenderViewParams
    {
    public:
        RenderView view; //0x0000
        RenderView prevView; //0x04E0
        RenderView secondaryStreamingView; //0x09C0
        bool secondaryStreamingViewEnable; //0x0EA0
        char _0x0EA1[0x2F];
        char firstPersonTransform[0x90]; //0x0ED0 RenderView::setTransform block
        char prevFirstPersonTransform[0x90]; //0x0F60
        bool visible; //0x0FF0
        float worldFadeAmount; //0x0FF4
        float blurAmount; //0x0FF8
        float dofFocusDistance; //0x0FFC
        char _0x1000[0x20];
    };//Size=0x1020
    static_assert(sizeof(GameRenderViewParams) == 0x1020);

    class GameRenderer
    {
    public:
        char pad[0x60];
        RenderView* m_pRenderView; // GameRenderViewParams[viewCount], view first

        static GameRenderer* Singleton()
        {
            return *(GameRenderer**)OFF_g_gameRenderer;
        }
    };

    inline void* getGameWorldBF4()
    {
        return *(void**)0x142668F90;
    }

    struct DebugRenderVertex
    {
        float x, y, z; //0x0000
        unsigned int color; //0x000C 0xAABBGGRR
        float nx, ny, nz; //0x0010
        unsigned int pad; //0x001C
    };//Size=0x0020

    class DebugRenderer
    {
    public:
        enum Geometry { Triangles3d = 0, Lines3d = 1, Triangles2d = 2, Lines2d = 3 };

        static DebugRenderer* Singleton()
        {
            using fn_t = DebugRenderer* (*)();
            return reinterpret_cast<fn_t>(OFF_DebugRenderer_singleton)();
        }
        template <typename Fn> Fn slot(size_t i) { return reinterpret_cast<Fn>((*reinterpret_cast<void***>(this))[i]); }

        bool getEnable() { return slot<bool (__fastcall*)(void*)>(3)(this); }
        void setTransparent(bool v) { slot<void (__fastcall*)(void*, bool)>(9)(this, v); }
        void setDoubleSided(bool v) { slot<void (__fastcall*)(void*, bool)>(10)(this, v); }
        void setDepthTest(bool v) { slot<void (__fastcall*)(void*, bool)>(11)(this, v); }
        void setDepthWrite(bool v) { slot<void (__fastcall*)(void*, bool)>(12)(this, v); }
        DebugRenderVertex* beginVertices(int type, unsigned int count)
        {
            return slot<DebugRenderVertex* (__fastcall*)(void*, int, unsigned int)>(19)(this, type, count);
        }
        void endVertices(int type, DebugRenderVertex* end)
        {
            slot<void (__fastcall*)(void*, int, DebugRenderVertex*)>(20)(this, type, end);
        }
        // sub_140B832F0, two unknown bools
        void drawText3d(const Vec3& pos, const char* text, unsigned int color, float scale)
        {
            slot<void (__fastcall*)(void*, const Vec3*, const char*, unsigned int, float, bool, bool)>(24)(this, &pos, text, color, scale, false, false);
        }
        void drawText2d(int x, int y, const char* text, unsigned int color, float scale)
        {
            slot<void (__fastcall*)(void*, int, int, const char*, unsigned int, float)>(25)(this, x, y, text, color, scale);
        }
    };

    class DebugRenderer2
    {
    public:
        static DebugRenderer2* Singleton()
        {
            using fn_t = DebugRenderer2* (*)();
            return reinterpret_cast<fn_t>(OFF_DebugRenderer2_singleton)();
        }
        // sub_14063E2E0
        void drawText(int x, int y, const char* text, unsigned int color, float scale)
        {
            using fn_t = void (__fastcall*)(DebugRenderer2*, int, int, const char*, unsigned int, float);
            reinterpret_cast<fn_t>(OFF_DebugRenderer2_drawText)(this, x, y, text, color, scale);
        }
        // sub_14063DEB0
        void drawLine2d(const float a[2], const float b[2], unsigned int color)
        {
            using fn_t = void (__fastcall*)(DebugRenderer2*, const float*, const float*, unsigned int);
            reinterpret_cast<fn_t>(OFF_DebugRenderer2_drawLine2d)(this, a, b, color);
        }
        void drawLineRect2d(const float min[2], const float max[2], unsigned int color)
        {
            using fn_t = void (__fastcall*)(DebugRenderer2*, const float*, const float*, unsigned int);
            reinterpret_cast<fn_t>(OFF_DebugRenderer2_drawLineRect2d)(this, min, max, color);
        }
        void drawRect2d(const float min[2], const float max[2], unsigned int color)
        {
            using fn_t = void (__fastcall*)(DebugRenderer2*, const float*, const float*, unsigned int);
            reinterpret_cast<fn_t>(OFF_DebugRenderer2_drawRect2d)(this, min, max, color);
        }
    };

    struct RayCastHit
    {
        Vec3 m_position; //0x0000 (16)
        Vec3 m_normal; //0x0010 (16)
        void* m_rigidBody; //0x0020 PhysicsEntityBase*
        void* m_material; //0x0028 MaterialContainerPair*
        unsigned __int32 m_part; //0x0030
        float m_lambda; //0x0034
        char _0x0038[0x38];
    };//Size=0x0070
    constexpr intptr_t LEVEL_HAVOK_PHYSICS = 0x120;
    constexpr intptr_t HAVOK_PHYSICS_RAY_CASTER = 0x58;
    // skip ragdolls, characters, phantoms
    constexpr unsigned int RAY_CAST_WORLD_ONLY = 0x10u | 0x20u | 0x80u;

    constexpr intptr_t GAME_WORLD_RAY_CASTER = 0x30;
    inline void* physicsRayCaster()
    {
        void* gw = getGameWorldBF4();
        return gw ? static_cast<uint8_t*>(gw) + GAME_WORLD_RAY_CASTER : nullptr;
    }

    // eastl::fixed_vector<PhysicsEntityBase const*, 8, false>
    struct PhysicsExcludeList
    {
        void** m_begin; //0x0000
        void** m_end; //0x0008
        void** m_capacity; //0x0010
        void* m_data[8]; //0x0018
    };

    // ClientControllableEntity vtable[60] getPhysicsEntity
    inline void* localSoldierPhysics()
    {
        ClientGameContext* ctx = ClientGameContext::GetInstance();
        if (!ctx || !ctx->m_pPlayerManager || !ctx->m_pPlayerManager->m_pLocalPlayer)
            return nullptr;
        void* soldier = ctx->m_pPlayerManager->m_pLocalPlayer->m_controlledControllable;
        if (!soldier)
            return nullptr;
        using fn_t = void* (__fastcall*)(void*);
        fn_t fn = (*static_cast<fn_t**>(soldier))[60];
        return fn(soldier);
    }

    inline bool physicsRayQuery(const Vec3& from, const Vec3& to, RayCastHit& hit, unsigned int flags,
                                void* exclude = nullptr)
    {
        void* caster = physicsRayCaster();
        if (!caster)
            return false;
        using fn_t = bool (__fastcall*)(void*, const char*, const void*, const void*, RayCastHit*, unsigned int, const void*);
        fn_t fn = (*static_cast<fn_t**>(caster))[0];
        alignas(16) float a[4] = { from.m_x, from.m_y, from.m_z, 0.0f };
        alignas(16) float b[4] = { to.m_x, to.m_y, to.m_z, 0.0f };
        PhysicsExcludeList ex{ };
        ex.m_begin = ex.m_data;
        ex.m_end = ex.m_data;
        ex.m_capacity = ex.m_data + 8;
        if (exclude)
            *ex.m_end++ = exclude;
        return fn(caster, "bfve", a, b, &hit, flags, exclude ? &ex : nullptr);
    }

    template <typename T>
    class EntityList
    {
        EntityIterableLink* m_first = nullptr;
        EntityIterableLink* m_current = nullptr;
        intptr_t m_offset = 0x40;

        static EntityIterableLink* getFirstIterableLink(void* classInfo, void* gameWorld)
        {
            using fn_t = EntityIterableLink* (__fastcall*)(void*, void*);
            auto fn = reinterpret_cast<fn_t>(OFF_EntityWorld_getFirstIterableLink);
            return (classInfo && gameWorld) ? fn(classInfo, gameWorld) : nullptr;
        }

    public:
        EntityList(void* classInfo, intptr_t offset = 0x40)
            : m_offset(offset)
        {
            void* gw = getGameWorldBF4();
            if (!gw) return;
            m_first = getFirstIterableLink(classInfo, gw);
            m_current = m_first;
        }

        T* nextOfKind()
        {
            if (!m_current)
                return nullptr;

            EntityIterableLink* link = m_current;
            m_current = m_current->next;
            if (m_current == m_first) m_current = nullptr;
            return reinterpret_cast<T*>(reinterpret_cast<intptr_t>(link) - m_offset);
        }
    };

}

namespace fb
{
    inline RenderView* getActiveRenderView(GameRenderer* gr)
    {
        return gr ? gr->m_pRenderView : nullptr;
    }

    inline LinearTransform* getViewMatrix(RenderView* v) { return v ? &v->m_View : nullptr; }
    inline LinearTransform* getViewMatrixInverse(RenderView* v) { return v ? &v->m_ViewInverse : nullptr; }
    inline LinearTransform* getProjectionMatrix(RenderView* v) { return v ? &v->m_Projection : nullptr; }
    inline LinearTransform* getViewProjectionMatrix(RenderView* v) { return v ? &v->m_ViewProjection : nullptr; }

    inline void updateRenderView(RenderView*) { }

    inline ClientPlayer* getLocalPlayer(ClientGameContext* ctx)
    {
        if (!ctx || !ctx->m_pPlayerManager)
            return nullptr;

        return ctx->m_pPlayerManager->m_pLocalPlayer;
    }

    inline const char* getCurrentLevelName(ClientGameContext* ctx)
    {
        if (!ctx || !ctx->m_pLevel)
            return nullptr;

        ClientLevel* lvl = reinterpret_cast<ClientLevel*>(ctx->m_pLevel);
        if (lvl->m_levelData)
            return lvl->m_levelData->m_Name;

        return nullptr;
    }

    inline WorldRenderSettings* getLiveWorldRenderSettings(ClientGameContext* /*ctx*/)
    {
        return *(WorldRenderSettings**)OFF_g_worldRenderSettings;
    }
}


// Enlighten, db ctor 0x140CF45F0, owner OFF_g_enlightenRenderer
namespace fb
{
#pragma pack(push, 4)
    // blob +0, pointers relocated to absolute
    class EnlightenDatabaseHeader
    {
    public:
        unsigned __int32 m_atlasWidth; //0x0000
        unsigned __int32 m_atlasHeight; //0x0004
        unsigned __int32 m_instanceCount; //0x0008
        unsigned __int32 m_terrainCount; //0x000C
        class EnlightenInstanceEntry* m_instances; //0x0010
        class EnlightenTerrainEntry* m_terrain; //0x0018
        unsigned __int32 m_systemCount; //0x0020
        class EnlightenSystemEntry* m_systems; //0x0024
        unsigned __int32 m_probeSetCount; //0x002C
        class EnlightenProbeSetEntry* m_probeSets; //0x0030
        unsigned __int32 m_cubeCount; //0x0038
        class EnlightenCubeEntry* m_cubes; //0x003C
    };//Size=0x0044
#pragma pack(pop)

    // uv = (u, v) * m_scale 2x2 + m_offset
    class EnlightenInstanceEntry
    {
    public:
        float m_scale[4]; //0x0000
        float m_offset[2]; //0x0010
        char _0x0018[8];
        unsigned __int32 m_hash; //0x0020
        char _0x0024[12];
    };//Size=0x0030

    // u = x*m0 + z*m2 + m4, v = x*m1 + z*m3 + m5
    class EnlightenTerrainEntry
    {
    public:
        float m_uv[6]; //0x0000
        char _0x0018[8];
        float m_x0; //0x0020
        float m_z0; //0x0024
        float m_x1; //0x0028
        float m_z1; //0x002C
    };//Size=0x0030

    class EnlightenSystemCore
    {
    public:
        unsigned __int64 m_guid[2]; //0x0000 (0, system index)
        unsigned __int32 m_width; //0x0010
        unsigned __int32 m_height; //0x0014
        unsigned __int32 m_basisBudget; //0x0018
        unsigned __int32 m_workspaceSize; //0x001C
        unsigned __int32 m_pixelCount; //0x0020
        unsigned __int32 m_environmentRes; //0x0024
        unsigned __int32 m_hasDirectional; //0x0028
        char _0x002C[4];
        void* m_transport; //0x0030 'GESG'
        unsigned __int32 m_transportSize; //0x0038
        unsigned __int16 m_type; //0x003C
        unsigned __int16 m_fourBit; //0x003E 1: 4-bit coefficients, 0: 8-bit
        char _0x0040[64];
    };//Size=0x0080

    class EnlightenWorkspaceRef
    {
    public:
        unsigned __int64 m_guid[2]; //0x0000
        void* m_workspace; //0x0010 'GEIW' at +0x20
        unsigned __int32 m_size; //0x0018
        unsigned __int16 m_version; //0x001C 4
        unsigned __int16 m_flags; //0x001E 1
    };//Size=0x0020

    class EnlightenSystemEntry
    {
    public:
        Vec3 m_boxMin; //0x0000
        Vec3 m_boxMax; //0x0010
        unsigned __int32 m_atlasX; //0x0020
        unsigned __int32 m_atlasY; //0x0024
        unsigned __int32 m_inputCount; //0x0028
        unsigned __int32 m_shaderCount; //0x002C
        __int32* m_inputs; //0x0030 self first
        unsigned __int64* m_shaders; //0x0038 material ids
        EnlightenSystemCore* m_core; //0x0040
        char _0x0048[8];
        EnlightenWorkspaceRef* m_workspace; //0x0050
        void* m_visibility; //0x0058 'GEVS'
        void* m_albedo; //0x0060
        char _0x0068[8];
    };//Size=0x0070

    class EnlightenProbeSetEntry
    {
    public:
        char _0x0000[16];
        LinearTransform m_transform; //0x0010
        char _0x0050[64];
        Vec3 m_boxMin; //0x0090
        Vec3 m_boxMax; //0x00A0
        char _0x00B0[4];
        __int32 m_dims[3]; //0x00B4
        char _0x00C0[4];
        unsigned __int32 m_probeCount; //0x00C4
        unsigned __int32 m_probeOffset; //0x00C8
        bool m_hasData; //0x00CC
        bool m_dynamic; //0x00CD 0 = static SH only
        char _0x00CE[2];
        unsigned __int32* m_cells; //0x00D0 grid cell -> probe
        void* m_positions; //0x00D8
        __int32* m_systemIndices; //0x00E0
        void* m_core; //0x00E8
    };//Size=0x00F0

    class EnlightenCubeEntry
    {
    public:
        char _0x0000[92];
        bool m_dynamic; //0x005C
        char _0x005D[3];
        void* m_core; //0x0060
        char _0x0068[8];
    };//Size=0x0070

    // per light and system, updateLightInput 0x140D02C50
    class EnlightenLightInput
    {
    public:
        Vec3 m_position; //0x0000
        Vec3 m_direction; //0x0010
        char _0x0020[32];
        Vec3 m_color; //0x0040
        unsigned __int32 m_type; //0x0050 0 spot, 1 point, 2 sun
        char _0x0054[60];
    };//Size=0x0090

    class EnlightenRuntimeSystem
    {
    public:
        char _0x0000[8];
        EnlightenSystemEntry* m_entry; //0x0008
        void* m_arena; //0x0010
        __int32 m_index; //0x0018
        char _0x001C[8];
        unsigned __int32 m_selectCount; //0x0024
        char _0x0028[32];
        unsigned __int16* m_chromaOut; //0x0048 w*h
        unsigned __int16* m_lumaOut; //0x0050 w*h
        unsigned __int32* m_directionOut; //0x0058 w*h, created 0xFF
        char _0x0060[8];
        void* m_inputLighting; //0x0068
        char _0x0070[8];
        class EnlightenSolveTask* m_solveTask; //0x0078
        char _0x0080[96];
        EnlightenLightInput* m_lightsBegin; //0x00E0
        EnlightenLightInput* m_lightsEnd; //0x00E8
        EnlightenLightInput* m_lightsCapacity; //0x00F0
        char _0x00F8[6824];
    };//Size=0x1BA0

    // SolveIrradianceTask 0x141199390, built in 0x140CF91B0
    class EnlightenSolveTask
    {
    public:
        EnlightenSystemCore* m_core; //0x0000
        void* m_inputLighting; //0x0008
        char _0x0010[20];
        unsigned __int32 m_outputFormat; //0x0024 3 = u16 luma + u16 chroma
        unsigned __int32 m_rgbaDirection; //0x0028 always 0, packs z,y,x, shader reads x,y,z
        float m_directionThreshold; //0x002C
        unsigned __int32 m_width; //0x0030
        unsigned __int32 m_stride; //0x0034
        char _0x0038[8];
        unsigned __int16* m_chromaOut; //0x0040
        unsigned __int16* m_lumaOut; //0x0048
        unsigned __int32* m_directionOut; //0x0050
        void* m_bounceOut; //0x0058
    };//Size=0x0060

    class EnlightenProbeSetRuntime
    {
    public:
        char _0x0000[16];
        EnlightenProbeSetEntry* m_entry; //0x0010
        float* m_sh; //0x0018 64 B per probe
        char _0x0020[8];
        void* m_arena; //0x0028
        char _0x0030[24];
        void* m_solveTask; //0x0048 dynamic sets only
        char _0x0050[16];
        unsigned __int32 m_cursor; //0x0060
        unsigned __int32 m_batch; //0x0064
        char _0x0068[64];
        bool m_hasStaticData; //0x00A8 static SH copied (0x140CF7FD0)
        bool m_ready; //0x00A9
        char _0x00AA[6];
    };//Size=0x00B0

    class EnlightenRuntimeDatabase
    {
    public:
        void* m_vtable; //0x0000 slot 1 addRef, slot 2 release
        volatile __int32 m_refCount; //0x0008
        char _0x000C[4];
        EnlightenDatabaseHeader* m_blob; //0x0010 freed via m_arena
        unsigned __int32 m_flags; //0x0018 bit 0 enable, bit 1 dynamic
        unsigned __int32 m_systemCursor; //0x001C
        unsigned __int32 m_probeCursor; //0x0020
        unsigned __int32 m_cubeCursor; //0x0024
        void* m_arena; //0x0028
        DxTexture* m_chromaTexture; //0x0030 R8G8 blue share, red share
        DxTexture* m_lumaTexture; //0x0038 R16 r+g+b
        DxTexture* m_directionTexture; //0x0040 RGBA8
        void** m_systemLighting; //0x0048
        unsigned __int32 m_systemCount; //0x0050
        char _0x0054[4];
        EnlightenRuntimeSystem* m_systems; //0x0058
        unsigned __int32 m_probeSetCount; //0x0060
        unsigned __int32 m_probeTotal; //0x0064
        EnlightenProbeSetRuntime* m_probeSets; //0x0068
        unsigned __int32 m_cubeCount; //0x0070
        char _0x0074[4];
        void* m_cubes; //0x0078 592 B each
    };//Size=0x0080

    // baked atlas + probe SH, paired by 0x140D050F0
    class EnlightenStaticEntity
    {
    public:
        char _0x0000[40];
        void* m_key; //0x0028 matched with entity +0x30
        void* m_probeData; //0x0030 +0x20 -> { +0x08 count, +0x0C SH }
        DxTexture* m_textures[3]; //0x0038 chroma, luma, direction
    };//Size=0x0050

    class EnlightenRendererEntity
    {
    public:
        char _0x0000[48];
        EnlightenRuntimeDatabase* m_database; //0x0030
        char _0x0038[24];
        EnlightenStaticEntity* m_staticEntity; //0x0050 non-null = static
        char _0x0058[8];
        DxTexture* m_textures[3]; //0x0060 chroma, luma, direction
        char _0x0078[24];
        DxTexture* m_direction; //0x0090 copy of m_textures[2]
    };//Size=0x0098

    class EnlightenRenderer
    {
    public:
        char _0x0000[24];
        unsigned __int32 m_state; //0x0018 0 dynamic, 1 static, 2 disabled (updateState 0x140D05020)
        char _0x001C[228];
        eastl::vector<EnlightenRuntimeSystem*> m_systemsToSolve; //0x0100
        eastl::vector<EnlightenProbeSetRuntime*> m_probeSetsToSolve; //0x0120
        char _0x0140[648];
        class EnlightenRuntimeSettings* m_settings; //0x03C8
        char _0x03D0[40];
        unsigned __int32 m_terrainCount; //0x03F8
        char _0x03FC[4];
        EnlightenTerrainEntry* m_terrain; //0x0400
        DxTexture** m_boundTextures; //0x0408 &entity->m_textures (selectTextures 0x140D053F0)
        EnlightenRuntimeDatabase* m_boundDatabase; //0x0410
        bool m_rebind; //0x0418
        bool m_repairStatic; //0x0419 re-pairs static entities (0x140D050F0)
        char _0x041A[6];
        eastl::vector<EnlightenRendererEntity*> m_entities; //0x0420
        eastl::vector<EnlightenStaticEntity*> m_staticEntities; //0x0440
        char _0x0460[16];
        struct HandleEntry { unsigned __int64 m_hash; const EnlightenInstanceEntry* m_instance; };
        HandleEntry* m_handlesBegin; //0x0470 addLightMapHandle
        HandleEntry* m_handlesEnd; //0x0478

        static EnlightenRenderer* GetInstance() { return *reinterpret_cast<EnlightenRenderer**>(OFF_g_enlightenRenderer); }
    };

    // probe update sub_140D037A0 (sub_140D03CC0 / new sampling sub_140BA1A90): resampled only in view and when forced or dirty
    struct LightProbeInstanceState
    {
        void* m_lookup; //0x0000
        unsigned __int16 m_flags; //0x0008 bit0 dirty, cleared when resampled
        char _0x000A[6];
    };//Size=0x0010

    class LightProbeInstanceManager
    {
    public:
        static LightProbeInstanceManager* GetInstance() { return *reinterpret_cast<LightProbeInstanceManager**>(OFF_g_lightProbeInstanceManager); }

        char _0x0000[64];
        unsigned char* m_chunks[64]; //0x0040 1024 instances of 0x50 each
        unsigned __int32 m_chunkCount; //0x0240
        char _0x0244[4];
        unsigned char* m_lastBegin; //0x0248
        unsigned char* m_lastEnd; //0x0250
        char _0x0258[16];
        LightProbeInstanceState* m_states; //0x0268

        unsigned __int32 count() const { return m_chunkCount ? ((m_chunkCount - 1) << 10) + unsigned(m_lastEnd - m_lastBegin) / 0x50 : 0; }
    };
}

#if defined(_WIN64)
static_assert(sizeof(fb::EnlightenDatabaseHeader) == 0x44, "EnlightenDatabaseHeader size");
static_assert(offsetof(fb::EnlightenDatabaseHeader, m_systems) == 0x24, "EnlightenDatabaseHeader::m_systems");
static_assert(offsetof(fb::EnlightenDatabaseHeader, m_cubes) == 0x3C, "EnlightenDatabaseHeader::m_cubes");
static_assert(sizeof(fb::EnlightenSystemEntry) == 0x70, "EnlightenSystemEntry size");
static_assert(sizeof(fb::EnlightenSystemCore) == 0x80, "EnlightenSystemCore size");
static_assert(sizeof(fb::EnlightenProbeSetEntry) == 0xF0, "EnlightenProbeSetEntry size");
static_assert(sizeof(fb::LightProbeInstanceState) == 0x10, "LightProbeInstanceState size");
static_assert(offsetof(fb::LightProbeInstanceManager, m_chunkCount) == 0x240, "LightProbeInstanceManager::m_chunkCount");
static_assert(offsetof(fb::LightProbeInstanceManager, m_states) == 0x268, "LightProbeInstanceManager::m_states");
static_assert(offsetof(fb::EnlightenProbeSetEntry, m_dynamic) == 0xCD, "EnlightenProbeSetEntry::m_dynamic");
static_assert(offsetof(fb::EnlightenProbeSetEntry, m_core) == 0xE8, "EnlightenProbeSetEntry::m_core");
static_assert(sizeof(fb::EnlightenCubeEntry) == 0x70, "EnlightenCubeEntry size");
static_assert(sizeof(fb::EnlightenLightInput) == 0x90, "EnlightenLightInput size");
static_assert(sizeof(fb::EnlightenRuntimeSystem) == 0x1BA0, "EnlightenRuntimeSystem size (7072)");
static_assert(offsetof(fb::EnlightenRuntimeSystem, m_lightsBegin) == 0xE0, "EnlightenRuntimeSystem::m_lightsBegin");
static_assert(offsetof(fb::EnlightenSolveTask, m_rgbaDirection) == 0x28, "EnlightenSolveTask::m_rgbaDirection");
static_assert(offsetof(fb::EnlightenSolveTask, m_directionOut) == 0x50, "EnlightenSolveTask::m_directionOut");
static_assert(sizeof(fb::EnlightenProbeSetRuntime) == 0xB0, "EnlightenProbeSetRuntime size (176)");
static_assert(offsetof(fb::EnlightenProbeSetRuntime, m_hasStaticData) == 0xA8, "EnlightenProbeSetRuntime::m_hasStaticData");
static_assert(sizeof(fb::EnlightenRuntimeDatabase) == 0x80, "EnlightenRuntimeDatabase size");
static_assert(offsetof(fb::EnlightenRuntimeDatabase, m_systems) == 0x58, "EnlightenRuntimeDatabase::m_systems");
static_assert(offsetof(fb::EnlightenRuntimeDatabase, m_probeSets) == 0x68, "EnlightenRuntimeDatabase::m_probeSets");
static_assert(offsetof(fb::EnlightenStaticEntity, m_textures) == 0x38, "EnlightenStaticEntity::m_textures");
static_assert(offsetof(fb::EnlightenRendererEntity, m_staticEntity) == 0x50, "EnlightenRendererEntity::m_staticEntity");
static_assert(offsetof(fb::EnlightenRendererEntity, m_textures) == 0x60, "EnlightenRendererEntity::m_textures");
static_assert(offsetof(fb::EnlightenRendererEntity, m_direction) == 0x90, "EnlightenRendererEntity::m_direction");
static_assert(offsetof(fb::EnlightenRenderer, m_settings) == 0x3C8, "EnlightenRenderer::m_settings");
static_assert(offsetof(fb::EnlightenRenderer, m_terrain) == 0x400, "EnlightenRenderer::m_terrain");
static_assert(offsetof(fb::EnlightenRenderer, m_rebind) == 0x418, "EnlightenRenderer::m_rebind");
static_assert(offsetof(fb::EnlightenRenderer, m_entities) == 0x420, "EnlightenRenderer::m_entities");
static_assert(offsetof(fb::EnlightenRenderer, m_staticEntities) == 0x440, "EnlightenRenderer::m_staticEntities");
static_assert(offsetof(fb::EnlightenRenderer, m_handlesBegin) == 0x470, "EnlightenRenderer::m_handlesBegin");
static_assert(sizeof(fb::DxTexture) == 0xC0, "DxTexture size");
static_assert(offsetof(fb::DxTexture, m_width) == 0x30, "DxTexture::m_width");
static_assert(offsetof(fb::DxTexture, m_handle) == 0x80, "DxTexture::m_handle");
static_assert(offsetof(fb::DxTexture, m_shaderFormat) == 0x8C, "DxTexture::m_shaderFormat");
static_assert(offsetof(fb::DxTexture, m_resource) == 0x98, "DxTexture::m_resource");
static_assert(offsetof(fb::DxTexture, m_shaderViews) == 0xA8, "DxTexture::m_shaderViews");
static_assert(sizeof(fb::TextureStreamingEntry) == 0x40, "TextureStreamingEntry size");
static_assert(offsetof(fb::TextureStreamingManager, m_entries) == 0x40, "TextureStreamingManager::m_entries");
static_assert(offsetof(fb::TextureStreamingManager, m_onDemandBudgetCap) == 0xAC064, "budget cap");
static_assert(offsetof(fb::TextureStreamingManager, m_onDemandBudgetUsed) == 0xAC088, "budget used");
static_assert(offsetof(fb::MeshVariationManager, m_buckets) == 0x40, "MeshVariationManager::m_buckets");
static_assert(offsetof(fb::ShaderParameterBlock, m_entries) == 0x10, "ShaderParameterBlock::m_entries");
static_assert(sizeof(fb::LensFlareEntity) == 0xC0, "LensFlareEntity size");
static_assert(offsetof(fb::LensFlareEntity, m_enabled) == 0xB0, "LensFlareEntity::m_enabled");
static_assert(offsetof(fb::LensFlareEntity, m_data) == 0x28, "LensFlareEntity::m_data");
static_assert(offsetof(fb::LensFlareEntity, m_transform) == 0x40, "LensFlareEntity::m_transform");
static_assert(offsetof(fb::LensFlareEntity, m_elementShaders) == 0x90, "LensFlareEntity::m_elementShaders");

static_assert(offsetof(fb::ClientStaticModelEntity, m_data) == 0x30, "ClientStaticModelEntity::m_data");
static_assert(offsetof(fb::ClientStaticModelEntity, m_position) == 0x240, "ClientStaticModelEntity::m_position");
#endif

#if defined(_WIN64)
static_assert(offsetof(fb::LocalLightEntity, m_data) == 0x28, "LocalLightEntity::m_data");
static_assert(offsetof(fb::LocalLightEntity, m_color) == 0x40, "LocalLightEntity::m_color");
static_assert(offsetof(fb::LocalLightEntity, m_transform) == 0x50, "LocalLightEntity::m_transform");
static_assert(offsetof(fb::LocalLightEntity, m_intensity) == 0x90, "LocalLightEntity::m_intensity");
static_assert(offsetof(fb::LocalLightEntity, m_radius) == 0xB0, "LocalLightEntity::m_radius");
static_assert(sizeof(fb::LocalLightEntity) == 0xE0, "LocalLightEntity size");
#endif
