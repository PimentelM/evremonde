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

// set statusbar text
typedef void __stdcall _tibia_set_statusbar_text(char* text, int shouldShowForever);
static _tibia_set_statusbar_text *tibia_set_statusbar_text = (_tibia_set_statusbar_text *)0x0053E500;

// trigger event
typedef void __stdcall _tibia_trigger_event(int event, int maw, int mow);
static _tibia_trigger_event *tibia_trigger_event = (_tibia_trigger_event *)0x00519770;

// trigger event ex
typedef void __stdcall _tibia_trigger_event_ex(int event, char* maw, char* mow);
static _tibia_trigger_event_ex *tibia_trigger_event_ex = (_tibia_trigger_event_ex *)0x00519770;

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

// use object
typedef void __stdcall _tibia_use_object(int fromX, int fromY, int fromZ, int id, int stack); //(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _tibia_use_object *tibia_use_object = (_tibia_use_object *)0x00406240;

/* memory addresses */

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
} tibiaContainerItem;

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
    tibiaContainerItem item[36];
} tibiaContainer;

// inventory
typedef struct
{
    tibiaContainer container[16];
} tibiaInventory;

tibiaInventory *_TIBIA_INVENTORY = (tibiaInventory *)TIBIA_CONTAINER_BEGIN;

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

/* enumerated types */

// action state
typedef enum
{
    TIBIA_ACTION_STATE_NONE                 = 0,
    TIBIA_ACTION_STATE_LEFT_CLICK           = 1,  // left-click to walk or to use the client interface
    TIBIA_ACTION_STATE_RIGHT_CLICK          = 2,  // right-click to use an object such as a torch or an apple
    TIBIA_ACTION_STATE_INSPECT_OBJECT       = 3,  // left-click + right-click to see or inspect an object
    TIBIA_ACTION_STATE_MOVE_OBJECT          = 6,  // dragging an object to move it to a new location
    TIBIA_ACTION_STATE_USE_OBJECT           = 7,  // using an object such as a rope, a shovel, a fishing rod, or a rune
    TIBIA_ACTION_STATE_SELECT_HOTKEY_OBJECT = 8,  // selecting an object to bind to a hotkey from the "Hotkey Options" window
    TIBIA_ACTION_STATE_TRADE_OBJECT         = 9,  // using "Trade with..." on an object to select a player with whom to trade
    TIBIA_ACTION_STATE_CLIENT_HELP          = 10, // client mouse over tooltip help
    TIBIA_ACTION_STATE_OPEN_DIALOG_WINDOW   = 11, // opening a dialog window such as the "Options" window, "Select Outfit" window, or "Move Objects" window
    TIBIA_ACTION_STATE_POPUP_MENU           = 12  // showing a popup menu with options such as "Invite to Party", "Set Outfit", "Copy Name", or "Set Mark"
} tibiaActionState;

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
} tibiaEquipmentSlot;

// flag
typedef enum
{
    TIBIA_FLAG_NONE                                   = 0,
    TIBIA_FLAG_POISONED                               = 1,
    TIBIA_FLAG_BURNING                                = 2,
    TIBIA_FLAG_ELECTRIFIED                            = 4,
    TIBIA_FLAG_DRUNK                                  = 8,
    TIBIA_FLAG_PROTECTED_BY_MAGIC_SHIELD              = 16,
    TIBIA_FLAG_PARALYSED                              = 32,
    TIBIA_FLAG_PARALYZED                              = TIBIA_FLAG_PARALYSED,
    TIBIA_FLAG_HASTED                                 = 64,
    TIBIA_FLAG_IN_BATTLE                              = 128,
    TIBIA_FLAG_DROWNING                               = 256,
    TIBIA_FLAG_FREEZING                               = 512,
    TIBIA_FLAG_DAZZLED                                = 1024,
    TIBIA_FLAG_CURSED                                 = 2048,
    TIBIA_FLAG_STRENGTHENED                           = 4096,
    TIBIA_FLAG_CANNOT_LOGOUT_OR_ENTER_PROTECTION_ZONE = 8192,
    TIBIA_FLAG_WITHIN_PROTECTION_ZONE                 = 16384
} tibiaFlag;

// light amount
typedef enum
{
    TIBIA_LIGHT_AMOUNT_DEFAULT = 128,
    TIBIA_LIGHT_AMOUNT_FULL    = 255
} tibiaLightAmount;

// light color
typedef enum
{
    TIBIA_LIGHT_COLOR_NONE    = 0,
    TIBIA_LIGHT_COLOR_DEFAULT = 206,
    TIBIA_LIGHT_COLOR_ORANGE  = TIBIA_LIGHT_COLOR_DEFAULT,
    TIBIA_LIGHT_COLOR_WHITE   = 215
} tibiaLightColor;

// light radius
typedef enum
{
    TIBIA_LIGHT_RADIUS_NONE  = 0,
    TIBIA_LIGHT_RADIUS_TORCH = 7,
    TIBIA_LIGHT_RADIUS_FULL  = 20
} tibiaLightRadius;

// login status
typedef enum
{
    TIBIA_LOGIN_STATUS_LOGGED_OUT = 0,
    TIBIA_LOGIN_STATUS_LOGGING_IN = 6,
    TIBIA_LOGIN_STATUS_LOGGED_IN  = 8
} tibiaLoginStatus;

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
} tibiaOffsetContainer;

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
} tibiaOutfitAddon;

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
} tibiaParty;


// skull
typedef enum
{
    TIBIA_SKULL_NONE   = 0,
    TIBIA_SKULL_YELLOW = 1,
    TIBIA_SKULL_GREEN  = 2,
    TIBIA_SKULL_WHITE  = 3,
    TIBIA_SKULL_RED    = 4,
    TIBIA_SKULL_BLACK  = 5
} tibiaSkull;

// trigger event
typedef enum
{
    TIBIA_TRIGGER_EVENT_DIALOG_OK          = 1,
    TIBIA_TRIGGER_EVENT_DIALOG_INFORMATION = 30 // version, copyright, website information
} tibiaTriggerEvent;

#endif // TIBIA_H
