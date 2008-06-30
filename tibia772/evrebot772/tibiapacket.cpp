#include "tibiapacket.h"

/*
Tibia Packet
by Evremonde
for client version 7.72
*/

// constructor
CTibiaPacket::CTibiaPacket()
{
	// load packet.dll
	hLib = ::LoadLibrary("packet.dll");

	// get SendPacket function from packet.dll
	SendPacket = (SENDPACKET)::GetProcAddress(hLib, "SendPacket");
}

// deconstructor
CTibiaPacket::~CTibiaPacket()
{
	// unload packet.dll
	::FreeLibrary(hLib);
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

void CTibiaPacket::sendSelectOutfit()
{
	char buffer[3];
	buffer[0] = 0x01;	// packet length
	buffer[1] = 0x00;
	buffer[2] = 0xD2;	// op code

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendSetOutfit(Outfit_t outfitId)
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

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendSetOutfitColors(OutfitColor_t head, OutfitColor_t body, OutfitColor_t legs, OutfitColor_t feet)
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
	int playerY = Tibia.getPlayerY();
	int playerDir = Tibia.getPlayerDirection();

	/*
	// if walking, shoot in front of self
	if (Tibia.isPlayerWalking())
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
	*/

	buffer[11] = playerX & 0xff;		// to x
	buffer[12] = playerX >> 8 & 0xff;	// to x
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
	if (Tibia.getTargetId() > 0 && Tibia.isCreatureVisible("", true))
		SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}

void CTibiaPacket::sendShootTargetBattleList()
{
	// must have rune to shoot
	int itemId = Tibia.getSeeId();
	if (itemId <= 0)
		return;

	// must be ready to shoot
	int useId = Tibia.getUseId();
	if (useId != USE_USING)
		return;

	ContainerOf cInfo = Tibia.getContainerOf(itemId, 0, "");
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

	// search through battle list
	int targetX, targetY, targetZ, targetDir, targetType = 0;
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += DIST_CREATURE)
	{
		int id = Tibia.Trainer.readBytes(i+DIST_CREATURE_ID, 3);
     
		// id matches target battlelist id
		if (id == Tibia.getTargetBattleListId())
		{
			targetX = Tibia.Trainer.readBytes(i+DIST_CREATURE_X, 2);
			targetY = Tibia.Trainer.readBytes(i+DIST_CREATURE_Y, 2);
			targetZ = Tibia.Trainer.readBytes(i+DIST_CREATURE_Z, 1);
			targetDir = Tibia.Trainer.readBytes(i+DIST_CREATURE_DIRECTION, 1);
			targetType = Tibia.Trainer.readBytes(i+DIST_CREATURE_TYPE, 1);
			break; // found
		}            
	}

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

	buffer[15] = targetZ;					// to z
	buffer[16] = 0x63;						// tile id
	buffer[17] = 0x00;						// tile id
	buffer[18] = 0x01;						// stack pos

	// only shoot if valid target
	if (Tibia.getTargetBattleListId() > 0 && targetType == CREATURE_TYPE_PLAYER)
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

void CTibiaPacket::sendSay(Speak_t speak, std::string message)
{
	int length = message.size();

	char buffer[255];
	buffer[0] = length+4;								// packet length
	buffer[1] = 0x00;
	buffer[2] = 0x96;									// op code
	buffer[3] = speak;									// Speak_t
	buffer[4] = length;									// message length
	buffer[5] = 0x00;
	::CopyMemory(&buffer[6], message.c_str(), length);	// message

	SendPacket(Tibia.Trainer.processId, buffer, 1, 0);
}
