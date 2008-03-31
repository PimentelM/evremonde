#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <windows.h>
#include <shellapi.h>

#include "ini.h"

#include "looter81.h"

#include "tibia.h"
#include "tibiapacket.h"

/*
Looter for Tibia 8.1
by Evremonde
Last updated on March 31st, 2008
*/

/* globals */

// tibia class
CTibia Tibia;

// tibia packet class
CTibiaPacket TibiaPacket;

// ini class
CIni Ini;

// looter ini filename
std::string looterIni = "looter81.ini";

// homepage url
std::string homepageUrl = "http://code.google.com/p/evremonde/";

// tray menu
HMENU trayMenu;

// tray icon
NOTIFYICONDATA trayIcon;
POINT trayLocation;

/* booleans */

bool bLoot	= true;
bool bStack	= true;
bool bEat	= true;

/* load */

void loadOptions()
{
	int lootItems = Ini.readInteger(looterIni, "Options", "LootItems");
	if (lootItems == 1)
	{
		::CheckMenuItem(trayMenu, ID_MENU_LOOT, MF_CHECKED);
		bLoot = true;
	}
	else
	{
		::CheckMenuItem(trayMenu, ID_MENU_LOOT, MF_UNCHECKED);
		bLoot = false;
	}
	
	int stackItems = Ini.readInteger(looterIni, "Options", "StackItems");
	if (stackItems == 1)
	{
		::CheckMenuItem(trayMenu, ID_MENU_STACK, MF_CHECKED);
		bStack = true;
	}
	else
	{
		::CheckMenuItem(trayMenu, ID_MENU_STACK, MF_UNCHECKED);
		bStack = false;
	}

	int eatItems = Ini.readInteger(looterIni, "Options", "EatItems");
	if (eatItems == 1)
	{
		::CheckMenuItem(trayMenu, ID_MENU_EAT, MF_CHECKED);
		bEat = true;
	}
	else
	{
		::CheckMenuItem(trayMenu, ID_MENU_EAT, MF_UNCHECKED);
		bEat = false;
	}
}

/* do */

void doLoot()
{
	// get player cap
	int playerCap = Tibia.getPlayerCap();

	// not enough cap to loot items
	if (playerCap == 0)
		return;

	// not enough space to loot items
	if (Tibia.getStatusbarText().find("You cannot put more objects in this container.") != std::string::npos)
		return;

	std::vector<std::string> sections;
	std::vector<std::string>::iterator sectionsIt;

	// load section names into vector
	sections = Ini.getSections(looterIni);
	if (sections.empty())
		return;

	// loop through section names
	for(sectionsIt = sections.begin(); sectionsIt != sections.end(); sectionsIt++)
	{
		// get section name
		std::string section = *(sectionsIt);

		// skip options
		if (section == "Options")
			continue;

		// check if item will be looted
		int loot = Ini.readInteger(looterIni, section, "Loot");
		if (loot == 0)
			continue;

		// get item id
		int id = Ini.readInteger(looterIni, section, "ID");
		if (id == 0)
			continue;

		// get slot
		int slot = Ini.readInteger(looterIni, section, "Slot");

		// set loot slot
		int lootSlot = 0;
		if (slot == 0)
			lootSlot = SLOT_BACKPACK;
		else
			lootSlot = slot;

		// get cap
		int cap = Ini.readInteger(looterIni, section, "Cap");

		// minimum amount of cap needed to loot the item
		if (playerCap < cap)
			continue;

		// loot item
		TibiaPacket.sendMoveItemToSlot(id, "Dead", lootSlot);
	}
}

void doStack()
{
	// do not stack while walking
	if (Tibia.isPlayerWalking())
		return;

	std::vector<std::string> sections;
	std::vector<std::string>::iterator sectionsIt;

	// load section names into vector
	sections = Ini.getSections(looterIni);
	if (sections.empty())
		return;

	for(sectionsIt = sections.begin(); sectionsIt != sections.end(); sectionsIt++)
	{
		// get section name
		std::string section = *(sectionsIt);

		// skip options
		if (section == "Options")
			continue;
		
		// check if item will be stacked
		int stack = Ini.readInteger(looterIni, section, "Stack");
		if (stack == 0)
			continue;

		// get item id
		int id = Ini.readInteger(looterIni, section, "ID");
		if (id == 0)
			continue;

		// stack item
		TibiaPacket.sendStackItems(id);
	}
}

void doEat()
{
	// do not eat while walking
	if (Tibia.isPlayerWalking())
		return;

	// cannot eat while full
	if (Tibia.getStatusbarText().find("You are full.") != std::string::npos)
		return;

	std::vector<std::string> sections;
	std::vector<std::string>::iterator sectionsIt;

	// load section names into vector
	sections = Ini.getSections(looterIni);
	if (sections.empty())
		return;

	for(sectionsIt = sections.begin(); sectionsIt != sections.end(); sectionsIt++)
	{
		// get section name
		std::string section = *(sectionsIt);

		// skip options
		if (section == "Options")
			continue;

		// check if item will be eaten
		int eat = Ini.readInteger(looterIni, section, "Eat");
		if (eat == 0)
			continue;

		// get item id
		int id = Ini.readInteger(looterIni, section, "ID");
		if (id == 0)
			continue;

		// eat item
		TibiaPacket.sendUseItem(id, "Dead");
	}
}

/* open */

void openIni()
{
	// open looter ini
	::ShellExecute(0, "open", "notepad.exe", looterIni.c_str(), 0, SW_SHOW);
}

void openHomepage()
{
	// open homepage
	::ShellExecute(0, 0, homepageUrl.c_str(), 0, 0, SW_SHOW);
}

/* show */

void showAbout()
{
	// about text
	std::stringstream buffer;
	buffer << "Looter for Tibia 8.1" << std::endl
		<< "by Evremonde" << std::endl
		<< "Last updated on March 31st, 2008"
		<< std::endl << std::endl;

	// get player name
	if (Tibia.isOnline())
		buffer << Tibia.getPlayerName() << std::endl << std::endl;

	// get number of items in ini file
	std::vector<std::string> sections;
	sections = Ini.getSections(looterIni);
	int numItems = sections.size() - 1; // -1 for Options section
	
	if (numItems >= 0)
		buffer << numItems << " item(s) loaded!" << std::endl << std::endl;

	// homepage
	buffer << "Homepage:" << std::endl << homepageUrl;

	// show about text
	::MessageBox(Tibia.Trainer.getHwnd(), // force to show in game window
		(char*)buffer.str().c_str(),
		"About Looter", MB_ICONINFORMATION);
}

void showSeeId()
{
	// add see id and count to buffer
	std::stringstream buffer;
	buffer << "ID: " << Tibia.getSeeId() << std::endl
		<< "Count: " << Tibia.getSeeCount() << std::endl << std::endl;

	// get see text
	std::string seeText = Tibia.getSeeText();

	// add see text to buffer
	if (seeText.find("You see") != std::string::npos)
		buffer << seeText << std::endl << std::endl;

	// note
	buffer << "This is the ID of the last" << std::endl
		<< "item you saw or inspected!" << std::endl << std::endl
		<< "Hotkey: Pause";

	// show see id message
	::MessageBox(Tibia.Trainer.getHwnd(), // force to show in game window
		(char*)buffer.str().c_str(),
		"Item ID", MB_ICONINFORMATION);
}

/* toggle */

void toggleLoot()
{
	if (bLoot)
	{
		::CheckMenuItem(trayMenu, ID_MENU_LOOT, MF_UNCHECKED);

		bLoot = false;
		Tibia.setStatusbarText("Loot Items: Off");
	}
	else
	{
		::CheckMenuItem(trayMenu, ID_MENU_LOOT, MF_CHECKED);

		bLoot = true;
		Tibia.setStatusbarText("Loot Items: On");
	}
}

void toggleStack()
{
	if (bStack)
	{
		::CheckMenuItem(trayMenu, ID_MENU_STACK, MF_UNCHECKED);

		bStack = false;
		Tibia.setStatusbarText("Stack Items: Off");
	}
	else
	{
		::CheckMenuItem(trayMenu, ID_MENU_STACK, MF_CHECKED);

		bStack = true;
		Tibia.setStatusbarText("Stack Items: On");
	}
}

void toggleEat()
{
	if (bEat)
	{
		::CheckMenuItem(trayMenu, ID_MENU_EAT, MF_UNCHECKED);

		bEat = false;
		Tibia.setStatusbarText("Eat Items: Off");
	}
	else
	{
		::CheckMenuItem(trayMenu, ID_MENU_EAT, MF_CHECKED);

		bEat = true;
		Tibia.setStatusbarText("Eat Items: On");
	}
}

/* tray */

void trayShowMenu(HWND hwnd)
{
	// get menu position
	::GetCursorPos(&trayLocation);

	// required to make menu show and hide correctly
	::SetForegroundWindow(hwnd);

	// show tray menu
	::TrackPopupMenu(::GetSubMenu(trayMenu, 0), TPM_LEFTALIGN | TPM_RIGHTBUTTON, trayLocation.x, trayLocation.y, 0, hwnd, 0);

	// required to make menu show and hide correctly
	::PostMessage(hwnd, WM_NULL, 0, 0);
}

void trayCreate(HWND hwnd)
{
	// initialize tray icon
	trayIcon.cbSize 			= sizeof(NOTIFYICONDATA);
	trayIcon.hWnd 				= hwnd;
	trayIcon.uID 				= ID_TRAY_LOOTER;
	trayIcon.uFlags 			= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	trayIcon.uCallbackMessage	= WM_SHELLNOTIFY;
	trayIcon.hIcon 				= ::LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_LOOTER));

	// set tray tip text
	strcpy(trayIcon.szTip, "Looter - Tibia 8.1");

	// add tray icon
	::Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* timers */

void timerHotkeys(HWND hwnd)
{
	// restrict hotkeys to game and application by window focus
	HWND currentWindow = ::GetForegroundWindow();
	HWND gameWindow = Tibia.Trainer.getHwnd();
	if (currentWindow != gameWindow && currentWindow != hwnd)
		return;

	// see id
	if (::GetAsyncKeyState(VK_PAUSE)) // pause
	{
		showSeeId();
		::Sleep(100); // wait
	}
}

// loot
void timerLoot()
{
	if (!bLoot)
		return;

	// loot items
	doLoot();
}

// stack
void timerStack()
{
	if (!bStack)
		return;

	// stack items
	doStack();
}

// eat
void timerEat()
{
	if (!bEat)
		return;

	// eat items
	doEat();
}

/* window messages */

void onCreate(HWND hwnd)
{
	// assign tray menu
	trayMenu = ::GetMenu(hwnd);

	// create tray icon
	trayCreate(hwnd);

	// initialize timers
	::SetTimer(hwnd, ID_TIMER_HOTKEYS,	10, 0);		// hotkeys very fast
	::SetTimer(hwnd, ID_TIMER_LOOT,		100, 0);	// loot fast
	::SetTimer(hwnd, ID_TIMER_STACK,	1000, 0);	// stack slow
	::SetTimer(hwnd, ID_TIMER_EAT,		100, 0);	// eat fast

	// loaded
	Tibia.setStatusbarText("Looter for Tibia 8.1 loaded!");
}

void onDestroy(HWND hwnd)
{
	// unloaded
	Tibia.setStatusbarText("Looter for Tibia 8.1 unloaded!");

	// remove tray icon
	::Shell_NotifyIcon(NIM_DELETE, &trayIcon);

	// exit
	::PostQuitMessage(0);
}

void onCommand(HWND hwnd, int command, int notifycode, HWND hwnditem)
{
	switch (command) 
	{
		// ini
		case ID_MENU_INI:
			openIni();
			break;

		// loot
		case ID_MENU_LOOT:
			toggleLoot();
			break;

		// stack
		case ID_MENU_STACK:
			toggleStack();
			break;

		// eat
		case ID_MENU_EAT:
			toggleEat();
			break;

		// see id
		case ID_MENU_SEE_ID:
			showSeeId();
			break;

		// about
		case ID_MENU_ABOUT:
			showAbout();
			break;

		// homepage
		case ID_MENU_HOMEPAGE:
			openHomepage();
			break;

		// exit
		case ID_MENU_EXIT:
			::DestroyWindow(hwnd);
			break;
	}
}

void onTimer(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	// tibia was closed
	if (!Tibia.findTibia())
		::DestroyWindow(hwnd); // exit

	// timers
	switch (wparam)
	{
		// hotkeys
		case ID_TIMER_HOTKEYS:
			timerHotkeys(hwnd);
			break;

		// loot
		case ID_TIMER_LOOT:
			timerLoot();
			break;

		// stack
		case ID_TIMER_STACK:
			timerStack();
			break;

		// eat
		case ID_TIMER_EAT:
			timerEat();
			break;
	}
}

void onTray(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	// tray icon
	if (wparam == ID_TRAY_LOOTER)
	{
		// left click or right click
		if (lparam == WM_LBUTTONDOWN || lparam == WM_RBUTTONDOWN)
		{
			// show tray menu
			trayShowMenu(hwnd);
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
			::DestroyWindow(hwnd); // exit
			break;

		case WM_DESTROY:
			onDestroy(hwnd);
			break;

		case WM_COMMAND:
			onCommand(hwnd, LOWORD(wparam), HIWORD(wparam), (HWND)lparam);
			::SetForegroundWindow(Tibia.Trainer.getHwnd()); // set focus to game window after using tray menu
			break;

		case WM_TIMER:
			onTimer(hwnd, wparam, lparam);
			break;

		case WM_SHELLNOTIFY:
			onTray(hwnd, wparam, lparam);
			break;

		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

/* main function */

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR cmdline, int cmdshow) 
{
	// find tibia window
	if (!Tibia.findTibia())
	{
		::MessageBox(0,
			"Tibia window not found!",
			"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	// find looter ini file
	std::ifstream fileIni(looterIni.c_str());
	if (fileIni.fail())
	{
		::MessageBox(0,
			"Looter ini file not found!",
			"Error", MB_ICONERROR | MB_OK);
		return 0;
	}
	fileIni.close();

	// find packet dll
	std::ifstream fileDll("packet.dll");
	if (fileDll.fail())
	{
		::MessageBox(0,
			"Packet dll file not found!",
			"Error", MB_ICONERROR | MB_OK);
		return 0;
	}
	fileDll.close();

	// register window class
	WNDCLASSEX wc;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= windowProc; // window messages handler
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hinstance;
	wc.hIcon			= ::LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_LOOTER));
	wc.hCursor			= ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= static_cast<HBRUSH>(::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE))); // window background color
	wc.lpszMenuName		= MAKEINTRESOURCE(ID_MENU_LOOTER);
	wc.lpszClassName	= "looter";
	wc.hIconSm			= ::LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_LOOTER));

	if(!::RegisterClassEx(&wc))
	{
		::MessageBox(0,
			"Register Window Class failed!",
			"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	// create window
	HWND hwnd;
	hwnd = ::CreateWindowEx(
		WS_EX_DLGMODALFRAME, // window style
		"looter", "Looter - Tibia 8.1", // window title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window options
		CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_WIDTH, WINDOW_HEIGHT, // window dimensions
		0, 0, hinstance, 0);

	if(hwnd == 0)
	{
		::MessageBox(0,
			"Create Window failed!",
			"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	// center window
	HWND desktopWindow = ::GetDesktopWindow();
	RECT desktopRect;
	::GetWindowRect(desktopWindow, &desktopRect);

	RECT hwndRect;
	::GetWindowRect(hwnd, &hwndRect);

	hwndRect.left	= desktopRect.left + ((desktopRect.right - desktopRect.left) - (hwndRect.right - hwndRect.left)) / 2;
	hwndRect.top	= desktopRect.top + ((desktopRect.bottom - desktopRect.top) - (hwndRect.bottom - hwndRect.top)) / 2;

	::SetWindowPos(hwnd, 0, hwndRect.left, hwndRect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

	// show window
	//::ShowWindow(hwnd, cmdshow);
	//::UpdateWindow(hwnd);

	// load options from ini file
	loadOptions();

	// message loop
	MSG msg;
	while(::GetMessage(&msg, 0, 0, 0) > 0)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}
