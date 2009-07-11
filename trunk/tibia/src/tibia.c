/**
 *  Tibia DLL
 *  by Evremonde
 */

#include "tibia.h"

// show version
DLL_EXPORT void tibiaShowVersion()
{
    MessageBox(NULL, "Tibia " TIBIA_VERSION, "Tibia Version", MB_OK | MB_ICONINFORMATION);
}

// get client window
DLL_EXPORT HWND tibiaGetClientWindow()
{
    return FindWindow("tibiaclient", NULL);
}

// get process id
DLL_EXPORT DWORD tibiaGetProcessId()
{
    // get client window
    HWND hwndClientWindow = tibiaGetClientWindow();

    // get process id
    DWORD dwProcessId;
    GetWindowThreadProcessId(hwndClientWindow, &dwProcessId);

    return dwProcessId;
}

// get process handle
DLL_EXPORT HANDLE tibiaGetProcessHandle()
{
    // get process id
    DWORD dwProcessId = tibiaGetProcessId();

    // get process handle
    HANDLE hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

    return hProcessHandle;
}

// write bytes to memory
DLL_EXPORT void tibiaWriteBytes(DWORD dwAddress, int value, int bytes)
{
    // get process handle
    HANDLE hProcessHandle = tibiaGetProcessHandle();

    // write to memory
    WriteProcessMemory(hProcessHandle, (LPVOID)dwAddress, &value, bytes, NULL);

    // close process handle
    CloseHandle(hProcessHandle);
}

// read bytes from memory
DLL_EXPORT int tibiaReadBytes(DWORD dwAddress, int bytes)
{
    // get process handle
    HANDLE hProcessHandle = tibiaGetProcessHandle();

    // read from memory
    int buffer = 0;
    ReadProcessMemory(hProcessHandle, (LPVOID)dwAddress, &buffer, bytes, NULL);

    // close process handle
    CloseHandle(hProcessHandle);

    return buffer;
}

// write string to memory
DLL_EXPORT void tibiaWriteString(DWORD dwAddress, char* text)
{
    // get process handle
    HANDLE hProcessHandle = tibiaGetProcessHandle();

    // write to memory
    WriteProcessMemory(hProcessHandle, (LPVOID)dwAddress, text, strlen(text) + 1, NULL);

    // close process handle
    CloseHandle(hProcessHandle);
}

// read string from memory
DLL_EXPORT char* tibiaReadString(DWORD dwAddress)
{
    // get process handle
    HANDLE hProcessHandle = tibiaGetProcessHandle();

    // read from memory
    static char buffer[256];
    ReadProcessMemory(hProcessHandle, (LPVOID)dwAddress, &buffer, sizeof(buffer), NULL);

    // close process handle
    CloseHandle(hProcessHandle);

    return buffer;
}

// write NOPs to memory
DLL_EXPORT void tibiaWriteNops(DWORD dwAddress, int nops)
{
    // get process handle
    HANDLE hProcessHandle = tibiaGetProcessHandle();

    // write to memory
    int i = 0;
    for (i = 0; i < nops; i++)
    {
        unsigned char nop = 0x90;
        WriteProcessMemory(hProcessHandle, (LPVOID)(dwAddress + i), &nop, 1, NULL);
    }

    // close process handle
    CloseHandle(hProcessHandle);
}

// set statusbar timer
DLL_EXPORT void tibiaSetStatusbarTimer(int duration)
{
    tibiaWriteBytes(TIBIA_STATUSBAR_TIMER, duration, 1);
}

// get statusbar timer
DLL_EXPORT int tibiaGetStatusbarTimer()
{
    return tibiaReadBytes(TIBIA_STATUSBAR_TIMER, 1);
}

// set statusbar text
DLL_EXPORT void tibiaSetStatusbarText(char* text)
{
    // set statusbar timer
    tibiaSetStatusbarTimer(TIBIA_STATUSBAR_TEXT_DEFAULT_DURATION);

    // set statusbar text
    tibiaWriteString(TIBIA_STATUSBAR_TEXT, text);
}

// get statusbar text
DLL_EXPORT char* tibiaGetStatusbarText()
{
    return tibiaReadString(TIBIA_STATUSBAR_TEXT);
}

// dll main
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}
