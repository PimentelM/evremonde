/**
 *  JSON Test
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

int main()
{
    FILE *file = fopen("jsontest.txt", "rb");
    if (file == NULL)
    {
        printf("Error: File cannot be opened.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);

    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    fread(data, 1, length, file);

    fclose(file);

    cJSON *root = cJSON_Parse(data);
    free(data);

    cJSON *items = cJSON_GetObjectItem(root, "items");

    cJSON *item = items->child;
    while (item)
    {
        int itemId = cJSON_GetObjectItem(item, "id")->valueint;
        char *itemName = cJSON_GetObjectItem(item, "name")->valuestring;
        int itemIsEnabled = cJSON_GetObjectItem(item, "enabled")->type;

        printf("%d %s\n", itemId, itemName);
        if (itemIsEnabled == cJSON_True)
            printf("^ is enabled\n");

        item = item->next;
    }

    cJSON_Delete(root);

    getchar();

    return 0;
}
