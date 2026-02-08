#include <Windows.h>

#include "hooks/hooks.h"
#include "SDK/sdk.h"
#include <thread>

std::jthread g_initThread;

static void initThread(std::stop_token stopToken)
{
    while (!stopToken.stop_requested())
    {
        if (fb::DxRenderer::GetInstance() && fb::DxRenderer::GetInstance()->pSwapChain)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (stopToken.stop_requested())
        return;

#ifdef _DEBUG
    AllocConsole();
    FILE* dummy;
    freopen_s(&dummy, "CONOUT$", "w", stdout);
    freopen_s(&dummy, "CONOUT$", "w", stderr);
    SetConsoleTitleA("VisEnv Debug");
#endif

    if (!hooks::init())
    {
        return;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    DisableThreadLibraryCalls(hModule);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        g_initThread = std::jthread{ initThread };
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        g_initThread.request_stop();
        if (g_initThread.joinable())
            g_initThread.join();

        hooks::shutdown();

#ifdef _DEBUG
        FreeConsole();
#endif
    }

    return TRUE;
}

