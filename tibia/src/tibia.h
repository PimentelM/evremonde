#ifndef TIBIA_H
#define TIBIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

/* defines */

// version
#define TIBIA_VERSION "8.50"

// dll export
#define DLL_EXPORT __declspec(dllexport)

/* exported functions */

DLL_EXPORT void tibiaShowVersion();

DLL_EXPORT HWND tibiaGetClientWindow();
DLL_EXPORT DWORD tibiaGetProcessId();
DLL_EXPORT HANDLE tibiaGetProcessHandle();

DLL_EXPORT void tibiaWriteBytes(DWORD dwAddress, int value, int bytes);
DLL_EXPORT int tibiaReadBytes(DWORD dwAddress, int bytes);

DLL_EXPORT void tibiaWriteString(DWORD dwAddress, char* text);
DLL_EXPORT char* tibiaReadString(DWORD dwAddress);

DLL_EXPORT void tibiaWriteNops(DWORD dwAddress, int nops);

DLL_EXPORT void tibiaSetStatusbarTimer(int duration);
DLL_EXPORT int tibiaGetStatusbarTimer();

DLL_EXPORT void tibiaSetStatusbarText(char* text);
DLL_EXPORT char* tibiaGetStatusbarText();

/* client functions */

// move object
typedef void __stdcall _tibia_move_object(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _tibia_move_object *tibia_move_object = (_tibia_move_object *)0x00405220;
// arguments
// 1 = from X else 0xFFFF
// 2 = from Container 0x40 + i or from Slot 0x01 - 0x0A or from Y
// 3 = from Container Position or from Z
// 4 = Item ID
// 5 = from Container Position or stack when moved from ground
// 6 = to X else 0xFFFF
// 7 = to Container 0x40 + i or to Slot 0x01 - 0x0A or to Y
// 8 = to Container Position or to Z
// 9 = Item Count

// set statusbar text
typedef void __stdcall _tibia_set_statusbar_text(char* text, int shouldShowForever);
static _tibia_set_statusbar_text *tibia_set_statusbar_text = (_tibia_set_statusbar_text *)0x0053E500;

// trigger event
typedef void __stdcall _tibia_trigger_event(int event, int maw, int mow);
static _tibia_trigger_event *tibia_trigger_event = (_tibia_trigger_event *)0x00519770;

// trigger event ex
typedef void __stdcall _tibia_trigger_event_ex(int event, char* maw, char* mow);
static _tibia_trigger_event_ex *tibia_trigger_event_ex = (_tibia_trigger_event_ex *)0x00519770;

// use object
typedef void __stdcall _tibia_use_object(int fromX, int fromY, int fromZ, int id, int stack); //(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _tibia_use_object *tibia_use_object = (_tibia_use_object *)0x00406240;

/* memory addresses and structures */

// action state
#define TIBIA_ACTION_STATE 0x0078F5F8 // tibiaActionState
DWORD *_TIBIA_ACTION_STATE = (DWORD *)TIBIA_ACTION_STATE;

// container
#define TIBIA_CONTAINER_BEGIN 0x0063F388
#define TIBIA_CONTAINER_END   0x00641248 // = TIBIA_CONTAINER_BEGIN + (TIBIA_STEP_CONTAINER * TIBIA_MAX_CONTAINERS)

typedef struct
{
    unsigned int id;
    unsigned int count;
    unsigned int unknown;
} tibiaContainerItem_s;

typedef struct
{
    unsigned int isOpen;
    unsigned int id;
    unsigned int unknown1;
    unsigned int unknown2;
    char name[32];
    unsigned int volume;
    unsigned int hasParent; // isChild
    unsigned int amount;
    tibiaContainerItem_s item[36];
} tibiaContainer_s;

// inventory
typedef struct
{
    tibiaContainer_s container[16];
} tibiaInventory_s;

tibiaInventory_s *_TIBIA_INVENTORY = (tibiaInventory_s *)TIBIA_CONTAINER_BEGIN;

// player
#define TIBIA_PLAYER_CAP 0x00632EA0 // TIBIA_PLAYER_CAP / 100 = float playerCap
DWORD *_TIBIA_PLAYER_CAP = (DWORD *)TIBIA_PLAYER_CAP;

#define TIBIA_PLAYER_Z   0x00641C78
#define TIBIA_PLAYER_Y   0x00641C7C // TIBIA_PLAYER_Z + 4
#define TIBIA_PLAYER_X   0x00641C80 // TIBIA_PLAYER_Z + 8
DWORD *_TIBIA_PLAYER_Z = (DWORD *)TIBIA_PLAYER_Z;
DWORD *_TIBIA_PLAYER_Y = (DWORD *)TIBIA_PLAYER_Y; 
DWORD *_TIBIA_PLAYER_X = (DWORD *)TIBIA_PLAYER_X;

// see
#define TIBIA_SEE_ID   0x0078F640
#define TIBIA_SEE_TEXT 0x00791640
DWORD *_TIBIA_SEE_ID   = (DWORD *)TIBIA_SEE_ID;
char  *_TIBIA_SEE_TEXT = (char *) TIBIA_SEE_TEXT;

// statusbar
#define TIBIA_STATUSBAR_TIMER 0x00791414 // TIBIA_STATUSBAR_TEXT - 4
#define TIBIA_STATUSBAR_TEXT  0x00791418
DWORD *_TIBIA_STATUSBAR_TIMER = (DWORD *)TIBIA_STATUSBAR_TIMER;
char  *_TIBIA_STATUSBAR_TEXT  = (char *) TIBIA_STATUSBAR_TEXT;

/* constants */

// container
const int TIBIA_FIRST_CONTAINER = 64; // = 0x40

const int TIBIA_STEP_CONTAINER      = 492; // containers in inventory
const int TIBIA_STEP_CONTAINER_ITEM = 12;  // items in containers

const int TIBIA_MAX_CONTAINERS      = 16; // number of containers open in client
const int TIBIA_MAX_CONTAINER_ITEMS = 36; // number of items in a container

// item
const int TIBIA_MAX_ITEM_STACK = 100; // max stacked item count

// statusbar
const int TIBIA_STATUSBAR_TEXT_DEFAULT_DURATION = 50; // duration to show statusbar text

/* enumerated constants */

// action state
typedef enum
{
    TIBIA_ACTION_STATE_NONE                 = 0,
    TIBIA_ACTION_STATE_LEFT_CLICK           = 1,  // left-click to walk or to use the client interface
    TIBIA_ACTION_STATE_RIGHT_CLICK          = 2,  // right-click to use an object such as a torch or an apple
    TIBIA_ACTION_STATE_INSPECT_OBJECT       = 3,  // left-click + right-click to see or inspect an object
//                                          = 4,
//                                          = 5,
    TIBIA_ACTION_STATE_MOVE_OBJECT          = 6,  // dragging an object to move it to a new location
    TIBIA_ACTION_STATE_USE_OBJECT           = 7,  // using an object such as a rope, a shovel, a fishing rod, or a rune
    TIBIA_ACTION_STATE_SELECT_HOTKEY_OBJECT = 8,  // selecting an object to bind to a hotkey from the "Hotkey Options" window
    TIBIA_ACTION_STATE_TRADE_OBJECT         = 9,  // using "Trade with..." on an object to select a player with whom to trade
    TIBIA_ACTION_STATE_CLIENT_HELP          = 10, // client mouse over tooltip help
    TIBIA_ACTION_STATE_OPEN_DIALOG_WINDOW   = 11, // opening a dialog window such as the "Options" window, "Select Outfit" window, or "Move Objects" window
    TIBIA_ACTION_STATE_POPUP_MENU           = 12  // showing a popup menu with options such as "Invite to Party", "Set Outfit", "Copy Name", or "Set Mark"
} tibiaActionState_t;

// creature
typedef enum
{
    TIBIA_CREATURE_PLAYER = 0,
    TIBIA_CREATURE_TARGET = 1,
    TIBIA_CREATURE_NPC    = 64 // npc, merchant, quest giver or monster
} tibiaCreature_t;

// direction type
typedef enum
{
    TIBIA_DIRECTION_UP         = 0,
    TIBIA_DIRECTION_RIGHT      = 1,
    TIBIA_DIRECTION_DOWN       = 2,
    TIBIA_DIRECTION_LEFT       = 3,
//                             = 4,
    TIBIA_DIRECTION_UP_RIGHT   = 5,
    TIBIA_DIRECTION_DOWN_RIGHT = 6,
    TIBIA_DIRECTION_DOWN_LEFT  = 7,
    TIBIA_DIRECTION_UP_LEFT    = 8,
} tibiaDirection_t;

// equipment slot
typedef enum
{
    TIBIA_EQUIPMENT_SLOT_NULL     = 0,
    TIBIA_EQUIPMENT_SLOT_HEAD     = 1,
    TIBIA_EQUIPMENT_SLOT_NECK     = 2,
    TIBIA_EQUIPMENT_SLOT_BACKPACK = 3, // container
    TIBIA_EQUIPMENT_SLOT_BODY     = 4,
    TIBIA_EQUIPMENT_SLOT_RIGHT    = 5,
    TIBIA_EQUIPMENT_SLOT_LEFT     = 6,
    TIBIA_EQUIPMENT_SLOT_LEGS     = 7,
    TIBIA_EQUIPMENT_SLOT_FEET     = 8,
    TIBIA_EQUIPMENT_SLOT_RING     = 9,
    TIBIA_EQUIPMENT_SLOT_AMMO     = 10,
    TIBIA_EQUIPMENT_SLOT_FIRST    = TIBIA_EQUIPMENT_SLOT_HEAD,
    TIBIA_EQUIPMENT_SLOT_LAST     = TIBIA_EQUIPMENT_SLOT_AMMO
} tibiaEquipmentSlot_t;

// flags
typedef enum
{
    TIBIA_FLAGS_NONE                                   = 0,
    TIBIA_FLAGS_POISONED                               = 1,
    TIBIA_FLAGS_BURNING                                = 2,
    TIBIA_FLAGS_ELECTRIFIED                            = 4,
    TIBIA_FLAGS_DRUNK                                  = 8,
    TIBIA_FLAGS_PROTECTED_BY_MAGIC_SHIELD              = 16,
    TIBIA_FLAGS_PARALYSED                              = 32,
    TIBIA_FLAGS_PARALYZED                              = TIBIA_FLAGS_PARALYSED,
    TIBIA_FLAGS_HASTED                                 = 64,
    TIBIA_FLAGS_IN_BATTLE                              = 128,
    TIBIA_FLAGS_DROWNING                               = 256,
    TIBIA_FLAGS_FREEZING                               = 512,
    TIBIA_FLAGS_DAZZLED                                = 1024,
    TIBIA_FLAGS_CURSED                                 = 2048,
    TIBIA_FLAGS_STRENGTHENED                           = 4096,
    TIBIA_FLAGS_CANNOT_LOGOUT_OR_ENTER_PROTECTION_ZONE = 8192,
    TIBIA_FLAGS_WITHIN_PROTECTION_ZONE                 = 16384
} tibiaFlags_t;

// hotkey
typedef enum
{
    TIBIA_HOTKEY_WITH_CROSSHAIRS = 0,
    TIBIA_HOTKEY_USE_ON_TARGET   = 1,
    TIBIA_HOTKEY_USE_ON_SELF     = 2
} tibiaHotkey_t;

// item
typedef enum
{
    TIBIA_ITEM_CONTAINER_BAG      = 2853,
    TIBIA_ITEM_CONTAINER_BACKPACK = 2854,

    TIBIA_ITEM_FLUID_EMPTY_VIAL   = 2874,

    TIBIA_ITEM_CURRENCY_GOLD      = 3031,
    TIBIA_ITEM_CURRENCY_PLATINUM  = 3035,
    TIBIA_ITEM_CURRENCY_CRYSTAL   = 3043,
} tibiaItem_t;

// light amount
typedef enum
{
    TIBIA_LIGHT_AMOUNT_DEFAULT = 128,
    TIBIA_LIGHT_AMOUNT_FULL    = 255
} tibiaLightAmount_t;

// light color
typedef enum
{
    TIBIA_LIGHT_COLOR_NONE    = 0,
    TIBIA_LIGHT_COLOR_DEFAULT = 206,
    TIBIA_LIGHT_COLOR_ORANGE  = TIBIA_LIGHT_COLOR_DEFAULT,
    TIBIA_LIGHT_COLOR_WHITE   = 215
} tibiaLightColor_t;

// light radius
typedef enum
{
    TIBIA_LIGHT_RADIUS_NONE  = 0,
    TIBIA_LIGHT_RADIUS_TORCH = 7,
    TIBIA_LIGHT_RADIUS_FULL  = 20
} tibiaLightRadius_t;

// login status
typedef enum
{
    TIBIA_LOGIN_STATUS_LOGGED_OUT = 0,
    TIBIA_LOGIN_STATUS_LOGGING_IN = 6,
    TIBIA_LOGIN_STATUS_LOGGED_IN  = 8
} tibiaLoginStatus_t;

// object
typedef enum
{
    TIBIA_OBJECT_CREATRUE = 99,

    TIBIA_OBJECT_LOCKER   = 3499,
    TIBIA_OBJECT_DEPOT    = 3502
} tibiaObject_t;

// offset container
typedef enum
{
    TIBIA_OFFSET_CONTAINER_IS_OPEN    = 0,
    TIBIA_OFFSET_CONTAINER_ID         = 4,
    TIBIA_OFFSET_CONTAINER_NAME       = 16,
    TIBIA_OFFSET_CONTAINER_VOLUME     = 48, // max number of items in the container
    TIBIA_OFFSET_CONTAINER_HAS_PARENT = 52, // the container is a container inside another container
    TIBIA_OFFSET_CONTAINER_IS_CHILD   = TIBIA_OFFSET_CONTAINER_HAS_PARENT,
    TIBIA_OFFSET_CONTAINER_AMOUNT     = 56, // current number of items in the container
    TIBIA_OFFSET_CONTAINER_ITEM_ID    = 60,
    TIBIA_OFFSET_CONTAINER_ITEM_COUNT = 64  // stacked item count
} tibiaOffsetContainer_t;

// offset creature
typedef enum
{
    TIBIA_OFFSET_CREATURE_ID                 = 0,
    TIBIA_OFFSET_CREATURE_TYPE               = 3,   // tibiaCreature_t
    TIBIA_OFFSET_CREATURE_NAME               = 4,
    TIBIA_OFFSET_CREATURE_X                  = 36,
    TIBIA_OFFSET_CREATURE_Y                  = 40,
    TIBIA_OFFSET_CREATURE_Z                  = 44,
    TIBIA_OFFSET_CREATURE_IS_WALKING         = 76,
    TIBIA_OFFSET_CREATURE_DIRECTION          = 80,  // tibiaDirection_t
    TIBIA_OFFSET_CREATURE_OUTFIT             = 96,  // tibiaOutfit_t
    TIBIA_OFFSET_CREATURE_OUTFIT_COLOR_HEAD  = 100, // tibiaOutfitColor_t
    TIBIA_OFFSET_CREATURE_OUTFIT_COLOR_BODY  = 104, // tibiaOutfitColor_t
    TIBIA_OFFSET_CREATURE_OUTFIT_COLOR_LEGS  = 108, // tibiaOutfitColor_t
    TIBIA_OFFSET_CREATURE_OUTFIT_COLOR_FEET  = 112, // tibiaOutfitColor_t
    TIBIA_OFFSET_CREATURE_OUTFIT_ADDON       = 116, // tibiaOutfitAddon_t
    TIBIA_OFFSET_CREATURE_LIGHT_RADIUS       = 120, // tibiaLightRadius_t
    TIBIA_OFFSET_CREATURE_LIGHT_COLOR        = 124, // tibiaLightColor_t
    TIBIA_OFFSET_CREATURE_HP_PERCENT         = 136, // health bar percentage
    TIBIA_OFFSET_CREATURE_WALK_SPEED         = 140,
    TIBIA_OFFSET_CREATURE_IS_VIEWABLE        = 144, // creature is within viewable distance
    TIBIA_OFFSET_CREATURE_SKULL              = 148, // tibiaSkull_t
    TIBIA_OFFSET_CREATURE_PARTY              = 152  // tibiaParty_t
} tibiaOffsetCreature_t;

// offset vip
typedef enum
{
    TIBIA_OFFSET_VIP_ID        = 0,
    TIBIA_OFFSET_VIP_NAME      = 4,
    TIBIA_OFFSET_VIP_IS_ONLINE = 34,
    TIBIA_OFFSET_VIP_ICON      = 40 // tibiaVipIcon
} tibiaOffsetVip_t;

// outfit
typedef enum
{
    TIBIA_OUTFIT_INVISIBLE                   = 0,   // invisible or stealth ring effect
    TIBIA_OUTFIT_IS_ITEM                     = TIBIA_OUTFIT_INVISIBLE, // outfit as item
    
    TIBIA_OUTFIT_SWIMMING                    = 267, // swimming in water

    TIBIA_OUTFIT_GAMEMASTER_VOLUNTARY        = 75,
    TIBIA_OUTFIT_GAMEMASTER_CUSTOMER_SUPPORT = 266,
    TIBIA_OUTFIT_COMMUNITY_MANAGER           = 302,

    TIBIA_OUTFIT_MALE_OLD_CLIENT             = 127, // old client, no animations

    TIBIA_OUTFIT_MALE_CITIZEN                = 128, // druid
    TIBIA_OUTFIT_MALE_HUNTER                 = 129, // paladin
    TIBIA_OUTFIT_MALE_MAGE                   = 130, // sorcerer
    TIBIA_OUTFIT_MALE_KNIGHT                 = 131, // knight

    TIBIA_OUTFIT_MALE_NOBLEMAN               = 132,
    TIBIA_OUTFIT_MALE_SUMMONER               = 133,
    TIBIA_OUTFIT_MALE_WARRIOR                = 134,
    TIBIA_OUTFIT_MALE_BARBARIAN              = 143,
    TIBIA_OUTFIT_MALE_DRUID                  = 144,
    TIBIA_OUTFIT_MALE_WIZARD                 = 145,
    TIBIA_OUTFIT_MALE_ORIENTAL               = 146,
    TIBIA_OUTFIT_MALE_PIRATE                 = 151,
    TIBIA_OUTFIT_MALE_ASSASSIN               = 152,
    TIBIA_OUTFIT_MALE_BEGGAR                 = 153,
    TIBIA_OUTFIT_MALE_SHAMAN                 = 154,
    TIBIA_OUTFIT_MALE_NORSEMAN               = 251,
    TIBIA_OUTFIT_MALE_NIGHTMARE              = 268,
    TIBIA_OUTFIT_MALE_JESTER                 = 273,
    TIBIA_OUTFIT_MALE_BROTHERHOOD            = 278,
    TIBIA_OUTFIT_MALE_DEMONHUNTER            = 289,
    TIBIA_OUTFIT_MALE_YALAHARIAN             = 325,
    TIBIA_OUTFIT_MALE_WEDDING                = 328,

    TIBIA_OUTFIT_FEMALE_OLD_CLIENT           = 126, // old client, no animations

    TIBIA_OUTFIT_FEMALE_CITIZEN              = 136, // druid
    TIBIA_OUTFIT_FEMALE_HUNTER               = 137, // paladin
    TIBIA_OUTFIT_FEMALE_SUMMONER             = 138, // sorcerer
    TIBIA_OUTFIT_FEMALE_KNIGHT               = 139, // knight

    TIBIA_OUTFIT_FEMALE_NOBLEMAN             = 140,
    TIBIA_OUTFIT_FEMALE_MAGE                 = 141,
    TIBIA_OUTFIT_FEMALE_WARRIOR              = 142,
    TIBIA_OUTFIT_FEMALE_BARBARIAN            = 147,
    TIBIA_OUTFIT_FEMALE_DRUID                = 148,
    TIBIA_OUTFIT_FEMALE_WIZARD               = 149,
    TIBIA_OUTFIT_FEMALE_ORIENTAL             = 150,
    TIBIA_OUTFIT_FEMALE_PIRATE               = 155,
    TIBIA_OUTFIT_FEMALE_ASSASSIN             = 156,
    TIBIA_OUTFIT_FEMALE_BEGGAR               = 157,
    TIBIA_OUTFIT_FEMALE_SHAMAN               = 158,
    TIBIA_OUTFIT_FEMALE_NORSEMAN             = 252,
    TIBIA_OUTFIT_FEMALE_NIGHTMARE            = 269,
    TIBIA_OUTFIT_FEMALE_JESTER               = 270,
    TIBIA_OUTFIT_FEMALE_BROTHERHOOD          = 279,
    TIBIA_OUTFIT_FEMALE_DEMONHUNTER          = 288,
    TIBIA_OUTFIT_FEMALE_YALAHARIAN           = 324,
    TIBIA_OUTFIT_FEMALE_WEDDING              = 329,

    TIBIA_OUTFIT_HERO                        = 73,
    TIBIA_OUTFIT_FERUMBRAS                   = 229,
    TIBIA_OUTFIT_QUEEN                       = 331,
    TIBIA_OUTFIT_KING                        = 332
} tibiaOutfit_t;

// outfit addon
typedef enum
{
    TIBIA_OUTFIT_ADDON_NONE   = 0,
    TIBIA_OUTFIT_ADDON_1      = 1,
    TIBIA_OUTFIT_ADDON_2      = 2,
    TIBIA_OUTFIT_ADDON_3      = 3,
    TIBIA_OUTFIT_ADDON_FIRST  = TIBIA_OUTFIT_ADDON_1,
    TIBIA_OUTFIT_ADDON_SECOND = TIBIA_OUTFIT_ADDON_2,
    TIBIA_OUTFIT_ADDON_BOTH   = TIBIA_OUTFIT_ADDON_3
} tibiaOutfitAddon_t;

// party
typedef enum
{
    TIBIA_PARTY_NONE                       = 0,
    TIBIA_PARTY_HOST                       = 1, // the host invites the guest to the party
    TIBIA_PARTY_INVITER                    = TIBIA_PARTY_HOST,
    TIBIA_PARTY_GUEST                      = 2, // the guest joins the host at the party
    TIBIA_PARTY_INVITEE                    = TIBIA_PARTY_GUEST,
    TIBIA_PARTY_MEMBER                     = 3,
    TIBIA_PARTY_LEADER                     = 4,
    TIBIA_PARTY_MEMBER_SHARED_EXP          = 5,
    TIBIA_PARTY_LEADER_SHARED_EXP          = 6,
    TIBIA_PARTY_MEMBER_SHARED_EXP_INACTIVE = 7,
    TIBIA_PARTY_LEADER_SHARED_EXP_INACTIVE = 8
} tibiaParty_t;

// skull
typedef enum
{
    TIBIA_SKULL_NONE   = 0,
    TIBIA_SKULL_YELLOW = 1,
    TIBIA_SKULL_GREEN  = 2,
    TIBIA_SKULL_WHITE  = 3,
    TIBIA_SKULL_RED    = 4,
    TIBIA_SKULL_BLACK  = 5
} tibiaSkull_t;

// speech
typedef enum
{
    TIBIA_SPEECH_SAY     = 1,
    TIBIA_SPEECH_YELL    = 2,
    TIBIA_SPEECH_WHISPER = 3
} tibiaSpeech_t;

// tile
typedef enum
{
    TIBIA_TILE_TRANSPARENT             = 470,

    TIBIA_TILE_LAVA                    = 727,

    TIBIA_TILE_WATER_HAS_FISH_BEGIN    = 4597,
    TIBIA_TILE_WATER_HAS_FISH_END      = 4602,

    TIBIA_TILE_WATER_HAS_NO_FISH_BEGIN = 4609,
    TIBIA_TILE_WATER_HAS_NO_FISH_END   = 4614
} tibiaTile_t;

// trigger event
typedef enum
{
    TIBIA_TRIGGER_EVENT_DIALOG_OK                     = 1,
    TIBIA_TRIGGER_EVENT_DIALOG_OK_BRIGHT              = 2,
    TIBIA_TRIGGER_EVENT_DIALOG_INFORMATION            = 30 // version, copyright, website information
} tibiaTriggerEvent_t;

#endif // TIBIA_H
