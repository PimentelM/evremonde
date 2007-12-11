#include "tibiapacket.h"

/*
Tibia Packet
by Evremonde
for Tibia 8.0
*/

/* constructor and deconstructor */

CTibiaPacket::CTibiaPacket()
{
	// load packet dll
	packetDll = ::LoadLibrary("packet.dll");

	// get SendPacket function from packet dll
	SendPacket = (SENDPACKET)::GetProcAddress(packetDll, "SendPacket");
}

CTibiaPacket::~CTibiaPacket()
{
	// unload packet dll
	::FreeLibrary(packetDll);
}

/* send */

void CTibiaPacket::sendSay(Speak_t speakType, std::string text)
{
	int length = text.size();

	char buffer[255];
	buffer[0] = length + 4;								// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x96;									// op code
	buffer[3] = speakType;								// Speak_t
	buffer[4] = length;									// text length
	buffer[5] = 0x00;
	::CopyMemory(&buffer[6], text.c_str(), length);		// text

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendUseItem(int itemId, std::string fromContainerName)
{
	Item item = Tibia.getItem(itemId, 0, fromContainerName, false, false);
	if (!item.found)
		return;

	char buffer[12];
	buffer[0] = 0x0A;					// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x82;					// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + item.container;	// container number
	buffer[6] = 0x00;
	buffer[7] = item.location;			// container position
	buffer[8] = itemId & 0xff;			// item id
	buffer[9] = itemId >> 8 & 0xff;		// item id
	buffer[10] = buffer[7];				// repeat container position
	buffer[11] = 0x00;

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendMoveItemToSlot(int itemId, std::string fromContainerName, int toSlot)
{
	Item item = Tibia.getItem(itemId, 0, fromContainerName, false, false);
	if (!item.found)
		return;

	char buffer[17];
	buffer[0] = 0x0F;					// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x78;					// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + item.container;	// container number
	buffer[6] = 0x00;
	buffer[7] = item.location;			// container location
	buffer[8] = itemId & 0xff;			// itemId
	buffer[9] = itemId >> 8 & 0xff;		// itemId
	buffer[10] = buffer[7];				// repeat container location
	buffer[11] = 0xFF;
	buffer[12] = 0xFF;
	buffer[13] = toSlot;				// to slot
	buffer[14] = 0x00;
	buffer[15] = 0x00;
	buffer[16] = item.count;				// item count

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendStackItems(int itemId)
{
	Item item = Tibia.getItem(itemId, 0, "", false, true);
	Item nextItem = Tibia.getItem(itemId, 0, "", true, true);
	if (!item.found || !nextItem.found)
		return;
	    
    // don't stack to special containers
	if (nextItem.containerName.find("Dead") != std::string::npos ||
		nextItem.containerName.find("Locker") != std::string::npos ||
		nextItem.containerName.find("Depot Chest") != std::string::npos)
		return;

	char buffer[17];
	buffer[0] = 0x0F;						// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x78;						// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + item.container;		// from container number
	buffer[6] = 0x00;
	buffer[7] = item.location;				// from container position
	buffer[8] = itemId & 0xff;				// itemId
	buffer[9] = itemId >> 8 & 0xff;			// itemId
	buffer[10] = buffer[7];					// repeat from container position
	buffer[11] = 0xFF;
	buffer[12] = 0xFF;
	//buffer[13] = 0x40 + nextItem.container;	// to container number
	buffer[13] = buffer[5];					// to container number
	buffer[14] = 0x00;
	buffer[15] = nextItem.location;			// to container position
	buffer[16] = item.count;				// item count

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}
