/*
 * Call of Duty 4 1.7.568
 * Ranks & Challenges Weapons Trainer
 * by Evremonde
 * Last updated on 12/21/2008
 */

#include <windows.h>

/* memory addresses */

// assault
#define ASSAULT_BEGIN 0x0CC19C38
#define ASSAULT_END   0x0CC19C6C // 0x0CC19C68 + 4

// smg
#define SMG_BEGIN     0x0CC19C88
#define SMG_END       0x0CC19CB0 // 0x0CC19CAC + 4

// lmg
#define LMG_BEGIN     0x0CC19CD8
#define LMG_END       0x0CC19CF0 // 0x0CC19CEC + 4

// shotgun
#define SHOTGUN_BEGIN 0x0CC19D28
#define SHOTGUN_END   0x0CC19D38 // 0x0CC19D34 + 4

// sniper
#define SNIPER_BEGIN  0x0CC19D78
#define SNIPER_END    0x0CC19DA0 // 0x0CC19D9C + 4

/* constants */

// distances to step between addresses in memory
const int STEP_WEAPON   = 4;
const int STEP_MARKSMAN = 8;

// ranks
const int RANK_25  = 25;
const int RANK_50  = 50;
const int RANK_75  = 75;
const int RANK_100 = 100; // sniper only
const int RANK_150 = 150;

/* functions */

// get game window
HWND getGameWindow()
    { return FindWindow(0, "Call of Duty 4"); }

// get game process id
DWORD getGameProcessId()
{
    // get game window
    HWND gameWindow = getGameWindow();

    // get process id
    DWORD processId;
    GetWindowThreadProcessId(gameWindow, &processId);

    return processId;
}

// get game process handle
HANDLE getGameProcessHandle()
{
    // get process id
    DWORD processId = getGameProcessId();

    // get process handle
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

    return processHandle;
}

// write bytes to memory
void writeBytes(DWORD address, int value, int bytes)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // write to memory
    WriteProcessMemory(processHandle, (LPVOID)address, &value, bytes, 0);

    // close process handle
    CloseHandle(processHandle);
}

// read bytes from memory
int readBytes(DWORD address, int bytes)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // read from memory
    int buffer = 0;
    ReadProcessMemory(processHandle, (LPVOID)address, &buffer, bytes, 0);

    // close process handle
    CloseHandle(processHandle);

    return buffer;
}

/* main function */

int main()
{
    int i = 0;

    // assault
    for (i = ASSAULT_BEGIN; i < ASSAULT_END; i += STEP_WEAPON)
    {
        int rank = readBytes(i, 4);

        int newRank = 0;
        if (rank < RANK_25)
            newRank = RANK_25 - 1;
        else if (rank < RANK_75 && rank >= RANK_25)
            newRank = RANK_75 - 1;
        else if (rank < RANK_150 && rank >= RANK_75)
            newRank = RANK_150 - 1;

        if (newRank > 0)
            writeBytes(i, newRank, 4);
    }

    // smg
    for (i = SMG_BEGIN; i < SMG_END; i += STEP_WEAPON)
    {
        int rank = readBytes(i, 4);

        int newRank = 0;
        if (rank < RANK_25)
            newRank = RANK_25 - 1;
        else if (rank < RANK_75 && rank >= RANK_25)
            newRank = RANK_75 - 1;
        else if (rank < RANK_150 && rank >= RANK_75)
            newRank = RANK_150 - 1;

        if (newRank > 0)
            writeBytes(i, newRank, 4);
    }

    // lmg
    for (i = LMG_BEGIN; i < LMG_END; i += STEP_WEAPON)
    {
        int rank = readBytes(i, 4);

        int newRank = 0;
        if (rank < RANK_25)
            newRank = RANK_25 - 1;
        else if (rank < RANK_75 && rank >= RANK_25)
            newRank = RANK_75 - 1;
        else if (rank < RANK_150 && rank >= RANK_75)
            newRank = RANK_150 - 1;

        if (newRank > 0)
            writeBytes(i, newRank, 4);
    }

    // shotgun
    for (i = SHOTGUN_BEGIN; i < SHOTGUN_END; i += STEP_WEAPON)
    {
        int rank = readBytes(i, 4);

        int newRank = 0;
        if (rank < RANK_25)
            newRank = RANK_25 - 1;
        else if (rank < RANK_50 && rank >= RANK_25)
            newRank = RANK_50 - 1;

        if (newRank > 0)
            writeBytes(i, newRank, 4);
    }

    // sniper
    for (i = SNIPER_BEGIN; i < SNIPER_END; i += STEP_WEAPON)
    {
        int rank = readBytes(i, 4);

        int newRank = 0;
        if (rank < RANK_25)
            newRank = RANK_25 - 1;
        else if (rank < RANK_75 && rank >= RANK_25)
            newRank = RANK_75 - 1;
        else if (rank < RANK_100 && rank >= RANK_25)
            newRank = RANK_100 - 1;

        if (newRank > 0)
            writeBytes(i, newRank, 4);
    }
    for (i = SNIPER_BEGIN; i < SNIPER_END; i += STEP_MARKSMAN)
    {
        int rank = readBytes(i, 4);

        int newRank = 0;
        if (rank < RANK_100)
            newRank = RANK_100 - 1;
        else if (rank < RANK_150 && rank >= RANK_100)
            newRank = RANK_150 - 1;

        if (newRank > 0)
            writeBytes(i, newRank, 4);
    }

    return 0;
}
