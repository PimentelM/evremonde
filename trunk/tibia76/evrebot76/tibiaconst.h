#ifndef TIBIACONST_H
#define TIBIACONST_H

/*
Tibia Constants
by Evremonde
for client version 7.6
*/

// distance between characters in battlelist
const int DIST_CHARACTER		= 156;

// distance between tiles in map
const int DIST_TILE				= 168;

// distance between containers in inventory
const int DIST_CONTAINER		= 492;

// distance between items in containers
const int DIST_CONTAINER_ITEM	= 12;

// map tiles max
const int MAP_TILES_MAX			= 2016;

// stacked item max
const int STACK_MAX				= 100;

// statusbar
const int STATUS_DURATION		= 50;

// ranges for outfits
const int OUTFIT_COLOR_MAX		= 132;
const int OUTFIT_MALE_MIN		= 128;
const int OUTFIT_MALE_MAX		= 134;
const int OUTFIT_FEMALE_MIN		= 136;
const int OUTFIT_FEMALE_MAX		= 142;

// map
#define MAP_BEGIN			0x004AF478

// battlelist
#define BATTLELIST_BEGIN	0x004A12AC
#define BATTLELIST_END		0x004A6E14

// container
#define CONTAINER_BEGIN		0x004A9630

// reveal names
#define REVEAL_NAMES_1		0x004489A8 // jnz 2 NOPs
#define REVEAL_NAMES_2		0x004489B6 // jnz 2 NOPs

// permanent Light
#define PERMA_LIGHT			0x00448A83 // pointer

// statusbar
#define STATUS_TEXT			0x005F7058 // -4
#define STATUS_TIMER		0x005F7054

// player
#define PLAYER_NAME			0x005F3DA6

#define PLAYER_X			0x004ABF48 // -4
#define PLAYER_Y			0x004ABF44
#define PLAYER_Z			0x004ABF40

#define PLAYER_TO_X			0x004A1288 // -4
#define PLAYER_TO_Y			0x004A1284
#define PLAYER_TO_Z			0x004A1280

#define PLAYER_ID					0x004A1244 // -4
#define PLAYER_HP					0x004A1240
#define PLAYER_HP_MAX				0x004A123C
#define PLAYER_EXP					0x004A1238
#define PLAYER_LEVEL				0x004A1234
#define PLAYER_MAGIC_LEVEL			0x004A1230
#define PLAYER_LEVEL_PERCENT		0x004A122C
#define PLAYER_MAGIC_LEVEL_PERCENT	0x004A1228
#define PLAYER_MP					0x004A1224
#define PLAYER_MP_MAX				0x004A1220
#define PLAYER_SOUL					0x004A121C
#define PLAYER_CAP					0x004A1218

#define PLAYER_ICONS		0x004A11D0 // Icon_t

#define PLAYER_FISHING		0x004A1208 // -4
#define PLAYER_SHIELDING	0x004A1204
#define PLAYER_DISTANCE		0x004A1200
#define PLAYER_AXE			0x004A11FC
#define PLAYER_SWORD		0x004A11F8
#define PLAYER_CLUB			0x004A11F4
#define PLAYER_FIST			0x004A11F0

#define PLAYER_SLOT_HEAD		0x004A9570 // +12
#define PLAYER_SLOT_NECKLACE	0x004A9582
#define PLAYER_SLOT_BACKPACK	0x004A9594
#define PLAYER_SLOT_ARMOR		0x004A95A6
#define PLAYER_SLOT_RIGHT		0x004A95B8
#define PLAYER_SLOT_LEFT		0x004A95CA
#define PLAYER_SLOT_LEGS		0x004A95EE
#define PLAYER_SLOT_FEET		0x004A9600
#define PLAYER_SLOT_RING		0x004A9612
#define PLAYER_SLOT_AMMO		0x004A9624

#define PLAYER_SLOT_RIGHT_COUNT	0x004A95EC // SLOT+4
#define PLAYER_SLOT_LEFT_COUNT	0x004A95F8
#define PLAYER_SLOT_AMMO_COUNT	0x004A9628

// see
#define SEE_ID				0x005F6D64 // +4
#define SEE_COUNT			0x005F6D68

// use id
#define USE_ID				0x005F6D44 // Use_t

// target ids
#define TARGET_ID				0x004A1214
#define TARGET_BATTLELIST_ID	0x004A120C

// attack ids
#define ATTACK_ID				0x004A1217 // Attack_t
#define ATTACK_BATTLELIST_ID	0x004A120F // Attack_t

// distance to variables from character in battle list
enum DistPlayer_t
{
	DIST_ID			    = -4,	// creature id
	//DIST_NAME			= 0,	// creature name
	DIST_X			    = 32,
	DIST_Y			    = 36,
	DIST_Z			    = 40,
	DIST_IS_WALKING		= 72,
	DIST_DIRECTION		= 76,	// Direction_t
	DIST_OUTFIT		    = 92,	// Outfit_t
	DIST_OUTFIT_HEAD	= 96,	// OutfitColor_t
	DIST_OUTFIT_BODY	= 100,	// OutfitColor_t
	DIST_OUTFIT_LEGS	= 104,	// OutfitColor_t
	DIST_OUTFIT_FEET	= 108,	// OutfitColor_t
	DIST_LIGHT		    = 112,	// LightLevel_t
	DIST_LIGHT_COLOR    = 116,	// LightColor_t
	DIST_HP_BAR		    = 128,
	DIST_WALK_SPEED	    = 132,
	DIST_IS_VISIBLE     = 136,
	DIST_SKULL		    = 140,	// Skull_t
	DIST_PARTY		    = 144,	// Party_t
};

// distance to variables from container begin
enum DistContainer_t
{
    DIST_CONTAINER_ID            = 4,
    DIST_CONTAINER_NAME          = 16,
    DIST_CONTAINER_VOLUME        = 48,	// max number of items in container
    DIST_CONTAINER_AMOUNT        = 56,	// current number of items in container
    DIST_CONTAINER_ITEM_ID       = 60,
    DIST_CONTAINER_ITEM_COUNT    = 64,	// stacked item count
};

// distance to variables from map begin
enum DistMap_t
{
	DIST_MAP_COUNT			= 0,	// number of items stacked/displayed on the tile
	DIST_MAP_TILE_ID		= 4,
	DIST_MAP_OBJECT_ID		= 16,	// creatures or items
	DIST_MAP_OBJECT_DATA	= 20,	// id or stacked item count
	DIST_MAP_OBJECT_DATA_EX	= 28,
};

// light level
enum LightLevel_t
{
	LIGHT_NONE			= 0,
	LIGHT_TORCH			= 7,
	LIGHT_FULL			= 20,

	LIGHT_PERMA_NORMAL	= 139,
	LIGHT_PERMA_FULL	= 255,
};

// light color
enum LightColor_t
{
	LIGHT_COLOR_WHITE	= 215,
	LIGHT_COLOR_ORANGE	= 209,
};

// use
enum Use_t
{
	USE_NONE		= 0,
	USE_LEFT		= 1, // walk, etc
	USE_RIGHT		= 2, // use
	USE_INSPECT		= 3,
	USE_DRAG		= 6,
	USE_USING		= 7,
};

// map
enum Map_t
{
	MAP_TILE		= 0,
	MAP_OBJECT		= 1,
};

// rookgaard coordinates
enum RookgaardCoords_t
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
	PARTY_INVITEE   = 2,
	PARTY_MEMBER    = 3,
	PARTY_LEADER    = 4,
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
	FLUID_LIFE		= 10,
	FLUID_MANA		= 7,
};

// attack
enum Attack_t
{
	ATTACK_PLAYER	= 16,
	ATTACK_NPC		= 48,
	ATTACK_CREATURE	= 64,
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

// icons
enum Icon_t
{
	ICON_POISON			= 1,
	ICON_FIRE			= 2, 
	ICON_ENERGY			= 4, 
	ICON_DRUNK			= 8, 
	ICON_MANA_SHIELD	= 16, 
	ICON_PARALYZE		= 32, 
	ICON_HASTE			= 64, 
	ICON_SWORDS			= 128,
};

// outfit colors
enum OutfitColor_t
{
	OUTFIT_COLOR_RED		= 94,
	OUTFIT_COLOR_ORANGE		= 77,
	OUTFIT_COLOR_YELLOW		= 79,
	OUTFIT_COLOR_GREEN		= 82,
	OUTFIT_COLOR_BLUE		= 88,
	OUTFIT_COLOR_PURPLE		= 90,
	OUTFIT_COLOR_BROWN		= 116,
	OUTFIT_COLOR_BLACK		= 114,
	OUTFIT_COLOR_WHITE		= 0,
	OUTFIT_COLOR_PINK		= 91,
	OUTFIT_COLOR_GREY		= 57,
	OUTFIT_COLOR_PEACH		= 1,
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

	ITEM_FLUID_VIAL					= 2874,
	
	ITEM_RUNE_BLANK					= 3147,
	ITEM_RUNE_MAGIC_WALL			= 3180,
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
	ITEM_RUNE_SOUL_FIRE				= 3196,
	ITEM_RUNE_ENVENOM				= 3179,
};

// container of
struct ContainerOf
{
	int id;
	char *name;
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