#include "hooks.h"
#include "functions.h"

#include "../utils/log.h"

#include <MinHook.h>

bool hooks::init()
{
    if (MH_Initialize() != MH_OK)
        return false;

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

    IDXGISwapChain* swapChain = fb::DxRenderer::GetInstance()->pSwapChain;
    void* present = vfunc::getVFunc(swapChain, 8);
    void* resizeBuffers = vfunc::getVFunc(swapChain, 13);

    MH_CreateHook(present, hkD3D11Present, (LPVOID*)&oD3D11Present);
    MH_CreateHook(resizeBuffers, hkResizeBuffers, (LPVOID*)&oResizeBuffers);

    MH_STATUS hS = MH_EnableHook(MH_ALL_HOOKS);

    LOG_INFO("MH status %s\n", MH_StatusToString(hS));

    return hS == MH_OK;
}

bool hooks::shutdown()
{
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