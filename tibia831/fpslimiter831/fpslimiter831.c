/*
 * FPS Limiter for Tibia 8.31
 * by Evremonde
 * Last updated on 11/16/2008
 */

#include <windows.h>
#include <shellapi.h>

#include "resource.h"

/* defines */

// window dimensions
#define WINDOW_HEIGHT	320
#define WINDOW_WIDTH	240

// tray notification message
#define WM_SHELLNOTIFY WM_USER+5

/* ids */

// tray icon
#define ID_TRAY_ICON 101

// tray menu
#define ID_TRAY_MENU_NULL			1001
#define ID_TRAY_MENU_FPS_LIMITER	1002
#define ID_TRAY_MENU_ABOUT			1003
#define ID_TRAY_MENU_HOMEPAGE		1004
#define ID_TRAY_MENU_EXIT			1005

// timers
#define ID_TIMER_FPS_LIMITER 10001

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

// tray menu
HMENU trayMenu;

/* memory addresses */

// statusbar
#define STATUSBAR_TEXT	0x00785328
#define STATUSBAR_TIMER	0x00785324 // timer for showing text

// fps
#define FPS_POINTER 0x00783FF4

/* constants */

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

// fps
const int FPS_MINIMIZED = 1; // game window is minimized
const int FPS_LOST_FOCUS = 10; // game window lost focus

/* enumerated constants */

// fps
enum OffsetFpsType
{
	OFFSET_FPS_LIMIT			= 88,
	OFFSET_FPS_CURRENT			= 96,
	OFFSET_FPS_LIMIT_READ_ONLY	= 104, // read-only
};

/* booleans */

// fps
int bFpsLimiter = 1;

/* functions */

// get game window
HWND getGameWindow()
	{ return FindWindow("tibiaclient", 0); }

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

// write double to memory
void writeDouble(DWORD address, double value)
{
	// get process handle
	HANDLE processHandle = getGameProcessHandle();

	// write to memory
	WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(value), 0);

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

// get fps pointer
int getFpsPointer()
	{ return readBytes(FPS_POINTER, 4); }

// get fps limit read only
int getFpsLimitReadOnly()
	{ return readBytes(getFpsPointer() + OFFSET_FPS_LIMIT_READ_ONLY, 1); }

// set fps limit
void setFpsLimit(int fps)
{
	// enforce a minimum of 1 frame per second
	if (fps <= 0) fps = 1;

	// convert fps
	double newFps = (double)(1000 / fps);

	writeDouble(getFpsPointer() + OFFSET_FPS_LIMIT, newFps);
}

// show about
void doShowAbout()
{
	MessageBox(0,
		"FPS Limiter for Tibia 8.31\nby Evremonde\nLast updated on 11/16/2008\n\nFPS is 1 when the Tibia window is minimized\nFPS is 10 when the Tibia window loses focus\n\nHomepage:\nhttp://code.google.com/p/evremonde/",
		"About", MB_OK | MB_ICONINFORMATION);
}

// open homepage
void doOpenHomepage()
	{ ShellExecute(0, 0, "http://code.google.com/p/evremonde/", 0, 0, SW_SHOW); }

// disable fps limiter
void doFpsLimiterOff()
{
	// set fps limit to fps limit read only
	setFpsLimit(getFpsLimitReadOnly());
}

// toggle fps limiter
void doFpsLimiterToggle()
{
	// check if fps is enabled
	if (bFpsLimiter == 0)
	{
		// enable fps limiter
		bFpsLimiter = 1;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_FPS_LIMITER, MF_CHECKED);

		setStatusbarText("FPS Limiter: On");
	}
	else
	{
		// disable fps limiter
		doFpsLimiterOff();

		// disable fps limiter
		bFpsLimiter = 0;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_FPS_LIMITER, MF_UNCHECKED);

		setStatusbarText("FPS Limiter: Off");
	}
}

/* tray icon */

// create tray icon
void doTrayIconCreate(HWND hwnd)
{
	// initialize tray icon
	trayIcon.cbSize				= sizeof(NOTIFYICONDATA);
	trayIcon.hWnd				= hwnd;
	trayIcon.uID				= ID_TRAY_ICON;
	trayIcon.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	trayIcon.uCallbackMessage	= WM_SHELLNOTIFY;
	trayIcon.hIcon				= LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_APPLICATION)); // LoadIcon(0, IDI_WINLOGO);

	// set tray icon tip text
	strcpy(trayIcon.szTip, "FPS Limiter - Tibia 8.31");

	// add tray icon
	Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* tray menu */

// create tray menu
void doTrayMenuCreate(HWND hwnd)
{
	// create tray menu
	trayMenu = CreatePopupMenu();
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NULL,			"FPS Limiter - Tibia 8.31");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_FPS_LIMITER,	"Limit FPS");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_ABOUT,			"About...");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOMEPAGE,		"Homepage...");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_EXIT,			"Exit");

	// set default tray menu item
	//SetMenuDefaultItem(trayMenu, ID_TRAY_MENU_NULL, MF_BYCOMMAND);

	// check tray menu items
	CheckMenuItem(trayMenu, ID_TRAY_MENU_FPS_LIMITER, MF_CHECKED);
}

// show tray menu
void doTrayMenuShow(HWND hwnd)
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

// fps limiter
void timerFpsLimiter(HWND hwnd)
{
	// check if fps limiter is enabled
	if (bFpsLimiter == 0)
		return;

	// only limit fps when game window loses focus
	HWND currentWindow = GetForegroundWindow();
	HWND gameWindow = getGameWindow();
	if (currentWindow != gameWindow && currentWindow != hwnd)
	{
		// check if game window is minimized
		if(GetWindowLong(gameWindow, GWL_STYLE) & WS_MINIMIZE)
			setFpsLimit(FPS_MINIMIZED); // game window is minimized
		else
			setFpsLimit(FPS_LOST_FOCUS); // game window lost focus
	}
	else
		doFpsLimiterOff(); // disable fps limiter
}

/* windows messages */

void onCreate(HWND hwnd)
{
	// create tray icon
	doTrayIconCreate(hwnd);

	// create tray menu
	doTrayMenuCreate(hwnd);

	// initialize timer
	SetTimer(hwnd, ID_TIMER_FPS_LIMITER, 10, 0); // speed is 10 milliseconds

	// loaded
	setStatusbarText("FPS Limiter for Tibia 8.31 loaded!");
}

void onDestroy(HWND hwnd)
{
	// disable fps limiter
	doFpsLimiterOff();

	// delete tray icon
	Shell_NotifyIcon(NIM_DELETE, &trayIcon);

	// destroy tray menu
	DestroyMenu(trayMenu);

	// kill timer
	KillTimer(hwnd, ID_TIMER_FPS_LIMITER);

	// unloaded
	setStatusbarText("FPS Limiter for Tibia 8.31 unloaded!");

	// exit
	PostQuitMessage(0);
}

void onTimer(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	// timers
	switch (wparam)
	{
		// fps limiter
		case ID_TIMER_FPS_LIMITER:
			timerFpsLimiter(hwnd);
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
			doTrayMenuShow(hwnd);
		}
	}
}

void onCommand(HWND hwnd, int command, int notifycode, HWND hwnditem)
{
	switch (command)
	{
		// fps limiter
		case ID_TRAY_MENU_FPS_LIMITER:
			doFpsLimiterToggle();
			break;

		// about
		case ID_TRAY_MENU_ABOUT:
			doShowAbout();
			break;

		// homepage
		case ID_TRAY_MENU_HOMEPAGE:
			doOpenHomepage();
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
	// only allow one instance of application
	HWND applicationWindow = FindWindow(0, "FPS Limiter - Tibia 8.31");
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
	wc.hIcon			= LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_APPLICATION)); // LoadIcon(0, IDI_WINLOGO);
	wc.hIconSm			= LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_APPLICATION)); // LoadIcon(0, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= 0; //(HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); // window background color
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= "fpslimiter";

	if(!RegisterClassEx(&wc))
	{
		MessageBox(0,
			"Register Window Class failed!",
			"Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	// create window
	HWND hwnd;
	hwnd = CreateWindowEx(
		WS_EX_DLGMODALFRAME, // extended window style
		"fpslimiter", "FPS Limiter - Tibia 8.31", // window class and title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window style
		CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_HEIGHT, WINDOW_WIDTH, // window dimensions
		HWND_DESKTOP, 0, hinstance, 0);

	if(hwnd == 0)
	{
		MessageBox(0,
			"Create Window failed!",
			"Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	// center window
	HWND desktopWindow = GetDesktopWindow();
	RECT desktopRect;
	GetWindowRect(desktopWindow, &desktopRect);

	RECT hwndRect;
	GetWindowRect(hwnd, &hwndRect);

	hwndRect.left = desktopRect.left + ((desktopRect.right - desktopRect.left) - (hwndRect.right - hwndRect.left)) / 2;
	hwndRect.top = desktopRect.top + ((desktopRect.bottom - desktopRect.top) - (hwndRect.bottom - hwndRect.top)) / 2;

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
