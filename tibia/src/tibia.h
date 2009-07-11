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

// statusbar
#define TIBIA_STATUSBAR_TIMER 0x00791414 // TIBIA_STATUSBAR_TEXT - 4
#define TIBIA_STATUSBAR_TEXT  0x00791418

typedef struct
{
    unsigned int timer;
    char text[256];
} TibiaStatusbar;

TibiaStatusbar *_TIBIA_STATUSBAR = (TibiaStatusbar *)TIBIA_STATUSBAR_TEXT;

// player
#define TIBIA_PLAYER_CAP 0x00632EA0 // TIBIA_PLAYER_CAP / 100 = float playerCap
DWORD *_TIBIA_PLAYER_CAP = (DWORD *)TIBIA_PLAYER_CAP;

#define TIBIA_PLAYER_Z   0x00641C78
#define TIBIA_PLAYER_Y   0x00641C7C // TIBIA_PLAYER_Z + 4
#define TIBIA_PLAYER_X   0x00641C80 // TIBIA_PLAYER_Z + 8
DWORD *_TIBIA_PLAYER_Z = (DWORD *)TIBIA_PLAYER_Z;
DWORD *_TIBIA_PLAYER_Y = (DWORD *)TIBIA_PLAYER_Y; 
DWORD *_TIBIA_PLAYER_X = (DWORD *)TIBIA_PLAYER_X;

// container
#define TIBIA_CONTAINER_BEGIN 0x0063F388
#define TIBIA_CONTAINER_END   0x00641248 // = TIBIA_CONTAINER_BEGIN + (TIBIA_STEP_CONTAINER * TIBIA_MAX_CONTAINERS)

typedef struct
{
    unsigned int id;
    unsigned int count;
    unsigned int unknown;
} TibiaContainerItem;

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
    TibiaContainerItem item[36];
} TibiaContainer;

typedef struct
{
    TibiaContainer container[16];
} TibiaInventory;

// inventory
TibiaInventory *_TIBIA_INVENTORY = (TibiaInventory *)TIBIA_CONTAINER_BEGIN;

// see
#define TIBIA_SEE_ID   0x0078F640
#define TIBIA_SEE_TEXT 0x00791640

/* constants */

// statusbar
const int TIBIA_STATUSBAR_TEXT_DEFAULT_DURATION = 50; // duration to show statusbar text

// container
const int TIBIA_FIRST_CONTAINER = 64; // = 0x40

const int TIBIA_STEP_CONTAINER      = 492; // containers in inventory
const int TIBIA_STEP_CONTAINER_ITEM = 12;  // items in containers

const int TIBIA_MAX_CONTAINERS      = 16; // number of containers open in client
const int TIBIA_MAX_CONTAINER_ITEMS = 36; // number of items in a container

const int TIBIA_OFFSET_CONTAINER_IS_OPEN    = 0;
const int TIBIA_OFFSET_CONTAINER_ID         = 4;
const int TIBIA_OFFSET_CONTAINER_NAME       = 16;
const int TIBIA_OFFSET_CONTAINER_VOLUME     = 48; // max number of items in the container
const int TIBIA_OFFSET_CONTAINER_HAS_PARENT = 52; // = TIBIA_OFFSET_CONTAINER_IS_CHILD
const int TIBIA_OFFSET_CONTAINER_AMOUNT     = 56; // current number of items in the container
const int TIBIA_OFFSET_CONTAINER_ITEM_ID    = 60;
const int TIBIA_OFFSET_CONTAINER_ITEM_COUNT = 64; // stacked item count

// item
const int TIBIA_MAX_ITEM_STACK = 100; // max stacked item count

/* enumerated types */

// equipment slots
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
    TIBIA_EQUIPMENT_SLOT_AMMO     = 10
} tibiaEquipmentSlot;

// trigger events
typedef enum
{
    TIBIA_TRIGGER_EVENT_DIALOG_OK          = 1,
    TIBIA_TRIGGER_EVENT_DIALOG_INFORMATION = 30 // version, copyright, website information
} tibiaTriggerEvent;

#endif // TIBIA_H

