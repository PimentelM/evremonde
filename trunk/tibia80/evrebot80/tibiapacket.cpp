#include "tibiapacket.h"

/*
Tibia Packet
by Evremonde
for Tibia 8.0
*/

/* constructor and deconstructor */

CTibiaPacket::CTibiaPacket()
{
	// load dll
	botDll = ::LoadLibrary("packet.dll");

	// get SendPacket function from dll
	SendPacket = (SENDPACKET)::GetProcAddress(botDll, "SendPacket");
}

CTibiaPacket::~CTibiaPacket()
{
	// unload dll
	::FreeLibrary(botDll);
}

/* send */

void CTibiaPacket::sendDragToSlot(int itemId, std::string fromContainerName, int toSlot)
{
	ContainerOf cInfo = Tibia.getContainerOf(itemId, 0, fromContainerName);
	if (!cInfo.found)
		return;

	char buffer[17];
	buffer[0] = 0x0F;				// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x78;				// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + cInfo.id;	// container number
	buffer[6] = 0x00;
	buffer[7] = cInfo.pos;			// container position
	buffer[8] = itemId & 0xff;		// itemId
	buffer[9] = itemId >> 8 & 0xff;	// itemId
	buffer[10] = buffer[7];			// repeat container position
	buffer[11] = 0xFF;
	buffer[12] = 0xFF;
	buffer[13] = toSlot;			// to slot
	buffer[14] = 0x00;
	buffer[15] = 0x00;
	buffer[16] = cInfo.count;		// item count

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendStack(int itemId)
{
	StackOf sInfo = Tibia.getStackOf(itemId);
	if (!sInfo.found)
		return;

	char buffer[17];
	buffer[0] = 0x0F;						// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x78;						// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + sInfo.fromContainer;	// from container number
	buffer[6] = 0x00;
	buffer[7] = sInfo.fromPos;				// from container position
	buffer[8] = itemId & 0xff;				// itemId
	buffer[9] = itemId >> 8 & 0xff;			// itemId
	buffer[10] = buffer[7];					// repeat from container position
	buffer[11] = 0xFF;
	buffer[12] = 0xFF;
	//buffer[13] = 0x40 + sInfo.toContainer;	// to container number
	buffer[13] = buffer[5];					// to container number
	buffer[14] = 0x00;
	buffer[15] = sInfo.toPos;				// to container position
	buffer[16] = sInfo.itemCount;			// item count

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

