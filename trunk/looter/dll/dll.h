#ifndef _DLL_H_
#define _DLL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#include "cJSON.h"

/* definitions */

// tibia version
#define TIBIA_VERSION "8.53"

// application name
#define APPLICATION_NAME "Looter"

// application class name
#define APPLICATION_CLASS_NAME "looter"

// application homepage
#define APPLICATION_HOMEPAGE "http://code.google.com/p/evremonde/"

// looter names
#define LOOTER_DLL_NAME "looter.dll"

#define LOOTER_PARSE_ITEMS_FILE_NAME "looter.txt"

#define LOOTER_WRITE_INVENTORY_TO_FILE_NAME "looter_inventory.txt"

// window properties
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH  240

/* ids */

// timers
#define ID_TIMER_LOOT  10001
#define ID_TIMER_STACK 10002
#define ID_TIMER_USE   10003

// hotkeys
#define ID_HOTKEY_LOOT_ITEMS_AROUND_PLAYER 100001
#define ID_HOTKEY_SHOW_ITEM_ID             100002
#define ID_HOTKEY_EXIT                     100003

/* global variables */

HANDLE hMod;

HANDLE hMainWindowThread;

/* enumerated constants */

typedef enum
{
    LOOTER_PARSE_ITEMS_TYPE_LOOT_ITEMS_AROUND_PLAYER,
    LOOTER_PARSE_ITEMS_TYPE_LOOT,
    LOOTER_PARSE_ITEMS_TYPE_STACK,
    LOOTER_PARSE_ITEMS_TYPE_USE
} LooterParseItemsType;

/* tibia internal client functions */

typedef void __stdcall _tibia_set_statusbar_text(char* text, int shouldShowForever);
static _tibia_set_statusbar_text *tibia_set_statusbar_text = (_tibia_set_statusbar_text *)0x0053F540;

//typedef void __stdcall _tibia_trigger_event(int event, int maw, int mow);
//static _tibia_trigger_event *tibia_trigger_event = (_tibia_trigger_event *)0x00519FC0;

typedef void __stdcall _tibia_trigger_event_ex(int event, char* maw, char* mow);
static _tibia_trigger_event_ex *tibia_trigger_event_ex = (_tibia_trigger_event_ex *)0x00519FC0;

typedef void __stdcall _tibia_move_object(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _tibia_move_object *tibia_move_object = (_tibia_move_object *)0x00405240;
// 1 = from X else 0xFFFF
// 2 = from Container 0x40+i or from Slot 0x01-0x0A or from Y
// 3 = from Container Position or from Z
// 4 = Item ID
// 5 = from Container Position or stack when moved from ground
// 6 = to X else 0xFFFF
// 7 = to Container 0x40+i or to Slot 0x01-0x0A or to Y
// 8 = to Container Position or to Z
// 9 = Item Count

typedef void __stdcall _tibia_use_object(int fromX, int fromY, int fromZ, int id, int stack); //(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _tibia_use_object *tibia_use_object = (_tibia_use_object *)0x00406260;

/* tibia memory addresses, structures, and constants */

DWORD *TIBIA_PLAYER_CAP = (DWORD *)0x00635EE0; // TIBIA_PLAYER_CAP / 100 = float playerCap

//#define TIBIA_PLAYER_CAP 0x00635EE0 //0x00632EA0

DWORD *TIBIA_PLAYER_Z = (DWORD *)0x00645148;
DWORD *TIBIA_PLAYER_Y = (DWORD *)0x0064514C; // TIBIA_PLAYER_Z + 4
DWORD *TIBIA_PLAYER_X = (DWORD *)0x00645150; // TIBIA_PLAYER_Z + 8

//#define TIBIA_PLAYER_Z 0x00645148
//#define TIBIA_PLAYER_Y 0x0064514C
//#define TIBIA_PLAYER_X 0x00645150

typedef struct
{
    unsigned int timer;
    char text[256];
} TibiaStatusbar;

TibiaStatusbar *TIBIA_STATUSBAR = (TibiaStatusbar *)0x007D62F0;

//#define TIBIA_STATUSBAR_TIMER 0x007D62EC // TIBIA_STATUSBAR_TEXT - 4
//#define TIBIA_STATUSBAR_TEXT  0x007D62F0

DWORD *TIBIA_SEE_ID   = (DWORD *)0x00792B14;
//DWORD *TIBIA_SEE_ID   = (DWORD *)0x00792B20;
char  *TIBIA_SEE_TEXT = (char *) 0x007D6518;

//#define TIBIA_SEE_ID   0x00792B14
//#define TIBIA_SEE_TEXT 0x007D6518

typedef struct
{
    int id;
    int count;
    int container;
    char* containerName;
    int position;
    int isFound;
} TibiaContainerItem;

typedef struct
{
    unsigned int id;
    unsigned int count;
    unsigned int unknown;
} TibiaItem;

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
    TibiaItem item[36];
} TibiaContainer;

typedef struct
{
    TibiaContainer container[16];
} TibiaContainers;

TibiaContainers *TIBIA_CONTAINERS = (TibiaContainers *)0x00642858; // TIBIA_CONTAINERS == TIBIA_INVENTORY

//#define TIBIA_CONTAINERS_BEGIN 0x00642858
//#define TIBIA_CONTAINERS_END   0x00644718 // = TIBIA_CONTAINERS_BEGIN + (TIBIA_STEP_CONTAINER * TIBIA_MAX_CONTAINERS)

const int TIBIA_FIRST_CONTAINER = 64; // = 0x40

//const int TIBIA_STEP_CONTAINER      = 492; // containers in inventory
//const int TIBIA_STEP_CONTAINER_ITEM = 12;  // items in containers

const int TIBIA_MAX_CONTAINERS      = 16; // max number of containers opened
const int TIBIA_MAX_CONTAINER_ITEMS = 36; // max number of items in a container

//const int TIBIA_OFFSET_CONTAINER_IS_OPEN    = 0;
//const int TIBIA_OFFSET_CONTAINER_ID         = 4;
//const int TIBIA_OFFSET_CONTAINER_NAME       = 16;
//const int TIBIA_OFFSET_CONTAINER_VOLUME     = 48; // max number of items in container
//const int TIBIA_OFFSET_CONTAINER_HAS_PARENT = 52; // TIBIA_OFFSET_CONTAINER_IS_CHILD
//const int TIBIA_OFFSET_CONTAINER_AMOUNT     = 56; // current number of items in container
//const int TIBIA_OFFSET_CONTAINER_ITEM_ID    = 60;
//const int TIBIA_OFFSET_CONTAINER_ITEM_COUNT = 64; // stacked item count

const int TIBIA_MAX_ITEM_STACK = 100; // max stacked item count

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
} TibiaEquipmentSlot;

//const int TIBIA_EQUIPMENT_SLOT_NULL     = 0;
//const int TIBIA_EQUIPMENT_SLOT_HEAD     = 1;
//const int TIBIA_EQUIPMENT_SLOT_NECK     = 2;
//const int TIBIA_EQUIPMENT_SLOT_BACKPACK = 3; // container
//const int TIBIA_EQUIPMENT_SLOT_BODY     = 4;
//const int TIBIA_EQUIPMENT_SLOT_RIGHT    = 5;
//const int TIBIA_EQUIPMENT_SLOT_LEFT     = 6;
//const int TIBIA_EQUIPMENT_SLOT_LEGS     = 7;
//const int TIBIA_EQUIPMENT_SLOT_FEET     = 8;
//const int TIBIA_EQUIPMENT_SLOT_RING     = 9;
//const int TIBIA_EQUIPMENT_SLOT_AMMO     = 10;

//const int TIBIA_ITEM_CURRENCY_GOLD     = 3031;
//const int TIBIA_ITEM_CURRENCY_PLATINUM = 3035;
//const int TIBIA_ITEM_CURRENCY_CRYSTAL  = 3043;

//const int TIBIA_ITEM_CONTAINER_BAG      = 2853;
//const int TIBIA_ITEM_CONTAINER_BACKPACK = 2854;

const int TIBIA_TRIGGER_EVENT_DIALOG_OK          = 1;
const int TIBIA_TRIGGER_EVENT_DIALOG_INFORMATION = 30; // version, copyright, website information

#endif // _DLL_H_
