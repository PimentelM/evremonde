/**
 *  Team Fortress 2 items_game
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

#include "cjson_helper.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: items_game input output\n");
        return 1;
    }

    printf("Compiler Version: gcc " __VERSION__ "\n");
    printf("Build Date: "           __DATE__    "\n");
    printf("Build Time: "           __TIME__    "\n");

    cJSON *items_game_root = cjson_helper_parse_file(argv[1]);
    if (items_game_root == NULL)
    {
        printf("Error: File \"%s\" could not be parsed.\n", argv[1]);
        return 1;
    }

    cJSON *items_game_items_game = cJSON_GetObjectItem(items_game_root, "items_game");
    cJSON *items_game_items = cJSON_GetObjectItem(items_game_items_game, "items");
    cJSON *items_game_item = items_game_items->child;

    char *items_game_out_data;

    cJSON *items_game_out_root = cJSON_CreateObject();

    cJSON *items_game_out_items;
    cJSON_AddItemToObject(items_game_out_root, "items", items_game_out_items = cJSON_CreateArray());
    
    while (items_game_item)
    {
        cJSON *items_game_out_item;
        cJSON_AddItemToArray(items_game_out_items, items_game_out_item = cJSON_CreateObject());

        int items_game_item_id = atoi(items_game_item->string);
        cJSON_AddNumberToObject(items_game_out_item, "id", items_game_item_id);
        printf("id: %d\n", items_game_item_id);

        char *items_game_item_name = cJSON_GetObjectItem(items_game_item, "name")->valuestring;
        cJSON_AddStringToObject(items_game_out_item, "name", items_game_item_name);
        printf("\tname: %s\n", items_game_item_name);

        int items_game_item_bool_craft_class     = 0;
        int items_game_item_bool_item_slot       = 0;
        int items_game_item_bool_item_quality    = 0;
        int items_game_item_bool_image_inventory = 0;
        int items_game_item_bool_used_by_classes = 0;

        cJSON *items_game_item_attr = items_game_item->child;
        while (items_game_item_attr)
        {
            if (strcmp(items_game_item_attr->string, "craft_class") == 0)
                items_game_item_bool_craft_class = 1;

            if (strcmp(items_game_item_attr->string, "item_slot") == 0)
                items_game_item_bool_item_slot = 1;

            if (strcmp(items_game_item_attr->string, "item_quality") == 0)
                items_game_item_bool_item_quality = 1;

            if (strcmp(items_game_item_attr->string, "image_inventory") == 0)
                items_game_item_bool_image_inventory = 1;

            if (strcmp(items_game_item_attr->string, "used_by_classes") == 0)
                items_game_item_bool_used_by_classes = 1;

            items_game_item_attr = items_game_item_attr->next;
        }

        if (items_game_item_bool_craft_class == 1)
        {
            char *items_game_item_craft_class = cJSON_GetObjectItem(items_game_item, "craft_class")->valuestring;
            cJSON_AddStringToObject(items_game_out_item, "craft_class", items_game_item_craft_class);
            printf("\tcraft_class: %s\n", items_game_item_craft_class);
        }

        if (items_game_item_bool_item_slot == 1)
        {
            char *items_game_item_item_slot = cJSON_GetObjectItem(items_game_item, "item_slot")->valuestring;
            cJSON_AddStringToObject(items_game_out_item, "item_slot", items_game_item_item_slot);
            printf("\titem_slot: %s\n", items_game_item_item_slot);
        }

        if (items_game_item_bool_item_quality == 1)
        {
            char *items_game_item_item_quality = cJSON_GetObjectItem(items_game_item, "item_quality")->valuestring;
            cJSON_AddStringToObject(items_game_out_item, "item_quality", items_game_item_item_quality);
            printf("\titem_quality: %s\n", items_game_item_item_quality);
        }

        if (items_game_item_bool_image_inventory == 1)
        {
            char *items_game_item_image_inventory = cJSON_GetObjectItem(items_game_item, "image_inventory")->valuestring;
            cJSON_AddStringToObject(items_game_out_item, "image_inventory", items_game_item_image_inventory);
            printf("\timage_inventory: ");
        }

        if (items_game_item_bool_used_by_classes == 1)
        {
            cJSON *items_game_item_used_by_classes = cJSON_GetObjectItem(items_game_item, "used_by_classes");
            printf("\tused_by_classes: ");

            cJSON *items_game_item_used_by_classes_item;
            cJSON_AddItemToObject(items_game_out_item, "used_by_classes", items_game_item_used_by_classes_item = cJSON_CreateObject());

            cJSON *items_game_item_used_by_classes_attr = items_game_item_used_by_classes->child;
            while (items_game_item_used_by_classes_attr)
            {
                printf("%s ", items_game_item_used_by_classes_attr->string);

                if (strcmp(items_game_item_used_by_classes_attr->valuestring, "1") == 0)
                    cJSON_AddTrueToObject(items_game_item_used_by_classes_item, items_game_item_used_by_classes_attr->string);
                else
                    cJSON_AddFalseToObject(items_game_item_used_by_classes_item, items_game_item_used_by_classes_attr->string);

                items_game_item_used_by_classes_attr = items_game_item_used_by_classes_attr->next;
            }

            printf("\n");
        }

        items_game_item = items_game_item->next;
    }

    items_game_out_data = cJSON_Print(items_game_out_root);

    cJSON_Delete(items_game_out_root);

    FILE *items_game_out_file = fopen(argv[2], "w");
    if (items_game_out_file == NULL)
    {
        printf("Error: File \"%s\" could not be opened.\n", argv[2]);
        getchar();
        return 1;
    }

    fprintf(items_game_out_file, items_game_out_data);

    free(items_game_out_data);

    fclose(items_game_out_file);

    cJSON_Delete(items_game_root);

    return 0;
}
