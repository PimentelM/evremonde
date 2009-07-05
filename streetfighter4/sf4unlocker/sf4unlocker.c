/*
 * Street Fighter IV Unlocker
 * by Evremonde
 */

#include <windows.h>

// application name
#define APPLICATION_NAME "Street Fighter IV Unlocker"

// application homepage
#define APPLICATION_HOMEPAGE "http://code.google.com/p/evremonde/"

/* memory addresses */

// unlock characters
#define SF4_UNLOCK_CHARACTERS_BEGIN 0x00A4BB38
#define SF4_UNLOCK_CHARACTERS_END   0x00A4BB5C // = SF4_UNLOCK_CHARACTERS_BEGIN + (SF4_STEP_UNLOCK_CHARACTERS * SF4_MAX_UNLOCK_CHARACTERS)
// cammy
// fei-long
// sakura
// rose
// gen
// dan
// akuma
// gouken
// seth

// unlock colors
#define SF4_UNLOCK_COLORS_BEGIN 0x00A4BB5C
#define SF4_UNLOCK_COLORS_END   0x00A4BB7C // = SF4_UNLOCK_COLORS_BEGIN + (SF4_STEP_UNLOCK_COLORS * SF4_MAX_UNLOCK_COLORS)
// color 3
// color 4
// color 5
// color 6
// color 7
// color 8
// color 9
// color 10

// unlock personal actions
#define SF4_UNLOCK_PERSONAL_ACTIONS_BEGIN 0x00A4BB7C
#define SF4_UNLOCK_PERSONAL_ACTIONS_END   0x00A4BBA0 // = SF4_UNLOCK_PERSONAL_ACTIONS_BEGIN + (SF4_STEP_UNLOCK_PERSONAL_ACTIONS * SF4_MAX_UNLOCK_PERSONAL_ACTIONS)
// personal action 2
// personal action 3
// personal action 4
// personal action 5
// personal action 6
// personal action 7
// personal action 8
// personal action 9
// personal action 10

// unlock voice settings per character
#define SF4_UNLOCK_VOICE_SETTINGS_PER_CHARACTER 0x00A4BBA0

/* constants */

// locked/unlocked status
const int SF4_LOCKED   = 0;
const int SF4_UNLOCKED = 1;

// unlock characters
const int SF4_STEP_UNLOCK_CHARACTERS = 4;

const int SF4_MAX_UNLOCK_CHARACTERS = 9;

// unlock colors
const int SF4_STEP_UNLOCK_COLORS = 4;

const int SF4_MAX_UNLOCK_COLORS = 8;

// unlock personal actions
const int SF4_STEP_UNLOCK_PERSONAL_ACTIONS = 4;

const int SF4_MAX_UNLOCK_PERSONAL_ACTIONS = 9;

int main()
{
    // get game window
    HWND gameWindow = FindWindow(NULL, "STREET FIGHTER IV");
    if (gameWindow == NULL)
    {
        MessageBox(NULL,
            "Street Fighter IV window not found!",
            "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // get process id
    DWORD processId;
    GetWindowThreadProcessId(gameWindow, &processId);

    // get process handle
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

    int i = 0;

    // unlock characters
    for (i = SF4_UNLOCK_CHARACTERS_BEGIN; i < SF4_UNLOCK_CHARACTERS_END; i += SF4_STEP_UNLOCK_CHARACTERS)
        WriteProcessMemory(processHandle, (LPVOID)i, &SF4_UNLOCKED, 4, NULL);

    // unlock colors
    for (i = SF4_UNLOCK_COLORS_BEGIN; i < SF4_UNLOCK_COLORS_END; i += SF4_STEP_UNLOCK_COLORS)
        WriteProcessMemory(processHandle, (LPVOID)i, &SF4_UNLOCKED, 4, NULL);

    // unlock personal actions
    for (i = SF4_UNLOCK_PERSONAL_ACTIONS_BEGIN; i < SF4_UNLOCK_PERSONAL_ACTIONS_END; i += SF4_STEP_UNLOCK_PERSONAL_ACTIONS)
        WriteProcessMemory(processHandle, (LPVOID)i, &SF4_UNLOCKED, 4, NULL);

    // unlock voice settings per character
    WriteProcessMemory(processHandle, (LPVOID)SF4_UNLOCK_VOICE_SETTINGS_PER_CHARACTER, &SF4_UNLOCKED, 4, NULL);

    // close process handle
    CloseHandle(processHandle);

    // success message
    MessageBox(NULL,
        APPLICATION_NAME "\nby Evremonde\n\nStreet Fighter IV characters, colors, personal actions, and voice settings per character unlocked!\n\nCompile Date: " __DATE__ "\nHomepage: " APPLICATION_HOMEPAGE,
        "Success", MB_OK | MB_ICONINFORMATION);

    return 0;
}
