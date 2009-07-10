#ifndef LOOTERDLL_H
#define LOOTERDLL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#include <libxml/tree.h>

// version
#define TIBIA_VERSION "8.50"

// application name
#define APPLICATION_NAME "Looter"

// application homepage
#define APPLICATION_HOMEPAGE "http://code.google.com/p/evremonde/"

// file names
#define FILENAME "looter.dll"

#define FILENAME_PARSE_LOOTER_FILE "looter.xml"

#define FILENAME_WRITE_INVENTORY_TO_FILE "looter_inventory.txt"

/* globals */

HANDLE hMod;

HANDLE hDllThread;

HANDLE hThreads[3];
const int nThreads = 3;

/* enums */

typedef enum
{
    LOOTER_PARSE_TYPE_LOOT_ITEMS_AROUND_PLAYER,
    LOOTER_PARSE_TYPE_LOOT,
    LOOTER_PARSE_TYPE_STACK,
    LOOTER_PARSE_TYPE_USE
} LooterParseType;

/* tibia client functions */

typedef void __stdcall _tibia_set_statusbar_text(char* text, int bShowForever);
static _tibia_set_statusbar_text *tibia_set_statusbar_text = (_tibia_set_statusbar_text *)0x0053E500;

//typedef void __stdcall _tibia_trigger_event(int event, int maw, int mow);
//static _tibia_trigger_event *tibia_trigger_event = (_tibia_trigger_event *)0x00519770;

typedef void __stdcall _tibia_trigger_event_ex(int event, char* maw, char* mow);
static _tibia_trigger_event_ex *tibia_trigger_event_ex = (_tibia_trigger_event_ex *)0x00519770;

typedef void __stdcall _tibia_move_object(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _tibia_move_object *tibia_move_object = (_tibia_move_object *)0x00405220;
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
static _tibia_use_object *tibia_use_object = (_tibia_use_object *)0x00406240;

/* tibia memory addresses, structures, and constants */

DWORD *TIBIA_PLAYER_CAP = (DWORD *)0x00632EA0; // TIBIA_PLAYER_CAP / 100 = float playerCap

//#define TIBIA_PLAYER_CAP 0x00632EA0 //0x00631D60

DWORD *TIBIA_PLAYER_Z = (DWORD *)0x00641C78;
DWORD *TIBIA_PLAYER_Y = (DWORD *)0x00641C7C; // TIBIA_PLAYER_Z + 4
DWORD *TIBIA_PLAYER_X = (DWORD *)0x00641C80; // TIBIA_PLAYER_Z + 8

//#define TIBIA_PLAYER_Z 0x00641C78
//#define TIBIA_PLAYER_Y 0x00641C7C
//#define TIBIA_PLAYER_X 0x00641C80

typedef struct
{
    unsigned int timer;
    char text[256];
} TibiaStatusbar;

TibiaStatusbar *TIBIA_STATUSBAR = (TibiaStatusbar *)0x00791418; //0x007902CC;

//#define TIBIA_STATUSBAR_TIMER 0x00791414 // TIBIA_STATUSBAR_TEXT - 4
//#define TIBIA_STATUSBAR_TEXT  0x00791418

DWORD *TIBIA_SEE_ID   = (DWORD *)0x0078F640;
char  *TIBIA_SEE_TEXT = (char *) 0x00791640;

//#define TIBIA_SEE_ID   0x0078F640
//#define TIBIA_SEE_TEXT 0x00791640

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
} TibiaInventory;

TibiaInventory *TIBIA_INVENTORY = (TibiaInventory *)0x0063F388;

//#define TIBIA_CONTAINER_BEGIN 0x0063F388
//#define TIBIA_CONTAINER_END   0x00641248 // = TIBIA_CONTAINER_BEGIN + (TIBIA_STEP_CONTAINER * TIBIA_MAX_CONTAINERS)

const int TIBIA_FIRST_CONTAINER = 64; // = 0x40

//const int TIBIA_STEP_CONTAINER      = 492; // containers in inventory
//const int TIBIA_STEP_CONTAINER_ITEM = 12;  // items in containers

const int TIBIA_MAX_CONTAINERS      = 16; // max number of containers opened
const int TIBIA_MAX_CONTAINER_ITEMS = 36; // max number of items in container

//const int TIBIA_OFFSET_CONTAINER_IS_OPEN    = 0;
//const int TIBIA_OFFSET_CONTAINER_ID         = 4;
//const int TIBIA_OFFSET_CONTAINER_NAME       = 16;
//const int TIBIA_OFFSET_CONTAINER_VOLUME     = 48; // max number of items in container
//const int TIBIA_OFFSET_CONTAINER_HAS_PARENT = 52; // OFFSET_CONTAINER_IS_CHILD
//const int TIBIA_OFFSET_CONTAINER_AMOUNT     = 56; // current number of items in container
//const int TIBIA_OFFSET_CONTAINER_ITEM_ID    = 60;
//const int TIBIA_OFFSET_CONTAINER_ITEM_COUNT = 64; // stacked item count

const int TIBIA_MAX_STACK = 100; // max stacked item count

typedef enum
{
    TIBIA_EQUIPMENT_SLOT_NULL,
    TIBIA_EQUIPMENT_SLOT_HEAD,
    TIBIA_EQUIPMENT_SLOT_NECK,
    TIBIA_EQUIPMENT_SLOT_BACKPACK, // container
    TIBIA_EQUIPMENT_SLOT_BODY,
    TIBIA_EQUIPMENT_SLOT_RIGHT,
    TIBIA_EQUIPMENT_SLOT_LEFT,
    TIBIA_EQUIPMENT_SLOT_LEGS,
    TIBIA_EQUIPMENT_SLOT_FEET,
    TIBIA_EQUIPMENT_SLOT_RING,
    TIBIA_EQUIPMENT_SLOT_AMMO
} TibiaEquipmentSlot;

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

const int TIBIA_TRIGGER_EVENT_DIALOG_OK   = 1;
const int TIBIA_TRIGGER_EVENT_DIALOG_INFO = 30; // version, copyright, website information

#endif // LOOTERDLL_H
