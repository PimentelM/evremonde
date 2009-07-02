/**
 *  Looter DLL for Tibia
 *  by Evremonde
 */

#include "looterdll.h"

// write inventory to file
void WriteInventoryToFile()
{
    FILE *file = fopen(FILENAME_WRITE_INVENTORY_TO_FILE, "w");
    if (file != NULL)
    {
        fprintf(file, "Inventory");
        fprintf(file, "\n");

        int i = 0;
        for (i = 0; i < TIBIA_MAX_CONTAINERS; i++)
        {
            fprintf(file, "....Container #: %i",          i + 1);
            fprintf(file, "\n");
            fprintf(file, "....Container Is Open: %u",    TIBIA_INVENTORY->container[i].bIsOpen);
            fprintf(file, "\n");
            fprintf(file, "....Container ID: %u",         TIBIA_INVENTORY->container[i].id);
            fprintf(file, "\n");
            fprintf(file, "....Container Unknown 1: %u",  TIBIA_INVENTORY->container[i].unknown1);
            fprintf(file, "\n");
            fprintf(file, "....Container Unknown 2: %u",  TIBIA_INVENTORY->container[i].unknown2);
            fprintf(file, "\n");
            fprintf(file, "....Container Name: %s",       TIBIA_INVENTORY->container[i].name);
            fprintf(file, "\n");
            fprintf(file, "....Container Volume: %u",     TIBIA_INVENTORY->container[i].volume);
            fprintf(file, "\n");
            fprintf(file, "....Container Has Parent: %u", TIBIA_INVENTORY->container[i].bHasParent);
            fprintf(file, "\n");
            fprintf(file, "....Container Amount: %u",     TIBIA_INVENTORY->container[i].amount);
            fprintf(file, "\n");

            int j = 0;
            for (j = 0; j < TIBIA_MAX_CONTAINER_ITEMS; j++)
            {
                fprintf(file, "........Item #: %i",       j + 1);
                fprintf(file, "\n");
                fprintf(file, "........Item ID: %u",      TIBIA_INVENTORY->container[i].item[j].id);
                fprintf(file, "\n");
                fprintf(file, "........Item Count: %u",   TIBIA_INVENTORY->container[i].item[j].count);
                fprintf(file, "\n");
                fprintf(file, "........Item Unknown: %u", TIBIA_INVENTORY->container[i].item[j].unknown);
                fprintf(file, "\n");
            }
        }
    }
    fclose(file);
}

// find container item
TibiaContainerItem FindContainerItem(int itemId, int itemCount, char* containerName, int bGetNextContainerItem, int bSkipMaxStackedItems)
{
    TibiaContainerItem containerItem;

    // next container item not found yet
    int bFoundNextContainerItem = 0;

    // loop through containers
    int i = 0;
    for(i = 0; i < TIBIA_MAX_CONTAINERS; i++)
    {
        // container must be open
        if (TIBIA_INVENTORY->container[i].bIsOpen == 0)
            continue;

        // container must not be empty
        if (TIBIA_INVENTORY->container[i].amount == 0)
            continue;

        // use container name
        if (strlen(containerName) > 0)
        {
            // container names must match
            if (strstr(TIBIA_INVENTORY->container[i].name, containerName) == NULL)
                continue;
        }

        // loop through container's items
        int j = 0;
        for (j = 0; j < TIBIA_INVENTORY->container[i].amount; j++)
        {
            // item id must exist
            if (TIBIA_INVENTORY->container[i].item[j].id == 0)
                continue;

            // skip max stacked items
            if (bSkipMaxStackedItems == 1)
            {
                // item must not already be stacked
                if (TIBIA_INVENTORY->container[i].item[j].count >= TIBIA_MAX_STACK)
                    continue;
            }

            // item ids must match
            if (TIBIA_INVENTORY->container[i].item[j].id == itemId)
            {
                // use get next container item
                if (bGetNextContainerItem == 1 && bFoundNextContainerItem == 0)
                {
                    // next container item found
                    bFoundNextContainerItem = 1;
                    continue;
                }

                containerItem.id            = TIBIA_INVENTORY->container[i].item[j].id;
                //containerItemitem.count   = TIBIA_INVENTORY->container[i].item[j].count;
                containerItem.container     = i;
                containerItem.containerName = TIBIA_INVENTORY->container[i].name;
                containerItem.position      = j;
                containerItem.bFound        = 1;

                // use item count
                if (itemCount > 0)
                    containerItem.count = itemCount;
                else
                    containerItem.count = TIBIA_INVENTORY->container[i].item[j].count;

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

// move container item to equipment slot
void MoveContainerItemToEquipmentSlot(int itemId, char* fromContainerName, int toEquipmentSlot)
{
    TibiaContainerItem containerItem = FindContainerItem(itemId, 0, fromContainerName, 0, 0);
    if (containerItem.bFound == 0)
        return;

    Tibia_MoveObject(0xFFFF, TIBIA_FIRST_CONTAINER + containerItem.container, containerItem.position, itemId, containerItem.position, 0xFFFF, toEquipmentSlot, 0x00, containerItem.count);
}

// stack container items
void StackContainerItems(int itemId)
{
    TibiaContainerItem firstContainerItem = FindContainerItem(itemId, 0, "", 0, 1);
    TibiaContainerItem nextContainerItem  = FindContainerItem(itemId, 0, "", 1, 1);
    if (firstContainerItem.bFound == 0 || nextContainerItem.bFound == 0)
        return;

    // do not stack to special containers
    if (strstr(nextContainerItem.containerName, "Dead")        != NULL ||
        strstr(nextContainerItem.containerName, "Locker")      != NULL ||
        strstr(nextContainerItem.containerName, "Depot Chest") != NULL)
        return;

    Tibia_MoveObject(0xFFFF, TIBIA_FIRST_CONTAINER + nextContainerItem.container, nextContainerItem.position, itemId, nextContainerItem.position, 0xFFFF, TIBIA_FIRST_CONTAINER + firstContainerItem.container, firstContainerItem.position, nextContainerItem.count);
}

// use container item
void UseContainerItem(int itemId)
{
    TibiaContainerItem containerItem = FindContainerItem(itemId, 0, "", 0, 0);
    if (containerItem.bFound == 0)
        return;

    Tibia_UseObject(0xFFFF, TIBIA_FIRST_CONTAINER + containerItem.container, containerItem.position, itemId, containerItem.position);
}

// loot items around player
void LootItemsAroundPlayer(int itemId, int toEquipmentSlot)
{
    Tibia_MoveObject(*TIBIA_PLAYER_X,     *TIBIA_PLAYER_Y,     *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_STACK);
    Tibia_MoveObject(*TIBIA_PLAYER_X - 1, *TIBIA_PLAYER_Y - 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_STACK);
    Tibia_MoveObject(*TIBIA_PLAYER_X + 1, *TIBIA_PLAYER_Y + 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_STACK);
    Tibia_MoveObject(*TIBIA_PLAYER_X - 1, *TIBIA_PLAYER_Y + 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_STACK);
    Tibia_MoveObject(*TIBIA_PLAYER_X + 1, *TIBIA_PLAYER_Y - 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_STACK);
    Tibia_MoveObject(*TIBIA_PLAYER_X - 1, *TIBIA_PLAYER_Y,     *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_STACK);
    Tibia_MoveObject(*TIBIA_PLAYER_X + 1, *TIBIA_PLAYER_Y,     *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_STACK);
    Tibia_MoveObject(*TIBIA_PLAYER_X,     *TIBIA_PLAYER_Y - 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_STACK);
    Tibia_MoveObject(*TIBIA_PLAYER_X,     *TIBIA_PLAYER_Y + 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_STACK);
}

// parse looter file
void ParseLooterFile(int parseType)
{
    FILE *file = fopen(FILENAME_PARSE_LOOTER_FILE, "r");
    if (file == NULL)
    {
        Tibia_SetStatusbarText("Error: " FILENAME_PARSE_LOOTER_FILE " file not found! Looter files must be in Tibia folder!", 0);
        fclose(file);
        return;
    }
    fclose(file);

    xmlDocPtr doc;
    doc = xmlParseFile(FILENAME_PARSE_LOOTER_FILE);
    if (doc == NULL)
    {
        Tibia_SetStatusbarText("Error: " FILENAME_PARSE_LOOTER_FILE " file not found or contains syntax errors!", 0);
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
                            int bIsItemEnabled = 0;
                            if (xmlHasProp(nodeItem, (xmlChar *)"enabled"))
                                bIsItemEnabled = atoi(xmlGetProp(nodeItem, (xmlChar *)"enabled"));

                            // skip disabled items
                            if (bIsItemEnabled == 0)
                                continue;

                            int itemId = 0;
                            if (xmlHasProp(nodeItem, (xmlChar *)"id"))
                                itemId = atoi(xmlGetProp(nodeItem, (xmlChar *)"id"));

                            //char* itemName = "";
                            //if (xmlHasProp(nodeItem, (xmlChar *)"name"))
                                //itemName = xmlGetProp(nodeItem, (xmlChar *)"name");

                            char* itemType = "";
                            if (xmlHasProp(nodeItem, (xmlChar *)"type"))
                                itemType = xmlGetProp(nodeItem, (xmlChar *)"type");

                            char* equipmentSlot = "";
                            if (xmlHasProp(nodeItem, (xmlChar *)"slot"))
                                equipmentSlot = xmlGetProp(nodeItem, (xmlChar *)"slot");

                            int toEquipmentSlot = 0;
                            if (strcmp(equipmentSlot, "Head") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_HEAD;
                            if (strcmp(equipmentSlot, "Neck") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_NECK;
                            if (strcmp(equipmentSlot, "Backpack") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_BACKPACK;
                            if (strcmp(equipmentSlot, "Body") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_BODY;
                            if (strcmp(equipmentSlot, "Right") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_RIGHT;
                            if (strcmp(equipmentSlot, "Left") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_LEFT;
                            if (strcmp(equipmentSlot, "Legs") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_LEGS;
                            if (strcmp(equipmentSlot, "Feet") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_FEET;
                            if (strcmp(equipmentSlot, "Ring") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_RING;
                            if (strcmp(equipmentSlot, "Ammo") == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_AMMO;

                            // default to backpack equipment slot
                            if (toEquipmentSlot == 0)
                                toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_BACKPACK;

                            int bIsLootEnabled = 0;
                            if (xmlHasProp(nodeItem, (xmlChar *)"loot"))
                                bIsLootEnabled = atoi(xmlGetProp(nodeItem, (xmlChar *)"loot"));

                            int bIsStackEnabled = 0;
                            if (xmlHasProp(nodeItem, (xmlChar *)"stack"))
                                bIsStackEnabled = atoi(xmlGetProp(nodeItem, (xmlChar *)"stack"));

                            int bIsUseEnabled = 0;
                            if (xmlHasProp(nodeItem, (xmlChar *)"use"))
                                bIsUseEnabled = atoi(xmlGetProp(nodeItem, (xmlChar *)"use"));

                            // loot or loot items around player
                            if (parseType == LOOTER_PARSE_TYPE_LOOT_ITEMS_AROUND_PLAYER || parseType == LOOTER_PARSE_TYPE_LOOT)
                            {
                                if (bIsLootEnabled == 1 && *TIBIA_PLAYER_CAP > 0 && strstr(TIBIA_STATUSBAR->text, "You cannot put more objects in this container.") == NULL)
                                {
                                    if (parseType == LOOTER_PARSE_TYPE_LOOT_ITEMS_AROUND_PLAYER)
                                        LootItemsAroundPlayer(itemId, toEquipmentSlot);
                                    else if (parseType == LOOTER_PARSE_TYPE_LOOT)
                                        MoveContainerItemToEquipmentSlot(itemId, "Dead", toEquipmentSlot);
                                }
                            }

                            // stack
                            if (parseType == LOOTER_PARSE_TYPE_STACK)
                            {
                                if (bIsStackEnabled == 1)
                                    StackContainerItems(itemId);
                            }

                            // use
                            if (parseType == LOOTER_PARSE_TYPE_USE)
                            {
                                if (bIsUseEnabled == 1)
                                {
                                    if (strcmp(itemType, "Food") == 0 && strstr(TIBIA_STATUSBAR->text, "You are full.") == NULL)
                                        UseContainerItem(itemId);
                                }
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
        ParseLooterFile(LOOTER_PARSE_TYPE_LOOT);
        Sleep(100);
    }
    return 0;
}

// stack thread
DWORD WINAPI StackThread()
{
    while (1)
    {
        ParseLooterFile(LOOTER_PARSE_TYPE_STACK);
        Sleep(1000); // stack slow to prevent rapid restacking due to lag
    }
    return 0;
}

// use thread
DWORD WINAPI UseThread()
{
    while (1)
    {
        ParseLooterFile(LOOTER_PARSE_TYPE_USE);
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
            ParseLooterFile(LOOTER_PARSE_TYPE_LOOT_ITEMS_AROUND_PLAYER);

        // show item id
        if (GetAsyncKeyState(VK_END))
        {
            char buffer[512];

            if (strstr(TIBIA_SEE_TEXT, "You see"))
                strcpy(buffer, TIBIA_SEE_TEXT);
            else
                strcpy(buffer, "...");

            strcat(buffer, " (Item ID: ");
            char itemId[128];
            itoa(*TIBIA_SEE_ID, itemId, 10);
            strcat(buffer, itemId);
            strcat(buffer, ")");

            Tibia_SetStatusbarText(buffer, 0);
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

            // free dll and exit thread
            FreeLibraryAndExitThread(hMod, 0);

            // close dll thread handle
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
        Tibia_TriggerEventEx(TIBIA_TRIGGER_EVENT_DIALOG_OK, "Looter for Tibia " TIBIA_VERSION, "Hello!\n\nPress \"Home\" to loot items around yourself from the ground.\nPress \"End\" to show the item ID number of the last seen item.\n\nPress \"Pause\" or \"Break\" to close or exit the looter.\n\nCompile Date: " __DATE__);

        // create threads
        hDllThread  = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DllThread,   (LPVOID)0, 0, 0);

        hThreads[0] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)LootThread,  (LPVOID)1, 0, 0);
        hThreads[1] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StackThread, (LPVOID)2, 0, 0);
        hThreads[2] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UseThread,   (LPVOID)3, 0, 0);

        // loaded
        Tibia_SetStatusbarText("Looter for Tibia " TIBIA_VERSION " loaded!", 0);
    }

    if (fdwReason == DLL_PROCESS_DETACH)
    {
        //MessageBox(0, "fdwReason == DLL_PROCESS_DETACH", "DllMain", MB_OK | MB_ICONINFORMATION);

        // unloaded
        Tibia_SetStatusbarText("Looter for Tibia " TIBIA_VERSION " unloaded!", 0);
    }

    return TRUE;
}
