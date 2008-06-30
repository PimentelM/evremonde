#ifndef __TIBIA_H__
#define __TIBIA_H__

#include <string>

#include "trainer.h"
#include "tibiaconst.h"

/*
Tibia Trainer
by Evremonde
*/

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

	// calculations
	int calcExpForLevel(int level);
	
	// booleans
	bool isInGame();
	
	bool isPlayerWalking();
	bool isTargetVisible();

	// do
	void doKey(int virtualKey);
	void doKeyEx(int virtualKey);
	void doKeyCombo(int virtualKey, int virtualKeyModifier);
	
	void doHotkey(int hotkeyNumber);
	
	void doPlayerWalk(Direction_t direction);
	void doPlayerTurn(Direction_t direction);
	void doPlayerStop();
	
	void doAntiAFK();
	
	void doXRay();
	void doShowFishingWater();
	void doTreesToBushes();
	
	void doReplaceTile(Tile_t oldTileId, Tile_t newTileId);
	void doReplaceObject(Object_t oldObjectId, Object_t newObjectId);
	
	void doShowInvisibleCreatures();

	// set
	void setHotkeyByObject(int hotkeyNumber, Item_t itemId, Hotkey_t hotkeyType);
	void setHotkeyByText(int hotkeyNumber, std::string hotkeyText);

	void setShowNames(bool visible);
	
	void setStatusbar(std::string text);
	void setStatusbarTimer(int duration);
	
	void setMouseId(Mouse_t id);
	
	void setTargetId(int id);
	void setTargetType(CreatureType_t type);
	
	void setPlayerBattleList(OffsetCreature_t offset, int value, int bytes);
	
	void setPlayerName(std::string name);

	void setPlayerLight(LightRadius_t lightRadius);
	void setPlayerOutfit(Outfit_t outfitId);
	void setPlayerOutfitColors(OutfitColor_t head, OutfitColor_t body, OutfitColor_t legs, OutfitColor_t feet);
	void setPlayerOutfitAddon(OutfitAddon_t outfitAddon);
	
	void setTargetBattleList(OffsetCreature_t offset, int value, int bytes);
	
	void setTargetOutfit(Outfit_t outfitId);
	void setTargetOutfitAddon(OutfitAddon_t outfitAddon);

	// get
	int getHotkeyByObject(Item_t itemId, Hotkey_t hotkeyType);
	int getHotkeyByText(std::string hotkeyText);
	
	int getExpToLevelUp();
	
	int getMapPointer();
	
	std::string getStatusbar();
	int getStatusbarTimer();
	
	int getMouseId();

	int getTargetId();
	int getTargetType();
	int getTargetBattleListId();
	int getTargetBattleListType();

	int getSeeId();
	int getSeeZ();
	int getSeeCount();

	int getPlayerBattleList(OffsetCreature_t offset, int bytes);
	
	std::string getPlayerName();
	
	int getPlayerId();
	int getPlayerX();
	int getPlayerY();
	int getPlayerZ();
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
	int getPlayerCap();
	int getPlayerFlags();

	int getPlayerFishing();
	int getPlayerShielding();
	int getPlayerDistance();
	int getPlayerAxe();
	int getPlayerSword();
	int getPlayerClub();
	int getPlayerFist();

	int getPlayerSlotHead();
	int getPlayerSlotNecklace();
	int getPlayerSlotBackpack();
	int getPlayerSlotArmor();
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
	int getPlayerLight();
	int getPlayerLightColor();
	int getPlayerHpBar();
	int getPlayerWalkSpeed();
	int getPlayerSkull();
	int getPlayerParty();

	int getTargetBattleList(OffsetCreature_t offset, int bytes);

	std::string getTargetName();
	
	int getTargetX();
	int getTargetY();
	int getTargetZ();

	int getTargetDirection();
	int getTargetOutfit();
	int getTargetSkull();
	int getTargetHpBar();
	
	ContainerOf getContainerOf(int itemId, int itemCount, std::string containerName);
	StackOf getStackOf(int itemId);
};

#endif // #ifndef __TIBIA_H__
