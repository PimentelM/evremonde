#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <windows.h>
#include <shellapi.h>

#include "ini.h"

#include "looter.h"

#include "tibia.h"
#include "tibiapacket.h"

/*
Looter
by Evremonde
for Tibia 8.0
*/

// tibia class
CTibia Tibia;

// tibia packet class
CTibiaPacket TibiaPacket;

// ini class
CIni Ini;

// looter ini filename
std::string looterIni = "looter80.ini";

// bot
HMENU botMenu;

// tray
NOTIFYICONDATA trayData;
POINT trayLocation;

// booleans
bool bLoot	= true;
bool bStack	= true;
bool bEat	= true;

/* load */

void loadOptions()
{
	int lootItems = Ini.readInteger(looterIni, "Options", "LootItems");
    if (lootItems == 1)
	{
        ::CheckMenuItem(botMenu, mnuLoot, MF_CHECKED);
	    bLoot = true;
	}
	else
	{
        ::CheckMenuItem(botMenu, mnuLoot, MF_UNCHECKED);
	    bLoot = false;
	}
	
	int stackItems = Ini.readInteger(looterIni, "Options", "StackItems");
    if (stackItems == 1)
	{
        ::CheckMenuItem(botMenu, mnuStack, MF_CHECKED);
	    bStack = true;
	}
	else
	{
        ::CheckMenuItem(botMenu, mnuStack, MF_UNCHECKED);
	    bStack = false;
	}

	int eatItems = Ini.readInteger(looterIni, "Options", "EatItems");
    if (eatItems == 1)
	{
        ::CheckMenuItem(botMenu, mnuEat, MF_CHECKED);
	    bEat = true;
	}
	else
	{
        ::CheckMenuItem(botMenu, mnuEat, MF_UNCHECKED);
	    bEat = false;
	}
}

/* do */

void doLoot()
{
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

		// loot item
        TibiaPacket.sendMoveItemToSlot(id, "Dead", lootSlot);
	}
}

void doStack()
{
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

/* show */

void showAbout(HWND hwnd)
{
    // get number of items in ini file
	std::vector<std::string> sections;
	sections = Ini.getSections(looterIni);
	int numItems = sections.size() - 1; // -1 for Options section

	// about text
 	std::stringstream buffer;
	buffer << "Looter for Tibia 8.0" << std::endl
		<< "by Evremonde" << std::endl
		<< "Last updated on November 19th, 2007"
		<< std::endl << std::endl
		<< "Homepage:" << std::endl
		<< "http://code.google.com/p/evremonde/"
		<< std::endl << std::endl
		<< numItems << " item(s) loaded!";

	// show about text
	::MessageBox(hwnd,
		(char*)buffer.str().c_str(),
		"About Looter", MB_ICONINFORMATION);
}

void showSeeId(HWND hwnd)
{
	// get see id
	int seeId = Tibia.getSeeId();
	
	// get see count
	int seeCount = Tibia.getSeeCount();
	
	// add see id text and count to buffer
	std::stringstream buffer;
	buffer << "ID: " << seeId << std::endl
		<< "Count: " << seeCount << std::endl << std::endl;
	
	// get see text
	std::string seeText = Tibia.getSeeText();
	
	// add see text to buffer
	if (seeText.find("You see") != std::string::npos)
	    buffer << seeText << std::endl << std::endl;
	
	// note
	buffer << "This is the ID of the last" << std::endl << "item you saw or inspected!";
	
	// show see id message
	::MessageBox(hwnd,
		(char*)buffer.str().c_str(),
		"Item ID", MB_ICONINFORMATION);
}

/* toggle */

void toggleLoot()
{
	if (bLoot)
	{
		::CheckMenuItem(botMenu, mnuLoot, MF_UNCHECKED);

		bLoot = false;
		Tibia.setStatusbar("Loot Items: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuLoot, MF_CHECKED);

		bLoot = true;
		Tibia.setStatusbar("Loot Items: On");
	}
}

void toggleStack()
{
	if (bStack)
	{
		::CheckMenuItem(botMenu, mnuStack, MF_UNCHECKED);

		bStack = false;
		Tibia.setStatusbar("Stack Items: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuStack, MF_CHECKED);

		bStack = true;
		Tibia.setStatusbar("Stack Items: On");
	}
}

void toggleEat()
{
	if (bEat)
	{
		::CheckMenuItem(botMenu, mnuEat, MF_UNCHECKED);

		bEat = false;
		Tibia.setStatusbar("Eat Items: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuEat, MF_CHECKED);

		bEat = true;
		Tibia.setStatusbar("Eat Items: On");
	}
}

/* tray */

void trayShowMenu(HWND hwnd)
{
    // get menu position
	::GetCursorPos(&trayLocation);

    // required to make menu show and hide correctly
	::SetForegroundWindow(hwnd);

	// show bot menu
	::TrackPopupMenu(::GetSubMenu(botMenu, 0), TPM_LEFTALIGN | TPM_RIGHTBUTTON, trayLocation.x, trayLocation.y, 0, hwnd, 0);

	// required to make menu show and hide correctly
	::PostMessage(hwnd, WM_NULL, 0, 0);
}

void trayCreate(HWND hwnd)
{
	// initialize tray data
    trayData.cbSize 			= sizeof(NOTIFYICONDATA);
	trayData.hWnd 				= hwnd;
	trayData.uID 				= TRAY_LOOTER;
	trayData.uFlags 			= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	trayData.uCallbackMessage	= WM_SHELLNOTIFY;
	trayData.hIcon 				= ::LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(ICON_LOOTER));

	// set tray tip text
	strcpy(trayData.szTip, "Looter - Tibia 8.0");

	// add tray icon
	::Shell_NotifyIcon(NIM_ADD, &trayData);
}

/* timer */

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
    // assign bot menu
    botMenu = ::GetMenu(hwnd);
    
    // create tray icon
    trayCreate(hwnd);
    
    // initialize timers
	::SetTimer(hwnd, TMR_LOOT,	100, 0);	// loot fast
	::SetTimer(hwnd, TMR_STACK,	1000, 0);	// stack slow
	::SetTimer(hwnd, TMR_EAT,	100, 0);	// eat fast

	// loaded
	Tibia.setStatusbar("Looter for Tibia 8.0 loaded!");
}

void onDestroy(HWND hwnd)
{
	// remove tray icon
	::Shell_NotifyIcon(NIM_DELETE, &trayData);
	
	// exit
	::PostQuitMessage(0);
}

void onCommand(HWND hwnd, int command, int notifyCode, HWND hwndItem)
{
	switch (command) 
	{
        // loot
		case mnuLoot:
			toggleLoot();
			break;
			
        // stack
		case mnuStack:
			toggleStack();
			break;
			
		// eat
		case mnuEat:
			toggleEat();
			break;
			
		// see id
		case mnuSeeId:
		    showSeeId(hwnd);
		    break;
		
		// about
		case mnuAbout:
            showAbout(hwnd);
			break;
		
		// exit
		case mnuExit:
			::DestroyWindow(hwnd);
			break;	
	}
}

void onTimer(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	// tibia was closed
	if (!Tibia.findTibia())
		::DestroyWindow(hwnd); // exit
	
	switch (wParam)
	{
		// loot
		case TMR_LOOT:
			timerLoot();
			break;
		
		// stack
		case TMR_STACK:
			timerStack();
			break;
			
		// eat
		case TMR_EAT:
		    timerEat();
		    break;
	}
}

void onTray(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if (wParam == TRAY_LOOTER)
	{
        // left click or right click
		if (lParam == WM_LBUTTONDOWN || lParam == WM_RBUTTONDOWN)
		{
   			// show tray menu
			trayShowMenu(hwnd);
		}
	}
}

/* window messages handler */

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
	{
		case WM_CREATE:
            onCreate(hwnd);
            break;
		
		case WM_CLOSE:
			::DestroyWindow(hwnd);
			break;
			
		case WM_DESTROY:
			onDestroy(hwnd);
			break;
	
		case WM_COMMAND:
			onCommand(hwnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
			::SetForegroundWindow(Tibia.Trainer.hwnd); // set focus to tibia after using menu
			break;
			
        case WM_TIMER:
			onTimer(hwnd, wParam, lParam);
			break;
			
		case WM_SHELLNOTIFY:
			onTray(hwnd, wParam, lParam);
			break;
			
		default:
            return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

/* main function */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	// find tibia window
	if (!Tibia.findTibia())
	{
		::MessageBox(0,
            "Tibia window not found!",
            "Error", MB_ICONERROR | MB_OK);
        return 0; // exit
	}
	
	// find looter ini file
	std::ifstream file(looterIni.c_str());
	if (file.fail())
	{
		::MessageBox(0,
            "Looter ini file not found!",
            "Error", MB_ICONERROR | MB_OK);
        return 0; // exit
	}
	file.close();
	
    // register window class
    WNDCLASSEX wc;
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = 0;
    wc.lpfnWndProc      = windowProc; // window messages handler
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = ::LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(ICON_LOOTER));
    wc.hCursor          = ::LoadCursor(0, IDC_ARROW);
    wc.hbrBackground    = static_cast<HBRUSH>(::CreateSolidBrush(RGB(236, 233, 216))); // window background color
    wc.lpszMenuName     = MAKEINTRESOURCE(MNU_LOOTER);
    wc.lpszClassName    = "looter";
    wc.hIconSm          = ::LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(ICON_LOOTER));
    
    if(!::RegisterClassEx(&wc))
    {
        ::MessageBox(0,
            "Register Window Class failed!",
            "Error", MB_ICONERROR | MB_OK);
        return 0; // exit
    }
    
    // create window
    HWND hwnd;
    hwnd = ::CreateWindowEx(
        WS_EX_DLGMODALFRAME, // icon
        "looter", "Looter - Tibia 8.0", // title
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window options
        CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_WIDTH, WINDOW_HEIGHT, // window dimensions
		0, 0, hInstance, 0);
        
    if(hwnd == 0)
    {
        ::MessageBox(0,
            "Create Window failed!",
            "Error", MB_ICONERROR | MB_OK);
        return 0; // exit
    }
    
    // center window
	//HWND desktopWindow = GetDesktopWindow();
	//RECT desktopRect;
	//GetWindowRect(desktopWindow, &desktopRect);

	//RECT hwndRect;
	//GetWindowRect(hwnd, &hwndRect);

	//hwndRect.left	= desktopRect.left + ((desktopRect.right - desktopRect.left) - (hwndRect.right - hwndRect.left)) / 2;
	//hwndRect.top	= desktopRect.top + ((desktopRect.bottom - desktopRect.top) - (hwndRect.bottom - hwndRect.top)) / 2;

	//SetWindowPos(hwnd, 0, hwndRect.left, hwndRect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    
    // show window
    //::ShowWindow(hwnd, nCmdShow);
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
