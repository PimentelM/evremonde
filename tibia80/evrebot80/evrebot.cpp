#include <iostream>
#include <sstream>
#include <string>

#include <windows.h>
#include <shellapi.h>

#include "evrebot.h"

#include "tibia.h"
#include "tibiapacket.h"

/*
Evre Bot
by Evremonde
for Tibia 8.0
*/

// tibia class
CTibia Tibia;

// tibia packet class
CTibiaPacket Packet;

// bot
HINSTANCE botInstance;
HMENU botMenu;

// tray
NOTIFYICONDATA trayData;

// fast drag
POINT fastDragLocation;

// hotkeys buffer
std::stringstream bufferHotkeys;

// booleans
bool bHotkeys		= true;
bool bAntiAFK		= true;
bool bNameSpy		= true;
bool bLootMoney     = true;
bool bStackMoney    = true;
bool bFishingWater	= false;
bool bTreesToBushes	= false;
bool bLight			= true;
bool bClick			= false;
bool bMouseClicking	= false;
bool bFastDrag		= false;
bool bOutfitAddons	= false;

// variables
int lightRadius		= LIGHT_TORCH;
int outfitAddon		= OUTFIT_ADDON_NONE;

/* toggle */

void toggleHotkeys()
{
	if (bHotkeys)
	{
		::CheckMenuItem(botMenu, mnuHotkeys, MF_UNCHECKED);
		
		bHotkeys = false;
		Tibia.setStatusbar("Hotkeys: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuHotkeys, MF_CHECKED);
		
		bHotkeys = true;
		Tibia.setStatusbar("Hotkeys: On");
	}
}

void toggleAntiAFK()
{
	if (bAntiAFK)
	{
		::CheckMenuItem(botMenu, mnuAntiAFK, MF_UNCHECKED);

		bAntiAFK = false;
		Tibia.setStatusbar("Anti-AFK: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuAntiAFK, MF_CHECKED);

		bAntiAFK = true;
		Tibia.setStatusbar("Anti-AFK: On");
	}
}

void toggleNameSpy()
{
	if (bNameSpy)
	{
		Tibia.setNameSpy(false);

		::CheckMenuItem(botMenu, mnuNameSpy, MF_UNCHECKED);

		bNameSpy = false;
		Tibia.setStatusbar("Name Spy: Off");
	}
	else
	{
		Tibia.setNameSpy(true);

		::CheckMenuItem(botMenu, mnuNameSpy, MF_CHECKED);

		bNameSpy = true;
		Tibia.setStatusbar("Name Spy: On");
	}
}

void toggleLootMoney()
{
	if (bLootMoney)
	{
		::CheckMenuItem(botMenu, mnuLootMoney, MF_UNCHECKED);

		bLootMoney = false;
		Tibia.setStatusbar("Loot Money: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuLootMoney, MF_CHECKED);

		bLootMoney = true;
		Tibia.setStatusbar("Loot Money: On");
	}
}

void toggleStackMoney()
{
	if (bStackMoney)
	{
		::CheckMenuItem(botMenu, mnuStackMoney, MF_UNCHECKED);

		bStackMoney = false;
		Tibia.setStatusbar("Stack Money: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuStackMoney, MF_CHECKED);

		bStackMoney = true;
		Tibia.setStatusbar("Stack Money: On");
	}
}

void toggleFishingWater()
{
	if (bFishingWater)
	{
		::CheckMenuItem(botMenu, mnuFishingWater, MF_UNCHECKED);

		bFishingWater = false;
		Tibia.setStatusbar("Show Water w/ Fish: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuFishingWater, MF_CHECKED);

		bFishingWater = true;
		Tibia.setStatusbar("Show Water w/ Fish: On");
	}
}

void toggleTreesToBushes()
{
	if (bTreesToBushes)
	{
		::CheckMenuItem(botMenu, mnuTreesToBushes, MF_UNCHECKED);

		bTreesToBushes = false;
		Tibia.setStatusbar("Change Trees to Bushes: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuTreesToBushes, MF_CHECKED);

		bTreesToBushes = true;
		Tibia.setStatusbar("Change Trees to Bushes: On");
	}
}

void toggleLight()
{
	switch (lightRadius)
	{
		case LIGHT_NONE:
			lightRadius = LIGHT_TORCH;

			::CheckMenuItem(botMenu, mnuLightNone, MF_UNCHECKED);
			::CheckMenuRadioItem(botMenu, mnuLightTorch, mnuLightFull, mnuLightTorch, MF_BYCOMMAND);

			bLight = true;
			Tibia.setStatusbar("Light: Torch");
			break;
		case LIGHT_TORCH:
			lightRadius = LIGHT_FULL;

			::CheckMenuItem(botMenu, mnuLightNone, MF_UNCHECKED);
			::CheckMenuRadioItem(botMenu, mnuLightTorch, mnuLightFull, mnuLightFull, MF_BYCOMMAND);

			bLight = true;
			Tibia.setStatusbar("Light: Full");
			break;
		case LIGHT_FULL:
			lightRadius = LIGHT_NONE;

			Tibia.setPlayerLight(LIGHT_NONE);
			
			::CheckMenuItem(botMenu, mnuLightNone, MF_CHECKED);
			::CheckMenuItem(botMenu, mnuLightTorch, MF_UNCHECKED);
			::CheckMenuItem(botMenu, mnuLightFull, MF_UNCHECKED);

			bLight = false;
			Tibia.setStatusbar("Light: Off");
			break;
	}
}

void toggleClick()
{
	if (bClick)
	{
		Tibia.setMouseId(MOUSE_NONE);

		::CheckMenuItem(botMenu, mnuClick, MF_UNCHECKED);

		bClick = false;
		Tibia.setStatusbar("Click: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuClick, MF_CHECKED);

		bClick = true;
		Tibia.setStatusbar("Click: On");
	}
}

void toggleFastDrag()
{
	if (bFastDrag)
	{
		::CheckMenuItem(botMenu, mnuFastDrag, MF_UNCHECKED);

		// reset fast drag location
		fastDragLocation.x = 0;
		fastDragLocation.y = 0;

		bFastDrag = false;
		Tibia.setStatusbar("Fast Drag: Off");
	}
	else
	{
		::CheckMenuItem(botMenu, mnuFastDrag, MF_CHECKED);

		bFastDrag = true;
		Tibia.setStatusbar("Fast Drag: On");
	}
}

void toggleMouseClicking()
{
	if (bMouseClicking)
	{
		bMouseClicking = false;
		Tibia.setStatusbar("Mouse Clicking: Off");
	}
	else
	{
		bMouseClicking = true;
		Tibia.setStatusbar("Mouse Clicking: On");
	}
}

void toggleOutfitAddons()
{
	switch (outfitAddon)
	{
		case OUTFIT_ADDON_NONE:
			outfitAddon = OUTFIT_ADDON_1;

			::CheckMenuItem(botMenu, mnuOutfitAddonNone, MF_UNCHECKED);
			::CheckMenuRadioItem(botMenu, mnuOutfitAddon1, mnuOutfitAddonBoth, mnuOutfitAddon1, MF_BYCOMMAND);

			bOutfitAddons = true;
			Tibia.setStatusbar("Outfit Addons: #1");
			break;
		case OUTFIT_ADDON_1:
			outfitAddon = OUTFIT_ADDON_2;

			::CheckMenuItem(botMenu, mnuOutfitAddonNone, MF_UNCHECKED);
			::CheckMenuRadioItem(botMenu, mnuOutfitAddon1, mnuOutfitAddonBoth, mnuOutfitAddon2, MF_BYCOMMAND);

			bOutfitAddons = true;
			Tibia.setStatusbar("Outfit Addons: #2");
			break;
		case OUTFIT_ADDON_2:
			outfitAddon = OUTFIT_ADDON_BOTH;

			::CheckMenuItem(botMenu, mnuOutfitAddonNone, MF_UNCHECKED);
			::CheckMenuRadioItem(botMenu, mnuOutfitAddon1, mnuOutfitAddonBoth, mnuOutfitAddonBoth, MF_BYCOMMAND);

			bOutfitAddons = true;
			Tibia.setStatusbar("Outfit Addons: Both");
			break;
		case OUTFIT_ADDON_BOTH:
			outfitAddon = OUTFIT_ADDON_NONE;

			Tibia.setPlayerOutfitAddon(OUTFIT_ADDON_NONE);

			::CheckMenuItem(botMenu, mnuOutfitAddonNone, MF_CHECKED);
			::CheckMenuItem(botMenu, mnuOutfitAddon1, MF_UNCHECKED);
			::CheckMenuItem(botMenu, mnuOutfitAddon2, MF_UNCHECKED);
			::CheckMenuItem(botMenu, mnuOutfitAddonBoth, MF_UNCHECKED);

			bOutfitAddons = false;
			Tibia.setStatusbar("Outfit Addons: None");
			break;
	}
}

/* show */

void showExp()
{
	int neededExp = Tibia.getExpToLevelUp();
	int levelPercent = Tibia.getPlayerLevelPercent();

	std::stringstream buffer;
	buffer << "Experience needed to level up: " << neededExp << " (" << levelPercent << "%)";
	Tibia.setStatusbar(buffer.str());
}

void showId()
{
	int seeId = Tibia.getSeeId();
	int seeCount = Tibia.Trainer.readBytes(SEE_COUNT, 3); // 3 bytes for creature id

	std::stringstream buffer;
	buffer << "ID of last seen tile or item: " << seeId;

	if (seeCount > 0)
	{
		buffer << " (";

		if (seeId == OBJECT_CREATURE)
		    buffer << "Unique Creature ID: ";
		else
		    buffer << "Count: ";

		buffer << seeCount;
		buffer << ")";
	}

	Tibia.setStatusbar(buffer.str());
}

void showCreaturesByZ(HWND hwnd)
{
    std::stringstream buffer;
    
    buffer << "Z:\t" << "HP%:\t" << "Name:" << std::endl;

    // search through battle list
    int bufferCount = 0;
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		// creature must be visible
		bool visible = Tibia.Trainer.readBytes(i + OFFSET_CREATURE_IS_VISIBLE, 1);
		if (!visible)
			continue;

		// get creature z
		int z = Tibia.Trainer.readBytes(i + OFFSET_CREATURE_Z, 1);
		
		// get player z
		int playerZ = Tibia.getPlayerZ();
		
		// calculate z-axis difference
		if (z > playerZ)
		{
			buffer << "-";
			z = z - playerZ;
		}
		else if (z < playerZ)
		{
            buffer << "+";
            z = playerZ - z;
		}
		else
		{
			continue; // creature must be above or below
		}

		// get creature health
		int hp = Tibia.Trainer.readBytes(i + OFFSET_CREATURE_HP_BAR, 1);
		
		// creature must be alive
		if (hp <= 0)
		    continue;

		// get creature name
        std::string name = Tibia.Trainer.readString(i);
        
        // add to buffer
		buffer << z << "\t" << hp << "\t" << name << std::endl;
		bufferCount++;
	}
	
	if (bufferCount == 0)
	{
		Tibia.setStatusbar("No creature information available.");
		return;
	}
	
	::SetForegroundWindow(hwnd);
	
	// show buffer
	::MessageBox(hwnd,
	    buffer.str().c_str(),
        "View Creatures", MB_ICONINFORMATION);
        
    ::SetForegroundWindow(Tibia.Trainer.hwnd);
}

/* do */

void doLevelSpyReset()
{
	Tibia.doLevelSpyReset();
	Tibia.setStatusbar("Level Spy: Reset");
}

void doLevelSpyUp()
{
	Tibia.doLevelSpyUp();
	Tibia.setStatusbar("Level Spy: Up");
}

void doLevelSpyDown()
{
	Tibia.doLevelSpyDown();
	Tibia.setStatusbar("Level Spy: Down");
}

void doFastDrag()
{
	// fast drag location must be set
	if (fastDragLocation.x <= 0 || fastDragLocation.y <= 0)
	{
		Tibia.setStatusbar("Please set Fast Drag location first!");
		return;
	}

	// get original mouse position
	POINT oldLocation;
	::GetCursorPos(&oldLocation);

	// drag to set location
	::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	::SetCursorPos(fastDragLocation.x, fastDragLocation.y);
	::Sleep(100);
	::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	// return to original mouse position
	::SetCursorPos(oldLocation.x, oldLocation.y);
	::Sleep(100);

	// press enter for dragging a stack or group
	Tibia.doKeyEx(VK_RETURN);
}

/* set */

void setHotkeyBuffer()
{
    bufferHotkeys
		<< "INSERT\t\tToggle light radius"						<< std::endl
		<< "DELETE\t\tToggle outfit addon"						<< std::endl
		<< "HOME\t\tShow EXP needed to level up"				<< std::endl
		<< "SHIFT+HOME\tShow ID of last seen tile or item"		<< std::endl
		<< "END\t\tLevel spy reset"								<< std::endl
		<< "PAGEUP\t\tLevel spy up"								<< std::endl
		<< "PAGEDN\t\tLevel spy down"							<< std::endl
		<< "PAUSE\t\tToggle click (fishing, roping, runes)"		<< std::endl
		<< "SHIFT+PAUSE\tToggle automatic mouse clicking"		<< std::endl
		<< "MOUSE3\t\tDo fast drag to the set location"			<< std::endl
		<< "SHIFT+MOUSE3\tSet fast drag location"				<< std::endl;
}

void setFastDrag()
{
	::GetCursorPos(&fastDragLocation);

	std::stringstream buffer;
	buffer << "Fast Drag location set! (X: " << fastDragLocation.x << ", Y: " << fastDragLocation.y << ")";
	Tibia.setStatusbar(buffer.str());
}

/* tray */

void trayShowMenu(HWND hwnd)
{
    // get menu position
    POINT trayLocation;
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
	trayData.uID 				= TRAY_EVREBOT;
	trayData.uFlags 			= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	trayData.uCallbackMessage	= WM_SHELLNOTIFY;
	trayData.hIcon 				= ::LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(ICON_EVREBOT));

	// set tray tip text
	strcpy(trayData.szTip, "evrebot");

	// add tray icon
	::Shell_NotifyIcon(NIM_ADD, &trayData);
}

/* timer */

// trainer process
void timerTrainerProcess(HWND hwnd)
{
	
	// tibia was closed
	if (!Tibia.findTibia())
		::DestroyWindow(hwnd); // exit
}

// hotkeys
void timerHotkeys(HWND hwnd)
{
	if (!bHotkeys)
		return;

	// restrict hotkeys to tibia and evrebot by window focus
	HWND currentWindow = ::GetForegroundWindow();
	if (currentWindow != Tibia.Trainer.hwnd && currentWindow != hwnd)
		return;

	// toggle light
	if (::GetAsyncKeyState(VK_INSERT))
		toggleLight();

	// toggle outfit addon
	if (::GetAsyncKeyState(VK_DELETE))
	    toggleOutfitAddons();

	// show exp or show id
	if (::GetAsyncKeyState(VK_HOME))
	{
		if (::GetAsyncKeyState(VK_SHIFT) & 0x8000)
			showId();
		else
			showExp();
	}

	// level spy reset
	if (::GetAsyncKeyState(VK_END))
		doLevelSpyReset();

	// level spy up
	if (::GetAsyncKeyState(VK_PRIOR)) // page up
		doLevelSpyUp();

	// level spy down
	if (::GetAsyncKeyState(VK_NEXT)) // page down
		doLevelSpyDown();
		
	// toggle click or toggle mouse clicking
	if (::GetAsyncKeyState(VK_PAUSE))
	{
		if (::GetAsyncKeyState(VK_SHIFT) & 0x8000)
			toggleMouseClicking();
		else
			toggleClick();
	}

	// do fast drag OR set fast drag
	if (::GetAsyncKeyState(VK_MBUTTON))	// middle-click
	{
		if (::GetAsyncKeyState(VK_SHIFT) & 0x8000)
			setFastDrag();
		else
		{
			if (bFastDrag)
				doFastDrag();
		}
	}
}

// anti-afk
void timerAntiAFK()
{
	if (!bAntiAFK)
		return;

	Tibia.doAntiAFK();
}

// loot money
void timerLootMoney()
{
	if (!bLootMoney)
		return;
		
    Packet.sendDragToSlot(ITEM_MONEY_GOLD,		"Dead", SLOT_BACKPACK);
    Packet.sendDragToSlot(ITEM_MONEY_PLATINUM,	"Dead", SLOT_BACKPACK);
    Packet.sendDragToSlot(ITEM_MONEY_CRYSTAL,	"Dead", SLOT_BACKPACK);
    Packet.sendDragToSlot(ITEM_MONEY_SCARAB,	"Dead", SLOT_BACKPACK);
}

// stack money
void timerStackMoney()
{
	if (!bStackMoney)
		return;

	Packet.sendStack(ITEM_MONEY_GOLD);
	Packet.sendStack(ITEM_MONEY_PLATINUM);
	Packet.sendStack(ITEM_MONEY_CRYSTAL);
	Packet.sendStack(ITEM_MONEY_SCARAB);
}

// fishing water
void timerFishingWater()
{
	if (!bFishingWater)
		return;

	Tibia.doShowFishingWater();
}

// trees to bushes
void timerTreesToBushes()
{
	if (!bTreesToBushes)
		return;

	Tibia.doTreesToBushes();
}

// light hack
void timerLight()
{
	if (!bLight)
		return;

	Tibia.setPlayerLight(static_cast<LightRadius_t>(lightRadius));
}

// click hack
void timerClick()
{
	if (!bClick)
		return;

	Tibia.setMouseId(MOUSE_USING);
}

// mouse clicking hack
void timerMouseClicking()
{
	if (!bMouseClicking)
		return;
		
    // restrict mouse clicking to tibia by window focus
	HWND currentWindow = ::GetForegroundWindow();
	if (currentWindow != Tibia.Trainer.hwnd)
		return;

	// left-click
	::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

// outfit addons
void timerOutfitAddons()
{
	if (!bOutfitAddons)
		return;

	Tibia.setPlayerOutfitAddon(static_cast<OutfitAddon_t>(outfitAddon));
}

/* window messages */

void onCreate(HWND hwnd)
{
    // assign bot menu
    botMenu = ::GetMenu(hwnd);
    
    // set bot menu default radio checkmarks
	::CheckMenuRadioItem(botMenu, mnuLightTorch, mnuLightFull, mnuLightTorch, MF_BYCOMMAND);

	// set hotkey buffer
    setHotkeyBuffer();
    
    // create tray icon
    trayCreate(hwnd);
    
    // initialize timers
	::SetTimer(hwnd, TMR_TRAINERPROCESS,	1000, 0);
    ::SetTimer(hwnd, TMR_HOTKEYS,			100, 0);
	::SetTimer(hwnd, TMR_ANTIAFK,			300000, 0); // 5 minutes
	::SetTimer(hwnd, TMR_LOOTMONEY,         100, 0);    // loot fast
	::SetTimer(hwnd, TMR_STACKMONEY,        1000, 0);   // stack slow
	::SetTimer(hwnd, TMR_FISHINGWATER,		1000, 0);
	::SetTimer(hwnd, TMR_TREESTOBUSHES,		1000, 0);
	::SetTimer(hwnd, TMR_LIGHT,				500, 0);
	::SetTimer(hwnd, TMR_CLICK,				100, 0);
	::SetTimer(hwnd, TMR_MOUSECLICKING,		100, 0);
	::SetTimer(hwnd, TMR_OUTFITADDONS,		100, 0);
	
	// do name spy
	Tibia.setNameSpy(true);

	// loaded
	Tibia.setStatusbar("evrebot loaded!");
}

void onDestroy(HWND hwnd)
{
    // undo name spy
	Tibia.setNameSpy(false);

	// remove tray icon
	::Shell_NotifyIcon(NIM_DELETE, &trayData);
	
	// exit
	::PostQuitMessage(0);
}

void onCommand(HWND hwnd, int command, int notifyCode, HWND hwndItem)
{
	switch (command) 
	{
		// hotkeys
		case mnuHotkeys:
			toggleHotkeys();
			break;
			
		// view hotkeys
		case mnuViewHotkeys:
			::MessageBox(hwnd,
				bufferHotkeys.str().c_str(),
				"Hotkeys", MB_ICONINFORMATION);
            break;
            
        // view creatures
		case mnuViewCreatures:
			showCreaturesByZ(hwnd);
			break;
            
        // anti-afk
		case mnuAntiAFK:
			toggleAntiAFK();
			break;
			
		// name spy
		case mnuNameSpy:
			toggleNameSpy();
			break;
			
        // loot money
		case mnuLootMoney:
			toggleLootMoney();
			break;
			
        // stack money
		case mnuStackMoney:
			toggleStackMoney();
			break;
			
        // fishing water
		case mnuFishingWater:
			toggleFishingWater();
			break;
			
        // trees to bushes
		case mnuTreesToBushes:
			toggleTreesToBushes();
			break;
			
        // light
		case mnuLightNone:
			lightRadius = LIGHT_FULL;
			toggleLight();
			break;

		case mnuLightTorch:
			lightRadius = LIGHT_NONE;
			toggleLight();
			break;

		case mnuLightFull:
			lightRadius = LIGHT_TORCH;
			toggleLight();
			break;

		// click
		case mnuClick:
			toggleClick();
			break;

		// fast drag
		case mnuFastDrag:
			toggleFastDrag();
			break;

		// outfit male
		case mnuOutfitMaleDruid:
			Tibia.setPlayerOutfit(OUTFIT_MALE_DRUID);
			break;

		case mnuOutfitMalePaladin:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PALADIN);
			break;

		case mnuOutfitMaleSorcerer:
			Tibia.setPlayerOutfit(OUTFIT_MALE_SORCERER);
			break;

		case mnuOutfitMaleKnight:
			Tibia.setPlayerOutfit(OUTFIT_MALE_KNIGHT);
			break;

		// outfit female
		case mnuOutfitFemaleDruid:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_DRUID);
			break;

		case mnuOutfitFemalePaladin:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PALADIN);
			break;

		case mnuOutfitFemaleSorcerer:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_SORCERER);
			break;

		case mnuOutfitFemaleKnight:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_KNIGHT);
			break;

		// outfit male premium
		case mnuOutfitMalePremium1:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_1);
			break;

		case mnuOutfitMalePremium2:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_2);
			break;

		case mnuOutfitMalePremium3:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_3);
			break;

		case mnuOutfitMalePremium4:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_4);
			break;

		case mnuOutfitMalePremium5:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_5);
			break;

		case mnuOutfitMalePremium6:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_6);
			break;

		case mnuOutfitMalePremium7:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_7);
			break;

		case mnuOutfitMalePremium8:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_8);
			break;

		case mnuOutfitMalePremium9:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_9);
			break;

		case mnuOutfitMalePremium10:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_10);
			break;

		case mnuOutfitMalePremium11:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_11);
			break;
			
		case mnuOutfitMalePremium12:
			Tibia.setPlayerOutfit(OUTFIT_MALE_PREMIUM_12);
			break;

		// outfit female premium
		case mnuOutfitFemalePremium1:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_1);
			break;

		case mnuOutfitFemalePremium2:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_2);
			break;

		case mnuOutfitFemalePremium3:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_3);
			break;

		case mnuOutfitFemalePremium4:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_4);
			break;

		case mnuOutfitFemalePremium5:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_5);
			break;

		case mnuOutfitFemalePremium6:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_6);
			break;

		case mnuOutfitFemalePremium7:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_7);
			break;

		case mnuOutfitFemalePremium8:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_8);
			break;

		case mnuOutfitFemalePremium9:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_9);
			break;

		case mnuOutfitFemalePremium10:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_10);
			break;

		case mnuOutfitFemalePremium11:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_11);
			break;
			
		case mnuOutfitFemalePremium12:
			Tibia.setPlayerOutfit(OUTFIT_FEMALE_PREMIUM_12);
			break;

		// outfit other
		case mnuOutfitOtherHero:
			Tibia.setPlayerOutfit(OUTFIT_OTHER_HERO);
			break;
		
		case mnuOutfitOtherGamemaster:
			Tibia.setPlayerOutfit(OUTFIT_OTHER_GAMEMASTER);
			break;

		case mnuOutfitOtherElf:
			Tibia.setPlayerOutfit(OUTFIT_OTHER_ELF);
			break;

		case mnuOutfitOtherDwarf:
			Tibia.setPlayerOutfit(OUTFIT_OTHER_DWARF);
			break;
			
		case mnuOutfitOtherCult:
			Tibia.setPlayerOutfit(OUTFIT_OTHER_CULT);
			break;
			
		case mnuOutfitOtherPanda:
			Tibia.setPlayerOutfit(OUTFIT_OTHER_PANDA);
			break;

		case mnuOutfitOtherFerumbras:
			Tibia.setPlayerOutfit(OUTFIT_OTHER_FERUMBRAS);
			break;

		// outfit addons
		case mnuOutfitAddonNone:
			outfitAddon = OUTFIT_ADDON_BOTH;
			toggleOutfitAddons();
			break;

		case mnuOutfitAddon1:
			outfitAddon = OUTFIT_ADDON_NONE;
			toggleOutfitAddons();
			break;

		case mnuOutfitAddon2:
			outfitAddon = OUTFIT_ADDON_1;
			toggleOutfitAddons();
			break;

		case mnuOutfitAddonBoth:
			outfitAddon = OUTFIT_ADDON_2;
			toggleOutfitAddons();
			break;
		
		// about
		case mnuAbout:
			::MessageBox(hwnd,
                "evrebot\nby Evremonde\nfor Tibia 8.0\n\nOpen source bot for the massive\nmultiplayer online roleplaying game Tibia\n\nLast updated on July 4th, 2007\n\nHomepage:\nhttp://sourceforge.net/projects/evrebot/",
                "About evrebot", MB_ICONINFORMATION);
			break;
		
		// exit
		case mnuExit:
			::DestroyWindow(hwnd);
			break;	
	}
}

void onTimer(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		// trainer process
		case TMR_TRAINERPROCESS:
			timerTrainerProcess(hwnd);
			break;

        // hotkeys
		case TMR_HOTKEYS:
			timerHotkeys(hwnd);
			break;

        // anti-afk
		case TMR_ANTIAFK:
			timerAntiAFK();
			break;
			
		// loot money
		case TMR_LOOTMONEY:
			timerLootMoney();
			break;
		
		// stack money
		case TMR_STACKMONEY:
			timerStackMoney();
			break;
			
        // fishing water
		case TMR_FISHINGWATER:
			timerFishingWater();
			break;

        // trees to bushes
		case TMR_TREESTOBUSHES:
			timerTreesToBushes();
			break;

        // light
		case TMR_LIGHT:
			timerLight();
			break;

        // click
		case TMR_CLICK:
			timerClick();
			break;

        // mouse clicking
		case TMR_MOUSECLICKING:
			timerMouseClicking();
			break;

        // outfit addons
		case TMR_OUTFITADDONS:
			timerOutfitAddons();
			break;
	}
}

void onTray(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if (wParam == TRAY_EVREBOT)
	{
        // left or right click
		if ((lParam == WM_LBUTTONDOWN) || (lParam == WM_RBUTTONDOWN))
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
			::SetForegroundWindow(Tibia.Trainer.hwnd);
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
        return 0;
	}
	
	// assign global bot instance
	botInstance = hInstance;
	
    // register window class
    WNDCLASSEX wc;
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = 0;
    wc.lpfnWndProc      = windowProc; // window messages handler
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = ::LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(ICON_EVREBOT));
    wc.hCursor          = ::LoadCursor(0, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName     = MAKEINTRESOURCE(MNU_EVREBOT);
    wc.lpszClassName    = "evrebot";
    wc.hIconSm          = ::LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(ICON_EVREBOT));
    
    if(!::RegisterClassEx(&wc))
    {
        ::MessageBox(0,
            "register window class Failed!",
            "Error", MB_ICONERROR | MB_OK);
        return 0;
    }
    
    // create window
    HWND hwnd;
    hwnd = ::CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "evrebot", "evrebot",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 320, 240,
        0, 0, hInstance, 0);
        
    if(hwnd == 0)
    {
        ::MessageBox(0,
            "create window Failed!",
            "Error", MB_ICONERROR | MB_OK);
        return 0;
    }
    
    // show window
    //::ShowWindow(hwnd, nCmdShow);
    //::UpdateWindow(hwnd);
    
    // message loop
    MSG msg;
    while(::GetMessage(&msg, 0, 0, 0) > 0)
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return msg.wParam;
}
