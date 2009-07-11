/**
 *  Tibia DLL
 *  by Evremonde
 */

#include "tibia.h"

DLL_EXPORT void tibiaShowVersion()
{
    MessageBox(NULL, "Tibia " TIBIA_VERSION, "Tibia Version", MB_OK | MB_ICONINFORMATION);
}

// dll main
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}
