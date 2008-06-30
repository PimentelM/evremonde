/*
Tibia Packet
by Evremonde
*/

#include "tibiapacket.h"

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

/* get */

int CTibiaPacket::getByte(int value, int count) { return value >> ((count - 1) * 8) & 0xFF; }

/* send */

void CTibiaPacket::sendPacket(char* buffer) { SendPacket(Tibia.Trainer.getProcessId(), buffer, 1, 0); }

void CTibiaPacket::sendLogout()
{
	char buffer[3];
	buffer[0] = 0x01; // packet length
	buffer[1] = 0x00;
	buffer[2] = 0x14; // op code

	sendPacket(buffer);
}

void CTibiaPacket::sendPing()
{
	char buffer[3];
	buffer[0] = 0x01; // packet length
	buffer[1] = 0x00;
	buffer[2] = 0x1E; // op code

	sendPacket(buffer);
}

void CTibiaPacket::sendAttack(int creatureId, CreatureType_t creatureType)
{
	char buffer[7];
	buffer[0] = 0x05; // packet length
	buffer[1] = 0x00;
	buffer[2] = 0xA1; // op code
	buffer[3] = getByte(creatureId, 1);
	buffer[4] = getByte(creatureId, 2);
	buffer[5] = getByte(creatureId, 3);
	buffer[6] = creatureType; // CreatureType_t

	sendPacket(buffer);
}

void CTibiaPacket::sendWalk(Direction_t direction)
{
	char buffer[3];
	buffer[0] = 0x01;				// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x65 + direction;	// direction

	sendPacket(buffer);
}

void CTibiaPacket::sendTurn(Direction_t direction)
{
	char buffer[3];
	buffer[0] = 0x01;				// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x6F + direction;	// direction

	sendPacket(buffer);
}

void CTibiaPacket::sendStop()
{
	char buffer[3];
	buffer[0] = 0x01; // packet length
	buffer[1] = 0x00;
	buffer[2] = 0xBE; // op code

	sendPacket(buffer);
}

void CTibiaPacket::sendSay(Speak_t speakType, std::string text)
{
	int length = text.size();

	char buffer[256];
	buffer[0] = length + 4;							// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x96;								// op code
	buffer[3] = speakType;							// Speak_t
	buffer[4] = length;								// text length
	buffer[5] = 0x00;
	::CopyMemory(&buffer[6], text.c_str(), length);	// text

	sendPacket(buffer);
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
	buffer[8] = getByte(itemId, 1);		// item id
	buffer[9] = getByte(itemId, 2);		// item id
	buffer[10] = buffer[7];				// repeat container position
	buffer[11] = 0x00;

	sendPacket(buffer);
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
	buffer[8] = getByte(itemId, 1);		// item id
	buffer[9] = getByte(itemId, 2);		// item id
	buffer[10] = buffer[7];				// repeat container location
	buffer[11] = 0xFF;
	buffer[12] = 0xFF;
	buffer[13] = toSlot;				// to slot
	buffer[14] = 0x00;
	buffer[15] = 0x00;
	buffer[16] = item.count;			// item count

	sendPacket(buffer);
}

void CTibiaPacket::sendStackItems(int itemId)
{
	Item item = Tibia.getItem(itemId, 0, "", false, true);
	Item nextItem = Tibia.getItem(itemId, 0, "", true, true);
	if (!item.found || !nextItem.found)
		return;

	// do not stack to special containers
	if (nextItem.containerName.find("Dead") != std::string::npos ||
		nextItem.containerName.find("Locker") != std::string::npos ||
		nextItem.containerName.find("Depot Chest") != std::string::npos)
		return;

	char buffer[17];
	buffer[0] = 0x0F;					// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x78;					// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + item.container;	// from container number
	buffer[6] = 0x00;
	buffer[7] = item.location;			// from container position
	buffer[8] = getByte(itemId, 1);		// item id
	buffer[9] = getByte(itemId, 2);		// item id
	buffer[10] = buffer[7];				// repeat from container position
	buffer[11] = 0xFF;
	buffer[12] = 0xFF;
	//buffer[13] = 0x40 + nextItem.container;	// to container number
	buffer[13] = buffer[5];				// to container number
	buffer[14] = 0x00;
	buffer[15] = nextItem.location;		// to container position
	buffer[16] = item.count;			// item count

	sendPacket(buffer);
}

void CTibiaPacket::sendOpenContainer(int itemId, Location location, int containerIndex)
{
	char buffer[12];
	buffer[0] = 0x0A;					// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x82;					// op code
	buffer[3] = getByte(location.x, 1);	// x
	buffer[4] = getByte(location.x, 2);	// x
	buffer[5] = getByte(location.y, 1);	// y
	buffer[6] = getByte(location.y, 2);	// y
	buffer[7] = location.z;				// z
	buffer[8] = getByte(itemId, 1);		// item id
	buffer[9] = getByte(itemId, 2);		// item id
	buffer[10] = 0x01;					// stack position
	//buffer[11] = 0x03;					// container index
	buffer[11] = containerIndex;		// container index

	sendPacket(buffer);
}
