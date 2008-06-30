#include "tibiapacket.h"

/*
Tibia Packet
by Evremonde
for protocol version 7.6
*/

// constructor
CTibiaPacket::CTibiaPacket()
{
	//
}

// deconstructor
CTibiaPacket::~CTibiaPacket()
{
	//
}

char *CTibiaPacket::getSay(char *buf, int len)
{
	int start = 6;
	int length = len-6;

	if (buf[2] == 0xFFFFFFFFFFFFFF96)			// op code = parseSay
	{
		if (buf[1] == 0x00 && buf[5] == 0x00)	// seperators
		{
			if (buf[4] == length)				// say length
			{
				char *say;
				say = (char*)malloc(length+1);
				strncpy(say, buf+start, length);
				say[length] = '\0';

				return say;						// say message
			}
		}
	}

	// nothing said
	return "";
}

void CTibiaPacket::sendPing(SOCKET socket)
{
	char buffer[3];
	buffer[0] = 0x01;	// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x1E;	// op code

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendAttack(SOCKET socket, int targetId, Attack_t attackId)
{
	Tibia.setTargetId(targetId);		// force client to attack
	Tibia.setAttackId(attackId);		// force client to attack

	char buffer[7];
	buffer[0] = 0x05;					// packet length
	buffer[1] = 0x00;
	buffer[2] = 0xA1;					// op code

	buffer[3] = targetId & 0xff;		// target id
	buffer[4] = targetId >> 8 & 0xff;	// target id

	buffer[5] = 0x00;
	buffer[6] = attackId;				// attack id

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendStop(SOCKET socket)
{
	char buffer[3];
	buffer[0] = 0x01;	// packet length
	buffer[1] = 0x00;
	buffer[2] = 0xBE;	// op code

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendLogout(SOCKET socket)
{
	char buffer[3];
	buffer[0] = 0x01;	// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x14;	// op code

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendMove(SOCKET socket, Direction_t direction)
{
	char buffer[3];
	buffer[0] = 0x01;				// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x65 + direction;	// op code

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendTurn(SOCKET socket, Direction_t direction)
{
	char buffer[3];
	buffer[0] = 0x01;				// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x6F + direction;	// op code

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendDragToSlot(SOCKET socket, int itemId, char *fromContainerName, int toSlot)
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

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendStack(SOCKET socket, int itemId)
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

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendSelectOutfit(SOCKET socket)
{
	char buffer[3];
	buffer[0] = 0x01;	// packet length
	buffer[1] = 0x00;
	buffer[2] = 0xD2;	// op code

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendSetOutfit(SOCKET socket, Outfit_t outfitId)
{
	char buffer[8];
	buffer[0] = 0x06;							// packet length
	buffer[1] = 0x00;
	buffer[2] = 0xD3;							// op code
	buffer[3] = outfitId;						// outfit id
	buffer[4] = Tibia.getPlayerOutfitHead();	// outfit head
	buffer[5] = Tibia.getPlayerOutfitBody();	// outfit body
	buffer[6] = Tibia.getPlayerOutfitLegs();	// outfit legs
	buffer[7] = Tibia.getPlayerOutfitFeet();	// outfit feet

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendSetOutfitColors(SOCKET socket, OutfitColor_t head, OutfitColor_t body, OutfitColor_t legs, OutfitColor_t feet)
{
	char buffer[8];
	buffer[0] = 0x06;						// packet length
	buffer[1] = 0x00;
	buffer[2] = 0xD3;						// op code
	buffer[3] = Tibia.getPlayerOutfit();	// outfit id
	buffer[4] = head;						// outfit head
	buffer[5] = body;						// outfit body
	buffer[6] = legs;						// outfit legs
	buffer[7] = feet;						// outfit feet

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendShootSelf(SOCKET socket, int itemId, int itemCount)
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
	int playerY = Tibia.getPlayerY();
	int playerDir = Tibia.getPlayerDirection();
	if (Tibia.isPlayerWalking())		// if walking, shoot in front of self
	{
		if (playerDir == DIRECTION_UP)
			playerY--;
		else if (playerDir == DIRECTION_DOWN)
			playerY++;
		else if (playerDir == DIRECTION_LEFT)
			playerX--;
		else if (playerDir == DIRECTION_RIGHT)
			playerX++;
	}
	buffer[11] = playerX & 0xff;		// to x
	buffer[12] = playerX >> 8 & 0xff;	// to x
	buffer[13] = playerY & 0xff;		// to y
	buffer[14] = playerY >> 8 & 0xff;	// to y

	buffer[15] = Tibia.getPlayerZ();
	buffer[16] = 0x63;					// tile id
	buffer[17] = 0x00;					// tile id
	buffer[18] = 0x01;					// stack pos

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendShootTarget(SOCKET socket, int itemId, int itemCount)
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
	if (itemId == ITEM_RUNE_MAGIC_WALL)		// magic wall shoots in front of target		
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
	if (Tibia.getTargetId() > 0 && Tibia.isCreatureVisible("", true))
		sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendUse(SOCKET socket, int itemId)
{
	ContainerOf cInfo = Tibia.getContainerOf(itemId, 0, "");
	if (!cInfo.found)
		return;

	char buffer[12];
	buffer[0] = 0x0A;					// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x82;					// op code
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	buffer[5] = 0x40 + cInfo.id;		// container number
	buffer[6] = 0x00;
	buffer[7] = cInfo.pos;				// container position
	buffer[8] = itemId & 0xff;			// item id
	buffer[9] = itemId >> 8 & 0xff;		// item id
	buffer[10] = buffer[7];				// repeat container position
	buffer[11] = 0x00;

	sendNext(socket, buffer, sizeof(buffer), 0);
}

void CTibiaPacket::sendSay(SOCKET socket, Speak_t speak, char *message)
{
	int length = strlen(message);

	char buffer[255];
	buffer[0] = length+4;						// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x96;							// op code
	buffer[3] = speak;							// Speak_t
	buffer[4] = length;							// message length
	buffer[5] = 0x00;
	CopyMemory(&buffer[6], message, length);	// message

	sendNext(socket, buffer, length+6, 0);
}