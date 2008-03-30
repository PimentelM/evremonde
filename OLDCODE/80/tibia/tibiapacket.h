#ifndef __TIBIAPACKET_H__
#define __TIBIAPACKET_H__

#include <string>

#include "tibia.h"

/*
Tibia Packet
by Evremonde
for Tibia 8.0
*/

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
	typedef int (WINAPI *SENDPACKET)(DWORD, char *, BOOL, BOOL);
	SENDPACKET SendPacket;
	
	// send
	void sendSay(Speak_t speakType, std::string text);
	
	void sendUseItem(int itemId, std::string fromContainerName);
	
	void sendMoveItemToSlot(int itemId, std::string fromContainerName, int toSlot);
	void sendStackItems(int itemId);
};

#endif // #ifndef __TIBIAPACKET_H__
