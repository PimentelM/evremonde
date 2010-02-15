/**
 *  Team Fortress 2 items_game
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

#include "cjson_helper.h"

#include "tf2.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: items_game items_game-in.txt items_game-out.txt\n");
        getchar();
        return 1;
    }

    printf("Compiler Version: gcc " __VERSION__ "\n");
    printf("Build Date: " __DATE__ "\n");
    printf("Build Time: " __TIME__ "\n");

    cJSON *itemsgame_root = cjson_helper_parse_file(argv[1]);
    if (itemsgame_root == NULL)
    {
        printf("Error: File \"%s\" could not be parsed.\n", argv[1]);
        getchar();
        return 1;
    }

    cJSON *itemsgame_items_game = cJSON_GetObjectItem(itemsgame_root, "items_game");
    cJSON *itemsgame_items = cJSON_GetObjectItem(itemsgame_items_game, "items");
    cJSON *itemsgame_item = itemsgame_items->child;

    char *itemsgame_out_data;

    cJSON *itemsgame_out_root = cJSON_CreateObject();

    cJSON *itemsgame_out_items;
    cJSON_AddItemToObject(itemsgame_out_root, "items", itemsgame_out_items = cJSON_CreateObject());
    
    while (itemsgame_item)
    {
        cJSON *itemsgame_out_item;
        cJSON_AddItemToObject(itemsgame_out_items, "item", itemsgame_out_item = cJSON_CreateObject());

        int itemsgame_item_id = atoi(itemsgame_item->string);
        cJSON_AddNumberToObject(itemsgame_out_item, "id", itemsgame_item_id);
        printf("id: %d\n", itemsgame_item_id);

        char *itemsgame_item_name = cJSON_GetObjectItem(itemsgame_item, "name")->valuestring;
        cJSON_AddStringToObject(itemsgame_out_item, "name", itemsgame_item_name);
        printf("\tname: %s\n", itemsgame_item_name);

        int itemsgame_item_bool_craft_class     = 0;
        int itemsgame_item_bool_item_slot       = 0;
        int itemsgame_item_bool_item_quality    = 0;
        int itemsgame_item_bool_used_by_classes = 0;

        cJSON *itemsgame_item_attr = itemsgame_item->child;
        while (itemsgame_item_attr)
        {
            if (strcmp(itemsgame_item_attr->string, "craft_class") == 0)
                itemsgame_item_bool_craft_class = 1;

            if (strcmp(itemsgame_item_attr->string, "item_slot") == 0)
                itemsgame_item_bool_item_slot = 1;

            if (strcmp(itemsgame_item_attr->string, "item_quality") == 0)
                itemsgame_item_bool_item_quality = 1;

            if (strcmp(itemsgame_item_attr->string, "used_by_classes") == 0)
                itemsgame_item_bool_used_by_classes = 1;

            itemsgame_item_attr = itemsgame_item_attr->next;
        }

        if (itemsgame_item_bool_craft_class == 1)
        {
            char *itemsgame_item_craft_class = cJSON_GetObjectItem(itemsgame_item, "craft_class")->valuestring;
            cJSON_AddStringToObject(itemsgame_out_item, "craft_class", itemsgame_item_craft_class);
            printf("\tcraft_class: %s\n", itemsgame_item_craft_class);
        }

        if (itemsgame_item_bool_item_slot == 1)
        {
            char *itemsgame_item_item_slot = cJSON_GetObjectItem(itemsgame_item, "item_slot")->valuestring;
            cJSON_AddStringToObject(itemsgame_out_item, "item_slot", itemsgame_item_item_slot);
            printf("\titem_slot: %s\n", itemsgame_item_item_slot);
        }

        if (itemsgame_item_bool_item_quality == 1)
        {
            char *itemsgame_item_item_quality = cJSON_GetObjectItem(itemsgame_item, "item_quality")->valuestring;
            cJSON_AddStringToObject(itemsgame_out_item, "item_quality", itemsgame_item_item_quality);
            printf("\titem_quality: %s\n", itemsgame_item_item_quality);
        }

        if (itemsgame_item_bool_used_by_classes == 1)
        {
            cJSON *itemsgame_item_used_by_classes = cJSON_GetObjectItem(itemsgame_item, "used_by_classes");
            printf("\tused_by_classes: ");

            cJSON *itemsgame_item_used_by_classes_array;
            cJSON_AddItemToObject(itemsgame_out_item, "used_by_classes", itemsgame_item_used_by_classes_array = cJSON_CreateArray());

            cJSON *itemsgame_item_used_by_classes_array_item;
            cJSON_AddItemToArray(itemsgame_item_used_by_classes_array, itemsgame_item_used_by_classes_array_item = cJSON_CreateObject());

            cJSON *itemsgame_item_used_by_classes_attr = itemsgame_item_used_by_classes->child;
            while (itemsgame_item_used_by_classes_attr)
            {
                printf("%s ", itemsgame_item_used_by_classes_attr->string);

                if (strcmp(itemsgame_item_used_by_classes_attr->valuestring, "1") == 0)
                    cJSON_AddTrueToObject(itemsgame_item_used_by_classes_array_item, itemsgame_item_used_by_classes_attr->string);
                else
                    cJSON_AddFalseToObject(itemsgame_item_used_by_classes_array_item, itemsgame_item_used_by_classes_attr->string);

                itemsgame_item_used_by_classes_attr = itemsgame_item_used_by_classes_attr->next;
            }

            printf("\n");
        }

        /*
            cJSON *itemsgame_item_attr = itemsgame_item->child;
            while (itemsgame_item_attr)
            {
                printf("item attr: %s\n", itemsgame_item_attr->string);

                itemsgame_item_attr = itemsgame_item_attr->next;
            }
        */

        itemsgame_item = itemsgame_item->next;
    }

    itemsgame_out_data = cJSON_Print(itemsgame_out_root);

    FILE *itemsgame_out_file = fopen(argv[2], "w");
    if (itemsgame_out_file == NULL)
    {
        printf("Error: File \"%s\" could not be opened.\n", argv[2]);
        getchar();
        return 1;
    }

    fprintf(itemsgame_out_file, itemsgame_out_data);

    fclose(itemsgame_out_file);

    cJSON_Delete(itemsgame_out_root);

    cJSON_Delete(itemsgame_root);

    free(itemsgame_out_data);

    return 0;
}
