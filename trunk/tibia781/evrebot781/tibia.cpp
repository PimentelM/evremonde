#include "tibia.h"

/*
Tibia Trainer
by Evremonde
*/

/* constructor and deconstructor */

CTibia::CTibia()
{
	// find tibia window
	HWND tibiaWindow = ::FindWindow("tibiaclient", 0);

	// set trainer process to tibia window
	Trainer.setProcess(tibiaWindow);
	
	// set tibia to low cpu usage
	::SetPriorityClass(Trainer.processHandle, Trainer.PRIORITY_BELOW_NORMAL);
}

CTibia::~CTibia()
{
	// close process
	Trainer.closeProcess();
}

/* other */

bool CTibia::findTibia() { return ::FindWindow("tibiaclient", 0); }

/* calculations */

int CTibia::calcExpForLevel(int level)
{
	return static_cast<int>(((50*level*level*level)/3-100*level*level+(850*level)/3-200) + 1);
}

/* booleans */

bool CTibia::isInGame() { return Trainer.readBytes(IS_INGAME, 1); }

bool CTibia::isPlayerWalking() { return getPlayerBattleList(OFFSET_CREATURE_IS_WALKING, 1); }

bool CTibia::isTargetVisible()
{
	return getTargetBattleList(OFFSET_CREATURE_IS_VISIBLE, 1);
}

/* do */

void CTibia::doKey(int virtualKey)
{
	::SendMessage(Trainer.hwnd, WM_KEYDOWN, virtualKey, 0);
	::SendMessage(Trainer.hwnd, WM_KEYUP, virtualKey, 0);
}

void CTibia::doKeyEx(int virtualKey)
{
	::SendMessage(Trainer.hwnd, WM_IME_KEYDOWN, virtualKey, 0);
}

void CTibia::doKeyCombo(int virtualKey, int virtualKeyModifier)
{
	::SendMessage(Trainer.hwnd, WM_KEYDOWN, virtualKeyModifier, 0);
	::SendMessage(Trainer.hwnd, WM_KEYDOWN, virtualKey, 0);
	::SendMessage(Trainer.hwnd, WM_KEYUP, virtualKey, 0);
	::SendMessage(Trainer.hwnd, WM_KEYUP, virtualKeyModifier, 0);
}

void CTibia::doHotkey(int hotkeyNumber)
{
	if ((hotkeyNumber < 0) || (hotkeyNumber > MAX_HOTKEYS))
		return;
	
	switch (hotkeyNumber)
	{
		case 1:
			doKey(VK_F1);
			break;
		case 2:
			doKey(VK_F2);
			break;
		case 3:
			doKey(VK_F3);
			break;
		case 4:
			doKey(VK_F4);
			break;
		case 5:
			doKey(VK_F5);
			break;
		case 6:
			doKey(VK_F6);
			break;
		case 7:
			doKey(VK_F7);
			break;
		case 8:
			doKey(VK_F8);
			break;
		case 9:
			doKey(VK_F9);
			break;
		case 10:
			doKey(VK_F10);
			break;
		case 11:
			doKey(VK_F11);
			break;
		case 12:
			doKey(VK_F12);
			break;
		case 13:
			doKeyCombo(VK_F1, VK_SHIFT);
			break;
		case 14:
			doKeyCombo(VK_F2, VK_SHIFT);
			break;
		case 15:
			doKeyCombo(VK_F3, VK_SHIFT);
			break;
		case 16:
			doKeyCombo(VK_F4, VK_SHIFT);
			break;
		case 17:
			doKeyCombo(VK_F5, VK_SHIFT);
			break;
		case 18:
			doKeyCombo(VK_F6, VK_SHIFT);
			break;
		case 19:
			doKeyCombo(VK_F7, VK_SHIFT);
			break;
		case 20:
			doKeyCombo(VK_F8, VK_SHIFT);
			break;
		case 21:
			doKeyCombo(VK_F9, VK_SHIFT);
			break;
		case 22:
			doKeyCombo(VK_F10, VK_SHIFT);
			break;
		case 23:
			doKeyCombo(VK_F11, VK_SHIFT);
			break;
		case 24:
			doKeyCombo(VK_F12, VK_SHIFT);
			break;
		case 25:
			doKeyCombo(VK_F1, VK_CONTROL);
			break;
		case 26:
			doKeyCombo(VK_F2, VK_CONTROL);
			break;
		case 27:
			doKeyCombo(VK_F3, VK_CONTROL);
			break;
		case 28:
			doKeyCombo(VK_F4, VK_CONTROL);
			break;
		case 29:
			doKeyCombo(VK_F5, VK_CONTROL);
			break;
		case 30:
			doKeyCombo(VK_F6, VK_CONTROL);
			break;
		case 31:
			doKeyCombo(VK_F7, VK_CONTROL);
			break;
		case 32:
			doKeyCombo(VK_F8, VK_CONTROL);
			break;
		case 33:
			doKeyCombo(VK_F9, VK_CONTROL);
			break;
		case 34:
			doKeyCombo(VK_F10, VK_CONTROL);
			break;
		case 35:
			doKeyCombo(VK_F11, VK_CONTROL);
			break;
		case 36:
			doKeyCombo(VK_F12, VK_CONTROL);
			break;
	} // switch
}

void CTibia::doPlayerWalk(Direction_t direction)
{
	switch (direction)
	{
		case DIRECTION_UP:
			doKey(VK_UP);
			break;
		case DIRECTION_RIGHT:
			doKey(VK_RIGHT);
			break;
		case DIRECTION_DOWN:
			doKey(VK_DOWN);
			break;
		case DIRECTION_LEFT:
			doKey(VK_LEFT);
			break;
	} // switch
}

void CTibia::doPlayerTurn(Direction_t direction)
{
	switch (direction)
	{
		case DIRECTION_UP:
			doKeyCombo(VK_UP, VK_CONTROL);
			break;
		case DIRECTION_RIGHT:
			doKeyCombo(VK_RIGHT, VK_CONTROL);
			break;
		case DIRECTION_DOWN:
			doKeyCombo(VK_DOWN, VK_CONTROL);
			break;
		case DIRECTION_LEFT:
			doKeyCombo(VK_LEFT, VK_CONTROL);
			break;
	} // switch
}

void CTibia::doPlayerStop()
{
	doKeyEx(VK_ESCAPE);
}

void CTibia::doAntiAFK()
{
	// player must be in-game
	if (!isInGame())
		return;
	
	// player must be standing still
	if (isPlayerWalking())
		return;
	
	int direction = getPlayerDirection();
	
	// turn in the direction the player is already facing
	doPlayerTurn(static_cast<Direction_t>(direction));
}

void CTibia::doXRay()
{
	// read see z-axis
	int seeZ = getSeeZ();

	// default z-axis does not work
	if (seeZ == Z_AXIS_DEFAULT)
		return;

	// read see id
	int seeId = getSeeId();
	if (seeId == 0)
		return;

	// replace last seen tile id with transparent tile id
	doReplaceTile(static_cast<Tile_t>(seeId), TILE_TRANSPARENT);
}

void CTibia::doShowFishingWater()
{
	// get map begin by reading map pointer
	int mapBegin = getMapPointer();

	// search through map data
	for(int i = mapBegin; i < mapBegin + (STEP_MAP_TILE * MAX_MAP_TILES); i += STEP_MAP_TILE)
	{
		// get tile id from current tile data
		int j = i + OFFSET_MAP_TILE_ID;

		// read tile id
		int tileId = Trainer.readBytes(j, 2);
		
		// skip blank ids
		if (tileId == 0)
			continue;

		// tile id matches nofish water tile id
		for (int k = TILE_WATER_NOFISH_BEGIN; k < (TILE_WATER_NOFISH_END + 1); k++)
		{
			if (tileId == k)
			{
				// replace current nofish tile id with old water tile id
				Trainer.writeBytes(j, TILE_WATER_OLD, 2);
			}
		}
	} // for
}

void CTibia::doTreesToBushes()
{
	// trees array
	int treesArray[] =
	{
		OBJECT_TREE_DEAD,
		OBJECT_TREE_FIR,
		OBJECT_TREE_SYCAMORE,
		OBJECT_TREE_WILLOW,		
		OBJECT_TREE_PLUM,
		OBJECT_TREE_MAPLE_RED,
		OBJECT_TREE_PEAR,
		OBJECT_TREE_MAPLE_YELLOW,
		OBJECT_TREE_BEECH,
		OBJECT_TREE_POPLAR,
		OBJECT_TREE_DEAD_2,
		OBJECT_TREE_DEAD_3,
		OBJECT_TREE_DWARF,
		OBJECT_TREE_FIR_SNOW,
		OBJECT_TREE_MAGIC,
		OBJECT_TREE_PINE,
		OBJECT_TREE_DEAD_4,
		OBJECT_TREE_DEAD_5,
		OBJECT_TREE_DEAD_6,
		OBJECT_TREE_OLD	,
		OBJECT_TREE_CACTUS,
		OBJECT_TREE_CACTUS_2,
		OBJECT_TREE_PALM,
		OBJECT_TREE_PALM_2,
		OBJECT_TREE_CACTUS_3,
		OBJECT_TREE_CACTUS_4,
		OBJECT_TREE_CACTUS_5
	};
		
	// get map begin by reading map pointer
	int mapBegin = getMapPointer();

	// search through map data
	for(int i = mapBegin; i < mapBegin + (STEP_MAP_TILE * MAX_MAP_TILES); i += STEP_MAP_TILE)
	{
		// get object id from current tile data
		int j = i + OFFSET_MAP_OBJECT_ID;

		// read object id
		int objectId = Trainer.readBytes(j, 2);
		
		// object id matches ANY tree object id
		for (unsigned int k = 0; k < sizeof(treesArray); k++)
		{
			if (objectId == treesArray[k])
			{
				// replace current object id with bush id
				Trainer.writeBytes(j, OBJECT_BUSH, 2);
			}
		}
		
		// get object data ex from current tile data
		int l = i + OFFSET_MAP_OBJECT_DATA_EX;

		// read object data ex
		int objectDataEx = Trainer.readBytes(l, 2);
			
		// object data ex matches ANY tree object id
		for (unsigned int m = 0; m < sizeof(treesArray); m++)
		{
			if (objectDataEx == treesArray[m])
			{
				// replace current object id with bush id
				Trainer.writeBytes(l, OBJECT_BUSH, 2);
			}
		}
	} // for
}

void CTibia::doReplaceTile(Tile_t oldTileId, Tile_t newTileId)
{
	// get map begin by reading map pointer
	int mapBegin = getMapPointer();

	// search through map data
	for(int i = mapBegin; i < mapBegin + (STEP_MAP_TILE * MAX_MAP_TILES); i += STEP_MAP_TILE)
	{
		// get tile id from current tile data
		int j = i + OFFSET_MAP_TILE_ID;

		// read tile id
		int tileId = Trainer.readBytes(j, 2);
		
		// skip blank ids
		if (tileId == 0)
			continue;

		// tile id matches old tile id
		if (tileId == oldTileId)
		{
			// replace current tile id with new tile id
			Trainer.writeBytes(j, newTileId, 2);
		}
	} // for
}

void CTibia::doReplaceObject(Object_t oldObjectId, Object_t newObjectId)
{
	// get map begin by reading map pointer
	int mapBegin = getMapPointer();

	// search through map data
	for(int i = mapBegin; i < mapBegin + (STEP_MAP_TILE * MAX_MAP_TILES); i += STEP_MAP_TILE)
	{
		// get object id from current tile data
		int j = i + OFFSET_MAP_OBJECT_ID;

		// read object id
		int objectId = Trainer.readBytes(j, 2);

		// object id matches old object id
		if (objectId == oldObjectId)
		{
			// replace current object id with new object id
			Trainer.writeBytes(j, newObjectId, 2);
		}
		
		// get object data ex from current tile data
		int k = i + OFFSET_MAP_OBJECT_DATA_EX;

		// read object data ex
		int objectDataEx = Trainer.readBytes(k, 2);
		
		// object data ex matches old object id
		if (objectDataEx == oldObjectId)
		{
			// replace current object data ex with new object id
			Trainer.writeBytes(k, newObjectId, 2);
		}
	} // for
}

void CTibia::doShowInvisibleCreatures()
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		// skip players
		int type = Trainer.readBytes(i + OFFSET_CREATURE_TYPE, 1);
		if (!type == CREATURE_TYPE_NPC)
			continue;
		
		// skip visible creatures
		int outfitId = Trainer.readBytes(i + OFFSET_CREATURE_OUTFIT, 1);
		if (!outfitId == OUTFIT_INVISIBLE)
			continue;
		
		// replace outfit to make creature visible
		Trainer.writeBytes(i + OFFSET_CREATURE_OUTFIT, OUTFIT_MALE_DRUID, 1);
	}
}

/* set */

void CTibia::setHotkeyByObject(int hotkeyNumber, Item_t itemId, Hotkey_t hotkeyType)
{
	Trainer.writeBytes(HOTKEYS_BEGIN + (STEP_HOTKEY * (hotkeyNumber - 1)), itemId, 2);
	Trainer.writeBytes(HOTKEYS_BEGIN + (STEP_HOTKEY * (hotkeyNumber - 1)) + OFFSET_HOTKEY_TYPE, hotkeyType, 1);
}

void CTibia::setHotkeyByText(int hotkeyNumber, std::string hotkeyText)
{
	Trainer.writeString(HOTKEYS_TEXT_BEGIN + (STEP_HOTKEY_TEXT * (hotkeyNumber - 1)), hotkeyText);
}

void CTibia::setShowNames(bool visible)
{
	if (visible)
	{
		// show
		Trainer.writeNops(SHOW_NAMES, 2);
		Trainer.writeNops(SHOW_NAMES_EX, 2);
	}
	else
	{
		// hide
		Trainer.writeBytes(SHOW_NAMES, SHOW_NAMES_DEFAULT, 2);
		Trainer.writeBytes(SHOW_NAMES_EX, SHOW_NAMES_EX_DEFAULT, 2);
	}
}

void CTibia::setStatusbar(std::string text)
{
	// show statusbar text by timer
	Trainer.writeBytes(STATUSBAR_TIMER, STATUSBAR_DURATION, 1);

	// set statusbar text
	Trainer.writeString(STATUSBAR_TEXT, text);
}

void CTibia::setStatusbarTimer(int value) { Trainer.writeBytes(STATUSBAR_TIMER, value, 1); }

void CTibia::setMouseId(Mouse_t id) { Trainer.writeBytes(MOUSE_ID, id, 1); }

void CTibia::setTargetId(int id) { Trainer.writeBytes(TARGET_ID, id, 3); }

void CTibia::setTargetType(CreatureType_t id) { Trainer.writeBytes(TARGET_TYPE, id, 1); }

void CTibia::setPlayerBattleList(OffsetCreature_t offset, int value, int bytes)
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		int id = Trainer.readBytes(i + OFFSET_CREATURE_ID, 3);
     
		// id matches player
		if (id == getPlayerId())
		{
			Trainer.writeBytes(i + offset, value, bytes);
			break; // found
		}            
	}
}

void CTibia::setPlayerName(std::string name)
{ 
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		int id = Trainer.readBytes(i + OFFSET_CREATURE_ID, 3);
     
		// id matches player
		if (id == getPlayerId())
			Trainer.writeString(i, name);
	}
}

void CTibia::setPlayerLight(LightRadius_t lightRadius)
{
    // light color
	setPlayerBattleList(OFFSET_CREATURE_LIGHT_COLOR, LIGHT_COLOR_ORANGE, 1);

	// light radius
	setPlayerBattleList(OFFSET_CREATURE_LIGHT, lightRadius, 1);
}

void CTibia::setPlayerOutfit(Outfit_t outfitId) { setPlayerBattleList(OFFSET_CREATURE_OUTFIT, outfitId, 2); }

void CTibia::setPlayerOutfitColors(OutfitColor_t head, OutfitColor_t body, OutfitColor_t legs, OutfitColor_t feet)
{
	setPlayerBattleList(OFFSET_CREATURE_OUTFIT_HEAD, head, 1);
	setPlayerBattleList(OFFSET_CREATURE_OUTFIT_BODY, body, 1);
	setPlayerBattleList(OFFSET_CREATURE_OUTFIT_LEGS, legs, 1);
	setPlayerBattleList(OFFSET_CREATURE_OUTFIT_FEET, feet, 1);
}

void CTibia::setPlayerOutfitAddon(OutfitAddon_t outfitAddon) { setPlayerBattleList(OFFSET_CREATURE_OUTFIT_ADDON, outfitAddon, 1); }

void CTibia::setTargetBattleList(OffsetCreature_t offset, int value, int bytes)
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		int id = Trainer.readBytes(i + OFFSET_CREATURE_ID, 3);
     
		// id matches target
		if (id == getTargetId())
		{
			Trainer.writeBytes(i + offset, value, bytes);
			break; // found
		}
	}
}

void CTibia::setTargetOutfit(Outfit_t outfitId)
{
	setTargetBattleList(OFFSET_CREATURE_OUTFIT, outfitId, 2);
}

void CTibia::setTargetOutfitAddon(OutfitAddon_t outfitAddon)
{
	setTargetBattleList(OFFSET_CREATURE_OUTFIT_ADDON, outfitAddon, 1);
}

/* get */

int CTibia::getHotkeyByObject(Item_t itemId, Hotkey_t hotkeyType)
{
	// search through hotkeys
	int j = 1;
	for(int i = HOTKEYS_BEGIN; i < HOTKEYS_END; i += STEP_HOTKEY)
	{
		int id = Trainer.readBytes(i, 2);
		
		int type = Trainer.readBytes(i + OFFSET_HOTKEY_TYPE, 1);
     
		// id matches itemId
		if (id == itemId)
		{
			// type matches hotkeyType
			if (type == hotkeyType)
				return j;
		}
			
		j++;
	} // for
	
	return 255; // not found
}

int CTibia::getHotkeyByText(std::string hotkeyText)
{
	// search through hotkeys text
	int j = 1;
	for(int i = HOTKEYS_TEXT_BEGIN; i < HOTKEYS_TEXT_END; i += STEP_HOTKEY_TEXT)
	{
		std::string text = Trainer.readString(i);
		
		// text matches hotkeyText
		if (text == hotkeyText)
			return j;
			
		j++;
	}
	
	return 255; // not found
}

int CTibia::getExpToLevelUp()
{
	return calcExpForLevel(getPlayerLevel() + 1) - getPlayerExp();
}

int CTibia::getMapPointer() { return Trainer.readBytes(MAP_POINTER, 4); }

std::string CTibia::getStatusbar() { return Trainer.readString(STATUSBAR_TEXT); }
int CTibia::getStatusbarTimer() { return Trainer.readBytes(STATUSBAR_TIMER, 1); }

int CTibia::getMouseId() { return Trainer.readBytes(MOUSE_ID, 1); }

int CTibia::getTargetId() { return Trainer.readBytes(TARGET_ID, 3); }
int CTibia::getTargetType() { return Trainer.readBytes(TARGET_TYPE, 1); }
int CTibia::getTargetBattleListId() { return Trainer.readBytes(TARGET_BATTLELIST_ID, 3); }
int CTibia::getTargetBattleListType() { return Trainer.readBytes(TARGET_BATTLELIST_TYPE, 1); }

int CTibia::getSeeId() { return Trainer.readBytes(SEE_ID, 2); }
int CTibia::getSeeCount() { return Trainer.readBytes(SEE_COUNT, 2); }
int CTibia::getSeeZ() { return Trainer.readBytes(SEE_Z, 1); }

int CTibia::getPlayerBattleList(OffsetCreature_t offset, int bytes)
{ 
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		int id = Trainer.readBytes(i + OFFSET_CREATURE_ID, 3);
     
		// id matches player
		if (id == getPlayerId())
			return Trainer.readBytes(i + offset, bytes);       
	}

	return 0; // not found
}

std::string CTibia::getPlayerName()
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		int id = Trainer.readBytes(i + OFFSET_CREATURE_ID, 3);
     
		// id matches player
		if (id == getPlayerId())
			return Trainer.readString(i);
	}

	return ""; // not found
}

int CTibia::getPlayerId() { return Trainer.readBytes(PLAYER_ID, 3); }
int CTibia::getPlayerX() { return Trainer.readBytes(PLAYER_X, 2); }
int CTibia::getPlayerY() { return Trainer.readBytes(PLAYER_Y, 2); }
int CTibia::getPlayerZ() { return Trainer.readBytes(PLAYER_Z, 1); }
int CTibia::getPlayerExp() { return Trainer.readBytes(PLAYER_EXP, 4); }
int CTibia::getPlayerLevel() { return Trainer.readBytes(PLAYER_LEVEL, 4); }
int CTibia::getPlayerLevelPercent() { return Trainer.readBytes(PLAYER_LEVEL_PERCENT, 1); }
int CTibia::getPlayerMagicLevel() { return Trainer.readBytes(PLAYER_MAGIC_LEVEL, 4); }
int CTibia::getPlayerMagicLevelPercent() { return Trainer.readBytes(PLAYER_MAGIC_LEVEL_PERCENT, 1); }
int CTibia::getPlayerHp() { return Trainer.readBytes(PLAYER_HP, 4); }
int CTibia::getPlayerHpMax() { return Trainer.readBytes(PLAYER_HP_MAX, 4); }
int CTibia::getPlayerMp() { return Trainer.readBytes(PLAYER_MP, 4); }
int CTibia::getPlayerMpMax() { return Trainer.readBytes(PLAYER_MP_MAX, 4); }
int CTibia::getPlayerSoul() { return Trainer.readBytes(PLAYER_SOUL, 1); }
int CTibia::getPlayerCap() { return Trainer.readBytes(PLAYER_CAP, 4); }
int CTibia::getPlayerFlags() { return Trainer.readBytes(PLAYER_FLAGS, 1); }

int CTibia::getPlayerFishing() { return Trainer.readBytes(PLAYER_FISHING, 2); }
int CTibia::getPlayerShielding() { return Trainer.readBytes(PLAYER_SHIELDING, 2); }
int CTibia::getPlayerDistance() { return Trainer.readBytes(PLAYER_DISTANCE, 2); }
int CTibia::getPlayerAxe() { return Trainer.readBytes(PLAYER_AXE, 2); }
int CTibia::getPlayerSword() { return Trainer.readBytes(PLAYER_SWORD, 2); }
int CTibia::getPlayerClub() { return Trainer.readBytes(PLAYER_CLUB, 2); }
int CTibia::getPlayerFist() { return Trainer.readBytes(PLAYER_FIST, 2); }

int CTibia::getPlayerSlotHead() { return Trainer.readBytes(PLAYER_SLOT_HEAD, 2); }
int CTibia::getPlayerSlotNecklace() { return Trainer.readBytes(PLAYER_SLOT_NECKLACE, 2); }
int CTibia::getPlayerSlotBackpack() { return Trainer.readBytes(PLAYER_SLOT_BACKPACK, 2); }
int CTibia::getPlayerSlotArmor() { return Trainer.readBytes(PLAYER_SLOT_ARMOR, 2); }
int CTibia::getPlayerSlotRight() { return Trainer.readBytes(PLAYER_SLOT_RIGHT, 2); }
int CTibia::getPlayerSlotLeft() { return Trainer.readBytes(PLAYER_SLOT_LEFT, 2); }
int CTibia::getPlayerSlotLegs() { return Trainer.readBytes(PLAYER_SLOT_LEGS, 2); }
int CTibia::getPlayerSlotFeet() { return Trainer.readBytes(PLAYER_SLOT_FEET, 2); }
int CTibia::getPlayerSlotRing() { return Trainer.readBytes(PLAYER_SLOT_RING, 2); }
int CTibia::getPlayerSlotAmmo() { return Trainer.readBytes(PLAYER_SLOT_AMMO, 2); }

int CTibia::getPlayerSlotRightCount() { return Trainer.readBytes(PLAYER_SLOT_RIGHT_COUNT, 1); }
int CTibia::getPlayerSlotLeftCount() { return Trainer.readBytes(PLAYER_SLOT_LEFT_COUNT, 1); }
int CTibia::getPlayerSlotAmmoCount() { return Trainer.readBytes(PLAYER_SLOT_AMMO_COUNT, 1); }

int CTibia::getPlayerDirection() { return getPlayerBattleList(OFFSET_CREATURE_DIRECTION, 1); }
int CTibia::getPlayerOutfit() { return getPlayerBattleList(OFFSET_CREATURE_OUTFIT, 2); }
int CTibia::getPlayerOutfitHead() { return getPlayerBattleList(OFFSET_CREATURE_OUTFIT_HEAD, 2); }
int CTibia::getPlayerOutfitBody() { return getPlayerBattleList(OFFSET_CREATURE_OUTFIT_BODY, 2); }
int CTibia::getPlayerOutfitLegs() { return getPlayerBattleList(OFFSET_CREATURE_OUTFIT_LEGS, 2); }
int CTibia::getPlayerOutfitFeet() { return getPlayerBattleList(OFFSET_CREATURE_OUTFIT_FEET, 2); }
int CTibia::getPlayerHpBar() { return getPlayerBattleList(OFFSET_CREATURE_HP_BAR, 1); }
int CTibia::getPlayerWalkSpeed() { return getPlayerBattleList(OFFSET_CREATURE_WALK_SPEED, 2); }
int CTibia::getPlayerLight() { return getPlayerBattleList(OFFSET_CREATURE_LIGHT, 1); }
int CTibia::getPlayerLightColor() { return getPlayerBattleList(OFFSET_CREATURE_LIGHT_COLOR, 1); }
int CTibia::getPlayerSkull() { return getPlayerBattleList(OFFSET_CREATURE_SKULL, 1); }
int CTibia::getPlayerParty() { return getPlayerBattleList(OFFSET_CREATURE_PARTY, 1); }

int CTibia::getTargetBattleList(OffsetCreature_t offset, int bytes)
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		int id = Trainer.readBytes(i + OFFSET_CREATURE_ID, 3);
     
		// id matches target
		if (id == getTargetId())
			return Trainer.readBytes(i + offset, bytes);
	}

	return 0; // not found
}

std::string CTibia::getTargetName()
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += STEP_CREATURE)
	{
		int id = Trainer.readBytes(i + OFFSET_CREATURE_ID, 3);
     
		// id matches target
		if (id == getTargetId())
			return Trainer.readString(i);
	}

	return ""; // not found
}

int CTibia::getTargetX() { return getTargetBattleList(OFFSET_CREATURE_X, 2); }
int CTibia::getTargetY() { return getTargetBattleList(OFFSET_CREATURE_Y, 2); }
int CTibia::getTargetZ() { return getTargetBattleList(OFFSET_CREATURE_Z, 1); }

int CTibia::getTargetDirection() { return getTargetBattleList(OFFSET_CREATURE_DIRECTION, 1); }
int CTibia::getTargetOutfit() { return getTargetBattleList(OFFSET_CREATURE_OUTFIT, 1); }
int CTibia::getTargetSkull() { return getTargetBattleList(OFFSET_CREATURE_SKULL, 1); }
int CTibia::getTargetHpBar() { return getTargetBattleList(OFFSET_CREATURE_HP_BAR, 1); }

ContainerOf CTibia::getContainerOf(int itemID, int itemCount, std::string containerName)
{
	ContainerOf cInfo;

	// search through containers
	int container = 0;
	for(int i = CONTAINER_BEGIN; i < CONTAINER_BEGIN + (STEP_CONTAINER * MAX_CONTAINERS); i += STEP_CONTAINER)
	{
		container++;

		// container must be open
		bool open = Trainer.readBytes(i, 1);
		if (!open)
			continue;

		// name of container
		std::string name = Trainer.readString(i + OFFSET_CONTAINER_NAME);

		// number of items inside container
		int amount = Trainer.readBytes(i + OFFSET_CONTAINER_AMOUNT, 1);
		if (amount == 0)
			continue;

		// search through container's items
		for (int position = 0; position < amount; position++)
		{
			// id of item
		 	int id = Trainer.readBytes(i + OFFSET_CONTAINER_ITEM_ID + (STEP_CONTAINER_ITEM * position), 2);
			if (id == 0)
				continue;

			// stacked count of item
 			int count = Trainer.readBytes((i + OFFSET_CONTAINER_ITEM_ID + (STEP_CONTAINER_ITEM * position)) + (OFFSET_CONTAINER_ITEM_COUNT - OFFSET_CONTAINER_ITEM_ID), 1);

			// item ids match
			if (id == itemID)
			{
				// container names match
				if (name.find(containerName) != std::string::npos)
				{
					cInfo.found	= true;
					cInfo.id	= container - 1;
					cInfo.name	= name;
					cInfo.pos	= position;

					// item count's match
					if (itemCount > 0)
					{
						if (count == itemCount)
							cInfo.count = itemCount;
						else
							continue;
					}
					else
						cInfo.count = count;

					return cInfo;
				} // if
			} // if
		} // for
	} // for

	// not found
	cInfo.found	= false;
	cInfo.id	= 0;
	cInfo.name	= "";
	cInfo.pos	= 0;
	cInfo.count	= 0;
	return cInfo;
}

StackOf CTibia::getStackOf(int itemId)
{
	StackOf sInfo;

	bool foundFrom = false;

	// search through containers
	int container = 0;
	for(int i = CONTAINER_BEGIN; i < CONTAINER_BEGIN + (STEP_CONTAINER * MAX_CONTAINERS); i += STEP_CONTAINER)
	{
		container++;

		// container must be open
		bool open = Trainer.readBytes(i, 1);
		if (!open)
			continue;

		// name of container
		std::string name = Trainer.readString(i + OFFSET_CONTAINER_NAME);

		// number of items inside container
		int amount = Trainer.readBytes(i + OFFSET_CONTAINER_AMOUNT, 1);
		if (amount == 0)
			continue;

		// search through container's items
		for (int position = 0; position < amount; position++)
		{
			// id of item
		 	int id = Trainer.readBytes(i + OFFSET_CONTAINER_ITEM_ID + (STEP_CONTAINER_ITEM * position), 2);
			if (id == 0)
				continue;

			// stacked count of item
 			int count = Trainer.readBytes((i + OFFSET_CONTAINER_ITEM_ID + (STEP_CONTAINER_ITEM * position)) + (OFFSET_CONTAINER_ITEM_COUNT - OFFSET_CONTAINER_ITEM_ID), 1);
			if (count >= MAX_STACK) // already stacked
				continue;

			// item id's match
			if (id == itemId && !foundFrom)
			{
				sInfo.fromContainer	= container - 1;
				sInfo.fromPos		= position;
				sInfo.itemCount		= count;
				foundFrom			= true;
				continue;
			}
			else if (id == itemId && foundFrom)
			{
				// don't stack to special containers
				if (name.find("Dead") != std::string::npos ||
					name.find("Locker") != std::string::npos ||
					name.find("Depot Chest") != std::string::npos)
					continue;

				sInfo.toContainer	= container - 1;
				sInfo.toPos			= position;
				sInfo.found			= true;

				// don't stack to same position
				if (sInfo.fromPos == sInfo.toPos)
					continue;

				return sInfo;
			}
		} // for
	} // for

	// not found
	sInfo.found			= false;
	sInfo.fromContainer	= 0;
	sInfo.fromPos		= 0;
	sInfo.toContainer	= 0;
	sInfo.toPos			= 0;
	sInfo.itemCount		= 0;
	return sInfo;
}
