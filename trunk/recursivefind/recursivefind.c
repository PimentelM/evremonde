/**
 *  Recursive Find
 *  by Kevin Roberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include "shlwapi.h"

void searchForFiles(char *searchPath, char *fileExtension, char *fileNameFilter)
{
    WIN32_FIND_DATA FindFileData;

    char folderRootFindAllFiles[MAX_PATH] = {0};
    sprintf(folderRootFindAllFiles, "%s*.*", searchPath);

    HANDLE hFind = FindFirstFile(folderRootFindAllFiles, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Error: INVALID_HANDLE_VALUE with FindFirstFile() function.\n");
        return;
    }

    do
    {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (strcmp(FindFileData.cFileName, ".") != 0)
            {
                if (strcmp(FindFileData.cFileName, "..") != 0)
                {
                    char *folderName = FindFileData.cFileName;

                    printf("Folder: %s\n", folderName);

                    char searchForNextFile[MAX_PATH] = {0};
                    sprintf(searchForNextFile, "%s%s/", searchPath, folderName);

                    searchForFiles(searchForNextFile, fileExtension, fileNameFilter);
                }
            }
        }
        else
        {
            char *fileName = FindFileData.cFileName;

            if (PathMatchSpec(fileName, fileExtension))
            {
                if (sizeof(fileNameFilter) > 0)
                {
                    if (strstr(fileName, fileNameFilter) == NULL)
                        continue;
                }

                printf("File: %s%s\n", searchPath, fileName);

                FILE *file = fopen("recursivefind.txt", "a");
                if (file != NULL)
                {
                    fprintf(file, "%s%s\n", searchPath, fileName);
                }
                fclose(file);
            }
        }
    }
    while (FindNextFile(hFind, &FindFileData) != 0);

    FindClose(hFind);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: searchPath fileExtension fileNameFilter");
        return 1;
    }

    searchForFiles(argv[1], argv[2], argv[3]);

    return 0;
}
