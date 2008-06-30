#ifndef __TIBIACONST_H__
#define __TIBIACONST_H__

/*
Tibia Constants
by Evremonde
for Tibia 8.0
*/

/* memory addresses */

// in-game
const int IS_INGAME                     = 0x00766DF8; // Ingame_t

// current window
const int CURRENT_WINDOW				= 0x006198B4; // Window_t

// map
const int MAP_POINTER					= 0x0061E408; // readBytes 4

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
const int LEVELSPY_NOP					= 0x004C4320; // writeNops 2; 2BC1 SUB EAX, ECX

const int LEVELSPY_ABOVE				= 0x004C431C; // B8 07 MOV EAX, 7
const int LEVELSPY_BELOW				= 0x004C4324; // B8 02 MOV EAX, 2

// name spy
const int NAME_SPY						= 0x004DD2D7; // writeNops 2
const int NAME_SPY_EX					= 0x004DD2E1; // writeNops 2

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

// mouse (fishing)
const int MOUSE_ID 						= 0x00766E58; // Mouse_t

// target
const int TARGET_ID						= 0x0060EA9C;
const int TARGET_TYPE					= 0x0060EA9F; // CreatureType_t
const int TARGET_BATTLELIST_ID			= 0x0060EA94;
const int TARGET_BATTLELIST_TYPE		= 0x0060EA97; // CreatureType_t

/* constants */

// z-axis
const int Z_AXIS_DEFAULT = 7; // default ground level

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

// level spy
const int LEVELSPY_NOP_DEFAULT = 49451; // integer 2 bytes; 2BC1 SUB EAX, ECX

const int LEVELSPY_ABOVE_DEFAULT = 7; // B8 07 MOV EAX, 7
const int LEVELSPY_BELOW_DEFAULT = 2; // B8 02 MOV EAX, 2

const int LEVELSPY_MIN = 0;
const int LEVELSPY_MAX = 7;

// name spy
const int NAME_SPY_DEFAULT	= 19573; // default values before nop; integer 2 bytes
const int NAME_SPY_EX_DEFAULT = 17013;

/* enumerated constants */

// max
enum Max_t
{
	MAX_CREATURES		= 150,	// creatures in battle list
	MAX_MAP_TILES		= 2016,	// 18*14*8
	MAX_CONTAINERS		= 16,
	MAX_STACK			= 100,	// stacked or grouped item
	MAX_LIGHT			= 255,
	MAX_OUTFIT_COLORS	= 132,	// possible colors
};

// distances to step between addresses in memory
enum Step_t
{
	STEP_CREATURE		= 160,	// creatures in battle list
	STEP_MAP_TILE		= 172,	// tiles in map
	STEP_CONTAINER		= 492,	// containers in inventory
	STEP_CONTAINER_ITEM	= 12,	// items in containers
};

// offsets to variables from battle list step
enum OffsetCreature_t
{
	OFFSET_CREATURE_ID				= -4,
	OFFSET_CREATURE_TYPE			= -1,	// CreatureType_t
	OFFSET_CREATURE_NAME			= 0,
	OFFSET_CREATURE_X				= 32,
	OFFSET_CREATURE_Y				= 36,
	OFFSET_CREATURE_Z				= 40,
	OFFSET_CREATURE_IS_WALKING		= 72,
	OFFSET_CREATURE_DIRECTION		= 76,	// Direction_t
	OFFSET_CREATURE_OUTFIT			= 92,	// Outfit_t
	OFFSET_CREATURE_OUTFIT_HEAD		= 96,	// OutfitColor_t
	OFFSET_CREATURE_OUTFIT_BODY		= 100,	// OutfitColor_t
	OFFSET_CREATURE_OUTFIT_LEGS		= 104,	// OutfitColor_t
	OFFSET_CREATURE_OUTFIT_FEET		= 108,	// OutfitColor_t
	OFFSET_CREATURE_OUTFIT_ADDON	= 112,	// OutfitAddon_t
	OFFSET_CREATURE_LIGHT			= 116,	// LightLevel_t
	OFFSET_CREATURE_LIGHT_COLOR		= 120,	// LightColor_t
	OFFSET_CREATURE_HP_BAR			= 132,
	OFFSET_CREATURE_WALK_SPEED		= 136,
	OFFSET_CREATURE_IS_VISIBLE		= 140,
	OFFSET_CREATURE_SKULL			= 144,	// Skull_t
	OFFSET_CREATURE_PARTY			= 148,	// Party_t
};
	
// offsets to variables from container step
enum OffsetContainer_t
{
    OFFSET_CONTAINER_ID			= 4,
    OFFSET_CONTAINER_NAME		= 16,
    OFFSET_CONTAINER_VOLUME		= 48,	// max number of items in container
    OFFSET_CONTAINER_AMOUNT		= 56,	// current number of items in container
    OFFSET_CONTAINER_ITEM_ID	= 60,
    OFFSET_CONTAINER_ITEM_COUNT	= 64,	// stacked or grouped item count
};

// offsets to variables from map step
enum OffsetMap_t
{
	OFFSET_MAP_COUNT		= 0,	// number of items existing on the tile
	OFFSET_MAP_TILE_ID		= 4,
	OFFSET_MAP_OBJECT_ID	= 16,	// creatures or items
	OFFSET_MAP_OBJECT_DATA	= 20,	// id OR stacked or grouped item count
	OFFSET_MAP_OBJECT_STEP	= 4,	// next object id or data
	OFFSET_MAP_OBJECT_STEPS	= 6,	// number of times to step
};

// current window
enum Window_t
{
	WINDOW_MOVE_OBJECTS = 88, // "Move Objects"
};

// map
enum Map_t
{
	MAP_X	= 18,
	MAP_Y	= 14,
	MAP_Z	= 8,
};

// light radius
enum LightRadius_t
{
	LIGHT_NONE	= 0,
	LIGHT_TORCH	= 7,
	LIGHT_FULL	= 27,
};

// light colors
enum LightColor_t
{
	LIGHT_COLOR_NONE    = 0,
	LIGHT_COLOR_ORANGE	= 206, // default light color
	LIGHT_COLOR_WHITE	= 215,
};

// mouse (fishing)
enum Mouse_t
{
	MOUSE_NONE		= 0,
	MOUSE_LEFT		= 1,	// walk, etc
	MOUSE_RIGHT		= 2,	// use
	MOUSE_INSPECT	= 3,
	MOUSE_DRAG		= 6,
	MOUSE_USING		= 7,	// in-use, fishing, shooting rune
	MOUSE_HELP		= 10,	// client help
};

// directions
enum Direction_t
{
	DIRECTION_UP			= 0,
	DIRECTION_RIGHT			= 1,
	DIRECTION_DOWN			= 2,
	DIRECTION_LEFT			= 3,
	//DIRECTION_UP_RIGHT	= 4,
	//DIRECTION_DOWN_RIGHT	= 5,
	//DIRECTION_DOWN_LEFT	= 6,
	//DIRECTION_UP_LEFT		= 7,
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
	PARTY_NONE		= 0,
	PARTY_INVITER	= 1,
	PARTY_INVITEE	= 2,
	PARTY_MEMBER	= 3,
	PARTY_LEADER	= 4,
};

// slots (equipment)
enum Slot_t
{
	SLOT_HEAD		= 1,
	SLOT_NECKLACE	= 2,
	SLOT_BACKPACK	= 3,
	SLOT_ARMOR		= 4,
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
	CREATURE_TYPE_NPC		= 64,
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
	FLAG_SLOWED			= 32,	// paralyzed
	FLAG_HASTE			= 64, 
	FLAG_LOGOUT_BLOCK	= 128,	// swords
	FLAG_DROWNING		= 256,
};

// in-game
enum Ingame_t
{
	INGAME_NO		= 0,
	INGAME_ALMOST	= 4,
	INGAME_YES		= 8,
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
	OUTFIT_INVISIBLE			= 0,	// stealth ring effect; also for item as outfit
	
	// other
	OUTFIT_OTHER_HERO			= 73,
	OUTFIT_OTHER_GAMEMASTER		= 75,	// gamemaster
	OUTFIT_OTHER_PANDA			= 123,
	OUTFIT_OTHER_ELF			= 159,
	OUTFIT_OTHER_DWARF			= 160,	// santa claus
	OUTFIT_OTHER_CULT			= 194,
	OUTFIT_OTHER_FERUMBRAS		= 229,
	
	// male
	OUTFIT_MALE_OLD				= 127,	// no animations
	
	OUTFIT_MALE_DRUID			= 128,  // citizen
	OUTFIT_MALE_PALADIN			= 129,	// hunter
	OUTFIT_MALE_SORCERER		= 130,  // mage
	OUTFIT_MALE_KNIGHT			= 131,  // knight
	
	OUTFIT_MALE_PREMIUM_1		= 132,	// nobleman
	OUTFIT_MALE_PREMIUM_2		= 133,	// summoner
	OUTFIT_MALE_PREMIUM_3		= 134,	// warrior
	OUTFIT_MALE_PREMIUM_4		= 143,	// barbarian
	OUTFIT_MALE_PREMIUM_5		= 144,	// druid
	OUTFIT_MALE_PREMIUM_6		= 145,	// wizard
	OUTFIT_MALE_PREMIUM_7		= 146,	// oriental
	OUTFIT_MALE_PREMIUM_8		= 151,	// pirate
	OUTFIT_MALE_PREMIUM_9		= 152,	// assassin
	OUTFIT_MALE_PREMIUM_10		= 153,	// beggar
	OUTFIT_MALE_PREMIUM_11		= 154,	// shaman
	OUTFIT_MALE_PREMIUM_12		= 251,	// eskimo
	
	// female
	OUTFIT_FEMALE_OLD			= 126,	// no animations
	
	OUTFIT_FEMALE_DRUID			= 136,	// citizen
	OUTFIT_FEMALE_PALADIN		= 137,	// hunter
	OUTFIT_FEMALE_SORCERER		= 138,  // summoner
	OUTFIT_FEMALE_KNIGHT		= 139,	// knight
	
	OUTFIT_FEMALE_PREMIUM_1		= 140,	// nobleman
	OUTFIT_FEMALE_PREMIUM_2		= 141,	// mage
	OUTFIT_FEMALE_PREMIUM_3		= 142,	// warrior
	OUTFIT_FEMALE_PREMIUM_4		= 147,	// barbarian
	OUTFIT_FEMALE_PREMIUM_5		= 148,	// druid
	OUTFIT_FEMALE_PREMIUM_6		= 149,	// wizard
	OUTFIT_FEMALE_PREMIUM_7		= 150,	// oriental
	OUTFIT_FEMALE_PREMIUM_8		= 155,	// pirate
	OUTFIT_FEMALE_PREMIUM_9		= 156,	// assassin
	OUTFIT_FEMALE_PREMIUM_10	= 157,	// beggar
	OUTFIT_FEMALE_PREMIUM_11	= 158,	// shaman
	OUTFIT_FEMALE_PREMIUM_12	= 252,	// eskimo
	
	// invalid and cause crash
	// 1, 135, 161-191?, 256+
};

// item ids
enum Item_t
{	
	ITEM_OTHER_WORMS		= 3492,

	ITEM_MONEY_GOLD			= 3031,
	ITEM_MONEY_PLATINUM		= 3035,
	ITEM_MONEY_CRYSTAL		= 3043,
	ITEM_MONEY_SCARAB		= 3042,
};

// tile ids
enum Tile_t
{
	TILE_WATER_OLD			= 865,
	
	TILE_WATER_FISH_BEGIN	= 4597,
	TILE_WATER_FISH_END		= 4602,
	
	TILE_WATER_NOFISH_BEGIN	= 4603,
	TILE_WATER_NOFISH_END	= 4614,
};

// object ids
enum Object_t
{
	OBJECT_CREATURE	= 99,

	OBJECT_LOCKER	= 3499,
	OBJECT_DEPOT	= 3502,
	
	OBJECT_BUSH		= 3681,
	OBJECT_BUSH_2	= 3682,
};

/* structures */

// container of
struct ContainerOf
{
	int id;
	std::string name;
	int pos;
	int count;
	bool found;
};

// stack of
struct StackOf
{
	int fromContainer;
	int fromPos;
	int toContainer;
	int toPos;
	int itemCount;
	bool found;
};

#endif // #ifndef __TIBIACONST_H__
