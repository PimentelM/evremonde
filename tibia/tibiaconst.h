#ifndef _TIBIACONST_H_
#define _TIBIACONST_H_

/*
Tibia Constants
for Tibia 8.1
by Evremonde
*/

/* memory addresses */

// xtea key
const int XTEA_KEY						= 0x00768C7C; // 16 bytes

// rsa key
const int RSA_KEY						= 0x00597610; // 309 bytes; read until 0x00; 1247104594268279...

// connection status
const int CONNECTION_STATUS				= 0x0076C2C8; // ConnectionStatus_t

// map
const int MAP_POINTER					= 0x006234D8; // OffsetMap_t

// window (gui)
const int WINDOW_POINTER				= 0x0061E984; // OffsetWindow_t

// fps
const int FPS_POINTER					= 0x0076CE0C; // OffsetFps_t

// battlelist
const int BATTLELIST_BEGIN				= 0x00613BD0; // OffsetCreature_t
const int BATTLELIST_END				= 0x00619990; // BEGIN + (STEP_CREATURE * MAX_CREATURES)

// container
const int CONTAINER_BEGIN 				= 0x0061C0D0; // OffsetContainer_t
const int CONTAINER_END					= 0x0061DF90; // BEGIN + (STEP_CONTAINER * MAX_CONTAINERS)

// hotkeys
const int HOTKEY_OBJECT_BEGIN			= 0x00769208;
const int HOTKEY_OBJECT_END				= 0x00769298; // BEGIN + (STEP_HOTKEY_OBJECT * MAX_HOTKEYS)
const int HOTKEY_TEXT_BEGIN				= 0x007692C0;
const int HOTKEY_TEXT_END				= 0x0076B6C0; // BEGIN + (STEP_HOTKEY_TEXT * MAX_HOTKEYS)
const int HOTKEY_TYPE_BEGIN				= 0x007690E8; // HotkeyType_t
const int HOTKEY_TYPE_END				= 0x00769178; // BEGIN + (STEP_HOTKEY_TYPE * MAX_HOTKEYS)
const int HOTKEY_AUTOMATIC_BEGIN		= 0x00769298;
const int HOTKEY_AUTOMATIC_END			= 0x007692BC; // BEGIN + (STEP_HOTKEY_AUTOMATIC * MAX_HOTKEYS)

// vip
const int VIP_BEGIN						= 0x00611890; // OffsetVip_t
const int VIP_END						= 0x006129C0; // BEGIN + (STEP_VIP * MAX_VIP)

// statusbar
const int STATUSBAR_TEXT				= 0x0076D928; // 0052EAA3 | MOV EDX,0076D928
const int STATUSBAR_TIMER				= 0x0076D924; // timer for showing text; writeBytes 1 byte; 0052EABF | MOV DWORD PTR DS:[0076D924],EAX

// full light
const int LIGHT_NOP						= 0x004D7C99; // writeNops 2 bytes; 004D7C99 | 7E 05 | JLE SHORT
const int LIGHT_AMOUNT					= 0x004D7C9C; // LightAmount_t; writeBytes 1 byte; 4D7C9B | BB 80 | MOV EBX,80

// level spy
const int LEVELSPY_NOP					= 0x004C4FC0; // writeNops 2 bytes; 004C4FC0 | 2BC1 | SUB EAX,ECX

const int LEVELSPY_ABOVE				= 0x004C4FBC; // writeBytes 1 byte; 004C4FBB | B8 07 | MOV EAX,7
const int LEVELSPY_BELOW				= 0x004C4FC4; // writeBytes 1 byte; 004C4FC3 | B8 02 | MOV EAX,2

// name spy
const int NAMESPY_NOP					= 0x004DF469; // writeNops 2 bytes; 004DF469 | 75 4A | JNZ SHORT then 004DF46B | CALL 004C4FB0
const int NAMESPY_NOP2					= 0x004DF473; // writeNops 2 bytes; 004DF473 | 75 40 | JNZ SHORT

// show invisible creatures
const int INVISIBLE1					= 0x00453AF3; // writeBytes 1 byte; 00453AF3 | 72 0A | JB SHORT
const int INVISIBLE2					= 0x004DE734; // writeBytes 1 byte; 004DE734 | 75 09 | JNZ SHORT

// player
const int PLAYER_X						= 0x0061E9C8; // coordinates
const int PLAYER_Y						= 0x0061E9C4;
const int PLAYER_Z						= 0x0061E9C0; // 004C4FB0 | MOV ECX,DWORD PTR DS:[61E9C0] then 004C4FB6 | CMP ECX,7

const int PLAYER_GO_TO_X				= 0x00613BB4; // walking to coordinates
const int PLAYER_GO_TO_Y				= 0x00613BB0;
const int PLAYER_GO_TO_Z				= 0x00613BAC;

const int PLAYER_ID						= 0x00613B70; // unique id

const int PLAYER_HP						= 0x00613B6C; // statistics
const int PLAYER_HP_MAX					= 0x00613B68;
const int PLAYER_EXP					= 0x00613B64;
const int PLAYER_LEVEL					= 0x00613B60;
const int PLAYER_MAGIC_LEVEL			= 0x00613B5C;
const int PLAYER_LEVEL_PERCENT			= 0x00613B58;
const int PLAYER_MAGIC_LEVEL_PERCENT	= 0x00613B54;
const int PLAYER_MP						= 0x00613B50;
const int PLAYER_MP_MAX					= 0x00613B4C;
const int PLAYER_SOUL					= 0x00613B48;
const int PLAYER_STAMINA				= 0x00613B44; // in seconds
const int PLAYER_CAP					= 0x00613B40;

const int PLAYER_FLAGS					= 0x00613AF8; // Flag_t

const int PLAYER_FIST					= 0x00613B18; // skills
const int PLAYER_CLUB					= 0x00613B1C;
const int PLAYER_SWORD					= 0x00613B20;
const int PLAYER_AXE					= 0x00613B24;
const int PLAYER_DISTANCE				= 0x00613B28;
const int PLAYER_SHIELDING				= 0x00613B2C;
const int PLAYER_FISHING				= 0x00613B30;

const int PLAYER_FIST_PERCENT			= 0x00613AFC;
const int PLAYER_CLUB_PERCENT			= 0x00613B00;
const int PLAYER_SWORD_PERCENT			= 0x00613B04;
const int PLAYER_AXE_PERCENT			= 0x00613B08;
const int PLAYER_DISTANCE_PERCENT		= 0x00613B0C;
const int PLAYER_SHIELDING_PERCENT		= 0x00613B10;
const int PLAYER_FISHING_PERCENT		= 0x00613B14;

const int PLAYER_SLOT_HEAD				= 0x0061C058; // equipment
const int PLAYER_SLOT_NECK				= 0x0061C064;
const int PLAYER_SLOT_BACKPACK			= 0x0061C070;
const int PLAYER_SLOT_CHEST				= 0x0061C07C;
const int PLAYER_SLOT_RIGHT				= 0x0061C088;
const int PLAYER_SLOT_LEFT				= 0x0061C094;
const int PLAYER_SLOT_LEGS				= 0x0061C0A0;
const int PLAYER_SLOT_FEET				= 0x0061C0AC;
const int PLAYER_SLOT_RING				= 0x0061C0B8;
const int PLAYER_SLOT_AMMO				= 0x0061C0C4;

const int PLAYER_SLOT_RIGHT_COUNT		= 0x0061C08C; // stacked or grouped item count
const int PLAYER_SLOT_LEFT_COUNT		= 0x0061C098;
const int PLAYER_SLOT_AMMO_COUNT		= 0x0061C0C8;

// left-click
const int CLICK_ID						= 0x0076C364;
const int CLICK_COUNT					= 0x0076C368;
const int CLICK_X						= 0x0076C304;
const int CLICK_Y						= 0x0076C300;
const int CLICK_Z						= 0x0076C2FC;

// see (inspect)
const int SEE_ID						= 0x0076C370;
const int SEE_COUNT						= 0x0076C374;
const int SEE_X							= 0x0076C2D8;
const int SEE_Y							= 0x0076C2D4;
const int SEE_Z							= 0x0076C2D0;
const int SEE_TEXT						= 0x0076DB50; // first line of last seen text

// mouse cursor
const int MOUSE_ID						= 0x0076C328; // Mouse_t; 005089E0 | MOV EAX,DWORD PTR DS:[0076C328] then 005089E5 | CMP EAX,0A

// target
const int TARGET_ID						= 0x00613B3C; // target attacked in battle list
const int TARGET_TYPE					= 0x00613B3F; // CreatureType_t
const int TARGET_BATTLELIST_ID			= 0x00613B34; // target highlighted in battle list
const int TARGET_BATTLELIST_TYPE		= 0x00613B37; // CreatureType_t
const int TARGET_FOLLOW_ID				= 0x00613B38; // target followed in battle list

// mode
const int MODE_COMBAT					= 0x007690A4; // ModeCombat_t
const int MODE_FOLLOW					= 0x007690A0;
const int MODE_SECURE					= 0x0076909C;

// tile movement
const int TILES_TO_GO_CURRENT			= 0x00613B78; // current number of tiles needed to reach your destination
const int TILES_TO_GO_TOTAL				= 0x00613B7C; // total number of tiles needed to reach your destination

/* constants */

// z-axis
const int Z_AXIS_DEFAULT = 7; // default ground level

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

// full light
const int LIGHT_NOP_DEFAULT = 1406; // integer 2 bytes; 004D7C99 | 7E 05 | JLE SHORT

// level spy
const int LEVELSPY_NOP_DEFAULT = 49451; // integer 2 bytes; 004C4FC0 | 2BC1 | SUB EAX,ECX

const int LEVELSPY_ABOVE_DEFAULT = 7; // 004C4FBB | B8 07 | MOV EAX,7
const int LEVELSPY_BELOW_DEFAULT = 2; // 004C4FC3 | B8 02 | MOV EAX,2

const int LEVELSPY_MIN = 0;
const int LEVELSPY_MAX = 7;

// name spy
const int NAMESPY_NOP_DEFAULT = 19061; // integer 2 bytes; 004DF469 | 75 4A | JNZ SHORT then 004DF46B | CALL 004C4FB0
const int NAMESPY_NOP2_DEFAULT = 16501; // integer 2 bytes; 004DF473 | 75 40 | JNZ SHORT

// show invisible creatures
const int INVISIBLE1_DEFAULT = 114; // integer 1 byte; 00453AF3 | 72 0A | JB SHORT
const int INVISIBLE2_DEFAULT = 117; // integer 1 byte; 004DE734 | 75 09 | JNZ SHORT

const int INVISIBLE_PATCH = 235; // integer 1 byte; 235 = EB = JMP; change JB and JNZ to JMP

/* array constants */

// trees
const int ARRAY_TREES[] =
{
	3608, 3609, 3613, 3614, 3615, 3616, 3617, 3618, 3619, 3620, // object ids of trees
	3621, 3622, 3623, 3624, 3625, 3626, 3631, 3632, 3633, 3634,
	3635, 3636, 3637, 3638, 3639, 3640, 3641, 3647, 3649, 3687,
	3688, 3689, 3691, 3692, 3694, 3742, 3743, 3744, 3745, 3750,
	3751, 3752, 3753, 3754, 3755, 3756, 3757, 3758, 3759, 3760,
	3761, 3762, 3780, 3871, 3872, 3873, 3877, 3878, 3884, 3885,
	3899, 3901, 3902, 3903, 3905, 3908, 3909, 3910, 3911, 3920,
	3921, 5091, 5092, 5093, 5094, 5095, 5155, 5156, 5389, 5390,
	5391, 5392, 6094, 7020, 7021, 7022, 7023, 7024
};

/* enumerated constants */

// max
enum Max_t
{
	MAX_CREATURES			= 150,	// creatures in battle list
	MAX_MAP_TILES			= 2016,	// Map_t; 2016 = 18*14*8 = x*y*z
	MAX_MAP_TILE_OBJECTS	= 13,	// objects on a tile
	MAX_CONTAINERS			= 16,	// containers opened
	MAX_STACK				= 100,	// stacked or grouped item
	MAX_LIGHT				= 255,	// light amount and light radius
	MAX_OUTFIT_COLORS		= 132,	// possible colors
	MAX_HOTKEYS				= 36,
	MAX_VIP					= 100,	// people in vip list
};

// distances to step between addresses in memory
enum Step_t
{
	STEP_CREATURE			= 160,	// creatures in battle list
	STEP_MAP_TILE			= 172,	// tiles in map
	STEP_MAP_OBJECT			= 4,	// objects in map
	STEP_CONTAINER			= 492,	// containers in inventory
	STEP_CONTAINER_ITEM		= 12,	// items in containers
	STEP_VIP				= 44,	// people in vip list
	STEP_HOTKEY_OBJECT		= 4,	// hotkeys by object
	STEP_HOTKEY_TEXT		= 256,	// hotkeys by text
	STEP_HOTKEY_TYPE		= 4,	// hotkeys by type
	STEP_HOTKEY_AUTOMATIC	= 1,	// hotkeys by send automatically
};

// offsets to variables from battle list step
enum OffsetCreature_t
{
	OFFSET_CREATURE_ID				= 0,
	OFFSET_CREATURE_TYPE			= 3,	// CreatureType_t
	OFFSET_CREATURE_NAME			= 4,
	OFFSET_CREATURE_X				= 36,
	OFFSET_CREATURE_Y				= 40,
	OFFSET_CREATURE_Z				= 44,
	OFFSET_CREATURE_IS_WALKING		= 76,
	OFFSET_CREATURE_DIRECTION		= 80,	// Direction_t
	OFFSET_CREATURE_OUTFIT			= 96,	// Outfit_t
	OFFSET_CREATURE_OUTFIT_HEAD		= 100,	// OutfitColor_t
	OFFSET_CREATURE_OUTFIT_BODY		= 104,	// OutfitColor_t
	OFFSET_CREATURE_OUTFIT_LEGS		= 108,	// OutfitColor_t
	OFFSET_CREATURE_OUTFIT_FEET		= 112,	// OutfitColor_t
	OFFSET_CREATURE_OUTFIT_ADDON	= 116,	// OutfitAddon_t
	OFFSET_CREATURE_LIGHT_RADIUS	= 120,	// LightRadius_t
	OFFSET_CREATURE_LIGHT_COLOR		= 124,	// LightColor_t
	OFFSET_CREATURE_HP_BAR			= 136,	// hp percentage
	OFFSET_CREATURE_WALK_SPEED		= 140,
	OFFSET_CREATURE_IS_VISIBLE		= 144,	// creature is on screen
	OFFSET_CREATURE_SKULL			= 148,	// Skull_t
	OFFSET_CREATURE_PARTY			= 152,	// Party_t
};

// offsets to variables from container step
enum OffsetContainer_t
{
	OFFSET_CONTAINER_ID			= 4,
	OFFSET_CONTAINER_NAME		= 16,
	OFFSET_CONTAINER_VOLUME		= 48, // max number of items in container
	OFFSET_CONTAINER_HAS_PARENT	= 52,
	OFFSET_CONTAINER_AMOUNT		= 56, // current number of items in container
	OFFSET_CONTAINER_ITEM_ID	= 60,
	OFFSET_CONTAINER_ITEM_COUNT	= 64, // stacked or grouped item count
};

// offsets to variables from map step
enum OffsetMap_t
{
	OFFSET_MAP_COUNT		= 0,	// number of items existing on the tile
	OFFSET_MAP_TILE_ID		= 4,
	OFFSET_MAP_OBJECT_ID	= 16,	// creatures or items
	OFFSET_MAP_OBJECT_DATA	= 20,	// id OR stacked or grouped item count
};

// window (gui)
enum OffsetWindow_t
{
	OFFSET_WINDOW_LEFT		= 20,
	OFFSET_WINDOW_TOP		= 24,
	OFFSET_WINDOW_WIDTH		= 28,
	OFFSET_WINDOW_HEIGHT	= 32,
	OFFSET_WINDOW_CAPTION	= 80,
	OFFSET_WINDOW_VISIBLE	= 751,
};

// fps
enum OffsetFps_t
{
	OFFSET_FPS_LIMIT	= 88,
	OFFSET_FPS_CURRENT	= 96,
	OFFSET_FPS_LIMIT2	= 104, // read-only
};

// offsets to variables from hotkey object step
enum OffsetHotkeyObject_t
{
	OFFSET_HOTKEY_OBJECT_TYPE = -288, // HotkeyType_t
};

// offsets to variables from vip step
enum OffsetVip_t
{
	OFFSET_VIP_ID			= 0,
	OFFSET_VIP_NAME			= 4,
	OFFSET_VIP_IS_ONLINE	= 34,
	OFFSET_VIP_ICON			= 40, // VipIcon_t
};

// connection status
enum ConnectionStatus_t
{
	CONNECTION_STATUS_OFFLINE	= 0,
	CONNECTION_STATUS_ONLINE	= 8,
};

// map
enum Map_t
{
	MAP_X = 18,
	MAP_Y = 14,
	MAP_Z = 8,
};

// light amount
enum LightAmount_t
{
	LIGHT_AMOUNT_DEFAULT	= 128,
	LIGHT_AMOUNT_FULL		= 255,
};

// light radius
enum LightRadius_t
{
	LIGHT_RADIUS_NONE	= 0,
	LIGHT_RADIUS_TORCH	= 7,
	LIGHT_RADIUS_FULL	= 27, // 20+
};

// light colors
enum LightColor_t
{
	LIGHT_COLOR_NONE	= 0,
	LIGHT_COLOR_DEFAULT	= 206, // orange
	LIGHT_COLOR_WHITE	= 215,
};

// mouse cursor
enum Mouse_t
{
	MOUSE_NONE		= 0,
	MOUSE_LEFT		= 1,	// walk, etc
	MOUSE_RIGHT		= 2,	// use
	MOUSE_INSPECT	= 3,	// see
	MOUSE_DRAGGING	= 6,
	MOUSE_USING		= 7,	// in-use, fishing, shooting rune
	MOUSE_HELP		= 10,	// client help
};

// hotkey type
enum HotkeyType_t
{
	HOTKEY_TYPE_WITH_CROSSHAIRS	= 0,
	HOTKEY_TYPE_ON_TARGET		= 1,
	HOTKEY_TYPE_ON_SELF			= 2,
};

// vip icons
enum VipIcon_t
{
	VIP_ICON_NONE		= 0,
	VIP_ICON_HEART		= 1,
	VIP_ICON_SKULL		= 2,	// skull and crossbones
	VIP_ICON_LIGHTNING	= 3,	// bolt of lightning
	VIP_ICON_CROSSHAIR	= 4,
	VIP_ICON_STAR		= 5,	// yellow star
	VIP_ICON_YING_YANG	= 6,
	VIP_ICON_TRIANGLE	= 7,	// green triangle
	VIP_ICON_X			= 8,	// red "X"
	VIP_ICON_MONEY		= 9,	// currency "$"
	VIP_ICON_CROSS		= 10,	// green "+"
};

// directions
enum Direction_t
{
	DIRECTION_UP			= 0,
	DIRECTION_RIGHT			= 1,
	DIRECTION_DOWN			= 2,
	DIRECTION_LEFT			= 3,
	DIRECTION_UP_RIGHT		= 4,
	DIRECTION_DOWN_RIGHT	= 5,
	DIRECTION_DOWN_LEFT		= 6,
	DIRECTION_UP_LEFT		= 7,
};

// skulls
enum Skull_t
{
	SKULL_NONE		= 0,
	SKULL_YELLOW	= 1,
	SKULL_GREEN		= 2,
	SKULL_WHITE		= 3,
	SKULL_RED		= 4,
};

// party
enum Party_t
{
	PARTY_NONE					= 0,
	PARTY_INVITER				= 1,
	PARTY_INVITEE				= 2,
	PARTY_MEMBER				= 3,
	PARTY_LEADER				= 4,
	PARTY_MEMBER_SHARED_EXP		= 5,
	PARTY_LEADER_SHARED_EXP		= 6,
	PARTY_MEMBER_NO_SHARED_EXP	= 7,
	PARTY_LEADER_NO_SHARED_EXP	= 8,
};

// slots (equipment)
enum Slot_t
{
	SLOT_HEAD		= 1,
	SLOT_NECK		= 2,
	SLOT_BACKPACK	= 3,
	SLOT_CHEST		= 4,
	SLOT_RIGHT		= 5,
	SLOT_LEFT		= 6,
	SLOT_LEGS		= 7,
	SLOT_FEET		= 8,
	SLOT_RING		= 9,
	SLOT_AMMO		= 10,
};

// fluids
enum Fluid_t
{
	FLUID_EMPTY	= 0,
	FLUID_MANA	= 7,
	FLUID_LIFE	= 10,
};

// creature type
enum CreatureType_t
{
	CREATURE_TYPE_PLAYER	= 0,
	CREATURE_TYPE_NPC		= 64, // merchant, monster, etc
};

// mode combat
enum ModeCombat_t
{
	MODE_COMBAT_OFFENSIVE	= 1,
	MODE_COMBAT_BALANCED	= 2,
	MODE_COMBAT_DEFENSIVE	= 3,
};

// speak
enum Speak_t
{
	SPEAK_SAY		= 1,
	SPEAK_WHISPER	= 2,
	SPEAK_YELL		= 3,
};

// flags
enum Flag_t
{
	FLAG_POISONED		= 1,
	FLAG_BURNING		= 2,
	FLAG_ELECTRIFIED	= 4,
	FLAG_DRUNK			= 8,
	FLAG_MAGIC_SHIELD	= 16,	// mana shield
	FLAG_PARALYZED		= 32,
	FLAG_HASTE			= 64,
	FLAG_LOGOUT_BLOCK	= 128,	// swords
	FLAG_DROWNING		= 256,
	FLAG_FREEZING		= 512,
	FLAG_DAZZLED		= 1024,
	FLAG_CURSED			= 2048,
};

// outfit colors
enum OutfitColor_t
{
	OUTFIT_COLOR_RED	= 94,
	OUTFIT_COLOR_ORANGE	= 77,
	OUTFIT_COLOR_YELLOW	= 79,
	OUTFIT_COLOR_GREEN	= 82,
	OUTFIT_COLOR_BLUE	= 88,
	OUTFIT_COLOR_PURPLE	= 90,
	OUTFIT_COLOR_BROWN	= 116,
	OUTFIT_COLOR_BLACK	= 114,
	OUTFIT_COLOR_WHITE	= 0,
	OUTFIT_COLOR_PINK	= 91,
	OUTFIT_COLOR_GREY	= 57,
	OUTFIT_COLOR_PEACH	= 1,
};

// outfit addons
enum OutfitAddon_t
{
	OUTFIT_ADDON_NONE	= 0,
	OUTFIT_ADDON_1		= 1,
	OUTFIT_ADDON_2		= 2,
	OUTFIT_ADDON_BOTH	= 3,
};

// outfits
enum Outfit_t
{
	// outfits that don't exist and/or cause client crash
	// 1, 135, 161-191, 284+

	// misc
	OUTFIT_INVISIBLE			= 0,	// stealth ring effect; also for item as outfit
	OUTFIT_SWIMMING				= 267,	// water effect

	// other
	OUTFIT_OTHER_HERO			= 73,
	OUTFIT_OTHER_GM				= 75,	// gamemaster
	OUTFIT_OTHER_PANDA			= 123,
	OUTFIT_OTHER_ELF			= 159,
	OUTFIT_OTHER_DWARF			= 160,	// santa claus
	OUTFIT_OTHER_CULT			= 194,
	OUTFIT_OTHER_FERUMBRAS		= 229,
	OUTFIT_OTHER_CM				= 266,	// community manager

	// male
	OUTFIT_MALE_OLD				= 127,	// no animations

	OUTFIT_MALE_CITIZEN			= 128,	// druid
	OUTFIT_MALE_HUNTER			= 129,	// paladin
	OUTFIT_MALE_MAGE			= 130,	// sorcerer
	OUTFIT_MALE_KNIGHT			= 131,	// knight

	OUTFIT_MALE_NOBLEMAN		= 132,
	OUTFIT_MALE_SUMMONER		= 133,
	OUTFIT_MALE_WARRIOR			= 134,
	OUTFIT_MALE_BARBARIAN		= 143,
	OUTFIT_MALE_DRUID			= 144,
	OUTFIT_MALE_WIZARD			= 145,
	OUTFIT_MALE_ORIENTAL		= 146,
	OUTFIT_MALE_PIRATE			= 151,
	OUTFIT_MALE_ASSASSIN		= 152,
	OUTFIT_MALE_BEGGAR			= 153,
	OUTFIT_MALE_SHAMAN			= 154,
	OUTFIT_MALE_NORSEMAN		= 251,
	OUTFIT_MALE_NIGHTMARE		= 268,
	OUTFIT_MALE_JESTER			= 273,
	OUTFIT_MALE_BROTHERHOOD		= 278,

	// female
	OUTFIT_FEMALE_OLD			= 126,	// no animations

	OUTFIT_FEMALE_CITIZEN		= 136,	// druid
	OUTFIT_FEMALE_HUNTER		= 137,	// paladin
	OUTFIT_FEMALE_SUMMONER		= 138,  // sorcerer
	OUTFIT_FEMALE_KNIGHT		= 139,	// knight

	OUTFIT_FEMALE_NOBLEMAN		= 140,
	OUTFIT_FEMALE_MAGE			= 141,
	OUTFIT_FEMALE_WARRIOR		= 142,
	OUTFIT_FEMALE_BARBARIAN		= 147,
	OUTFIT_FEMALE_DRUID			= 148,
	OUTFIT_FEMALE_WIZARD		= 149,
	OUTFIT_FEMALE_ORIENTAL		= 150,
	OUTFIT_FEMALE_PIRATE		= 155,
	OUTFIT_FEMALE_ASSASSIN		= 156,
	OUTFIT_FEMALE_BEGGAR		= 157,
	OUTFIT_FEMALE_SHAMAN		= 158,
	OUTFIT_FEMALE_NORSEMAN		= 252,
	OUTFIT_FEMALE_NIGHTMARE		= 269,
	OUTFIT_FEMALE_JESTER		= 270,
	OUTFIT_FEMALE_BROTHERHOOD	= 279,
};

// item ids
enum Item_t
{
	ITEM_MONEY_GOLD					= 3031,
	ITEM_MONEY_PLATINUM				= 3035,
	ITEM_MONEY_CRYSTAL				= 3043,
	
	ITEM_FLUID_VIAL					= 2874,

	ITEM_CONTAINER_BAG				= 2853,
	ITEM_CONTAINER_BACKPACK			= 2854,

	ITEM_RUNE_BLANK					= 3147,
	ITEM_RUNE_MAGIC_WALL			= 3180,
	ITEM_RUNE_ANTIDOTE				= 3153,
	ITEM_RUNE_INTENSE_HEALING		= 3152,
	ITEM_RUNE_ULTIMATE_HEALING		= 3160,
	ITEM_RUNE_SUDDEN_DEATH			= 3155,
	ITEM_RUNE_EXPLOSION				= 3200,
	ITEM_RUNE_GREAT_FIREBALL		= 3191,
	ITEM_RUNE_LIGHT_MAGIC_MISSILE	= 3174,
	ITEM_RUNE_HEAVY_MAGIC_MISSILE	= 3198,
	ITEM_RUNE_FIRE_BOMB				= 3192,
	ITEM_RUNE_ENERGY_BOMB			= 3149,
	ITEM_RUNE_POISON_BOMB			= 3173,
	ITEM_RUNE_FIRE_WALL				= 3190,
	ITEM_RUNE_ENERGY_WALL			= 3166,
	ITEM_RUNE_POISON_WALL			= 3176,
	ITEM_RUNE_SOUL_FIRE				= 3195,
	ITEM_RUNE_ENVENOM				= 3179,
};

// tile ids
enum Tile_t
{
	TILE_TRANSPARENT			= 470,

	TILE_LAVA					= 727,

	TILE_WATER_FISH_BEGIN		= 4597,
	TILE_WATER_FISH_END			= 4602,

	TILE_WATER_NO_FISH_BEGIN	= 4603,
	TILE_WATER_NO_FISH_END		= 4614,
};

// object ids
enum Object_t
{
	OBJECT_CREATURE	= 99,

	OBJECT_LOCKER	= 3499,
	OBJECT_DEPOT	= 3502,

	OBJECT_BUSH		= 3681,
	OBJECT_BUSH2	= 3682,
};

/* structures */

struct Item
{
	int id;
	int count;
	int location;
	int container;
	std::string containerName;
	bool found;
};

struct Location
{
	int x;
	int y;
	int z;
};

#endif // #ifndef _TIBIACONST_H_
