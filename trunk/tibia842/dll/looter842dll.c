/**
 *  Looter DLL for Tibia 8.42
 *  by Evremonde
 *  Last updated on 5/3/2009
 */

#include "looter842dll.h"

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
    int i = 0;
    for (i = 0; i < nops; i++)
    {
        unsigned char nop = 0x90;
        WriteProcessMemory(processHandle, (LPVOID)(address + i), &nop, 1, 0);
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

// get player cap
int getPlayerCap()
    { return readBytes(PLAYER_CAP, 1); }

// get player z
int getPlayerZ()
    { return readBytes(PLAYER_Z, 1); }

// get player y
int getPlayerY()
    { return readBytes(PLAYER_Y, 2); }

// get player x
int getPlayerX()
    { return readBytes(PLAYER_X, 2); }

// get statusbar text
char* getStatusbarText()
    { return readString(STATUSBAR_TEXT); }

// get item
Item GetItem(int itemId, int itemCount, char* containerName, int bGetNextItem, int bStackItem)
{
    Item item;

    int bFoundNext = 0;

    // search through containers
    int container = 0;
    int i = 0;
    for(i = CONTAINER_BEGIN; i < CONTAINER_BEGIN + (STEP_CONTAINER * MAX_CONTAINERS); i += STEP_CONTAINER)
    {
        // current container
        container++;

        // container must be open
        int bOpen = readBytes(i, 1);
        if (bOpen == 0)
            continue;

        // name of container
        char* name = readString(i + OFFSET_CONTAINER_NAME);

        // use container name
        if (strlen(containerName) > 0)
            if (strstr(name, containerName) == NULL)
                continue;

        // number of items in container
        int amount = readBytes(i + OFFSET_CONTAINER_AMOUNT, 1);
        if (amount == 0)
            continue;

        // search through container's items
        int position = 0;
        for (position = 0; position < amount; position++)
        {
            // id of item
            int id = readBytes(i + OFFSET_CONTAINER_ITEM_ID + (STEP_CONTAINER_ITEM * position), 2);
            if (id == 0)
                continue;

            // stacked count of item
            int count = readBytes((i + OFFSET_CONTAINER_ITEM_ID + (STEP_CONTAINER_ITEM * position)) + (OFFSET_CONTAINER_ITEM_COUNT - OFFSET_CONTAINER_ITEM_ID), 1);

            // use stack item
            if (bStackItem != 0 && count >= MAX_STACK) // already stacked
                continue;

            // item ids match
            if (id == itemId)
            {
                // use get next item
                if (bGetNextItem && !bFoundNext)
                {
                    bFoundNext = 1;
                    continue;
                }

                item.id            = id;
                //item.count       = count;
                item.container     = container - 1;
                item.containerName = name;
                item.position      = position;
                item.bFound        = 1;

                // use item count
                if (itemCount > 0)
                    item.count = itemCount;
                else
                    item.count = count;

                return item; // item found
            }
        }
    }

    // item not found
    item.id            = 0;
    item.count         = 0;
    item.container     = 0;
    item.containerName = "";
    item.position      = 0;
    item.bFound        = 0;
    return item;
}

// move item to slot
void MoveItemToSlot(int itemId, char* fromContainerName, int toSlot)
{
    Item item = GetItem(itemId, 0, fromContainerName, 0, 0);
    if (item.bFound == 0)
        return;

    MoveObject(0xFFFF, 0x40 + item.container, item.position, itemId, item.position, 0xFFFF, toSlot, 0x00, item.count);
}

// stack items
void StackItems(int itemId)
{
    Item item     = GetItem(itemId, 0, "", 0, 1);
    Item nextItem = GetItem(itemId, 0, "", 1, 1);
    if (item.bFound == 0 || nextItem.bFound == 0)
        return;

    // do not stack to special containers
    if (strstr(nextItem.containerName, "Dead")        != NULL ||
        strstr(nextItem.containerName, "Locker")      != NULL ||
        strstr(nextItem.containerName, "Depot Chest") != NULL)
        return;

    MoveObject(0xFFFF, 0x40 + nextItem.container, nextItem.position, itemId, nextItem.position, 0xFFFF, 0x40 + item.container, item.position, nextItem.count);
}

// use item
void UseItem(int itemId)
{
    Item item = GetItem(itemId, 0, "", 0, 0);
    if (item.bFound == 0)
        return;

    UseObject(0xFFFF, 0x40 + item.container, item.position, itemId, item.position);
}

// loot items around player
void LootItemsAroundPlayer(int itemId, int toSlot)
{
    MoveObject(getPlayerX(),     getPlayerY(),     getPlayerZ(), itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(getPlayerX() - 1, getPlayerY() - 1, getPlayerZ(), itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(getPlayerX() + 1, getPlayerY() + 1, getPlayerZ(), itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(getPlayerX() - 1, getPlayerY() + 1, getPlayerZ(), itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(getPlayerX() + 1, getPlayerY() - 1, getPlayerZ(), itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(getPlayerX() - 1, getPlayerY(),     getPlayerZ(), itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(getPlayerX() + 1, getPlayerY(),     getPlayerZ(), itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(getPlayerX(),     getPlayerY() - 1, getPlayerZ(), itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(getPlayerX(),     getPlayerY() + 1, getPlayerZ(), itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
}

// dll thread
DWORD WINAPI DllThread()
{
    while(1)
    {
        // loot and stack items
        FILE *file = fopen("looter842.txt", "r");
        if (file != NULL)
        {
            char line[255];
            while (fgets(line, sizeof(line), file) != NULL)
            {
                // ignore lines with comments
                if (strstr(line, "//") != NULL)
                    continue;

                char *token;

                // item id
                token = strtok(line, ",");
                if (token == NULL)
                    continue;
                int itemId = atoi(token);

                // slot
                token = strtok(NULL, ",");
                if (token == NULL)
                    continue;
                int toSlot = atoi(token);

                // loot
                token = strtok(NULL, ",");
                if (token == NULL)
                    continue;
                int bLoot = atoi(token);

                // stack
                token = strtok(NULL, ",");
                if (token == NULL)
                    continue;
                int bStack = atoi(token);

                // use
                token = strtok(NULL, ",");
                if (token == NULL)
                    continue;
                int bUse = atoi(token);

                // item name
                token = strtok(NULL, "");
                if (token == NULL)
                    continue;

                if (bLoot == 1 && getPlayerCap() > 0 && strstr(getStatusbarText(), "You cannot put more objects in this container.") == NULL)
                    MoveItemToSlot(itemId, "Dead", toSlot);

                if (bStack == 1)
                    StackItems(itemId);

                if (bUse == 1 && strstr(getStatusbarText(), "You are full.") == NULL)
                    UseItem(itemId);
            }
        }
        else
        {
            MessageBox(0, "File not found!\nlooter842.txt must be in Tibia folder!\n\nClosing looter...", "Error", MB_ICONERROR | MB_OK);
            FreeLibraryAndExitThread(hMod, 0);
        }
        fclose(file);

        // loot items around player
        if (GetAsyncKeyState(VK_HOME))
        {
            FILE *file = fopen("looter842.txt", "r");
            if (file != NULL)
            {
                char line[255];
                while (fgets(line, sizeof(line), file) != NULL)
                {
                    // ignore lines with comments
                    if (strstr(line, "//") != NULL)
                        continue;

                    char *token;

                    // item id
                    token = strtok(line, ",");
                    if (token == NULL)
                        continue;
                    int itemId = atoi(token);

                    // slot
                    token = strtok(NULL, ",");
                    if (token == NULL)
                        continue;
                    int toSlot = atoi(token);

                    // loot
                    token = strtok(NULL, ",");
                    if (token == NULL)
                        continue;
                    int bLoot = atoi(token);

                    if (bLoot == 1 && getPlayerCap() > 0 && strstr(getStatusbarText(), "You cannot put more objects in this container.") == NULL)
                        LootItemsAroundPlayer(itemId, toSlot);
                }
            }
            else
            {
                MessageBox(0, "File not found!\nlooter842.txt must be in Tibia folder!\n\nClosing looter...", "Error", MB_ICONERROR | MB_OK);
                FreeLibraryAndExitThread(hMod, 0);
            }
            fclose(file);
        }

        // show item id
        if (GetAsyncKeyState(VK_END))
        {
            char buffer[255];
            strcpy(buffer, readString(SEE_TEXT));
            strcat(buffer, "(Item ID: ");
            char itemId[255];
            itoa(readBytes(SEE_ID, 2), itemId, 10);
            strcat(buffer, itemId);
            strcat(buffer, ")");
            StatusbarText(buffer, 0);
        }

        // uninject dll
        if (GetAsyncKeyState(VK_PAUSE))
        {
            StatusbarText("Looter for Tibia 8.42 unloaded!", 0);
            FreeLibraryAndExitThread(hMod, 0);
        }

        Sleep(100);
    }
}

// dll main
BOOL APIENTRY DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    hMod = hModule;

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        //MessageBox(0, "DLL_PROCESS_ATTACH", "fdwReason", MB_OK);
        TriggerEvent2(EVENT_DIALOG, "Looter for Tibia 8.42", "Hello!\n\nPress 'Home' to loot items around player off ground.\nPress 'End' to show item ID number of last seen item.\n\nPress 'Pause' or 'Break' to close looter.");
        StatusbarText("Looter for Tibia 8.42 loaded!", 0);
        CreateThread(0, 0, DllThread, 0, 0, 0);
    }
    return TRUE;
}
