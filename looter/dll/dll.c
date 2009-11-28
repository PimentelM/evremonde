/**
 *  Looter DLL for Tibia
 *  by Evremonde
 */

#include "dll.h"

// write inventory to file
void LooterWriteInventoryToFile()
{
    cJSON *root;
    root = cJSON_CreateObject();

    cJSON *containers;
    cJSON_AddItemToObject(root, "containers", containers = cJSON_CreateObject());

    int i = 0;
    for (i = 0; i < TIBIA_MAX_CONTAINERS; i++)
    {
        cJSON *container;
        cJSON_AddItemToObject(containers, "container", container = cJSON_CreateObject());

        cJSON_AddNumberToObject(container, "number",    i + 1);
        cJSON_AddNumberToObject(container, "isOpen",    TIBIA_CONTAINERS->container[i].isOpen);
        cJSON_AddNumberToObject(container, "id",        TIBIA_CONTAINERS->container[i].id);
        cJSON_AddNumberToObject(container, "unknown1",  TIBIA_CONTAINERS->container[i].unknown1);
        cJSON_AddNumberToObject(container, "unknown2",  TIBIA_CONTAINERS->container[i].unknown2);
        cJSON_AddStringToObject(container, "name",      TIBIA_CONTAINERS->container[i].name);
        cJSON_AddNumberToObject(container, "volume",    TIBIA_CONTAINERS->container[i].volume);
        cJSON_AddNumberToObject(container, "hasParent", TIBIA_CONTAINERS->container[i].hasParent);
        cJSON_AddNumberToObject(container, "amount",    TIBIA_CONTAINERS->container[i].amount);

        int j = 0;
        for (j = 0; j < TIBIA_MAX_CONTAINER_ITEMS; j++)
        {
            cJSON *item;
            cJSON_AddItemToObject(container, "item", item = cJSON_CreateObject());

            cJSON_AddNumberToObject(item, "number",  j + 1);
            cJSON_AddNumberToObject(item, "id",      TIBIA_CONTAINERS->container[i].item[j].id);
            cJSON_AddNumberToObject(item, "count",   TIBIA_CONTAINERS->container[i].item[j].count);
            cJSON_AddNumberToObject(item, "unknown", TIBIA_CONTAINERS->container[i].item[j].unknown);
        }
    }

    FILE *file = fopen(LOOTER_WRITE_INVENTORY_TO_FILE_NAME, "w");
    if (file != NULL)
    {
        char *out = cJSON_Print(root);
        fprintf(file, "%s\n", out);
        free(out);
    }

    cJSON_Delete(root);
}

// find container item
TibiaContainerItem LooterFindContainerItem(int itemId, int itemCount, int startContainer, int startPosition, char* containerName, int shouldSkipMaxStackedItems)
{
    TibiaContainerItem containerItem;

    // loop through containers
    int i = 0;
    for(i = startContainer; i < TIBIA_MAX_CONTAINERS; i++)
    {
        // container must be open
        if (TIBIA_CONTAINERS->container[i].isOpen == 0)
            continue;

        // container must not be empty
        if (TIBIA_CONTAINERS->container[i].amount == 0)
            continue;

        // use container name
        if (strlen(containerName) > 0)
        {
            // container names must match
            if (strstr(TIBIA_CONTAINERS->container[i].name, containerName) == NULL)
                continue;
        }

        // loop through container's items
        int j = 0;
        for (j = startPosition; j < TIBIA_CONTAINERS->container[i].amount; j++)
        {
            // item id must exist
            if (TIBIA_CONTAINERS->container[i].item[j].id == 0)
                continue;

            // skip max stacked items
            if (shouldSkipMaxStackedItems == 1)
            {
                // item must not already be stacked
                if (TIBIA_CONTAINERS->container[i].item[j].count >= TIBIA_MAX_ITEM_STACK)
                    continue;
            }

            // item ids must match
            if (TIBIA_CONTAINERS->container[i].item[j].id == itemId)
            {
                containerItem.id            = TIBIA_CONTAINERS->container[i].item[j].id;
                //containerItem.count       = TIBIA_CONTAINERS->container[i].item[j].count;
                containerItem.container     = i;
                containerItem.containerName = TIBIA_CONTAINERS->container[i].name;
                containerItem.position      = j;
                containerItem.isFound       = 1;

                // use item count
                if (itemCount > 0)
                    containerItem.count = itemCount;
                else
                    containerItem.count = TIBIA_CONTAINERS->container[i].item[j].count;

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
    containerItem.isFound       = 0;
    return containerItem;
}

// move container item to equipment slot
void LooterMoveContainerItemToEquipmentSlot(int itemId, char* fromContainerName, TibiaEquipmentSlot toEquipmentSlot)
{
    TibiaContainerItem containerItem = LooterFindContainerItem(itemId, 0, 0, 0, fromContainerName, 0);
    if (containerItem.isFound == 0)
        return;

    tibia_move_object(0xFFFF, TIBIA_FIRST_CONTAINER + containerItem.container, containerItem.position, itemId, containerItem.position, 0xFFFF, toEquipmentSlot, 0x00, containerItem.count);
}

// stack container items
void LooterStackContainerItems(int itemId)
{
    TibiaContainerItem firstContainerItem = LooterFindContainerItem(itemId, 0, 0, 0, "", 1);
    TibiaContainerItem nextContainerItem  = LooterFindContainerItem(itemId, 0, firstContainerItem.container, firstContainerItem.position + 1, "", 1);
    if (firstContainerItem.isFound == 0 || nextContainerItem.isFound == 0)
        return;

    // do not stack to special containers
    if (strstr(nextContainerItem.containerName, "Dead")        != NULL ||
        strstr(nextContainerItem.containerName, "Locker")      != NULL ||
        strstr(nextContainerItem.containerName, "Depot Chest") != NULL)
        return;

    tibia_move_object(0xFFFF, TIBIA_FIRST_CONTAINER + nextContainerItem.container, nextContainerItem.position, itemId, nextContainerItem.position, 0xFFFF, TIBIA_FIRST_CONTAINER + firstContainerItem.container, firstContainerItem.position, nextContainerItem.count);
}

// use container item
void LooterUseContainerItem(int itemId)
{
    TibiaContainerItem containerItem = LooterFindContainerItem(itemId, 0, 0, 0, "", 0);
    if (containerItem.isFound == 0)
        return;

    tibia_use_object(0xFFFF, TIBIA_FIRST_CONTAINER + containerItem.container, containerItem.position, itemId, containerItem.position);
}

// loot items around player
void LooterLootItemsAroundPlayer(int itemId, TibiaEquipmentSlot toEquipmentSlot)
{
    tibia_move_object(*TIBIA_PLAYER_X,     *TIBIA_PLAYER_Y,     *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_ITEM_STACK);
    tibia_move_object(*TIBIA_PLAYER_X - 1, *TIBIA_PLAYER_Y - 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_ITEM_STACK);
    tibia_move_object(*TIBIA_PLAYER_X + 1, *TIBIA_PLAYER_Y + 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_ITEM_STACK);
    tibia_move_object(*TIBIA_PLAYER_X - 1, *TIBIA_PLAYER_Y + 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_ITEM_STACK);
    tibia_move_object(*TIBIA_PLAYER_X + 1, *TIBIA_PLAYER_Y - 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_ITEM_STACK);
    tibia_move_object(*TIBIA_PLAYER_X - 1, *TIBIA_PLAYER_Y,     *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_ITEM_STACK);
    tibia_move_object(*TIBIA_PLAYER_X + 1, *TIBIA_PLAYER_Y,     *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_ITEM_STACK);
    tibia_move_object(*TIBIA_PLAYER_X,     *TIBIA_PLAYER_Y - 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_ITEM_STACK);
    tibia_move_object(*TIBIA_PLAYER_X,     *TIBIA_PLAYER_Y + 1, *TIBIA_PLAYER_Z, itemId, 1, 0xFFFF, toEquipmentSlot, 0x0, TIBIA_MAX_ITEM_STACK);
}

// parse items
void LooterParseItems(LooterParseItemsType parseItemsType)
{
    FILE *file = fopen(LOOTER_PARSE_ITEMS_FILE_NAME, "rb");

    // check if looter parse items file exists
    if (file == NULL)
    {
        tibia_set_statusbar_text("Error: " LOOTER_PARSE_ITEMS_FILE_NAME " file not found! " APPLICATION_NAME " files must be inside the Tibia folder!", 0);
        return;
    }

    // allocate data buffer and read text from parse items file to data buffer
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    fread(data, 1, length, file);

    fclose(file);

    // parse items file
    cJSON *root = cJSON_Parse(data);

    // free data buffer
    free(data);

    // find items
    cJSON *items = cJSON_GetObjectItem(root, "items");

    // loop through all items
    cJSON *item = items->child;
    while (item)
    {
        int itemIsEnabled = cJSON_GetObjectItem(item, "enabled")->type;

        // skip disabled items
        if (itemIsEnabled == 0)
        {
            // go to next item
            item = item->next;

            continue;
        }

        int itemId = cJSON_GetObjectItem(item, "id")->valueint;
        //char *itemName = cJSON_GetObjectItem(item, "name")->valuestring;
        char *itemType = cJSON_GetObjectItem(item, "type")->valuestring;
        char *itemSlot = cJSON_GetObjectItem(item, "slot")->valuestring;

        TibiaEquipmentSlot toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_BACKPACK; // default to backpack equipment slot
        if (strcmp(itemSlot, "head") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_HEAD;
        if (strcmp(itemSlot, "neck") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_NECK;
        if (strcmp(itemSlot, "backpack") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_BACKPACK;
        if (strcmp(itemSlot, "body") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_BODY;
        if (strcmp(itemSlot, "right") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_RIGHT;
        if (strcmp(itemSlot, "left") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_LEFT;
        if (strcmp(itemSlot, "legs") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_LEGS;
        if (strcmp(itemSlot, "feet") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_FEET;
        if (strcmp(itemSlot, "ring") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_RING;
        if (strcmp(itemSlot, "ammo") == 0)
            toEquipmentSlot = TIBIA_EQUIPMENT_SLOT_AMMO;

        int itemIsLootEnabled  = cJSON_GetObjectItem(item, "loot")->type;
        int itemIsStackEnabled = cJSON_GetObjectItem(item, "stack")->type;
        int itemIsUseEnabled   = cJSON_GetObjectItem(item, "use")->type;

        // loot or loot items around player
        if (parseItemsType == LOOTER_PARSE_ITEMS_TYPE_LOOT || parseItemsType == LOOTER_PARSE_ITEMS_TYPE_LOOT_ITEMS_AROUND_PLAYER)
        {
            if (itemIsLootEnabled == 1)
            {
                // check if player has enough cap or if container is full
                if (*TIBIA_PLAYER_CAP == 0 || *TIBIA_PLAYER_CAP < 1 || strstr(TIBIA_STATUSBAR->text, "You cannot put more objects in this container.") != NULL)
                {
                    // go to next item
                    item = item->next;

                    continue;
                }

                if (parseItemsType == LOOTER_PARSE_ITEMS_TYPE_LOOT)
                    LooterMoveContainerItemToEquipmentSlot(itemId, "Dead", toEquipmentSlot);
                else if (parseItemsType == LOOTER_PARSE_ITEMS_TYPE_LOOT_ITEMS_AROUND_PLAYER)
                    LooterLootItemsAroundPlayer(itemId, toEquipmentSlot);
            }
        }

        // stack
        if (parseItemsType == LOOTER_PARSE_ITEMS_TYPE_STACK)
            if (itemIsStackEnabled == 1)
                LooterStackContainerItems(itemId);

        // use
        if (parseItemsType == LOOTER_PARSE_ITEMS_TYPE_USE)
        {
            if (itemIsUseEnabled == 1)
            {
                // do not use food if player is full
                if (strcmp(itemType, "food") == 0 && strstr(TIBIA_STATUSBAR->text, "You are full.") != NULL)
                {
                    // go to next item
                    item = item->next;

                    continue;
                }

                LooterUseContainerItem(itemId);
            }
        }

        // go to next item
        item = item->next;
    }

    // free parsed items file
    cJSON_Delete(root);
}

// show item id
void LooterShowItemId()
{
    char buffer[1024];

    if (strstr(TIBIA_SEE_TEXT, "You see"))
        strcpy(buffer, TIBIA_SEE_TEXT);
    else
        strcpy(buffer, "...");

    strcat(buffer, " (Item ID: ");
    char itemId[128];
    itoa(*TIBIA_SEE_ID, itemId, 10);
    strcat(buffer, itemId);
    strcat(buffer, ")");

    tibia_set_statusbar_text(buffer, 0);
}

// loot
void timerLoot(HWND hwnd)
{
    LooterParseItems(LOOTER_PARSE_ITEMS_TYPE_LOOT);
}


// stack
void timerStack(HWND hwnd)
{
    LooterParseItems(LOOTER_PARSE_ITEMS_TYPE_STACK);
}

// use
void timerUse(HWND hwnd)
{
    LooterParseItems(LOOTER_PARSE_ITEMS_TYPE_USE);
}

// WM_CREATE
void onCreate(HWND hwnd)
{
    // register hotkeys
    RegisterHotKey(hwnd, ID_HOTKEY_LOOT_ITEMS_AROUND_PLAYER, 0, VK_HOME);
    RegisterHotKey(hwnd, ID_HOTKEY_SHOW_ITEM_ID,             0, VK_END);
    RegisterHotKey(hwnd, ID_HOTKEY_EXIT,                     0, VK_PAUSE);

    // set timers
    SetTimer(hwnd, ID_TIMER_LOOT,  100,  0); // loot fast
    SetTimer(hwnd, ID_TIMER_STACK, 1000, 0); // stack slow to prevent rapid restacking due to lag
    SetTimer(hwnd, ID_TIMER_USE,   100,  0); // use fast
}

// WM_DESTROY
void onDestroy(HWND hwnd)
{
    // unregister hotkeys
    UnregisterHotKey(hwnd, ID_HOTKEY_LOOT_ITEMS_AROUND_PLAYER);
    UnregisterHotKey(hwnd, ID_HOTKEY_SHOW_ITEM_ID);
    UnregisterHotKey(hwnd, ID_HOTKEY_EXIT);

    // kill timers
    KillTimer(hwnd, ID_TIMER_LOOT);
    KillTimer(hwnd, ID_TIMER_STACK);
    KillTimer(hwnd, ID_TIMER_USE);

    // free dll and exit thread
    FreeLibraryAndExitThread(hMod, 0);

    // close main window thread handle
    CloseHandle(hMainWindowThread);

    // exit
    PostQuitMessage(0);
}

// WM_HOTKEY
void onHotkey(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // hotkeys
    switch (wparam)
    {
        // loot items around player
        case ID_HOTKEY_LOOT_ITEMS_AROUND_PLAYER:
            LooterParseItems(LOOTER_PARSE_ITEMS_TYPE_LOOT_ITEMS_AROUND_PLAYER);
            break;

        // show item id
        case ID_HOTKEY_SHOW_ITEM_ID:
            LooterShowItemId();
            break;

        // exit
        case ID_HOTKEY_EXIT:
            DestroyWindow(hwnd); // exit
            break;
    }

}

// WM_TIMER
void onTimer(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // timers
    switch (wparam)
    {
        // loot
        case ID_TIMER_LOOT:
            timerLoot(hwnd);
            break;

        // stack
        case ID_TIMER_STACK:
            timerStack(hwnd);
            break;

        // use
        case ID_TIMER_USE:
            timerUse(hwnd);
            break;
    }
}

// main window procedure
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd); // exit
            break;

        case WM_CREATE:
            onCreate(hwnd);
            break;

        case WM_DESTROY:
            onDestroy(hwnd);
            break;

        case WM_HOTKEY:
            onHotkey(hwnd, wparam, lparam);
            break;

       case WM_TIMER:
            onTimer(hwnd, wparam, lparam);
            break;

        default:
            return DefWindowProc(hwnd, umsg, wparam, lparam);
    }
    return 0;
}

// create main window
DWORD WINAPI CreateMainWindow()
{
    // register window class
    WNDCLASSEX wcx;
    wcx.cbSize        = sizeof(WNDCLASSEX);
    wcx.style         = 0;
    wcx.lpfnWndProc   = MainWindowProc; // window messages handler
    wcx.cbClsExtra    = 0;
    wcx.cbWndExtra    = 0;
    wcx.hInstance     = GetModuleHandle(NULL);
    wcx.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
    wcx.hIconSm       = LoadIcon(NULL, IDI_WINLOGO);
    wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); // window background color
    wcx.lpszMenuName  = NULL;
    wcx.lpszClassName = APPLICATION_CLASS_NAME;

    RegisterClassEx(&wcx);

    // create window
    HWND hwnd;
    hwnd = CreateWindowEx(WS_EX_DLGMODALFRAME, // extended window styles
                          APPLICATION_CLASS_NAME, APPLICATION_NAME, // window class and title
                          WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window styles
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          WINDOW_HEIGHT, WINDOW_WIDTH, // window dimensions
                          HWND_DESKTOP, NULL, GetModuleHandle(NULL), NULL);

    // show window
    //ShowWindow(hwnd, SW_SHOW);
    //UpdateWindow(hwnd);

    // message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

// dll main
BOOL APIENTRY DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
    hMod = hModule;

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        //MessageBox(0, "fdwReason == DLL_PROCESS_ATTACH", "DllMain", MB_OK | MB_ICONINFORMATION);

        // welcome message
        tibia_trigger_event_ex(TIBIA_TRIGGER_EVENT_DIALOG_OK, APPLICATION_NAME " for Tibia " TIBIA_VERSION, "Hello!\n\nPress \"Home\" to loot items around yourself from the ground.\nPress \"End\" to show the item ID number of the last seen item.\n\nPress \"Pause\" or \"Break\" to close or exit the " APPLICATION_NAME ".\n\nCompile Date: " __DATE__ "\nHomepage: " APPLICATION_HOMEPAGE);

        // create main window
        hMainWindowThread = CreateThread(NULL, 0, CreateMainWindow, 0, 0, NULL);

        // loaded
        tibia_set_statusbar_text(APPLICATION_NAME " for Tibia " TIBIA_VERSION " loaded!", 0);
    }

    if (fdwReason == DLL_PROCESS_DETACH)
    {
        //MessageBox(0, "fdwReason == DLL_PROCESS_DETACH", "DllMain", MB_OK | MB_ICONINFORMATION);

        // unloaded
        tibia_set_statusbar_text(APPLICATION_NAME " for Tibia " TIBIA_VERSION " unloaded!", 0);
    }

    return TRUE;
}
