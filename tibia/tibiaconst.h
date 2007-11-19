#ifndef __TIBIACONST_H__
#define __TIBIACONST_H__

/*
Tibia Constants
by Evremonde
for Tibia 8.0
*/

/* memory addresses */

// in-game
const int IS_INGAME						= 0x00766DF8; // Ingame_t

// map
const int MAP_POINTER					= 0x0061E408; // readBytes 4

// window (gui)
const int WINDOW_POINTER				= 0x006198B4; // OffsetWindow_t

// battlelist
const int BATTLELIST_BEGIN				= 0x0060EB34; // 0x0060EB30 + 4 for OFFSET_CREATURE_ID
const int BATTLELIST_END				= 0x006148F4; // 0x006148F0 + 4 for OFFSET_CREATURE_ID

// container
const int CONTAINER_BEGIN 				= 0x00617000;
const int CONTAINER_END					= 0x00618EC0;

// statusbar
const int STATUSBAR_TEXT				= 0x00768458;
const int STATUSBAR_TIMER				= 0x00768454;

// level spy
const int LEVELSPY_NOP					= 0x004C4320; // writeNops 2 bytes; 2BC1 SUB EAX, ECX

const int LEVELSPY_ABOVE				= 0x004C431C; // B8 07 MOV EAX, 7
const int LEVELSPY_BELOW				= 0x004C4324; // B8 02 MOV EAX, 2

// name spy
const int NAMESPY_NOP					= 0x004DD2D7; // writeNops 2 bytes; 75 4C JNZ SHORT
const int NAMESPY_NOP2					= 0x004DD2E1; // writeNops 2 bytes; 75 42 JNZ SHORT

// player
const int PLAYER_X						= 0x006198F8; // coordinates
const int PLAYER_Y						= 0x006198F4;
const int PLAYER_Z						= 0x006198F0;

const int PLAYER_ID						= 0x0060EAD0; // unique id

const int PLAYER_HP						= 0x0060EACC; // statistics
const int PLAYER_HP_MAX					= 0x0060EAC8;
const int PLAYER_EXP					= 0x0060EAC4;
const int PLAYER_LEVEL					= 0x0060EAC0;
const int PLAYER_MAGIC_LEVEL			= 0x0060EABC;
const int PLAYER_LEVEL_PERCENT			= 0x0060EAB8;
const int PLAYER_MAGIC_LEVEL_PERCENT	= 0x0060EAB4;
const int PLAYER_MP						= 0x0060EAB0;
const int PLAYER_MP_MAX					= 0x0060EAAC;
const int PLAYER_SOUL					= 0x0060EAA8;
const int PLAYER_STAMINA				= 0x0060EAA4; // in seconds
const int PLAYER_CAP					= 0x0060EAA0;

const int PLAYER_FLAGS					= 0x0060EA58; // Flag_t

const int PLAYER_FIST					= 0x0060EA78; // skills
const int PLAYER_CLUB					= 0x0060EA7C;
const int PLAYER_SWORD					= 0x0060EA80;
const int PLAYER_AXE					= 0x0060EA84;
const int PLAYER_DISTANCE				= 0x0060EA88;
const int PLAYER_SHIELDING				= 0x0060EA8C;
const int PLAYER_FISHING				= 0x0060EA90;

const int PLAYER_SLOT_HEAD				= 0x00616F88; // equipment
const int PLAYER_SLOT_NECKLACE			= 0x00616F94;
const int PLAYER_SLOT_BACKPACK			= 0x00616FA0;
const int PLAYER_SLOT_ARMOR				= 0x00616FAC;
const int PLAYER_SLOT_RIGHT				= 0x00616FB8;
const int PLAYER_SLOT_LEFT				= 0x00616FC4;
const int PLAYER_SLOT_LEGS				= 0x00616FD0;
const int PLAYER_SLOT_FEET				= 0x00616FDC;
const int PLAYER_SLOT_RING				= 0x00616FE8;
const int PLAYER_SLOT_AMMO				= 0x00616FF4;

const int PLAYER_SLOT_RIGHT_COUNT		= 0x00616FBC;
const int PLAYER_SLOT_LEFT_COUNT		= 0x00616FC8;
const int PLAYER_SLOT_AMMO_COUNT		= 0x00616FF8;

// left-click
const int CLICK_ID						= 0x00766E94;
const int CLICK_COUNT					= 0x00766E98;
const int CLICK_Z						= 0x00766E2C;

// see (inspect)
const int SEE_ID						= 0x00766EA0; // also 0x00766EAC
const int SEE_COUNT						= 0x00766EA4; // also 0x00766EB0
const int SEE_Z							= 0x00766E00; // also 0x00766E10
const int SEE_TEXT              		= 0x00768680; // first line of last seen text

// mouse cursor (fishing)
const int MOUSE_ID 						= 0x00766E58; // Mouse_t

// target
const int TARGET_ID						= 0x0060EA9C; // target attacked in battle list
const int TARGET_TYPE					= 0x0060EA9F; // CreatureType_t
const int TARGET_BATTLELIST_ID			= 0x0060EA94; // target highlighted in battle list
const int TARGET_BATTLELIST_TYPE		= 0x0060EA97; // CreatureType_t

/* constants */

// z-axis
const int Z_AXIS_DEFAULT = 7;			// default ground level

// statusbar
const int STATUSBAR_DURATION = 50;		// duration to show text

// level spy
const int LEVELSPY_NOP_DEFAULT = 49451;	// integer 2 bytes; 2BC1 SUB EAX, ECX

const int LEVELSPY_ABOVE_DEFAULT = 7;	// B8 07 MOV EAX, 7
const int LEVELSPY_BELOW_DEFAULT = 2;	// B8 02 MOV EAX, 2

const int LEVELSPY_MIN = 0;
const int LEVELSPY_MAX = 7;

// name spy
const int NAMESPY_NOP_DEFAULT = 19573;	// integer 2 bytes; 75 4C JNZ SHORT
const int NAMESPY_NOP2_DEFAULT = 17013;	// integer 2 bytes; 75 42 JNZ SHORT

#endif // #ifndef __TIBIACONST_H__
