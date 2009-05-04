#ifndef _LOOTER842DLL_H_
#define _LOOTER842DLL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

HANDLE hMod;

typedef void __stdcall _StatusbarText(char* text, int bShowForever);
static _StatusbarText *StatusbarText = (_StatusbarText *)0x0053D7E0;

//typedef void __stdcall _Say(int type, char* text);
//static _Say *Say = (_Say *)0x00407330;

typedef void __stdcall _MoveObject(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _MoveObject *MoveObject = (_MoveObject *)0x00405260;
// 1 = from X else 0xFFFF
// 2 = from Container 0x40+i or from Slot 0x01-0x0A or from Y
// 3 = from Container Position or from Z
// 4 = Item ID
// 5 = from Container Position or stack when moved from ground
// 6 = to X else 0xFFFF
// 7 = to Container 0x40+i or to Slot 0x01-0x0A or to Y
// 8 = to Container position or to Z
// 9 = Item Count

typedef void __stdcall _UseObject(int fromX, int fromY, int fromZ, int id, int stack); //(int fromX, int fromY, int fromZ, int id, int stack, int toX, int toY, int toZ, int count);
static _UseObject *UseObject = (_UseObject *)0x00406280;

// TradeObject? 0x00405BA0 A player with this name is not online.

//typedef void __stdcall _RotateObject(int x, int y, int z, int id);
//static _RotateObject *RotateObject = (_RotateObject *)0x00406880;

//typedef void __stdcall _InspectObject(int x, int y, int z);
//static _InspectObject *InspectObject = (_InspectObject *)0x00407150;

//typedef void __stdcall _TriggerEvent(int event, int maw, int mow);
//static _TriggerEvent *TriggerEvent = (_TriggerEvent *)0x00518E40;

typedef void __stdcall _TriggerEvent2(int event, char* maw, char* mow);
static _TriggerEvent2 *TriggerEvent2 = (_TriggerEvent2 *)0x00518E40;

typedef struct
{
    int id;
    int count;
    int container;
    char* containerName;
    int position;
    int bFound;
} Item;

#define PLAYER_CAP      0x00631D60

#define PLAYER_Z        0x00640B38
#define PLAYER_Y        0x00640B3C // PLAYER_Z + 4
#define PLAYER_X        0x00640B40 // PLAYER_Z + 8

#define STATUSBAR_TIMER 0x007902CC // STATUSBAR_TEXT - 4
#define STATUSBAR_TEXT  0x007902D0

#define SEE_ID          0x0078E500
#define SEE_TEXT        0x007904F8

#define CONTAINER_BEGIN 0x0063E248
#define CONTAINER_END   0x00640108 // BEGIN + (STEP_CONTAINER * MAX_CONTAINERS)

const int STEP_CONTAINER      = 492; // containers in inventory
const int STEP_CONTAINER_ITEM = 12;  // items in containers

const int MAX_CONTAINERS = 16; // max number of containers opened

const int OFFSET_CONTAINER_ID         = 4;
const int OFFSET_CONTAINER_NAME       = 16;
const int OFFSET_CONTAINER_VOLUME     = 48; // max number of items in container
const int OFFSET_CONTAINER_HAS_PARENT = 52;
const int OFFSET_CONTAINER_AMOUNT     = 56; // current number of items in container
const int OFFSET_CONTAINER_ITEM_ID    = 60;
const int OFFSET_CONTAINER_ITEM_COUNT = 64; // stacked item count

const int MAX_STACK = 100; // max stack item count

const int SLOT_BACKPACK = 3;

const int ITEM_MONEY_GOLD     = 3031;
const int ITEM_MONEY_PLATINUM = 3035;
const int ITEM_MONEY_CRYSTAL  = 3043;

const int EVENT_DIALOG = 1;

#endif // _LOOTER842DLL_H_
