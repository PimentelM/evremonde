/**
 *  cJSON Helper
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

#include "cjson_helper.h"

cJSON *cjson_helper_parse_file(char* fileName)
{
    FILE *file = fopen(fileName, "rb");
    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);

    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);

    fread(data, 1, length, file);

    fclose(file);

    cJSON *root = cJSON_Parse(data);
    free(data);

    return root;
}