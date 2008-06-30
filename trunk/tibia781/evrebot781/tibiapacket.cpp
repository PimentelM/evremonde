#include "tibiapacket.h"

/*
Tibia Packet
by Evremonde
for Tibia 7.81
*/

/* constructor and deconstructor */

CTibiaPacket::CTibiaPacket()
{
	// load dll
	botDll = ::LoadLibrary("evrebot.dll");

	// get SendPacket function from dll
	SendPacket = (SENDPACKET)::GetProcAddress(botDll, "SendPacket");
}

CTibiaPacket::~CTibiaPacket()
{
	// unload dll
	::FreeLibrary(botDll);
}

/* send */

void CTibiaPacket::sendLogout()
{
	char buffer[3];
	buffer[0] = 0x01;	// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x14;	// op code

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendWalk(Direction_t direction)
{
	char buffer[3];
	buffer[0] = 0x01;				// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x65 + direction;	// op code

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendTurn(Direction_t direction)
{
	char buffer[3];
	buffer[0] = 0x01;				// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x6F + direction;	// op code

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendStop()
{
	char buffer[3];
	buffer[0] = 0x01;	// packet length
	buffer[1] = 0x00;
	buffer[2] = 0xBE;	// op code

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

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

void CTibiaPacket::sendUse(int itemId)
{
	ContainerOf cInfo = Tibia.getContainerOf(itemId, 0, "");
	if (!cInfo.found)
		return;

	char buffer[12];
	buffer[0] = 0x0A;				// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x82;				// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + cInfo.id;	// container number
	buffer[6] = 0x00;
	buffer[7] = cInfo.pos;			// container position
	buffer[8] = itemId & 0xff;		// item id
	buffer[9] = itemId >> 8 & 0xff;	// item id
	buffer[10] = buffer[7];			// repeat container position
	buffer[11] = 0x00;

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

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

void CTibiaPacket::sendAttack(int targetId, CreatureType_t creatureType)
{
	Tibia.setTargetId(targetId);		// force client to attack
	Tibia.setTargetType(creatureType);	// force client to attack

	char buffer[7];
	buffer[0] = 0x05;					// packet length
	buffer[1] = 0x00;
	buffer[2] = 0xA1;					// op code

	buffer[3] = targetId & 0xff;		// target id
	buffer[4] = targetId >> 8 & 0xff;	// target id

	buffer[5] = 0x00;
	buffer[6] = creatureType;			// creature type

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendShootSelf(int itemId, int itemCount)
{
	ContainerOf cInfo = Tibia.getContainerOf(itemId, itemCount, "");
	if (!cInfo.found)
		return;

	char buffer[19];
	buffer[0] = 0x11;					// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x83;					// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + cInfo.id;		// container number
	buffer[6] = 0x00;
	buffer[7] = cInfo.pos;				// container position
	buffer[8] = itemId & 0xff;			// item id
	buffer[9] = itemId >> 8 & 0xff;		// item id

	if (itemCount > 0)
		buffer[10] = buffer[7];			// repeat container position
	else
		buffer[10] = 0x00;

	int playerX = Tibia.getPlayerX();
	buffer[11] = playerX & 0xff;		// to x
	buffer[12] = playerX >> 8 & 0xff;	// to x

	int playerY = Tibia.getPlayerY();
	buffer[13] = playerY & 0xff;		// to y
	buffer[14] = playerY >> 8 & 0xff;	// to y

	buffer[15] = Tibia.getPlayerZ();
	buffer[16] = 0x63;					// tile id
	buffer[17] = 0x00;					// tile id
	buffer[18] = 0x01;					// stack pos

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendShootTarget(int itemId, int itemCount)
{
	ContainerOf cInfo = Tibia.getContainerOf(itemId, itemCount, "");
	if (!cInfo.found)
		return;

	char buffer[19];
	buffer[0] = 0x11;						// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x83;						// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + cInfo.id;			// container number
	buffer[6] = 0x00;
	buffer[7] = cInfo.pos;					// container position
	buffer[8] = itemId & 0xff;				// item id
	buffer[9] = itemId >> 8 & 0xff;			// item id
	buffer[10] = 0x00;

	int targetX = Tibia.getTargetX();
	int targetY = Tibia.getTargetY();
	int targetDir = Tibia.getTargetDirection();

	// magic wall shoots in front of target
	if (itemId == ITEM_RUNE_MAGIC_WALL)
	{
		if (targetDir == DIRECTION_UP)
			targetY--;
		else if (targetDir == DIRECTION_DOWN)
			targetY++;
		else if (targetDir == DIRECTION_LEFT)
			targetX--;
		else if (targetDir == DIRECTION_RIGHT)
			targetX++;
	}

	buffer[11] = targetX & 0xff;			// to x
	buffer[12] = targetX >> 8 & 0xff;		// to x
	buffer[13] = targetY & 0xff;			// to y
	buffer[14] = targetY >> 8 & 0xff;		// to y

	buffer[15] = Tibia.getTargetZ();		// to z
	buffer[16] = 0x63;						// tile id
	buffer[17] = 0x00;						// tile id
	buffer[18] = 0x01;						// stack pos

	// only shoot if valid target
	if (Tibia.getTargetId() > 0 && Tibia.isTargetVisible())
		SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}
