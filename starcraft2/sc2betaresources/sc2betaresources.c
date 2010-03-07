/**
 *  StarCraft 2 Beta Resources
 *  by Evremonde
 */

#include <stdio.h>

#include <windows.h>

#define GAME_WINDOW_TITLE "StarCraft II Beta"

#define SC2_PLAYER1_MINERALS    0x0235D0F8
#define SC2_PLAYER1_VESPENE_GAS 0x0235D100 // SC2_PLAYER1_MINERALS + 8

/*
    #define SC2_PLAYER2_MINERALS    0x0235D8C0 // SC2_PLAYER1_MINERALS + 1992
    #define SC2_PLAYER2_VESPENE_GAS 0x0235D8C8

    #define SC2_PLAYER3_MINERALS    0x0235E088
    #define SC2_PLAYER3_VESPENE_GAS 0x0235E090

    #define SC2_PLAYER4_MINERALS    0x0235E850
    #define SC2_PLAYER4_VESPENE_GAS 0x0235E858
*/

const int SC2_OFFSET_PLAYER  = 1992; // = 0x7C8

const int SC2_MAX_PLAYERS = 4;

const long SC2_MAX_RESOURCES = 99999;

int main()
{
    while (1)
    {
        HWND hwndGameWindow = FindWindow(NULL, GAME_WINDOW_TITLE);

        DWORD dwProcessId;
        GetWindowThreadProcessId(hwndGameWindow, &dwProcessId);

        HANDLE hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, dwProcessId);

        DWORD dwOldProtection;
        DWORD dwNewProtection;

        VirtualProtectEx(hProcessHandle, (LPVOID)SC2_PLAYER1_MINERALS, sizeof(SC2_MAX_RESOURCES), PAGE_WRITECOPY, &dwOldProtection);
        WriteProcessMemory(hProcessHandle, (LPVOID)SC2_PLAYER1_MINERALS, &SC2_MAX_RESOURCES, sizeof(SC2_MAX_RESOURCES), NULL);
        VirtualProtectEx(hProcessHandle, (LPVOID)SC2_PLAYER1_MINERALS, sizeof(SC2_MAX_RESOURCES), dwOldProtection, &dwNewProtection);

        VirtualProtectEx(hProcessHandle, (LPVOID)SC2_PLAYER1_VESPENE_GAS, sizeof(SC2_MAX_RESOURCES), PAGE_WRITECOPY, &dwOldProtection);
        WriteProcessMemory(hProcessHandle, (LPVOID)SC2_PLAYER1_VESPENE_GAS, &SC2_MAX_RESOURCES, sizeof(SC2_MAX_RESOURCES), NULL);
        VirtualProtectEx(hProcessHandle, (LPVOID)SC2_PLAYER1_VESPENE_GAS, sizeof(SC2_MAX_RESOURCES), dwOldProtection, &dwNewProtection);

        CloseHandle(hProcessHandle);
    }

    return 0;
}
