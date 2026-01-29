#include "functions.h"

#include "../SDK/sdk.h"

#include <iostream>
#include "../editor/editor.h"

LRESULT CALLBACK hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void __fastcall hkfb__VisualEnvironment__operator(fb::VisualEnvironment* _this, void*, fb::VisualEnvironment* _that)
{
    ofb__VisualEnvironment__operator(_this, _that);

    VisualEnvironmentEditor::Instance().OnVisualEnvironmentUpdated(_this);
}

int __fastcall hkfb__VisualEnvironmentManager__update(fb::VisualEnvironmentManager* _this, void*, const void* a2)
{
    VisualEnvironmentEditor::Instance().OnManagerUpdateBegin(_this);
    int ret = ofb__VisualEnvironmentManager__update(_this, a2);
    VisualEnvironmentEditor::Instance().OnManagerUpdateEnd(_this);

    return ret;
}

void __fastcall hkfb__InternalDatabasePartition_onPartitonLoaded(fb::InternalDatabasePartition* _this, void* edx)
{
    //printf("Ret addr %p\n", _ReturnAddress());

    ofb__InternalDatabasePartition_onPartitonLoaded(_this);
}

void __fastcall hkfb__MessageManager__dispatchMessage(int pMessageManager, void* edx, fb::Message* pMessage)
{
    if (pMessage)
    {
        //printf("Message %s C%u T%u R%p\n", pMessage->GetType()->m_InfoData->m_Name, pMessage->m_Category, pMessage->m_Type, _ReturnAddress());

        VisualEnvironmentEditor::Instance().OnMessage(pMessage->m_Category, pMessage->m_Type);
    }

    ofb__MessageManager__dispatchMessage(pMessageManager, pMessage);
}

fb::EntityBusPeer* __cdecl hkfb__ClientEntityFactory__internalCreateEntity(fb::ClientEntityFactoryParams* params, fb::DataContext* dc)
{
    return ofb__ClientEntityFactory__internalCreateEntity(params, dc);
}

int __fastcall hkfb__LocalLightEntity__LocalLightEntity(fb::LocalLightEntity* _this, void*, void* info, fb::LocalLightEntityData* data, int lightType)
{
    int result = ofb__LocalLightEntity__LocalLightEntity(_this, info, data, lightType);
    VisualEnvironmentEditor::Instance().OnLightEntityCreated(_this, data);
    return result;
}

void __fastcall hkLocalLightEntityDestr(fb::LocalLightEntity* _this, void*)
{
    VisualEnvironmentEditor::Instance().OnLightEntityDestroyed(_this);

    oLocalLightEntityDestr(_this);

}

void __fastcall hkfb__StreamingPartitionReader__finalize(void* _this, void* edx)
{
    ofb__StreamingPartitionReader__finalize(_this);
}

int __fastcall hkfb__VisualEnvironmentEntityConstrsub_F7E030(fb::VisualEnvironmentEntity* _this, void*, DWORD* a2, fb::VisualEnvironmentEntityData* data)
{
    auto ret = ofb__VisualEnvironmentEntityConstrsub_F7E030(_this, a2, data);

    VisualEnvironmentEditor::Instance().OnVisualEnvironmentEntityCreated(_this, data);

    return ret;
}

void __fastcall hkfb__VisualEnvironmentEntityDestrsub_F7A7E0(fb::VisualEnvironmentEntity* _this)
{
    VisualEnvironmentEditor::Instance().OnVisualEnvironmentEntityDestroyed(_this);

    ofb__VisualEnvironmentEntityDestrsub_F7A7E0(_this);
}

void InitImGui(IDXGISwapChain* pSwapChain)
{
    if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pDevice)))
    {
        g_pDevice->GetImmediateContext(&g_pContext);

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
        VisualEnvironmentEditor::Instance().Render();
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
        InitImGui(pSwapChain);
    }

    RenderImGui();

    return oD3D11Present(pSwapChain, SyncInterval, Flags);
}

LRESULT CALLBACK hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_KEYDOWN && wParam == VK_INSERT)
    {
        bool& enabled = VisualEnvironmentEditor::Instance().IsEnabled();
        enabled = !enabled;

        fb::BorderInputNode::GetInstance()->m_mouse->enableCursorMode(enabled, 1);

        return 0;
    }

    if (VisualEnvironmentEditor::Instance().IsEnabled())
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return TRUE;
    }

    return CallWindowProc(g_oWndProc, hWnd, msg, wParam, lParam);
}

struct ImGui_ImplDX11_Data
{
    ID3D11Device* pd3dDevice;
};

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
