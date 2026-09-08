#pragma once

#include <cstdint>
#include <Windows.h>
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <xmmintrin.h>

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "../SDK/fb.h"
#include "../editor/camera/camera.h"

struct IDXGISwapChain;

typedef void(__thiscall* tfb__VisualEnvironment__operator)(fb::VisualEnvironment* _this, fb::VisualEnvironment* _that);
inline tfb__VisualEnvironment__operator ofb__VisualEnvironment__operator = 0;
void __fastcall hkfb__VisualEnvironment__operator(fb::VisualEnvironment* _this, void*, fb::VisualEnvironment* _that);

typedef int(__thiscall* tfb__VisualEnvironmentManager__update)(fb::VisualEnvironmentManager* _this, const void* a2);
inline tfb__VisualEnvironmentManager__update ofb__VisualEnvironmentManager__update = 0;
int __fastcall hkfb__VisualEnvironmentManager__update(fb::VisualEnvironmentManager* _this, void*, const void* a2);

typedef void(__thiscall* tfb__GameRenderer__createUpdateJob)(void* _this, float simDt, float wallDt, fb::GameRenderViewParams* params, void* outSync, void* outRoot);
inline tfb__GameRenderer__createUpdateJob ofb__GameRenderer__createUpdateJob = 0;
void __fastcall hkfb__GameRenderer__createUpdateJob(void* _this, void*, float simDt, float wallDt, fb::GameRenderViewParams* params, void* outSync, void* outRoot);

typedef HRESULT(WINAPI* tD3D11Present)(IDXGISwapChain*, UINT, UINT);
inline tD3D11Present oD3D11Present = 0;
HRESULT WINAPI hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

typedef int(__thiscall* tfb__ClientSoldierEntity__onSpawn)(int _this, int a2);
inline tfb__ClientSoldierEntity__onSpawn ofb__ClientSoldierEntity__onSpawn = 0;
int __fastcall hkfb__ClientSoldierEntity__onSpawn(int _this, void*, int a2);

typedef void(__thiscall* tfb__ClientRoundOverEntity__update)(fb::ClientRoundOverEntity* _this, int a2);
inline tfb__ClientRoundOverEntity__update ofb__ClientRoundOverEntity__update = 0;
void __fastcall hkfb__ClientRoundOverEntity__update(fb::ClientRoundOverEntity* _this, void* edx, int a2);

typedef int(__thiscall* tsub_1F21E10)(DWORD* _this, int a2);
inline tsub_1F21E10 osub_1F21E10 = 0;
int __fastcall hksub_1F21E10(DWORD* _this, void*, int a2);

typedef void(__cdecl* tfb__lerpColor)(fb::CapturedColorCorrectionComponentData* a, fb::CapturedColorCorrectionComponentData* b, float t, fb::CapturedColorCorrectionComponentData* dest);
inline tfb__lerpColor ofb__lerpColor = 0;
void __cdecl hkfb__lerpColor(fb::CapturedColorCorrectionComponentData* a, fb::CapturedColorCorrectionComponentData* b, float t, fb::CapturedColorCorrectionComponentData* dest);

typedef void(__cdecl* tfb__lerpWind)(fb::CapturedWindComponentData* a, fb::CapturedWindComponentData* b, float t, fb::CapturedWindComponentData* dest);
inline tfb__lerpWind ofb__lerpWind = 0;
void __cdecl hkfb__lerpWind(fb::CapturedWindComponentData* a, fb::CapturedWindComponentData* b, float t, fb::CapturedWindComponentData* dest);

typedef void(__cdecl* tfb__lerpCharacterLight)(fb::CapturedCharacterLightingComponentData* a, fb::CapturedCharacterLightingComponentData* b, float t, fb::CapturedCharacterLightingComponentData* dest);
inline tfb__lerpCharacterLight ofb__lerpCharacterLight = 0;
void __cdecl hkfb__lerpCharacterLight(fb::CapturedCharacterLightingComponentData* a, fb::CapturedCharacterLightingComponentData* b, float t, fb::CapturedCharacterLightingComponentData* dest);

struct GRect_float
{
    float Left;
    float Top;
    float Right;
    float Bottom;
};

struct BitmapDesc {
    GRect_float Coords;
    GRect_float TextureCoords;
    uint32_t Color;
};

typedef __m128* (__thiscall* tsub_93BC00)(float* thisPtr, __m128* a2, float* a3, float a4, int a5);
inline tsub_93BC00 osub_93BC00 = 0;
__m128* __fastcall hooked_sub_93BC00(float* thisPtr, void* edx, __m128* a2, float* a3, float a4, int a5);

typedef void(__thiscall* fn_DrawBitmaps)(
    void* thisPtr,
    BitmapDesc* pbitmapList,
    int listSize,
    int startIndex,
    int count,
    void* pti,
    void* m,
    void* pcache
    );
inline fn_DrawBitmaps original_DrawBitmaps = nullptr;
void __fastcall hooked_DrawBitmaps(
    void* thisPtr, void* edx,
    BitmapDesc* pbitmapList,
    int listSize,
    int startIndex,
    int count,
    void* pti,
    void* m,
    void* pcache);

typedef int(__thiscall* fn_sub_1763720)(void* thisPtr, int size, void* offsetOut, void* sourceData);
inline fn_sub_1763720 original_sub_1763720 = nullptr;

int __fastcall hooked_sub_1763720(void* thisPtr, void* edx, int size, void* offsetOut, void* sourceData);

#if defined(BFVE_GAME_BF3)
typedef void(__thiscall* tfb__DxTexture__reassign)(fb::DxTexture* _this, fb::DxTexture* source);
inline tfb__DxTexture__reassign ofb__DxTexture__reassign = 0;
void __fastcall hkfb__DxTexture__reassign(fb::DxTexture* _this, void* edx, fb::DxTexture* source);

typedef void(__thiscall* tfb__ClientCameraManager__getTransform)(void* _this, fb::LinearTransform* out);
inline tfb__ClientCameraManager__getTransform ofb__ClientCameraManager__getTransform = 0;
void __fastcall hkfb__ClientCameraManager__getTransform(void* _this, void* edx, fb::LinearTransform* out);

typedef void(__thiscall* tfb__InternalDatabasePartition_addInstance)(fb::InternalDatabasePartition* _this, fb::DataContainer* obj);
inline tfb__InternalDatabasePartition_addInstance ofb__InternalDatabasePartition_addInstance = 0;
void __fastcall hkfb__InternalDatabasePartition_addInstance(fb::InternalDatabasePartition* _this, void* edx, fb::DataContainer* obj);

typedef void(__thiscall* tfb__InternalDatabasePartition_onPartitonLoaded)(fb::InternalDatabasePartition* _this);
inline tfb__InternalDatabasePartition_onPartitonLoaded ofb__InternalDatabasePartition_onPartitonLoaded = 0;
void __fastcall hkfb__InternalDatabasePartition_onPartitonLoaded(fb::InternalDatabasePartition* _this, void* edx);

typedef void(__thiscall* tfb__ClientGameContext__unloadLevel)(void* _this);
inline tfb__ClientGameContext__unloadLevel ofb__ClientGameContext__unloadLevel = 0;
void __fastcall hkfb__ClientGameContext__unloadLevel(void* _this, void* edx);
#endif

typedef void(__thiscall* tfb__MessageManager__dispatchMessage)(int pMessageManager, fb::Message* pMessage);
inline tfb__MessageManager__dispatchMessage ofb__MessageManager__dispatchMessage = 0;
void __fastcall hkfb__MessageManager__dispatchMessage(int pMessageManager, void* edx, fb::Message* pMessage);

typedef int(__thiscall* tfb__LocalLightEntity__LocalLightEntity)(fb::LocalLightEntity* _this, void* info, fb::LocalLightEntityData* data, int lightType);
inline tfb__LocalLightEntity__LocalLightEntity ofb__LocalLightEntity__LocalLightEntity = 0;

int __fastcall hkfb__LocalLightEntity__LocalLightEntity(fb::LocalLightEntity* _this, void*, void* info, fb::LocalLightEntityData* data, int lightType);

typedef int(__thiscall* tfb__LensFlareEntity__buildShaders)(fb::LensFlareEntity* _this, void* data);
inline tfb__LensFlareEntity__buildShaders ofb__LensFlareEntity__buildShaders = 0;
int __fastcall hkfb__LensFlareEntity__buildShaders(fb::LensFlareEntity* _this, void*, void* data);

typedef void(__thiscall* tfb__DxTexture__releaseGpu)(fb::DxTexture* _this);
inline tfb__DxTexture__releaseGpu ofb__DxTexture__releaseGpu = 0;

#if defined(BFVE_GAME_BF3)
typedef fb::DxTexture*(__cdecl* tfb__DxTexture__create)(void* arena, fb::DxTextureCreateDesc* desc);
inline tfb__DxTexture__create ofb__DxTexture__create = 0;
fb::DxTexture* __cdecl hkfb__DxTexture__create(void* arena, fb::DxTextureCreateDesc* desc);
#endif
void __fastcall hkfb__DxTexture__releaseGpu(fb::DxTexture* _this, void*);

typedef void(__thiscall* tfb__ShaderParameterBlock__setVector)(void* block, unsigned int index, int handle, const void* value);
inline tfb__ShaderParameterBlock__setVector ofb__ShaderParameterBlock__setVector = 0;
void __fastcall hkfb__ShaderParameterBlock__setVector(void* block, void*, unsigned int index, int handle, const void* value);

typedef void(__thiscall* tLocalLightEntityDestr)(fb::LocalLightEntity* _this);
inline tLocalLightEntityDestr oLocalLightEntityDestr = 0;
void __fastcall hkLocalLightEntityDestr(fb::LocalLightEntity* _this, void*);

typedef void(__thiscall* tfb__StreamingPartitionReader__finalize)(void* _this);
inline tfb__StreamingPartitionReader__finalize ofb__StreamingPartitionReader__finalize = 0;
void __fastcall hkfb__StreamingPartitionReader__finalize(void* _this, void* edx);

typedef int(__thiscall* tfb__VisualEnvironmentEntityConstrsub_F7E030)(fb::VisualEnvironmentEntity* _this, DWORD* a2, fb::VisualEnvironmentEntityData* data);
inline tfb__VisualEnvironmentEntityConstrsub_F7E030 ofb__VisualEnvironmentEntityConstrsub_F7E030 = 0;
int __fastcall hkfb__VisualEnvironmentEntityConstrsub_F7E030(fb::VisualEnvironmentEntity* _this, void*, DWORD* a2, fb::VisualEnvironmentEntityData* data);

typedef void(__thiscall* tfb__VisualEnvironmentEntityDestrsub_F7A7E0)(fb::VisualEnvironmentEntity* _this);
inline tfb__VisualEnvironmentEntityDestrsub_F7A7E0 ofb__VisualEnvironmentEntityDestrsub_F7A7E0 = 0;
void __fastcall hkfb__VisualEnvironmentEntityDestrsub_F7A7E0(fb::VisualEnvironmentEntity* _this);

typedef int (__thiscall* tsub_1880390)(char* _this, int a2, fb::EmitterTemplateData* a3, char a4);
inline tsub_1880390 osub_1880390 = 0;
int __fastcall hksub_1880390(char* _this, void*, int a2, fb::EmitterTemplateData* a3, char a4);

typedef fb::EmitterTemplate* (__thiscall* tfb__EmitterManager__createEmitterTemplate)(void* _this, fb::EmitterTemplateData* data);
inline tfb__EmitterManager__createEmitterTemplate ofb__EmitterManager__createEmitterTemplate = 0;
fb::EmitterTemplate* __fastcall hkfb__EmitterManager__createEmitterTemplate(void* _this, void*, fb::EmitterTemplateData* data);

typedef void(__thiscall* tfb__EmitterTemplate__EmitterTemplate)(void* _this, fb::EmitterTemplateData* data);
inline tfb__EmitterTemplate__EmitterTemplate ofb__EmitterTemplate__EmitterTemplate = 0;
void __fastcall hkfb__EmitterTemplate__EmitterTemplate(void* _this, void*, fb::EmitterTemplateData* data);

typedef void* (__thiscall* tfb__ClientEmitterEntity__ctor)(void* _this, void* a2, void* a3, fb::EmitterEntityData* data);
inline tfb__ClientEmitterEntity__ctor ofb__ClientEmitterEntity__ctor = 0;
void* __fastcall hkfb__ClientEmitterEntity__ctor(void* _this, void*, void* a2, void* a3, fb::EmitterEntityData* data);

typedef HRESULT(__stdcall* tResizeBuffers)(
    IDXGISwapChain*,
    UINT,
    UINT,
    UINT,
    DXGI_FORMAT,
    UINT
    );

inline tResizeBuffers oResizeBuffers = 0;
HRESULT __stdcall hkResizeBuffers(
    IDXGISwapChain* pSwapChain,
    UINT BufferCount,
    UINT Width,
    UINT Height,
    DXGI_FORMAT NewFormat,
    UINT SwapChainFlags);

inline ID3D11Device* g_pDevice = nullptr;
inline ID3D11DeviceContext* g_pContext = nullptr;
inline ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

inline HWND g_hWnd = nullptr;
inline WNDPROC g_oWndProc = nullptr;

inline bool g_ImGuiInitialized = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#if defined(BFVE_GAME_BF4)

typedef void(*tBf4_VisualEnvironmentManager_update)(fb::VisualEnvironmentManager* _this, const void* a2);
inline tBf4_VisualEnvironmentManager_update oBf4_VisualEnvironmentManager_update = nullptr;
void hkBf4_VisualEnvironmentManager_update(fb::VisualEnvironmentManager* _this, const void* a2);

typedef void(*tBf4_VisualEnvironment_operator)(fb::VisualEnvironment* _this, fb::VisualEnvironment* _that);
inline tBf4_VisualEnvironment_operator oBf4_VisualEnvironment_operator = nullptr;
void hkBf4_VisualEnvironment_operator(fb::VisualEnvironment* _this, fb::VisualEnvironment* _that);

typedef void(*tBf4_GameRenderer_createUpdateJob)(void* _this, float simDt, float wallDt, uint32_t viewCount, fb::GameRenderViewParams* params, void* outSync, void* outRoot);
inline tBf4_GameRenderer_createUpdateJob oBf4_GameRenderer_createUpdateJob = nullptr;
void hkBf4_GameRenderer_createUpdateJob(void* _this, float simDt, float wallDt, uint32_t viewCount, fb::GameRenderViewParams* params, void* outSync, void* outRoot);

typedef __int64(*tBf4_ClientCameraManager_getTransform)(void* _this, fb::LinearTransform* out, int viewIndex);
inline tBf4_ClientCameraManager_getTransform oBf4_ClientCameraManager_getTransform = nullptr;
__int64 hkBf4_ClientCameraManager_getTransform(void* _this, fb::LinearTransform* out, int viewIndex);

typedef void(*tBf4_MessageManager_dispatch)(void* pMessageManager, fb::Message* pMessage);
inline tBf4_MessageManager_dispatch oBf4_MessageManager_dispatch = nullptr;
void hkBf4_MessageManager_dispatch(void* pMessageManager, fb::Message* pMessage);

typedef void* (*tBf4_VisualEnvironmentEntity_ctor)(fb::VisualEnvironmentEntity* _this, void* a2, void* a3);
inline tBf4_VisualEnvironmentEntity_ctor oBf4_VisualEnvironmentEntity_ctor = nullptr;
void* hkBf4_VisualEnvironmentEntity_ctor(fb::VisualEnvironmentEntity* _this, void* a2, void* a3);

typedef void(*tBf4_VisualEnvironmentEntity_dtor)(fb::VisualEnvironmentEntity* _this);
inline tBf4_VisualEnvironmentEntity_dtor oBf4_VisualEnvironmentEntity_dtor = nullptr;
void hkBf4_VisualEnvironmentEntity_dtor(fb::VisualEnvironmentEntity* _this);

typedef void* (*tBf4_LocalLightEntity_ctor)(fb::LocalLightEntity* _this, void* a2, fb::LocalLightEntityData* data, int lightType);
inline tBf4_LocalLightEntity_ctor oBf4_LocalLightEntity_ctor = nullptr;
void* hkBf4_LocalLightEntity_ctor(fb::LocalLightEntity* _this, void* a2, fb::LocalLightEntityData* data, int lightType);

typedef __int64(*tBf4_ShaderParamBlock_set)(__int64 block, unsigned int index, int handle,
                                            void* value);
inline tBf4_ShaderParamBlock_set oBf4_ShaderParamBlock_set = nullptr;
__int64 hkBf4_ShaderParamBlock_set(__int64 block, unsigned int index, int handle, void* value);

typedef void(*tBf4_DxTexture_releaseGpu)(fb::DxTexture* texture);
inline tBf4_DxTexture_releaseGpu oBf4_DxTexture_releaseGpu = nullptr;
void hkBf4_DxTexture_releaseGpu(fb::DxTexture* texture);

typedef fb::DxTexture*(*tBf4_DxTexture_create)(void* arena, void* desc);
inline tBf4_DxTexture_create oBf4_DxTexture_create = nullptr;
fb::DxTexture* hkBf4_DxTexture_create(void* arena, void* desc);

typedef void(*tBf4_DxTexture_assign)(fb::DxTexture* dst, fb::DxTexture* src);
inline tBf4_DxTexture_assign oBf4_DxTexture_assign = nullptr;
void hkBf4_DxTexture_assign(fb::DxTexture* dst, fb::DxTexture* src);

typedef __int64(*tBf4_LensFlareEntity_buildShaders)(__int64 entity, __int64 a2);
inline tBf4_LensFlareEntity_buildShaders oBf4_LensFlareEntity_buildShaders = nullptr;
__int64 hkBf4_LensFlareEntity_buildShaders(__int64 entity, __int64 a2);

typedef void(*tBf4_LocalLightEntity_dtor)(fb::LocalLightEntity* _this);
inline tBf4_LocalLightEntity_dtor oBf4_LocalLightEntity_dtor = nullptr;
void hkBf4_LocalLightEntity_dtor(fb::LocalLightEntity* _this);

typedef __int64(*tBf4_ClientGameContext_unloadLevel)(void* _this);
inline tBf4_ClientGameContext_unloadLevel oBf4_ClientGameContext_unloadLevel = nullptr;
__int64 hkBf4_ClientGameContext_unloadLevel(void* _this);

typedef void* (*tBf4_EmitterEntity_ctor)(void* _this, void* a2, fb::EmitterEntityData* data);
inline tBf4_EmitterEntity_ctor oBf4_EmitterEntity_ctor = nullptr;
void* hkBf4_EmitterEntity_ctor(void* _this, void* a2, fb::EmitterEntityData* data);

typedef uint32_t(*bf4playeff)(fb::EffectManager*, fb::Asset*, fb::LinearTransform*, void* /*level*/, char /*flags*/, fb::EffectParams*, const __m128* /*velVec*/, char /*trailingFlag*/);
inline bf4playeff ofb4playeff = nullptr;
uint32_t hkplayeff(fb::EffectManager* effectManager, fb::Asset* asset, fb::LinearTransform* tr, void* level, char flags, fb::EffectParams* params, const __m128* velVec, char trailingFlag);

#endif // BFVE_GAME_BF4