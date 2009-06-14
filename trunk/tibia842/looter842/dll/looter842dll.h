#ifndef _LOOTER842DLL_H_
#define _LOOTER842DLL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/tree.h>

#include <windows.h>

/* globals */

HANDLE hMod;

HANDLE hDllThread;

HANDLE hThreads[3];
const int nThreads = 3;

/* tibia functions */

typedef void __stdcall _Tibia_StatusbarText(char* text, int bShowForever);
static _Tibia_StatusbarText *Tibia_StatusbarText = (_Tibia_StatusbarText *)0x0053D7E0;

//typedef void __stdcall _Tibia_TriggerEvent(int event, int maw, int mow);
//static _Tibia_TriggerEvent *Tibia_TriggerEvent = (_Tibia_TriggerEvent *)0x00518E40;

typedef void __stdcall _Tibia_TriggerEventEx(int event, char* maw, char* mow);
static _Tibia_TriggerEventEx *Tibia_TriggerEventEx = (_Tibia_TriggerEventEx *)0x00518E40;

typedef void __stdcall _Tibia_MoveObject(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _Tibia_MoveObject *Tibia_MoveObject = (_Tibia_MoveObject *)0x00405260;
// 1 = from X else 0xFFFF
// 2 = from Container 0x40+i or from Slot 0x01-0x0A or from Y
// 3 = from Container Position or from Z
// 4 = Item ID
// 5 = from Container Position or stack when moved from ground
// 6 = to X else 0xFFFF
// 7 = to Container 0x40+i or to Slot 0x01-0x0A or to Y
// 8 = to Container position or to Z
// 9 = Item Count

typedef void __stdcall _Tibia_UseObject(int fromX, int fromY, int fromZ, int id, int stack); //(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _Tibia_UseObject *Tibia_UseObject = (_Tibia_UseObject *)0x00406280;

/* memory addresses and structures */

DWORD *PLAYER_CAP = (DWORD *)0x00631D60;

//#define PLAYER_CAP 0x00631D60

DWORD *PLAYER_Z = (DWORD *)0x00640B38;
DWORD *PLAYER_Y = (DWORD *)0x00640B3C; // PLAYER_Z + 4
DWORD *PLAYER_X = (DWORD *)0x00640B40; // PLAYER_Z + 8

//#define PLAYER_Z 0x00640B38
//#define PLAYER_Y 0x00640B3C
//#define PLAYER_X 0x00640B40

typedef struct
{
    unsigned int timer;
    char text[255];
} Statusbar;

Statusbar *STATUSBAR = (Statusbar *)0x007902CC;

//#define STATUSBAR_TIMER 0x007902CC; // STATUSBAR_TEXT - 4
//#define STATUSBAR_TEXT  0x007902D0;

DWORD *SEE_ID   = (DWORD *)0x0078E500;
char  *SEE_TEXT = (char *) 0x007904F8;

//#define SEE_ID   0x0078E500;
//#define SEE_TEXT 0x007904F8;

typedef struct
{
    int id;
    int count;
    int container;
    char* containerName;
    int position;
    int bFound;
} ContainerItem;

typedef struct
{
    unsigned int id;
    unsigned int count;
    unsigned int unknown;
} Item;

typedef struct
{
    unsigned int bIsOpen;
    unsigned int id;
    unsigned int unknown1;
    unsigned int unknown2;
    char name[32];
    unsigned int volume;
    unsigned int bHasParent; // bIsChild
    unsigned int amount;
    Item item[36];
} Container;

typedef struct
{
    Container container[16];
} Inventory;

Inventory *INVENTORY = (Inventory *)0x0063E248;

//#define CONTAINER_BEGIN 0x0063E248
//#define CONTAINER_END   0x00640108 // CONTAINER_BEGIN + (STEP_CONTAINER * MAX_CONTAINERS)

/* constants */

//const int STEP_CONTAINER      = 492; // containers in inventory
//const int STEP_CONTAINER_ITEM = 12;  // items in containers

const int MAX_CONTAINERS      = 16; // max number of containers opened
const int MAX_CONTAINER_ITEMS = 36; // max number of items in container

//const int OFFSET_CONTAINER_IS_OPEN    = 0;
//const int OFFSET_CONTAINER_ID         = 4;
//const int OFFSET_CONTAINER_NAME       = 16;
//const int OFFSET_CONTAINER_VOLUME     = 48; // max number of items in container
//const int OFFSET_CONTAINER_HAS_PARENT = 52; // OFFSET_CONTAINER_IS_CHILD
//const int OFFSET_CONTAINER_AMOUNT     = 56; // current number of items in container
//const int OFFSET_CONTAINER_ITEM_ID    = 60;
//const int OFFSET_CONTAINER_ITEM_COUNT = 64; // stacked item count

const int MAX_STACK = 100; // max stacked item count

const int EQUIPMENT_SLOT_HEAD     = 1;
const int EQUIPMENT_SLOT_NECK     = 2;
const int EQUIPMENT_SLOT_BACKPACK = 3;
const int EQUIPMENT_SLOT_BODY     = 4;
const int EQUIPMENT_SLOT_RIGHT    = 5;
const int EQUIPMENT_SLOT_LEFT     = 6;
const int EQUIPMENT_SLOT_LEGS     = 7;
const int EQUIPMENT_SLOT_FEET     = 8;
const int EQUIPMENT_SLOT_RING     = 9;
const int EQUIPMENT_SLOT_AMMO     = 10;

//const int ITEM_CURRENCY_GOLD     = 3031;
//const int ITEM_CURRENCY_PLATINUM = 3035;
//const int ITEM_CURRENCY_CRYSTAL  = 3043;

const int TRIGGER_EVENT_DIALOG_OK = 1;

#endif // _LOOTER842DLL_H_
