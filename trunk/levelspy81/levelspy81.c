/*
Level Spy for Tibia 8.1
by Evremonde
Last updated on April 3rd, 2008
*/

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

// timers
#define ID_TIMER_HOTKEYS 1001

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

/* memory addresses */

// statusbar
const int STATUSBAR_TEXT	= 0x0076D928; // 0052EAA3 | MOV EDX,0076D928
const int STATUSBAR_TIMER	= 0x0076D924; // timer for showing text;  0052EABF | MOV DWORD PTR DS:[0076D924],EAX

// level spy
const int LEVELSPY_NOP		= 0x004C4FC0; // writeNops 2 bytes; 004C4FC0 | 2BC1 | SUB EAX,ECX

const int LEVELSPY_ABOVE	= 0x004C4FBC; // writeBytes 1 byte; 004C4FBB | B8 07 | MOV EAX,7
const int LEVELSPY_BELOW	= 0x004C4FC4; // writeBytes 1 byte; 004C4FC3 | B8 02 | MOV EAX,2

// name spy
const int NAMESPY_NOP		= 0x004DF469; // writeNops 2 bytes; 004DF469 | 75 4A | JNZ SHORT then 004DF46B | CALL 004C4FB0
const int NAMESPY_NOP2		= 0x004DF473; // writeNops 2 bytes; 004DF473 | 75 40 | JNZ SHORT

// show invisible creatures
const int INVISIBLE1		= 0x00453AF3; // writeBytes 1 byte; 00453AF3 | 72 0A | JB SHORT
const int INVISIBLE2		= 0x004DE734; // writeBytes 1 byte; 004DE734 | 75 09 | JNZ SHORT

// full light
const int LIGHT_NOP			= 0x004D7C99; // writeNops 2 bytes; 004D7C99 | 7E 05 | JLE SHORT
const int LIGHT_AMOUNT		= 0x004D7C9C; // writeBytes 1 byte; 004D7C9B | BB 80 | MOV EBX,80

// player
const int PLAYER_Z			= 0x0061E9C0; // 004C4FB0 | MOV ECX,DWORD PTR DS:[61E9C0] then 004C4FB6 | CMP ECX,7

/* constants */

// z-axis
const int Z_AXIS_DEFAULT = 7; // default ground level

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

// level spy
const int LEVELSPY_NOP_DEFAULT = 49451; // integer 2 bytes; 004C4FC0 | 2BC1 | SUB EAX,ECX

const int LEVELSPY_ABOVE_DEFAULT = 7; // 004C4FBB | B8 07 | MOV EAX,7
const int LEVELSPY_BELOW_DEFAULT = 2; // 004C4FC3 | B8 02 | MOV EAX,2

const int LEVELSPY_MIN = 0;
const int LEVELSPY_MAX = 7;

// name spy
const int NAMESPY_NOP_DEFAULT = 19061; // integer 2 bytes; 004DF469 | 75 4A | JNZ SHORT then 004DF46B | CALL 004C4FB0
const int NAMESPY_NOP2_DEFAULT = 16501; // integer 2 bytes; 004DF473 | 75 40 | JNZ SHORT

// show invisible creatures
const int INVISIBLE1_DEFAULT = 114; // integer 1 byte; 00453AF3 | 72 0A | JB SHORT
const int INVISIBLE2_DEFAULT = 117; // integer 1 byte; 004DE734 | 75 09 | JNZ SHORT

const int INVISIBLE_PATCH = 235; // integer 1 byte; 235 = EB = JMP; change JB and JNZ to JMP

// full light
const int LIGHT_NOP_DEFAULT = 1406; // integer 2 bytes; 004D7C99 | 7E 05 | JLE SHORT

const int LIGHT_AMOUNT_DEFAULT = 128; // 004D7C9B | BB 80 | MOV EBX,80
const int LIGHT_AMOUNT_FULL = 255; // 004D7C9B | BB 80 | MOV EBX,80

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

// restore window
void restoreWindow(HWND hwnd)
{
	// show and restore window
	ShowWindow(hwnd, SW_SHOW);
	ShowWindow(hwnd, SW_RESTORE);
	UpdateWindow(hwnd);

	// bring window to front
	SetForegroundWindow(hwnd);
}

// set statusbar text
void setStatusbarText(char* text)
{
	// show statusbar text by timer
	writeBytes(STATUSBAR_TIMER, STATUSBAR_DURATION, 1);

	// set statusbar text
	writeString(STATUSBAR_TEXT, text);
}

// check if level spying is enabled
int isLevelSpyEnabled()
{
	int levelSpyNop = readBytes(LEVELSPY_NOP, 2);
	if (levelSpyNop == LEVELSPY_NOP_DEFAULT)
		return 0;
	else
		return 1;
}

// initialize level spying
void doLevelSpyInit()
{
	// get player z
	int playerZ = readBytes(PLAYER_Z, 1);

	// set level spy to current level
	if (playerZ <= Z_AXIS_DEFAULT)
	{
		// above ground
		writeBytes(LEVELSPY_ABOVE, Z_AXIS_DEFAULT - playerZ, 1); // z-axis calculated
		writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
	}
	else
	{
		// below ground
		writeBytes(LEVELSPY_ABOVE, LEVELSPY_ABOVE_DEFAULT, 1);
		writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
	}
}

// disable level spying
void doLevelSpyOff()
{
	// disable level spying by restoring default values
	writeBytes(LEVELSPY_NOP, LEVELSPY_NOP_DEFAULT, 2);

	writeBytes(LEVELSPY_ABOVE, LEVELSPY_ABOVE_DEFAULT, 1);
	writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
}

// toggle level spying
void doLevelSpyToggle()
{
	// check if level spying is enabled
	if (isLevelSpyEnabled() == 0)
	{
		// enable level spying
		writeNops(LEVELSPY_NOP, 2);

		// initialize level spying
		doLevelSpyInit();

		setStatusbarText("Level Spy: On");
	}
	else
	{
		// disable level spying
		doLevelSpyOff();

		setStatusbarText("Level Spy: Off");
	}
}

// increase spy level
void doLevelSpyUp()
{
	// level spy must be enabled
	if (isLevelSpyEnabled() == 0)
	{
		setStatusbarText("Please enable Level Spy first!");
		return;
	}

	// get player z
	int playerZ = readBytes(PLAYER_Z, 1);

	// ground level
	int groundLevel = 0;
	if (playerZ <= Z_AXIS_DEFAULT)
		groundLevel = LEVELSPY_ABOVE; // above ground
	else
		groundLevel = LEVELSPY_BELOW; // below ground

	// get current level
	int currentLevel = readBytes(groundLevel, 1);

	if (currentLevel >= LEVELSPY_MAX)
		writeBytes(groundLevel, LEVELSPY_MIN, 1); // loop back to start
	else
		writeBytes(groundLevel, currentLevel + 1, 1); // increase spy level

	setStatusbarText("Level Spy: Up");
}

// decrease spy level
void doLevelSpyDown()
{
	// level spy must be enabled
	if (isLevelSpyEnabled() == 0)
	{
		setStatusbarText("Please enable Level Spy first!");
		return;
	}

	// get player z
	int playerZ = readBytes(PLAYER_Z, 1);

	// ground level
	int groundLevel = 0;
	if (playerZ <= Z_AXIS_DEFAULT)
		groundLevel = LEVELSPY_ABOVE; // above ground
	else
		groundLevel = LEVELSPY_BELOW; // below ground

	// get current level
	int currentLevel = readBytes(groundLevel, 1);

	if (currentLevel <= LEVELSPY_MIN)
		writeBytes(groundLevel, LEVELSPY_MAX, 1); // loop back to start
	else
		writeBytes(groundLevel, currentLevel - 1, 1); // decrease spy level

	setStatusbarText("Level Spy: Down");
}

// disable name spying
void doNameSpyOff()
{
	// disable name spying by restoring default values
	writeBytes(NAMESPY_NOP, NAMESPY_NOP_DEFAULT, 2);
	writeBytes(NAMESPY_NOP2, NAMESPY_NOP2_DEFAULT, 2);
}

// toggle name spying
void doNameSpyToggle()
{
	// check if name spying is enabled
	int nameSpyNop = readBytes(NAMESPY_NOP, 2);
	int nameSpyNop2 = readBytes(NAMESPY_NOP2, 2);
	if ((nameSpyNop == NAMESPY_NOP_DEFAULT) || (nameSpyNop2 == NAMESPY_NOP2_DEFAULT))
	{
		// enable name spying
		writeNops(NAMESPY_NOP, 2);
		writeNops(NAMESPY_NOP2, 2);

		setStatusbarText("Name Spy: On");
	}
	else
	{
		// disable name spying
		doNameSpyOff();

		setStatusbarText("Name Spy: Off");
	}
}

// disable show invisible creatures
void doShowInvisibleCreaturesOff()
{
	// disable show invisible creatures by restoring default values
	writeBytes(INVISIBLE1, INVISIBLE1_DEFAULT, 1);
	writeBytes(INVISIBLE2, INVISIBLE2_DEFAULT, 1);
}

// toggle show invisible creatures
void doShowInvisibleCreaturesToggle()
{
	// check if show invisible creatures is enabled
	int invisible1 = readBytes(INVISIBLE1, 1);
	int invisible2 = readBytes(INVISIBLE2, 1);
	if ((invisible1 == INVISIBLE1_DEFAULT) || (invisible2 == INVISIBLE2_DEFAULT))
	{
		// enable show invisible creatures
		writeBytes(INVISIBLE1, INVISIBLE_PATCH, 1);
		writeBytes(INVISIBLE2, INVISIBLE_PATCH, 1);

		setStatusbarText("Show Invisible Creatures: On");
	}
	else
	{
		// disable show invisible creatures
		doShowInvisibleCreaturesOff();

		setStatusbarText("Show Invisible Creatures: Off");
	}
}

// disable full light
void doFullLightOff()
{
	// disable full light by restoring default values
	writeBytes(LIGHT_NOP, LIGHT_NOP_DEFAULT, 2);
	writeBytes(LIGHT_AMOUNT, LIGHT_AMOUNT_DEFAULT, 1);
}

// toggle full light
void doFullLightToggle()
{
	// check if full light is enabled
	int lightNop = readBytes(LIGHT_NOP, 2);
	if (lightNop == LIGHT_NOP_DEFAULT)
	{
		// enable full light
		writeNops(LIGHT_NOP, 2);
		writeBytes(LIGHT_AMOUNT, LIGHT_AMOUNT_FULL, 1);

		setStatusbarText("Full Light: On");
	}
	else
	{
		// disable full light
		doFullLightOff();

		setStatusbarText("Full Light: Off");
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
	strcpy(trayIcon.szTip, "Level Spy - Tibia 8.1");

	// add tray icon
	Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* timer */

// hotkeys
void timerHotkeys(HWND hwnd)
{
	// restrict hotkeys to game and application by window focus
	HWND currentWindow = GetForegroundWindow();
	HWND gameWindow = getGameWindow();
	if (currentWindow != gameWindow && currentWindow != hwnd)
		return;

	// increase spy level
	if (GetAsyncKeyState(VK_PRIOR)) // page up
	{
		doLevelSpyUp();
		Sleep(100); // wait
	}

	// decrease spy level
	if (GetAsyncKeyState(VK_NEXT)) // page down
	{
		doLevelSpyDown();
		Sleep(100); // wait
	}

	// toggle level spying
	if (GetAsyncKeyState(VK_END)) // end
	{
		doLevelSpyToggle();
		Sleep(500); // wait
	}

	// toggle name spying
	if (GetAsyncKeyState(VK_HOME)) // home
	{
		doNameSpyToggle();
		Sleep(500); // wait
	}

	// toggle show invisible creatures
	if (GetAsyncKeyState(VK_DELETE)) // delete
	{
		doShowInvisibleCreaturesToggle();
		Sleep(500); // wait
	}

	// toggle full light
	if (GetAsyncKeyState(VK_INSERT)) // insert
	{
		doFullLightToggle();
		Sleep(500); // wait
	}
}

/* windows messages */

void onCreate(HWND hwnd)
{
	// create tray icon
	trayCreate(hwnd);

	// initialize timers
	SetTimer(hwnd, ID_TIMER_HOTKEYS, 10, 0); // speed is 10 milliseconds

	// loaded
	setStatusbarText("Level Spy for Tibia 8.1 loaded!");
}

void onDestroy(HWND hwnd)
{
	// unloaded
	setStatusbarText("Level Spy for Tibia 8.1 unloaded!");

	// disable level spying
	doLevelSpyOff();

	// disable name spying
	//doNameSpyOff();

	// disable show invisible creatures
	//doShowInvisibleCreaturesOff();

	// disable full light
	//doFullLightOff();

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
		"Level Spy for Tibia 8.1\nby Evremonde\nLast updated on April 3rd, 2008\n\nHotkeys:\nPage Up\t\tLevel spy up\nPage Down\t\tLevel spy down\nEnd\t\t\t\tToggle level spy on/off\nHome\t\t\tToggle name spy on/off\nDelete\t\t\tToggle show invisible creatures on/off\nInsert\t\t\tToggle full light on/off\n\nHomepage:\nhttp://code.google.com/p/evremonde/",
		-1, &rect, DT_TABSTOP | DT_EXPANDTABS, &dtp);

	// end paint
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
			// restore window
			restoreWindow(hwnd);
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
	// only one instance of application
	HWND applicationWindow = FindWindow(0, "Level Spy - Tibia 8.1");
	if (applicationWindow)
	{
		// restore window
		restoreWindow(applicationWindow);
		return 0;
	}

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
	wc.lpszClassName	= "levelspy";
	wc.hIconSm			= LoadIcon(0, IDI_WINLOGO);

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
		"levelspy", "Level Spy - Tibia 8.1", // window title
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
