#ifndef TIBIA_H
#define TIBIA_H

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
	// constructor/deconstructor
	CTibia();
	~CTibia();

	// trainer class
	CTrainer Trainer;

	// -> other
	bool findTibia();

	// -> calc
	int calcExpForLevel(int level);
	int calcZAxisForCell(int cell);
	
	// -> bool
	bool isOnline();
	bool isPlayerAtRookgaard();
	bool isPlayerWalking();
	bool isCreatureWalking(std::string creatureName, bool useTargetId);
	bool isCreatureVisible(std::string creatureName, bool useTargetId);
	bool isContainerOpen();

	// -> do
	void doRevealNames(bool enabled);
	void doXRay();

	// -> set
	void setStatusbar(std::string message);
	void setStatusbarTimer(int duration);
	
	void setTargetId(int id);
	void setTargetType(CreatureType_t type);
	void setUseId(Use_t id);
	
	void setPlayerBattleList(DistCreature_t dist, int value, int bytes);
	void setCreatureBattleList(DistCreature_t dist, int value, int bytes, std::string creatureName, bool useTargetId);
	
	void setPlayerName(std::string name);
	void setPlayerHp(int hp);
	void setPlayerMp(int mp);

	void setPlayerLight(LightLevel_t lightLevel);
	void setPlayerOutfit(Outfit_t outfitId);
	void setCreatureOutfit(Outfit_t outfitId, std::string creatureName, bool useTargetId);

	// -> get
	int getMapPointer();
	
	std::string getStatusbar();
	int getStatusbarTimer();

	int getTargetId();
	int getTargetBattleListId();

	int getTargetType();
	int getTargetBattleListType();

	int getUseId();

	int getSeeId();
	int getSeeZ();
	int getSeeCount();

	int getPlayerBattleList(DistCreature_t dist, int bytes);
	int getCreatureBattleList(DistCreature_t dist, int bytes, std::string creatureName, bool useTargetId);

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

	int getPlayerSex();

	int getCreatureId(std::string creatureName);

	int getTargetX();
	int getTargetY();
	int getTargetZ();

	int getTargetDirection();
	int getTargetHpBar();

	ContainerOf getContainerOf(int itemId, int itemCount, std::string containerName);
	StackOf getStackOf(int itemId);
};

#endif // #ifndef TIBIA_H
