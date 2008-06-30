#include <windows.h>
#include "madCHook.h"	// madCodeHook

/*
Evre Bot
by Evremonde
for Tibia version 7.6
*/

// BelowNormal process priority
const int PRIORITY_BELOW_NORMAL	= 16384;

int main()
{
	HWND tibiaWindow;
	DWORD processId;
	HANDLE processHandle;

	// find tibia window
	tibiaWindow = FindWindow("tibiaclient", NULL);
    if (tibiaWindow)
	{
		// get process id
		GetWindowThreadProcessId(tibiaWindow, &processId);

		// get handle
		processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

		// set tibia to low cpu usage
		SetPriorityClass(processHandle, PRIORITY_BELOW_NORMAL);

		// inject dll into tibia
		InjectLibrary((DWORD)processHandle, "evrebot.dll");

		// close process
		return CloseHandle(processHandle);
	}
	else
		return MessageBox(NULL, "Tibia not found!", "evrebot", MB_ICONERROR);
}