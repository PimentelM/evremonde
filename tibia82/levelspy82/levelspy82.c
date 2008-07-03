/*
Level Spy for Tibia 8.2
by Evremonde
Last updated on July 2nd, 2008
*/

#include <stdio.h>

#include <windows.h>
#include <shellapi.h>

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
#define ID_TRAY_MENU_LEVELSPY		1002
#define ID_TRAY_MENU_LEVELSPY_UP	1003
#define ID_TRAY_MENU_LEVELSPY_DOWN	1004
#define ID_TRAY_MENU_NAMESPY		1005
#define ID_TRAY_MENU_INVISIBLE		1006
#define ID_TRAY_MENU_LIGHT			1007
#define ID_TRAY_MENU_HOTKEYS		1008
#define ID_TRAY_MENU_NUMPAD_HOTKEYS	1009
#define ID_TRAY_MENU_ABOUT			1010
#define ID_TRAY_MENU_HOMEPAGE		1011
#define ID_TRAY_MENU_EXIT			1012

// timers
#define ID_TIMER_HOTKEYS 10001

// numpad hotkeys
#define ID_NUMPAD_HOTKEY_LEVELSPY		100001
#define ID_NUMPAD_HOTKEY_LEVELSPY_UP	100002
#define ID_NUMPAD_HOTKEY_LEVELSPY_DOWN	100003
#define ID_NUMPAD_HOTKEY_NAMESPY		100005
#define ID_NUMPAD_HOTKEY_INVISIBLE		100006
#define ID_NUMPAD_HOTKEY_LIGHT			100007

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

// tray menu
HMENU trayMenu;

/* memory addresses */

// statusbar
#define STATUSBAR_TEXT	0x0077BA58 // 00535359 | AND EAX,0077BA58 or 00537B53 | MOV EDX,0077BA58
#define STATUSBAR_TIMER	0x0077BA54 // timer for showing text; 00535350 | MOV EAX,DWORD PTR DS:[77BA54] or 00537B6F | MOV DWORD PTR DS:[77BA54],EAX

// level spy
#define LEVELSPY_NOP	0x004CD600 // writeNops 2 bytes; 004CD600 | 2BC1 | SUB EAX,ECX

#define LEVELSPY_ABOVE	0x004CD5FC // writeBytes 1 byte; 004CD5FB | B8 07 | MOV EAX,7
#define LEVELSPY_BELOW	0x004CD604 // writeBytes 1 byte; 004CD603 | B8 02 | MOV EAX,2

// name spy
#define NAMESPY_NOP		0x004E7A09 // writeNops 2 bytes; 004E7A09 | 75 4A | JNZ SHORT then 004E7A0B | CALL 4CDF50
#define NAMESPY_NOP2	0x004E7A13 // writeNops 2 bytes; 004E7A13 | 75 40 | JNZ SHORT

// show invisible creatures
#define INVISIBLE1		0x0045B5D3 // writeBytes 1 byte; 0045B5D3 | 72 0A | JB SHORT
#define INVISIBLE2		0x004E6CD4 // writeBytes 1 byte; 004E6CD4 | 75 09 | JNZ SHORT

// full light
#define LIGHT_NOP		0x004E0369 // writeNops 2 bytes; 004E0369 | 7E 05 | JLE SHORT
#define LIGHT_AMOUNT	0x004E036C // writeBytes 1 byte; 004E036B | BB 80 | MOV EBX,80

// player
#define PLAYER_Z		0x0062CAF8 // 004CD5F0 | MOV ECX,DWORD PTR DS:[62CAF8] then 004CD5F6 | CMP ECX,7

/* constants */

// z-axis
const int Z_AXIS_DEFAULT = 7; // default ground level

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

// level spy
const int LEVELSPY_NOP_DEFAULT = 49451; // integer 2 bytes; 004CD600 | 2BC1 | SUB EAX,ECX

const int LEVELSPY_ABOVE_DEFAULT = 7; // 004CD5FB | B8 07 | MOV EAX,7
const int LEVELSPY_BELOW_DEFAULT = 2; // 004CD603 | B8 02 | MOV EAX,2

const int LEVELSPY_MIN = 0;
const int LEVELSPY_MAX = 7;

// name spy
const int NAMESPY_NOP_DEFAULT = 19061; // integer 2 bytes; 004E7A09 | 75 4A | JNZ SHORT then 004E7A0B | CALL 4CDF50
const int NAMESPY_NOP2_DEFAULT = 16501; // integer 2 bytes; 004E7A13 | 75 40 | JNZ SHORT

// show invisible creatures
const int INVISIBLE1_DEFAULT = 114; // integer 1 byte; 00453AF3 | 72 0A | JB SHORT
const int INVISIBLE2_DEFAULT = 117; // integer 1 byte; 004DE734 | 75 09 | JNZ SHORT

const int INVISIBLE_PATCH = 235; // integer 1 byte; 235 = EB = JMP; change JB and JNZ to JMP

// full light
const int LIGHT_NOP_DEFAULT = 1406; // integer 2 bytes; 004E0369 | 7E 05 | JLE SHORT

const int LIGHT_AMOUNT_DEFAULT = 128; // 004E036B | BB 80 | MOV EBX,80
const int LIGHT_AMOUNT_FULL = 255; // 004E036B | BB 80 | MOV EBX,80

/* booleans */

// hotkeys
int bHotkeys = 1;

// numpad hotkeys
int bNumpadHotkeys = 1;

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
void doShowAbout()
{
	MessageBox(0,
		"Level Spy for Tibia 8.2\nby Evremonde\nLast updated on July 2nd, 2008\n\nHotkeys:\nEnd\t\tToggle Level Spy On/Off\nPage Up\t\tLevel Spy Up\nPage Down\tLevel Spy Down\nHome\t\tToggle Name Spy On/Off\nDelete\t\tToggle Show Invisible Creatures On/Off\nInsert\t\tToggle Full Light On/Off\n\nNumpad Hotkeys:\n*\t\tToggle Level Spy On/Off\n+\t\tLevel Spy Up\n-\t\tLevel Spy Down\n/\t\tToggle Name Spy On/Off\nDel\t\tToggle Show Invisible Creatures On/Off\nIns\t\tToggle Full Light On/Off\n\nHomepage:\nhttp://code.google.com/p/evremonde/",
		"About", MB_OK | MB_ICONINFORMATION);
}

// open homepage
void doOpenHomepage()
{
	ShellExecute(0, 0, "http://code.google.com/p/evremonde/", 0, 0, SW_SHOW);
}

// check if level spy is enabled
int isLevelSpyEnabled()
{
	int levelSpyNop = readBytes(LEVELSPY_NOP, 2);
	if (levelSpyNop == LEVELSPY_NOP_DEFAULT)
		return 0;
	else
		return 1;
}

// check if name spy is enabled
int isNameSpyEnabled()
{
	int nameSpyNop = readBytes(NAMESPY_NOP, 2);
	int nameSpyNop2 = readBytes(NAMESPY_NOP2, 2);
	if ((nameSpyNop == NAMESPY_NOP_DEFAULT) || (nameSpyNop2 == NAMESPY_NOP2_DEFAULT))
		return 0;
	else
		return 1;
}

// check if show invisible creatures is enabled
int isShowInvisibleCreaturesEnabled()
{
	int invisible1 = readBytes(INVISIBLE1, 1);
	int invisible2 = readBytes(INVISIBLE2, 1);
	if ((invisible1 == INVISIBLE1_DEFAULT) || (invisible2 == INVISIBLE2_DEFAULT))
		return 0;
	else
		return 1;
}

// check if full light is enabled
int isFullLightEnabled()
{
	int lightNop = readBytes(LIGHT_NOP, 2);
	if (lightNop == LIGHT_NOP_DEFAULT)
		return 0;
	else
		return 1;
}

// initialize level spy
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

// disable level spy
void doLevelSpyOff()
{
	// disable level spy by restoring default values
	writeBytes(LEVELSPY_NOP, LEVELSPY_NOP_DEFAULT, 2);

	writeBytes(LEVELSPY_ABOVE, LEVELSPY_ABOVE_DEFAULT, 1);
	writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
}

// toggle level spy
void doLevelSpyToggle()
{
	// check if level spy is enabled
	if (isLevelSpyEnabled() == 0)
	{
		// enable level spy
		writeNops(LEVELSPY_NOP, 2);

		// initialize level spy
		doLevelSpyInit();

		CheckMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY, MF_CHECKED);
		EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_ENABLED);
		EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_ENABLED);

		setStatusbarText("Level Spy: On");
	}
	else
	{
		// disable level spy
		doLevelSpyOff();

		CheckMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY, MF_UNCHECKED);
		EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_DISABLED | MF_GRAYED);
		EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_DISABLED | MF_GRAYED);

		setStatusbarText("Level Spy: Off");
	}
}

// level spy up
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

	// get ground level
	int groundLevel = 0;
	if (playerZ <= Z_AXIS_DEFAULT)
		groundLevel = LEVELSPY_ABOVE; // above ground
	else
		groundLevel = LEVELSPY_BELOW; // below ground

	// get current level
	int currentLevel = readBytes(groundLevel, 1);

	// get spy level
	int spyLevel = 0;
	if (currentLevel >= LEVELSPY_MAX)
		spyLevel = LEVELSPY_MIN; // loop back to start
	else
		spyLevel = currentLevel + 1; // increase spy level

	// level spy up
	writeBytes(groundLevel, spyLevel, 1);

	// calculate display level
	int displayLevel = 0;
	if (playerZ <= Z_AXIS_DEFAULT)
		displayLevel = spyLevel; // above ground
	else
		displayLevel = spyLevel - (LEVELSPY_BELOW_DEFAULT + 1); // below ground

	// display level
	char buffer[256];
	sprintf(buffer, "Level Spy: %d", displayLevel);
	setStatusbarText(buffer);
}

// level spy down
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

	// get ground level
	int groundLevel = 0;
	if (playerZ <= Z_AXIS_DEFAULT)
		groundLevel = LEVELSPY_ABOVE; // above ground
	else
		groundLevel = LEVELSPY_BELOW; // below ground

	// get current level
	int currentLevel = readBytes(groundLevel, 1);

	// cannot spy below default z-axis
	//if (playerZ == Z_AXIS_DEFAULT && currentLevel == LEVELSPY_MIN)
	//{
		//setStatusbarText("You cannot spy below the default ground level!");
		//return;
	//}

	// get spy level
	int spyLevel = 0;
	if (currentLevel <= LEVELSPY_MIN)
		spyLevel = LEVELSPY_MAX; // loop back to start
	else
		spyLevel = currentLevel - 1; // decrease spy level

	// level spy down
	writeBytes(groundLevel, spyLevel, 1);

	// calculate display level
	int displayLevel = 0;
	if (playerZ <= Z_AXIS_DEFAULT)
		displayLevel = spyLevel; // above ground
	else
		displayLevel = spyLevel - (LEVELSPY_BELOW_DEFAULT + 1); // below ground

	// display level
	char buffer[256];
	sprintf(buffer, "Level Spy: %d", displayLevel);
	setStatusbarText(buffer);
}

// disable name spy
void doNameSpyOff()
{
	// disable name spy by restoring default values
	writeBytes(NAMESPY_NOP, NAMESPY_NOP_DEFAULT, 2);
	writeBytes(NAMESPY_NOP2, NAMESPY_NOP2_DEFAULT, 2);
}

// toggle name spy
void doNameSpyToggle()
{
	// check if name spy is enabled
	if (isNameSpyEnabled() == 0)
	{
		// enable name spy
		writeNops(NAMESPY_NOP, 2);
		writeNops(NAMESPY_NOP2, 2);

		CheckMenuItem(trayMenu, ID_TRAY_MENU_NAMESPY, MF_CHECKED);

		setStatusbarText("Name Spy: On");
	}
	else
	{
		// disable name spy
		doNameSpyOff();

		CheckMenuItem(trayMenu, ID_TRAY_MENU_NAMESPY, MF_UNCHECKED);

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
	if (isShowInvisibleCreaturesEnabled() == 0)
	{
		// enable show invisible creatures
		writeBytes(INVISIBLE1, INVISIBLE_PATCH, 1);
		writeBytes(INVISIBLE2, INVISIBLE_PATCH, 1);

		CheckMenuItem(trayMenu, ID_TRAY_MENU_INVISIBLE, MF_CHECKED);

		setStatusbarText("Show Invisible Creatures: On");
	}
	else
	{
		// disable show invisible creatures
		doShowInvisibleCreaturesOff();

		CheckMenuItem(trayMenu, ID_TRAY_MENU_INVISIBLE, MF_UNCHECKED);

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
	if (isFullLightEnabled() == 0)
	{
		// enable full light
		writeNops(LIGHT_NOP, 2);
		writeBytes(LIGHT_AMOUNT, LIGHT_AMOUNT_FULL, 1);

		CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_CHECKED);

		setStatusbarText("Full Light: On");
	}
	else
	{
		// disable full light
		doFullLightOff();

		CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_UNCHECKED);

		setStatusbarText("Full Light: Off");
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

// register numpad hotkeys
void doNumpadHotkeysRegister(HWND hwnd)
{
	RegisterHotKey(hwnd, ID_NUMPAD_HOTKEY_LEVELSPY, 0,		VK_MULTIPLY);	// numpad *
	RegisterHotKey(hwnd, ID_NUMPAD_HOTKEY_LEVELSPY_UP, 0,	VK_ADD);		// numpad +
	RegisterHotKey(hwnd, ID_NUMPAD_HOTKEY_LEVELSPY_DOWN, 0,	VK_SUBTRACT);	// numpad -
	RegisterHotKey(hwnd, ID_NUMPAD_HOTKEY_NAMESPY, 0,		VK_DIVIDE);		// numpad /
	RegisterHotKey(hwnd, ID_NUMPAD_HOTKEY_INVISIBLE, 0,		VK_DECIMAL);	// numpad del, numpad .
	RegisterHotKey(hwnd, ID_NUMPAD_HOTKEY_LIGHT, 0,			VK_NUMPAD0);	// numpad ins, numpad 0
}

// unregister numpad hotkeys
void doNumpadHotkeysUnregister(HWND hwnd)
{
	UnregisterHotKey(hwnd, ID_NUMPAD_HOTKEY_LEVELSPY);
	UnregisterHotKey(hwnd, ID_NUMPAD_HOTKEY_LEVELSPY_UP);
	UnregisterHotKey(hwnd, ID_NUMPAD_HOTKEY_LEVELSPY_DOWN);
	UnregisterHotKey(hwnd, ID_NUMPAD_HOTKEY_NAMESPY);
	UnregisterHotKey(hwnd, ID_NUMPAD_HOTKEY_INVISIBLE);
	UnregisterHotKey(hwnd, ID_NUMPAD_HOTKEY_LIGHT);
}

// toggle numpad hotkeys
void doNumpadHotkeysToggle()
{
	// check if numpad hotkeys are enabled
	if (bNumpadHotkeys == 0)
	{
		// enable numpad hotkeys
		bNumpadHotkeys = 1;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_NUMPAD_HOTKEYS, MF_CHECKED);

		setStatusbarText("Numpad Hotkeys: On");
	}
	else
	{
		// disable numpad hotkeys
		bNumpadHotkeys = 0;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_NUMPAD_HOTKEYS, MF_UNCHECKED);

		setStatusbarText("Numpad Hotkeys: Off");
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
	trayIcon.hIcon				= LoadIcon(0, IDI_WINLOGO);

	// set tray icon tip text
	strcpy(trayIcon.szTip, "Level Spy - Tibia 8.2");

	// add tray icon
	Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* tray menu */

// create tray menu
void doTrayMenuCreate(HWND hwnd)
{
	// create tray menu
	trayMenu = CreatePopupMenu();
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NULL,				"Level Spy - Tibia 8.2");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LEVELSPY,			"&Level Spy\tEnd");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LEVELSPY_UP,		"&Level Spy Up\tPage Up");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LEVELSPY_DOWN,		"&Level Spy Down\tPage Down");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NAMESPY,			"&Name Spy\tHome");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_INVISIBLE,			"Show &Invisible Creatures\tDelete");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LIGHT,				"&Full Light\tInsert");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOTKEYS,			"&Hotkeys");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NUMPAD_HOTKEYS,	"Numpad H&otkeys");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_ABOUT,				"&About...");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOMEPAGE,			"Home&page...");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_EXIT,				"E&xit");

	// set default tray menu item
	//SetMenuDefaultItem(trayMenu, ID_TRAY_MENU_NULL, MF_BYCOMMAND);

	// check tray menu items
	CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_CHECKED);
	CheckMenuItem(trayMenu, ID_TRAY_MENU_NUMPAD_HOTKEYS, MF_CHECKED);

	EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_DISABLED | MF_GRAYED);
	EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_DISABLED | MF_GRAYED);

	if (isLevelSpyEnabled() == 1)
	{
		CheckMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY, MF_CHECKED);
		EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_ENABLED);
		EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_ENABLED);
	}

	if (isNameSpyEnabled() == 1)
		CheckMenuItem(trayMenu, ID_TRAY_MENU_NAMESPY, MF_CHECKED);

	if (isShowInvisibleCreaturesEnabled() == 1)
		CheckMenuItem(trayMenu, ID_TRAY_MENU_INVISIBLE, MF_CHECKED);

	if (isFullLightEnabled() == 1)
		CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_CHECKED);

	if (getGameWindow() == 0)
	{
		CheckMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY, MF_UNCHECKED);
		EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_DISABLED | MF_GRAYED);
		EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_DISABLED | MF_GRAYED);
		CheckMenuItem(trayMenu, ID_TRAY_MENU_NAMESPY, MF_UNCHECKED);
		CheckMenuItem(trayMenu, ID_TRAY_MENU_INVISIBLE, MF_UNCHECKED);
		CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_UNCHECKED);
	}
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

	// level spy
	if (GetAsyncKeyState(VK_END)) // end
	{
		doLevelSpyToggle();
		Sleep(500); // wait
	}

	// level spy up
	if (GetAsyncKeyState(VK_PRIOR)) // page up
	{
		doLevelSpyUp();
		Sleep(100); // wait
	}

	// level spy down
	if (GetAsyncKeyState(VK_NEXT)) // page down
	{
		doLevelSpyDown();
		Sleep(100); // wait
	}

	// name spy
	if (GetAsyncKeyState(VK_HOME)) // home
	{
		doNameSpyToggle();
		Sleep(500); // wait
	}

	// show invisible creatures
	if (GetAsyncKeyState(VK_DELETE)) // delete
	{
		doShowInvisibleCreaturesToggle();
		Sleep(500); // wait
	}

	// full light
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
	doTrayIconCreate(hwnd);

	// create tray menu
	doTrayMenuCreate(hwnd);

	// register numpad hotkeys
	doNumpadHotkeysRegister(hwnd);

	// initialize timer
	SetTimer(hwnd, ID_TIMER_HOTKEYS, 10, 0); // speed is 10 milliseconds

	// loaded
	setStatusbarText("Level Spy for Tibia 8.2 loaded!");
}

void onDestroy(HWND hwnd)
{
	// unloaded
	setStatusbarText("Level Spy for Tibia 8.2 unloaded!");

	// disable level spy
	doLevelSpyOff();

	// disable name spy
	//doNameSpyOff();

	// disable show invisible creatures
	//doShowInvisibleCreaturesOff();

	// disable full light
	//doFullLightOff();

	// remove tray icon
	Shell_NotifyIcon(NIM_DELETE, &trayIcon);

	// destroy tray menu
	DestroyMenu(trayMenu);

	// unregister numpad hotkeys
	doNumpadHotkeysUnregister(hwnd);

	// kill timer
	KillTimer(hwnd, ID_TIMER_HOTKEYS);

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

void onHotkey(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	// check if numpad hotkeys are enabled
	if (bNumpadHotkeys == 0)
		return;

	// restrict numpad hotkeys to game and application by window focus
	HWND currentWindow = GetForegroundWindow();
	HWND gameWindow = getGameWindow();
	if (currentWindow != gameWindow && currentWindow != hwnd)
		return;

	// numpad hotkeys
	switch (wparam)
	{
		// level spy
		case ID_NUMPAD_HOTKEY_LEVELSPY:
			doLevelSpyToggle();
			break;

		// level spy up
		case ID_NUMPAD_HOTKEY_LEVELSPY_UP:
			doLevelSpyUp();
			break;

		// level spy down
		case ID_NUMPAD_HOTKEY_LEVELSPY_DOWN:
			doLevelSpyDown();
			break;

		// name spy
		case ID_NUMPAD_HOTKEY_NAMESPY:
			doNameSpyToggle();
			break;

		// show invisible creatures
		case ID_NUMPAD_HOTKEY_INVISIBLE:
			doShowInvisibleCreaturesToggle();
			break;

		// full light
		case ID_NUMPAD_HOTKEY_LIGHT:
			doFullLightToggle();
			break;
	}
}

void onCommand(HWND hwnd, int command, int notifycode, HWND hwnditem)
{
	switch (command)
	{
		// level spy
		case ID_TRAY_MENU_LEVELSPY:
			doLevelSpyToggle();
			break;

		// level spy up
		case ID_TRAY_MENU_LEVELSPY_UP:
			doLevelSpyUp();
			break;

		// level spy down
		case ID_TRAY_MENU_LEVELSPY_DOWN:
			doLevelSpyDown();
			break;

		// name spy
		case ID_TRAY_MENU_NAMESPY:
			doNameSpyToggle();
			break;

		// show invisible creatures
		case ID_TRAY_MENU_INVISIBLE:
			doShowInvisibleCreaturesToggle();
			break;

		// full light
		case ID_TRAY_MENU_LIGHT:
			doFullLightToggle();
			break;

		// hotkeys
		case ID_TRAY_MENU_HOTKEYS:
			doHotkeysToggle();
			break;

		// numpad hotkeys
		case ID_TRAY_MENU_NUMPAD_HOTKEYS:
			doNumpadHotkeysToggle();
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

		case WM_HOTKEY:
			onHotkey(hwnd, wparam, lparam);
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
	HWND applicationWindow = FindWindow(0, "Level Spy - Tibia 8.2");
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
	wc.lpszClassName	= "levelspy";

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
		"levelspy", "Level Spy - Tibia 8.2", // window class and title
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
