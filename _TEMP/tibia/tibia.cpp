/*
Tibia Trainer
by Evremonde
*/

#include "tibia.h"

/* constructor and deconstructor */

CTibia::CTibia()
{
	// find tibia window
	HWND tibiaWindow = ::FindWindow("tibiaclient", 0);

	// set trainer process to tibia window
	Trainer.setProcess(tibiaWindow);
}

CTibia::~CTibia()
{
	// close process
	Trainer.closeProcess();
}

/* other */

bool CTibia::findTibia() { return getTibiaWindow(); }
HWND CTibia::getTibiaWindow() { return ::FindWindow("tibiaclient", 0); }

/* calculations */

int CTibia::calcExpForLevel(int level) { return static_cast<int>(((50*level*level*level)/3-100*level*level+(850*level)/3-200) + 1); }

/* booleans */

bool CTibia::isOnline() { return getConnectionStatus() == CONNECTION_STATUS_ONLINE; }

bool CTibia::isPlayerFlagged(Flag_t flagType) { return (getPlayerFlags() & flagType) == flagType; }

bool CTibia::isPlayerFullHp() { return getPlayerHp() == getPlayerHpMax(); }
bool CTibia::isPlayerFullMp() { return getPlayerMp() == getPlayerMpMax(); }

bool CTibia::isPlayerWalking() { return getPlayerBattleList(OFFSET_CREATURE_IS_WALKING, 1); }

bool CTibia::isTargetWalking() { return getTargetBattleList(OFFSET_CREATURE_IS_WALKING, 1); }
bool CTibia::isTargetVisible() { return getTargetBattleList(OFFSET_CREATURE_IS_VISIBLE, 1); }
bool CTibia::isTargetAlive() { return getTargetBattleList(OFFSET_CREATURE_HP_BAR, 1); }

bool CTibia::isFullLightEnabled() { return Trainer.readBytes(LIGHT_NOP, 2) != LIGHT_NOP_DEFAULT; }

bool CTibia::isLevelSpyEnabled() { return Trainer.readBytes(LEVELSPY_NOP, 2) != LEVELSPY_NOP_DEFAULT; }

bool CTibia::isNameSpyEnabled()
{
	int nameSpyNop = Trainer.readBytes(NAMESPY_NOP, 2);
	int nameSpyNop2 = Trainer.readBytes(NAMESPY_NOP2, 2);
	if (nameSpyNop == NAMESPY_NOP_DEFAULT || nameSpyNop2 == NAMESPY_NOP2_DEFAULT)
		return false;
	else
		return true;
}

bool CTibia::isShowInvisibleCreaturesEnabled()
{
	int invisible1 = Trainer.readBytes(INVISIBLE1, 1);
	int invisible2 = Trainer.readBytes(INVISIBLE2, 1);
	if ((invisible1 == INVISIBLE1_DEFAULT) || (invisible2 == INVISIBLE2_DEFAULT))
		return false;
	else
		return true;
}

bool CTibia::isWindowVisible() { return Trainer.readBytes(getWindowPointer() + OFFSET_WINDOW_VISIBLE, 1); }

/* do */

void CTibia::doLevelSpyInit()
{
	// get player z
	int playerZ = getPlayerZ();

	// set level spy to current level
	if (playerZ <= Z_AXIS_DEFAULT)
	{
		// above ground
		Trainer.writeBytes(LEVELSPY_ABOVE, Z_AXIS_DEFAULT - playerZ, 1); // z-axis calculated
		Trainer.writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
	}
	else
	{
		// below ground
		Trainer.writeBytes(LEVELSPY_ABOVE, LEVELSPY_ABOVE_DEFAULT, 1);
		Trainer.writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
	}
}

void CTibia::doLevelSpyUp()
{
	// level spy must be enabled
	if (!isLevelSpyEnabled())
		return;

	// get player z
	int playerZ = getPlayerZ();

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
	// level spy must be enabled
	if (!isLevelSpyEnabled())
		return;

	// get player z
	int playerZ = getPlayerZ();

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

void CTibia::doReplaceMapTile(int oldTileId, int newTileId)
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

		// skip blank tile ids
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

void CTibia::doReplaceMapObject(int oldObjectId, int newObjectId)
{
	// get map begin by reading map pointer
	int mapBegin = getMapPointer();

	// search through map data
	for(int i = mapBegin; i < mapBegin + (STEP_MAP_TILE * MAX_MAP_TILES); i += STEP_MAP_TILE)
	{
		for (int k = 0; k < (MAX_MAP_TILE_OBJECTS + 1); k++)
		{
			// get object id from current tile data
			int j = i + OFFSET_MAP_OBJECT_ID + (k * STEP_MAP_OBJECT);

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

		// skip blank tile ids
		if (tileId == 0)
			continue;

		// tile id matches nofish water tile id
		for (int k = TILE_WATER_NO_FISH_BEGIN; k < (TILE_WATER_NO_FISH_END + 1); k++)
		{
			if (tileId == k)
			{
				// replace current nofish tile id with lava tile id
				Trainer.writeBytes(j, TILE_LAVA, 2);
			}
		}
	}
}

void CTibia::doTreesToBushes()
{
	// get map begin by reading map pointer
	int mapBegin = getMapPointer();

	// search through map data
	for(int i = mapBegin; i < mapBegin + (STEP_MAP_TILE * MAX_MAP_TILES); i += STEP_MAP_TILE)
	{
		for (int k = 0; k < (MAX_MAP_TILE_OBJECTS + 1); k++)
		{
			// get object id from current tile data
			int j = i + OFFSET_MAP_OBJECT_ID + (k * STEP_MAP_OBJECT);

			// read object id
			int objectId = Trainer.readBytes(j, 2);

			// object id matches ANY tree object id
			for (unsigned int l = 0; l < sizeof(ARRAY_TREES); l++)
			{
				if (objectId == ARRAY_TREES[l])
				{
					// replace current object id with bush id
					Trainer.writeBytes(j, OBJECT_BUSH, 2);
				}
			}
		}
	}
}

void CTibia::doAntiAfk()
{
	// must be online
	if (!isOnline())
		return;

	// must be standing still
	if (isPlayerWalking())
		return;

	// turn in the direction the player is already facing
	doPlayerTurn(static_cast<Direction_t>(getPlayerDirection()));
}

void CTibia::doKey(int virtualKey)
{
	::SendMessage(Trainer.getHwnd(), WM_KEYDOWN, virtualKey, 0);
	::SendMessage(Trainer.getHwnd(), WM_KEYUP, virtualKey, 0);
}

void CTibia::doKeyIme(int virtualKey)
{
	::SendMessage(Trainer.getHwnd(), WM_IME_KEYDOWN, virtualKey, 0);
	::SendMessage(Trainer.getHwnd(), WM_IME_KEYUP, virtualKey, 0);
}

void CTibia::doKeyCombo(int virtualKey, int virtualKeyModifier)
{
	::SendMessage(Trainer.getHwnd(), WM_KEYDOWN, virtualKeyModifier, 0);
	::SendMessage(Trainer.getHwnd(), WM_KEYDOWN, virtualKey, 0);
	::SendMessage(Trainer.getHwnd(), WM_KEYUP, virtualKey, 0);
	::SendMessage(Trainer.getHwnd(), WM_KEYUP, virtualKeyModifier, 0);
}

void CTibia::doHotkey(int hotkeyNumber)
{
	if ((hotkeyNumber <= 0) || (hotkeyNumber > MAX_HOTKEYS))
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

void CTibia::doPlayerWalkTo(Location location)
{
	// set the player's walking-to coordinates
	setPlayerGoTo(location);

	// force the player to walk
	setPlayerIsWalking(true);
}

void CTibia::doPlayerStop() { doKeyIme(VK_ESCAPE); }

void CTibia::doPlayerStopTo()
{
	// set the player's walking-to coordinates
	Location locationStop;
	locationStop.x = 0;
	locationStop.y = 0;
	locationStop.z = 0;
	setPlayerGoTo(locationStop);

	// force the player to stop walking
	setPlayerIsWalking(false);
}

/* set */

void CTibia::setHotkeyByObject(int hotkeyNumber, int itemId, HotkeyType_t hotkeyType)
{
	Trainer.writeBytes(HOTKEY_OBJECT_BEGIN + (STEP_HOTKEY_OBJECT * (hotkeyNumber - 1)), itemId, 2);
	Trainer.writeBytes(HOTKEY_OBJECT_BEGIN + (STEP_HOTKEY_OBJECT * (hotkeyNumber - 1)) + OFFSET_HOTKEY_OBJECT_TYPE, hotkeyType, 1);
}

void CTibia::setHotkeyByText(int hotkeyNumber, std::string hotkeyText)
{
	Trainer.writeString(HOTKEY_TEXT_BEGIN + (STEP_HOTKEY_TEXT * (hotkeyNumber - 1)), hotkeyText);
}

void CTibia::setFullLight(bool enable)
{
	if (enable)
	{
		// enable full light
		Trainer.writeNops(LIGHT_NOP, 2);
		setLightAmount(LIGHT_AMOUNT_FULL);
	}
	else
	{
		// disable full light by restoring default values
		Trainer.writeBytes(LIGHT_NOP, LIGHT_NOP_DEFAULT, 2);
		setLightAmount(LIGHT_AMOUNT_DEFAULT);
	}
}

void CTibia::setLevelSpy(bool enable)
{
	if (enable)
	{
		// enable level spying
		Trainer.writeNops(LEVELSPY_NOP, 2);

		// initialize level spying
		doLevelSpyInit();
	}
	else
	{
		// disable level spying by restoring default values
		Trainer.writeBytes(LEVELSPY_NOP, LEVELSPY_NOP_DEFAULT, 2);

		Trainer.writeBytes(LEVELSPY_ABOVE, LEVELSPY_ABOVE_DEFAULT, 1);
		Trainer.writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
	}
}

void CTibia::setNameSpy(bool enable)
{
	if (enable)
	{
		// enable name spying
		Trainer.writeNops(NAMESPY_NOP, 2);
		Trainer.writeNops(NAMESPY_NOP2, 2);
	}
	else
	{
		// disable name spying by restoring default values
		Trainer.writeBytes(NAMESPY_NOP, NAMESPY_NOP_DEFAULT, 2);
		Trainer.writeBytes(NAMESPY_NOP2, NAMESPY_NOP2_DEFAULT, 2);
	}
}

void CTibia::setShowInvisibleCreatures(bool enable)
{
	if (enable)
	{
		// enable show invisible creatures
		Trainer.writeBytes(INVISIBLE1, INVISIBLE_PATCH, 1);
		Trainer.writeBytes(INVISIBLE2, INVISIBLE_PATCH, 1);
	}
	else
	{
		// disable show invisible creatures by restoring default values
		Trainer.writeBytes(INVISIBLE1, INVISIBLE1_DEFAULT, 1);
		Trainer.writeBytes(INVISIBLE2, INVISIBLE2_DEFAULT, 1);
	}
}

void CTibia::setRsaKey(unsigned char* key)
{
	// unprotect
	Trainer.setProtection(RSA_KEY, PAGE_READWRITE, RSA_KEY_SIZE);

	// set rsa key
	Trainer.writeChar(RSA_KEY, key, RSA_KEY_SIZE);

	// reprotect
	Trainer.setProtection(RSA_KEY, PAGE_READONLY, RSA_KEY_SIZE);
}

void CTibia::setFpsLimit(int fps)
{
	double newFps = (1110 / fps) - 5;

	Trainer.writeDouble(getFpsPointer() + OFFSET_FPS_LIMIT, newFps);
}

void CTibia::setStatusbarText(std::string text)
{
	// show statusbar text by timer
	Trainer.writeBytes(STATUSBAR_TIMER, STATUSBAR_DURATION, 1);

	// set statusbar text
	Trainer.writeString(STATUSBAR_TEXT, text);
}

void CTibia::setStatusbarTimer(int duration) { Trainer.writeBytes(STATUSBAR_TIMER, duration, 1); }

void CTibia::setLightAmount(LightAmount_t lightAmount) { Trainer.writeBytes(LIGHT_AMOUNT, lightAmount, 1); }

void CTibia::setMouseId(Mouse_t mouseType) { Trainer.writeBytes(MOUSE_ID, mouseType, 1); }

void CTibia::setTargetId(int id) { Trainer.writeBytes(TARGET_ID, id, 3); }
void CTibia::setTargetType(CreatureType_t creatureType) { Trainer.writeBytes(TARGET_TYPE, creatureType, 1); }
void CTibia::setTargetBattleListId(int id) { Trainer.writeBytes(TARGET_BATTLELIST_ID, id, 3); }
void CTibia::setTargetBattleListType(CreatureType_t creatureType) { Trainer.writeBytes(TARGET_BATTLELIST_TYPE, creatureType, 1); }
void CTibia::setTargetFollowId(int id) { Trainer.writeBytes(TARGET_FOLLOW_ID, id, 3); }
void CTibia::setTargetFollowType(CreatureType_t creatureType) { Trainer.writeBytes(TARGET_FOLLOW_TYPE, creatureType, 1); }

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

void CTibia::setPlayerGoTo(Location location)
{
	setPlayerGoToX(location.x);
	setPlayerGoToY(location.y);
	setPlayerGoToZ(location.z);
}

void CTibia::setPlayerGoToX(int x) { Trainer.writeBytes(PLAYER_GO_TO_X, x, 2); }
void CTibia::setPlayerGoToY(int y) { Trainer.writeBytes(PLAYER_GO_TO_Y, y, 2); }
void CTibia::setPlayerGoToZ(int z) { Trainer.writeBytes(PLAYER_GO_TO_Z, z, 1); }

void CTibia::setPlayerOutfit(Outfit_t outfitType) { setPlayerBattleList(OFFSET_CREATURE_OUTFIT, outfitType, 2); }

void CTibia::setPlayerOutfitColors(OutfitColor_t head, OutfitColor_t body, OutfitColor_t legs, OutfitColor_t feet)
{
	setPlayerBattleList(OFFSET_CREATURE_OUTFIT_HEAD, head, 1);
	setPlayerBattleList(OFFSET_CREATURE_OUTFIT_BODY, body, 1);
	setPlayerBattleList(OFFSET_CREATURE_OUTFIT_LEGS, legs, 1);
	setPlayerBattleList(OFFSET_CREATURE_OUTFIT_FEET, feet, 1);
}

void CTibia::setPlayerOutfitAddon(OutfitAddon_t outfitAddon) { setPlayerBattleList(OFFSET_CREATURE_OUTFIT_ADDON, outfitAddon, 1); }

void CTibia::setPlayerLight(LightColor_t lightColor, LightRadius_t lightRadius)
{
	// light color
	setPlayerBattleList(OFFSET_CREATURE_LIGHT_COLOR, lightColor, 1);

	// light radius
	setPlayerBattleList(OFFSET_CREATURE_LIGHT_RADIUS, lightRadius, 1);
}

void CTibia::setPlayerIsWalking(bool enable) { setPlayerBattleList(OFFSET_CREATURE_IS_WALKING, enable, 1); }

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

void CTibia::setTargetOutfit(Outfit_t outfitType) { setTargetBattleList(OFFSET_CREATURE_OUTFIT, outfitType, 2); }
void CTibia::setTargetOutfitAddon(OutfitAddon_t outfitAddon) { setTargetBattleList(OFFSET_CREATURE_OUTFIT_ADDON, outfitAddon, 1); }
void CTibia::setTargetSkull(Skull_t skullType) { setTargetBattleList(OFFSET_CREATURE_SKULL, skullType, 1); }

/* get */

int CTibia::getHotkeyType(int hotkeyNumber) { return Trainer.readBytes(HOTKEY_TYPE_BEGIN + (STEP_HOTKEY_TYPE * (hotkeyNumber - 1)), 1); }

int CTibia::getHotkeyByObject(int itemId, HotkeyType_t hotkeyType)
{
	// search through hotkeys
	int j = 1; // starting hotkey number
	for(int i = HOTKEY_OBJECT_BEGIN; i < HOTKEY_OBJECT_END; i += STEP_HOTKEY_OBJECT)
	{
		int id = Trainer.readBytes(i, 2);

		int type = Trainer.readBytes(i + OFFSET_HOTKEY_OBJECT_TYPE, 1);

		// id matches itemId
		if (id == itemId)
		{
			// type matches hotkeyType
			if (type == hotkeyType)
				return j; // return hotkey number
		}

		j++; // increment hotkey number
	}

	return 0; // not found
}

int CTibia::getHotkeyByText(std::string hotkeyText)
{
	// search through hotkeys text
	int j = 1; // starting hotkey number
	for(int i = HOTKEY_TEXT_BEGIN; i < HOTKEY_TEXT_END; i += STEP_HOTKEY_TEXT)
	{
		std::string text = Trainer.readString(i);

		// text matches hotkeyText
		if (text == hotkeyText)
			return j; // return hotkey number

		j++; // increment hotkey number
	}

	return 0; // not found
}

int CTibia::getExpToLevelUp() { return calcExpForLevel(getPlayerLevel() + 1) - getPlayerExp(); }

int CTibia::getConnectionStatus() { return Trainer.readBytes(CONNECTION_STATUS, 1); }

int CTibia::getMapPointer() { return Trainer.readBytes(MAP_POINTER, 4); }
int CTibia::getWindowPointer() { return Trainer.readBytes(WINDOW_POINTER, 4); }
int CTibia::getFpsPointer() { return Trainer.readBytes(FPS_POINTER, 4); }

std::string CTibia::getWindowCaption() { return Trainer.readString(getWindowPointer() + OFFSET_WINDOW_CAPTION); }

int CTibia::getFpsLimit() { return Trainer.readBytes(getFpsPointer() + OFFSET_FPS_LIMIT2, 1); }

std::string CTibia::getStatusbarText()
{
	// text must be visible
	if (getStatusbarTimer() == 0)
		return "";

	return Trainer.readString(STATUSBAR_TEXT);
}

int CTibia::getStatusbarTimer() { return Trainer.readBytes(STATUSBAR_TIMER, 1); }

int CTibia::getLightAmount() { return Trainer.readBytes(LIGHT_AMOUNT, 1); }

int CTibia::getMouseId() { return Trainer.readBytes(MOUSE_ID, 1); }

int CTibia::getClickId() { return Trainer.readBytes(CLICK_ID, 2); }
int CTibia::getClickCount() { return Trainer.readBytes(CLICK_COUNT, 3); }
int CTibia::getClickX() { return Trainer.readBytes(CLICK_X, 2); }
int CTibia::getClickY() { return Trainer.readBytes(CLICK_Y, 2); }
int CTibia::getClickZ() { return Trainer.readBytes(CLICK_Z, 1); }

Location CTibia::getClickLocation()
{
	Location location;

	// get see x, y, z
	location.x = Trainer.readBytes(CLICK_X, 2);
	location.y = Trainer.readBytes(CLICK_Y, 2);
	location.z = Trainer.readBytes(CLICK_Z, 1);

	return location;
}

int CTibia::getSeeId() { return Trainer.readBytes(SEE_ID, 2); }
int CTibia::getSeeCount() { return Trainer.readBytes(SEE_COUNT, 3); }
int CTibia::getSeeX() { return Trainer.readBytes(SEE_X, 2); }
int CTibia::getSeeY() { return Trainer.readBytes(SEE_Y, 2); }
int CTibia::getSeeZ() { return Trainer.readBytes(SEE_Z, 1); }
std::string CTibia::getSeeText() { return Trainer.readString(SEE_TEXT); }

Location CTibia::getSeeLocation()
{
	Location location;

	// get see x, y, z
	location.x = Trainer.readBytes(SEE_X, 2);
	location.y = Trainer.readBytes(SEE_Y, 2);
	location.z = Trainer.readBytes(SEE_Z, 1);

	return location;
}

int CTibia::getTargetId() { return Trainer.readBytes(TARGET_ID, 3); }
int CTibia::getTargetType() { return Trainer.readBytes(TARGET_TYPE, 1); }
int CTibia::getTargetBattleListId() { return Trainer.readBytes(TARGET_BATTLELIST_ID, 3); }
int CTibia::getTargetBattleListType() { return Trainer.readBytes(TARGET_BATTLELIST_TYPE, 1); }
int CTibia::getTargetFollowId() { return Trainer.readBytes(TARGET_FOLLOW_ID, 3); }
int CTibia::getTargetFollowType() { return Trainer.readBytes(TARGET_FOLLOW_TYPE, 3); }

int CTibia::getModeCombat() { return Trainer.readBytes(MODE_COMBAT, 1); }
int CTibia::getModeFollow() { return Trainer.readBytes(MODE_FOLLOW, 1); }
int CTibia::getModeSecure() { return Trainer.readBytes(MODE_SECURE, 1); }

int CTibia::getTilesToGoCurrent() { return Trainer.readBytes(TILES_TO_GO_CURRENT, 2); }
int CTibia::getTilesToGoTotal() { return Trainer.readBytes(TILES_TO_GO_TOTAL, 2); }

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
			return Trainer.readString(i + OFFSET_CREATURE_NAME);
	}

	return ""; // not found
}

int CTibia::getPlayerId() { return Trainer.readBytes(PLAYER_ID, 3); }
int CTibia::getPlayerX() { return Trainer.readBytes(PLAYER_X, 2); }
int CTibia::getPlayerY() { return Trainer.readBytes(PLAYER_Y, 2); }
int CTibia::getPlayerZ() { return Trainer.readBytes(PLAYER_Z, 1); }
int CTibia::getPlayerGoToX() { return Trainer.readBytes(PLAYER_GO_TO_X, 2); }
int CTibia::getPlayerGoToY() { return Trainer.readBytes(PLAYER_GO_TO_Y, 2); }
int CTibia::getPlayerGoToZ() { return Trainer.readBytes(PLAYER_GO_TO_Z, 1); }
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
int CTibia::getPlayerStamina() { return Trainer.readBytes(PLAYER_STAMINA, 4); }
int CTibia::getPlayerCap() { return Trainer.readBytes(PLAYER_CAP, 4); }
int CTibia::getPlayerFlags() { return Trainer.readBytes(PLAYER_FLAGS, 4); }

int CTibia::getPlayerFishing() { return Trainer.readBytes(PLAYER_FISHING, 2); }
int CTibia::getPlayerShielding() { return Trainer.readBytes(PLAYER_SHIELDING, 2); }
int CTibia::getPlayerDistance() { return Trainer.readBytes(PLAYER_DISTANCE, 2); }
int CTibia::getPlayerAxe() { return Trainer.readBytes(PLAYER_AXE, 2); }
int CTibia::getPlayerSword() { return Trainer.readBytes(PLAYER_SWORD, 2); }
int CTibia::getPlayerClub() { return Trainer.readBytes(PLAYER_CLUB, 2); }
int CTibia::getPlayerFist() { return Trainer.readBytes(PLAYER_FIST, 2); }

int CTibia::getPlayerFishingPercent() { return Trainer.readBytes(PLAYER_FISHING_PERCENT, 1); }
int CTibia::getPlayerShieldingPercent() { return Trainer.readBytes(PLAYER_SHIELDING_PERCENT, 1); }
int CTibia::getPlayerDistancePercent() { return Trainer.readBytes(PLAYER_DISTANCE_PERCENT, 1); }
int CTibia::getPlayerAxePercent() { return Trainer.readBytes(PLAYER_AXE_PERCENT, 1); }
int CTibia::getPlayerSwordPercent() { return Trainer.readBytes(PLAYER_SWORD_PERCENT, 1); }
int CTibia::getPlayerClubPercent() { return Trainer.readBytes(PLAYER_CLUB_PERCENT, 1); }
int CTibia::getPlayerFistPercent() { return Trainer.readBytes(PLAYER_FIST_PERCENT, 1); }

int CTibia::getPlayerSlotHead() { return Trainer.readBytes(PLAYER_SLOT_HEAD, 2); }
int CTibia::getPlayerSlotNeck() { return Trainer.readBytes(PLAYER_SLOT_NECK, 2); }
int CTibia::getPlayerSlotBackpack() { return Trainer.readBytes(PLAYER_SLOT_BACKPACK, 2); }
int CTibia::getPlayerSlotChest() { return Trainer.readBytes(PLAYER_SLOT_CHEST, 2); }
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
int CTibia::getPlayerLightRadius() { return getPlayerBattleList(OFFSET_CREATURE_LIGHT_RADIUS, 1); }
int CTibia::getPlayerLightColor() { return getPlayerBattleList(OFFSET_CREATURE_LIGHT_COLOR, 1); }
int CTibia::getPlayerSkull() { return getPlayerBattleList(OFFSET_CREATURE_SKULL, 1); }
int CTibia::getPlayerParty() { return getPlayerBattleList(OFFSET_CREATURE_PARTY, 1); }

Location CTibia::getPlayerLocation()
{
	Location location;

	// get player x, y, z
	location.x = Trainer.readBytes(PLAYER_X, 2);
	location.y = Trainer.readBytes(PLAYER_Y, 2);
	location.z = Trainer.readBytes(PLAYER_Z, 1);

	return location;
}

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
			return Trainer.readString(i + OFFSET_CREATURE_NAME);
	}

	return ""; // not found
}

int CTibia::getTargetX() { return getTargetBattleList(OFFSET_CREATURE_X, 2); }
int CTibia::getTargetY() { return getTargetBattleList(OFFSET_CREATURE_Y, 2); }
int CTibia::getTargetZ() { return getTargetBattleList(OFFSET_CREATURE_Z, 1); }

Location CTibia::getTargetLocation()
{
	Location location;

	// get target x, y, z
	location.x = getTargetBattleList(OFFSET_CREATURE_X, 2);
	location.y = getTargetBattleList(OFFSET_CREATURE_Y, 2);
	location.z = getTargetBattleList(OFFSET_CREATURE_Z, 1);

	return location;
}

int CTibia::getTargetDirection() { return getTargetBattleList(OFFSET_CREATURE_DIRECTION, 1); }
int CTibia::getTargetOutfit() { return getTargetBattleList(OFFSET_CREATURE_OUTFIT, 2); }
int CTibia::getTargetOutfitAddon() { return getTargetBattleList(OFFSET_CREATURE_OUTFIT_ADDON, 1); }
int CTibia::getTargetHpBar() { return getTargetBattleList(OFFSET_CREATURE_HP_BAR, 1); }
int CTibia::getTargetSkull() { return getTargetBattleList(OFFSET_CREATURE_SKULL, 1); }
int CTibia::getTargetParty() { return getTargetBattleList(OFFSET_CREATURE_PARTY, 1); }

Item CTibia::getItem(int itemId, int itemCount, std::string containerName, bool getNextItem, bool stackItem)
{
	Item item;

	bool foundNext = false;

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

		// use container name
		if (containerName.size())
			if (name.find(containerName) == std::string::npos)
				continue;

		// number of items inside container
		int amount = Trainer.readBytes(i + OFFSET_CONTAINER_AMOUNT, 1);
		if (amount == 0)
			continue;

		// search through container's items
		for (int location = 0; location < amount; location++)
		{
			// id of item
			int id = Trainer.readBytes(i + OFFSET_CONTAINER_ITEM_ID + (STEP_CONTAINER_ITEM * location), 2);
			if (id == 0)
				continue;

			// stacked count of item
 			int count = Trainer.readBytes((i + OFFSET_CONTAINER_ITEM_ID + (STEP_CONTAINER_ITEM * location)) + (OFFSET_CONTAINER_ITEM_COUNT - OFFSET_CONTAINER_ITEM_ID), 1);

			// use stack item
			if (stackItem && count >= MAX_STACK) // already stacked
				continue;

			// item ids match
			if (id == itemId)
			{
				// use get next item
				if (getNextItem && !foundNext)
				{
					foundNext = true;
					continue;
				}

				item.id				= id;
				//item.count			= count;
				item.location		= location;
				item.container		= container - 1;
				item.containerName	= name;
				item.found			= true;

				// use item count
				if (itemCount > 0)
					item.count = itemCount;
				else
					item.count = count;

				return item; // item found
			}
		}
	}

	// item not found
	item.id				= 0;
	item.count			= 0;
	item.location		= 0;
	item.container		= 0;
	item.containerName	= "";
	item.found			= false;
	return item;
}
