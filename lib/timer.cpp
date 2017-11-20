#include <windows.h>

#include "timer.h"

void Terminate();

BOOL Init(HINSTANCE);

LRESULT CALLBACK KeyboardProc(int, WPARAM, LPARAM);

LRESULT CALLBACK MouseProc(int, WPARAM, LPARAM);


static HHOOK hhkKeyboard = NULL;
static HHOOK hhkMouse = NULL;
//static HINSTANCE hInstance = NULL;
static DWORD lastTickCount = 0;
static LONG mptX = -1;
static LONG mptY = -1;


TIMER_API LONG KS_GetLastInputTickCount() {
    return GetTickCount() - lastTickCount;
}


LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code == HC_ACTION) {
        lastTickCount = GetTickCount();
    }
    return ::CallNextHookEx(hhkKeyboard, code, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {

        auto mhkStruct = (MOUSEHOOKSTRUCT *) lParam;
        POINT pt = mhkStruct->pt;

        if (mptX != pt.x && mptY != pt.y) {
            mptX = pt.x;
            mptY = pt.y;
            lastTickCount = GetTickCount();
        }
    }
    return ::CallNextHookEx(hhkMouse, nCode, wParam, lParam);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        Init(hinstDLL);
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        Terminate();
    }
    return TRUE;
}


BOOL Init(HINSTANCE hinstDLL) {

    if (hhkKeyboard == NULL) {
        hhkKeyboard = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hinstDLL, 0);
    }

    if (hhkMouse == NULL) {
        hhkMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, hinstDLL, 0);
    }

    lastTickCount = GetTickCount();

    if (!hhkMouse || !hhkKeyboard) {
        return FALSE;
    } else {
        return TRUE;
    }

}

void Terminate() {
    if (hhkKeyboard) {
        UnhookWindowsHookEx(hhkKeyboard);
        hhkKeyboard = NULL;
    }

    if (hhkMouse) {
        UnhookWindowsHookEx(hhkMouse);
        hhkMouse = NULL;
    }
}
