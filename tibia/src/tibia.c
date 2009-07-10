#include "tibia.h"

DLL_EXPORT void tibiaMessageBox()
{
    MessageBox(NULL, "Tibia version is: " TIBIA_VERSION, "tibiaMessageBox", MB_OK);
}

// dll main
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}
