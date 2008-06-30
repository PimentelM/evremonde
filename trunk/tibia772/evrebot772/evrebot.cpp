#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <windows.h>

#include "tibia.h"
#include "tibiapacket.h"

/*
Evre Bot
by Evremonde
for client version 7.72
*/

// classes
CTibia Tibia;
CTibiaPacket Packet;

// file
std::string botFile		= "evrebot.xml";

// booleans
bool bLight				= true;
bool bClick				= false;

bool bBattleListAim		= true;

bool bAutoUh			= true;
bool bAutoSd			= false;

bool bLootItems			= true;
bool bStackItems		= true;
bool bEatItems			= true;

// variables
int lightLevel		= LIGHT_TORCH;
int autoUhPercent	= 75;

// functions
void botShootTarget(int itemId)	{ Packet.sendShootTarget(itemId, 0); }
void botShootSelf(int itemId)	{ Packet.sendShootSelf(itemId, 0); }

void botDrinkManaFluid()		{ Packet.sendShootSelf(ITEM_FLUID_VIAL, FLUID_MANA); }
void botReloadAmmo()			{ Packet.sendDragToSlot(Tibia.getPlayerSlotAmmo(), "", SLOT_AMMO); }
void botXRay()					{ Tibia.doXRay(); }

void botLootItem(int itemId)	{ Packet.sendDragToSlot(itemId, "Dead", SLOT_BACKPACK); }
void botStackItem(int itemId)	{ Packet.sendStack(itemId); }
void botEatItem(int itemId)		{ Packet.sendUse(itemId); }

std::string botGetBool(bool boolean)
{
	if (boolean == true)
		return "On";
	else
		return "Off";
}

void botShowStatus()
{
	std::stringstream buffer;

	buffer << "Player Name:\t"		<< Tibia.getPlayerName()		<< std::endl
			<< std::endl
			<< "Light Hack:\t"		<< botGetBool(bLight)			<< std::endl
			<< "Light Level:\t"		<< lightLevel					<< std::endl
			<< "Clicking Hack:\t"	<< botGetBool(bClick)			<< std::endl
			<< "Battle List Aim:\t"	<< botGetBool(bBattleListAim)	<< std::endl
			<< "Auto-UH:\t\t"		<< botGetBool(bAutoUh)			<< std::endl
			<< "Auto-UH Percent:\t"	<< autoUhPercent << "%"			<< std::endl
			<< "Auto-SD:\t\t"		<< botGetBool(bAutoSd)			<< std::endl
			<< "Loot Items:\t"		<< botGetBool(bLootItems)		<< std::endl
			<< "Stack Items:\t"		<< botGetBool(bStackItems)		<< std::endl
			<< "Eat Items:\t"		<< botGetBool(bEatItems)		<< std::endl;

	MessageBox(Tibia.Trainer.hwnd, buffer.str().c_str(), "evrebot", MB_ICONINFORMATION);
}

void botShowExp()
{
	int neededExp = Tibia.calcExpForLevel(Tibia.getPlayerLevel()+1);

	std::stringstream buffer;
	buffer << "Experience needed to level up: " << neededExp;
	Tibia.setStatusbar((char*)buffer.str().c_str());
}

void botShowId()
{
	int seeId = Tibia.getSeeId();

	std::stringstream buffer;
	buffer << "ID: " << seeId;
	Tibia.setStatusbar((char*)buffer.str().c_str());
}

void botToggleLight()
{
	if (lightLevel == LIGHT_NONE)
	{
		lightLevel = LIGHT_TORCH;
		
		bLight = true;
		Tibia.setStatusbar("Light: Torch");
	}
	else if (lightLevel == LIGHT_TORCH)
	{
		lightLevel = LIGHT_FULL;
		
		bLight = true;
		Tibia.setStatusbar("Light: Full");
	}
	else if (lightLevel == LIGHT_FULL)
	{
		lightLevel = LIGHT_NONE;
		
		Tibia.setPlayerLight(LIGHT_NONE);
		
		bLight = false;
		Tibia.setStatusbar("Light: Off");
	}
}

void botToggleClick()
{
	if (bClick)
	{
		Tibia.setUseId(USE_NONE);

		bClick = false;
		Tibia.setStatusbar("Click: Off");
	}
	else
	{
		bClick = true;
		Tibia.setStatusbar("Click: On");
	}
}

void botToggleAutoUh()
{
	if (bAutoUh)
	{
		bAutoUh = false;
		Tibia.setStatusbar("Auto-UH: Off");
	}
	else
	{
		bAutoUh = true;
		Tibia.setStatusbar("Auto-UH: On");
	}
}

void botToggleAutoSd()
{
	if (bAutoSd)
	{
		bAutoSd = false;
		Tibia.setStatusbar("Auto-SD: Off");
	}
	else
	{
		bAutoSd = true;
		Tibia.setStatusbar("Auto-SD: On");
	}
}

// xml functions
void xmlParseOptions()
{
	// parse bot file
	std::ifstream file(botFile.c_str());
    
	// read line by line
    std::string line;
	while (std::getline(file, line))
	{
		// comments
		if (line.find("//") != std::string::npos)
			continue;

		// read token by token
		std::vector<std::string> tokens;
		std::string buffer;
		std::stringstream ss(line);
		while(ss >> buffer)
			tokens.push_back(buffer);

		// 3 variables; 0:definition 1:name 2:value
		if (tokens.size() == 3)
		{
			// get value
			std::stringstream stringToInt(tokens.at(2));
			int value = 0;
			stringToInt >> value;

			if (tokens.at(0) == "option")
			{
				if (tokens.at(1) == "lightlevel")
				{
					// overflow
					if (value > MAX_LIGHT)
						value = MAX_LIGHT;

					// set light level
					lightLevel = (LightLevel_t)value;
				}
				else if (tokens.at(1) == "autouhpercent")
				{
					// overflow
					if (value > 100)
						value = 100;

					// set auto uh percent
					autoUhPercent = value;
				}
			} // if
		} // if
    } // while
    
    file.close();
}

int xmlGetIdOfItem(std::string itemName)
{
	// parse bot file
	std::ifstream file(botFile.c_str());
    
	// read line by line
    std::string line;
	while (std::getline(file, line))
	{
		// comments
		if (line.find("//") != std::string::npos)
			continue;

		// read token by token
		std::vector<std::string> tokens;
		std::string buffer;
		std::stringstream ss(line);
		while(ss >> buffer)
			tokens.push_back(buffer);

		// 3 variables; 0:definition 1:itemId 2:itemName
		if (tokens.size() == 3)
		{
			if (tokens.at(0) == "item")
			{
				if (tokens.at(2) == itemName)
				{
					file.close();

					std::stringstream stringToInt(tokens.at(1));
					int itemId = 0;
					stringToInt >> itemId;

					return itemId;
				} // if
			} // if
		} // if
    } // while
    
    file.close();

	return 0; // not found
}

void xmlParseDefinition(std::string definition)
{
	// parse bot file
	std::ifstream file(botFile.c_str());
    
	// read line by line
    std::string line;
	while (std::getline(file, line))
	{
		// comments
		if (line.find("//") != std::string::npos)
			continue;

		// read token by token
		std::vector<std::string> tokens;
		std::string buffer;
		std::stringstream ss(line);
		while(ss >> buffer)
			tokens.push_back(buffer);

		// 2 variables; 0:definition 1:itemName
		if (tokens.size() == 2)
		{
			if (tokens.at(0) == definition)
			{
				int itemId = xmlGetIdOfItem(tokens.at(1));

				if (definition == "loot")
					botLootItem(itemId);
				else if (definition == "stack")
					botStackItem(itemId);
				else if (definition == "eat")
					botEatItem(itemId);
			} // if
		} // if
    } // while
    
    file.close();
}

// hotkeys
VOID CALLBACK timerHotkeys(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	// ultimate healing OR mana fluid
	if (GetAsyncKeyState(VK_INSERT))
	{
		if (GetAsyncKeyState(VK_SHIFT))
			botDrinkManaFluid();
		else
			botShootSelf(ITEM_RUNE_ULTIMATE_HEALING);
	}

	// sudden death OR heavy magic missile
	if (GetAsyncKeyState(VK_DELETE))
	{
		if (GetAsyncKeyState(VK_SHIFT))
			botShootTarget(ITEM_RUNE_HEAVY_MAGIC_MISSILE);
		else
			botShootTarget(ITEM_RUNE_SUDDEN_DEATH);
	}

	// magic wall OR show bot status
	if (GetAsyncKeyState(VK_HOME))
	{
		if (GetAsyncKeyState(VK_SHIFT))
			botShowStatus();
		else
			botShootTarget(ITEM_RUNE_MAGIC_WALL);
	}

	// explosion OR great fireball
	if (GetAsyncKeyState(VK_END))
	{
		if (GetAsyncKeyState(VK_SHIFT))
			botShootTarget(ITEM_RUNE_GREAT_FIREBALL);
		else
			botShootTarget(ITEM_RUNE_EXPLOSION);
	}

	// light hack OR show exp
	if (GetAsyncKeyState(VK_PRIOR)) // page up
	{
		if (GetAsyncKeyState(VK_SHIFT))
			botShowExp();
		else	
			botToggleLight();
	}

	// click hack or show id
	if (GetAsyncKeyState(VK_NEXT)) // page down
	{
		if (GetAsyncKeyState(VK_SHIFT))
			botShowId();
		else	
			botToggleClick();
	}

	// reload ammo or x-ray
	if (GetAsyncKeyState(VK_PAUSE))
	{
		if (GetAsyncKeyState(VK_SHIFT))
			botXRay();
		else	
			botReloadAmmo();
	}

	// toggle auto uh OR toggle auto sd
	if (GetAsyncKeyState(VK_SCROLL))
	{
		if (GetAsyncKeyState(VK_SHIFT))
			botToggleAutoSd();
		else	
			botToggleAutoUh();
	}
}

// battlelist aim
VOID CALLBACK timerBattleListAim(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (!bBattleListAim)
		return;

	if (GetAsyncKeyState(VK_LBUTTON)) // left-click
		Packet.sendShootTargetBattleList();
}

// light
VOID CALLBACK timerLight(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (!bLight)
		return;

	Tibia.setPlayerLight((LightLevel_t)lightLevel);
}

// click
VOID CALLBACK timerClick(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (!bClick)
		return;

	Tibia.setUseId(USE_USING);
}

// auto uh
VOID CALLBACK timerAutoUh(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (!bAutoUh)
		return;

	if (Tibia.getPlayerHpBar() <= autoUhPercent)
		botShootSelf(ITEM_RUNE_ULTIMATE_HEALING);
}

// auto sd
VOID CALLBACK timerAutoSd(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (!bAutoSd)
		return;

	// dont shoot if you need to auto uh
	if (bAutoUh && Tibia.getPlayerHpBar() <= autoUhPercent)
		return;

	botShootTarget(ITEM_RUNE_SUDDEN_DEATH);
}

// loot items
VOID CALLBACK timerLootItems(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (!bLootItems)
		return;

	xmlParseDefinition("loot");

	/*
	botLootItem(ITEM_MONEY_GOLD);
	botLootItem(ITEM_MONEY_PLATINUM);
	botLootItem(ITEM_MONEY_CRYSTAL);
	*/
}

// stack items
VOID CALLBACK timerStackItems(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (!bStackItems)
		return;

	xmlParseDefinition("stack");

	/*
	botStackItem(ITEM_MONEY_GOLD);
	botStackItem(ITEM_MONEY_PLATINUM);
	botStackItem(ITEM_MONEY_CRYSTAL);
	*/
}

// eat items
VOID CALLBACK timerEatItems(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (!bEatItems)
		return;

	xmlParseDefinition("eat");

	//botEatItem(ITEM_);
}

int main()
{
	// introduction
	std::cout << "evrebot by Evremonde" << std::endl << "packet.dll by Azura" << std::endl << "for Tibia version 7.72" << std::endl << std::endl;
	
	// hotkey information
	std::cout << "Hotkeys:" << std::endl
			<< "INSERT\t\tShoot Ultimate Healing at self"			<< std::endl
			<< "SHIFT+INSERT\tDrink Manafluid"						<< std::endl
			<< "DELETE\t\tShoot Sudden Death at target"				<< std::endl
			<< "SHIFT+DELETE\tShoot Heavy Magic Missle at target"	<< std::endl
			<< "HOME\t\tShoot Magic Wall in front of target"		<< std::endl
			<< "SHIFT+HOME\tShow Bot status"						<< std::endl
			<< "END\t\tShoot Explosion at target"					<< std::endl
			<< "SHIFT+END\tShoot Great Fireball at target"			<< std::endl
			<< "PAGEUP\t\tToggle Light hack"						<< std::endl
			<< "SHIFT+PAGEUP\tShow experience needed to level up"	<< std::endl
			<< "PAGEDN\t\tToggle Clicking hack"						<< std::endl
			<< "SHIFT+PAGEDN\tShow ID of last seen tile or object"	<< std::endl
			<< "PAUSE\t\tReload ammo slot from backpack"			<< std::endl
			<< "SHIFT+PAUSE\tApply X-Ray to last seen tile"			<< std::endl
			<< "SCRL\t\tToggle Auto-UH"								<< std::endl
			<< "SHIFT+SCRL\tToggle Auto-SD"							<< std::endl;

	// find tibia window
	if (!Tibia.findTibia())
		return ::MessageBox(0, "Tibia not found!", "evrebot", MB_ICONERROR);

	// name of player using bot
	if (Tibia.isOnline())
		std::cout << std::endl << "Bot loaded for player: " << Tibia.getPlayerName() << std::endl;
	
	// loaded
	Tibia.setStatusbar("evrebot loaded!");

	// parse bot file options
	xmlParseOptions();

	// start timers
	UINT_PTR Timer;
	Timer = ::SetTimer(NULL, 0,	100,	&timerHotkeys);
	
	Timer = ::SetTimer(NULL, 0,	100,	&timerBattleListAim);
	
	Timer = ::SetTimer(NULL, 0,	500,	&timerLight);
	Timer = ::SetTimer(NULL, 0,	100,	&timerClick);
	
	Timer = ::SetTimer(NULL, 0,	100,	&timerAutoUh);
	Timer = ::SetTimer(NULL, 0,	1000,	&timerAutoSd);

	Timer = ::SetTimer(NULL, 0,	100,	&timerLootItems);
	Timer = ::SetTimer(NULL, 0,	1000,	&timerStackItems);
	Timer = ::SetTimer(NULL, 0,	100,	&timerEatItems);

	// handle timers
	MSG Msg;
	while (::GetMessage(&Msg, NULL, 0, 0))
	{	
		::TranslateMessage(&Msg);
		::DispatchMessage(&Msg);
	}

	return 0;
}
