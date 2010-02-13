/**
 *  FLV Renamer
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include "shlwapi.h" // PathMatchSpec()

void findAndRenameFlvFiles(char *path)
{
    printf("Path: %s\n", path);

    WIN32_FIND_DATA FindFileData;

    char findAllFiles[MAX_PATH] = {0};
    sprintf(findAllFiles, "%s*.*", path);

    HANDLE hFind = FindFirstFile(findAllFiles, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE) 
    {
        printf("Error: Returned INVALID_HANDLE_VALUE in FindFirstFile() function.\n");
        return;
    }

    do
    {
        if (FindFileData.dwFileAttributes &~ FILE_ATTRIBUTE_DIRECTORY)
        {
            char *fileName = FindFileData.cFileName;

            printf("File Name: %s\n", fileName);

            char fullFileName[MAX_PATH];
            sprintf(fullFileName, "%s%s", path, fileName);

            if (PathMatchSpec(fullFileName, "*.flv"))
            {
                printf("Already Renamed, Skipping: %s\n", fullFileName);
                continue;
            }

            if (strstr(fullFileName, ".mp4") != NULL)
            {
                printf("Invalid File Format (*.mp4), Skipping: %s\n", fullFileName);
                continue;
            }

            printf("Full File Name: %s\n", fullFileName);

            FILE *file = fopen(fullFileName, "r");
            if (file != NULL)
            {
                char buffer[4];
                fgets(buffer, 4, file);

                printf("fgets(buffer, 4, file);: %s\n", buffer);

                if (strcmp(buffer, "FLV") == 0)
                {
                    char newFileName[MAX_PATH];
                    sprintf(newFileName, "%s.flv", fullFileName);

                    printf("New File Name: %s\n", newFileName);

                    fclose(file);
                    MoveFileEx(fullFileName, newFileName, MOVEFILE_REPLACE_EXISTING);
                }
                else
                {
                    printf("Error, File Is Not FLV Format: %s\n", fullFileName);
                }
            }
            fclose(file);
        }
    }
    while (FindNextFile(hFind, &FindFileData) != 0);

    FindClose(hFind);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Error: Invalid number of command line arguments.\n");
        return 0;
    }

    findAndRenameFlvFiles(argv[1]);

    return 0;
}
