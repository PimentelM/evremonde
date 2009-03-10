/*
 * Level Spy for Tibia 8.40
 * by Evremonde
 * Last updated on 03/05/2009
 */

#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <shellapi.h>

#include "resource.h"

/* defines */

// window dimensions
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH  240

// tray notification message
#define WM_SHELLNOTIFY WM_USER+5

/* ids */

// tray icon
#define ID_TRAY_ICON               101

// tray menu
#define ID_TRAY_MENU_NULL          1001
#define ID_TRAY_MENU_LEVELSPY      1002
#define ID_TRAY_MENU_LEVELSPY_UP   1003
#define ID_TRAY_MENU_LEVELSPY_DOWN 1004
#define ID_TRAY_MENU_NAMESPY       1005
#define ID_TRAY_MENU_LIGHT         1006
#define ID_TRAY_MENU_INVISIBLE     1007
#define ID_TRAY_MENU_TREES         1008
#define ID_TRAY_MENU_HOTKEYS       1009
#define ID_TRAY_MENU_ABOUT         1010
#define ID_TRAY_MENU_HOMEPAGE      1011
#define ID_TRAY_MENU_EXIT          1012

// hotkeys
#define ID_HOTKEY_LEVELSPY         10001
#define ID_HOTKEY_LEVELSPY_UP      10002
#define ID_HOTKEY_LEVELSPY_DOWN    10003
#define ID_HOTKEY_NAMESPY          10005
#define ID_HOTKEY_LIGHT            10006
#define ID_HOTKEY_INVISIBLE        10007
#define ID_HOTKEY_TREES            10008

// timers

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

// tray menu
HMENU trayMenu;

// hotkeys
int VK_HOTKEY_LEVELSPY      = VK_MULTIPLY; // *
int VK_HOTKEY_LEVELSPY_UP   = VK_ADD;      // +
int VK_HOTKEY_LEVELSPY_DOWN = VK_SUBTRACT; // -
int VK_HOTKEY_NAMESPY       = VK_DIVIDE;   // /
int VK_HOTKEY_LIGHT         = VK_NUMPAD0;  // ins
int VK_HOTKEY_INVISIBLE     = VK_DECIMAL;  // del
int VK_HOTKEY_TREES         = VK_NUMPAD5;  // 5

/* memory addresses */

// statusbar
#define STATUSBAR_TEXT  0x0078A1E8
#define STATUSBAR_TIMER 0x0078A1E4 // timer for showing text

// dat
#define DAT_POINTER     0x00784D2C

// level spy
#define LEVELSPY_NOP    0x004D0DF0 // writeNops 2 bytes // 004D0DF0 | 2BC1 | SUB EAX,ECX
#define LEVELSPY_ABOVE  0x004D0DEC // writeBytes 1 byte // 004D0DEB | B8 07 | MOV EAX,7
#define LEVELSPY_BELOW  0x004D0DF4 // writeBytes 1 byte // 004D0DF3 | B8 02 | MOV EAX,2
// 004D0DE0 | 8B0D 50AA6300 | MOV ECX,DWORD PTR DS:[63AA50]
// 004D0DE6 | 83F9 07       | CMP ECX,7
// 004D0DE9 | 7F 08         | JG SHORT Tibia.004D0DF3
// 004D0DEB | B8 07000000   | MOV EAX,7
// 004D0DF0 | 2BC1          | SUB EAX,ECX
// 004D0DF2 | C3            | RETN
// 004D0DF3 | B8 02000000   | MOV EAX,2
// 004D0DF8 | C3            | RETN

// name spy
#define NAMESPY_NOP     0x004EC179 // writeNops 2 bytes // 004EC179 | 75 4A | JNZ SHORT Tibia.004EC1C5
#define NAMESPY_NOP2    0x004EC183 // writeNops 2 bytes // 004EC183 | 75 40 | JNZ SHORT Tibia.004EC1C5
// 004EC177 | 3901        | CMP DWORD PTR DS:[ECX],EAX
// 004EC179 | 75 4A       | JNZ SHORT Tibia.004EC1C5
// 004EC17B | E8 604CFEFF | CALL Tibia.004D0DE0
// 004EC180 | 3945 18     | CMP DWORD PTR SS:[EBP+18],EAX
// 004EC183 | 75 40       | JNZ SHORT Tibia.004EC1C5

// full light
#define LIGHT_NOP       0x004E4929 // writeNops 2 bytes // 004E4929 | 7E 05 | JLE SHORT Tibia.004E4930
#define LIGHT_AMOUNT    0x004E492C // writeBytes 1 byte // 004E492B | BB 80 | MOV EBX,80
// 004E4919 | 8BD8          | MOV EBX,EAX
// 004E491B | 85DB          | TEST EBX,EBX
// 004E491D | 7D 04         | JGE SHORT Tibia.004E4923
// 004E491F | 33DB          | XOR EBX,EBX
// 004E4921 | EB 0D         | JMP SHORT Tibia.004E4930
// 004E4923 | 81FB 80000000 | CMP EBX,80
// 004E4929 | 7E 05         | JLE SHORT Tibia.004E4930
// 004E492B | BB 80000000   | MOV EBX,80
// 004E4930 | 8D4424 20     | LEA EAX,DWORD PTR SS:[ESP+20]
// 004E4934 | 50            | PUSH EAX
// 004E4935 | E8 76C7FEFF   | CALL Tibia.004D10B0

// show invisible creatures
#define INVISIBLE1      0x0045E2F3 // writeBytes 1 byte // 0045E2F3 | 72 0A | JB SHORT Tibia.0045E2FF
// 0045E2EE | 3D 00000040 | CMP EAX,40000000
// 0045E2F3 | 72 0A       | JB SHORT Tibia.0045E2FF

#define INVISIBLE2      0x004EB445 // writeBytes 1 byte // 004EB445 | 75 09 | JNZ SHORT Tibia.004EB450
// 004EB443 | 84C0        | TEST AL,AL
// 004EB445 | 75 09       | JNZ SHORT Tibia.004EB450
// 004EB447 | C645 FC 22  | MOV BYTE PTR SS:[EBP-4],22
// 004EB44B | E9 B0020000 | JMP Tibia.004EB700
// 004EB450 | B8 08000000 | MOV EAX,8

// player
#define PLAYER_Z        0x0063AA50 // 004D0DE0 | 8B0D 50AA6300 | MOV ECX,DWORD PTR DS:[63AA50]
// 004D0DE0 | 8B0D 50AA6300 | MOV ECX,DWORD PTR DS:[63AA50]
// 004D0DE6 | 83F9 07       | CMP ECX,7

/* constants */

// z-axis
const int Z_AXIS_DEFAULT = 7; // default ground level // 004D0DE6 | 83F9 07 | CMP ECX,7
// 004D0DE0 | 8B0D 50AA6300 | MOV ECX,DWORD PTR DS:[63AA50]
// 004D0DE6 | 83F9 07       | CMP ECX,7

// statusbar
const int STATUSBAR_DURATION = 50; // duration to show text

// dat
const int DAT_MAGIC_NUMBER1 = 76;
const int DAT_MAGIC_NUMBER2 = 100;

const int OFFSET_DAT_ITEMS_POINTER = 8;

const int OFFSET_DAT_ITEM_WIDTH    = 0;
const int OFFSET_DAT_ITEM_HEIGHT   = 4;

// level spy
const int LEVELSPY_NOP_DEFAULT   = 49451; // integer 2 bytes // 004D0DF0 | 2BC1 | SUB EAX,ECX

const int LEVELSPY_ABOVE_DEFAULT = 7; // integer 1 byte // 004D0DEB | B8 07 | MOV EAX,7
const int LEVELSPY_BELOW_DEFAULT = 2; // integer 1 byte // 004D0DF3 | B8 02 | MOV EAX,2

const int LEVELSPY_MIN           = 0;
const int LEVELSPY_MAX           = 7;

// name spy
const int NAMESPY_NOP_DEFAULT  = 19061; // integer 2 bytes // 004EC179 | 75 4A | JNZ SHORT Tibia.004EC1C5
const int NAMESPY_NOP2_DEFAULT = 16501; // integer 2 bytes // 004EC183 | 75 40 | JNZ SHORT Tibia.004EC1C5

// full light
const int LIGHT_NOP_DEFAULT    = 1406; // integer 2 bytes // 004E4929 | 7E 05 | JLE SHORT Tibia.004E4930

const int LIGHT_AMOUNT_DEFAULT = 128; // integer 1 byte // 004E492B | BB 80 | MOV EBX,80
const int LIGHT_AMOUNT_FULL    = 255; // integer 1 byte // 004E492B | BB 80 | MOV EBX,80

// show invisible creatures
const int INVISIBLE1_DEFAULT = 114; // integer 1 byte // 0045E2F3 | 72 0A | JB SHORT Tibia.0045E2FF
const int INVISIBLE2_DEFAULT = 117; // integer 1 byte // 004EB445 | 75 09 | JNZ SHORT Tibia.004EB450

const int INVISIBLE_PATCH    = 235; // integer 1 byte // 235 = EB = JMP // change JB and JNZ to JMP

// hide trees
const int TREE_WIDTH_DEFAULT  = 2;
const int TREE_HEIGHT_DEFAULT = 2;

const int TREE_WIDTH_HIDDEN   = 1;
const int TREE_HEIGHT_HIDDEN  = 1;

const int TREES_ARRAY[] =
{
    957, 958,
    3608, 3609, 3613, 3614, 3615, 3616, 3617, 3618, 3619, 3620,
    3621, 3622, 3623, 3624, 3625, 3626, 3631, 3632, 3633, 3634,
    3635, 3636, 3637, 3638, 3639, 3640, 3641, 3647, 3649, 3687,
    3688, 3689, 3691, 3692, 3694, 3742, 3743, 3744, 3745, 3750,
    3751, 3752, 3753, 3754, 3755, 3756, 3757, 3758, 3759, 3760,
    3761, 3762, 3780, 3871, 3872, 3873, 3877, 3878, 3884, 3885,
    3899, 3901, 3902, 3903, 3905, 3908, 3909, 3910, 3911, 3920,
    3921, 4433, 5091, 5092, 5093, 5094, 5095, 5155, 5156, 5389,
    5390, 5391, 5392, 6094, 7020, 7021, 7022, 7023, 7024
};

/* booleans */

// hotkeys
int bHotkeys = 1;

/* functions */

// get game window
HWND getGameWindow()
    { return FindWindow("tibiaclient", 0); }

// get game process id
DWORD getGameProcessId()
{
    // get game window
    HWND gameWindow = getGameWindow();

    // get process id
    DWORD processId;
    GetWindowThreadProcessId(gameWindow, &processId);

    return processId;
}

// get game process handle
HANDLE getGameProcessHandle()
{
    // get process id
    DWORD processId = getGameProcessId();

    // get process handle
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

    return processHandle;
}

// write bytes to memory
void writeBytes(DWORD address, int value, int bytes)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // write to memory
    WriteProcessMemory(processHandle, (LPVOID)address, &value, bytes, 0);

    // close process handle
    CloseHandle(processHandle);
}

// write string to memory
void writeString(DWORD address, char* text)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // write to memory
    WriteProcessMemory(processHandle, (LPVOID)address, text, strlen(text) + 1, 0);

    // close process handle
    CloseHandle(processHandle);
}

// write NOPs to memory
void writeNops(DWORD address, int nops)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // write to memory
    int i, j = 0;
    for (i = 0; i < nops; i++)
    {
        unsigned char nop = 0x90;
        WriteProcessMemory(processHandle, (LPVOID)(address + j), &nop, 1, 0);
        j++; // increment address
    }

    // close process handle
    CloseHandle(processHandle);
}

// read bytes from memory
int readBytes(DWORD address, int bytes)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // read from memory
    int buffer = 0;
    ReadProcessMemory(processHandle, (LPVOID)address, &buffer, bytes, 0);

    // close process handle
    CloseHandle(processHandle);

    return buffer;
}

// read string from memory
char* readString(DWORD address)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // read from memory
    static char buffer[256];
    ReadProcessMemory(processHandle, (LPVOID)address, &buffer, sizeof(buffer), 0);

    // close process handle
    CloseHandle(processHandle);

    return buffer;
}

// set statusbar text
void setStatusbarText(char* text)
{
    // show statusbar text by timer
    writeBytes(STATUSBAR_TIMER, STATUSBAR_DURATION, 1);

    // set statusbar text
    writeString(STATUSBAR_TEXT, text);
}

// get dat pointer
int getDatPointer()
    { return readBytes(DAT_POINTER, 4); }

// get dat items pointer
int getDatItemsPointer()
    { return readBytes(getDatPointer() + OFFSET_DAT_ITEMS_POINTER, 4); }

// get dat item offset
int getDatItemOffset(int itemId, int offset, int bytes)
    { return readBytes(getDatItemsPointer() + (DAT_MAGIC_NUMBER1 * (itemId - DAT_MAGIC_NUMBER2)) + offset, bytes); }

// set dat item offset
void setDatItemOffset(int itemId, int offset, int value, int bytes)
    { writeBytes(getDatItemsPointer() + (DAT_MAGIC_NUMBER1 * (itemId - DAT_MAGIC_NUMBER2)) + offset, value, bytes); }

// check if level spy is enabled
int isLevelSpyEnabled()
{
    int levelSpyNop = readBytes(LEVELSPY_NOP, 2);
    if (levelSpyNop == LEVELSPY_NOP_DEFAULT)
        return 0;
    else
        return 1;
}

// check if name spy is enabled
int isNameSpyEnabled()
{
    int nameSpyNop = readBytes(NAMESPY_NOP, 2);
    int nameSpyNop2 = readBytes(NAMESPY_NOP2, 2);
    if ((nameSpyNop == NAMESPY_NOP_DEFAULT) || (nameSpyNop2 == NAMESPY_NOP2_DEFAULT))
        return 0;
    else
        return 1;
}

// check if full light is enabled
int isFullLightEnabled()
{
    int lightNop = readBytes(LIGHT_NOP, 2);
    if (lightNop == LIGHT_NOP_DEFAULT)
        return 0;
    else
        return 1;
}

// check if show invisible creatures is enabled
int isShowInvisibleCreaturesEnabled()
{
    int invisible1 = readBytes(INVISIBLE1, 1);
    int invisible2 = readBytes(INVISIBLE2, 1);
    if ((invisible1 == INVISIBLE1_DEFAULT) || (invisible2 == INVISIBLE2_DEFAULT))
        return 0;
    else
        return 1;
}

// check if hide trees is enabled
int isHideTreesEnabled()
{
    int treeWidth = getDatItemOffset(TREES_ARRAY[0], OFFSET_DAT_ITEM_WIDTH, 1);
    int treeHeight = getDatItemOffset(TREES_ARRAY[0], OFFSET_DAT_ITEM_HEIGHT, 1);
    if ((treeWidth == TREE_WIDTH_DEFAULT) || (treeHeight == TREE_HEIGHT_DEFAULT))
        return 0;
    else
        return 1;
}

// initialize level spy
void doLevelSpyInitialize()
{
    // get player z
    int playerZ = readBytes(PLAYER_Z, 1);

    // set level spy to current level
    if (playerZ <= Z_AXIS_DEFAULT)
    {
        // above ground
        writeBytes(LEVELSPY_ABOVE, Z_AXIS_DEFAULT - playerZ, 1); // z-axis calculated
        writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
    }
    else
    {
        // below ground
        writeBytes(LEVELSPY_ABOVE, LEVELSPY_ABOVE_DEFAULT, 1);
        writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
    }
}

// disable level spy
void doLevelSpyOff()
{
    // disable level spy by restoring default values
    writeBytes(LEVELSPY_NOP, LEVELSPY_NOP_DEFAULT, 2);

    writeBytes(LEVELSPY_ABOVE, LEVELSPY_ABOVE_DEFAULT, 1);
    writeBytes(LEVELSPY_BELOW, LEVELSPY_BELOW_DEFAULT, 1);
}

// enable level spy
void doLevelSpyOn()
{
    // enable level spy
    writeNops(LEVELSPY_NOP, 2);
}

// toggle level spy
void doLevelSpyToggle()
{
    // check if level spy is enabled
    if (isLevelSpyEnabled() == 0)
    {
        // enable level spy
        doLevelSpyOn();

        // initialize level spy
        doLevelSpyInitialize();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY, MF_CHECKED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_ENABLED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_ENABLED);

        setStatusbarText("Level Spy: On");
    }
    else
    {
        // disable level spy
        doLevelSpyOff();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY, MF_UNCHECKED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_DISABLED | MF_GRAYED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_DISABLED | MF_GRAYED);

        setStatusbarText("Level Spy: Off");
    }
}

// level spy up
void doLevelSpyUp()
{
    // level spy must be enabled
    if (isLevelSpyEnabled() == 0)
    {
        setStatusbarText("Please enable Level Spy first!");
        return;
    }

    // get player z
    int playerZ = readBytes(PLAYER_Z, 1);

    // get ground level
    int groundLevel = 0;
    if (playerZ <= Z_AXIS_DEFAULT)
        groundLevel = LEVELSPY_ABOVE; // above ground
    else
        groundLevel = LEVELSPY_BELOW; // below ground

    // get current level
    int currentLevel = readBytes(groundLevel, 1);

    // get spy level
    int spyLevel = 0;
    if (currentLevel >= LEVELSPY_MAX)
        spyLevel = LEVELSPY_MIN; // loop back to start
    else
        spyLevel = currentLevel + 1; // increase spy level

    // level spy up
    writeBytes(groundLevel, spyLevel, 1);

    // calculate display level
    int displayLevel = 0;
    if (playerZ <= Z_AXIS_DEFAULT)
        displayLevel = spyLevel; // above ground
    else
        displayLevel = spyLevel - (LEVELSPY_BELOW_DEFAULT + 1); // below ground

    // display level
    char buffer[256];
    sprintf(buffer, "Level Spy: %d", displayLevel);
    setStatusbarText(buffer);
}

// level spy down
void doLevelSpyDown()
{
    // level spy must be enabled
    if (isLevelSpyEnabled() == 0)
    {
        setStatusbarText("Please enable Level Spy first!");
        return;
    }

    // get player z
    int playerZ = readBytes(PLAYER_Z, 1);

    // get ground level
    int groundLevel = 0;
    if (playerZ <= Z_AXIS_DEFAULT)
        groundLevel = LEVELSPY_ABOVE; // above ground
    else
        groundLevel = LEVELSPY_BELOW; // below ground

    // get current level
    int currentLevel = readBytes(groundLevel, 1);

    // cannot spy below default z-axis
    //if (playerZ == Z_AXIS_DEFAULT && currentLevel == LEVELSPY_MIN)
    //{
        //setStatusbarText("You cannot spy below the default ground level!");
        //return;
    //}

    // get spy level
    int spyLevel = 0;
    if (currentLevel <= LEVELSPY_MIN)
        spyLevel = LEVELSPY_MAX; // loop back to start
    else
        spyLevel = currentLevel - 1; // decrease spy level

    // level spy down
    writeBytes(groundLevel, spyLevel, 1);

    // calculate display level
    int displayLevel = 0;
    if (playerZ <= Z_AXIS_DEFAULT)
        displayLevel = spyLevel; // above ground
    else
        displayLevel = spyLevel - (LEVELSPY_BELOW_DEFAULT + 1); // below ground

    // display level
    char buffer[256];
    sprintf(buffer, "Level Spy: %d", displayLevel);
    setStatusbarText(buffer);
}

// disable name spy
void doNameSpyOff()
{
    // disable name spy by restoring default values
    writeBytes(NAMESPY_NOP, NAMESPY_NOP_DEFAULT, 2);
    writeBytes(NAMESPY_NOP2, NAMESPY_NOP2_DEFAULT, 2);
}

// enable name spy
void doNameSpyOn()
{
    // enable name spy
    writeNops(NAMESPY_NOP, 2);
    writeNops(NAMESPY_NOP2, 2);
}

// toggle name spy
void doNameSpyToggle()
{
    // check if name spy is enabled
    if (isNameSpyEnabled() == 0)
    {
        // enable name spy
        doNameSpyOn();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_NAMESPY, MF_CHECKED);

        setStatusbarText("Name Spy: On");
    }
    else
    {
        // disable name spy
        doNameSpyOff();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_NAMESPY, MF_UNCHECKED);

        setStatusbarText("Name Spy: Off");
    }
}

// disable full light
void doFullLightOff()
{
    // disable full light by restoring default values
    writeBytes(LIGHT_NOP, LIGHT_NOP_DEFAULT, 2);
    writeBytes(LIGHT_AMOUNT, LIGHT_AMOUNT_DEFAULT, 1);
}

// enable full light
void doFullLightOn()
{
    // enable full light
    writeNops(LIGHT_NOP, 2);
    writeBytes(LIGHT_AMOUNT, LIGHT_AMOUNT_FULL, 1);
}

// toggle full light
void doFullLightToggle()
{
    // check if full light is enabled
    if (isFullLightEnabled() == 0)
    {
        // enable full light
        doFullLightOn();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_CHECKED);

        setStatusbarText("Full Light: On");
    }
    else
    {
        // disable full light
        doFullLightOff();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_UNCHECKED);

        setStatusbarText("Full Light: Off");
    }
}

// disable show invisible creatures
void doShowInvisibleCreaturesOff()
{
    // disable show invisible creatures by restoring default values
    writeBytes(INVISIBLE1, INVISIBLE1_DEFAULT, 1);
    writeBytes(INVISIBLE2, INVISIBLE2_DEFAULT, 1);
}

// enable show invisible creatures
void doShowInvisibleCreaturesOn()
{
    // enable show invisible creatures
    writeBytes(INVISIBLE1, INVISIBLE_PATCH, 1);
    writeBytes(INVISIBLE2, INVISIBLE_PATCH, 1);
}

// toggle show invisible creatures
void doShowInvisibleCreaturesToggle()
{
    // check if show invisible creatures is enabled
    if (isShowInvisibleCreaturesEnabled() == 0)
    {
        // enable show invisible creatures
        doShowInvisibleCreaturesOn();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_INVISIBLE, MF_CHECKED);

        setStatusbarText("Show Invisible Creatures: On");
    }
    else
    {
        // disable show invisible creatures
        doShowInvisibleCreaturesOff();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_INVISIBLE, MF_UNCHECKED);

        setStatusbarText("Show Invisible Creatures: Off");
    }
}

// disable hide trees
void doHideTreesOff()
{
    unsigned int i = 0;
    for (i = 0; i < sizeof(TREES_ARRAY); i++)
    {
        setDatItemOffset(TREES_ARRAY[i], OFFSET_DAT_ITEM_WIDTH, TREE_WIDTH_DEFAULT, 1);
        setDatItemOffset(TREES_ARRAY[i], OFFSET_DAT_ITEM_HEIGHT, TREE_HEIGHT_DEFAULT, 1);
    }
}

// enable hide trees
void doHideTreesOn()
{
    unsigned int i = 0;
    for (i = 0; i < sizeof(TREES_ARRAY); i++)
    {
        setDatItemOffset(TREES_ARRAY[i], OFFSET_DAT_ITEM_WIDTH, TREE_WIDTH_HIDDEN, 1);
        setDatItemOffset(TREES_ARRAY[i], OFFSET_DAT_ITEM_HEIGHT, TREE_HEIGHT_HIDDEN, 1);
    }
}

// toggle hide trees
void doHideTreesToggle()
{
    // check if hide trees is enabled
    if (isHideTreesEnabled() == 0)
    {
        // enable hide trees
        doHideTreesOn();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_TREES, MF_CHECKED);

        setStatusbarText("Hide Trees: On");
    }
    else
    {
        // disable hide trees
        doHideTreesOff();

        CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_UNCHECKED);

        setStatusbarText("Hide Trees: Off");
    }
}

// register hotkeys
void doHotkeysRegister(HWND hwnd)
{
    RegisterHotKey(hwnd, ID_HOTKEY_LEVELSPY,      0, VK_HOTKEY_LEVELSPY);
    RegisterHotKey(hwnd, ID_HOTKEY_LEVELSPY_UP,   0, VK_HOTKEY_LEVELSPY_UP);
    RegisterHotKey(hwnd, ID_HOTKEY_LEVELSPY_DOWN, 0, VK_HOTKEY_LEVELSPY_DOWN);
    RegisterHotKey(hwnd, ID_HOTKEY_NAMESPY,       0, VK_HOTKEY_NAMESPY);
    RegisterHotKey(hwnd, ID_HOTKEY_LIGHT,         0, VK_HOTKEY_LIGHT);
    RegisterHotKey(hwnd, ID_HOTKEY_INVISIBLE,     0, VK_HOTKEY_INVISIBLE);
    RegisterHotKey(hwnd, ID_HOTKEY_TREES,         0, VK_HOTKEY_TREES);
}

// unregister hotkeys
void doHotkeysUnregister(HWND hwnd)
{
    UnregisterHotKey(hwnd, ID_HOTKEY_LEVELSPY);
    UnregisterHotKey(hwnd, ID_HOTKEY_LEVELSPY_UP);
    UnregisterHotKey(hwnd, ID_HOTKEY_LEVELSPY_DOWN);
    UnregisterHotKey(hwnd, ID_HOTKEY_NAMESPY);
    UnregisterHotKey(hwnd, ID_HOTKEY_LIGHT);
    UnregisterHotKey(hwnd, ID_HOTKEY_INVISIBLE);
    UnregisterHotKey(hwnd, ID_HOTKEY_TREES);
}

// toggle hotkeys
void doHotkeysToggle()
{
    // check if hotkeys are enabled
    if (bHotkeys == 0)
    {
        // enable hotkeys
        bHotkeys = 1;

        CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_CHECKED);

        setStatusbarText("Hotkeys: On");
    }
    else
    {
        // disable hotkeys
        bHotkeys = 0;

        CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_UNCHECKED);

        setStatusbarText("Hotkeys: Off");
    }
}

/* tray icon */

// create tray icon
void doTrayIconCreate(HWND hwnd)
{
    // initialize tray icon
    trayIcon.cbSize           = sizeof(NOTIFYICONDATA);
    trayIcon.hWnd             = hwnd;
    trayIcon.uID              = ID_TRAY_ICON;
    trayIcon.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    trayIcon.uCallbackMessage = WM_SHELLNOTIFY;
    trayIcon.hIcon            = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_APPLICATION), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON), LR_SHARED); //LoadIcon(0, IDI_WINLOGO);

    // set tray icon tip text
    strcpy(trayIcon.szTip, "Level Spy for Tibia 8.40");

    // add tray icon
    Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* tray menu */

// update tray menu
void doTrayMenuUpdate(HWND hwnd)
{
    // check if game window is found
    if (getGameWindow() == 0)
    {
        // disable tray menus
        CheckMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY, MF_UNCHECKED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_DISABLED | MF_GRAYED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_DISABLED | MF_GRAYED);

        CheckMenuItem(trayMenu, ID_TRAY_MENU_NAMESPY, MF_UNCHECKED);
        CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_UNCHECKED);
        CheckMenuItem(trayMenu, ID_TRAY_MENU_INVISIBLE, MF_UNCHECKED);
        CheckMenuItem(trayMenu, ID_TRAY_MENU_TREES, MF_UNCHECKED);
        return;
    }

    // check if level spy is enabled
    if (isLevelSpyEnabled() == 1)
    {
        // enable level spy menus
        CheckMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY, MF_CHECKED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_ENABLED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_ENABLED);
    }
    else
    {
        // disable level spy menus
        CheckMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY, MF_UNCHECKED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_DISABLED | MF_GRAYED);
        EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_DISABLED | MF_GRAYED);
    }

    // check if name spy is enabled
    if (isNameSpyEnabled() == 1)
        CheckMenuItem(trayMenu, ID_TRAY_MENU_NAMESPY, MF_CHECKED);
    else
        CheckMenuItem(trayMenu, ID_TRAY_MENU_NAMESPY, MF_UNCHECKED);

    // check if full light is enabled
    if (isFullLightEnabled() == 1)
        CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_CHECKED);
    else
        CheckMenuItem(trayMenu, ID_TRAY_MENU_LIGHT, MF_UNCHECKED);

    // check if show invisible creatures is enabled
    if (isShowInvisibleCreaturesEnabled() == 1)
        CheckMenuItem(trayMenu, ID_TRAY_MENU_INVISIBLE, MF_CHECKED);
    else
        CheckMenuItem(trayMenu, ID_TRAY_MENU_INVISIBLE, MF_UNCHECKED);

    // check if hide trees is enabled
    if (isHideTreesEnabled() == 1)
        CheckMenuItem(trayMenu, ID_TRAY_MENU_TREES, MF_CHECKED);
    else
        CheckMenuItem(trayMenu, ID_TRAY_MENU_TREES, MF_UNCHECKED);

    // check if hotkeys are enabled
    if (bHotkeys == 1)
        CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_CHECKED);
    else
        CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_UNCHECKED);
}

// create tray menu
void doTrayMenuCreate(HWND hwnd)
{
    // create tray menu
    trayMenu = CreatePopupMenu();
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NULL,          "Level Spy for Tibia 8.40");
    AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LEVELSPY,      "Level Spy\t*");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LEVELSPY_UP,   "Level Spy Up\t+");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LEVELSPY_DOWN, "Level Spy Down\t-");
    AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NAMESPY,       "Name Spy\t/");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LIGHT,         "Full Light\tIns");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_INVISIBLE,     "Show Invisible Creatures\tDel");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_TREES,         "Hide Trees\t5");
    AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOTKEYS,       "Hotkeys");
    AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_ABOUT,         "About...");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOMEPAGE,      "Homepage...");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_EXIT,          "Exit");

    // set default tray menu item
    SetMenuDefaultItem(trayMenu, ID_TRAY_MENU_NULL, MF_BYCOMMAND);

    // check hotkeys menu
    CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_CHECKED);

    // disable level spy menus
    EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_UP, MF_DISABLED | MF_GRAYED);
    EnableMenuItem(trayMenu, ID_TRAY_MENU_LEVELSPY_DOWN, MF_DISABLED | MF_GRAYED);

    // update tray menu
    doTrayMenuUpdate(hwnd);
}

// show tray menu
void doTrayMenuShow(HWND hwnd)
{
    // update tray menu
    doTrayMenuUpdate(hwnd);

    // get menu location
    POINT trayMenuLocation;
    GetCursorPos(&trayMenuLocation);

    // required to make menu show and hide correctly
    SetForegroundWindow(hwnd);

    // show tray menu
    TrackPopupMenu(trayMenu,
        TPM_RIGHTALIGN | TPM_RIGHTBUTTON, // align right and allow right-click on menu items
        trayMenuLocation.x, trayMenuLocation.y,
        0, hwnd, 0);

    // required to make menu show and hide correctly
    PostMessage(hwnd, WM_NULL, 0, 0);
}

// show about
void doShowAbout()
{
    MessageBox(0,
        "Level Spy for Tibia 8.40\nby Evremonde\nLast updated on 03/05/2009\n\nHotkeys:\n*\tLevel Spy On/Off\n+\tLevel Spy Up\n-\tLevel Spy Down\n/\tName Spy On/Off\nIns\tFull Light On/Off\nDel\tShow Invisible Creatures On/Off\n5\tHide Trees On/Off\n\nHomepage:\nhttp://code.google.com/p/evremonde/",
        "About", MB_OK | MB_ICONINFORMATION);
}

// open homepage
void doOpenHomepage()
    { ShellExecute(0, 0, "http://code.google.com/p/evremonde/", 0, 0, SW_SHOW); }

// load ini
void doLoadIni()
{
    /* hotkeys */

    // level spy
    VK_HOTKEY_LEVELSPY = GetPrivateProfileInt("Hotkeys", "LevelSpy", VK_HOTKEY_LEVELSPY, "./levelspy840.ini");
    VK_HOTKEY_LEVELSPY_UP = GetPrivateProfileInt("Hotkeys", "LevelSpyUp", VK_HOTKEY_LEVELSPY_UP, "./levelspy840.ini");
    VK_HOTKEY_LEVELSPY_DOWN = GetPrivateProfileInt("Hotkeys", "LevelSpyDown", VK_HOTKEY_LEVELSPY_DOWN, "./levelspy840.ini");

    // name spy
    VK_HOTKEY_NAMESPY = GetPrivateProfileInt("Hotkeys", "NameSpy", VK_HOTKEY_NAMESPY, "./levelspy840.ini");

    // full light
    VK_HOTKEY_LIGHT = GetPrivateProfileInt("Hotkeys", "FullLight", VK_HOTKEY_LIGHT, "./levelspy840.ini");

    // show invisible creatures
    VK_HOTKEY_INVISIBLE = GetPrivateProfileInt("Hotkeys", "ShowInvisibleCreatures", VK_HOTKEY_INVISIBLE, "./levelspy840.ini");

    // hide trees
    VK_HOTKEY_TREES = GetPrivateProfileInt("Hotkeys", "HideTrees", VK_HOTKEY_TREES, "./levelspy840.ini");

    /* options */

    // name spy
    if (GetPrivateProfileInt("Options", "NameSpy", 0, "./levelspy840.ini") == 1)
        doNameSpyOn();

    // full light
    if (GetPrivateProfileInt("Options", "FullLight", 0, "./levelspy840.ini") == 1)
        doFullLightOn();

    // show invisible creatures
    if (GetPrivateProfileInt("Options", "ShowInvisibleCreatures", 0, "./levelspy840.ini") == 1)
        doShowInvisibleCreaturesOn();

    // hide trees
    if (GetPrivateProfileInt("Options", "HideTrees", 0, "./levelspy840.ini") == 1)
        doHideTreesOn();

    // hotkeys
    if (GetPrivateProfileInt("Options", "Hotkeys", 1, "./levelspy840.ini") == 0)
        bHotkeys = 0;
    else
        bHotkeys = 1;
}

/* windows messages */

void onCreate(HWND hwnd)
{
    // load ini
    doLoadIni();

    // create tray icon
    doTrayIconCreate(hwnd);

    // create tray menu
    doTrayMenuCreate(hwnd);

    // register hotkeys
    doHotkeysRegister(hwnd);

    // check if num lock is enabled
    if (GetKeyState(VK_NUMLOCK) == 0)
    {
        // enable num lock for hotkeys
        keybd_event(VK_NUMLOCK, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(VK_NUMLOCK, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }

    // loaded
    setStatusbarText("Level Spy for Tibia 8.40 loaded!");
}

void onDestroy(HWND hwnd)
{
    // disable level spy
    doLevelSpyOff();

    // disable name spy
    //doNameSpyOff();

    // disable full light
    //doFullLightOff();

    // disable show invisible creatures
    //doShowInvisibleCreaturesOff();

    // disable hide trees
    //doHideTreesOff();

    // delete tray icon
    Shell_NotifyIcon(NIM_DELETE, &trayIcon);

    // destroy tray menu
    DestroyMenu(trayMenu);

    // unregister hotkeys
    doHotkeysUnregister(hwnd);

    // unloaded
    setStatusbarText("Level Spy for Tibia 8.40 unloaded!");

    // exit
    PostQuitMessage(0);
}

void onTray(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // tray icon
    if (wparam == ID_TRAY_ICON)
    {
        // left click or right click
        if (lparam == WM_LBUTTONDOWN || lparam == WM_RBUTTONDOWN)
        {
            // show tray menu
            doTrayMenuShow(hwnd);
        }
    }
}

void onHotkey(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // check if hotkeys are enabled
    if (bHotkeys == 0)
        return;

    // restrict hotkeys to game and application by window focus
    HWND currentWindow = GetForegroundWindow();
    HWND gameWindow = getGameWindow();
    if (currentWindow != gameWindow && currentWindow != hwnd)
        return;

    // hotkeys
    switch (wparam)
    {
        // level spy
        case ID_HOTKEY_LEVELSPY:
            doLevelSpyToggle();
            Sleep(1);
            break;

        // level spy up
        case ID_HOTKEY_LEVELSPY_UP:
            doLevelSpyUp();
            Sleep(1);
            break;

        // level spy down
        case ID_HOTKEY_LEVELSPY_DOWN:
            doLevelSpyDown();
            Sleep(1);
            break;

        // name spy
        case ID_HOTKEY_NAMESPY:
            doNameSpyToggle();
            Sleep(1);
            break;

        // full light
        case ID_HOTKEY_LIGHT:
            doFullLightToggle();
            Sleep(1);
            break;

        // show invisible creatures
        case ID_HOTKEY_INVISIBLE:
            doShowInvisibleCreaturesToggle();
            Sleep(1);
            break;

        // hide trees
        case ID_HOTKEY_TREES:
            doHideTreesToggle();
            Sleep(1);
            break;
    }
}

void onCommand(HWND hwnd, int command, int notifycode, HWND hwnditem)
{
    switch (command)
    {
        // level spy
        case ID_TRAY_MENU_LEVELSPY:
            doLevelSpyToggle();
            break;

        // level spy up
        case ID_TRAY_MENU_LEVELSPY_UP:
            doLevelSpyUp();
            break;

        // level spy down
        case ID_TRAY_MENU_LEVELSPY_DOWN:
            doLevelSpyDown();
            break;

        // name spy
        case ID_TRAY_MENU_NAMESPY:
            doNameSpyToggle();
            break;

        // full light
        case ID_TRAY_MENU_LIGHT:
            doFullLightToggle();
            break;

        // show invisible creatures
        case ID_TRAY_MENU_INVISIBLE:
            doShowInvisibleCreaturesToggle();
            break;

        // hide trees
        case ID_TRAY_MENU_TREES:
            doHideTreesToggle();
            break;

        // hotkeys
        case ID_TRAY_MENU_HOTKEYS:
            doHotkeysToggle();
            break;

        // about
        case ID_TRAY_MENU_ABOUT:
            doShowAbout();
            break;

        // homepage
        case ID_TRAY_MENU_HOMEPAGE:
            doOpenHomepage();
            break;

        // exit
        case ID_TRAY_MENU_EXIT:
            DestroyWindow(hwnd); // exit
            break;
    }
}

/* window messages handler */

LRESULT CALLBACK windowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        case WM_CREATE:
            onCreate(hwnd);
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd); // exit
            break;

        case WM_DESTROY:
            onDestroy(hwnd);
            break;

        case WM_SHELLNOTIFY:
            onTray(hwnd, wparam, lparam);
            break;

        case WM_HOTKEY:
            onHotkey(hwnd, wparam, lparam);
            break;

        case WM_COMMAND:
            onCommand(hwnd, LOWORD(wparam), HIWORD(wparam), (HWND)lparam);
            SetForegroundWindow(getGameWindow()); // set focus to game window after using tray menu
            break;

        default:
            return DefWindowProc(hwnd, umsg, wparam, lparam);
    }
    return 0;
}

/* main function */

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR cmdline, int cmdshow)
{
    // only allow one instance of application
    HWND applicationWindow = FindWindow(0, "Level Spy - Tibia 8.40");
    if (applicationWindow)
        return 0;

    // register window class
    WNDCLASSEX wc;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = windowProc; // window messages handler
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hinstance;
    wc.hIcon         = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_APPLICATION), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CXICON), LR_SHARED); //LoadIcon(0, IDI_WINLOGO);
    wc.hIconSm       = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_APPLICATION), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON), LR_SHARED); //LoadIcon(0, IDI_WINLOGO);
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = 0; //(HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); // window background color
    wc.lpszMenuName  = 0;
    wc.lpszClassName = "levelspy";

    if(!RegisterClassEx(&wc))
    {
        MessageBox(0,
            "Register Window Class failed!",
            "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // create window
    HWND hwnd;
    hwnd = CreateWindowEx(
        WS_EX_DLGMODALFRAME, // extended window style
        "levelspy", "Level Spy - Tibia 8.40", // window class and title
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window style
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_HEIGHT, WINDOW_WIDTH, // window dimensions
        HWND_DESKTOP, 0, hinstance, 0);

    if(hwnd == 0)
    {
        MessageBox(0,
            "Create Window failed!",
            "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // center window
    //HWND desktopWindow = GetDesktopWindow();
    //RECT desktopRect;
    //GetWindowRect(desktopWindow, &desktopRect);

    //RECT hwndRect;
    //GetWindowRect(hwnd, &hwndRect);

    //hwndRect.left = desktopRect.left + ((desktopRect.right  - desktopRect.left) - (hwndRect.right  - hwndRect.left)) / 2;
    //hwndRect.top  = desktopRect.top  + ((desktopRect.bottom - desktopRect.top)  - (hwndRect.bottom - hwndRect.top))  / 2;

    //SetWindowPos(hwnd, 0, hwndRect.left, hwndRect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

    // show window
    //ShowWindow(hwnd, cmdshow);
    //UpdateWindow(hwnd);

    // message loop
    MSG msg;
    while(GetMessage(&msg, 0, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
