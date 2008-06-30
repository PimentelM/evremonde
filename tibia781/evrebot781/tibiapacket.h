#ifndef __TIBIAPACKET_H__
#define __TIBIAPACKET_H__

#include <string>

#include "tibia.h"

/*
Tibia Packet
by Evremonde
for Tibia 7.81
*/

class CTibiaPacket
{
  public:
	// constructor and deconstructor
	CTibiaPacket();
	~CTibiaPacket();

	// tibia class
	CTibia Tibia;

	// dll
	HINSTANCE botDll;
	
	// dll functions
	typedef int (WINAPI *SENDPACKET)(DWORD, char *, BOOL, BOOL);
	SENDPACKET SendPacket;

	// send
	void sendLogout();

	void sendWalk(Direction_t direction);
	void sendTurn(Direction_t direction);
	void sendStop();
	
	void sendSay(Speak_t speakType, std::string text);

    void sendUse(int itemId);

	void sendDragToSlot(int itemId, std::string fromContainerName, int toSlot);
	void sendStack(int itemId);

	void sendAttack(int targetId, CreatureType_t attackType);

	void sendShootSelf(int itemId, int itemCount);
	void sendShootTarget(int itemId, int itemCount);
};

#endif // #ifndef __TIBIAPACKET_H__
