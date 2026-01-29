#include <Windows.h>

#include "hooks/hooks.h"
#include "SDK/sdk.h"

DWORD WINAPI InitThread(LPVOID)
{
    while (!fb::DxRenderer::GetInstance() || !fb::DxRenderer::GetInstance()->pSwapChain)
    {
        Sleep(100);
    }

#ifdef _DEBUG
    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    SetConsoleTitleA("debug");
#endif

    if (!hooks::init())
        return FALSE;

    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    DisableThreadLibraryCalls(hModule);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CloseHandle(CreateThread(nullptr, 0, InitThread, nullptr, 0, nullptr));
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        if (!hooks::shutdown())
            return FALSE;
    }

    return TRUE;
}

