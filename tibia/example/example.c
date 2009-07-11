/**
 *  Tibia DLL Example
 *  by Evremonde
 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include <tibia/tibia.h>

int main()
{
    printf("Tibia DLL Example\n");
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

    printf("tibiaGetStatusbarTimer: %i\n", tibiaGetStatusbarTimer());
    printf("tibiaGetStatusbarText: %s\n", tibiaGetStatusbarText());
    printf("\n");

    printf("Press enter to exit...");

    // wait for key press
    getchar();

    return 0;
}
