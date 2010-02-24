/**
 *  Team Fortress 2 Items Append
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

#include "cjson_helper.h"

int main()
{
    printf("Compiler Version: gcc " __VERSION__ "\n");
    printf("Build Date: "           __DATE__    "\n");
    printf("Build Time: "           __TIME__    "\n");

    cJSON *items_game_out_root = cjson_helper_parse_file("items_game-out.json");
    if (items_game_out_root == NULL)
    {
        printf("Error: File \"items_game-out.json\" could not be parsed.\n");
        return 1;
    }

    cJSON *tf2items_root = cjson_helper_parse_file("tf2items.json");
    if (tf2items_root == NULL)
    {
        printf("Error: File \"tf2items.json\" could not be parsed.\n");
        return 1;
    }

    cJSON *items_game_out_items = cJSON_GetObjectItem(items_game_out_root, "items");

    cJSON *items_game_out_item = items_game_out_items->child;
    while (items_game_out_item)
    {
        int items_game_out_item_id = atoi(items_game_out_item->string);
        printf("\tid: %d\n", items_game_out_item_id);

        char *items_game_out_item_image_inventory = cJSON_GetObjectItem(items_game_out_item, "image_inventory")->valuestring;
        printf("\timage_inventory: %s\n", items_game_out_item_image_inventory);

        cJSON *tf2items_items = cJSON_GetObjectItem(tf2items_root, "items");

        cJSON *tf2items_item = tf2items_items->child;
        while (tf2items_item)
        {
            int tf2items_item_id = atoi(tf2items_item->string);

            /*
                char *tf2items_item_name    = cJSON_GetObjectItem(tf2items_item, "name")->valuestring;
                char *tf2items_item_type    = cJSON_GetObjectItem(tf2items_item, "type")->valuestring;
                char *tf2items_item_quality = cJSON_GetObjectItem(tf2items_item, "quality")->valuestring;
                char *tf2items_item_slot    = cJSON_GetObjectItem(tf2items_item, "slot")->valuestring;
                char *tf2items_item_class   = cJSON_GetObjectItem(tf2items_item, "class")->valuestring;
                char *tf2items_item_image   = cJSON_GetObjectItem(tf2items_item, "image")->valuestring;
            */

            if (tf2items_item_id == items_game_out_item_id)
            {
                /*
                    cJSON_DeleteItemFromObject(tf2items_item, "name");
                    cJSON_DeleteItemFromObject(tf2items_item, "type");
                    cJSON_DeleteItemFromObject(tf2items_item, "quality");
                    cJSON_DeleteItemFromObject(tf2items_item, "slot");
                    cJSON_DeleteItemFromObject(tf2items_item, "class");
                    cJSON_DeleteItemFromObject(tf2items_item, "image");

                    cJSON_AddNumberToObject(tf2items_item, "id",      tf2items_item_id);
                    cJSON_AddStringToObject(tf2items_item, "name",    tf2items_item_name);
                    cJSON_AddStringToObject(tf2items_item, "type",    tf2items_item_type);
                    cJSON_AddStringToObject(tf2items_item, "quality", tf2items_item_quality);
                    cJSON_AddStringToObject(tf2items_item, "slot",    tf2items_item_slot);
                    cJSON_AddStringToObject(tf2items_item, "class",   tf2items_item_class);
                    cJSON_AddStringToObject(tf2items_item, "image",   tf2items_item_image);
                */

                //cJSON_ReplaceItemInObject(tf2items_item, "id", cJSON_CreateNumber(tf2items_item_id));

                //cJSON_DeleteItemFromObject(tf2items_item, "id");
                //cJSON_AddNumberToObject(tf2items_item, "id", items_game_out_item_image_inventory);

                //cJSON_DeleteItemFromObject(tf2items_item, "image");
                //cJSON_AddStringToObject(tf2items_item, "image", items_game_out_item_image_inventory);
            }

            /*
                free(tf2items_item_name);
                free(tf2items_item_type);
                free(tf2items_item_quality);
                free(tf2items_item_slot);
                free(tf2items_item_class);
                free(tf2items_item_image);
            */

            tf2items_item = tf2items_item->next;
        }

        items_game_out_item = items_game_out_item->next;
    }

    cJSON_Delete(items_game_out_root);

    FILE *tf2items_out_file = fopen("tf2items_append.json", "w");
    if (tf2items_out_file == NULL)
    {
        printf("Error: File \"tf2items_append.json\" could not be opened.\n");
        return 1;
    }

    char *tf2items_out_data;
    tf2items_out_data = cJSON_PrintUnformatted(tf2items_root);

    fprintf(tf2items_out_file, tf2items_out_data);

    free(tf2items_out_data);

    fclose(tf2items_out_file);

    cJSON_Delete(tf2items_root);

    return 0;
}
