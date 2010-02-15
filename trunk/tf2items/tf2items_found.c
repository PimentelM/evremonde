/**
 *  Team Fortress 2 Items Found
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
        printf("Usage: tf2items_found steamuserid\n");
        getchar();
        return 1;
    }

    printf("Compiler Version: gcc " __VERSION__ "\n");
    printf("Build Date: " __DATE__ "\n");
    printf("Build Time: " __TIME__ "\n");

    FILE *curl_file = fopen("tfitems", "w");

    CURL *curl;
    curl_global_init(CURL_GLOBAL_WIN32);
    curl = curl_easy_init();

    printf("cURL version: %s\n", curl_version());

    char curl_error_buffer[CURL_ERROR_SIZE];

    char curl_url[1024];
    sprintf(curl_url, "http://steamcommunity.com/id/%s/tfitems?json=1", argv[1]);

    curl_easy_setopt(curl, CURLOPT_URL, curl_url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, curl_file);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

    CURLcode curl_result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    if (curl_result == CURLE_OK)
    {
        printf("File \"tfitems\" successfully downloaded.\n");
    }
    else
    {
        printf("Error: An error has occured with cURL: %d %s\n", curl_result, curl_error_buffer);
        getchar();
        return 1;
    }

    fclose(curl_file);

    cJSON *tfitems_root = cjson_helper_parse_file("tfitems");
    if (tfitems_root == NULL)
    {
        printf("Error: File \"tfitems\" could not be parsed.\n");
        getchar();
        return 1;
    }

    cJSON *tf2items_root = cjson_helper_parse_file("tf2items_compressed.txt");
    if (tf2items_root == NULL)
    {
        printf("Error: File \"tf2items_compressed.txt\" could not be parsed.\n");
        getchar();
        return 1;
    }

    char user_file_name[256];
    sprintf(user_file_name, "profiles/%s.txt", argv[1]);

    FILE *user_file = fopen(user_file_name, "w");
    if (user_file == NULL)
    {
        printf("Error: File \"%s\" could not be opened.\n", user_file_name);
        getchar();
        return 1;
    }

    int nItemsFound = 0;

    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time );
    fprintf(user_file, "Date/Time:\n%s\n", asctime(time_info));

    fprintf(user_file, "http://steamcommunity.com/id/%s\n", argv[1]);
    fprintf(user_file, "http://www.tf2items.com/id/%s\n", argv[1]);
    fprintf(user_file, "\n");
    fprintf(user_file, "%s = Found but not yet in backpack\n", TF2_ITEM_STRING_FOUND);
    fprintf(user_file, "%s = Hat\n",                           TF2_ITEM_STRING_HAT);
    fprintf(user_file, "%s = Hatless\n",                       TF2_ITEM_STRING_HATLESS);
    fprintf(user_file, "\n");

    cJSON *tfitems_item = tfitems_root->child;
    while (tfitems_item)
    {
        int tfitems_item_defIndex = cJSON_GetObjectItem(tfitems_item, "defindex")->valueint;
        printf("defindex: %d\n", tfitems_item_defIndex);

        int tfitems_item_inventory = cJSON_GetObjectItem(tfitems_item, "inventory")->valueint;
        printf("inventory: %d\n", tfitems_item_inventory);

        cJSON *tf2items_items = cJSON_GetObjectItem(tf2items_root, "items");

        cJSON *tf2items_item = tf2items_items->child;
        while (tf2items_item)
        {
            //int tf2items_item_id = cJSON_GetObjectItem(tf2items_item, "id")->valueint;
            int tf2items_item_id = atoi(tf2items_item->string);

            if (tf2items_item_id == tfitems_item_defIndex)
            {
                printf("\tid: %d\n", tf2items_item_id);

                char *tf2items_item_name = cJSON_GetObjectItem(tf2items_item, "name")->valuestring;
                printf("\tname: %s\n", tf2items_item_name);

                char *tf2items_item_type = cJSON_GetObjectItem(tf2items_item, "type")->valuestring;
                printf("\ttype: %s\n", tf2items_item_type);

                if (tfitems_item_inventory == 0)
                {
                    if (nItemsFound > 0)
                        if (nItemsFound % 10 == 0)
                            fprintf(user_file, "\n");

                    nItemsFound++;

                    fprintf(user_file, "%s ", TF2_ITEM_STRING_FOUND);

                    if (strcmp(tf2items_item_type, "Hat") == 0)
                        fprintf(user_file, "%s ", TF2_ITEM_STRING_HAT);
                    else if (strcmp(tf2items_item_type, "Hatless") == 0)
                        fprintf(user_file, "%s ", TF2_ITEM_STRING_HATLESS);

                    fprintf(user_file, "%s\n", tf2items_item_name);
                }
            }

            tf2items_item = tf2items_item->next;
        }

        tfitems_item = tfitems_item->next;
    }

    cJSON_Delete(tfitems_root);
    
    cJSON_Delete(tf2items_root);

    fprintf(user_file, "\n");

    fprintf(user_file, "Items Summary!\n");
    fprintf(user_file, "--------------\n");

    fprintf(user_file, "Items Found: %d\n", nItemsFound);

    fprintf(user_file, "\n");

    fclose(user_file);

    return 0;
}
