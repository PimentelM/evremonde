#ifndef TIBIACONST_H
#define TIBIACONST_H

/*
Tibia Constants
by Evremonde
for client version 7.72
*/

// XTEA encryption
const int XTEA_KEY						= 0x00719D78;

// connection
const int IS_ONLINE						= 0x0071C588; // Connection_t

// battlelist
const int BATTLELIST_BEGIN				= 0x005C68B4;
const int BATTLELIST_END				= 0x005CC248;

// map
const int MAP_POINTER					= 0x005D4C20;

// container
const int CONTAINER_BEGIN 				= 0x005CEDD8;

// statusbar
const int STATUS_TEXT					= 0x0071DBE0; // -4
const int STATUS_TIMER					= 0x0071DBDC;

// player
const int PLAYER_NAME 					= 0x00000000;

const int PLAYER_X						= 0x005D16F0; // -4
const int PLAYER_Y						= 0x005D16EC;
const int PLAYER_Z						= 0x005D16E8;

const int PLAYER_TO_X					= 0x00000000; // -4
const int PLAYER_TO_Y					= 0x00000000;
const int PLAYER_TO_Z					= 0x00000000;

const int PLAYER_ID						= 0x005C684C; // -4
const int PLAYER_HP						= 0x005C6848;
const int PLAYER_HP_MAX					= 0x005C6844;
const int PLAYER_EXP					= 0x005C6840;
const int PLAYER_LEVEL					= 0x005C683C;
const int PLAYER_MAGIC_LEVEL			= 0x005C6838;
const int PLAYER_LEVEL_PERCENT			= 0x005C6834;
const int PLAYER_MAGIC_LEVEL_PERCENT	= 0x005C6830;
const int PLAYER_MP						= 0x005C682C;
const int PLAYER_MP_MAX					= 0x005C6828;
const int PLAYER_SOUL					= 0x005C6824;
const int PLAYER_CAP					= 0x005C6820;

const int PLAYER_FLAGS					= 0x005C67D8; // Flag_t

const int PLAYER_FISHING				= 0x00000000; // -4
const int PLAYER_SHIELDING				= 0x00000000;
const int PLAYER_DISTANCE				= 0x00000000;
const int PLAYER_AXE					= 0x00000000;
const int PLAYER_SWORD					= 0x00000000;
const int PLAYER_CLUB					= 0x00000000;
const int PLAYER_FIST					= 0x00000000;

const int PLAYER_SLOT_HEAD				= 0x00000000; // +12
const int PLAYER_SLOT_NECKLACE			= 0x00000000;
const int PLAYER_SLOT_BACKPACK			= 0x005CED78;
const int PLAYER_SLOT_ARMOR				= 0x00000000;
const int PLAYER_SLOT_RIGHT				= 0x005CED90;
const int PLAYER_SLOT_LEFT				= 0x005CED9C;
const int PLAYER_SLOT_LEGS				= 0x00000000;
const int PLAYER_SLOT_FEET				= 0x005CEDB4;
const int PLAYER_SLOT_RING				= 0x00000000;
const int PLAYER_SLOT_AMMO				= 0x005CEDCC;

const int PLAYER_SLOT_RIGHT_COUNT		= 0x005CED94; // SLOT+4
const int PLAYER_SLOT_LEFT_COUNT		= 0x005CEDA0;
const int PLAYER_SLOT_AMMO_COUNT		= 0x005CEDD0; 

// see (inspect)
const int SEE_ID						= 0x0071C630;
const int SEE_COUNT						= 0x0071C634; // = ID+4
const int SEE_Z							= 0x0071C590;

// use id (fishing)
const int USE_ID 						= 0x0071C5E8; // Use_t

// target ids
const int TARGET_ID						= 0x005C681C;
const int TARGET_BATTLELIST_ID			= 0x005C6814;

// target types
const int TARGET_TYPE					= 0x005C681F; // CreatureType_t
const int TARGET_BATTLELIST_TYPE		= 0x005C6817; // CreatureType_t

// z-axis
const int Z_AXIS_DEFAULT	= 7; // default ground level

// statusbar
const int STATUS_DURATION	= 50; // duration to show text

// max
enum Max_t
{
	MAX_CREATURES		= 150,	// creatures in battlelist
	MAX_MAP_TILES		= 2016,	// 8*14*18
	MAX_CONTAINERS		= 10,
	MAX_STACK			= 100,	// stacked item
	MAX_LIGHT			= 255,
	MAX_OUTFIT_COLORS	= 132,	// possible colors
};

// outfit ranges by sex
enum OutfitRange_t
{
	OUTFIT_MALE_MIN		= 128,
	OUTFIT_MALE_MAX		= 134,
	OUTFIT_FEMALE_MIN	= 136,
	OUTFIT_FEMALE_MAX	= 142,
};

// distances between addresses in memory
enum Dist_t
{
	DIST_CREATURE		= 156,	// creatures in battlelist
	DIST_MAP_TILE		= 172,	// tiles in map
	DIST_CONTAINER		= 492,	// containers in inventory
	DIST_CONTAINER_ITEM	= 12,	// items in containers
};

// distance to variables from battle list i
enum DistCreature_t
{
	DIST_CREATURE_ID			= -4,	// creature id
	DIST_CREATURE_TYPE			= -1,	// creature type
	DIST_CREATURE_NAME			= 0,	// creature name
	DIST_CREATURE_X				= 32,
	DIST_CREATURE_Y				= 36,
	DIST_CREATURE_Z				= 40,
	DIST_CREATURE_IS_WALKING	= 72,
	DIST_CREATURE_DIRECTION		= 76,	// Direction_t
	DIST_CREATURE_OUTFIT		= 92,	// Outfit_t
	DIST_CREATURE_OUTFIT_HEAD	= 96,	// OutfitColor_t
	DIST_CREATURE_OUTFIT_BODY	= 100,	// OutfitColor_t
	DIST_CREATURE_OUTFIT_LEGS	= 104,	// OutfitColor_t
	DIST_CREATURE_OUTFIT_FEET	= 108,	// OutfitColor_t
	DIST_CREATURE_LIGHT			= 112,	// LightLevel_t
	DIST_CREATURE_LIGHT_COLOR	= 116,	// LightColor_t
	DIST_CREATURE_HP_BAR		= 128,
	DIST_CREATURE_WALK_SPEED	= 132,
	DIST_CREATURE_IS_VISIBLE	= 136,
	DIST_CREATURE_SKULL			= 140,	// Skull_t
	DIST_CREATURE_PARTY			= 144,	// Party_t
};

// distance to variables from container i
enum DistContainer_t
{
    DIST_CONTAINER_ID			= 4,
    DIST_CONTAINER_NAME			= 16,
    DIST_CONTAINER_VOLUME		= 48,	// max number of items in container
    DIST_CONTAINER_AMOUNT		= 56,	// current number of items in container
    DIST_CONTAINER_ITEM_ID		= 60,
    DIST_CONTAINER_ITEM_COUNT	= 64,	// stacked item count
};

// distance to variables from map i
enum DistMap_t
{
	DIST_MAP_COUNT			= 0,	// number of items stacked/displayed on the tile
	DIST_MAP_TILE_ID		= 4,
	DIST_MAP_OBJECT_ID		= 16,	// creatures or items
	DIST_MAP_OBJECT_DATA	= 20,	// id or stacked item count
	DIST_MAP_OBJECT_DATA_EX	= 28,
};

// light level (radius)
enum LightLevel_t
{
	LIGHT_NONE	= 0,
	LIGHT_FS	= 1,
	LIGHT_TORCH	= 7,
	LIGHT_FULL	= 20,
};

// light color
enum LightColor_t
{
	LIGHT_COLOR_ORANGE	= 206,
	LIGHT_COLOR_WHITE	= 215,
};

// use (fishing)
enum Use_t
{
	USE_NONE	= 0,
	USE_LEFT	= 1,	// walk, etc
	USE_RIGHT	= 2,	// use
	USE_INSPECT	= 3,
	USE_DRAG	= 6,
	USE_USING	= 7,
};

// rookgaard
enum Rookgaard_t
{
	ROOKGAARD_WEST	= 30000,	// 31956 real (let it go west forever)
	ROOKGAARD_EAST	= 32200,	// 32189 real (perfect as is)
	ROOKGAARD_NORTH	= 31070,	// 32079 real (-1000 for underground caves)
	ROOKGAARD_SOUTH	= 33250,	// 32252 real (+1000 for underground caves)
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
	PARTY_NONE		= 0,
	PARTY_INVITER	= 1,
	PARTY_INVITEE	= 2,
	PARTY_MEMBER	= 3,
	PARTY_LEADER	= 4,
};

// slots
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
	FLUID_MANA	= 7,
	FLUID_LIFE	= 10,
};

// creature type
enum CreatureType_t
{
	CREATURE_TYPE_PLAYER	= 16,
	CREATURE_TYPE_NPC		= 48,
	CREATURE_TYPE_MONSTER	= 64,
};

// sex
enum Sex_t
{
	SEX_MALE		= 0,
	SEX_FEMALE		= 1,
	SEX_CREATURE	= 2,
};

// speak
enum Speak_t
{
	SPEAK_SAY		= 1,
	SPEAK_WHISPER	= 2,
	SPEAK_YELL		= 3,
};

// flags
enum Flags_t
{
	FLAG_POISON			= 1,
	FLAG_FIRE			= 2, 
	FLAG_ENERGY			= 4, 
	FLAG_DRUNK			= 8, 
	FLAG_MANA_SHIELD	= 16, 
	FLAG_PARALYZE		= 32, 
	FLAG_HASTE			= 64, 
	FLAG_SWORDS			= 128,
};

// connection
enum Connection_t
{
	CONNECTION_OFFLINE	= 0,
	CONNECTION_ONLINE	= 8,
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

// outfits
enum Outfit_t
{
	OUTFIT_MALE_DRUID			= 128,
	OUTFIT_MALE_PALADIN			= 129,
	OUTFIT_MALE_SORCERER		= 130,
	OUTFIT_MALE_KNIGHT			= 131,
	OUTFIT_MALE_PREMIUM_1		= 132,
	OUTFIT_MALE_PREMIUM_2		= 133,
	OUTFIT_MALE_PREMIUM_3		= 134,

	OUTFIT_FEMALE_DRUID			= 136,
	OUTFIT_FEMALE_PALADIN		= 137,
	OUTFIT_FEMALE_SORCERER		= 138,
	OUTFIT_FEMALE_KNIGHT		= 139,
	OUTFIT_FEMALE_PREMIUM_1		= 140,
	OUTFIT_FEMALE_PREMIUM_2		= 141,
	OUTFIT_FEMALE_PREMIUM_3		= 142,

	OUTFIT_OTHER_ELF			= 144,
	OUTFIT_OTHER_DWARF			= 160,	// santa claus

	OUTFIT_MALE_OLDCLIENT		= 126,	// no animations
	OUTFIT_FEMALE_OLDCLIENT		= 127,	// no animations

	OUTFIT_ORC_WARLORD			= 2,
	OUTFIT_WAR_WOLF				= 3,
	OUTFIT_ORC_RIDER			= 4,
	OUTFIT_ORC					= 5,
	OUTFIT_ORC_SHAMAN			= 6,
	OUTFIT_ORC_WARRIOR			= 7,
	OUTFIT_ORC_BERSERKER		= 8,
	OUTFIT_NECROMANCER			= 9,
	OUTFIT_YELLOW_BUTTERFLY		= 10,
	// CLIENT CRASH!			= 11,
	// CLIENT CRASH!			= 12,
	OUTFIT_BLACK_SHEEP			= 13,
	OUTFIT_SHEEP				= 14,
	OUTFIT_TROLL				= 15,
	OUTFIT_BEAR					= 16,
	OUTFIT_BEHOLDER				= 17,
	OUTFIT_GHOUL				= 18,
	OUTFIT_SLIME				= 19,
	// CLIENT CRASH!			= 20,
	OUTFIT_RAT					= 21,
	OUTFIT_CYCLOPS				= 22,
	OUTFIT_MINOTAUR_MAGE		= 23,
	OUTFIT_MINOTAUR_ARCHER		= 24,
	OUTFIT_MINOTAUR				= 25,
	OUTFIT_ROTWORM				= 26,
	OUTFIT_WOLF					= 27,
	OUTFIT_SNAKE				= 28,
	OUTFIT_MINOTAUR_GUARD		= 29,
	OUTFIT_SPIDER				= 30,
	OUTFIT_DEER					= 31,
	OUTFIT_DOG					= 32,
	OUTFIT_SKELETON				= 33,
	OUTFIT_DRAGON				= 34,
	OUTFIT_DEMON				= 35,
	OUTFIT_POISON_SPIDER		= 36,
	OUTFIT_DEMON_SKELETON		= 37,
	OUTFIT_GIANT_SPIDER			= 38,
	OUTFIT_DRAGON_LORD			= 39,
	OUTFIT_FIRE_DEVIL			= 40,
	OUTFIT_LION					= 41,
	OUTFIT_POLAR_BEAR			= 42,
	OUTFIT_SCORPION				= 43,
	OUTFIT_WASP					= 44,
	OUTFIT_BUG					= 45,
	// CLIENT CRASH!			= 46,
	// CLIENT CRASH!			= 47,
	OUTFIT_GHOST				= 48,
	OUTFIT_FIRE_ELEMENTAL		= 49,
	OUTFIT_ORC_SPEARMEN			= 50,
	OUTFIT_GREEN_DJINN			= 51,
	OUTFIT_WINTER_WOLF			= 52,
	OUTFIT_FROST_TROLL			= 53,
	OUTFIT_WITCH				= 54,
	OUTFIT_BEHEMOTH				= 55,
	OUTFIT_CAVE_RAT				= 56,
	OUTFIT_MONK					= 57,
	OUTFIT_PRIESTESS			= 58,
	OUTFIT_ORC_LEADER			= 59,
	OUTFIT_PIG					= 60,
	OUTFIT_GOBLIN				= 61,
	OUTFIT_ELF					= 62,
	OUTFIT_ELF_ARCANIST			= 63,
	OUTFIT_ELF_SCOUT			= 64,
	OUTFIT_MUMMY				= 65,
	OUTFIT_DWARF_GEOMANCER		= 66,
	OUTFIT_STONE_GOLEM			= 67,
	OUTFIT_VAMPIRE				= 68,
	OUTFIT_DWARF				= 69,
	OUTFIT_DWARF_GUARD			= 70,
	OUTFIT_DWARF_SOLDIER		= 71,
	// CLIENT CRASH!			= 72,
	OUTFIT_HERO					= 73,
	OUTFIT_RABBIT				= 74,
	OUTFIT_GM					= 75,	// gamemaster
	OUTFIT_SWAMP_TROLL			= 76,
	// CLIENT CRASH!			= 77,
	OUTFIT_BANSHEE				= 78,
	OUTFIT_ANCIENT_SCARAB		= 79,
	OUTFIT_BLUE_DJINN			= 80,
	OUTFIT_COBRA				= 81,
	OUTFIT_LARVA				= 82,
	OUTFIT_SCARAB				= 83,
	OUTFIT_PHARAOH				= 84,
	OUTFIT_PHARAOH_2			= 85,
	OUTFIT_PHARAOH_3			= 86,
	OUTFIT_PHARAOH_CLOTHES		= 87,
	OUTFIT_PHARAOH_CLOTHES_2	= 88,
	OUTFIT_PHARAOH_4			= 89,
	OUTFIT_PHARAOH_5			= 90,
	OUTFIT_PHARAOH_CLOTHES_3	= 91,
	OUTFIT_CHEST				= 92,
	OUTFIT_PYRAMID_PILLAR		= 93,
	OUTFIT_HYAENA				= 94,
	OUTFIT_GARGOYLE				= 95,
	OUTFIT_PYRAMID_PILLAR_2		= 96,
	OUTFIT_PYRAMID_PILLAR_3		= 97,
	OUTFIT_PYRAMID_PILLAR_4		= 98,
	OUTFIT_LICH					= 99,
	OUTFIT_CRYPT_SHAMBLER		= 100,
	OUTFIT_BONE_BEAST			= 101,
	OUTFIT_DEATHSLICER			= 102,
	OUTFIT_EFREET				= 103,
	OUTFIT_MARID				= 104,
	OUTFIT_BADGER				= 105,
	OUTFIT_SKUNK				= 106,
	OUTFIT_DEMON_2				= 107,
	OUTFIT_ELDER_BEHOLDER		= 108,
	OUTFIT_GAZER				= 109,
	OUTFIT_YETI					= 110,
	OUTFIT_CHICKEN				= 111,
	OUTFIT_CRAB					= 112,
	OUTFIT_LIZARD_TEMPLAR		= 113,
	OUTFIT_LIZARD_SENTINEL		= 114,
	OUTFIT_LIZARD_SNAKECHARMER	= 115,
	OUTFIT_KONGRA				= 116,
	OUTFIT_MERLKIN				= 117,
	OUTFIT_SIBANG				= 118,
	OUTFIT_CROCODILE			= 119,
	OUTFIT_CARNIPHILAS			= 120,
	OUTFIT_HYDRA				= 121,
	OUTFIT_BAT					= 122,
	OUTFIT_PANDA				= 123,
	OUTFIT_CENTIPEDE			= 124,
	OUTFIT_TIGER				= 125,
	OUTFIT_DARK_MONK			= 206,
	OUTFIT_ELEPHANT				= 211,
	OUTFIT_FLAMINGO				= 212,
	OUTFIT_BUTTERFLY			= 213,
	OUTFIT_DWORC_VOODOOMASTER	= 214,
	OUTFIT_DWORC_FLESHHUNTER	= 215,
	OUTFIT_DWORC_VENOMSNIPER	= 216,
	OUTFIT_PARROT				= 217,
	OUTFIT_TERROR_BIRD			= 218,
	OUTFIT_TARANTULA			= 219,
	OUTFIT_GIANT_BUTTERFLY		= 220,
	OUTFIT_SPIT_NETTLE			= 221,
	OUTFIT_BLUE_BUTTERFLY		= 227,
	OUTFIT_RED_BUTTERFLY		= 228,
};

// item ids
enum Item_t
{
	ITEM_CREATURE					= 99,

	ITEM_LOCKER						= 3499,
	ITEM_DEPOT						= 3502,

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
enum TileId_t
{
	TILE_ID_LADDER_HOLE	= 411,
	TILE_ID_TRANSPARENT	= 470,
};

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

#endif // #ifndef TIBIACONST_H
