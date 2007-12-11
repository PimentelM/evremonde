#ifndef __TIBIAENUM_H__
#define __TIBIAENUM_H__

/*
Tibia Enumerations
by Evremonde
for Tibia 8.0
*/

/* enumerated constants */

// max
enum Max_t
{
	MAX_CREATURES			= 150,	// creatures in battle list
	MAX_MAP_TILES			= 2016,	// 18*14*8, x*y*z
	MAX_MAP_OBJECT_STEPS	= 6,    // max steps for objects in map
	MAX_CONTAINERS			= 16,   // containers opened
	MAX_STACK				= 100,	// stacked or grouped item
	MAX_LIGHT				= 255,  // light radius
	MAX_OUTFIT_COLORS		= 132,	// possible colors
};

// distances to step between addresses in memory
enum Step_t
{
	STEP_CREATURE		= 160,	// creatures in battle list
	STEP_MAP_TILE		= 172,	// tiles in map
	STEP_MAP_OBJECT     = 4,    // objects in map
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
	OFFSET_CREATURE_BLACK_SQUARE    = 128,  // BlackSquare_t
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
    OFFSET_CONTAINER_VOLUME		= 48, // max number of items in container
    OFFSET_CONTAINER_HAS_PARENT = 52,
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
	OFFSET_WINDOW_LEFT      = 20,
	OFFSET_WINDOW_TOP       = 24,
	OFFSET_WINDOW_WIDTH     = 28,
	OFFSET_WINDOW_HEIGHT    = 32,
	OFFSET_WINDOW_CAPTION 	= 80,
	OFFSET_WINDOW_VISIBLE	= 88,
};

// in-game
enum Ingame_t
{
	INGAME_NO		= 0,
    INGAME_LOGIN	= 4,
	INGAME_YES		= 8,
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
	LIGHT_FULL	= 27, // 20+
};

// light colors
enum LightColor_t
{
	LIGHT_COLOR_NONE    = 0,
	LIGHT_COLOR_ORANGE	= 206, // default light color
	LIGHT_COLOR_WHITE	= 215,
};

// mouse cursor (fishing)
enum Mouse_t
{
	MOUSE_NONE		= 0,
	MOUSE_LEFT		= 1,	// walk, etc
	MOUSE_RIGHT		= 2,	// use
	MOUSE_INSPECT	= 3,    // see
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
	CREATURE_TYPE_NPC		= 64, // merchant, monster, etc
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
	OUTFIT_OTHER_GM				= 75,	// gamemaster
	OUTFIT_OTHER_PANDA			= 123,
	OUTFIT_OTHER_ELF			= 159,
	OUTFIT_OTHER_DWARF			= 160,	// santa claus
	OUTFIT_OTHER_CULT			= 194,
	OUTFIT_OTHER_FERUMBRAS		= 229,
	OUTFIT_OTHER_CM             = 266,	// community manager

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
	OUTFIT_MALE_PREMIUM_12		= 251,	// norseman

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
	OUTFIT_FEMALE_PREMIUM_12	= 252,	// norseman

	// outfits that don't exist and/or cause client crash
	// 1, 135, 161-191, 225, 267+
};

// item ids
enum Item_t
{
	ITEM_MONEY_GOLD					= 3031,
	ITEM_MONEY_PLATINUM				= 3035,
	ITEM_MONEY_CRYSTAL				= 3043,
	ITEM_MONEY_SCARAB				= 3042,
	
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
	OBJECT_CREATURE	= 99,

	OBJECT_LOCKER	= 3499,
	OBJECT_DEPOT	= 3502,

	OBJECT_BUSH		= 3681,
	OBJECT_BUSH_2	= 3682,
};

#endif // #ifndef __TIBIAENUM_H__
