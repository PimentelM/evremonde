/*
Chameleon for Tibia 8.22
by Evremonde
Last updated on August 13th, 2008
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
#define ID_TRAY_MENU_NULL								1001
#define ID_TRAY_MENU_NULL_OUTFIT						1002
#define ID_TRAY_MENU_OUTFIT_RANDOM						1003
#define ID_TRAY_MENU_NULL_OUTFIT_ADDON					1004
#define ID_TRAY_MENU_OUTFIT_ADDON_NONE					1005
#define ID_TRAY_MENU_OUTFIT_ADDON_1						1006
#define ID_TRAY_MENU_OUTFIT_ADDON_2						1007
#define ID_TRAY_MENU_OUTFIT_ADDON_BOTH					1008
#define ID_TRAY_MENU_OUTFIT								1009
#define ID_TRAY_MENU_OUTFIT_ADDON						1010
#define ID_TRAY_MENU_RAINBOW							1011
#define ID_TRAY_MENU_ABOUT								1012
#define ID_TRAY_MENU_HOMEPAGE							1013
#define ID_TRAY_MENU_EXIT								1014

#define ID_TRAY_MENU_OUTFIT_MALE_CITIZEN				1015
#define ID_TRAY_MENU_OUTFIT_MALE_HUNTER					1016
#define ID_TRAY_MENU_OUTFIT_MALE_MAGE					1017
#define ID_TRAY_MENU_OUTFIT_MALE_KNIGHT					1018
#define ID_TRAY_MENU_OUTFIT_MALE_NOBLEMAN				1019
#define ID_TRAY_MENU_OUTFIT_MALE_SUMMONER				1020
#define ID_TRAY_MENU_OUTFIT_MALE_WARRIOR				1021
#define ID_TRAY_MENU_OUTFIT_MALE_BARBARIAN				1022
#define ID_TRAY_MENU_OUTFIT_MALE_DRUID					1023
#define ID_TRAY_MENU_OUTFIT_MALE_WIZARD					1024
#define ID_TRAY_MENU_OUTFIT_MALE_ORIENTAL				1025
#define ID_TRAY_MENU_OUTFIT_MALE_PIRATE					1026
#define ID_TRAY_MENU_OUTFIT_MALE_ASSASSIN				1027
#define ID_TRAY_MENU_OUTFIT_MALE_BEGGAR					1028
#define ID_TRAY_MENU_OUTFIT_MALE_SHAMAN					1029
#define ID_TRAY_MENU_OUTFIT_MALE_NORSEMAN				1030
#define ID_TRAY_MENU_OUTFIT_MALE_NIGHTMARE				1031
#define ID_TRAY_MENU_OUTFIT_MALE_JESTER					1032
#define ID_TRAY_MENU_OUTFIT_MALE_BROTHERHOOD			1033
#define ID_TRAY_MENU_OUTFIT_MALE_DEMONHUNTER			1034

#define ID_TRAY_MENU_OUTFIT_FEMALE_CITIZEN				1035
#define ID_TRAY_MENU_OUTFIT_FEMALE_HUNTER				1036
#define ID_TRAY_MENU_OUTFIT_FEMALE_MAGE					1037
#define ID_TRAY_MENU_OUTFIT_FEMALE_KNIGHT				1038
#define ID_TRAY_MENU_OUTFIT_FEMALE_NOBLEMAN				1039
#define ID_TRAY_MENU_OUTFIT_FEMALE_SUMMONER				1040
#define ID_TRAY_MENU_OUTFIT_FEMALE_WARRIOR				1041
#define ID_TRAY_MENU_OUTFIT_FEMALE_BARBARIAN			1042
#define ID_TRAY_MENU_OUTFIT_FEMALE_DRUID				1043
#define ID_TRAY_MENU_OUTFIT_FEMALE_WIZARD				1044
#define ID_TRAY_MENU_OUTFIT_FEMALE_ORIENTAL				1045
#define ID_TRAY_MENU_OUTFIT_FEMALE_PIRATE				1046
#define ID_TRAY_MENU_OUTFIT_FEMALE_ASSASSIN				1047
#define ID_TRAY_MENU_OUTFIT_FEMALE_BEGGAR				1048
#define ID_TRAY_MENU_OUTFIT_FEMALE_SHAMAN				1049
#define ID_TRAY_MENU_OUTFIT_FEMALE_NORSEMAN				1050
#define ID_TRAY_MENU_OUTFIT_FEMALE_NIGHTMARE			1051
#define ID_TRAY_MENU_OUTFIT_FEMALE_JESTER				1052
#define ID_TRAY_MENU_OUTFIT_FEMALE_BROTHERHOOD			1053
#define ID_TRAY_MENU_OUTFIT_FEMALE_DEMONHUNTER			1054

#define ID_TRAY_MENU_OUTFIT_OTHER_GAMEMASTER			1055
#define ID_TRAY_MENU_OUTFIT_OTHER_COMMUNITY_MAMANGER	1056
#define ID_TRAY_MENU_OUTFIT_OTHER_HERO					1057
#define ID_TRAY_MENU_OUTFIT_OTHER_FERUMBRAS				1058
#define ID_TRAY_MENU_OUTFIT_OTHER_DEMON					1059
#define ID_TRAY_MENU_OUTFIT_OTHER_BLUE_DEMON			1060
#define ID_TRAY_MENU_OUTFIT_OTHER_PANDA					1061
#define ID_TRAY_MENU_OUTFIT_OTHER_SHEEP					1062
#define ID_TRAY_MENU_OUTFIT_OTHER_COCKROACH				1063
#define ID_TRAY_MENU_OUTFIT_OTHER_OLD_MALE				1064
#define ID_TRAY_MENU_OUTFIT_OTHER_OLD_FEMALE			1065
#define ID_TRAY_MENU_OUTFIT_OTHER_SWIMMING				1066

// timers
#define ID_TIMER_CHAMELEON 10001

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

// tray menu
HMENU trayMenu;
HMENU traySubMenu;

/* memory addresses */

// statusbar
#define STATUSBAR_TEXT		0x00781270
#define STATUSBAR_TIMER		0x0078126C // timer for showing text

// battlelist
#define BATTLELIST_BEGIN	0x00626CD0 // OffsetCreatureType
#define BATTLELIST_END		0x0062CA90 // = BATTLELIST_BEGIN + (STEP_CREATURE * MAX_CREATURES)

// player
#define PLAYER_ID			0x00626C70 // unique id

/* constants */

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

// distances to step between addresses in memory
const int STEP_CREATURE = 160; // creatures in battle list

/* enumerated constants */

// max
enum
{
	MAX_CREATURES		= 150,	// creatures in battle list
	MAX_OUTFIT			= 301,	// highest possible outfit before client crashes
	MAX_OUTFIT_COLORS	= 132	// possible outfit colors to choose
};


// offsets to variables from battle list step
enum OffsetCreatureType
{
	OFFSET_CREATURE_ID				= 0,
	OFFSET_CREATURE_OUTFIT			= 96,	// OutfitType
	OFFSET_CREATURE_OUTFIT_HEAD		= 100,	// OutfitColorType
	OFFSET_CREATURE_OUTFIT_BODY		= 104,	// OutfitColorType
	OFFSET_CREATURE_OUTFIT_LEGS		= 108,	// OutfitColorType
	OFFSET_CREATURE_OUTFIT_FEET		= 112,	// OutfitColorType
	OFFSET_CREATURE_OUTFIT_ADDON	= 116	// OutfitAddonType
};

// outfit
enum OutfitType
{
	//outfits that don't exist and cause the client to crash
	// 1, 135, 161-191, 302+

	OUTFIT_INVISIBLE			= 0,
	OUTFIT_SWIMMING				= 267,

	OUTFIT_GAMEMASTER			= 75,
	OUTFIT_COMMUNITY_MANAGER	= 266,

	OUTFIT_BLUE_DEMON			= 12,
	OUTFIT_SHEEP				= 14,
	OUTFIT_DEMON				= 35,
	OUTFIT_HERO					= 73,
	OUTFIT_PANDA				= 123,
	OUTFIT_FERUMBRAS			= 229,
	OUTFIT_COCKROACH			= 284,

	OUTFIT_MALE_OLD				= 127,	// no animations
	OUTFIT_FEMALE_OLD			= 126,	// no animations

	OUTFIT_MALE_CITIZEN			= 128,	// druid
	OUTFIT_MALE_HUNTER			= 129,	// paladin
	OUTFIT_MALE_MAGE			= 130,	// sorcerer
	OUTFIT_MALE_KNIGHT			= 131,	// knight

	OUTFIT_MALE_NOBLEMAN		= 132,
	OUTFIT_MALE_SUMMONER		= 133,
	OUTFIT_MALE_WARRIOR			= 134,
	OUTFIT_MALE_BARBARIAN		= 143,
	OUTFIT_MALE_DRUID			= 144,
	OUTFIT_MALE_WIZARD			= 145,
	OUTFIT_MALE_ORIENTAL		= 146,
	OUTFIT_MALE_PIRATE			= 151,
	OUTFIT_MALE_ASSASSIN		= 152,
	OUTFIT_MALE_BEGGAR			= 153,
	OUTFIT_MALE_SHAMAN			= 154,
	OUTFIT_MALE_NORSEMAN		= 251,
	OUTFIT_MALE_NIGHTMARE		= 268,
	OUTFIT_MALE_JESTER			= 273,
	OUTFIT_MALE_BROTHERHOOD		= 278,
	OUTFIT_MALE_DEMONHUNTER		= 289,

	OUTFIT_FEMALE_CITIZEN		= 136,	// druid
	OUTFIT_FEMALE_HUNTER		= 137,	// paladin
	OUTFIT_FEMALE_SUMMONER		= 138,  // sorcerer
	OUTFIT_FEMALE_KNIGHT		= 139,	// knight

	OUTFIT_FEMALE_NOBLEMAN		= 140,
	OUTFIT_FEMALE_MAGE			= 141,
	OUTFIT_FEMALE_WARRIOR		= 142,
	OUTFIT_FEMALE_BARBARIAN		= 147,
	OUTFIT_FEMALE_DRUID			= 148,
	OUTFIT_FEMALE_WIZARD		= 149,
	OUTFIT_FEMALE_ORIENTAL		= 150,
	OUTFIT_FEMALE_PIRATE		= 155,
	OUTFIT_FEMALE_ASSASSIN		= 156,
	OUTFIT_FEMALE_BEGGAR		= 157,
	OUTFIT_FEMALE_SHAMAN		= 158,
	OUTFIT_FEMALE_NORSEMAN		= 252,
	OUTFIT_FEMALE_NIGHTMARE		= 269,
	OUTFIT_FEMALE_JESTER		= 270,
	OUTFIT_FEMALE_BROTHERHOOD	= 279,
	OUTFIT_FEMALE_DEMONHUNTER	= 288
};

// outfit addon
enum OutfitAddonType
{
	OUTFIT_ADDON_NONE	= 0,
	OUTFIT_ADDON_1		= 1,
	OUTFIT_ADDON_2		= 2,
	OUTFIT_ADDON_BOTH	= 3
};

/* booleans */

// outfit
int bOutfit = 1;

// outfit addon
int bOutfitAddon = 1;

// rainbow

int bRainbow = 0;

/* variables */

// outfit
int outfit = 0;

// outfit addon
int outfitAddon = 0;

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

// get player id
int getPlayerId() { return readBytes(PLAYER_ID, 3); }

// set player offset
void setPlayerOffset(int offset, int value, int bytes)
{
	// search through battle list
	int i = 0;
	for(i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		// get creature id
		int creatureId = readBytes(i + OFFSET_CREATURE_ID, 3);

		// creature is player
		if (creatureId == getPlayerId())
		{
			writeBytes(i + offset, value, bytes);
			break; // player was found
		}
	}
}

// set player outfit
void setPlayerOutfit(int outfitType) { setPlayerOffset(OFFSET_CREATURE_OUTFIT, outfitType, 2); }

// set player outfit addon
void setPlayerOutfitAddon(int outfitAddonType) { setPlayerOffset(OFFSET_CREATURE_OUTFIT_ADDON, outfitAddonType, 1); }

// set player outfit colors
void setPlayerOutfitColors(int head, int body, int legs, int feet)
{
	setPlayerOffset(OFFSET_CREATURE_OUTFIT_HEAD, head, 1);
	setPlayerOffset(OFFSET_CREATURE_OUTFIT_BODY, body, 1);
	setPlayerOffset(OFFSET_CREATURE_OUTFIT_LEGS, legs, 1);
	setPlayerOffset(OFFSET_CREATURE_OUTFIT_FEET, feet, 1);
}

// show about
void doShowAbout()
{
	MessageBox(0,
		"Chameleon for Tibia 8.22\nby Evremonde\nLast updated on August 13th, 2008\n\nHomepage:\nhttp://code.google.com/p/evremonde/",
		"About", MB_OK | MB_ICONINFORMATION);
}

// open homepage
void doOpenHomepage()
{
	ShellExecute(0, 0, "http://code.google.com/p/evremonde/", 0, 0, SW_SHOW);
}

// outfit random
void doOutfitRandom()
{
	// get random outfit
	int randomOutfit = rand() % MAX_OUTFIT;

	// avoid client crash
	switch (randomOutfit)
	{
		case 0:
		case 1:
		case 135:
		case 161:
		case 162:
		case 163:
		case 164:
		case 165:
		case 166:
		case 167:
		case 168:
		case 169:
		case 170:
		case 171:
		case 172:
		case 173:
		case 174:
		case 175:
		case 176:
		case 177:
		case 178:
		case 179:
		case 180:
		case 181:
		case 182:
		case 183:
		case 184:
		case 185:
		case 186:
		case 187:
		case 188:
		case 189:
		case 190:
		case 191:
		//case 302:
			doOutfitRandom();
			return;
	}

	// avoid same outfit
	if (randomOutfit == outfit)
	{
		doOutfitRandom();
		return;
	}

	// set outfit
	outfit = randomOutfit;
}

// toggle outfit
void doOutfitToggle()
{
	// check if outfit is enabled
	if (bOutfit == 0)
	{
		// enable outfit
		bOutfit = 1;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_OUTFIT, MF_CHECKED);

		setStatusbarText("Outfit: On");
	}
	else
	{
		// disable outfit
		bOutfit = 0;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_OUTFIT, MF_UNCHECKED);

		setStatusbarText("Outfit: Off");
	}
}

// toggle outfit addon
void doOutfitAddonToggle()
{
	// check if outfit addon is enabled
	if (bOutfitAddon == 0)
	{
		// enable outfit addon
		bOutfitAddon = 1;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_OUTFIT_ADDON, MF_CHECKED);

		setStatusbarText("Outfit Addon: On");
	}
	else
	{
		// disable outfit addon
		bOutfitAddon = 0;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_OUTFIT_ADDON, MF_UNCHECKED);

		setStatusbarText("Outfit Addon: Off");
	}
}

// toggle rainbow
void doRainbowToggle()
{
	// check if rainbow is enabled
	if (bRainbow == 0)
	{
		// enable rainbow
		bRainbow = 1;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_RAINBOW, MF_CHECKED);

		setStatusbarText("Rainbow: On");
	}
	else
	{
		// disable rainbow
		bRainbow = 0;

		CheckMenuItem(trayMenu, ID_TRAY_MENU_RAINBOW, MF_UNCHECKED);

		setStatusbarText("Rainbow: Off");
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
	strcpy(trayIcon.szTip, "Chameleon - Tibia 8.22");

	// add tray icon
	Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* tray menu */

// create tray menu
void doTrayMenuCreate(HWND hwnd)
{
	// create tray menu
	trayMenu = CreatePopupMenu();
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NULL,									"Chameleon - Tibia 8.22");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NULL_OUTFIT,							"(Outfit)");

	traySubMenu = CreatePopupMenu();
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_CITIZEN,				"Citizen");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_HUNTER,					"Hunter");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_MAGE,					"Mage");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_KNIGHT,					"Knight");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_NOBLEMAN,				"Nobleman");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_SUMMONER,				"Summoner");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_WARRIOR,				"Warrior");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_BARBARIAN,				"Barbarian");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_DRUID,					"Druid");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_WIZARD,					"Wizard");
	AppendMenu(traySubMenu, MF_STRING | MF_MENUBARBREAK, ID_TRAY_MENU_OUTFIT_MALE_ORIENTAL, "Oriental");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_PIRATE,					"Pirate");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_ASSASSIN,				"Assassin");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_BEGGAR,					"Beggar");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_SHAMAN,					"Shaman");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_NORSEMAN,				"Norseman");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_NIGHTMARE,				"Nightmare");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_JESTER,					"Jester");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_BROTHERHOOD,			"Brotherhood");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_MALE_DEMONHUNTER,			"Demonhunter");
	AppendMenu(trayMenu, MF_STRING | MF_POPUP, (UINT)traySubMenu,						"Male");

	traySubMenu = CreatePopupMenu();
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_CITIZEN,				"Citizen");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_HUNTER,				"Hunter");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_MAGE,					"Mage");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_KNIGHT,				"Knight");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_NOBLEMAN,				"Nobleman");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_SUMMONER,				"Summoner");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_WARRIOR,				"Warrior");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_BARBARIAN,			"Barbarian");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_DRUID,				"Druid");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_WIZARD,				"Wizard");
	AppendMenu(traySubMenu, MF_STRING | MF_MENUBARBREAK, ID_TRAY_MENU_OUTFIT_FEMALE_ORIENTAL, "Oriental");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_PIRATE,				"Pirate");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_ASSASSIN,				"Assassin");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_BEGGAR,				"Beggar");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_SHAMAN,				"Shaman");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_NORSEMAN,				"Norseman");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_NIGHTMARE,			"Nightmare");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_JESTER,				"Jester");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_BROTHERHOOD,			"Brotherhood");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_FEMALE_DEMONHUNTER,			"Demonhunter");
	AppendMenu(trayMenu, MF_STRING | MF_POPUP, (UINT)traySubMenu,						"Female");

	traySubMenu = CreatePopupMenu();
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_GAMEMASTER,			"Gamemaster");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_COMMUNITY_MAMANGER,	"Community Manager");
	AppendMenu(traySubMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_FERUMBRAS,				"Ferumbras");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_HERO,					"Hero");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_DEMON,					"Demon");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_BLUE_DEMON,			"Blue Demon");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_PANDA,					"Panda");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_SHEEP,					"Sheep");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_COCKROACH,				"Cockroach");
	AppendMenu(traySubMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_OLD_MALE,				"Old Male");
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_OLD_FEMALE,			"Old Female");
	AppendMenu(traySubMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(traySubMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_OTHER_SWIMMING,				"Swimming");
	AppendMenu(trayMenu, MF_STRING | MF_POPUP, (UINT)traySubMenu,						"Other");

	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_RANDOM,							"Random");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NULL_OUTFIT_ADDON,						"(Outfit Addon)");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_ADDON_NONE,						"None");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_ADDON_1,						"Addon #1");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_ADDON_2,						"Addon #2");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_ADDON_BOTH,						"Both");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_OUTFIT,								"&Outfit");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_OUTFIT_ADDON,							"O&utfit Addon");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_RAINBOW,								"&Rainbow");
	AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_ABOUT,									"&About...");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOMEPAGE,								"Home&page...");
	AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_EXIT,									"E&xit");

	// set default tray menu item
	//SetMenuDefaultItem(trayMenu, ID_TRAY_MENU_NULL, MF_BYCOMMAND);

	// check tray menu items
	CheckMenuItem(trayMenu, ID_TRAY_MENU_OUTFIT, MF_CHECKED);
	CheckMenuItem(trayMenu, ID_TRAY_MENU_OUTFIT_ADDON, MF_CHECKED);

	CheckMenuRadioItem(trayMenu,
		ID_TRAY_MENU_OUTFIT_ADDON_NONE, ID_TRAY_MENU_OUTFIT_ADDON_BOTH,
		ID_TRAY_MENU_OUTFIT_ADDON_NONE, MF_BYCOMMAND);

	EnableMenuItem(trayMenu, ID_TRAY_MENU_NULL_OUTFIT, MF_DISABLED | MF_GRAYED);
	EnableMenuItem(trayMenu, ID_TRAY_MENU_NULL_OUTFIT_ADDON, MF_DISABLED | MF_GRAYED);
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

// chameleon
void timerChameleon(HWND hwnd)
{
	// outfit
	if (bOutfit == 1 && outfit != 0)
		setPlayerOutfit(outfit);

	// outfit addon
	if (bOutfitAddon == 1)
		setPlayerOutfitAddon(outfitAddon);

	// rainbow
	if (bRainbow == 1)
		setPlayerOutfitColors(rand() % MAX_OUTFIT_COLORS, rand() % MAX_OUTFIT_COLORS, rand() % MAX_OUTFIT_COLORS, rand() % MAX_OUTFIT_COLORS);
}

/* windows messages */

void onCreate(HWND hwnd)
{
	// create tray icon
	doTrayIconCreate(hwnd);

	// create tray menu
	doTrayMenuCreate(hwnd);

	// initialize timer
	SetTimer(hwnd, ID_TIMER_CHAMELEON, 100, 0); // speed is 100 milliseconds

	// loaded
	setStatusbarText("Chameleon for Tibia 8.22 loaded!");
}

void onDestroy(HWND hwnd)
{
	// unloaded
	setStatusbarText("Chameleon for Tibia 8.22 unloaded!");

	// remove tray icon
	Shell_NotifyIcon(NIM_DELETE, &trayIcon);

	// destroy tray menu
	DestroyMenu(trayMenu);

	// kill timer
	KillTimer(hwnd, ID_TIMER_CHAMELEON);

	// exit
	PostQuitMessage(0);
}

void onTimer(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	// timers
	switch (wparam)
	{
		// chameleon
		case ID_TIMER_CHAMELEON:
			timerChameleon(hwnd);
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
		// outfit male
		case ID_TRAY_MENU_OUTFIT_MALE_CITIZEN:
			outfit = OUTFIT_MALE_CITIZEN;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_HUNTER:
			outfit = OUTFIT_MALE_HUNTER;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_MAGE:
			outfit = OUTFIT_MALE_MAGE;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_KNIGHT:
			outfit = OUTFIT_MALE_KNIGHT;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_NOBLEMAN:
			outfit = OUTFIT_MALE_NOBLEMAN;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_SUMMONER:
			outfit = OUTFIT_MALE_SUMMONER;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_WARRIOR:
			outfit = OUTFIT_MALE_WARRIOR;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_BARBARIAN:
			outfit = OUTFIT_MALE_BARBARIAN;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_DRUID:
			outfit = OUTFIT_MALE_DRUID;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_WIZARD:
			outfit = OUTFIT_MALE_WIZARD;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_ORIENTAL:
			outfit = OUTFIT_MALE_ORIENTAL;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_PIRATE:
			outfit = OUTFIT_MALE_PIRATE;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_ASSASSIN:
			outfit = OUTFIT_MALE_ASSASSIN;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_BEGGAR:
			outfit = OUTFIT_MALE_BEGGAR;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_SHAMAN:
			outfit = OUTFIT_MALE_SHAMAN;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_NORSEMAN:
			outfit = OUTFIT_MALE_NORSEMAN;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_NIGHTMARE:
			outfit = OUTFIT_MALE_NIGHTMARE;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_JESTER:
			outfit = OUTFIT_MALE_JESTER;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_BROTHERHOOD:
			outfit = OUTFIT_MALE_BROTHERHOOD;
			break;

		case ID_TRAY_MENU_OUTFIT_MALE_DEMONHUNTER:
			outfit = OUTFIT_MALE_DEMONHUNTER;
			break;

		// outfit female
		case ID_TRAY_MENU_OUTFIT_FEMALE_CITIZEN:
			outfit = OUTFIT_FEMALE_CITIZEN;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_HUNTER:
			outfit = OUTFIT_FEMALE_HUNTER;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_MAGE:
			outfit = OUTFIT_FEMALE_MAGE;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_KNIGHT:
			outfit = OUTFIT_FEMALE_KNIGHT;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_NOBLEMAN:
			outfit = OUTFIT_FEMALE_NOBLEMAN;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_SUMMONER:
			outfit = OUTFIT_FEMALE_SUMMONER;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_WARRIOR:
			outfit = OUTFIT_FEMALE_WARRIOR;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_BARBARIAN:
			outfit = OUTFIT_FEMALE_BARBARIAN;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_DRUID:
			outfit = OUTFIT_FEMALE_DRUID;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_WIZARD:
			outfit = OUTFIT_FEMALE_WIZARD;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_ORIENTAL:
			outfit = OUTFIT_FEMALE_ORIENTAL;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_PIRATE:
			outfit = OUTFIT_FEMALE_PIRATE;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_ASSASSIN:
			outfit = OUTFIT_FEMALE_ASSASSIN;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_BEGGAR:
			outfit = OUTFIT_FEMALE_BEGGAR;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_SHAMAN:
			outfit = OUTFIT_FEMALE_SHAMAN;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_NORSEMAN:
			outfit = OUTFIT_FEMALE_NORSEMAN;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_NIGHTMARE:
			outfit = OUTFIT_FEMALE_NIGHTMARE;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_JESTER:
			outfit = OUTFIT_FEMALE_JESTER;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_BROTHERHOOD:
			outfit = OUTFIT_FEMALE_BROTHERHOOD;
			break;

		case ID_TRAY_MENU_OUTFIT_FEMALE_DEMONHUNTER:
			outfit = OUTFIT_FEMALE_DEMONHUNTER;
			break;

		// outfit other
		case ID_TRAY_MENU_OUTFIT_OTHER_GAMEMASTER:
			outfit = OUTFIT_GAMEMASTER;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_COMMUNITY_MAMANGER:
			outfit = OUTFIT_COMMUNITY_MANAGER;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_HERO:
			outfit = OUTFIT_HERO;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_FERUMBRAS:
			outfit = OUTFIT_FERUMBRAS;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_DEMON:
			outfit = OUTFIT_DEMON;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_BLUE_DEMON:
			outfit = OUTFIT_BLUE_DEMON;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_PANDA:
			outfit = OUTFIT_PANDA;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_SHEEP:
			outfit = OUTFIT_SHEEP;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_COCKROACH:
			outfit = OUTFIT_COCKROACH;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_OLD_MALE:
			outfit = OUTFIT_MALE_OLD;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_OLD_FEMALE:
			outfit = OUTFIT_FEMALE_OLD;
			break;

		case ID_TRAY_MENU_OUTFIT_OTHER_SWIMMING:
			outfit = OUTFIT_SWIMMING;
			break;

		// outfit random
		case ID_TRAY_MENU_OUTFIT_RANDOM:
			doOutfitRandom();
			break;

		// outfit addon none
		case ID_TRAY_MENU_OUTFIT_ADDON_NONE:
			outfitAddon = OUTFIT_ADDON_NONE;
			CheckMenuRadioItem(trayMenu,
				ID_TRAY_MENU_OUTFIT_ADDON_NONE, ID_TRAY_MENU_OUTFIT_ADDON_BOTH,
				ID_TRAY_MENU_OUTFIT_ADDON_NONE, MF_BYCOMMAND);
			break;

		// outfit addon 1
		case ID_TRAY_MENU_OUTFIT_ADDON_1:
			outfitAddon = OUTFIT_ADDON_1;
			CheckMenuRadioItem(trayMenu,
				ID_TRAY_MENU_OUTFIT_ADDON_NONE, ID_TRAY_MENU_OUTFIT_ADDON_BOTH,
				ID_TRAY_MENU_OUTFIT_ADDON_1, MF_BYCOMMAND);
			break;

		// outfit addon 2
		case ID_TRAY_MENU_OUTFIT_ADDON_2:
			outfitAddon = OUTFIT_ADDON_2;
			CheckMenuRadioItem(trayMenu,
				ID_TRAY_MENU_OUTFIT_ADDON_NONE, ID_TRAY_MENU_OUTFIT_ADDON_BOTH,
				ID_TRAY_MENU_OUTFIT_ADDON_2, MF_BYCOMMAND);
			break;

		// outfit addon both
		case ID_TRAY_MENU_OUTFIT_ADDON_BOTH:
			outfitAddon = OUTFIT_ADDON_BOTH;
			CheckMenuRadioItem(trayMenu,
				ID_TRAY_MENU_OUTFIT_ADDON_NONE, ID_TRAY_MENU_OUTFIT_ADDON_BOTH,
				ID_TRAY_MENU_OUTFIT_ADDON_BOTH, MF_BYCOMMAND);
			break;

		// outfit
		case ID_TRAY_MENU_OUTFIT:
			doOutfitToggle();
			break;

		// outfit addon
		case ID_TRAY_MENU_OUTFIT_ADDON:
			doOutfitAddonToggle();
			break;

		// rainbow
		case ID_TRAY_MENU_RAINBOW:
			doRainbowToggle();
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
	// initialize random number seed
	srand(time(0));

	// only allow one instance of application
	HWND applicationWindow = FindWindow(0, "Chameleon - Tibia 8.22");
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
	wc.lpszClassName	= "chameleon";

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
		"chameleon", "Chameleon - Tibia 8.22", // window class and title
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
