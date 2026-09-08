#include "functions.h"
#include "../editor/textures/texgen.h"
#include "../editor/textures/surfacepick.h"
#include "../editor/textures/textures.h"
#include "../editor/camera/camera.h"


#include "../SDK/fb.h"

#include <iostream>
#include <algorithm>
#include <cstring>
#include <unordered_set>
#include <cstdio>
#include <filesystem>
#include "../editor/editor.h"
#include "../editor/editor_context.h"
#include "../utils/log.h"
#include "../editor/emitters/emitters.h"
#include "../editor/lights/lights.h"

LRESULT CALLBACK hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#if defined(BFVE_GAME_BF3)

void __fastcall hkfb__VisualEnvironment__operator(fb::VisualEnvironment* _this, void*, fb::VisualEnvironment* _that)
{
    ofb__VisualEnvironment__operator(_this, _that);

    editor::onVisualEnvironmentUpdated(_this);
}

int __fastcall hkfb__VisualEnvironmentManager__update(fb::VisualEnvironmentManager* _this, void*, const void* a2)
{
    editor::onManagerUpdateBegin(_this);
    int ret = ofb__VisualEnvironmentManager__update(_this, a2);
    editor::onManagerUpdateEnd(_this);

    return ret;
}

void __fastcall hkfb__GameRenderer__createUpdateJob(void* _this, void*, float simDt, float wallDt, fb::GameRenderViewParams* params, void* outSync, void* outRoot)
{
    editor::camera::onRenderViewParams(params ? &params->view : nullptr);

    ofb__GameRenderer__createUpdateJob(_this, simDt, wallDt, params, outSync, outRoot);
}

void __fastcall hkfb__InternalDatabasePartition_onPartitonLoaded(fb::InternalDatabasePartition* _this, void* edx)
{
    editor::lights::onPartitionLoaded(_this);
    ofb__InternalDatabasePartition_onPartitonLoaded(_this);
}

void __fastcall hkfb__ClientGameContext__unloadLevel(void* _this, void*)
{
    editor::onLevelUnloadBegin();
    ofb__ClientGameContext__unloadLevel(_this);
}

void __fastcall hkfb__MessageManager__dispatchMessage(int pMessageManager, void* edx, fb::Message* pMessage)
{
    if (pMessage)
    {
        editor::onMessage(pMessage->m_Category, pMessage->m_Type);
    }

    ofb__MessageManager__dispatchMessage(pMessageManager, pMessage);
}

int __fastcall hkfb__LocalLightEntity__LocalLightEntity(fb::LocalLightEntity* _this, void*, void* info, fb::LocalLightEntityData* data, int lightType)
{
    int result = ofb__LocalLightEntity__LocalLightEntity(_this, info, data, lightType);
    editor::onLightEntityCreated(_this, data);
    return result;
}

int __fastcall hkfb__LensFlareEntity__buildShaders(fb::LensFlareEntity* _this, void*, void* data)
{
    const int result = ofb__LensFlareEntity__buildShaders(_this, data);

    if (editor::lights::hasFlareShaderClaims())
        editor::lights::applyFlareShaderClaims(_this);

    return result;
}

void __fastcall hkfb__DxTexture__releaseGpu(fb::DxTexture* _this, void*)
{
    editor::textures::gen::onEngineReleasingViews(_this);
    ofb__DxTexture__releaseGpu(_this);
}

fb::DxTexture* __cdecl hkfb__DxTexture__create(void* arena, fb::DxTextureCreateDesc* desc)
{
    fb::DxTexture* const inPlace = desc->m_rebuildInPlace ? desc->m_target : nullptr;
    const bool edited = inPlace && editor::textures::gen::hasOverride(inPlace);
    fb::DxTexture* const tex = ofb__DxTexture__create(arena, desc);
    if (tex)
        editor::textures::gen::onEngineCreatedViews(tex);
    if (edited && tex)
    {
        static uint32_t n = 0;
        if (++n <= 16 || (n % 64) == 0)
            logger::info("[texgen] create in place {}: {}x{} mips {} ({} total)", static_cast<const void*>(tex), tex->m_width, tex->m_height, tex->m_mipmapCount, n);
    }
    return tex;
}

void __fastcall hkfb__DxTexture__reassign(fb::DxTexture* _this, void*, fb::DxTexture* source)
{
    const bool edited = editor::textures::gen::hasOverride(_this);
    const uint32_t mipsBefore = _this->m_mipmapCount, widthBefore = _this->m_width;
    editor::textures::gen::onEngineReleasingViews(_this);
    ofb__DxTexture__reassign(_this, source);
    editor::textures::gen::onEngineCreatedViews(_this);
    if (edited)
    {
        static uint32_t n = 0;
        if (++n <= 16 || (n % 64) == 0)
            logger::info("[texgen] reassign {}: {}x mips {} -> {}x mips {} ({} total)", static_cast<const void*>(_this),
                         widthBefore, mipsBefore, _this->m_width, _this->m_mipmapCount, n);
    }
}

void __fastcall hkfb__ClientCameraManager__getTransform(void* _this, void*, fb::LinearTransform* out)
{
    ofb__ClientCameraManager__getTransform(_this, out);
    editor::camera::onGameCameraTransform(out, 0);
}

void __fastcall hkfb__ShaderParameterBlock__setVector(void* block, void*, unsigned int index, int handle, const void* value)
{
    alignas(16) float claimed[4];

    if (editor::lights::shaderParamOverride(uint32_t(handle), claimed) ||
        editor::textures::heldVecParam(block, uint32_t(handle), claimed))
    {
        ofb__ShaderParameterBlock__setVector(block, index, handle, claimed);
        return;
    }

    ofb__ShaderParameterBlock__setVector(block, index, handle, value);
}

void __fastcall hkLocalLightEntityDestr(fb::LocalLightEntity* _this, void*)
{
    editor::onLightEntityDestroyed(_this);
    oLocalLightEntityDestr(_this);

}

void __fastcall hkfb__StreamingPartitionReader__finalize(void* _this, void* edx)
{
    ofb__StreamingPartitionReader__finalize(_this);
}

int __fastcall hkfb__VisualEnvironmentEntityConstrsub_F7E030(fb::VisualEnvironmentEntity* _this, void*, DWORD* a2, fb::VisualEnvironmentEntityData* data)
{
    auto ret = ofb__VisualEnvironmentEntityConstrsub_F7E030(_this, a2, data);

    editor::onVisualEnvironmentEntityCreated(_this, data);

    return ret;
}

void __fastcall hkfb__VisualEnvironmentEntityDestrsub_F7A7E0(fb::VisualEnvironmentEntity* _this)
{
    editor::onVisualEnvironmentEntityDestroyed(_this);

    ofb__VisualEnvironmentEntityDestrsub_F7A7E0(_this);
}

int __fastcall hksub_1880390(char* _this, void*, int a2, fb::EmitterTemplateData* a3, char a4)
{
    auto ret = osub_1880390(_this, a2, a3, a4);

    return ret;
}

void __fastcall hkfb__EmitterTemplate__EmitterTemplate(void* _this, void*, fb::EmitterTemplateData* data)
{
    ofb__EmitterTemplate__EmitterTemplate(_this, data);
}

void* __fastcall hkfb__ClientEmitterEntity__ctor(void* _this, void*, void* a2, void* a3, fb::EmitterEntityData* data)
{
    void* ret = ofb__ClientEmitterEntity__ctor(_this, a2, a3, data);
    std::lock_guard<std::recursive_mutex> guard(editor::lock());
    editor::emitters::onEmitterEntityCreated(data, _this);
    return ret;
}

fb::EmitterTemplate* __fastcall hkfb__EmitterManager__createEmitterTemplate(void* _this, void*, fb::EmitterTemplateData* data)
{
    auto emitter = ofb__EmitterManager__createEmitterTemplate(_this, data);

    editor::onEmitterCreated(emitter, data);

    return emitter;
}

#endif

void InitImGui(IDXGISwapChain* pSwapChain)
{
    if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pDevice)))
    {
        g_pDevice->GetImmediateContext(&g_pContext);
        editor::textures::pick::init(g_pDevice, g_pContext);

        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);
        g_hWnd = sd.OutputWindow;

        ID3D11Texture2D* pBackBuffer = nullptr;
        pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
        pBackBuffer->Release();

        g_oWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDrawCursor = false;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplDX11_Init(g_pDevice, g_pContext);

        g_ImGuiInitialized = true;
    }
}

void RenderImGui()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    {
        editor::render();
    }
    ImGui::EndFrame();
    ImGui::Render();

    g_pContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

HRESULT WINAPI hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!g_ImGuiInitialized)
    {
        editor::init();
        InitImGui(pSwapChain);
    }

    if (g_ImGuiInitialized && ImGui::GetCurrentContext())
    {
        const ImVec2 d = ImGui::GetIO().DisplaySize;
        editor::textures::pick::onPresent(int(d.x * 0.5f), int(d.y * 0.5f), int(d.x), int(d.y));
    }
    if (!g_ImGuiInitialized)
        return oD3D11Present(pSwapChain, SyncInterval, Flags);
    RenderImGui();

    return oD3D11Present(pSwapChain, SyncInterval, Flags);
}

LRESULT CALLBACK hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INPUT)
    {
        RAWINPUT ri{};
        UINT size = sizeof(ri);
        if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &ri, &size,
                            sizeof(RAWINPUTHEADER)) != UINT(-1) &&
            ri.header.dwType == RIM_TYPEMOUSE &&
            (ri.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == 0)
        {
            editor::camera::onRawMouse(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
        }
    }

    if (msg == WM_SETCURSOR && editor::camera::hidesCursor())
    {
        SetCursor(nullptr);
        return TRUE;
    }

    if (msg == WM_KEYDOWN && wParam == VK_INSERT)
    {
        bool& enabled = editor::isEnabled();
        enabled = !enabled;

        if (fb::BorderInputNode* bin = fb::BorderInputNode::GetInstance())
        {
#ifdef BFVE_GAME_BF3
            if (bin->m_mouse)
                bin->m_mouse->enableCursorMode(enabled, 1);
            if (bin->m_keyboard)
                bin->m_keyboard->enableTypingMode(enabled);
#elif defined(BFVE_GAME_BF4)

            if (bin->m_keyboard)
                bin->m_keyboard->enableTypingMode(enabled);

            if (bin->m_mouse && bin->m_mouse->m_pDevice)
            {
                const bool gameOwnsCursor = bin->m_mouse->m_pDevice->m_UIOwnsInput;
                if (!gameOwnsCursor)
                    bin->m_mouse->enableCursorMode(enabled, 1);
            }
#endif
        }

        return 0;
    }

    if (editor::isEnabled())
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return TRUE;
    }

    return CallWindowProc(g_oWndProc, hWnd, msg, wParam, lParam);
}

struct ImGui_ImplDX11_Data
{
    ID3D11Device* pd3dDevice;
}; //Size=0x0100

static ImGui_ImplDX11_Data* ImGui_ImplDX11_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplDX11_Data*)ImGui::GetIO().BackendRendererUserData : nullptr;
}

HRESULT __stdcall hkResizeBuffers(
    IDXGISwapChain* pSwapChain,
    UINT BufferCount,
    UINT Width,
    UINT Height,
    DXGI_FORMAT NewFormat,
    UINT SwapChainFlags)
{
    if (g_pRenderTargetView)
    {
        g_pContext->OMSetRenderTargets(0, nullptr, nullptr);
        g_pRenderTargetView->Release();
        g_pRenderTargetView = nullptr;
    }

    ImGui_ImplDX11_Data* bd = ImGui_ImplDX11_GetBackendData();
    if (bd && bd->pd3dDevice)
    {
        ImGui_ImplDX11_InvalidateDeviceObjects();
    }

    HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

    if (FAILED(hr))
        return hr;

    ID3D11Texture2D* pBackBuffer = nullptr;
    if (SUCCEEDED(pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
    {
        if (g_pDevice)
        {
            g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
        }
        pBackBuffer->Release();
    }

    if (bd && bd->pd3dDevice)
    {
        ImGui_ImplDX11_CreateDeviceObjects();
    }

    return hr;
}

#if defined(BFVE_GAME_BF4)

void hkBf4_VisualEnvironmentManager_update(fb::VisualEnvironmentManager* _this, const void* a2)
{
    editor::onManagerUpdate_BF4(_this);

    oBf4_VisualEnvironmentManager_update(_this, a2);

    editor::onManagerUpdateEnd(_this);
}

void hkBf4_VisualEnvironment_operator(fb::VisualEnvironment* _this, fb::VisualEnvironment* _that)
{
    oBf4_VisualEnvironment_operator(_this, _that);
}

void hkBf4_GameRenderer_createUpdateJob(void* _this, float simDt, float wallDt, uint32_t viewCount, fb::GameRenderViewParams* params, void* outSync, void* outRoot)
{
    editor::camera::onRenderViewParams(params && viewCount ? &params->view : nullptr);

    oBf4_GameRenderer_createUpdateJob(_this, simDt, wallDt, viewCount, params, outSync, outRoot);
}

__int64 hkBf4_ClientCameraManager_getTransform(void* _this, fb::LinearTransform* out, int viewIndex)
{
    const __int64 result = oBf4_ClientCameraManager_getTransform(_this, out, viewIndex);

    editor::camera::onGameCameraTransform(out, viewIndex);

    return result;
}

void hkBf4_MessageManager_dispatch(void* pMessageManager, fb::Message* pMessage)
{
    if (pMessage)
        editor::onMessage(pMessage->m_Category, pMessage->m_Type);

    oBf4_MessageManager_dispatch(pMessageManager, pMessage);
}

void* hkBf4_VisualEnvironmentEntity_ctor(fb::VisualEnvironmentEntity* _this, void* a2, void* a3)
{
    void* ret = oBf4_VisualEnvironmentEntity_ctor(_this, a2, a3);
    editor::onVisualEnvironmentEntityCreated(_this, reinterpret_cast<fb::VisualEnvironmentEntityData*>(a3));
    return ret;
}

void hkBf4_VisualEnvironmentEntity_dtor(fb::VisualEnvironmentEntity* _this)
{
    editor::onVisualEnvironmentEntityDestroyed(_this);
    oBf4_VisualEnvironmentEntity_dtor(_this);
}

void* hkBf4_LocalLightEntity_ctor(fb::LocalLightEntity* _this, void* a2, fb::LocalLightEntityData* data, int lightType)
{
    void* ret = oBf4_LocalLightEntity_ctor(_this, a2, data, lightType);
    editor::onLightEntityCreated(_this, data);
    return ret;
}

void hkBf4_DxTexture_releaseGpu(fb::DxTexture* texture)
{
    editor::textures::gen::onEngineReleasingViews(texture);
    oBf4_DxTexture_releaseGpu(texture);
}

fb::DxTexture* hkBf4_DxTexture_create(void* arena, void* desc)
{
    fb::DxTexture* tex = oBf4_DxTexture_create(arena, desc);
    editor::textures::gen::onEngineCreatedViews(tex);
    return tex;
}

void hkBf4_DxTexture_assign(fb::DxTexture* dst, fb::DxTexture* src)
{
    editor::textures::gen::onEngineReleasingViews(dst);
    oBf4_DxTexture_assign(dst, src);
    editor::textures::gen::onEngineCreatedViews(dst);
}

__int64 hkBf4_LensFlareEntity_buildShaders(__int64 entity, __int64 a2)
{
    const __int64 result = oBf4_LensFlareEntity_buildShaders(entity, a2);

    if (editor::lights::hasFlareShaderClaims())
        editor::lights::applyFlareShaderClaims(reinterpret_cast<void*>(entity));

    return result;
}

__int64 hkBf4_ShaderParamBlock_set(__int64 block, unsigned int index, int handle,
                                   void* value)
{
    alignas(16) float claimed[4];

    if (editor::lights::shaderParamOverride(uint32_t(handle), claimed) ||
        editor::textures::heldVecParam(reinterpret_cast<const void*>(block), uint32_t(handle), claimed))
        return oBf4_ShaderParamBlock_set(block, index, handle, claimed);

    return oBf4_ShaderParamBlock_set(block, index, handle, value);
}

void hkBf4_LocalLightEntity_dtor(fb::LocalLightEntity* _this)
{
    editor::onLightEntityDestroyed(_this);
    oBf4_LocalLightEntity_dtor(_this);
}

__int64 hkBf4_ClientGameContext_unloadLevel(void* _this)
{
    editor::onLevelUnloadBegin();
    return oBf4_ClientGameContext_unloadLevel(_this);
}

void* hkBf4_EmitterEntity_ctor(
    void* _this, void* a2, fb::EmitterEntityData* data)
{
    void* ret = oBf4_EmitterEntity_ctor(_this, a2, data);

    std::lock_guard<std::recursive_mutex> guard(editor::lock());
    editor::emitters::onEmitterEntityCreatedBF4(data, _this);
    return ret;
}

uint_fast32_t hkplayeff(fb::EffectManager* effectManager, fb::Asset* asset, fb::LinearTransform* tr, void* level, char flags, fb::EffectParams* params, const __m128* velVec, char trailingFlag)
{
    if (level)
        fb::g_lastEffectLevel = level;

    uint_fast32_t handle = ofb4playeff(effectManager, asset, tr, level, flags, params, velVec, trailingFlag);

    return handle;
}

#endif
