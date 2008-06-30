#include "tibia.h"

/*
Tibia Trainer
by Evremonde
*/

// constructor
CTibia::CTibia()
{
	// find window
	Trainer.hwnd = FindWindow("tibiaclient", NULL);
	if (!Trainer.hwnd)
		ExitProcess(0); // exit trainer

	// get process id
	GetWindowThreadProcessId(Trainer.hwnd, &Trainer.processId);

	// get handle
	Trainer.processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Trainer.processId);
  
	// set tibia to low cpu usage
	SetPriorityClass(Trainer.processHandle, Trainer.PRIORITY_BELOW_NORMAL);
}

// deconstructor
CTibia::~CTibia()
{
	// close process
	CloseHandle(Trainer.processHandle);    
}

bool CTibia::findTibia() { return FindWindow("tibiaclient", NULL); }

int CTibia::computeExpForLevel(int level)
{
	return ((50*level*level*level)/3-100*level*level+(850*level)/3-200);
}

bool CTibia::isPlayerAtRookgaard()
{
	int playerX = getPlayerX();
	int playerY = getPlayerY();
	return ( (playerX >= ROOKGAARD_WEST)  &&
			 (playerX <= ROOKGAARD_EAST)  &&
		     (playerY >= ROOKGAARD_NORTH) &&
		     (playerY <= ROOKGAARD_SOUTH) );
}

bool CTibia::isPlayerWalking() { return getPlayerBattleList(DIST_IS_WALKING, 1); }

bool CTibia::isCreatureWalking(char *creatureName, bool useTargetID)
{
	return getCreatureBattleList(DIST_IS_WALKING, 1, creatureName, useTargetID);
}

bool CTibia::isCreatureVisible(char *creatureName, bool useTargetID)
{
	return getCreatureBattleList(DIST_IS_VISIBLE, 1, creatureName, useTargetID);
}

bool CTibia::isContainerOpen() { return Trainer.readBytes(CONTAINER_BEGIN, 1); }

void CTibia::setRevealNames(bool status)
{
	if (status)
	{
		Trainer.writeNops(REVEAL_NAMES_1, 2);
		Trainer.writeNops(REVEAL_NAMES_2, 2);
	}
	else
	{
		// hard-coded values!
		Trainer.writeBytes(REVEAL_NAMES_1, 22389, 2);
		Trainer.writeBytes(REVEAL_NAMES_2, 18805, 2);
	}
}

void CTibia::setPermaLight(LightLevel_t lightLevel) { Trainer.writeBytes(PERMA_LIGHT, lightLevel, 1); }

void CTibia::setStatusbar(char *message)
{
	// set statusbar text
	Trainer.writeString(STATUS_TEXT, message);

	// show statusbar text
	Trainer.writeBytes(STATUS_TIMER, STATUS_DURATION, 1);
}

void CTibia::setStatusbarTimer(int value) { Trainer.writeBytes(STATUS_TIMER, value, 1); }

void CTibia::setTargetId(int id) { Trainer.writeBytes(TARGET_ID, id, 3); }

void CTibia::setAttackId(Attack_t id) { Trainer.writeBytes(ATTACK_ID, id, 1); }

void CTibia::setUseId(Use_t id) { Trainer.writeBytes(USE_ID, id, 1); }

void CTibia::setPlayerBattleList(DistPlayer_t dist, int value, int bytes)
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += DIST_CHARACTER)
	{
		int id = Trainer.readBytes(i+DIST_ID, 3);
     
		// id matches player
		if (id == getPlayerId())
		{
            Trainer.writeBytes(i+dist, value, bytes);
			break; // found
		}            
	}
}

void CTibia::setCreatureBattleList(DistPlayer_t dist, int value, int bytes, char *creatureName, bool useTargetId)
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += DIST_CHARACTER)
	{
		// use target id
		if (useTargetId)
		{
			int id = Trainer.readBytes(i+DIST_ID, 3);
     
			// id matches target
			if (id == getTargetId())
			{
				Trainer.writeBytes(i+dist, value, bytes);
				break; // found
			}
		}
		else
		{
			char *name = Trainer.readString(i);

			// name matches creature
			if ( strcmp(name, creatureName) == 0 )
			{
				Trainer.writeBytes(i+dist, value, bytes);
				break; // found
			}
		} // if
	} // for
}

void CTibia::setPlayerName(char *name)
{ 
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += DIST_CHARACTER)
	{
		int id = Trainer.readBytes(i+DIST_ID, 3);
     
		// id matches player
		if (id == getPlayerId())
			Trainer.writeString(i, name);
	}
}

void CTibia::setPlayerHp(int value) { Trainer.writeBytes(PLAYER_HP, value, 4); }
void CTibia::setPlayerMp(int value) { Trainer.writeBytes(PLAYER_MP, value, 4); }

void CTibia::setPlayerLight(LightLevel_t lightLevel)
{
	// light color
	setPlayerBattleList(DIST_LIGHT_COLOR, LIGHT_COLOR_WHITE, 1);

	// light level
	setPlayerBattleList(DIST_LIGHT, lightLevel, 1);
}

void CTibia::setPlayerOutfit(Outfit_t outfitId) { setPlayerBattleList(DIST_OUTFIT, outfitId, 2); }

void CTibia::setCreatureOutfit(Outfit_t outfitId, char *creatureName, bool useTargetID)
{ 
	setCreatureBattleList(DIST_OUTFIT, outfitId, 2, creatureName, useTargetID);
}

int CTibia::getPermaLight() { return Trainer.readBytes(PERMA_LIGHT, 1); }

char *CTibia::getStatusbar() { return Trainer.readString(STATUS_TEXT); }
int CTibia::getStatusbarTimer() { return Trainer.readBytes(STATUS_TIMER, 1); }

int CTibia::getTargetId() { return Trainer.readBytes(TARGET_ID, 3); }
int CTibia::getTargetBattleListId() { return Trainer.readBytes(TARGET_BATTLELIST_ID, 3); }

int CTibia::getAttackId() { return Trainer.readBytes(ATTACK_ID, 1); }
int CTibia::getAttackBattleListId() { return Trainer.readBytes(ATTACK_BATTLELIST_ID, 1); }

int CTibia::getUseId() { return Trainer.readBytes(USE_ID, 1); }

int CTibia::getSeeId() { return Trainer.readBytes(SEE_ID, 2); }
int CTibia::getSeeCount() { return Trainer.readBytes(SEE_COUNT, 1); }

int CTibia::getPlayerBattleList(DistPlayer_t dist, int bytes)
{ 
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += DIST_CHARACTER)
	{
		int id = Trainer.readBytes(i+DIST_ID, 3);
     
		// id matches player
		if (id == getPlayerId())
			return Trainer.readBytes(i+dist, bytes);       
	}

	// not found
	return 0;
}

int CTibia::getCreatureBattleList(DistPlayer_t dist, int bytes, char *creatureName, bool useTargetId)
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += DIST_CHARACTER)
	{
		// use target id
		if (useTargetId)
		{
			int id = Trainer.readBytes(i+DIST_ID, 3);
     
			// id matches target
			if (id == getTargetId())
				return Trainer.readBytes(i+dist, bytes);
		}
		else
		{
			char *name = Trainer.readString(i);

			// name matches creature
			if ( strcmp(name, creatureName) == 0 )
				return Trainer.readBytes(i+dist, bytes);
		} // if
	} // for

	// not found
	return 0;
}

char *CTibia::getPlayerName() { return Trainer.readString(PLAYER_NAME); }
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
int CTibia::getPlayerIcons() { return Trainer.readBytes(PLAYER_ICONS, 1); }

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

int CTibia::getPlayerDirection() { return getPlayerBattleList(DIST_DIRECTION, 1); }
int CTibia::getPlayerOutfit() { return getPlayerBattleList(DIST_OUTFIT, 2); }
int CTibia::getPlayerOutfitHead() { return getPlayerBattleList(DIST_OUTFIT_HEAD, 2); }
int CTibia::getPlayerOutfitBody() { return getPlayerBattleList(DIST_OUTFIT_BODY, 2); }
int CTibia::getPlayerOutfitLegs() { return getPlayerBattleList(DIST_OUTFIT_LEGS, 2); }
int CTibia::getPlayerOutfitFeet() { return getPlayerBattleList(DIST_OUTFIT_FEET, 2); }
int CTibia::getPlayerHpBar() { return getPlayerBattleList(DIST_HP_BAR, 1); }
int CTibia::getPlayerWalkSpeed() { return getPlayerBattleList(DIST_WALK_SPEED, 2); }
int CTibia::getPlayerLight() { return getPlayerBattleList(DIST_LIGHT, 1); }
int CTibia::getPlayerLightColor() { return getPlayerBattleList(DIST_LIGHT_COLOR, 1); }
int CTibia::getPlayerSkull() { return getPlayerBattleList(DIST_SKULL, 1); }
int CTibia::getPlayerParty() { return getPlayerBattleList(DIST_PARTY, 1); }

int CTibia::getPlayerSex()
{
	int outfit = getPlayerOutfit();
	if (outfit >= OUTFIT_MALE_MIN && outfit <= OUTFIT_MALE_MAX)
		return SEX_MALE;
	else if (outfit >= OUTFIT_FEMALE_MIN && outfit <= OUTFIT_FEMALE_MAX)
		return SEX_FEMALE;
	else
		return SEX_CREATURE;
}

int CTibia::getCreatureId(char *creatureName)
{
	// search through battle list
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += DIST_CHARACTER)
	{
		char *name = Trainer.readString(i);

		// name matches creature
		if ( strcmp(name, creatureName) == 0 )
			return Trainer.readBytes(i+DIST_ID, 3);
	}

	return 0; // not found
}

int CTibia::getTargetX() { return getCreatureBattleList(DIST_X, 2, "", true); }
int CTibia::getTargetY() { return getCreatureBattleList(DIST_Y, 2, "", true); }
int CTibia::getTargetZ() { return getCreatureBattleList(DIST_Z, 1, "", true); }

int CTibia::getTargetDirection() { return getCreatureBattleList(DIST_DIRECTION, 1, "", true); }
int CTibia::getTargetHpBar() { return getCreatureBattleList(DIST_HP_BAR, 1, "", true); }

ContainerOf CTibia::getContainerOf(int itemID, int itemCount, char *containerName)
{
	ContainerOf cInfo;

	// search through first 16 open containers
	int container = 0;
	for(int i = CONTAINER_BEGIN; i < CONTAINER_BEGIN+(DIST_CONTAINER*16); i += DIST_CONTAINER)
	{
		container++;

		// is container open
		bool open = Trainer.readBytes(i, 1);
		if (!open)
			continue;

		// name of container
		char *name = Trainer.readString(i+DIST_CONTAINER_NAME);

		// number of items inside container
		int size = Trainer.readBytes(i+DIST_CONTAINER_AMOUNT, 1);
		if (size == 0)
			continue;

		// search through container's items
		for (int position = 0; position < size; position++)
		{
			// id of item
		 	int id = Trainer.readBytes(i+DIST_CONTAINER_ITEM_ID+(DIST_CONTAINER_ITEM*position), 2);
			if (id == 0)
				continue;

			// stacked count of item
 			int count = Trainer.readBytes((i+DIST_CONTAINER_ITEM_ID+(DIST_CONTAINER_ITEM*position)) + (DIST_CONTAINER_ITEM_COUNT-DIST_CONTAINER_ITEM_ID), 1);

			// item id's match
			if (id == itemID)
			{
				// container name's match
				if (strstr(name, containerName))
				{
					cInfo.found = true;
					cInfo.id = container - 1;
					cInfo.name = name;
					cInfo.pos = position;
			
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
				}
			}
		} // for
	} // for

	// not found
	cInfo.found = false;
	cInfo.id = 0;
	cInfo.name = "";
	cInfo.pos = 0;
	cInfo.count = 0;
	return cInfo;
}

StackOf CTibia::getStackOf(int itemId)
{
	StackOf sInfo;

	bool foundFrom = false;

	// search through first 16 open containers
	int container = 0;
	for(int i = CONTAINER_BEGIN; i < CONTAINER_BEGIN+(DIST_CONTAINER*16); i += DIST_CONTAINER)
	{
		container++;

		// is container open
		bool open = Trainer.readBytes(i, 1);
		if (!open)
			continue;

		// name of container
		char *name = Trainer.readString(i+DIST_CONTAINER_NAME);

		// number of items inside container
		int size = Trainer.readBytes(i+DIST_CONTAINER_AMOUNT, 1);
		if (size == 0)
			continue;

		// search through container's items
		for (int position = 0; position < size; position++)
		{
			// id of item
		 	int id = Trainer.readBytes(i+DIST_CONTAINER_ITEM_ID+(DIST_CONTAINER_ITEM*position), 2);
			if (id == 0)
				continue;

			// stacked count of item
 			int count = Trainer.readBytes((i+DIST_CONTAINER_ITEM_ID+(DIST_CONTAINER_ITEM*position)) + (DIST_CONTAINER_ITEM_COUNT-DIST_CONTAINER_ITEM_ID), 1);
			if (count >= STACK_MAX) // already stacked
				continue;

			// item id's match
			if (id == itemId && !foundFrom)
			{
				sInfo.fromContainer = container - 1;
				sInfo.fromPos = position;
				sInfo.itemCount = count;
				foundFrom = true;
				continue;
			}
			else if (id == itemId && foundFrom)
			{
				// don't stack to special containers
				if (strstr(name, "Dead") ||
					strstr(name, "Locker") ||
					strstr(name, "Depot Chest"))
					continue;

				sInfo.toContainer = container - 1;
				sInfo.toPos = position;
				sInfo.found = true;
				return sInfo;
			}
		} // for
	} // for

	// not found
	sInfo.found = false;
	sInfo.fromContainer = 0;
	sInfo.fromPos = 0;
	sInfo.toContainer = 0;
	sInfo.toPos = 0;
	sInfo.itemCount = 0;
	return sInfo;
}