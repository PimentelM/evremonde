#ifndef TIBIAPACKET_H
#define TIBIAPACKET_H

#include "winsock2.h"

#include "tibia.h"

/*
Tibia Packet
by Evremonde
for protocol version 7.6
*/

class CTibiaPacket
{
  public:
	CTibiaPacket();
	~CTibiaPacket();

	// -> tibia
	CTibia Tibia;

	// original winsock functions
	int (WINAPI *sendNext) (SOCKET s, char *buf, int len, int flags);
	int (WINAPI *recvNext) (SOCKET s, char *buf, int len, int flags);

	// -> get
	char *getSay(char *buf, int len);

	// -> send
	void sendPing(SOCKET socket);

	void sendAttack(SOCKET socket, int targetId, Attack_t attackId);

	void sendStop(SOCKET socket);
	void sendLogout(SOCKET socket);

	void sendMove(SOCKET socket, Direction_t direction);
	void sendTurn(SOCKET socket, Direction_t direction);

	void sendDragToSlot(SOCKET socket, int itemId, char *fromContainerName, int toSlot);
	void sendStack(SOCKET socket, int itemId);

	void sendSelectOutfit(SOCKET socket);
	void sendSetOutfit(SOCKET socket, Outfit_t outfitId);
	void sendSetOutfitColors(SOCKET socket, OutfitColor_t head, OutfitColor_t body, OutfitColor_t legs, OutfitColor_t feet);

	void sendShootSelf(SOCKET socket, int itemId, int itemCount);
	void sendShootTarget(SOCKET socket, int itemId, int itemCount);

	void sendUse(SOCKET socket, int itemId);

	void sendSay(SOCKET socket, Speak_t speak, char *message);
};

#endif // #ifndef TIBIAPACKET_H