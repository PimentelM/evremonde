#ifndef _TIBIAPACKET_H_
#define _TIBIAPACKET_H_

/*
Tibia Packet
by Evremonde
*/

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
	
	// send
	void sendSay(Speak_t speakType, std::string text);

	void sendUseItem(int itemId, std::string fromContainerName);

	void sendMoveItemToSlot(int itemId, std::string fromContainerName, int toSlot);
	void sendStackItems(int itemId);
};

#endif // #ifndef _TIBIAPACKET_H_
