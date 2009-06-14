/**
 *  Looter DLL for Tibia 8.42
 *  by Evremonde
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
            fprintf(file, "....Container #: %i",          i + 1);
            fprintf(file, "\n");
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
                fprintf(file, "........Item #: %i",       j + 1);
                fprintf(file, "\n");
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

// find container item
ContainerItem FindContainerItem(int itemId, int itemCount, char* containerName, int bGetNextContainerItem, int bSkipMaxStackedItems)
{
    ContainerItem containerItem;

    // next container item not found yet
    int bFoundNextContainerItem = 0;

    // loop through containers
    int i = 0;
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
        int j = 0;
        for (j = 0; j < INVENTORY->container[i].amount; j++)
        {
            // item id must exist
            if (INVENTORY->container[i].item[j].id == 0)
                continue;

            // skip max stacked items
            if (bSkipMaxStackedItems == 1)
            {
                // item must not already be stacked
                if (INVENTORY->container[i].item[j].count >= MAX_STACK)
                    continue;
            }

            // item ids must match
            if (INVENTORY->container[i].item[j].id == itemId)
            {
                // use get next container item
                if (bGetNextContainerItem && !bFoundNextContainerItem)
                {
                    // next container item found
                    bFoundNextContainerItem = 1;
                    continue;
                }

                containerItem.id            = INVENTORY->container[i].item[j].id;
                //containerItemitem.count   = INVENTORY->container[i].item[j].count;
                containerItem.container     = i;
                containerItem.containerName = INVENTORY->container[i].name;
                containerItem.position      = j;
                containerItem.bFound        = 1;

                // use item count
                if (itemCount > 0)
                    containerItem.count = itemCount;
                else
                    containerItem.count = INVENTORY->container[i].item[j].count;

                // container item found
                return containerItem;
            }
        }
    }

    // container item not found
    containerItem.id            = 0;
    containerItem.count         = 0;
    containerItem.container     = 0;
    containerItem.containerName = "";
    containerItem.position      = 0;
    containerItem.bFound        = 0;
    return containerItem;
}

// move container item to slot
void MoveContainerItemToSlot(int itemId, char* fromContainerName, int toSlot)
{
    ContainerItem containerItem = FindContainerItem(itemId, 0, fromContainerName, 0, 0);
    if (containerItem.bFound == 0)
        return;

    Tibia_MoveObject(0xFFFF, 0x40 + containerItem.container, containerItem.position, itemId, containerItem.position, 0xFFFF, toSlot, 0x00, containerItem.count);
}

// stack container items
void StackContainerItems(int itemId)
{
    ContainerItem firstContainerItem = FindContainerItem(itemId, 0, "", 0, 1);
    ContainerItem nextContainerItem  = FindContainerItem(itemId, 0, "", 1, 1);
    if (firstContainerItem.bFound == 0 || nextContainerItem.bFound == 0)
        return;

    // do not stack to special containers
    if (strstr(nextContainerItem.containerName, "Dead")        != NULL ||
        strstr(nextContainerItem.containerName, "Locker")      != NULL ||
        strstr(nextContainerItem.containerName, "Depot Chest") != NULL)
        return;

    Tibia_MoveObject(0xFFFF, 0x40 + nextContainerItem.container, nextContainerItem.position, itemId, nextContainerItem.position, 0xFFFF, 0x40 + firstContainerItem.container, firstContainerItem.position, nextContainerItem.count);
}

// use container item
void UseContainerItem(int itemId)
{
    ContainerItem containerItem = FindContainerItem(itemId, 0, "", 0, 0);
    if (containerItem.bFound == 0)
        return;

    Tibia_UseObject(0xFFFF, 0x40 + containerItem.container, containerItem.position, itemId, containerItem.position);
}

// loot items around player
void LootItemsAroundPlayer(int itemId, int toSlot)
{
    Tibia_MoveObject(*PLAYER_X,     *PLAYER_Y,     *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    Tibia_MoveObject(*PLAYER_X - 1, *PLAYER_Y - 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    Tibia_MoveObject(*PLAYER_X + 1, *PLAYER_Y + 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    Tibia_MoveObject(*PLAYER_X - 1, *PLAYER_Y + 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    Tibia_MoveObject(*PLAYER_X + 1, *PLAYER_Y - 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    Tibia_MoveObject(*PLAYER_X - 1, *PLAYER_Y,     *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    Tibia_MoveObject(*PLAYER_X + 1, *PLAYER_Y,     *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    Tibia_MoveObject(*PLAYER_X,     *PLAYER_Y - 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
    Tibia_MoveObject(*PLAYER_X,     *PLAYER_Y + 1, *PLAYER_Z, itemId, 1, 0xFFFF, toSlot, 0x0, MAX_STACK);
}

// parse looter file
void ParseLooterFile(int parseType)
{
    // parse type
    // 0 = loot items around player
    // 1 = loot
    // 2 = stack
    // 3 = use

    xmlDocPtr doc;
    doc = xmlParseFile("looter842.xml");
    if (doc == NULL)
    {
        Tibia_StatusbarText("Error: looter842.xml not found! File must be in Tibia folder!", 0);
        xmlFreeDoc(doc);
        return;
    }

    xmlNodePtr nodeItems;
    xmlNodePtr nodeItem;

    for (nodeItems = doc->children; nodeItems != NULL; nodeItems = nodeItems->next)
    {
        if (nodeItems->type == XML_ELEMENT_NODE)
        {
            if (xmlStrcmp(nodeItems->name, (xmlChar *)"items") == 0)
            {
                for (nodeItem = nodeItems->children; nodeItem != NULL; nodeItem = nodeItem->next)
                {
                    if (nodeItem->type == XML_ELEMENT_NODE)
                    {
                        if (xmlStrcmp(nodeItem->name, (xmlChar *)"item") == 0)
                        {
                            int itemId = 0;
                            if (xmlHasProp(nodeItem, (xmlChar *)"id"))
                                itemId = atoi(xmlGetProp(nodeItem, (xmlChar *)"id"));

                            //char* name = "";
                            //if (xmlHasProp(nodeItem, (xmlChar *)"name"))
                                //name = xmlGetProp(nodeItem, (xmlChar *)"name");

                            char* slot = "";
                            if (xmlHasProp(nodeItem, (xmlChar *)"slot"))
                                slot = xmlGetProp(nodeItem, (xmlChar *)"slot");

                            int toSlot = 0;
                            if (strcmp(slot, "Head") == 0)
                                toSlot = EQUIPMENT_SLOT_HEAD;
                            if (strcmp(slot, "Neck") == 0)
                                toSlot = EQUIPMENT_SLOT_NECK;
                            if (strcmp(slot, "Backpack") == 0)
                                toSlot = EQUIPMENT_SLOT_BACKPACK;
                            if (strcmp(slot, "Body") == 0)
                                toSlot = EQUIPMENT_SLOT_BODY;
                            if (strcmp(slot, "Right") == 0)
                                toSlot = EQUIPMENT_SLOT_RIGHT;
                            if (strcmp(slot, "Left") == 0)
                                toSlot = EQUIPMENT_SLOT_LEFT;
                            if (strcmp(slot, "Legs") == 0)
                                toSlot = EQUIPMENT_SLOT_LEGS;
                            if (strcmp(slot, "Feet") == 0)
                                toSlot = EQUIPMENT_SLOT_FEET;
                            if (strcmp(slot, "Ring") == 0)
                                toSlot = EQUIPMENT_SLOT_RING;
                            if (strcmp(slot, "Ammo") == 0)
                                toSlot = EQUIPMENT_SLOT_AMMO;

                            int bLoot = 0;
                            if (xmlHasProp(nodeItem, (xmlChar *)"loot"))
                                bLoot = atoi(xmlGetProp(nodeItem, (xmlChar *)"loot"));

                            int bStack = 0;
                            if (xmlHasProp(nodeItem, (xmlChar *)"stack"))
                                bStack = atoi(xmlGetProp(nodeItem, (xmlChar *)"stack"));

                            int bUse = 0;
                            if (xmlHasProp(nodeItem, (xmlChar *)"use"))
                                bUse = atoi(xmlGetProp(nodeItem, (xmlChar *)"use"));

                            if (parseType == 0 || parseType == 1)
                            {
                                if (bLoot == 1 && *PLAYER_CAP > 0 && strstr(STATUSBAR->text, "You cannot put more objects in this container.") == NULL)
                                {
                                    if (parseType == 0)
                                        LootItemsAroundPlayer(itemId, toSlot);
                                    else if (parseType == 1)
                                        MoveContainerItemToSlot(itemId, "Dead", toSlot);
                                }
                            }

                            if (parseType == 2)
                            {
                                if (bStack == 1)
                                    StackContainerItems(itemId);
                            }

                            if (parseType == 3)
                            {
                                if (bUse == 1 && strstr(STATUSBAR->text, "You are full.") == NULL)
                                    UseContainerItem(itemId);
                            }
                        }
                    }
                }
            }
        }
    }

    xmlFreeDoc(doc);
}

// loot thread
DWORD WINAPI LootThread()
{
    while (1)
    {
        ParseLooterFile(1);
        Sleep(100);
    }
    return 0;
}

// stack thread
DWORD WINAPI StackThread()
{
    while (1)
    {
        ParseLooterFile(2);
        Sleep(1000); // stack slow to prevent rapid restacking
    }
    return 0;
}

// use thread
DWORD WINAPI UseThread()
{
    while (1)
    {
        ParseLooterFile(3);
        Sleep(100);
    }
    return 0;
}

// dll thread
DWORD WINAPI DllThread()
{
    while (1)
    {
        // loot items around player
        if (GetAsyncKeyState(VK_HOME))
            ParseLooterFile(0);

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
            Tibia_StatusbarText(buffer, 0);
        }

        // uninject dll
        if (GetAsyncKeyState(VK_PAUSE))
        {
            // terminate and close threads
            int i = 0;
            for (i = 0; i < nThreads; i++)
            {
                DWORD dwExitCode;
                GetExitCodeThread(hThreads[i], &dwExitCode);
                TerminateThread(hThreads[i], dwExitCode);
                CloseHandle(hThreads[i]);
            }

            // wait for threads to close
            WaitForMultipleObjects(nThreads, hThreads, TRUE, INFINITE);

            // uninject dll and exit thread
            FreeLibraryAndExitThread(hMod, 0);

            // close dll thread
            CloseHandle(hDllThread);
        }

        Sleep(100);
    }

    return 0;
}

// dll main
BOOL APIENTRY DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    hMod = hModule;

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        //MessageBox(0, "fdwReason == DLL_PROCESS_ATTACH", "DllMain", MB_OK | MB_ICONINFORMATION);

        // welcome message
        Tibia_TriggerEventEx(TRIGGER_EVENT_DIALOG_OK, "Looter for Tibia 8.42", "Hello!\n\nPress 'Home' to loot items around yourself from the ground.\nPress 'End' to show the item ID number of the last seen item.\n\nPress 'Pause' or 'Break' to close or exit the looter.");

        // create threads
        hDllThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DllThread, (LPVOID)0, 0, 0);

        hThreads[0] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)LootThread,  (LPVOID)1, 0, 0);
        hThreads[1] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StackThread, (LPVOID)2, 0, 0);
        hThreads[2] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UseThread,   (LPVOID)3, 0, 0);

        // loaded
        Tibia_StatusbarText("Looter for Tibia 8.42 loaded!", 0);
    }

    if (fdwReason == DLL_PROCESS_DETACH)
    {
        //MessageBox(0, "fdwReason == DLL_PROCESS_DETACH", "DllMain", MB_OK | MB_ICONINFORMATION);

        // unloaded
        Tibia_StatusbarText("Looter for Tibia 8.42 unloaded!", 0);
    }

    return TRUE;
}
