
#include <Windows.h>
#include <stdio.h>

int running = false;
DWORD tid = 0;
HANDLE thread;

void mouseClickEvent() {//Click event using mouse event
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

DWORD WINAPI myThreadFunction(LPVOID lpParam) {///Thread with click loop
    int* r = (int*)lpParam;//Thread used to change focous to any software that is running
    while (*r) {
        mouseClickEvent();
        Sleep(500);
    }
    return 0;
}

LRESULT CALLBACK lowLevelKbdProc(int nCode, WPARAM wParam, LPARAM lParam) {
    bool bEatKeyStroke = false;
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

	if (nCode == HC_ACTION) {
		switch (wParam) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (bEatKeyStroke = (p->vkCode == VK_F6)) {

				if (running == false) {
					printf("AutoClicker started. Press F6 again to stop it.\n");
					running = true;
					thread = CreateThread(NULL, 0, myThreadFunction, &running, 0, &tid);
				}
				else {
					running = false;
					CloseHandle(thread);
					printf("AutoClicker stopped!\n");
				}

			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			break;
		}
    }
	return (bEatKeyStroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

int main()
{
	HHOOK hookLowLevelKbd = SetWindowsHookEx(WH_KEYBOARD_LL, lowLevelKbdProc, 0, 0);//Not necessary dll to create hook
	MSG msg;

	while (!GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hookLowLevelKbd);
	return 0;
}
