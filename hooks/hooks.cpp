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
    MH_CreateHook((LPVOID)OFF_VisualEnvironment_operatorAssign, hkfb__VisualEnvironment__operator, (LPVOID*)&ofb__VisualEnvironment__operator);
    MH_CreateHook((LPVOID)OFF_VisualEnvironmentManager_update, hkfb__VisualEnvironmentManager__update, (LPVOID*)&ofb__VisualEnvironmentManager__update);
    MH_CreateHook((LPVOID)OFF_MessageManager_dispatchMessage, hkfb__MessageManager__dispatchMessage, (LPVOID*)&ofb__MessageManager__dispatchMessage);
    MH_CreateHook((LPVOID)OFF_LocalLightEntity_ctor, hkfb__LocalLightEntity__LocalLightEntity, (LPVOID*)&ofb__LocalLightEntity__LocalLightEntity);
    MH_CreateHook((LPVOID)OFF_LocalLightEntity_dtor, hkLocalLightEntityDestr, (LPVOID*)&oLocalLightEntityDestr);
    MH_CreateHook((LPVOID)OFF_LensFlareEntity_buildShaders, hkfb__LensFlareEntity__buildShaders, (LPVOID*)&ofb__LensFlareEntity__buildShaders);
    MH_CreateHook((LPVOID)OFF_DxTexture_releaseGpu, hkfb__DxTexture__releaseGpu, (LPVOID*)&ofb__DxTexture__releaseGpu);
    MH_CreateHook((LPVOID)OFF_DxTexture_create, hkfb__DxTexture__create, (LPVOID*)&ofb__DxTexture__create);
    MH_CreateHook((LPVOID)OFF_DxTexture_reassign, hkfb__DxTexture__reassign, (LPVOID*)&ofb__DxTexture__reassign);
    MH_CreateHook((LPVOID)OFF_ClientCameraManager_getTransform, hkfb__ClientCameraManager__getTransform, (LPVOID*)&ofb__ClientCameraManager__getTransform);
    MH_CreateHook((LPVOID)OFF_ShaderParameterBlock_setVector, hkfb__ShaderParameterBlock__setVector, (LPVOID*)&ofb__ShaderParameterBlock__setVector);
    MH_CreateHook((LPVOID)OFF_VisualEnvironmentEntity_ctor, hkfb__VisualEnvironmentEntityConstrsub_F7E030, (LPVOID*)&ofb__VisualEnvironmentEntityConstrsub_F7E030);
    MH_CreateHook((LPVOID)OFF_VisualEnvironmentEntity_dtor, hkfb__VisualEnvironmentEntityDestrsub_F7A7E0, (LPVOID*)&ofb__VisualEnvironmentEntityDestrsub_F7A7E0);
    MH_CreateHook((LPVOID)OFF_EmitterManager_createEmitterTemplate, hkfb__EmitterManager__createEmitterTemplate, (LPVOID*)&ofb__EmitterManager__createEmitterTemplate);
    MH_CreateHook((LPVOID)OFF_ClientEmitterEntity_ctor, hkfb__ClientEmitterEntity__ctor, (LPVOID*)&ofb__ClientEmitterEntity__ctor);
    MH_CreateHook((LPVOID)OFF_GameRenderer_createUpdateJob, hkfb__GameRenderer__createUpdateJob, (LPVOID*)&ofb__GameRenderer__createUpdateJob);
    MH_CreateHook((LPVOID)OFF_InternalDatabasePartition_onPartitionLoaded, hkfb__InternalDatabasePartition_onPartitonLoaded, (LPVOID*)&ofb__InternalDatabasePartition_onPartitonLoaded);
    if (OFF_ClientGameContext_unloadLevel)
        MH_CreateHook((LPVOID)OFF_ClientGameContext_unloadLevel, hkfb__ClientGameContext__unloadLevel, (LPVOID*)&ofb__ClientGameContext__unloadLevel);
#elif defined(BFVE_GAME_BF4)
    MH_CreateHook((LPVOID)OFF_VisualEnvironmentManager_update, hkBf4_VisualEnvironmentManager_update, (LPVOID*)&oBf4_VisualEnvironmentManager_update);
    MH_CreateHook((LPVOID)OFF_VisualEnvironment_operatorAssign, hkBf4_VisualEnvironment_operator, (LPVOID*)&oBf4_VisualEnvironment_operator);
    MH_CreateHook((LPVOID)OFF_MessageManager_dispatchMessage, hkBf4_MessageManager_dispatch, (LPVOID*)&oBf4_MessageManager_dispatch);
    MH_CreateHook((LPVOID)OFF_VisualEnvironmentEntity_ctor, hkBf4_VisualEnvironmentEntity_ctor, (LPVOID*)&oBf4_VisualEnvironmentEntity_ctor);
    MH_CreateHook((LPVOID)OFF_VisualEnvironmentEntity_dtor, hkBf4_VisualEnvironmentEntity_dtor, (LPVOID*)&oBf4_VisualEnvironmentEntity_dtor);
    MH_CreateHook((LPVOID)OFF_LocalLightEntity_ctor, hkBf4_LocalLightEntity_ctor, (LPVOID*)&oBf4_LocalLightEntity_ctor);
    MH_CreateHook((LPVOID)OFF_LocalLightEntity_dtor, hkBf4_LocalLightEntity_dtor, (LPVOID*)&oBf4_LocalLightEntity_dtor);
    MH_CreateHook((LPVOID)OFF_ClientGameContext_unloadLevel, hkBf4_ClientGameContext_unloadLevel, (LPVOID*)&oBf4_ClientGameContext_unloadLevel);
    MH_CreateHook((LPVOID)OFF_GameRenderer_createUpdateJob, hkBf4_GameRenderer_createUpdateJob, (LPVOID*)&oBf4_GameRenderer_createUpdateJob);
    MH_CreateHook((LPVOID)OFF_ClientCameraManager_getTransform, hkBf4_ClientCameraManager_getTransform, (LPVOID*)&oBf4_ClientCameraManager_getTransform);
    MH_CreateHook((LPVOID)OFF_EmitterEntity_ctor, hkBf4_EmitterEntity_ctor, (LPVOID*)&oBf4_EmitterEntity_ctor);
    MH_CreateHook((LPVOID)OFF_EffectManager_playEffect, hkplayeff, (LPVOID*)&ofb4playeff);
    MH_CreateHook((LPVOID)OFF_ShaderParameterBlock_setVector, hkBf4_ShaderParamBlock_set, (LPVOID*)&oBf4_ShaderParamBlock_set);
    MH_CreateHook((LPVOID)OFF_LensFlareEntity_buildShaders, hkBf4_LensFlareEntity_buildShaders, (LPVOID*)&oBf4_LensFlareEntity_buildShaders);
    MH_CreateHook((LPVOID)OFF_DxTexture_releaseGpu, hkBf4_DxTexture_releaseGpu, (LPVOID*)&oBf4_DxTexture_releaseGpu);
    MH_CreateHook((LPVOID)OFF_DxTexture_create, hkBf4_DxTexture_create, (LPVOID*)&oBf4_DxTexture_create);
    MH_CreateHook((LPVOID)OFF_DxTexture_assign, hkBf4_DxTexture_assign, (LPVOID*)&oBf4_DxTexture_assign);

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
