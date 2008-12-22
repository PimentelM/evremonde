#include <windows.h>

typedef int (WINAPI *SENDPACKET)(DWORD, char*);

SENDPACKET SendPacket;

int main()
{
    char packet[3];
    packet[0] = 0x01;
    packet[1] = 0x00;
    packet[2] = 0x65;

    HWND tibiaWindow = FindWindow("tibiaclient", 0);

    DWORD processId;
    GetWindowThreadProcessId(tibiaWindow, &processId);

    HINSTANCE dll = LoadLibrary("packet840.dll");
    SendPacket = (SENDPACKET)GetProcAddress(dll, "SendPacket");

    SendPacket(processId, packet);

    if(dll)
        FreeLibrary(dll);

    return 0;
}
