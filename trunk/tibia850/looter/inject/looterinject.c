/**
 *  Looter for Tibia
 *  by Evremonde
 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

// version
//#define TIBIA_VERSION "8.50"

// tibia version information
#define TIBIA_FILE_PRODUCT_NAME    "Tibia Player"
#define TIBIA_FILE_PRODUCT_VERSION "8.5.0.0"

// file names
#define FILENAME "looter.dll"

#define FILENAME_TIBIA_CLIENT "Tibia.exe"

int main()
{
    // check if tibia client file exists
    FILE *file = fopen(FILENAME_TIBIA_CLIENT, "r");
    if (file == NULL)
    {
        MessageBox(NULL,
            FILENAME_TIBIA_CLIENT " not found!\nLooter files must be in Tibia folder!",
            "Error", MB_OK | MB_ICONERROR);
        fclose(file);
        return 0;
    }
    fclose(file);

    // get tibia client version
    DWORD dwHandle;
    DWORD dwVersionInfoSize = GetFileVersionInfoSize(FILENAME_TIBIA_CLIENT, &dwHandle);
    if (dwVersionInfoSize > 0)
    {
        void *pVersionInfo = GlobalAlloc(GMEM_FIXED, dwVersionInfoSize);
        if (pVersionInfo)
        {
            if (GetFileVersionInfo(FILENAME_TIBIA_CLIENT, 0, dwVersionInfoSize, pVersionInfo))
            {
                VS_FIXEDFILEINFO *pFileInfo;

                void *pBuffer;
                UINT uBufferSize;

                if (VerQueryValue(pVersionInfo, "\\", &pBuffer, &uBufferSize))
                {
                    pFileInfo = (VS_FIXEDFILEINFO *)pBuffer;

                    char productVersion[128] = {0};
                    sprintf(productVersion, "%d.%d.%d.%d",
                        HIWORD(pFileInfo->dwProductVersionMS),
                        LOWORD(pFileInfo->dwProductVersionMS),
                        HIWORD(pFileInfo->dwProductVersionLS),
                        LOWORD(pFileInfo->dwProductVersionLS));

                    // check if tibia client product version corresponds with application
                    if (strcmp(TIBIA_FILE_PRODUCT_VERSION, productVersion) != 0)
                    {
                        char versionError[512] = {0};
                        sprintf(versionError, "Sorry, Tibia client version \"%s\" is not valid!\nYou need Tibia client version \"" TIBIA_FILE_PRODUCT_VERSION "\" to use this software!", productVersion);
                        MessageBox(NULL, versionError, "Error", MB_OK | MB_ICONERROR);
                        GlobalFree(pVersionInfo);
                        return 0;
                    }

                    GlobalFree(pVersionInfo);
                }
            }
        }
    }

    // get tibia client window
    HWND hwndClientWindow = FindWindow("tibiaclient", NULL);
    if (hwndClientWindow == NULL)
    {
        MessageBox(NULL,
            "Tibia window not found!\nPlease open the Tibia client first!",
            "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // get process id
    DWORD dwProcessId;
    GetWindowThreadProcessId(hwndClientWindow, &dwProcessId);

    // get process handle
    HANDLE hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

    // get full path name of dll
    char dllPathName[MAX_PATH] = {0};
    GetFullPathName(FILENAME, MAX_PATH, dllPathName, NULL);

    // inject dll
    LPVOID lpRemoteMemory = VirtualAllocEx(hProcessHandle, NULL, strlen(dllPathName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    WriteProcessMemory(hProcessHandle, lpRemoteMemory, dllPathName, strlen(dllPathName), NULL);

    HANDLE hRemoteThread = CreateRemoteThread(hProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA"), lpRemoteMemory, 0, NULL);

    WaitForSingleObject(hRemoteThread, INFINITE);

    VirtualFreeEx(hProcessHandle, lpRemoteMemory, strlen(dllPathName), MEM_RELEASE);

    // close thread handle
    CloseHandle(hRemoteThread);

    // close process handle
    CloseHandle(hProcessHandle);

    return 0;
}
