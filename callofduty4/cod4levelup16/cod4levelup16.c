/*
Call of Duty 4 Multiplayer 1.6.565
Level Up Trainer
by Evremonde
Last updated on June 30th, 2008
*/

#include <windows.h>

// memory addresses
#define PLAYER_EXP		0x0CC19898 // = XP
#define PLAYER_EXP_NEXT	0x0CC19964 // = XP + XP Required

// main function
int main()
{
	// get game window
	HWND gameWindow = FindWindow(0, "Call of Duty 4");
	if (gameWindow == 0)
	{
		MessageBox(0,
			"Call of Duty 4 window not found!",
			"Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	// get process id
	DWORD processId;
	GetWindowThreadProcessId(gameWindow, &processId);

	// get process handle
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

	// get player exp
	int playerExp = 0;
	ReadProcessMemory(processHandle, (LPVOID)PLAYER_EXP, &playerExp, 4, 0);

	// get player exp next
	int playerExpNext = 0;
	ReadProcessMemory(processHandle, (LPVOID)PLAYER_EXP_NEXT, &playerExpNext, 4, 0);

	// calculate needed experience
	int neededExp = playerExpNext - playerExp;

	// calculate level up experience
	int levelUpExp = (playerExp + neededExp) - 1; // 1 experience to level up

	// set player exp
	WriteProcessMemory(processHandle, (LPVOID)PLAYER_EXP, &levelUpExp, 4, 0);

	// close process handle
	CloseHandle(processHandle);

	// beep sound
	MessageBeep(0);

	// exit
	ExitProcess(0);

	return 0;
}
