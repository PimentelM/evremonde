/**
 *  Team Fortress 2 Items
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include <curl/curl.h>

#include "jansson.h"

#include "cjson.h"

#include "tf2.h"

int isNumeric(char *str)
{
    while (*str)
    {
        if (isdigit(*str) == 0)
            return 0;

        str++;
    }

    return 1;
}

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

    curl_global_init(CURL_GLOBAL_ALL);

    printf("cURL Version: %s\n", curl_version());

    CURL *curl_download_json = curl_easy_init();

    FILE *curl_download_json_file = fopen("tfitems", "w");

    char curl_download_json_url[1024];

    if (isNumeric(argv[1]) == 0)
        sprintf(curl_download_json_url, "http://steamcommunity.com/id/%s/tfitems?json=1", argv[1]);
    else
        sprintf(curl_download_json_url, "http://steamcommunity.com/profiles/%s/tfitems?json=1", argv[1]);

    char curl_download_json_error_buffer[CURL_ERROR_SIZE];

    curl_easy_setopt(curl_download_json, CURLOPT_URL,         curl_download_json_url);
    curl_easy_setopt(curl_download_json, CURLOPT_WRITEDATA,   curl_download_json_file);
    curl_easy_setopt(curl_download_json, CURLOPT_ERRORBUFFER, curl_download_json_error_buffer);
    curl_easy_setopt(curl_download_json, CURLOPT_NOPROGRESS,  0);

    CURLcode curl_download_json_result = curl_easy_perform(curl_download_json);

    if (curl_download_json_result == CURLE_OK)
    {
        printf("File \"tfitems\" successfully downloaded from:\n\"%s\"\n", curl_download_json_url);
    }
    else
    {
        printf("Error: An error occured with cURL: %d; %s\n", curl_download_json_result, curl_download_json_error_buffer);
        return 1;
    }

    fclose(curl_download_json_file);

    curl_easy_cleanup(curl_download_json);

    CURL *curl_download_xml = curl_easy_init();

    char curl_download_xml_file_name[128];
    sprintf(curl_download_xml_file_name, "profiles/%s.xml", argv[1]);

    FILE *curl_download_xml_file = fopen(curl_download_xml_file_name, "w");

    char curl_download_xml_url[1024];

    if (isNumeric(argv[1]) == 0)
        sprintf(curl_download_xml_url, "http://steamcommunity.com/id/%s?xml=1", argv[1]);
    else
        sprintf(curl_download_xml_url, "http://steamcommunity.com/profiles/%s?xml=1", argv[1]);

    char curl_download_xml_error_buffer[CURL_ERROR_SIZE];

    curl_easy_setopt(curl_download_xml, CURLOPT_URL,         curl_download_xml_url);
    curl_easy_setopt(curl_download_xml, CURLOPT_WRITEDATA,   curl_download_xml_file);
    curl_easy_setopt(curl_download_xml, CURLOPT_ERRORBUFFER, curl_download_xml_error_buffer);
    curl_easy_setopt(curl_download_xml, CURLOPT_NOPROGRESS,  0);

    CURLcode curl_download_xml_result = curl_easy_perform(curl_download_xml);

    if (curl_download_xml_result == CURLE_OK)
    {
        printf("File \"%s.xml\" successfully downloaded from:\n\"%s\"\n", argv[1], curl_download_xml_url);
    }
    else
    {
        printf("Error: An error occured with cURL: %d; %s\n", curl_download_xml_result, curl_download_xml_error_buffer);
        return 1;
    }

    fclose(curl_download_xml_file);

    curl_easy_cleanup(curl_download_xml);

    FILE *debug_file = fopen("debug.txt", "w");
    if (debug_file == NULL)
    {
        printf("Error: File \"debug.txt\" could not be opened.\n");
        return 1;
    }

    fprintf(debug_file, "profile: %s\n\n", argv[1]);

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

    cJSON *user_file_profile;
    cJSON_AddItemToObject(user_file_root, "profile", user_file_profile = cJSON_CreateObject());

    cJSON_AddStringToObject(user_file_profile, "name", argv[1]);

    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);

    char date_buffer[128];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", time_info);

    cJSON_AddStringToObject(user_file_profile, "date", date_buffer);

    char time_buffer[128];
    strftime(time_buffer, sizeof(time_buffer), "%I:%M %p", time_info);

    cJSON_AddStringToObject(user_file_profile, "time", time_buffer);

    cJSON *user_file_items;
    cJSON_AddItemToObject(user_file_root, "items", user_file_items = cJSON_CreateArray());

    void *tfitems_file_items = json_object_iter(tfitems_file_root);
    while (tfitems_file_items)
    {
        json_t *tfitems_file_item = json_object_iter_value(tfitems_file_items);

        const char *tfitems_file_item_key_string = json_object_iter_key(tfitems_file_items);
        
        json_t *tfitems_file_item_defIndex   = json_object_get(tfitems_file_item, "defindex");
        json_t *tfitems_file_item_level      = json_object_get(tfitems_file_item, "level");
        json_t *tfitems_file_item_quality    = json_object_get(tfitems_file_item, "quality");
        json_t *tfitems_file_item_inventory  = json_object_get(tfitems_file_item, "inventory");
        json_t *tfitems_file_item_attributes = json_object_get(tfitems_file_item, "attributes");

        int tfitems_file_item_defIndex_integer  = json_integer_value(tfitems_file_item_defIndex);
        int tfitems_file_item_level_integer     = json_integer_value(tfitems_file_item_level);
        int tfitems_file_item_quality_integer   = json_integer_value(tfitems_file_item_quality);
        int tfitems_file_item_inventory_integer = json_integer_value(tfitems_file_item_inventory);

        int tfitems_file_item_position_integer = tfitems_file_item_inventory_integer & TF2_INVENTORY_POSITION;
        int tfitems_file_item_equipped_integer = tfitems_file_item_inventory_integer & TF2_INVENTORY_EQUIPPED;

        fprintf(debug_file, "key: %s\n",             tfitems_file_item_key_string);

        fprintf(debug_file, "defindex: %d\n",        tfitems_file_item_defIndex_integer);
        fprintf(debug_file, "level: %d\n",           tfitems_file_item_level_integer);
        fprintf(debug_file, "quality_integer: %d\n", tfitems_file_item_quality_integer);
        fprintf(debug_file, "inventory: %d\n",       tfitems_file_item_inventory_integer);

        fprintf(debug_file, "position: %d\n", tfitems_file_item_position_integer);
        fprintf(debug_file, "equipped: %d\n", tfitems_file_item_equipped_integer);

        json_t *tf2items_file_items = json_object_get(tf2items_file_root, "items");

        int i = 0;
        for(i = 0; i < json_array_size(tf2items_file_items); i++)
        {
            json_t *tf2items_file_item = json_array_get(tf2items_file_items, i);

            json_t *tf2items_file_item_id = json_object_get(tf2items_file_item, "id");

            int tf2items_file_item_id_integer = json_integer_value(tf2items_file_item_id);

            if (json_equal(tf2items_file_item_id, tfitems_file_item_defIndex))
            {
                json_t *tf2items_file_item_name        = json_object_get(tf2items_file_item, "name");
                json_t *tf2items_file_item_description = json_object_get(tf2items_file_item, "description");
                json_t *tf2items_file_item_type        = json_object_get(tf2items_file_item, "type");
                json_t *tf2items_file_item_subtype     = json_object_get(tf2items_file_item, "subtype");
                json_t *tf2items_file_item_quality     = json_object_get(tf2items_file_item, "quality");
                json_t *tf2items_file_item_slot        = json_object_get(tf2items_file_item, "slot");
                json_t *tf2items_file_item_classes     = json_object_get(tf2items_file_item, "classes");
                json_t *tf2items_file_item_image       = json_object_get(tf2items_file_item, "image");
                json_t *tf2items_file_item_holiday     = json_object_get(tf2items_file_item, "holiday");
                json_t *tf2items_file_item_properties  = json_object_get(tf2items_file_item, "properties");
                
                const char *tf2items_file_item_name_string        = json_string_value(tf2items_file_item_name);

                const char *tf2items_file_item_description_string;
                if (tf2items_file_item_description != NULL)
                    tf2items_file_item_description_string = json_string_value(tf2items_file_item_description);

                const char *tf2items_file_item_type_string        = json_string_value(tf2items_file_item_type);
                const char *tf2items_file_item_subtype_string     = json_string_value(tf2items_file_item_subtype);
                const char *tf2items_file_item_quality_string     = json_string_value(tf2items_file_item_quality);
                const char *tf2items_file_item_slot_string        = json_string_value(tf2items_file_item_slot);
                const char *tf2items_file_item_classes_string       = json_string_value(tf2items_file_item_classes);
                const char *tf2items_file_item_image_string       = json_string_value(tf2items_file_item_image);

                const char *tf2items_file_item_holiday_string;
                if (tf2items_file_item_holiday != NULL)
                    tf2items_file_item_holiday_string = json_string_value(tf2items_file_item_holiday);

                fprintf(debug_file, "\tid: %d\n",             tf2items_file_item_id_integer);

                fprintf(debug_file, "\tname: %s\n",           tf2items_file_item_name_string);

                if (tf2items_file_item_description != NULL)
                    fprintf(debug_file, "\tdescription: %s\n", tf2items_file_item_description_string);

                fprintf(debug_file, "\ttype: %s\n",           tf2items_file_item_type_string);

                if (tf2items_file_item_subtype != NULL)
                    fprintf(debug_file, "\tsubtype: %s\n", tf2items_file_item_subtype_string);

                fprintf(debug_file, "\tquality_string: %s\n", tf2items_file_item_quality_string);
                fprintf(debug_file, "\tslot: %s\n",           tf2items_file_item_slot_string);
                fprintf(debug_file, "\tclasses: %s\n",        tf2items_file_item_classes_string);
                fprintf(debug_file, "\timage: %s\n",          tf2items_file_item_image_string);

                if (tf2items_file_item_holiday != NULL)
                    fprintf(debug_file, "\tholiday: %s\n", tf2items_file_item_holiday_string);

                cJSON *user_file_item;
                cJSON_AddItemToArray(user_file_items, user_file_item = cJSON_CreateObject());

                cJSON_AddNumberToObject(user_file_item, "id",             tf2items_file_item_id_integer);

                cJSON_AddStringToObject(user_file_item, "name",           tf2items_file_item_name_string);

                if (tf2items_file_item_description != NULL)
                    cJSON_AddStringToObject(user_file_item, "description", tf2items_file_item_description_string);

                cJSON_AddStringToObject(user_file_item, "type",           tf2items_file_item_type_string);

                if (tf2items_file_item_subtype != NULL)
                    cJSON_AddStringToObject(user_file_item, "subtype", tf2items_file_item_subtype_string);

                cJSON_AddStringToObject(user_file_item, "quality_string", tf2items_file_item_quality_string);
                cJSON_AddStringToObject(user_file_item, "slot",           tf2items_file_item_slot_string);
                cJSON_AddStringToObject(user_file_item, "classes",        tf2items_file_item_classes_string);
                cJSON_AddStringToObject(user_file_item, "image",          tf2items_file_item_image_string);

                if (tf2items_file_item_holiday != NULL)
                    cJSON_AddStringToObject(user_file_item, "holiday", tf2items_file_item_holiday_string);

                if (tf2items_file_item_properties != NULL)
                {
                    cJSON *user_file_item_properties;
                    cJSON_AddItemToObject(user_file_item, "properties", user_file_item_properties = cJSON_CreateArray());

                    int j = 0;
                    for(j = 0; j < json_array_size(tf2items_file_item_properties); j++)
                    {
                        json_t *tf2items_file_property = json_array_get(tf2items_file_item_properties, j);

                        json_t *tf2items_file_property_text = json_object_get(tf2items_file_property, "text");
                        json_t *tf2items_file_property_type = json_object_get(tf2items_file_property, "type");

                        const char* tf2items_file_property_text_string = json_string_value(tf2items_file_property_text);
                        const char* tf2items_file_property_type_string = json_string_value(tf2items_file_property_type);

                        fprintf(debug_file, "\t\tproperty text: %s\n", tf2items_file_property_text_string);
                        fprintf(debug_file, "\t\tproperty type: %s\n", tf2items_file_property_type_string);

                        cJSON *user_file_item_properties_item;
                        cJSON_AddItemToArray(user_file_item_properties, user_file_item_properties_item = cJSON_CreateObject());

                        cJSON_AddStringToObject(user_file_item_properties_item, "text", tf2items_file_property_text_string);
                        cJSON_AddStringToObject(user_file_item_properties_item, "type", tf2items_file_property_type_string);
                    }
                }

                cJSON_AddStringToObject(user_file_item, "key",              tfitems_file_item_key_string);

                cJSON_AddNumberToObject(user_file_item, "level",            tfitems_file_item_level_integer);
                cJSON_AddNumberToObject(user_file_item, "quality_integer",  tfitems_file_item_quality_integer);
                cJSON_AddNumberToObject(user_file_item, "inventory",        tfitems_file_item_inventory_integer);

                cJSON_AddNumberToObject(user_file_item, "position",         tfitems_file_item_position_integer);
                cJSON_AddNumberToObject(user_file_item, "equipped",         tfitems_file_item_equipped_integer);

                if (tfitems_file_item_attributes != NULL)
                {
                    cJSON *user_file_item_attributes;
                    cJSON_AddItemToObject(user_file_item, "attributes", user_file_item_attributes = cJSON_CreateArray());

                    void *tfitems_file_item_attributes_items = json_object_iter(tfitems_file_item_attributes);
                    while (tfitems_file_item_attributes_items)
                    {
                        json_t *tfitems_file_item_attributes_item_value = json_object_iter_value(tfitems_file_item_attributes_items);

                        double tfitems_file_item_attributes_item_value_real = json_real_value(tfitems_file_item_attributes_item_value);

                        const char *tfitems_file_item_attributes_item_key_string = json_object_iter_key(tfitems_file_item_attributes_items);

                        int tfitems_file_item_attributes_item_key_integer = atoi(tfitems_file_item_attributes_item_key_string);

                        fprintf(debug_file, "\t\tattribute key: %d\n", tfitems_file_item_attributes_item_key_integer);
                        fprintf(debug_file, "\t\tattribute value: %f\n", tfitems_file_item_attributes_item_value_real);

                        json_t *tf2items_file_attributes = json_object_get(tf2items_file_root, "attributes");

                        int k = 0;
                        for(k = 0; k < json_array_size(tf2items_file_attributes); k++)
                        {
                            json_t *tf2items_file_attribute = json_array_get(tf2items_file_attributes, k);

                            json_t *tf2items_file_attribute_id = json_object_get(tf2items_file_attribute, "id");

                            int tf2items_file_attribute_id_integer = json_integer_value(tf2items_file_attribute_id);

                            if (tf2items_file_attribute_id_integer == tfitems_file_item_attributes_item_key_integer)
                            {
                                json_t *tf2items_file_attribute_name        = json_object_get(tf2items_file_attribute, "name");
                                json_t *tf2items_file_attribute_description = json_object_get(tf2items_file_attribute, "description");

                                const char *tf2items_file_attribute_name_string        = json_string_value(tf2items_file_attribute_name);
                                const char *tf2items_file_attribute_description_string = json_string_value(tf2items_file_attribute_description);

                                fprintf(debug_file, "\t\t\tattribute id: %d\n",          tf2items_file_attribute_id_integer);

                                fprintf(debug_file, "\t\t\tattribute name: %s\n",        tf2items_file_attribute_name_string);
                                fprintf(debug_file, "\t\t\tattribute description: %s\n", tf2items_file_attribute_description_string);

                                cJSON *user_file_item_attributes_item;
                                cJSON_AddItemToArray(user_file_item_attributes, user_file_item_attributes_item = cJSON_CreateObject());

                                cJSON_AddNumberToObject(user_file_item_attributes_item, "id",          tf2items_file_attribute_id_integer);

                                cJSON_AddStringToObject(user_file_item_attributes_item, "name",        tf2items_file_attribute_name_string);
                                cJSON_AddStringToObject(user_file_item_attributes_item, "description", tf2items_file_attribute_description_string);

                                cJSON_AddNumberToObject(user_file_item_attributes_item, "value", tfitems_file_item_attributes_item_value_real);
                            }
                        }

                        tfitems_file_item_attributes_items = json_object_iter_next(tfitems_file_item_attributes, tfitems_file_item_attributes_items);
                    }
                }
            }
        }

        tfitems_file_items = json_object_iter_next(tfitems_file_root, tfitems_file_items);
    }

    json_decref(tfitems_file_root);
    
    json_decref(tf2items_file_root);

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

    printf("Profile for \"%s\" saved to: \"%s\"\n", argv[1], user_file_name);

    fclose(user_file);

    fclose(debug_file);

    /*
        CURL *curl_upload = curl_easy_init();

        FILE *curl_upload_file = fopen(user_file_name, "r");

        char curl_upload_url[1024];
        sprintf(curl_upload_url, "ftp://tf2items.zzl.org/profiles/%s.txt", argv[1]);

        char curl_upload_error_buffer[CURL_ERROR_SIZE];

        curl_easy_setopt(curl_upload, CURLOPT_UPLOAD,      1L);
        curl_easy_setopt(curl_upload, CURLOPT_USERPWD,     "tf2items_zzl:azxcvbnm1");
        curl_easy_setopt(curl_upload, CURLOPT_URL,         curl_upload_url);
        curl_easy_setopt(curl_upload, CURLOPT_INFILE,      curl_upload_file);
        curl_easy_setopt(curl_upload, CURLOPT_ERRORBUFFER, curl_upload_error_buffer);
        curl_easy_setopt(curl_upload, CURLOPT_NOPROGRESS,  0);

        CURLcode curl_upload_result = curl_easy_perform(curl_upload);

        if (curl_upload_result == CURLE_OK)
        {
            printf("File \"%s.txt\" successfully uploaded to:\n\"%s\"\n", argv[1], curl_upload_url);
        }
        else
        {
            printf("Error: An error occured with cURL: %d; %s\n", curl_upload_result, curl_upload_error_buffer);
            return 1;
        }

        fclose(curl_upload_file);

        curl_easy_cleanup(curl_upload);
    */

    return 0;
}
