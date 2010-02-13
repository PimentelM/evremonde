/**
 *  Team Fortress 2 items_game
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

int main()
{
    FILE *itemsgame_file = fopen("items_game-json.txt", "rb");
    if (itemsgame_file == NULL)
    {
        printf("Error: File \"items_game-json.txt\" cannot be opened.\n");
        return 1;
    }

    fseek(itemsgame_file, 0, SEEK_END);
    long itemsgame_length = ftell(itemsgame_file);

    fseek(itemsgame_file, 0, SEEK_SET);
    char *itemsgame_data = malloc(itemsgame_length + 1);
    fread(itemsgame_data, 1, itemsgame_length, itemsgame_file);

    fclose(itemsgame_file);

    cJSON *itemsgame_root = cJSON_Parse(itemsgame_data);
    free(itemsgame_data);

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

        int itemId = atoi(itemsgame_item->string);
        cJSON_AddNumberToObject(itemsgame_out_item, "id", itemId);
        printf("item id: %d\n", itemId);

        char *itemName = cJSON_GetObjectItem(itemsgame_item, "name")->valuestring;
        cJSON_AddStringToObject(itemsgame_out_item, "name", itemName);
        printf("\titem name: %s\n", itemName);

        //cJSON *itemsgame_item_attr = itemsgame_item->child;
        //while (itemsgame_item_attr)
        //{
            //printf("item attr: %s\n", itemsgame_item_attr->string);

            //itemsgame_item_attr = itemsgame_item_attr->next;
        //}

        itemsgame_item = itemsgame_item->next;
    }

    itemsgame_out_data = cJSON_Print(itemsgame_out_root);

    FILE *itemsgame_out_file = fopen("items_game-out.txt", "w");
    if (itemsgame_out_file == NULL)
    {
        printf("Error: File \"items_game-out.txt\" cannot be opened.\n");
        return 1;
    }

    fprintf(itemsgame_out_file, itemsgame_out_data);

    fclose(itemsgame_out_file);

    cJSON_Delete(itemsgame_out_root);

    cJSON_Delete(itemsgame_root);

    free(itemsgame_out_data);

    return 0;
}
