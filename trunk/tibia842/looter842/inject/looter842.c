/**
 *  Looter for Tibia 8.42
 *  by Evremonde
 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

int main()
{
    // check if tibia client file exists
    FILE *file = fopen("tibia.exe", "r");
    if (file == NULL)
    {
        MessageBox(NULL, "Tibia.exe not found!\nLooter files must be in Tibia folder!", "Error", MB_OK | MB_ICONERROR);
        fclose(file);
        return 0;
    }
    fclose(file);

    // get tibia client window
    HWND clientWindow = FindWindow("tibiaclient", NULL);
    if (clientWindow == NULL)
    {
        MessageBox(NULL, "Tibia window not found!\nPlease open the Tibia client first!", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // get process id
    DWORD processId;
    GetWindowThreadProcessId(clientWindow, &processId);

    // get process handle
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

    // get full path name of dll
    char dllPathName[MAX_PATH] = {0};
    GetFullPathName("looter842.dll", MAX_PATH, dllPathName, NULL);

    // inject dll
    LPVOID remoteMemory = VirtualAllocEx(processHandle, NULL, strlen(dllPathName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    WriteProcessMemory(processHandle, remoteMemory, dllPathName, strlen(dllPathName), 0);

    HANDLE remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA"), remoteMemory, 0, NULL);

    WaitForSingleObject(remoteThread, INFINITE);

    VirtualFreeEx(processHandle, remoteMemory, strlen(dllPathName), MEM_RELEASE);

    // close thread handle
    CloseHandle(remoteThread);

    // close process handle
    CloseHandle(processHandle);

    return 0;
}
