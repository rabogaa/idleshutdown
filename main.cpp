#include <iostream>
#include <windows.h>

BOOL ForceShutdown();
static const LONG minutes = 1;

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
            PSTR lpCmdLine, INT nCmdShow)
{
    FARPROC addrPtrGetLastTickCount;

    HMODULE hModKeyDLL = LoadLibrary(TEXT("libtimer.dll"));

    if (hModKeyDLL) {
        addrPtrGetLastTickCount = GetProcAddress(hModKeyDLL, TEXT("KS_GetLastInputTickCount"));
    } else {
        MessageBox(nullptr, "Error %s not found", "Error!", MB_ICONERROR);
        return 0;
    }

    MSG msg;

    while (TRUE) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        LONG lastTick = addrPtrGetLastTickCount();
        printf("Last Tick count: %d\n", lastTick);
        if (lastTick >= (LONG) 1000 * 60 * minutes) {
//            ForceShutdown();
            printf("Shutting down %d", minutes);
            break;
        }

    }

    FreeLibrary(hModKeyDLL);
    FreeConsole();

    return 0;
}

BOOL ForceShutdown() {
    printf("Windows is shutting down...\n");

    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return FALSE;
    }

    LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) nullptr, 0);

    if (GetLastError() != ERROR_SUCCESS)
        return FALSE;

    if(InitiateSystemShutdown(nullptr, nullptr, 0, TRUE , FALSE))
        return FALSE;

    return TRUE;
}
