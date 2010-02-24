/**
 *  Team Fortress 2 Items
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <curl/curl.h>

#include "cJSON.h"

#include "cjson_helper.h"

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

    cJSON *tfitems_root = cjson_helper_parse_file("tfitems");
    if (tfitems_root == NULL)
    {
        printf("Error: File \"tfitems\" could not be parsed.\n");
        return 1;
    }

    cJSON *tf2items_root = cjson_helper_parse_file("tf2items.json");
    if (tf2items_root == NULL)
    {
        printf("Error: File \"tf2items.json\" could not be parsed.\n");
        return 1;
    }

    char user_file_json_name[128];
    sprintf(user_file_json_name, "profiles/%s.json", argv[1]);

    FILE *user_file_json = fopen(user_file_json_name, "w");
    if (user_file_json == NULL)
    {
        printf("Error: File \"%s\" could not be opened.\n", user_file_json_name);
        return 1;
    }

    cJSON *user_file_json_root = cJSON_CreateObject();

    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);

    char date_buffer[128];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", time_info);

    cJSON_AddStringToObject(user_file_json_root, "date", date_buffer);

    char time_buffer[128];
    strftime(time_buffer, sizeof(time_buffer), "%I:%M %p", time_info);

    cJSON_AddStringToObject(user_file_json_root, "time", time_buffer);

    char url_steam_community[1024];
    sprintf(url_steam_community, "http://steamcommunity.com/id/%s", argv[1]);

    cJSON_AddStringToObject(user_file_json_root, "url_steam_community", url_steam_community);

    char url_tf2items[1024];
    sprintf(url_tf2items, "http://www.tf2items.com/id/%s", argv[1]);

    cJSON_AddStringToObject(user_file_json_root, "url_tf2items", url_tf2items);

    int nItems      = 0;
    int nItemsFound = 0;
    int nHats       = 0;
    int nHatsFound  = 0;
    int nMetals      = 0;
    int nMetalsWorth = 0;
    int nTokens      = 0;

    cJSON *user_file_json_items;
    cJSON_AddItemToObject(user_file_json_root, "items", user_file_json_items = cJSON_CreateArray());

    cJSON *tfitems_item = tfitems_root->child;
    while (tfitems_item)
    {
        int tfitems_item_defIndex  = cJSON_GetObjectItem(tfitems_item, "defindex")->valueint;
        int tfitems_item_level     = cJSON_GetObjectItem(tfitems_item, "level")->valueint;
        int tfitems_item_quality   = cJSON_GetObjectItem(tfitems_item, "quality")->valueint;
        int tfitems_item_inventory = cJSON_GetObjectItem(tfitems_item, "inventory")->valueint;

        printf("defindex: %d\n",  tfitems_item_defIndex);
        printf("level: %d\n",     tfitems_item_level);
        printf("quality: %d\n",   tfitems_item_quality);
        printf("inventory: %d\n", tfitems_item_inventory);

        cJSON *tf2items_items = cJSON_GetObjectItem(tf2items_root, "items");

        int i;
        for (i = 0; i < cJSON_GetArraySize(tf2items_items); i++)
        {
            cJSON *tf2items_item = cJSON_GetArrayItem(tf2items_items, i);

            int tf2items_item_id = cJSON_GetObjectItem(tf2items_item, "id")->valueint;

            if (tf2items_item_id == tfitems_item_defIndex)
            {
                char *tf2items_item_name    = cJSON_GetObjectItem(tf2items_item, "name")->valuestring;
                char *tf2items_item_type    = cJSON_GetObjectItem(tf2items_item, "type")->valuestring;
                char *tf2items_item_quality = cJSON_GetObjectItem(tf2items_item, "quality")->valuestring;
                char *tf2items_item_slot    = cJSON_GetObjectItem(tf2items_item, "slot")->valuestring;
                char *tf2items_item_class   = cJSON_GetObjectItem(tf2items_item, "class")->valuestring;
                char *tf2items_item_image   = cJSON_GetObjectItem(tf2items_item, "image")->valuestring;

                printf("\tid: %d\n",      tf2items_item_id);
                printf("\tname: %s\n",    tf2items_item_name);
                printf("\ttype: %s\n",    tf2items_item_type);
                printf("\tquality: %s\n", tf2items_item_quality);
                printf("\tslot: %s\n",    tf2items_item_slot);
                printf("\tclass: %s\n",   tf2items_item_class);
                printf("\timage: %s\n",   tf2items_item_image);

                if (tfitems_item_inventory == 0)
                {
                    nItemsFound++;

                    if (strstr(tf2items_item_type, "Hat") != NULL)
                        nHatsFound++;
                }

                if (strstr(tf2items_item_type, "Hat") != NULL)
                {
                    nHats++;
                }
                else if (strcmp(tf2items_item_type, "Metal") == 0)
                {
                    nMetals++;

                    if (strcmp(tf2items_item_name, "Scrap Metal") == 0)
                        nMetalsWorth += TF2_METAL_MULTIPLIER;
                    else if (strcmp(tf2items_item_name, "Reclaimed Metal") == 0)
                        nMetalsWorth += (TF2_METAL_MULTIPLIER * TF2_METAL_MULTIPLIER);
                    else if (strcmp(tf2items_item_name, "Refined Metal") == 0)
                        nMetalsWorth += (TF2_METAL_MULTIPLIER * TF2_METAL_MULTIPLIER * TF2_METAL_MULTIPLIER);
                }
                else if (strcmp(tf2items_item_type, "Token") == 0)
                {
                    nTokens++;
                }
                else
                {
                    nItems++;
                }

                cJSON *user_file_json_item;
                cJSON_AddItemToArray(user_file_json_items, user_file_json_item = cJSON_CreateObject());

                cJSON_AddNumberToObject(user_file_json_item, "id",      tf2items_item_id);
                cJSON_AddStringToObject(user_file_json_item, "name",    tf2items_item_name);
                cJSON_AddStringToObject(user_file_json_item, "type",    tf2items_item_type);
                cJSON_AddStringToObject(user_file_json_item, "quality", tf2items_item_quality);
                cJSON_AddStringToObject(user_file_json_item, "slot",    tf2items_item_slot);
                cJSON_AddStringToObject(user_file_json_item, "class",   tf2items_item_class);
                cJSON_AddStringToObject(user_file_json_item, "image",   tf2items_item_image);

                cJSON_AddNumberToObject(user_file_json_item, "level",     tfitems_item_level);
                cJSON_AddNumberToObject(user_file_json_item, "inventory", tfitems_item_inventory);
            }
        }

        tfitems_item = tfitems_item->next;
    }

    cJSON_Delete(tfitems_root);
    
    cJSON_Delete(tf2items_root);

    cJSON_AddNumberToObject(user_file_json_root, "n_items",        nItems);
    cJSON_AddNumberToObject(user_file_json_root, "n_items_found",  nItemsFound);
    cJSON_AddNumberToObject(user_file_json_root, "n_hats",         nHats);
    cJSON_AddNumberToObject(user_file_json_root, "n_hats_found",   nHatsFound);
    cJSON_AddNumberToObject(user_file_json_root, "n_metals",       nMetals);
    cJSON_AddNumberToObject(user_file_json_root, "n_metals_worth", nMetalsWorth);
    cJSON_AddNumberToObject(user_file_json_root, "n_tokens",       nTokens);
    
    char *user_file_json_out;
    user_file_json_out = cJSON_PrintUnformatted(user_file_json_root);
    cJSON_Delete(user_file_json_root);

    fprintf(user_file_json, "%s\n", user_file_json_out);
    free(user_file_json_out);

    fclose(user_file_json);

    return 0;
}
