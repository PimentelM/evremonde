#ifndef _TIBIAPACKET_H_
#define _TIBIAPACKET_H_

#include <string>

#include "tibia.h"

class CTibiaPacket
{
  public:
	// constructor and deconstructor
	CTibiaPacket();
	~CTibiaPacket();

	// tibia class
	CTibia Tibia;

	// packet dll
	HINSTANCE packetDll;

	// dll functions
	typedef int (WINAPI *SENDPACKET)(DWORD, char*, BOOL, BOOL);
	SENDPACKET SendPacket;

	// get
	int getByte(int value, int count);

	// send
	void sendPacket(char* buffer);

	void sendLogout();

	void sendPing();

	void sendAttack(int creatureId, CreatureType_t creatureType);

	void sendWalk(Direction_t direction);
	void sendTurn(Direction_t direction);
	void sendStop();

	void sendSay(Speak_t speakType, std::string text);

	void sendUseItem(int itemId, std::string fromContainerName);

	void sendMoveItemToSlot(int itemId, std::string fromContainerName, int toSlot);
	void sendStackItems(int itemId);

	void sendOpenContainer(int itemId, Location location, int containerIndex);
	void sendCloseContainer(int containerId);

	void sendCloseEmptyContainers(std::string containerName);
};

#endif // #ifndef _TIBIAPACKET_H_
