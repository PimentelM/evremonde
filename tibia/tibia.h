#ifndef _TIBIA_H_
#define _TIBIA_H_

#include <string>

#include "trainer.h"

#include "tibiaconst.h"

class CTibia
{
public:
	// constructor and deconstructor
	CTibia();
	~CTibia();

	// trainer class
	CTrainer Trainer;

	// other
	bool findTibia();
	HWND getTibiaWindow();

	// calculations
	int calcExpForLevel(int level);

	// booleans
	bool isOnline();

	bool isPlayerFlagged(Flag_t flagType);
	bool isPlayerFullHp();
	bool isPlayerFullMp();
	bool isPlayerWalking();

	bool isTargetWalking();
	bool isTargetVisible();
	bool isTargetAlive();

	bool isFullLightEnabled();
	bool isLevelSpyEnabled();
	bool isNameSpyEnabled();
	bool isShowInvisibleCreaturesEnabled();

	bool isWindowVisible();

	// do
	void doLevelSpyInit();
	void doLevelSpyUp();
	void doLevelSpyDown();

	void doReplaceMapTile(int oldTileId, int newTileId);
	void doReplaceMapObject(int oldObjectId, int newObjectId);

	void doShowFishingWater();
	void doTreesToBushes();

	void doAntiAfk();

	void doKey(int virtualKey);
	void doKeyIme(int virtualKey);
	void doKeyCombo(int virtualKey, int virtualKeyModifier);

	void doHotkey(int hotkeyNumber);

	void doPlayerTurn(Direction_t direction);
	void doPlayerWalk(Direction_t direction);
	void doPlayerWalkTo(Location location);
	void doPlayerStop();
	void doPlayerStopTo();

	// set
	void setHotkeyByObject(int hotkeyNumber, int itemId, HotkeyType_t hotkeyType);
	void setHotkeyByText(int hotkeyNumber, std::string hotkeyText);

	void setFullLight(bool enable);
	void setLevelSpy(bool enable);
	void setNameSpy(bool enable);
	void setShowInvisibleCreatures(bool enable);

	void setRsaKey(unsigned char* key);

	void setFpsLimit(int fps);

	void setStatusbarText(std::string text);
	void setStatusbarTimer(int duration);

	void setLightAmount(LightAmount_t lightAmount);

	void setMouseId(Mouse_t mouseType);

	void setTargetId(int id);
	void setTargetType(CreatureType_t creatureType);

	void setPlayerBattleList(OffsetCreature_t offset, int value, int bytes);

	void setPlayerGoTo(Location location);

	void setPlayerGoToX(int x);
	void setPlayerGoToY(int y);
	void setPlayerGoToZ(int z);

	void setPlayerOutfit(Outfit_t outfitType);
	void setPlayerOutfitColors(OutfitColor_t head, OutfitColor_t body, OutfitColor_t legs, OutfitColor_t feet);
	void setPlayerOutfitAddon(OutfitAddon_t outfitAddon);
	void setPlayerLight(LightColor_t lightColor, LightRadius_t lightRadius);
	void setPlayerIsWalking(bool enable);

	void setTargetBattleList(OffsetCreature_t offset, int value, int bytes);

	void setTargetOutfit(Outfit_t outfitType);
	void setTargetOutfitAddon(OutfitAddon_t outfitAddon);
	void setTargetSkull(Skull_t skullType);

	// get
	int getHotkeyType(int hotkeyNumber);
	int getHotkeyByObject(int itemId, HotkeyType_t hotkeyType);
	int getHotkeyByText(std::string hotkeyText);

	int getExpToLevelUp();

	int getConnectionStatus();

	int getMapPointer();
	int getWindowPointer();
	int getFpsPointer();

	std::string getWindowCaption();

	int getFpsLimit();

	std::string getStatusbarText();
	int getStatusbarTimer();

	int getLightAmount();

	int getMouseId();

	int getClickId();
	int getClickCount();
	int getClickX();
	int getClickY();
	int getClickZ();

	Location getClickLocation();

	int getSeeId();
	int getSeeCount();
	int getSeeX();
	int getSeeY();
	int getSeeZ();
	std::string getSeeText();

	Location getSeeLocation();

	int getTargetId();
	int getTargetType();
	int getTargetBattleListId();
	int getTargetBattleListType();
	int getTargetFollowId();

	int getModeCombat();
	int getModeFollow();
	int getModeSecure();

	int getTilesToGoCurrent();
	int getTilesToGoTotal();

	int getPlayerBattleList(OffsetCreature_t offset, int bytes);

	std::string getPlayerName();

	int getPlayerId();
	int getPlayerX();
	int getPlayerY();
	int getPlayerZ();
	int getPlayerGoToX();
	int getPlayerGoToY();
	int getPlayerGoToZ();
	int getPlayerExp();
	int getPlayerLevel();
	int getPlayerLevelPercent();
	int getPlayerMagicLevel();
	int getPlayerMagicLevelPercent();
	int getPlayerHp();
	int getPlayerHpMax();
	int getPlayerMp();
	int getPlayerMpMax();
	int getPlayerSoul();
	int getPlayerStamina();
	int getPlayerCap();
	int getPlayerFlags();

	int getPlayerFishing();
	int getPlayerShielding();
	int getPlayerDistance();
	int getPlayerAxe();
	int getPlayerSword();
	int getPlayerClub();
	int getPlayerFist();

	int getPlayerFishingPercent();
	int getPlayerShieldingPercent();
	int getPlayerDistancePercent();
	int getPlayerAxePercent();
	int getPlayerSwordPercent();
	int getPlayerClubPercent();
	int getPlayerFistPercent();

	int getPlayerSlotHead();
	int getPlayerSlotNeck();
	int getPlayerSlotBackpack();
	int getPlayerSlotChest();
	int getPlayerSlotRight();
	int getPlayerSlotLeft();
	int getPlayerSlotLegs();
	int getPlayerSlotFeet();
	int getPlayerSlotRing();
	int getPlayerSlotAmmo();

	int getPlayerSlotRightCount();
	int getPlayerSlotLeftCount();
	int getPlayerSlotAmmoCount();

	int getPlayerDirection();
	int getPlayerOutfit();
	int getPlayerOutfitHead();
	int getPlayerOutfitBody();
	int getPlayerOutfitLegs();
	int getPlayerOutfitFeet();
	int getPlayerLightRadius();
	int getPlayerLightColor();
	int getPlayerHpBar();
	int getPlayerWalkSpeed();
	int getPlayerSkull();
	int getPlayerParty();

	Location getPlayerLocation();

	int getTargetBattleList(OffsetCreature_t offset, int bytes);

	std::string getTargetName();

	int getTargetX();
	int getTargetY();
	int getTargetZ();

	int getTargetDirection();
	int getTargetOutfit();
	int getTargetOutfitAddon();
	int getTargetHpBar();
	int getTargetSkull();
	int getTargetParty();

	Location getTargetLocation();

	Item getItem(int itemId, int itemCount, std::string containerName, bool getNextItem, bool stackItem);
};

#endif // #ifndef _TIBIA_H_
