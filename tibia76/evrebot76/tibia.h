#ifndef TIBIA_H
#define TIBIA_H

#include <sstream>

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

	// -> trainer
	CTrainer Trainer;

	// -> other
	bool findTibia();
	int computeExpForLevel(int level);

	// -> bool
	bool isPlayerAtRookgaard();
	bool isPlayerWalking();
	bool isCreatureWalking(char *creatureName, bool useTargetId);
	bool isCreatureVisible(char *creatureName, bool useTargetId);
	bool isContainerOpen();

	// -> set
	void setRevealNames(bool status);

	void setPermaLight(LightLevel_t lightLevel);
	
	void setStatusbar(char *message);
	void setStatusbarTimer(int duration);
	
	void setTargetId(int id);
	void setAttackId(Attack_t id);
	void setUseId(Use_t id);
	
	void setPlayerBattleList(DistPlayer_t dist, int value, int bytes);
	void setCreatureBattleList(DistPlayer_t dist, int value, int bytes, char *creatureName, bool useTargetId);
	
	void setPlayerName(char *name);
	void setPlayerHp(int hp);
	void setPlayerMp(int mp);

	void setPlayerLight(LightLevel_t lightLevel);
	void setPlayerOutfit(Outfit_t outfitId);
	void setCreatureOutfit(Outfit_t outfitId, char *creatureName, bool useTargetId);

	// -> get
	int getPermaLight();
	
	char *getStatusbar();
	int getStatusbarTimer();

	int getTargetId();
	int getTargetBattleListId();

	int getAttackId();
	int getAttackBattleListId();

	int getUseId();

	int getSeeId();
	int getSeeCount();

	int getPlayerBattleList(DistPlayer_t dist, int bytes);
	int getCreatureBattleList(DistPlayer_t dist, int bytes, char *creatureName, bool useTargetId);

	char *getPlayerName();
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
	int getPlayerIcons();

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

	int getCreatureId(char *creatureName);

	int getTargetX();
	int getTargetY();
	int getTargetZ();

	int getTargetDirection();
	int getTargetHpBar();

	ContainerOf getContainerOf(int itemId_, int itemCount, char *containerName);
	StackOf getStackOf(int itemId);
};

#endif // #ifndef TIBIA_H