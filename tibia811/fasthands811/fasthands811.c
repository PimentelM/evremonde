/*
Fast Hands for Tibia 8.11
by Evremonde
Last updated on June 30th, 2008
*/

#include <stdio.h>

#include <windows.h>
#include <shellapi.h>

/* defines */

// window dimensions
#define WINDOW_WIDTH	320
#define WINDOW_HEIGHT	240

// tray notification message
#define WM_SHELLNOTIFY WM_USER+5

/* ids */

// tray icon
#define ID_TRAY_ICON 101

// tray menu
#define ID_TRAY_MENU_NULL			1001
#define ID_TRAY_MENU_LOCATION		1002
#define ID_TRAY_MENU_RESET_LOCATION	1003
#define ID_TRAY_MENU_HOTKEYS		1004
#define ID_TRAY_MENU_ABOUT			1005
#define ID_TRAY_MENU_HOMEPAGE		1006
#define ID_TRAY_MENU_EXIT			1007

// timers
#define ID_TIMER_HOTKEYS	10001
#define ID_TIMER_FASTHANDS	10002

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

// tray menu
HMENU trayMenu;

/* fast hands */

// point
POINT fastHandsLocation;

/* memory addresses */

// statusbar
#define STATUSBAR_TEXT		0x0076D928 // 0052EAA3 | MOV EDX,0076D928
#define STATUSBAR_TIMER		0x0076D924 // timer for showing text;  0052EABF | MOV DWORD PTR DS:[0076D924],EAX

/* constants */

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

/* booleans */

// hotkeys
int bHotkeys = 1;

// fast hands
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
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

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
	WriteProcessMemory(processHandle, (LPVOID)address, text, strlen(text) + 1, 0);

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

// read string from memory
char* readString(DWORD address)
{
	// get process handle
	HANDLE processHandle = getGameProcessHandle();

	// read from memory
	static char buffer[256];
	ReadProcessMemory(processHandle, (LPVOID)address, &buffer, sizeof(buffer), 0);

	// close process handle
	CloseHandle(processHandle);

	return buffer;
}

// set statusbar text
void setStatusbarText(char* text)
{
	// show statusbar text by timer
	writeBytes(STATUSBAR_TIMER, STATUSBAR_DURATION, 1);

	// set statusbar text
	writeString(STATUSBAR_TEXT, text);
}

// show about
void showAbout()
{
	MessageBox(0,
		"Fast Hands for Tibia 8.11\nby Evremonde\nLast updated on June 30th, 2008\n\nHotkeys:\nMiddle-click\t\tDrag items to the set location\nShift+Middle-click\t\tSet the location where to drag items\n\nEscape\t\t\tToggle automatic dragging of items\nUp,Down,Left,Right\tDisable automatic dragging of items\n\nHomepage:\nhttp://code.google.com/p/evremonde/",
		"About", MB_OK | MB_ICONINFORMATION);
}

// open homepage
void openHomepage()
{
	ShellExecute(0, 0, "http://code.google.com/p/evremonde/", 0, 0, SW_SHOW);
}

// reset fast hands
void resetFastHands()
{
	// reset fast hands location
	fastHandsLocation.x = 0;
	fastHandsLocation.y = 0;

	// set fast hands location menu item text
	ModifyMenu(trayMenu, ID_TRAY_MENU_LOCATION, MF_BYCOMMAND | MF_STRING, ID_TRAY_MENU_LOCATION, "Location: 0, 0");

	setStatusbarText("Fast Hands location reset!");
}

// set fast hands
void setFastHands()
{
	// fast hands location must be set
	if (fastHandsLocation.x <= 0 || fastHandsLocation.y <= 0)
	{
		// get fast hands mouse position
		GetCursorPos(&fastHandsLocation);

		// set fast hands location menu item text
		char buffer[256];
		sprintf(buffer, "Location: %d, %d", (int)fastHandsLocation.x, (int)fastHandsLocation.y);
		ModifyMenu(trayMenu, ID_TRAY_MENU_LOCATION, MF_BYCOMMAND | MF_STRING, ID_TRAY_MENU_LOCATION, buffer);

		setStatusbarText("Fast Hands location set!");
	}
	else
	{
		// reset fast hands location
		resetFastHands();
	}
}

// do fast hands
void doFastHands()
{
	// game window not found
	if (getGameWindow() == 0)
		return;

	// fast hands location must be set
	if (fastHandsLocation.x <= 0 || fastHandsLocation.y <= 0)
	{
		setStatusbarText("Please set Fast Hands location first!");
		return;
	}

	// get original mouse location
	POINT originalLocation;
	GetCursorPos(&originalLocation);

	// left mouse button down
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);

	// go to fast hands location
	SetCursorPos(fastHandsLocation.x, fastHandsLocation.y);
	Sleep(10); // wait

	// left mouse button up
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	// return to original mouse location
	SetCursorPos(originalLocation.x, originalLocation.y);
	Sleep(10); // wait

	// press enter for dragging a stack or group of items
	SendMessage(getGameWindow(), WM_IME_KEYDOWN, VK_RETURN, 0);
	SendMessage(getGameWindow(), WM_IME_KEYUP, VK_RETURN, 0);
}

// disable fast hands
void doFastHandsOff()
{
	// fast hands disabled
	bFastHands = 0;

	setStatusbarText("Fast Hands: Off");
}

// toggle fast hands
void doFastHandsToggle()
{
	// fast hands must be enabled
	if (bFastHands == 0)
	{
		// fast hands location must be set
		if (fastHandsLocation.x <= 0 || fastHandsLocation.y <= 0)
		{
			setStatusbarText("Please set Fast Hands location first!");
			return;
		}

		// fast hands enabled
		bFastHands = 1;

		setStatusbarText("Fast Hands: On");
	}
	else
	{
		// disable fast hands
		doFastHandsOff();
	}
}

// toggle hotkeys
void doHotkeysToggle()
{
	// check if hotkeys are enabled
	if (bHotkeys == 0)
	{
		// enable hotkeys
		bHotkeys = 1;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_CHECKED);

		setStatusbarText("Hotkeys: On");
	}
	else
	{
		// disable hotkeys
		bHotkeys = 0;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_UNCHECKED);

		setStatusbarText("Hotkeys: Off");
	}
}

/* tray */

void trayIconCreate(HWND hwnd)
{
	// initialize tray data
	trayIcon.cbSize				= sizeof(NOTIFYICONDATA);
	trayIcon.hWnd				= hwnd;
	trayIcon.uID				= ID_TRAY_ICON;
	trayIcon.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	trayIcon.uCallbackMessage	= WM_SHELLNOTIFY;
	trayIcon.hIcon				= LoadIcon(0, IDI_WINLOGO);

	// set tray icon tip text
	strcpy(trayIcon.szTip, "Fast Hands - Tibia 8.11");

	// add tray icon
	Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* tray menu */

// create tray menu
void trayMenuCreate(HWND hwnd)
{
	// create tray menu
	trayMenu = CreatePopupMenu();
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NULL,				"Fast Hands - Tibia 8.11");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LOCATION,			"&Location: 0, 0");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_RESET_LOCATION,	"&Reset Location");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOTKEYS,			"&Hotkeys");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_ABOUT,				"&About...");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOMEPAGE,			"Home&page...");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_EXIT,				"E&xit");

	// set default tray menu item
	//SetMenuDefaultItem(trayMenu, ID_TRAY_MENU_NULL, MF_BYCOMMAND);

	// check tray menu items
	CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_CHECKED);
}

// show tray menu
void trayMenuShow(HWND hwnd)
{
	// get menu location
	POINT trayMenuLocation;
	GetCursorPos(&trayMenuLocation);

	// required to make menu show and hide correctly
	SetForegroundWindow(hwnd);

	// show tray menu
	TrackPopupMenu(trayMenu,
		TPM_RIGHTALIGN | TPM_RIGHTBUTTON, // align right and allow right-click on menu items
		trayMenuLocation.x, trayMenuLocation.y,
		0, hwnd, 0);

	// required to make menu show and hide correctly
	PostMessage(hwnd, WM_NULL, 0, 0);
}

/* timer */

// hotkeys
void timerHotkeys(HWND hwnd)
{
	// check if hotkeys are enabled
	if (bHotkeys == 0)
		return;

	// restrict hotkeys to game and application by window focus
	HWND currentWindow = GetForegroundWindow();
	HWND gameWindow = getGameWindow();
	if (currentWindow != gameWindow && currentWindow != hwnd)
		return;

	// set or do fast hands
	if (GetAsyncKeyState(VK_MBUTTON)) // middle-click
	{
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) // shift
		{
			// fast hands must be disabled
			if (bFastHands == 1)
				return;
			setFastHands();
			Sleep(500); // wait
		}
		else
			doFastHands();
	}

	// toggle fast hands
	if (GetAsyncKeyState(VK_ESCAPE)) // escape
	{
		doFastHandsToggle();
		Sleep(500); // wait
	}

	// disable fast hands upon movement
	if (GetAsyncKeyState(VK_UP) || // up, down, left, right
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
	// restrict fast hands to game and application by window focus
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
	trayIconCreate(hwnd);

	// create tray menu
	trayMenuCreate(hwnd);

	// initialize timers
	SetTimer(hwnd, ID_TIMER_HOTKEYS,	10, 0); // speed is 10 milliseconds
	SetTimer(hwnd, ID_TIMER_FASTHANDS,	10, 0);

	// loaded
	setStatusbarText("Fast Hands for Tibia 8.11 loaded!");
}

void onDestroy(HWND hwnd)
{
	// unloaded
	setStatusbarText("Fast Hands for Tibia 8.11 unloaded!");

	// remove tray icon
	Shell_NotifyIcon(NIM_DELETE, &trayIcon);

	// destroy tray menu
	DestroyMenu(trayMenu);

	// kill timers
	KillTimer(hwnd, ID_TIMER_HOTKEYS);
	KillTimer(hwnd, ID_TIMER_FASTHANDS);

	// exit
	PostQuitMessage(0);
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
			// show tray menu
			trayMenuShow(hwnd);
		}
	}
}

void onCommand(HWND hwnd, int command, int notifycode, HWND hwnditem)
{
	switch (command)
	{
		// reset fast hands location
		case ID_TRAY_MENU_RESET_LOCATION:
			resetFastHands();
			break;

		// hotkeys
		case ID_TRAY_MENU_HOTKEYS:
			doHotkeysToggle();
			break;

		// about
		case ID_TRAY_MENU_ABOUT:
			showAbout();
			break;

		// homepage
		case ID_TRAY_MENU_HOMEPAGE:
			openHomepage();
			break;

		// exit
		case ID_TRAY_MENU_EXIT:
			DestroyWindow(hwnd); // exit
			break;
	}
}

/* window messages handler */

LRESULT CALLBACK windowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
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

		case WM_TIMER:
			onTimer(hwnd, wparam, lparam);
			break;

		case WM_SHELLNOTIFY:
			onTray(hwnd, wparam, lparam);
			break;

		case WM_COMMAND:
			onCommand(hwnd, LOWORD(wparam), HIWORD(wparam), (HWND)lparam);
			SetForegroundWindow(getGameWindow()); // set focus to game window after using tray menu
			break;

		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	return 0;
}

/* main function */

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR cmdline, int cmdshow)
{
	// only one instance of application
	HWND applicationWindow = FindWindow(0, "Fast Hands - Tibia 8.11");
	if (applicationWindow)
		return 0;

	// register window class
	WNDCLASSEX wc;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= windowProc; // window messages handler
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hinstance;
	wc.hIcon			= LoadIcon(0, IDI_WINLOGO);
	wc.hIconSm			= LoadIcon(0, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= 0; //(HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); // window background color
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= "fasthands";

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
		WS_EX_DLGMODALFRAME, // extended window style
		"fasthands", "Fast Hands - Tibia 8.11", // window class and title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window style
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
	//ShowWindow(hwnd, cmdshow);
	//UpdateWindow(hwnd);

	// message loop
	MSG msg;
	while(GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
