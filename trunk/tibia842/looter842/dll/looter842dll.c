/**
 *  Looter DLL for Tibia 8.42
 *  by Evremonde
 *  Last updated on 5/4/2009
 */

#include "looter842dll.h"

// write inventory to file
void WriteInventoryToFile()
{
    FILE *file = fopen("looter842_inventory.txt", "w");
    if (file != NULL)
    {
        fprintf(file, "Inventory");
        fprintf(file, "\n");

        int i = 0;
        for (i = 0; i < MAX_CONTAINERS; i++)
        {
            fprintf(file, "....Container Is Open: %u",    INVENTORY->container[i].bIsOpen);
            fprintf(file, "\n");
            fprintf(file, "....Container ID: %u",         INVENTORY->container[i].id);
            fprintf(file, "\n");
            fprintf(file, "....Container Unknown 1: %u",  INVENTORY->container[i].unknown1);
            fprintf(file, "\n");
            fprintf(file, "....Container Unknown 2: %u",  INVENTORY->container[i].unknown2);
            fprintf(file, "\n");
            fprintf(file, "....Container Name: %s",       INVENTORY->container[i].name);
            fprintf(file, "\n");
            fprintf(file, "....Container Volume: %u",     INVENTORY->container[i].volume);
            fprintf(file, "\n");
            fprintf(file, "....Container Has Parent: %u", INVENTORY->container[i].bHasParent);
            fprintf(file, "\n");
            fprintf(file, "....Container Amount: %u",     INVENTORY->container[i].amount);
            fprintf(file, "\n");

            int j = 0;
            for (j = 0; j < MAX_CONTAINER_ITEMS; j++)
            {
                fprintf(file, "........Item ID: %u",      INVENTORY->container[i].item[j].id);
                fprintf(file, "\n");
                fprintf(file, "........Item Count: %u",   INVENTORY->container[i].item[j].count);
                fprintf(file, "\n");
                fprintf(file, "........Item Unknown: %u", INVENTORY->container[i].item[j].unknown);
                fprintf(file, "\n");
            }
        }
    }
    fclose(file);
}

// get item
Item GetItem(int itemId, int itemCount, char* containerName, int bGetNextItem, int bStackItem)
{
    Item item;

    // next item not found yet
    int bFoundNext = 0;

    // loop through containers
    int i = 0; // current container
    for(i = 0; i < MAX_CONTAINERS; i++)
    {
        // container must be open
        if (INVENTORY->container[i].bIsOpen == 0)
            continue;

        // container must not be empty
        if (INVENTORY->container[i].amount == 0)
            continue;

        // use container name
        if (strlen(containerName) > 0)
        {
            // container names must match
            if (strstr(INVENTORY->container[i].name, containerName) == NULL)
                continue;
        }

        // loop through container's items
        int j = 0; // current item
        for (j = 0; j < INVENTORY->container[i].amount; j++)
        {
            // item id must exist
            if (INVENTORY->container[i].item[j].id == 0)
                continue;

            // use stack item
            if (bStackItem == 1)
            {
                // item must not already be stacked
                if (INVENTORY->container[i].item[j].count >= MAX_STACK)
                    continue;
            }

            // item ids must match
            if (INVENTORY->container[i].item[j].id == itemId)
            {
                // use get next item
                if (bGetNextItem && !bFoundNext)
                {
                    // next item found
                    bFoundNext = 1;
                    continue;
                }

                item.id            = INVENTORY->container[i].item[j].id;
                //item.count       = INVENTORY->container[i].item[j].count;
                item.container     = i;
                item.containerName = INVENTORY->container[i].name;
                item.position      = j;
                item.bFound        = 1;

                // use item count
                if (itemCount > 0)
                    item.count = itemCount;
                else
                    item.count = INVENTORY->container[i].item[j].count;

                // item found
                return item;
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
    MoveObject(*PLAYER_X,     *PLAYER_Y,     *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(*PLAYER_X - 1, *PLAYER_Y - 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(*PLAYER_X + 1, *PLAYER_Y + 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(*PLAYER_X - 1, *PLAYER_Y + 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(*PLAYER_X + 1, *PLAYER_Y - 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(*PLAYER_X - 1, *PLAYER_Y,     *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(*PLAYER_X + 1, *PLAYER_Y,     *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(*PLAYER_X,     *PLAYER_Y - 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    MoveObject(*PLAYER_X,     *PLAYER_Y + 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
}

void ParseLootFile(int bLootItemsAroundPlayer)
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

            if (bLoot == 1 && *PLAYER_CAP > 0 && strstr(STATUSBAR->text, "You cannot put more objects in this container.") == NULL)
            {
                if (bLootItemsAroundPlayer == 1)
                    LootItemsAroundPlayer(itemId, toSlot);
                else
                    MoveItemToSlot(itemId, "Dead", toSlot);
            }

            if (bStack == 1)
                StackItems(itemId);

            if (bUse == 1 && strstr(STATUSBAR->text, "You are full.") == NULL)
                UseItem(itemId);
        }
    }
    else
    {
        MessageBox(0, "File not found!\nlooter842.txt must be in Tibia folder!\n\nClosing looter...", "Error", MB_ICONERROR | MB_OK);
        FreeLibraryAndExitThread(hMod, 0);
    }
    fclose(file);
}

// dll thread
DWORD WINAPI DllThread()
{
    while(1)
    {
        // loot, stack and use items
        ParseLootFile(0);

        // loot items around player
        if (GetAsyncKeyState(VK_HOME))
            ParseLootFile(1);

        // show item id
        if (GetAsyncKeyState(VK_END))
        {
            char buffer[255];
            if (strstr(SEE_TEXT, "You see"))
                strcpy(buffer, SEE_TEXT);
            else
                strcpy(buffer, "...");
            strcat(buffer, " (Item ID: ");
            char itemId[255];
            itoa(*SEE_ID, itemId, 10);
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
        TriggerEvent2(EVENT_DIALOG, "Looter for Tibia 8.42", "Hello!\n\nPress 'Home' to loot items around yourself from the ground.\nPress 'End' to show the item ID number of the last seen item.\n\nPress 'Pause' or 'Break' to close or exit the looter.");
        StatusbarText("Looter for Tibia 8.42 loaded!", 0);
        CreateThread(0, 0, DllThread, 0, 0, 0);
    }
    return TRUE;
}
