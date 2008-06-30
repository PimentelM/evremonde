#ifndef TIBIAPACKET_H
#define TIBIAPACKET_H

#include <string>

#include "tibia.h"

/*
Tibia Packet
by Evremonde
for client version 7.72
*/

class CTibiaPacket
{
  public:
	CTibiaPacket();
	~CTibiaPacket();

	// -> tibia
	CTibia Tibia;

	// -> packet.dll
	typedef int (WINAPI *SENDPACKET)(DWORD, char *, BOOL, BOOL);
	HINSTANCE hLib;
	SENDPACKET SendPacket;

	// -> send
	void sendLogout();

	void sendAttack(int targetId, CreatureType_t attackType);
	
	void sendWalk(Direction_t direction);
	void sendTurn(Direction_t direction);
	void sendStop();
	
	void sendDragToSlot(int itemId, std::string fromContainerName, int toSlot);
	void sendStack(int itemId);

	void sendSelectOutfit();
	void sendSetOutfit(Outfit_t outfitId);
	void sendSetOutfitColors(OutfitColor_t head, OutfitColor_t body, OutfitColor_t legs, OutfitColor_t feet);

	void sendShootSelf(int itemId, int itemCount);
	void sendShootTarget(int itemId, int itemCount);
	void sendShootTargetBattleList();

	void sendUse(int itemId);

	void sendSay(Speak_t speak, std::string message);
};

#endif // #ifndef TIBIAPACKET_H
