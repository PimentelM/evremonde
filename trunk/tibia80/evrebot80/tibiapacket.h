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

	// dll
	HINSTANCE botDll;
	
	// dll functions
	typedef int (WINAPI *SENDPACKET)(DWORD, char *, BOOL, BOOL);
	SENDPACKET SendPacket;

	// send
	void sendDragToSlot(int itemId, std::string fromContainerName, int toSlot);
	void sendStack(int itemId);
};

#endif // #ifndef __TIBIAPACKET_H__
