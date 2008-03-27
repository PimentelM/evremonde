/*
Fast Hands for Tibia 8.1
by Evremonde
Last updated on March 26th, 2008
*/

#include <windows.h>
#include <shellapi.h>

/* defines */

// windows api
//#define WM_IME_KEYDOWN	0x0290
//#define WM_IME_KEYUP	0x0291

// window dimensions
#define WINDOW_WIDTH	320
#define WINDOW_HEIGHT	240

// tray notification message
#define WM_SHELLNOTIFY WM_USER+5

/* ids */

// tray icon
#define ID_TRAY_ICON 101

// timers
#define ID_TIMER_HOTKEYS	1001
#define ID_TIMER_FASTHANDS	1002

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

/* fast hands */

// point
POINT pFastHands;

/* memory addresses */

// statusbar
const int STATUSBAR_TEXT	= 0x0076D928; // 0052EAA3 | MOV EDX,0076D928
const int STATUSBAR_TIMER	= 0x0076D924; // timer for showing text;  0052EABF | MOV DWORD PTR DS:[0076D924],EAX

// mouse cursor
const int MOUSE_ID			= 0x0076C328; // 005089E0 | MOV EAX,DWORD PTR DS:[0076C328] then 005089E5 | CMP EAX,0A

/* constants */

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

// mouse cursor
const int MOUSE_NONE = 0;

/* booleans */

int bFastHands = 0;

/* functions */

// get game window
HWND getGameWindow() { return FindWindow("tibiaclient", 0); }

// get game process handle
HANDLE getGameProcessHandle()
{
	// get game window
	HWND gameWindow = getGameWindow();

	// get process id
	DWORD processId;
	GetWindowThreadProcessId(gameWindow, &processId);

	// get process handle
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	return processHandle;
}

// write bytes to memory
void writeBytes(DWORD address, int value, int bytes)
{
	// get process handle
	HANDLE processHandle = getGameProcessHandle();

	// write to memory
	WriteProcessMemory(processHandle, (LPVOID)address, &value, bytes, 0);

	// close process handle
	CloseHandle(processHandle);
}

// write string to memory
void writeString(DWORD address, char* text)
{
	// get process handle
	HANDLE processHandle = getGameProcessHandle();

	// write to memory
	int i, j = 0;
	for(i = 0; i < strlen(text); i++)
	{
		WriteProcessMemory(processHandle, (LPVOID)(address + j), &text[i], 1, NULL);
		j++; // increment address
	}
	unsigned char nullTerminator = 0x00; // end of string
	WriteProcessMemory(processHandle, (LPVOID)(address + j), &nullTerminator, 1, 0);

	// close process handle
	CloseHandle(processHandle);
}

// write NOPs to memory
void writeNops(DWORD address, int nops)
{
	// get process handle
	HANDLE processHandle = getGameProcessHandle();

	// write to memory
	int i, j = 0;
	for (i = 0; i < nops; i++)
	{
		unsigned char nop = 0x90;
		WriteProcessMemory(processHandle, (LPVOID)(address + j), &nop, 1, 0);
		j++; // increment address
	}

	// close process handle
	CloseHandle(processHandle);
}

// read bytes from memory
int readBytes(DWORD address, int bytes)
{
	// get process handle
	HANDLE processHandle = getGameProcessHandle();

	// read from memory
	int buffer = 0;
	ReadProcessMemory(processHandle, (LPVOID)address, &buffer, bytes, 0);

	// close process handle
	CloseHandle(processHandle);

	return buffer;
}

// set statusbar text
void setStatusbar(char* text)
{
	// show statusbar text by timer
	writeBytes(STATUSBAR_TIMER, STATUSBAR_DURATION, 1);

	// set statusbar text
	writeString(STATUSBAR_TEXT, text);
}

// set fast hands
void setFastHands()
{
	// fast hands location must be set
	if (pFastHands.x <= 0 || pFastHands.y <= 0)
	{
		// get fast hands point
		GetCursorPos(&pFastHands);

		setStatusbar("Fast Hands location set!");
		return;
	}
	else
	{
		// reset fast hands point
		pFastHands.x = 0;
		pFastHands.y = 0;

		setStatusbar("Fast Hands location reset!");
		return;
	}
}

// do fast hands
void doFastHands()
{
	// fast hands location must be set
	if (pFastHands.x <= 0 || pFastHands.y <= 0)
	{
		setStatusbar("Please set Fast Hands location first!");
		return;
	}

	// mouse must be idle
	int mouseId = readBytes(MOUSE_ID, 1);
	if (mouseId != MOUSE_NONE)
		return;

	// get original mouse position
	POINT pOriginal;
	GetCursorPos(&pOriginal);

	// drag to set location
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	SetCursorPos(pFastHands.x, pFastHands.y);
	Sleep(100); // wait
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	// return to original mouse position
	SetCursorPos(pOriginal.x, pOriginal.y);
	Sleep(100);

	// press enter for dragging a stack or group of items
	SendMessage(getGameWindow(), WM_IME_KEYDOWN, VK_RETURN, 0);
	SendMessage(getGameWindow(), WM_IME_KEYUP, VK_RETURN, 0);
}

// disable fast hands
void doFastHandsOff()
{
	// fast hands disabled
	bFastHands = 0;

	// set statusbar
	setStatusbar("Fast Hands: Off");
}

// toggle fast hands
void doFastHandsToggle()
{
	if (bFastHands == 0)
	{
		// fast hands location must be set
		if (pFastHands.x <= 0 || pFastHands.y <= 0)
		{
			setStatusbar("Please set Fast Hands location first!");
			return;
		}

		// fast hands enabled
		bFastHands = 1;

		// set statusbar
		setStatusbar("Fast Hands: On");
	}
	else
	{
		// disable fast hands
		doFastHandsOff();
	}
}

/* tray */

void trayCreate(HWND hwnd)
{
	// initialize tray data
	trayIcon.cbSize				= sizeof(NOTIFYICONDATA);
	trayIcon.hWnd				= hwnd;
	trayIcon.uID				= ID_TRAY_ICON;
	trayIcon.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	trayIcon.uCallbackMessage	= WM_SHELLNOTIFY;
	trayIcon.hIcon				= LoadIcon(0, IDI_WINLOGO);

	// set tray tip text
	strcpy(trayIcon.szTip, "Fast Hands - Tibia 8.1");

	// add tray icon
	Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* timer */

// hotkeys
void timerHotkeys(HWND hwnd)
{
	// restrict hotkeys to game and fast hands by window focus
	HWND currentWindow = GetForegroundWindow();
	HWND gameWindow = getGameWindow();
	if (currentWindow != gameWindow && currentWindow != hwnd)
		return;

	// set and do fast hands
	if (GetAsyncKeyState(VK_MBUTTON)) // middle-click
	{
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) // shift
		{
			// fast hands must be disabled
			if (bFastHands == 1)
				return;
			setFastHands();
			Sleep(100); // wait
		}
		else
			doFastHands();
	}

	// toggle fast hands
	if (GetAsyncKeyState(VK_PAUSE))
	{
		doFastHandsToggle();
		Sleep(500); // wait
	}

	// disable fast hands upon movement
	if (GetAsyncKeyState(VK_UP) ||
		GetAsyncKeyState(VK_DOWN) ||
		GetAsyncKeyState(VK_LEFT) ||
		GetAsyncKeyState(VK_RIGHT))
	{
		// fast hands must be enabled
		if (bFastHands == 1)
			doFastHandsOff();
	}
}

// fast hands
void timerFastHands(HWND hwnd)
{
	// restrict fast hands to game and fast hands by window focus
	HWND currentWindow = GetForegroundWindow();
	HWND gameWindow = getGameWindow();
	if (currentWindow != gameWindow && currentWindow != hwnd)
		return;

	// fast hands must be enabled
	if (bFastHands == 0)
		return;

	doFastHands();
}

/* windows messages */

void onCreate(HWND hwnd)
{
	// create tray icon
	trayCreate(hwnd);

	// initialize timers
	SetTimer(hwnd, ID_TIMER_HOTKEYS,	10, 0); // speed is 10 milliseconds
	SetTimer(hwnd, ID_TIMER_FASTHANDS,	10, 0);

	// loaded
	setStatusbar("Fast Hands for Tibia 8.1 loaded!");
}

void onDestroy(HWND hwnd)
{
	// remove tray icon
	Shell_NotifyIcon(NIM_DELETE, &trayIcon);

	// exit
	PostQuitMessage(0);
}

void onSize(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	// minimized
	if (wparam == SIZE_MINIMIZED)
	{
		// hide window
		ShowWindow(hwnd, SW_HIDE);
		UpdateWindow(hwnd);
	}
}

void onPaint(HWND hwnd)
{
	// set drawing area
	RECT rect;
	rect.top	= 10; // small margin
	rect.left	= 10; // small margin
	rect.bottom	= WINDOW_HEIGHT;
	rect.right	= WINDOW_WIDTH;

	// begin paint
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);

	// set text to windows font
	HFONT hfont = GetStockObject(ANSI_VAR_FONT);
	SelectObject(hdc, hfont);

	// draw text on transparent background
	SetBkMode(hdc, TRANSPARENT);

	// draw the text
	DRAWTEXTPARAMS dtp;
	dtp.cbSize			= sizeof(DRAWTEXTPARAMS);
	dtp.iLeftMargin		= 0;
	dtp.iRightMargin	= 0;
	dtp.iTabLength		= 4;
	DrawTextEx(
		hdc,
		"Fast Hands for Tibia 8.1\nby Evremonde\nLast updated on March 26th, 2008\n\nHotkeys:\nMiddle-click\t\t\tDrag items to the set location\nShift+Middle-click\tSet the location where to drag items\n\nPause\t\t\t\tToggle automatic dragging of items\nUp,Down,Left,Right\tDisable automatic dragging of items",
		-1, &rect, DT_TABSTOP | DT_EXPANDTABS, &dtp);

	//end paint
	EndPaint(hwnd, &ps);
}

void onTimer(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	// timers
	switch (wparam)
	{
		// hotkeys
		case ID_TIMER_HOTKEYS:
			timerHotkeys(hwnd);
			break;

		// fast hands
		case ID_TIMER_FASTHANDS:
			timerFastHands(hwnd);
			break;
	}
}

void onTray(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	// tray icon
	if (wparam == ID_TRAY_ICON)
	{
		// left click or right click
		if (lparam == WM_LBUTTONDOWN || lparam == WM_RBUTTONDOWN)
		{
			// show and restore window
			ShowWindow(hwnd, SW_SHOW);
			ShowWindow(hwnd, SW_RESTORE);
			UpdateWindow(hwnd);
		}
	}
}

/* window messages handler */

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
			onCreate(hwnd);
			break;

		case WM_CLOSE:
			DestroyWindow(hwnd); // exit
			break;

		case WM_DESTROY:
			onDestroy(hwnd);
			break;

		case WM_SIZE:
			onSize(hwnd, wparam, lparam);
			break;

		case WM_PAINT:
			onPaint(hwnd);

		case WM_TIMER:
			onTimer(hwnd, wparam, lparam);
			break;

		case WM_SHELLNOTIFY:
			onTray(hwnd, wparam, lparam);
			break;

		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

/* main function */

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR cmdline, int cmdshow)
{
	// register window class
	WNDCLASSEX wc;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= windowProc; // window messages handler
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hinstance;
	wc.hIcon			= LoadIcon(0, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); // window background color
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= "fasthands";
	wc.hIconSm			= 0;

	if(!RegisterClassEx(&wc))
	{
		MessageBox(0,
			"Register Window Class failed!",
			"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	// create window
	HWND hwnd;
	hwnd = CreateWindowEx(
		WS_EX_DLGMODALFRAME, // window style
		"fasthands", "Fast Hands - Tibia 8.1", // window title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window options
		CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_WIDTH, WINDOW_HEIGHT, // window dimensions
		0, 0, hinstance, 0);

	if(hwnd == 0)
	{
		MessageBox(0,
			"Create Window failed!",
			"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	// center window
	HWND desktopWindow = GetDesktopWindow();
	RECT desktopRect;
	GetWindowRect(desktopWindow, &desktopRect);

	RECT hwndRect;
	GetWindowRect(hwnd, &hwndRect);

	hwndRect.left	= desktopRect.left + ((desktopRect.right - desktopRect.left) - (hwndRect.right - hwndRect.left)) / 2;
	hwndRect.top	= desktopRect.top + ((desktopRect.bottom - desktopRect.top) - (hwndRect.bottom - hwndRect.top)) / 2;

	SetWindowPos(hwnd, 0, hwndRect.left, hwndRect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

	// show window
	ShowWindow(hwnd, cmdshow);
	UpdateWindow(hwnd);

	// message loop
	MSG msg;
	while(GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
