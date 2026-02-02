#pragma once

#include <cstdint>
#include <Windows.h>
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <xmmintrin.h>

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

//namespace fb
//{
//    class WeaponFiring;
//    class GunSway;
//    class VisualEnvironment;
//    class ClientRoundOverEntity;
//    class VisualEnvironmentManager;
//    class ColorCorrectionComponentData;
//    class WindComponentData;
//    class InternalDatabasePartition;
//    class DataContainer;
//    class Message;
//    class CharacterLightingComponentData;
//    class EntityBusPeer;
//    struct ClientEntityFactoryParams;
//    class DataContext;
//}

#include "../SDK/sdk.h"

struct IDXGISwapChain;

typedef void(__thiscall* tfb__VisualEnvironment__operator)(fb::VisualEnvironment* _this, fb::VisualEnvironment* _that);
inline tfb__VisualEnvironment__operator ofb__VisualEnvironment__operator = 0;
void __fastcall hkfb__VisualEnvironment__operator(fb::VisualEnvironment* _this, void*, fb::VisualEnvironment* _that);

typedef int(__thiscall* tfb__VisualEnvironmentManager__update)(fb::VisualEnvironmentManager* _this, const void* a2);
inline tfb__VisualEnvironmentManager__update ofb__VisualEnvironmentManager__update = 0;
int __fastcall hkfb__VisualEnvironmentManager__update(fb::VisualEnvironmentManager* _this, void*, const void* a2);

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

typedef void(__cdecl* tfb__lerpColor)(fb::ColorCorrectionComponentData* a, fb::ColorCorrectionComponentData* b, float t, fb::ColorCorrectionComponentData* dest);
inline tfb__lerpColor ofb__lerpColor = 0;
void __cdecl hkfb__lerpColor(fb::ColorCorrectionComponentData* a, fb::ColorCorrectionComponentData* b, float t, fb::ColorCorrectionComponentData* dest);

typedef void(__cdecl* tfb__lerpWind)(fb::WindComponentData* a, fb::WindComponentData* b, float t, fb::WindComponentData* dest);
inline tfb__lerpWind ofb__lerpWind = 0;
void __cdecl hkfb__lerpWind(fb::WindComponentData* a, fb::WindComponentData* b, float t, fb::WindComponentData* dest);

typedef void(__cdecl* tfb__lerpCharacterLight)(fb::CharacterLightingComponentData* a, fb::CharacterLightingComponentData* b, float t, fb::CharacterLightingComponentData* dest);
inline tfb__lerpCharacterLight ofb__lerpCharacterLight = 0;
void __cdecl hkfb__lerpCharacterLight(fb::CharacterLightingComponentData* a, fb::CharacterLightingComponentData* b, float t, fb::CharacterLightingComponentData* dest);

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

typedef void(__thiscall* tfb__InternalDatabasePartition_addInstance)(fb::InternalDatabasePartition* _this, fb::DataContainer* obj);
inline tfb__InternalDatabasePartition_addInstance ofb__InternalDatabasePartition_addInstance = 0;
void __fastcall hkfb__InternalDatabasePartition_addInstance(fb::InternalDatabasePartition* _this, void* edx, fb::DataContainer* obj);

//vfunc
typedef void(__thiscall* tfb__InternalDatabasePartition_onPartitonLoaded)(fb::InternalDatabasePartition* _this);
inline tfb__InternalDatabasePartition_onPartitonLoaded ofb__InternalDatabasePartition_onPartitonLoaded = 0;
void __fastcall hkfb__InternalDatabasePartition_onPartitonLoaded(fb::InternalDatabasePartition* _this, void* edx);

typedef void(__thiscall* tfb__MessageManager__dispatchMessage)(int pMessageManager, fb::Message* pMessage);
inline tfb__MessageManager__dispatchMessage ofb__MessageManager__dispatchMessage = 0;
void __fastcall hkfb__MessageManager__dispatchMessage(int pMessageManager, void* edx, fb::Message* pMessage);

typedef fb::EntityBusPeer* (__cdecl* tfb__ClientEntityFactory__internalCreateEntity)(fb::ClientEntityFactoryParams* params, fb::DataContext* dc);
inline tfb__ClientEntityFactory__internalCreateEntity ofb__ClientEntityFactory__internalCreateEntity = 0;
fb::EntityBusPeer* __cdecl hkfb__ClientEntityFactory__internalCreateEntity(fb::ClientEntityFactoryParams* params, fb::DataContext* dc);

typedef int(__thiscall* tfb__LocalLightEntity__LocalLightEntity)(fb::LocalLightEntity* _this, void* info, fb::LocalLightEntityData* data, int lightType);
inline tfb__LocalLightEntity__LocalLightEntity ofb__LocalLightEntity__LocalLightEntity = 0;
int __fastcall hkfb__LocalLightEntity__LocalLightEntity(fb::LocalLightEntity* _this, void*, void* info, fb::LocalLightEntityData* data, int lightType);

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

typedef int (__thiscall* tsub_17B0180)(fb::EnlightenRenderer* _this, int a2, __m128** a3, int a4, int a5);
inline tsub_17B0180 osub_17B0180 = 0;
int __fastcall hksub_17B0180(fb::EnlightenRenderer* _this, void*, int a2, __m128** a3, int a4, int a5);

typedef __m128* (__thiscall* tsub_17A4E90)(
    fb::EnlightenRenderer* a1,
    fb::Vec3* sky,
    fb::Vec3* ground,
    fb::Vec3* sunlight,
    fb::Vec3 sunLightDir,
    float sunSize,
    fb::Vec3* backLightColor,
    float backLightRotationX,
    float backLightRotationY,
    float backLightSize,
    unsigned int skyBoxScale,
    __m128* outSkyBox);

inline tsub_17A4E90 osub_17A4E90 = 0;

__m128* __fastcall hksub_17A4E90(
    fb::EnlightenRenderer* a1,
    void*,
    fb::Vec3* sky,
    fb::Vec3* ground,
    fb::Vec3* sunlight,
    fb::Vec3 sunLightDir,
    float sunSize,
    fb::Vec3* backLightColor,
    float backLightRotationX,
    float backLightRotationY,
    float backLightSize,
    unsigned int skyBoxScale,
    __m128* outSkyBox);


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