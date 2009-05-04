/**
 *  Looter for Tibia 8.42
 *  by Evremonde
 *  Last updated on 5/3/2009
 */

#include <windows.h>

int main()
{
    // get game window
    HWND gameWindow = FindWindow("tibiaclient", 0);

    // get process id
    DWORD processId;
    GetWindowThreadProcessId(gameWindow, &processId);

    // get process handle
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

    // get full path name of dll
    char dllName[MAX_PATH] = {0};
    GetFullPathName("looter842.dll", MAX_PATH, dllName, NULL);

    // inject dll
    LPVOID remoteMemory = VirtualAllocEx(processHandle, NULL, strlen(dllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    WriteProcessMemory(processHandle, remoteMemory, dllName, strlen(dllName), 0);

    HANDLE remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA"), remoteMemory, 0, NULL);

    WaitForSingleObject(remoteThread, INFINITE);

    VirtualFreeEx(processHandle, remoteMemory, strlen(dllName), MEM_RELEASE);

    // close process handle
    CloseHandle(processHandle);

    return 0;
}
