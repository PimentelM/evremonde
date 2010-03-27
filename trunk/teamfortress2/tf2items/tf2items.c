/**
 *  Team Fortress 2 Items
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <curl/curl.h>

#include "jansson.h"

#include "cjson.h"

#include "tf2.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: tf2items steamuserid\n");
        return 1;
    }

    printf("Compiler Version: gcc " __VERSION__ "\n");
    printf("Build Date: "           __DATE__    "\n");
    printf("Build Time: "           __TIME__    "\n");

    CURL *curl;
    curl_global_init(CURL_GLOBAL_WIN32);
    curl = curl_easy_init();

    printf("cURL Version: %s\n", curl_version());

    FILE *curl_file = fopen("tfitems", "w");

    char curl_url[1024];
    sprintf(curl_url, "http://steamcommunity.com/id/%s/tfitems?json=1", argv[1]);

    char curl_error_buffer[CURL_ERROR_SIZE];

    curl_easy_setopt(curl, CURLOPT_WRITEDATA,   curl_file);
    curl_easy_setopt(curl, CURLOPT_URL,         curl_url);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS,  0);

    CURLcode curl_result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    if (curl_result == CURLE_OK)
    {
        printf("File \"tfitems\" successfully downloaded.\n");
    }
    else
    {
        printf("Error: An error occured with cURL: %d; %s\n", curl_result, curl_error_buffer);
        return 1;
    }

    fclose(curl_file);

    json_error_t tfitems_file_error;
    json_t *tfitems_file_root = json_load_file("tfitems", &tfitems_file_error);
    if (!tfitems_file_root)
    {
        printf("Error: File \"tfitems\" could not be parsed.\n");
        return 1;
    }

    json_error_t tf2items_file_error;
    json_t *tf2items_file_root = json_load_file("tf2items.txt", &tf2items_file_error);
    if (!tf2items_file_root)
    {
        printf("Error: File \"tf2items.txt\" could not be parsed.\n");
        return 1;
    }

    char user_file_name[128];
    sprintf(user_file_name, "profiles/%s.txt", argv[1]);

    cJSON *user_file_root = cJSON_CreateObject();

    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);

    char date_buffer[128];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", time_info);

    cJSON_AddStringToObject(user_file_root, "date", date_buffer);

    char time_buffer[128];
    strftime(time_buffer, sizeof(time_buffer), "%I:%M %p", time_info);

    cJSON_AddStringToObject(user_file_root, "time", time_buffer);

    char url_steam_community[1024];
    sprintf(url_steam_community, "http://steamcommunity.com/id/%s", argv[1]);

    cJSON_AddStringToObject(user_file_root, "url_steam_community", url_steam_community);

    char url_tf2items[1024];
    sprintf(url_tf2items, "http://www.tf2items.com/id/%s", argv[1]);

    cJSON_AddStringToObject(user_file_root, "url_tf2items", url_tf2items);

    int nItems       = 0;
    int nItemsFound  = 0;
    int nHats        = 0;
    int nHatsFound   = 0;
    int nMetals      = 0;
    int nMetalsWorth = 0;
    int nTokens      = 0;

    cJSON *user_file_items;
    cJSON_AddItemToObject(user_file_root, "items", user_file_items = cJSON_CreateArray());

    void *tfitems_file_items = json_object_iter(tfitems_file_root);
    while (tfitems_file_items)
    {
        json_t *tfitems_file_item = json_object_iter_value(tfitems_file_items);

        const char *tfitems_file_item_key_string = json_object_iter_key(tfitems_file_items);
        
        json_t *tfitems_file_item_defIndex  = json_object_get(tfitems_file_item, "defindex");
        json_t *tfitems_file_item_level     = json_object_get(tfitems_file_item, "level");
        json_t *tfitems_file_item_quality   = json_object_get(tfitems_file_item, "quality");
        json_t *tfitems_file_item_inventory = json_object_get(tfitems_file_item, "inventory");

        int tfitems_file_item_defIndex_integer  = json_integer_value(tfitems_file_item_defIndex);
        int tfitems_file_item_level_integer     = json_integer_value(tfitems_file_item_level);
        int tfitems_file_item_quality_integer   = json_integer_value(tfitems_file_item_quality);
        int tfitems_file_item_inventory_integer = json_integer_value(tfitems_file_item_inventory);

        int tfitems_file_item_position_integer = tfitems_file_item_inventory_integer & TF2_INVENTORY_POSITION;

        int tfitems_file_item_equipped_integer = tfitems_file_item_inventory_integer & TF2_INVENTORY_EQUIPPED;

        printf("key: %s\n",       tfitems_file_item_key_string);

        printf("defindex: %d\n",  tfitems_file_item_defIndex_integer);
        printf("level: %d\n",     tfitems_file_item_level_integer);
        printf("quality: %d\n",   tfitems_file_item_quality_integer);
        printf("inventory: %d\n", tfitems_file_item_inventory_integer);

        printf("position: %d\n", tfitems_file_item_position_integer);
        printf("equipped: %d\n", tfitems_file_item_equipped_integer);

        json_t *tf2items_file_items = json_object_get(tf2items_file_root, "items");

        int i = 0;
        for(i = 0; i < json_array_size(tf2items_file_items); i++)
        {
            json_t *tf2items_file_item = json_array_get(tf2items_file_items, i);

            json_t *tf2items_file_item_id = json_object_get(tf2items_file_item, "id");

            int tf2items_file_item_id_integer = json_integer_value(tf2items_file_item_id);

            if (json_equal(tf2items_file_item_id, tfitems_file_item_defIndex))
            {
                json_t *tf2items_file_item_name    = json_object_get(tf2items_file_item, "name");
                json_t *tf2items_file_item_type    = json_object_get(tf2items_file_item, "type");
                json_t *tf2items_file_item_quality = json_object_get(tf2items_file_item, "quality");
                json_t *tf2items_file_item_slot    = json_object_get(tf2items_file_item, "slot");
                json_t *tf2items_file_item_class   = json_object_get(tf2items_file_item, "class");
                json_t *tf2items_file_item_image   = json_object_get(tf2items_file_item, "image");

                const char *tf2items_file_item_name_string    = json_string_value(tf2items_file_item_name);
                const char *tf2items_file_item_type_string    = json_string_value(tf2items_file_item_type);
                const char *tf2items_file_item_quality_string = json_string_value(tf2items_file_item_quality);
                const char *tf2items_file_item_slot_string    = json_string_value(tf2items_file_item_slot);
                const char *tf2items_file_item_class_string   = json_string_value(tf2items_file_item_class);
                const char *tf2items_file_item_image_string   = json_string_value(tf2items_file_item_image);

                printf("\tid: %d\n",      tf2items_file_item_id_integer);

                printf("\tname: %s\n",    tf2items_file_item_name_string);
                printf("\ttype: %s\n",    tf2items_file_item_type_string);
                printf("\tquality: %s\n", tf2items_file_item_quality_string);
                printf("\tslot: %s\n",    tf2items_file_item_slot_string);
                printf("\tclass: %s\n",   tf2items_file_item_class_string);
                printf("\timage: %s\n",   tf2items_file_item_image_string);

                if (tfitems_file_item_position_integer == 0)
                {
                    nItemsFound++;

                    if (strcmp(tf2items_file_item_type_string, "Hat") == 0)
                    {
                        nHatsFound++;
                    }
                }

                if (strcmp(tf2items_file_item_type_string, "Hat") == 0)
                {
                    if (tfitems_file_item_position_integer != 0)
                    {
                        nHats++;
                    }
                }
                else if (strcmp(tf2items_file_item_type_string, "Metal") == 0)
                {
                    nMetals++;

                    if (strcmp(tf2items_file_item_name_string, "Scrap Metal") == 0)
                        nMetalsWorth += TF2_METAL_MULTIPLIER;
                    else if (strcmp(tf2items_file_item_name_string, "Reclaimed Metal") == 0)
                        nMetalsWorth += (TF2_METAL_MULTIPLIER * TF2_METAL_MULTIPLIER);
                    else if (strcmp(tf2items_file_item_name_string, "Refined Metal") == 0)
                        nMetalsWorth += (TF2_METAL_MULTIPLIER * TF2_METAL_MULTIPLIER * TF2_METAL_MULTIPLIER);
                }
                else if (strcmp(tf2items_file_item_type_string, "Token") == 0)
                {
                    nTokens++;
                }
                else
                {
                    if (tfitems_file_item_position_integer != 0)
                    {
                        nItems++;
                    }
                }

                cJSON *user_file_item;
                cJSON_AddItemToArray(user_file_items, user_file_item = cJSON_CreateObject());

                cJSON_AddNumberToObject(user_file_item, "id",      tf2items_file_item_id_integer);

                cJSON_AddStringToObject(user_file_item, "name",    tf2items_file_item_name_string);
                cJSON_AddStringToObject(user_file_item, "type",    tf2items_file_item_type_string);
                cJSON_AddStringToObject(user_file_item, "quality", tf2items_file_item_quality_string);
                cJSON_AddStringToObject(user_file_item, "slot",    tf2items_file_item_slot_string);
                cJSON_AddStringToObject(user_file_item, "class",   tf2items_file_item_class_string);
                cJSON_AddStringToObject(user_file_item, "image",   tf2items_file_item_image_string);

                cJSON_AddStringToObject(user_file_item, "key",       tfitems_file_item_key_string);

                cJSON_AddNumberToObject(user_file_item, "level",     tfitems_file_item_level_integer);
                cJSON_AddNumberToObject(user_file_item, "inventory", tfitems_file_item_inventory_integer);

                cJSON_AddNumberToObject(user_file_item, "position",  tfitems_file_item_position_integer);
                cJSON_AddNumberToObject(user_file_item, "equipped",  tfitems_file_item_equipped_integer);
            }
        }

        tfitems_file_items = json_object_iter_next(tfitems_file_root, tfitems_file_items);
    }

    json_decref(tfitems_file_root);
    
    json_decref(tf2items_file_root);
    
    cJSON_AddNumberToObject(user_file_root, "n_items",        nItems);
    cJSON_AddNumberToObject(user_file_root, "n_items_found",  nItemsFound);
    cJSON_AddNumberToObject(user_file_root, "n_hats",         nHats);
    cJSON_AddNumberToObject(user_file_root, "n_hats_found",   nHatsFound);
    cJSON_AddNumberToObject(user_file_root, "n_metals",       nMetals);
    cJSON_AddNumberToObject(user_file_root, "n_metals_worth", nMetalsWorth);
    cJSON_AddNumberToObject(user_file_root, "n_tokens",       nTokens);
    
    FILE *user_file = fopen(user_file_name, "w");
    if (user_file == NULL)
    {
        printf("Error: File \"%s\" could not be opened.\n", user_file_name);
        return 1;
    }

    char *user_file_out = cJSON_PrintUnformatted(user_file_root);
    cJSON_Delete(user_file_root);
    fprintf(user_file, "%s\n", user_file_out);
    free(user_file_out);

    fclose(user_file);

    return 0;
}
