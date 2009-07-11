/**
 *  Tibia DLL Test
 *  by Evremonde
 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include <tibia/tibia.h>

#define FILENAME_TEST "test.txt"

int main()
{
    printf("Tibia DLL Test\n");
    printf("\n");

    printf("Tibia Version: " TIBIA_VERSION "\n");
    printf("\n");

    HWND hwndClientWindow = tibiaGetClientWindow();
    if (hwndClientWindow == NULL)
    {
        printf("Tibia client window NOT found!\n");
        printf("\n");
    }
    else
    {
        printf("Tibia client window found!\n");
        printf("\n");
    }

    printf("Printing test results to " FILENAME_TEST "...");

    FILE *file = fopen(FILENAME_TEST, "w");
    if (file != NULL)
    {
        fprintf(file, "tibiaGetStatusbarTimer: %i\n", tibiaGetStatusbarTimer());
        fprintf(file, "tibiaGetStatusbarText: %s\n", tibiaGetStatusbarText());
    }
    fclose(file);

    printf("Done!\n");
    printf("\n");

    printf("Press enter to exit...");

    // wait for key press
    getchar();

    return 0;
}
