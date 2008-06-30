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

int CTibia::isInGame() { return Trainer.readBytes(IS_INGAME, 1); }

bool CTibia::isPlayerWalking() { return getPlayerBattleList(OFFSET_CREATURE_IS_WALKING, 1); }

bool CTibia::isTargetVisible() { return getTargetBattleList(OFFSET_CREATURE_IS_VISIBLE, 1); }

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
	}
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
	}
}

void CTibia::doPlayerStop()
{
	doKeyEx(VK_ESCAPE);
}

void CTibia::doAntiAFK()
{
	// player must be in-game
	if (isInGame() == INGAME_NO)
		return;
	
	// player must be standing still
	if (isPlayerWalking())
		return;
	
	int direction = getPlayerDirection();
	
	// turn in the direction the player is already facing
	doPlayerTurn(static_cast<Direction_t>(direction));
}

void CTibia::doLevelSpyUp()
{
	// do NOPs
	Trainer.writeNops(LEVELSPY_NOP, 2);
	
	// get player z
	int playerZ = Trainer.readBytes(PLAYER_Z, 1);

	// ground level
	int groundLevel = 0;
	if (playerZ <= Z_AXIS_DEFAULT)
		groundLevel = LEVELSPY_ABOVE; // above ground
	else
		groundLevel = LEVELSPY_BELOW; // below ground
	
	// get current level
	int currentLevel = Trainer.readBytes(groundLevel, 1);
	
	if (currentLevel >= LEVELSPY_MAX)
		Trainer.writeBytes(groundLevel, LEVELSPY_MIN, 1); // loop back to start
	else
		Trainer.writeBytes(groundLevel, currentLevel + 1, 1); // increase spy level
}

void CTibia::doLevelSpyDown()
{
	// do NOPs
	Trainer.writeNops(LEVELSPY_NOP, 2);
	
	// get player z
	int playerZ = Trainer.readBytes(PLAYER_Z, 1);

	// ground level
	int groundLevel = 0;
	if (playerZ <= Z_AXIS_DEFAULT)
		groundLevel = LEVELSPY_ABOVE; // above ground
	else
		groundLevel = LEVELSPY_BELOW; // below ground
		
	// get current level
	int currentLevel = Trainer.readBytes(groundLevel, 1);
	
	if (currentLevel <= LEVELSPY_MIN)
		Trainer.writeBytes(groundLevel, LEVELSPY_MAX, 1); // loop back to start
	else
		Trainer.writeBytes(groundLevel, currentLevel - 1, 1); // decrease spy level	
}

void CTibia::doLevelSpyReset()
{
	// restore default values
	Trainer.writeBytes(LEVELSPY_ABOVE, LEVELSPY_ABOVE_DEFAULT, 1);
	Trainer.writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
	
	// undo NOPs
	Trainer.writeBytes(LEVELSPY_NOP, LEVELSPY_NOP_DEFAULT, 2);
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
	}
}

void CTibia::doTreesToBushes()
{
	// trees array; hard-coded object ids
	int treesArray[] =
	{
		3608, 3614, 3615, 3616, 3617,
		3618, 3619, 3620, 3621, 3622,
		3623, 3624, 3625, 3609, 3613,
		3626, 3632, 3633, 3634, 3636,
		3637, 3638, 3639, 3640, 3641,
		3647, 3649
	};
		
	// get map begin by reading map pointer
	int mapBegin = getMapPointer();

	// search through map data
	for(int i = mapBegin; i < mapBegin + (STEP_MAP_TILE * MAX_MAP_TILES); i += STEP_MAP_TILE)
	{
		for (int k = 0; k < (OFFSET_MAP_OBJECT_STEPS + 1); k++)
		{
			// get object id from current tile data
			int j = i + OFFSET_MAP_OBJECT_ID + (k * OFFSET_MAP_OBJECT_STEP);

			// read object id
			int objectId = Trainer.readBytes(j, 2);
		
			// object id matches ANY tree object id
			for (int l = 0; l < sizeof(treesArray); l++)
			{
				if (objectId == treesArray[l])
				{
					// replace current object id with bush id
					Trainer.writeBytes(j, OBJECT_BUSH, 2);
				}
			}
		}
	}
}

void CTibia::doReplaceTile(int oldTileId, int newTileId)
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
	}
}

void CTibia::doReplaceObject(int oldObjectId, int newObjectId)
{
	// get map begin by reading map pointer
	int mapBegin = getMapPointer();

	// search through map data
	for(int i = mapBegin; i < mapBegin + (STEP_MAP_TILE * MAX_MAP_TILES); i += STEP_MAP_TILE)
	{
		for (int k = 0; k < (OFFSET_MAP_OBJECT_STEPS + 1); k++)
		{
			// get object id from current tile data
			int j = i + OFFSET_MAP_OBJECT_ID + (k * OFFSET_MAP_OBJECT_STEP);

			// read object id
			int objectId = Trainer.readBytes(j, 2);
	
			// object id matches old object id
			if (objectId == oldObjectId)
			{
				// replace current object id with new object id
				Trainer.writeBytes(j, newObjectId, 2);
			}
		}
	}
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

void CTibia::setNameSpy(bool visible)
{
	if (visible)
	{
		// show
		Trainer.writeNops(NAME_SPY, 2);
		Trainer.writeNops(NAME_SPY_EX, 2);
	}
	else
	{
		// hide
		Trainer.writeBytes(NAME_SPY, NAME_SPY_DEFAULT, 2);
		Trainer.writeBytes(NAME_SPY_EX, NAME_SPY_EX_DEFAULT, 2);
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
