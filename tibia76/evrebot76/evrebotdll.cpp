#include <cstdlib>		// random number generator
#include <ctime>		// random number generator

#include <deque>
#include <map>
#include <sstream>
#include <fstream>

#include <winsock2.h>
#include <windows.h>

#include "madCHook.h"	// madCodeHook

#include "tibia.h"
#include "tibiapacket.h"

/*
Evre Bot DLL
by Evremonde
for Tibia version 7.6
*/

CTibia Tibia;
CTibiaPacket Packet;

SOCKET mySocket; // socket used to send packets

char *botFile		= "evrebot.xml";

bool bReady			= false;		// initialize bot

bool bHotkeys		= true;			// hotkeys

bool bLight			= true;			// light hack
bool bClick			= false;		// click hack

bool bRevealNames	= true;			// reveal names

bool bDance			= false;		// dance

bool bRandomOutfit	= false;		// random outfit
bool bRainbowOutfit	= false;		// rainbow outfit
int iRainbowOutfit	= 0;
std::deque<int> deqRainbowOutfit;

bool bAlwaysShoot	= false;		// always shoot
bool bKeepTarget	= false;		// keep target
int keepTargetId	= 0;

bool bAutoUh		= true;			// auto-uh
int autoUhPercent	= 75;
bool bAutoHeal		= true;			// auto-heal
int autoHealPercent	= 50;

bool bAutoLoot		= true;			// auto-loot
bool bAutoStack		= true;			// auto-stack
bool bAutoEat		= false;		// auto-eat

bool bAutoSpell		= false;		// auto-spell
std::map<char*, int> mapAutoSpell;
std::map<char*, int>::iterator mapAutoSpellIt;

bool bAutoSpam		= false;		// auto-spam
char *spam			= "a@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";

// custom runes
int runeUh			= ITEM_RUNE_ULTIMATE_HEALING;
int runeSd			= ITEM_RUNE_SUDDEN_DEATH;
int runeCustom		= ITEM_RUNE_EXPLOSION;

// bot functions
char *botGetBool(bool boolean)
{
	if (boolean == true)
		return "On";
	else
		return "Off";
}
void botSelfUh()			{ Packet.sendShootSelf(mySocket, runeUh, 0); }
void botSelfMf()			{ Packet.sendShootSelf(mySocket, ITEM_FLUID_VIAL, FLUID_MANA); }
void botShootSd()			{ Packet.sendShootTarget(mySocket, runeSd, 0); }
void botShootCustom()		{ Packet.sendShootTarget(mySocket, runeCustom, 0); }
void botShootMwall()		{ Packet.sendShootTarget(mySocket, ITEM_RUNE_MAGIC_WALL, 0); }
void botLoot(int itemId)	{ Packet.sendDragToSlot(mySocket, itemId, "Dead", SLOT_BACKPACK); }
void botStack(int itemId)	{ Packet.sendStack(mySocket, itemId); }
void botEat(int itemId)		{ Packet.sendUse(mySocket, itemId); }
void botReloadAmmo()		{ Packet.sendDragToSlot(mySocket, Tibia.getPlayerSlotAmmo(), "", SLOT_AMMO); }
void botRandomOutfit()		{ Packet.sendSetOutfitColors(mySocket, (OutfitColor_t)(rand()%OUTFIT_COLOR_MAX), (OutfitColor_t)(rand()%OUTFIT_COLOR_MAX), (OutfitColor_t)(rand()%OUTFIT_COLOR_MAX), (OutfitColor_t)(rand()%OUTFIT_COLOR_MAX)); }
void botKeepTarget()		{ Packet.sendAttack(mySocket, keepTargetId, ATTACK_PLAYER); }
void botSpam()				{ Packet.sendSay(mySocket, SPEAK_SAY, spam); }
void botSetOutfit(int outfitId)
{
	Packet.sendSetOutfit(mySocket, (Outfit_t)outfitId);
}
void botDance()
{
	int direction = rand() % 4;
	Packet.sendTurn(mySocket, (Direction_t)direction);
}
void botShowExp()
{
	int neededExp = Tibia.computeExpForLevel(Tibia.getPlayerLevel()+1);

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
void botSetUh()
{
	runeUh = Tibia.getSeeId();
	
	std::stringstream buffer;
	buffer << "UH rune set to ID: " << runeUh;
	Tibia.setStatusbar((char*)buffer.str().c_str());
}
void botSetSd()
{
	runeSd = Tibia.getSeeId();

	std::stringstream buffer;
	buffer << "SD rune set to ID: " << runeSd;
	Tibia.setStatusbar((char*)buffer.str().c_str());
}
void botSetDefault()
{
	runeUh = ITEM_RUNE_ULTIMATE_HEALING;
	runeSd = ITEM_RUNE_SUDDEN_DEATH;

	Tibia.setStatusbar("UH and SD runes set to default IDs");
}
void botSetCustom(int itemId)
{
	runeCustom = itemId;

	std::stringstream buffer;
	buffer << "Custom rune set to ID: " << runeCustom;
	Tibia.setStatusbar((char*)buffer.str().c_str());
}
void botToggleHotkeys()
{
	if (bHotkeys)
	{
		bHotkeys = false;
		Tibia.setStatusbar("Hotkeys: Off");
	}
	else
	{
		bHotkeys = true;
		Tibia.setStatusbar("Hotkeys: On");
	}
}
void botToggleKeepTarget()
{
	if (bKeepTarget)
	{
		keepTargetId = 0;

		bKeepTarget = false;
		Tibia.setStatusbar("Keep Target: Off");
	}
	else
	{
		keepTargetId = Tibia.getTargetId();

		bKeepTarget = true;
		std::stringstream buffer;
		buffer << "Keep Target: On (Creature ID: " << keepTargetId << ")";
		Tibia.setStatusbar((char*)buffer.str().c_str());
	}
}
void botToggleLight()
{
	if (bLight)
	{
		Tibia.setPermaLight(LIGHT_PERMA_NORMAL);
		Tibia.setPlayerLight(LIGHT_NONE);
		
		bLight = false;
		Tibia.setStatusbar("Light: Off");
	}
	else
	{
		Tibia.setPermaLight(LIGHT_PERMA_FULL);
		Tibia.setPlayerLight(LIGHT_FULL);
		
		bLight = true;
		Tibia.setStatusbar("Light: On");
	}
}
void botToggleRevealNames()
{
	if (bRevealNames)
	{
		Tibia.setRevealNames(false);

		bRevealNames = false;
		Tibia.setStatusbar("Reveal Names: Off");
	}
	else
	{
		Tibia.setRevealNames(true);

		bRevealNames = true;
		Tibia.setStatusbar("Reveal Names: On");
	}
}
void botToggleDance()
{
	if (bDance)
	{
		bDance = false;
		Tibia.setStatusbar("Dance: Off");
	}
	else
	{
		bDance = true;
		Tibia.setStatusbar("Dance: On");
	}
}
void botToggleRandomOutfit()
{
	if (bRandomOutfit)
	{
		bRandomOutfit = false;
		Tibia.setStatusbar("Random Outfit: Off");
	}
	else
	{
		bRandomOutfit = true;
		Tibia.setStatusbar("Random Outfit: On");
	}
}
void botToggleRainbowOutfit()
{
	if (bRainbowOutfit)
	{
		// clear the rainbow queue
		deqRainbowOutfit.clear();

		bRainbowOutfit = false;
		Tibia.setStatusbar("Rainbow Outfit: Off");
	}
	else
	{
		// reset the rainbow color iterator
		iRainbowOutfit = 0;
		
		// initialize the rainbow queue
		deqRainbowOutfit.push_back(OUTFIT_COLOR_RED);
		deqRainbowOutfit.push_back(OUTFIT_COLOR_WHITE);
		deqRainbowOutfit.push_back(OUTFIT_COLOR_WHITE);
		deqRainbowOutfit.push_back(OUTFIT_COLOR_WHITE);

		bRainbowOutfit = true;
		Tibia.setStatusbar("Rainbow Outfit: On");
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
void botToggleAutoHeal()
{
	if (bAutoHeal)
	{
		bAutoHeal = false;
		Tibia.setStatusbar("Auto-Heal: Off");
	}
	else
	{
		bAutoHeal = true;
		Tibia.setStatusbar("Auto-Heal: On");
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
void botToggleAutoSpell()
{
	if (bAutoSpell)
	{
		bAutoSpell = false;
		Tibia.setStatusbar("Auto-Spell: Off");
	}
	else
	{
		bAutoSpell = true;
		Tibia.setStatusbar("Auto-Spell: On");
	}
}
void botToggleAutoLoot()
{
	if (bAutoLoot)
	{
		bAutoLoot = false;
		Tibia.setStatusbar("Auto-Loot: Off");
	}
	else
	{
		bAutoLoot = true;
		Tibia.setStatusbar("Auto-Loot: On");
	}
}
void botToggleAutoStack()
{
	if (bAutoStack)
	{
		bAutoStack = false;
		Tibia.setStatusbar("Auto-Stack: Off");
	}
	else
	{
		bAutoStack = true;
		Tibia.setStatusbar("Auto-Stack: On");
	}
}
void botToggleAutoEat()
{
	if (bAutoEat)
	{
		bAutoEat = false;
		Tibia.setStatusbar("Auto-Eat: Off");
	}
	else
	{
		bAutoEat = true;
		Tibia.setStatusbar("Auto-Eat: On");
	}
}
void botToggleAlwaysShoot()
{
	if (bAlwaysShoot)
	{
		bAlwaysShoot = false;
		Tibia.setStatusbar("Always Shoot: Off");
	}
	else
	{
		bAlwaysShoot = true;
		Tibia.setStatusbar("Always Shoot: On");
	}
}
void botToggleAutoSpam()
{
	if (bAutoSpam)
	{
		bAutoSpam = false;
		Tibia.setStatusbar("Auto-Spam: Off");
	}
	else
	{
		bAutoSpam = true;
		Tibia.setStatusbar("Auto-Spam: On");
	}
}
void botStatus()
{
	std::stringstream buffer;
	buffer << "Status:" << std::endl << std::endl;

	buffer << "Hotkeys:		" << botGetBool(bHotkeys) << std::endl;
	buffer << "Light Hack:	" << botGetBool(bLight) << std::endl;
	buffer << "Click Hack:	" << botGetBool(bClick) << std::endl;
	buffer << "Reveal Names:	" << botGetBool(bRevealNames) << std::endl;
	buffer << "Dance:		" << botGetBool(bDance) << std::endl;
	buffer << "Random Outfit:	" << botGetBool(bRandomOutfit) << std::endl;
	buffer << "Rainbow Outfit:	" << botGetBool(bRainbowOutfit) << std::endl;
	buffer << "Auto-Spam:	" << botGetBool(bAutoSpam) << std::endl;
	buffer << "Auto-Spell:	" << botGetBool(bAutoSpell) << std::endl;
	buffer << "Auto-Loot:	" << botGetBool(bAutoLoot) << std::endl;
	buffer << "Auto-Stack:	" << botGetBool(bAutoStack) << std::endl;
	buffer << "Auto-Eat:		" << botGetBool(bAutoEat) << std::endl;
	buffer << "Auto-UH:		" << botGetBool(bAutoUh) << std::endl;
	buffer << "Auto-UH Percent:	" << autoUhPercent << std::endl;
	buffer << "Auto-Heal:	" << botGetBool(bAutoHeal) << std::endl;
	buffer << "Auto-Heal Percent:	" << autoHealPercent << std::endl;
	buffer << "Always Shoot:	" << botGetBool(bAlwaysShoot) << std::endl;
	buffer << "Keep Target:	" << botGetBool(bKeepTarget) << std::endl;
	
	MessageBox(NULL, buffer.str().c_str(), "evrebot", NULL);
}

// xml functions
int xmlGetNameById(char *definition, char *name)
{
	// parse bot file
	std::fstream file(botFile, std::ios::in);
	
	while (file.good())
	{
		char buffer[255];
		file.getline(buffer, 255);
		
		if (strstr(buffer, definition))
		{
			char *token;

			// command
			token = strtok(buffer, ":");
			if (token == NULL)
				continue;
			
			// id
			token = strtok(NULL, ":");
			if (token == NULL)
				continue;
			int id = atoi(token);

			// name
			token = strtok(NULL, "");
			if (token == NULL)
				continue;
			char *name_ = token;
			
			if (strcmp(name_, name) == 0)
				return id;
		} // if
	} // while

	file.close();

	// name not found
	return 0;
}

// fastest timer
VOID CALLBACK timerFastest(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	// -> Hotkeys

	if (bHotkeys)
	{
		// ultimate healing self
		if (GetAsyncKeyState(VK_DELETE))
			botSelfUh();

		// light hack on/off
		if (GetAsyncKeyState(VK_END))
			botToggleLight();

		// mana fluid self
		if (GetAsyncKeyState(VK_NEXT)) // page down
			botSelfMf();

		// sudden death target
		if (GetAsyncKeyState(VK_INSERT))
			botShootSd();
		
		// custom shoot target
		if (GetAsyncKeyState(VK_HOME))
			botShootCustom();

		// magic wall target
		if (GetAsyncKeyState(VK_PRIOR)) // page up
			botShootMwall();

		// reload ammo
		if (GetAsyncKeyState(VK_PAUSE))
			botReloadAmmo();
	} // if

	// -> auto-uh, click hack, random outfit

	// auto-uh
	if (bAutoUh && Tibia.getPlayerHpBar() <= autoUhPercent)
		botSelfUh();

	// click hack
	if (bClick)
		Tibia.setUseId(USE_USING);

	// random outfit
	if (bRandomOutfit)
		botRandomOutfit();
}

// fast timer
VOID CALLBACK timerFast(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	// keep target
	if (bKeepTarget)
		botKeepTarget();

	// dance
	if (bDance)
		botDance();

	// auto-spam
	if (bAutoSpam)
		botSpam();

	// auto-loot
	if (bAutoLoot)
	{
		// parse bot file
		std::fstream file(botFile, std::ios::in);

		while (file.good())
		{
			char buffer[255];
			file.getline(buffer, 255);
			
			if (strstr(buffer, "loot:"))
			{
				char *token;

				// command
				token = strtok(buffer, ":");
				if (token == NULL)
					continue;

				// item name
				token = strtok(NULL, "");
				if (token == NULL)
					continue;
				char *itemName = token;

				// item id
				int itemId = xmlGetNameById("item:", itemName);
				if (itemId <= 0)
					continue;
				
				// loot item
				botLoot(itemId);
			} // if
		} // while

		file.close();
	} // if

	// auto-stack
	if (bAutoStack)
	{
		// parse bot file
		std::fstream file(botFile, std::ios::in);

		while (file.good())
		{
			char buffer[255];
			file.getline(buffer, 255);
			
			if (strstr(buffer, "stack:"))
			{
				char *token;

				// command
				token = strtok(buffer, ":");
				if (token == NULL)
					continue;

				// item name
				token = strtok(NULL, "");
				if (token == NULL)
					continue;
				char *itemName = token;

				// item id
				int itemId = xmlGetNameById("item:", itemName);
				if (itemId <= 0)
					continue;
				
				// stack item
				botStack(itemId);
			} // if
		} // while

		file.close();
	} // if

	// rainbow outfit
	if (bRainbowOutfit && deqRainbowOutfit.size() == 4)
	{
		// go to begin
		if (iRainbowOutfit > 3)
			iRainbowOutfit = 0;

		// set outfit
		Packet.sendSetOutfitColors(mySocket,
			(OutfitColor_t)deqRainbowOutfit.at(0),
			(OutfitColor_t)deqRainbowOutfit.at(1),
			(OutfitColor_t)deqRainbowOutfit.at(2),
			(OutfitColor_t)deqRainbowOutfit.at(3));

		// increment queue
		deqRainbowOutfit.push_front(deqRainbowOutfit.at(3));
		deqRainbowOutfit.pop_back();

		// go to next
		iRainbowOutfit++;

		// color change
		if (iRainbowOutfit == 4)
		{
			int color = deqRainbowOutfit.at(0);

			// rainbow
			switch (color)
			{
				case OUTFIT_COLOR_RED:
					color = OUTFIT_COLOR_ORANGE;
					break;
				case OUTFIT_COLOR_ORANGE:
					color = OUTFIT_COLOR_YELLOW;
					break;
				case OUTFIT_COLOR_YELLOW:
					color = OUTFIT_COLOR_GREEN;
					break;
				case OUTFIT_COLOR_GREEN:
					color = OUTFIT_COLOR_BLUE;
					break;
				case OUTFIT_COLOR_BLUE:
					color = OUTFIT_COLOR_PURPLE;
					break;
				case OUTFIT_COLOR_PURPLE:
					color = OUTFIT_COLOR_RED;
					break;
			} // switch

			deqRainbowOutfit.at(0) = color;
		} // if
	} // if
}

// normal timer
VOID CALLBACK timerNormal(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	// light hack
	if (bLight)
	{
		Tibia.setPermaLight(LIGHT_PERMA_FULL);
		Tibia.setPlayerLight(LIGHT_FULL);
	}
}

// slow timer
VOID CALLBACK timerSlow(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	// always shoot
	if (bAlwaysShoot)
	{
		// only shoot if you dont need to auto-uh
		if (bAutoUh && Tibia.getPlayerHpBar() > autoUhPercent)
			botShootCustom();
	}

	// auto-eat
	if (bAutoEat)
	{
		// parse bot file
		std::fstream file(botFile, std::ios::in);

		while (file.good())
		{
			char buffer[255];
			file.getline(buffer, 255);
			
			if (strstr(buffer, "eat:"))
			{
				char *token;

				// command
				token = strtok(buffer, ":");
				if (token == NULL)
					continue;

				// item name
				token = strtok(NULL, "");
				if (token == NULL)
					continue;
				char *itemName = token;

				// item id
				int itemId = xmlGetNameById("item:", itemName);
				if (itemId <= 0)
					continue;
				
				// eat item
				botEat(itemId);
			} // if
		} // while

		file.close();
	} // if

	// auto-spell
	if (bAutoSpell && !mapAutoSpell.empty())
	{
		// go to begin
		if (mapAutoSpellIt == mapAutoSpell.end())
			mapAutoSpellIt = mapAutoSpell.begin();

		// if you have the required mana, say the spell name
		if (Tibia.getPlayerMp() >= mapAutoSpellIt->second)
		{
			// get conditions
			int playerIcons = Tibia.getPlayerIcons();

			// only cast haste if needed
			if ( strcmp(mapAutoSpellIt->first, "utani hur") == 0 || strcmp(mapAutoSpellIt->first, "utani gran hur") == 0 )
			{	
				// already has haste
				if (playerIcons == (playerIcons | ICON_HASTE))
				{
					// go to next
					if (mapAutoSpellIt != mapAutoSpell.end())
						mapAutoSpellIt++;
					return;
				}
			}
			// only cast mana shield if needed
			if ( strcmp(mapAutoSpellIt->first, "utamo vita") == 0 )
			{
				// already has mana shield
				if (playerIcons == (playerIcons | ICON_MANA_SHIELD))
				{
					// go to next
					if (mapAutoSpellIt != mapAutoSpell.end())
						mapAutoSpellIt++;
					return;
				}
			}
			// only cast antidote if needed
			if ( strcmp(mapAutoSpellIt->first, "exana pox") == 0 )
			{
				// not poisoned
				if (playerIcons != (playerIcons | ICON_POISON))
				{
					// go to next
					if (mapAutoSpellIt != mapAutoSpell.end())
						mapAutoSpellIt++;
					return;
				}
			}

			// auto-heal
			if ( strcmp(mapAutoSpellIt->first, "exura") == 0 || strcmp(mapAutoSpellIt->first, "exura gran") == 0 || strcmp(mapAutoSpellIt->first, "exura vita") == 0 )
			{	
				// only cast heal if needed
				if (bAutoHeal && Tibia.getPlayerHpBar() > autoHealPercent)
				{
					// go to next
					if (mapAutoSpellIt != mapAutoSpell.end())
						mapAutoSpellIt++;
					return;
				}
			}

			// say the spell name
			Packet.sendSay(mySocket, SPEAK_SAY, mapAutoSpellIt->first);
		} // if

		// go to next
		if (mapAutoSpellIt != mapAutoSpell.end())
			mapAutoSpellIt++;
	} // if
}

// check say
bool checkSay(char *buf, int len)
{
	char *command = Packet.getSay(buf, len);

	// status
	if (strcmp(command, "/status") == 0)
	{
		botStatus();
		return true;
	}

	// hotkeys on/off
	else if (strcmp(command, "/hotkeys") == 0 || strcmp(command, "/h") == 0)
	{
		botToggleHotkeys();
		return true;
	}

	// light hack on/off
	else if (strcmp(command, "/light") == 0 || strcmp(command, "/l") == 0)
	{
		botToggleLight();
		return true;
	}

	// click hack on/off
	else if (strcmp(command, "/use") == 0 || strcmp(command, "/u") == 0 || strcmp(command, "/click") == 0 || strcmp(command, "/c") == 0)
	{
		botToggleClick();
		return true;
	}

	// reveal names on/off
	else if (strcmp(command, "/reveal") == 0 || strcmp(command, "/names") == 0)
	{
		botToggleRevealNames();
		return true;
	}

	// dance on/off
	else if (strcmp(command, "/dance") == 0 || strcmp(command, "/d") == 0)
	{
		botToggleDance();
		return true;
	}

	// show experience needed to level up
	else if (strcmp(command, "/experience") == 0 || strcmp(command, "/exp") == 0 || strcmp(command, "/e") == 0)
	{
		botShowExp();
		return true;
	}

	// reload ammo
	else if (strcmp(command, "/ammo") == 0)
	{
		botReloadAmmo();
		Tibia.setStatusbar("Reload ammo");
		return true;
	}

	// mana fluid self
	else if (strcmp(command, "/manafluid") == 0 || strcmp(command, "/mf") == 0)
	{
		botSelfMf();
		Tibia.setStatusbar("Drink: Mana fluid");
		return true;
	}

	// custom shoot target
	else if (strcmp(command, "/custom") == 0)
	{
		botShootCustom();
		Tibia.setStatusbar("Shoot: Custom rune");
		return true;
	}

	// always shoot on/off
	else if (strcmp(command, "/shoot") == 0 || strcmp(command, "/s") == 0)
	{
		botToggleAlwaysShoot();
		return true;
	}

	// keep target on/off
	else if (strcmp(command, "/target") == 0 || strcmp(command, "/t") == 0)
	{
		botToggleKeepTarget();
		return true;
	}

	// auto-spam on/off
	else if (strcmp(command, "/spam") == 0)
	{
		botToggleAutoSpam();
		return true;
	}

	// auto-loot on/off
	else if (strcmp(command, "/loot") == 0)
	{
		botToggleAutoLoot();
		return true;
	}

	// auto-stack on/off
	else if (strcmp(command, "/stack") == 0)
	{
		botToggleAutoStack();
		return true;
	}

	// auto-eat on/off
	else if (strcmp(command, "/eat") == 0)
	{
		botToggleAutoEat();
		return true;
	}

	// auto-uh on/off
	else if (strcmp(command, "/uh") == 0)
	{
		botToggleAutoUh();
		return true;
	}
	// auto-uh percent
	else if ( strstr(command, "/uh") && strlen(command) > strlen("/uh") )
	{
		char *token;

		// command
		token = strtok(command, " ");
		if (token == NULL)
			return true;

		// auto-uh percent
		token = strtok(NULL, "");
		if (token == NULL)
			return true;
		int autoUhPercent = atoi(token);

		std::stringstream buffer;
		buffer << "Auto-UH: Heal rune at " << autoUhPercent << "% health";
		Tibia.setStatusbar((char*)buffer.str().c_str());
		return true;
	}

	// auto-heal on/off
	else if (strcmp(command, "/heal") == 0)
	{
		botToggleAutoHeal();
		return true;
	}
	// auto-heal percent
	else if ( strstr(command, "/heal") && strlen(command) > strlen("/heal") )
	{
		char *token;

		// command
		token = strtok(command, " ");
		if (token == NULL)
			return true;

		// auto-heal percent
		token = strtok(NULL, "");
		if (token == NULL)
			return true;
		int autoHealPercent = atoi(token);

		std::stringstream buffer;
		buffer << "Auto-Heal: Heal spell at " << autoHealPercent << "% health";
		Tibia.setStatusbar((char*)buffer.str().c_str());
		return true;
	}

	// auto-spell on/off
	else if (strcmp(command, "/spell") == 0)
	{
		botToggleAutoSpell();
		return true;
	}
	// auto-spell show
	else if (strcmp(command, "/spell show") == 0)
	{
		std::stringstream buffer;
		buffer << "Spell list: ";

		if (mapAutoSpell.empty())
			buffer << "Empty";
		else
		{
			std::map<char*, int>::iterator it;
			int numSpells = 0;
			for (it = mapAutoSpell.begin(); it != mapAutoSpell.end(); it++)
			{
				buffer << it->first;

				numSpells++;
				if (mapAutoSpell.size() > 1 && numSpells != mapAutoSpell.size())
					buffer << ", ";
			}
		}

		Tibia.setStatusbar((char*)buffer.str().c_str());
		return true;
	}
	// auto-spell clear
	else if (strcmp(command, "/spell clear") == 0)
	{
		mapAutoSpell.clear();
		Tibia.setStatusbar("Auto-Spell: Spells list cleared");
		return true;
	}
	// auto-spell remove
	else if ( strstr(command, "/spell remove") && strlen(command) > strlen("/spell remove") )
	{
		char *token;

		// command
		token = strtok(command, " ");
		if (token == NULL)
			return true;

		// remove
		token = strtok(NULL, " ");
		if (token == NULL)
			return true;

		// spell name
		token = strtok(NULL, "");
		if (token == NULL)
			return true;
		char *autoSpellName = token;
		
		// remove spell from spells list
		std::map<char*, int>::iterator it;
		for (it = mapAutoSpell.begin(); it != mapAutoSpell.end(); it++)
		{
			if ( strstr(it->first, autoSpellName) )
			{
				mapAutoSpell.erase(it->first);
				
				std::stringstream buffer;
				buffer << "Auto-Spell: Spell '" << autoSpellName << "' removed from spells list";
				Tibia.setStatusbar((char*)buffer.str().c_str());
				break;
			}
		}

		return true;
	} // else if
	// auto-spell add
	else if ( strstr(command, "/spell") && strlen(command) > strlen("/spell") )
	{
		char *token;

		// command
		token = strtok(command, " ");
		if (token == NULL)
			return true;

		// mana cost
		token = strtok(NULL, " ");
		if (token == NULL)
			return true;
		int autoSpellManaCost = atoi(token);

		// spell name
		token = strtok(NULL, "");
		if (token == NULL)
			return true;
		char *autoSpellName = token;

		// update mana cost if spell already exists in spells list
		std::map<char*, int>::iterator it;
		for (it = mapAutoSpell.begin(); it != mapAutoSpell.end(); it++)
		{
			if ( strstr(it->first, autoSpellName) )
			{
				it->second = autoSpellManaCost;
				
				std::stringstream buffer;
				buffer << "Auto-Spell: Spell '" << autoSpellName << "' updated to '" << autoSpellManaCost << "' mana";
				Tibia.setStatusbar((char*)buffer.str().c_str());
				return true;
			}
		}

		// add spell to spells list
		mapAutoSpell.insert(std::pair<char*, int>(autoSpellName, autoSpellManaCost));
	
		std::stringstream buffer;
		buffer << "Auto-Spell: Spell '" << autoSpellName << "' added to spells list";
		Tibia.setStatusbar((char*)buffer.str().c_str());
		return true;
	} // else if

	// outfit
	else if ( strstr(command, "/outfit") && strlen(command) > strlen("/outfit") )
	{
		char *token;

		// command
		token = strtok(command, " ");
		if (token == NULL)
			return true;

		// outfit name
		token = strtok(NULL, "");
		if (token == NULL)
			return true;
		char *outfitName = token;

		// outfit id
		int outfitId = xmlGetNameById("outfit:", outfitName);
		if (outfitId == 0)
			outfitId = Tibia.getPlayerOutfit();

		// set outfit
		Tibia.setPlayerOutfit((Outfit_t)outfitId);
		if (Tibia.getPlayerSex() != SEX_CREATURE)
			botSetOutfit(outfitId);

		std::stringstream buffer;
		buffer << "Outfit: " << outfitName << " (" << outfitId << ")";
		Tibia.setStatusbar((char*)buffer.str().c_str());
		return true;
	}

	// random outfit
	else if (strcmp(command, "/random") == 0 || strcmp(command, "/rand") == 0 || strcmp(command, "/r") == 0)
	{
		botToggleRandomOutfit();
		return true;
	}
	// rainbow outfit
	else if (strcmp(command, "/rainbow") == 0 || strcmp(command, "/rbow") == 0|| strcmp(command, "/r2") == 0)
	{
		botToggleRainbowOutfit();
		return true;
	}

	// set uh rune
	else if (strcmp(command, "/set uh") == 0)
	{
		botSetUh();
		return true;
	}
	// set sd rune
	else if (strcmp(command, "/set sd") == 0)
	{
		botSetSd();
		return true;
	}
	// set default uh and sd runes
	else if (strcmp(command, "/set default") == 0)
	{
		botSetDefault();
		return true;
	}

	// set custom rune
	else if (strcmp(command, "/set custom") == 0)
	{
		botSetCustom(Tibia.getSeeId());
		return true;
	}

	// show id
	else if (strcmp(command, "/id") == 0 || strcmp(command, "/i") == 0)
	{
		botShowId();
		return true;
	}

	// set custom rune
	else if (strcmp(command, "/magicwall") == 0 || strcmp(command, "/mwall") == 0 || strcmp(command, "/mw") == 0)
	{
		runeCustom = ITEM_RUNE_MAGIC_WALL;
		Tibia.setStatusbar("Custom rune set to: Magic Wall");
		return true;
	}
	else if (strcmp(command, "/sd") == 0)
	{
		runeCustom = ITEM_RUNE_SUDDEN_DEATH;
		Tibia.setStatusbar("Custom rune set to: Sudden Death");
		return true;
	}
	else if (strcmp(command, "/explosion") == 0 || strcmp(command, "/explo") == 0 || strcmp(command, "/ex") == 0)
	{
		runeCustom = ITEM_RUNE_EXPLOSION;
		Tibia.setStatusbar("Custom rune set to: Explosion");
		return true;
	}
	else if (strcmp(command, "/lmm") == 0)
	{
		runeCustom = ITEM_RUNE_LIGHT_MAGIC_MISSILE;
		Tibia.setStatusbar("Custom rune set to: Light Magic Missile");
		return true;
	}
	else if (strcmp(command, "/hmm") == 0)
	{
		runeCustom = ITEM_RUNE_HEAVY_MAGIC_MISSILE;
		Tibia.setStatusbar("Custom rune set to: Heavy Magic Missile");
		return true;
	}
	else if (strcmp(command, "/gfb") == 0)
	{
		runeCustom = ITEM_RUNE_GREAT_FIREBALL;
		Tibia.setStatusbar("Custom rune set to: Great Fireball");
		return true;
	}
	else if (strcmp(command, "/fbomb") == 0 || strcmp(command, "/fb") == 0)
	{
		runeCustom = ITEM_RUNE_FIRE_BOMB;
		Tibia.setStatusbar("Custom rune set to: Fire Bomb");
		return true;
	}
	else if (strcmp(command, "/ebomb") == 0 || strcmp(command, "/eb") == 0)
	{
		runeCustom = ITEM_RUNE_ENERGY_BOMB;
		Tibia.setStatusbar("Custom rune set to: Energy Bomb");
		return true;
	}
	else if (strcmp(command, "/pbomb") == 0 || strcmp(command, "/pb") == 0)
	{
		runeCustom = ITEM_RUNE_POISON_BOMB;
		Tibia.setStatusbar("Custom rune set to: Poison Bomb");
		return true;
	}
	else if (strcmp(command, "/fwall") == 0 || strcmp(command, "/fw") == 0)
	{
		runeCustom = ITEM_RUNE_FIRE_WALL;
		Tibia.setStatusbar("Custom rune set to: Fire Wall");
		return true;
	}
	else if (strcmp(command, "/ewall") == 0 || strcmp(command, "/ew") == 0)
	{
		runeCustom = ITEM_RUNE_ENERGY_WALL;
		Tibia.setStatusbar("Custom rune set to: Energy Wall");
		return true;
	}
	else if (strcmp(command, "/pwall") == 0 || strcmp(command, "/pw") == 0)
	{
		runeCustom = ITEM_RUNE_POISON_WALL;
		Tibia.setStatusbar("Custom rune set to: Poison Wall");
		return true;
	}
	else if (strcmp(command, "/soulfire") == 0 || strcmp(command, "/sfire") == 0 || strcmp(command, "/sf") == 0)
	{
		runeCustom = ITEM_RUNE_SOUL_FIRE;
		Tibia.setStatusbar("Custom rune set to: Soul Fire");
		return true;
	}
	else if (strcmp(command, "/envenom") == 0)
	{
		runeCustom = ITEM_RUNE_ENVENOM;
		Tibia.setStatusbar("Custom rune set to: Envenom");
		return true;
	}

	// dont say commands out loud
	if (command[0] == '/')
		return true;

	// command not found
	return false;
}

// hook send function
int WINAPI sendHook(SOCKET s, char *buf, int len, int flags)
{
	// assign socket used to send packets
	mySocket = s;

	// initialize bot
	if (!bReady)
	{
		srand ( time(NULL) );					// initialize random number generator
		mapAutoSpellIt = mapAutoSpell.begin();	// initialize auto-spell
		Tibia.setRevealNames(true);				// initialize reveal names
		
		UINT timer;
		timer = SetTimer(NULL, 0, 100, &timerFastest);	// start fastest timer
		timer = SetTimer(NULL, 0, 500, &timerFast);		// start fast timer
		timer = SetTimer(NULL, 0, 1000, &timerNormal);	// start normal timer
		timer = SetTimer(NULL, 0, 1500, &timerSlow);	// start slow timer

		bReady = true;
		Tibia.setStatusbar("Evre Bot ready.");
	}

	// check say
	if (checkSay(buf, len))
		return 0; // dont say command out loud

	// call original send function
	return Packet.sendNext(s, buf, len, flags);
}

// hook recv function
int WINAPI recvHook(SOCKET s, char *buf, int len, int flags)
{
	// call original recv function
	return Packet.recvNext(s, buf, len, flags);
}

// main function
BOOL WINAPI DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		// hook winsock functions
		HookAPI("ws2_32.dll", "send", sendHook, (PVOID*) &Packet.sendNext);
		HookAPI("ws2_32.dll", "recv", recvHook, (PVOID*) &Packet.recvNext);

		Tibia.setStatusbar("Evre Bot loaded...");
	}
	
    return TRUE;
}