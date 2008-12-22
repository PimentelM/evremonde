#include <windows.h>

typedef int (WINAPI *SENDPACKET)(DWORD, char*);

SENDPACKET SendPacket;

int main()
{
    // walk north packet
    char packet[3];
    packet[0] = 0x01;
    packet[1] = 0x00;
    packet[2] = 0x65;

    // get tibia window
    HWND tibiaWindow = FindWindow("tibiaclient", 0);

    // get process id
    DWORD processId;
    GetWindowThreadProcessId(tibiaWindow, &processId);

    // load packet dll
    HINSTANCE packetDll = LoadLibrary("packet840.dll");

    // get send packet function from packet dll
    SendPacket = (SENDPACKET)GetProcAddress(packetDll, "SendPacket");

    // send packet
    SendPacket(processId, packet);

    // unload packet dll
    if(packetDll)
        FreeLibrary(packetDll);

    return 0;
}
