#ifndef __TIBIA_H__
#define __TIBIA_H__

#include <string>

#include "trainer.h"

#include "tibiaconst.h"
#include "tibiaenum.h"
#include "tibiastruct.h"

/*
Tibia Trainer
by Evremonde
for Tibia 8.0
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
	HWND getTibiaWindow();
	
	// calculations
	int calcExpForLevel(int level);
	
	// booleans
	bool isInGame();
	
	bool isPlayerWalking();
	bool isTargetVisible();
	
	bool isLevelSpyEnabled();
	bool isNameSpyEnabled();
	
	// do
	void doLevelSpyInit();
	void doLevelSpyUp();
	void doLevelSpyDown();
	
	void doReplaceMapTile(int oldTileId, int newTileId);
	void doReplaceMapObject(int oldObjectId, int newObjectId);
	
	void doShowFishingWater();
	void doTreesToBushes();
	
	void doShowInvisibleCreatures();

	// set
	void setLevelSpy(bool enabled);
	
	void setNameSpy(bool enabled);
	
	void setStatusbar(std::string text);
	void setStatusbarTimer(int duration);
	
	void setMouseId(Mouse_t id);
	
	void setTargetId(int id);
	void setTargetType(CreatureType_t type);

	void setPlayerBattleList(OffsetCreature_t offset, int value, int bytes);

	void setPlayerLight(LightColor_t lightColor, LightRadius_t lightRadius);
	void setPlayerOutfit(Outfit_t outfitId);
	void setPlayerOutfitColors(OutfitColor_t head, OutfitColor_t body, OutfitColor_t legs, OutfitColor_t feet);
	void setPlayerOutfitAddon(OutfitAddon_t outfitAddon);

	void setTargetBattleList(OffsetCreature_t offset, int value, int bytes);

	void setTargetOutfit(Outfit_t outfitId);
	void setTargetOutfitAddon(OutfitAddon_t outfitAddon);

	// get
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
	std::string getSeeText();
	
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
	int getTargetHpBar();
	int getTargetSkull();
	
	Item getItem(int itemId, int itemCount, std::string containerName, bool getNextItem, bool stackItem);
};

#endif // #ifndef __TIBIA_H__
