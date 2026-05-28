#include "hooks.h"
#include "functions.h"

#include "../utils/log.h"
#include "../editor/editor.h"
#include "../SDK/fb.h"

#include <MinHook.h>

bool hooks::init()
{
    if (MH_Initialize() != MH_OK)
        return false;

#if defined(BFVE_GAME_BF3)
    MH_CreateHook((LPVOID)0x01791F30, hkfb__VisualEnvironment__operator, (LPVOID*)&ofb__VisualEnvironment__operator);
    MH_CreateHook((LPVOID)0x01797FD0, hkfb__VisualEnvironmentManager__update, (LPVOID*)&ofb__VisualEnvironmentManager__update);
    MH_CreateHook((LPVOID)0x004A6050, hkfb__MessageManager__dispatchMessage, (LPVOID*)&ofb__MessageManager__dispatchMessage);
    MH_CreateHook((LPVOID)0x017A6220, hkfb__LocalLightEntity__LocalLightEntity, (LPVOID*)&ofb__LocalLightEntity__LocalLightEntity);
    MH_CreateHook((LPVOID)0x017A81F0, hkLocalLightEntityDestr, (LPVOID*)&oLocalLightEntityDestr);
    MH_CreateHook((LPVOID)0x00F7E030, hkfb__VisualEnvironmentEntityConstrsub_F7E030, (LPVOID*)&ofb__VisualEnvironmentEntityConstrsub_F7E030);
    MH_CreateHook((LPVOID)0x00F7A7E0, hkfb__VisualEnvironmentEntityDestrsub_F7A7E0, (LPVOID*)&ofb__VisualEnvironmentEntityDestrsub_F7A7E0);
    MH_CreateHook((LPVOID)0x018802E0, hkfb__EmitterManager__createEmitterTemplate, (LPVOID*)&ofb__EmitterManager__createEmitterTemplate);
    //MH_CreateHook((LPVOID)0x00F82D60, hksub_F82D60, (LPVOID*)&osub_F82D60);
    //MH_CreateHook((LPVOID)0x004D5790, hkfb__InternalDatabasePartition_onPartitonLoaded, (LPVOID*)&ofb__InternalDatabasePartition_onPartitonLoaded);
    // EffectEntity
#elif defined(BFVE_GAME_BF4)
    // ---------------------------------------------------------------------
    // Battlefield 4 (Frostbite 3) hooks.
    //
    // Calling convention is plain x64 fastcall — no separate `edx` parameter.
    // BF4 hook signatures live in functions.h under BFVE_GAME_BF4 with hkBf4_*
    // names and bodies in functions.cpp.
    //
    // The 0x140626A80 dispatch entry point is a 2-instruction thunk that
    // does `mov rcx, [rcx]; jmp 0x140626810` — we hook the real callee.
    // ---------------------------------------------------------------------
    MH_CreateHook((LPVOID)0x140CC53B0, hkBf4_VisualEnvironmentManager_update, (LPVOID*)&oBf4_VisualEnvironmentManager_update);
    MH_CreateHook((LPVOID)0x140B79380, hkBf4_VisualEnvironment_operator, (LPVOID*)&oBf4_VisualEnvironment_operator);
    MH_CreateHook((LPVOID)0x140626810, hkBf4_MessageManager_dispatch, (LPVOID*)&oBf4_MessageManager_dispatch);
    MH_CreateHook((LPVOID)0x1408EA570, hkBf4_VisualEnvironmentEntity_ctor, (LPVOID*)&oBf4_VisualEnvironmentEntity_ctor);
    MH_CreateHook((LPVOID)0x1408EBAC0, hkBf4_VisualEnvironmentEntity_dtor, (LPVOID*)&oBf4_VisualEnvironmentEntity_dtor);
    MH_CreateHook((LPVOID)0x140CC7EE0, hkBf4_LocalLightEntity_ctor, (LPVOID*)&oBf4_LocalLightEntity_ctor);
    MH_CreateHook((LPVOID)0x140CC9010, hkBf4_LocalLightEntity_dtor, (LPVOID*)&oBf4_LocalLightEntity_dtor);
    MH_CreateHook((LPVOID)0x1407A7D60, hkBf4_EmitterEntity_ctor, (LPVOID*)&oBf4_EmitterEntity_ctor);

#endif

    IDXGISwapChain* swapChain = nullptr;
#if defined(BFVE_GAME_BF3)
    swapChain = fb::DxRenderer::GetInstance()->pSwapChain;
#elif defined(BFVE_GAME_BF4)
    if (auto* dx = fb::DxRenderer::GetInstance(); dx && dx->m_pScreen)
        swapChain = dx->m_pScreen->m_pSwapChain;
#endif

    if (swapChain)
    {
        void* present = vfunc::getVFunc(swapChain, 8);
        void* resizeBuffers = vfunc::getVFunc(swapChain, 13);

        MH_CreateHook(present, hkD3D11Present, (LPVOID*)&oD3D11Present);
        MH_CreateHook(resizeBuffers, hkResizeBuffers, (LPVOID*)&oResizeBuffers);
    }
    else
    {
        logger::warning("Swap chain unavailable — Present/ResizeBuffers not hooked");
    }

    MH_STATUS hS = MH_EnableHook(MH_ALL_HOOKS);
    logger::info("MH status {}", MH_StatusToString(hS));
    return hS == MH_OK;
}

bool hooks::shutdown()
{
    editor::shutdown();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    if (g_oWndProc)
        SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)g_oWndProc);

    if (g_ImGuiInitialized)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    if (g_pRenderTargetView)
        g_pRenderTargetView->Release();
    if (g_pContext)
        g_pContext->Release();
    if (g_pDevice)
        g_pDevice->Release();

    return true;
}
