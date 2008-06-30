#ifndef __TIBIACONST_H__
#define __TIBIACONST_H__

/*
Tibia Constants
by Evremonde
for Tibia 7.81
*/

/* memory addresses */

// xtea
const int XTEA_KEY						= 0x0074F1AC; // readBytes 16

// in-game
const int IS_INGAME						= 0x00751B78; // Ingame_t

// map
const int MAP_POINTER					= 0x0060A038; // readBytes 4

// battlelist
const int BATTLELIST_BEGIN				= 0x005FB994;
const int BATTLELIST_END				= 0x00601754;

// viplist
const int VIPLIST_BEGIN					= 0x005F9654;
const int VIPLIST_END					= 0x005FA784;

// container
const int CONTAINER_BEGIN 				= 0x006041F0;
const int CONTAINER_END					= 0x006060B0;

// hotkeys
const int HOTKEYS_BEGIN					= 0x0074F420;
const int HOTKEYS_END					= 0x0074F4AC;

// hotkeys text
const int HOTKEYS_TEXT_BEGIN			= 0x0074F4D8;
const int HOTKEYS_TEXT_END				= 0x007517D8;

// statusbar
const int STATUSBAR_TEXT				= 0x007531D8;
const int STATUSBAR_TIMER				= 0x007531D4;

// show names
const int SHOW_NAMES					= 0x004D7FFD; // writeNops 2
const int SHOW_NAMES_EX					= 0x004D8007; // writeNops 2

// player
const int PLAYER_X						= 0x00606B08; // coordinates
const int PLAYER_Y						= 0x00606B04;
const int PLAYER_Z						= 0x00606B00;

const int PLAYER_ID						= 0x005FB930; // unique id

const int PLAYER_HP						= 0x005FB92C; // statistics
const int PLAYER_HP_MAX					= 0x005FB928;
const int PLAYER_EXP					= 0x005FB924;
const int PLAYER_LEVEL					= 0x005FB920;
const int PLAYER_MAGIC_LEVEL			= 0x005FB91C;
const int PLAYER_LEVEL_PERCENT			= 0x005FB918;
const int PLAYER_MAGIC_LEVEL_PERCENT	= 0x005FB914;
const int PLAYER_MP						= 0x005FB910;
const int PLAYER_MP_MAX					= 0x005FB90C;
const int PLAYER_SOUL					= 0x005FB908;
const int PLAYER_STAMINA				= 0x005FB904; // in seconds
const int PLAYER_CAP					= 0x005FB900;

const int PLAYER_FLAGS					= 0x005FB8B8; // Flag_t

const int PLAYER_FIST					= 0x005FB8D8; // skills
const int PLAYER_CLUB					= 0x005FB8DC;
const int PLAYER_SWORD					= 0x005FB8E0;
const int PLAYER_AXE					= 0x005FB8E4;
const int PLAYER_DISTANCE				= 0x005FB8E8;
const int PLAYER_SHIELDING				= 0x005FB8EC;
const int PLAYER_FISHING				= 0x005FB8F0;

const int PLAYER_SLOT_HEAD				= 0x00604178; // equipment
const int PLAYER_SLOT_NECKLACE			= 0x00604184;
const int PLAYER_SLOT_BACKPACK			= 0x00604190;
const int PLAYER_SLOT_ARMOR				= 0x0060419C;
const int PLAYER_SLOT_RIGHT				= 0x006041A8;
const int PLAYER_SLOT_LEFT				= 0x006041B4;
const int PLAYER_SLOT_LEGS				= 0x006041C0;
const int PLAYER_SLOT_FEET				= 0x006041CC;
const int PLAYER_SLOT_RING				= 0x006041D8;
const int PLAYER_SLOT_AMMO				= 0x006041E4;

const int PLAYER_SLOT_RIGHT_COUNT		= 0x006041AC;
const int PLAYER_SLOT_LEFT_COUNT		= 0x006041B8;
const int PLAYER_SLOT_AMMO_COUNT		= 0x006041E8; 

// left-click
const int CLICK_ID						= 0x00751C14;
const int CLICK_COUNT					= 0x00751C18;
const int CLICK_Z						= 0x00751BAC;

// see (inspect)
const int SEE_ID						= 0x00751C20; // also 0x00751C2C
const int SEE_COUNT						= 0x00751C24; // also 0x00751C30
const int SEE_Z							= 0x00751B80; // also 0x00751B90

// mouse (fishing)
const int MOUSE_ID 						= 0x00751BD8; // Mouse_t

// target
const int TARGET_ID						= 0x005FB8FC;
const int TARGET_TYPE					= 0x005FB8FF; // CreatureType_t
const int TARGET_BATTLELIST_ID			= 0x005FB8F4;
const int TARGET_BATTLELIST_TYPE		= 0x005FB8F7; // CreatureType_t

/* constants */

// z-axis
const int Z_AXIS_DEFAULT = 7; // default ground level

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

// show names
const int SHOW_NAMES_DEFAULT	= 19573; // default values before nop; integer 2 bytes
const int SHOW_NAMES_EX_DEFAULT	= 17013;

/* enumerated constants */

// max
enum Max_t
{
	MAX_CREATURES		= 150,	// creatures in battle list
	MAX_VIP				= 100,	// friends in vip list
	MAX_MAP_TILES		= 2016,	// 18*14*8
	MAX_CONTAINERS		= 16,
	MAX_STACK			= 100,	// stacked or grouped item
	MAX_LIGHT			= 255,
	MAX_OUTFIT_COLORS	= 132,	// possible colors
	MAX_HOTKEYS			= 36,
};

// distances to step between addresses in memory
enum Step_t
{
	STEP_CREATURE		= 160,	// creatures in battle list
	STEP_VIP			= 44,	// friends in vip list
	STEP_MAP_TILE		= 172,	// tiles in map
	STEP_CONTAINER		= 492,	// containers in inventory
	STEP_CONTAINER_ITEM	= 12,	// items in containers
	STEP_HOTKEY			= 4,	// hotkeys by objects;
	STEP_HOTKEY_TEXT	= 256,	// hotkeys by text;
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

// offsets to variables from vip list step
enum OffsetVip_t
{
	OFFSET_VIP_ONLINE	= 30,
	OFFSET_VIP_ICON		= 36,
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
	OFFSET_MAP_COUNT			= 0,	// number of items existing on the tile
	OFFSET_MAP_TILE_ID			= 4,
	OFFSET_MAP_OBJECT_ID		= 16,	// creatures or items
	OFFSET_MAP_OBJECT_DATA		= 20,	// id OR stacked or grouped item count
	OFFSET_MAP_OBJECT_DATA_EX	= 28,
};

// distance to variables from hotkey step
enum OffsetHotkey_t
{
	OFFSET_HOTKEY_TYPE= -288, // Hotkey_t
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

// vip
enum Vip_t
{
	VIP_ONLINE_NO	= 0,
	VIP_ONLINE_YES	= 1,
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
	VIP_ICON_MONEY		= 9,	// dollar sign "$"
	VIP_ICON_IRON_CROSS	= 10,	// chopper symbol
};

// hotkeys
enum Hotkey_t
{
	HOTKEY_WITH_CROSSHAIRS	= 0,
	HOTKEY_ON_TARGET		= 1,
	HOTKEY_ON_SELF			= 2,
};

// hotkeys text
enum HotkeyText_t
{
	HOTKEY_TEXT_AUTOMATIC_NO	= 0,
	HOTKEY_TEXT_AUTOMATIC_YES	= 1,
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
	INGAME_NO	= 0,
	INGAME_YES	= 8,
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
	OUTFIT_INVISIBLE			= 0,    // stealth ring effect; also for item as outfit
	OUTFIT_SEPARATOR 			= 135,	// client crash

	// other
	OUTFIT_GM					= 75,	// gamemaster
	OUTFIT_ELF					= 159,
	OUTFIT_DWARF				= 160,	// santa claus
	OUTFIT_CULT					= 194,	// " of the cult
	OUTFIT_FROG					= 226,
	OUTFIT_FERUMBRAS			= 229,
	
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
	
	// client crash
	OUTFIT_INVALID_1            = 1,    // min
	OUTFIT_INVALID_2            = 135,  // outfit separator
	OUTFIT_INVALID_3			= 161,  // 3 -> 4
	OUTFIT_INVALID_4          	= 191,  // 3 -> 4
	OUTFIT_INVALID_5            = 230,	// max
};

// item ids
enum Item_t
{	
	ITEM_OTHER_WORMS				= 3492,

	ITEM_MONEY_GOLD					= 3031,
	ITEM_MONEY_PLATINUM				= 3035,
	ITEM_MONEY_CRYSTAL				= 3043,
	ITEM_MONEY_SCARAB				= 3042,

	ITEM_FLUID_VIAL					= 2874,

	ITEM_CONTAINER_BAG				= 2853,
	ITEM_CONTAINER_BACKPACK			= 2854,

	ITEM_UTILITY_ROPE				= 3003,
	ITEM_UTILITY_FISHING_ROD		= 3483,
	ITEM_UTILITY_PICK				= 3456,
	ITEM_UTILITY_SHOVEL				= 3457,
	ITEM_UTILITY_SPELL_BOOK			= 3059,

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
	TILE_LADDER_HOLE		= 411,
	TILE_TRANSPARENT		= 470,

	TILE_WATER_OLD			= 865,
	
	TILE_WATER_FISH_BEGIN	= 4597,
	TILE_WATER_FISH_END		= 4602,
	
	TILE_WATER_NOFISH_BEGIN	= 4603,
	TILE_WATER_NOFISH_END	= 4614,
};

// object ids
enum Object_t
{
	OBJECT_CREATURE				= 99,

	OBJECT_LOCKER				= 3499,
	OBJECT_DEPOT				= 3502,
	
	OBJECT_BUSH					= 3681,
	OBJECT_BUSH_2				= 3682,
	
	OBJECT_TREE_DEAD			= 3608,
	OBJECT_TREE_FIR				= 3614,
	OBJECT_TREE_SYCAMORE		= 3615,
	OBJECT_TREE_WILLOW			= 3616,
	OBJECT_TREE_PLUM			= 3617,
	OBJECT_TREE_MAPLE_RED		= 3618,
	OBJECT_TREE_PEAR			= 3619,
	OBJECT_TREE_MAPLE_YELLOW	= 3620,
	OBJECT_TREE_BEECH			= 3621,
	OBJECT_TREE_POPLAR			= 3622,
	OBJECT_TREE_DEAD_2			= 3623,
	OBJECT_TREE_DEAD_3			= 3624,
	OBJECT_TREE_DWARF			= 3625,
	OBJECT_TREE_FIR_SNOW		= 3609,
	OBJECT_TREE_MAGIC			= 3613,
	OBJECT_TREE_PINE			= 3626,
	OBJECT_TREE_DEAD_4			= 3632,
	OBJECT_TREE_DEAD_5			= 3633,
	OBJECT_TREE_DEAD_6			= 3634,
	OBJECT_TREE_OLD				= 3636,
	OBJECT_TREE_CACTUS			= 3637,
	OBJECT_TREE_CACTUS_2		= 3638,
	OBJECT_TREE_PALM			= 3639,
	OBJECT_TREE_PALM_2			= 3640,
	OBJECT_TREE_CACTUS_3		= 3641,
	OBJECT_TREE_CACTUS_4		= 3647,
	OBJECT_TREE_CACTUS_5		= 3649,
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
