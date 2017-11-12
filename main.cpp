#include <iostream>
#include <windows.h>

BOOL ShowWarningDialog();
void ForceShutdown();

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
            PSTR lpCmdLine, INT nCmdShow)
{
    FARPROC addrPtrGetLastTickCount;

    HMODULE hModKeyDLL = LoadLibrary(TEXT("libkey.dll"));

    if (hModKeyDLL) {
        addrPtrGetLastTickCount = GetProcAddress(hModKeyDLL, TEXT("KS_GetLastInputTickCount"));
    }

    LPMSG msg;

    while (TRUE) {
        while (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(msg);
            DispatchMessage(msg);
        }

        if (msg->message == WM_QUIT) {
            printf("Quiting....\n");
            break;
        }

        DWORD lastTick = addrPtrGetLastTickCount();
        printf("Last Tick count: %d:\n", lastTick);
        if (lastTick >= (DWORD) 30000.1) {
            BOOL result = ShowWarningDialog();
            break;
        }

    }

    FreeLibrary(hModKeyDLL);
    FreeConsole();

    return 0;
}

void ForceShutdown() {
    InitiateSystemShutdown(NULL, NULL, 0, TRUE , FALSE);
}

BOOL ShowWarningDialog() {
    printf("System shutting down...\n");
    return TRUE;
}