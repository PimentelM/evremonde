/*
 * Zezenia 2.90 Level Spy
 * by Evremonde
 * Last updated on 03/11/2009
 */

#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <psapi.h>
#include <shellapi.h>

#include "resource.h"

/* defines */

// window dimensions
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH  240

// tray notification message
#define WM_SHELLNOTIFY WM_USER+5

/* ids */

// tray icon
#define ID_TRAY_ICON               101

// tray menu
#define ID_TRAY_MENU_NULL          1001
#define ID_TRAY_MENU_LEVELSPY_UP   1002
#define ID_TRAY_MENU_LEVELSPY_DOWN 1003
#define ID_TRAY_MENU_HOTKEYS       1004
#define ID_TRAY_MENU_ABOUT         1005
#define ID_TRAY_MENU_HOMEPAGE      1006
#define ID_TRAY_MENU_EXIT          1007

// hotkeys
#define ID_HOTKEY_LEVELSPY_UP      10001
#define ID_HOTKEY_LEVELSPY_DOWN    10002

// timers

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

// tray menu
HMENU trayMenu;

// hotkeys
int VK_HOTKEY_LEVELSPY_UP   = VK_ADD;      // +
int VK_HOTKEY_LEVELSPY_DOWN = VK_SUBTRACT; // -

/* memory addresses */

// level spy
#define LEVELSPY_CAVE   0x00400019
#define LEVELSPY_HOLDER 0x0040010F
#define LEVELSPY_JUMP   0x004B7016

/* constants */

// level spy
BYTE LEVELSPY_CAVE_PATCH[17] =
{
    0x8B, 0x42, 0x08,                   // mov eax,[edx+08]
    0x89, 0x41, 0x08,                   // mov [ecx+08],eax
    0x89, 0x15, 0x0F, 0x01, 0x40, 0x00, // mov [0040010f],edx : 06329BBC
    0xE9, 0xF1, 0x6F, 0x0B, 0x00        // jmp 004b701b
};

BYTE LEVELSPY_JUMP_PATCH[6] =
{
    0xE9, 0xFE, 0x8F, 0xF4, 0xFF, // jmp 00400019
    0x90                          // nop
};

const int LEVELSPY_MIN = 0;
const int LEVELSPY_MAX = 7;

/* booleans */

// hotkeys
int bHotkeys = 1;

/* functions */

// get game process id
DWORD getGameProcessId()
{
    DWORD aProcesses[1024];
    DWORD cProcesses;
    DWORD cbNeeded;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return 0;

    cProcesses = cbNeeded / sizeof(DWORD);

    DWORD processId;

    unsigned int i;
    for (i = 0; i < cProcesses; i++)
    {
        if(aProcesses[i] != 0)
        {
            processId = aProcesses[i];

            char processName[MAX_PATH] = "";

            HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

            if (processHandle)
            {
                HMODULE hMod;

                if (EnumProcessModules(processHandle, &hMod, sizeof(hMod), &cbNeeded))
                    GetModuleBaseName(processHandle, hMod, processName, sizeof(processName) / sizeof(char));
            }

            CloseHandle(processHandle);

            if (strcmp(processName, "Zezenia.exe") == 0)
                return processId;
        }
    }

    return 0;
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

// write string to memory
void writeString(DWORD address, char* text)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // write to memory
    WriteProcessMemory(processHandle, (LPVOID)address, text, strlen(text) + 1, 0);

    // close process handle
    CloseHandle(processHandle);
}

// write NOPs to memory
void writeNops(DWORD address, int nops)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // write to memory
    int i = 0;
    for (i = 0; i < nops; i++)
    {
        unsigned char nop = 0x90;
        WriteProcessMemory(processHandle, (LPVOID)(address + i), &nop, 1, 0);
    }

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

// read string from memory
char* readString(DWORD address)
{
    // get process handle
    HANDLE processHandle = getGameProcessHandle();

    // read from memory
    static char buffer[256];
    ReadProcessMemory(processHandle, (LPVOID)address, &buffer, sizeof(buffer), 0);

    // close process handle
    CloseHandle(processHandle);

    return buffer;
}

// initialize level spy
void doLevelSpyInitialize()
{
    DWORD oldProtection;
    DWORD newProtection;

    // level spy cave
    VirtualProtectEx(getGameProcessHandle(), (LPVOID)LEVELSPY_CAVE, sizeof(LEVELSPY_CAVE_PATCH), PAGE_EXECUTE_READWRITE, &oldProtection);
    WriteProcessMemory(getGameProcessHandle(), (LPVOID)LEVELSPY_CAVE, &LEVELSPY_CAVE_PATCH, sizeof(LEVELSPY_CAVE_PATCH), 0);
    //VirtualProtectEx(getGameProcessHandle(), (LPVOID)LEVELSPY_CAVE, sizeof(LEVELSPY_CAVE_PATCH), oldProtection, &newProtection);

    // level spy jump
    VirtualProtectEx(getGameProcessHandle(), (LPVOID)LEVELSPY_JUMP, sizeof(LEVELSPY_JUMP_PATCH), PAGE_EXECUTE_READWRITE, &oldProtection);
    WriteProcessMemory(getGameProcessHandle(), (LPVOID)LEVELSPY_JUMP, &LEVELSPY_JUMP_PATCH, sizeof(LEVELSPY_JUMP_PATCH), 0);
    //VirtualProtectEx(getGameProcessHandle(), (LPVOID)LEVELSPY_JUMP, sizeof(LEVELSPY_JUMP_PATCH), oldProtection, &newProtection);
}

// level spy up
void doLevelSpyUp()
{
    // get level spy address
    int levelSpyAddress = readBytes(LEVELSPY_HOLDER, 4) + 8;

    // get current level
    int currentLevel = readBytes(levelSpyAddress, 1);

    // get spy level
    int spyLevel = 0;
    if (currentLevel <= LEVELSPY_MIN)
        spyLevel = LEVELSPY_MAX; // loop back to start
    else
        spyLevel = currentLevel - 1; // decrease spy level

    // level spy up
    writeBytes(levelSpyAddress, spyLevel, 1);
}

// level spy down
void doLevelSpyDown()
{
    // get level spy address
    int levelSpyAddress = readBytes(LEVELSPY_HOLDER, 4) + 8;

    // get current level
    int currentLevel = readBytes(levelSpyAddress, 1);

    // get spy level
    int spyLevel = 0;
    if (currentLevel >= LEVELSPY_MAX)
        spyLevel = LEVELSPY_MIN; // loop back to start
    else
        spyLevel = currentLevel + 1; // increase spy level

    // level spy down
    writeBytes(levelSpyAddress, spyLevel, 1);
}

// register hotkeys
void doHotkeysRegister(HWND hwnd)
{
    RegisterHotKey(hwnd, ID_HOTKEY_LEVELSPY_UP,   0, VK_HOTKEY_LEVELSPY_UP);
    RegisterHotKey(hwnd, ID_HOTKEY_LEVELSPY_DOWN, 0, VK_HOTKEY_LEVELSPY_DOWN);
}

// unregister hotkeys
void doHotkeysUnregister(HWND hwnd)
{
    UnregisterHotKey(hwnd, ID_HOTKEY_LEVELSPY_UP);
    UnregisterHotKey(hwnd, ID_HOTKEY_LEVELSPY_DOWN);
}

// toggle hotkeys
void doHotkeysToggle()
{
    // check if hotkeys are enabled
    if (bHotkeys == 0)
    {
        // enable hotkeys
        bHotkeys = 1;

        CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_CHECKED);
    }
    else
    {
        // disable hotkeys
        bHotkeys = 0;

        CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_UNCHECKED);
    }
}

/* tray icon */

// create tray icon
void doTrayIconCreate(HWND hwnd)
{
    // initialize tray icon
    trayIcon.cbSize           = sizeof(NOTIFYICONDATA);
    trayIcon.hWnd             = hwnd;
    trayIcon.uID              = ID_TRAY_ICON;
    trayIcon.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    trayIcon.uCallbackMessage = WM_SHELLNOTIFY;
    trayIcon.hIcon            = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_APPLICATION), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON), LR_SHARED); //LoadIcon(0, IDI_WINLOGO);

    // set tray icon tip text
    strcpy(trayIcon.szTip, "Zezenia 2.90 Level Spy");

    // add tray icon
    Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* tray menu */

// update tray menu
void doTrayMenuUpdate(HWND hwnd)
{
    // check if hotkeys are enabled
    if (bHotkeys == 1)
        CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_CHECKED);
    else
        CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_UNCHECKED);
}

// create tray menu
void doTrayMenuCreate(HWND hwnd)
{
    // create tray menu
    trayMenu = CreatePopupMenu();
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_NULL,          "Zezenia 2.90 Level Spy");
    AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LEVELSPY_UP,   "Level Spy Up\t+");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_LEVELSPY_DOWN, "Level Spy Down\t-");
    AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOTKEYS,       "Hotkeys");
    AppendMenu(trayMenu, MF_SEPARATOR, 0, 0);
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_ABOUT,         "About...");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_HOMEPAGE,      "Homepage...");
    AppendMenu(trayMenu, MF_STRING, ID_TRAY_MENU_EXIT,          "Exit");

    // set default tray menu item
    SetMenuDefaultItem(trayMenu, ID_TRAY_MENU_NULL, MF_BYCOMMAND);

    // check hotkeys menu
    CheckMenuItem(trayMenu, ID_TRAY_MENU_HOTKEYS, MF_CHECKED);

    // update tray menu
    doTrayMenuUpdate(hwnd);
}

// show tray menu
void doTrayMenuShow(HWND hwnd)
{
    // update tray menu
    doTrayMenuUpdate(hwnd);

    // get menu location
    POINT trayMenuLocation;
    GetCursorPos(&trayMenuLocation);

    // required to make menu show and hide correctly
    SetForegroundWindow(hwnd);

    // show tray menu
    TrackPopupMenu(trayMenu,
        TPM_RIGHTALIGN | TPM_RIGHTBUTTON, // align right and allow right-click on menu items
        trayMenuLocation.x, trayMenuLocation.y,
        0, hwnd, 0);

    // required to make menu show and hide correctly
    PostMessage(hwnd, WM_NULL, 0, 0);
}

// show about
void doShowAbout()
{
    MessageBox(0,
        "Zezenia 2.90 Level Spy\nby Evremonde\nLast updated on 03/11/09\n\nHotkeys:\n+\tLevel Spy Up\n-\tLevel Spy Down\n\nHomepage:\nhttp://code.google.com/p/evremonde/",
        "About", MB_OK | MB_ICONINFORMATION);
}

// open homepage
void doOpenHomepage()
    { ShellExecute(0, 0, "http://code.google.com/p/evremonde/", 0, 0, SW_SHOW); }

// load ini
void doLoadIni()
{
    /* hotkeys */

    // level spy
    VK_HOTKEY_LEVELSPY_UP = GetPrivateProfileInt("Hotkeys", "LevelSpyUp", VK_HOTKEY_LEVELSPY_UP, "./zezenia290levelspy.ini");
    VK_HOTKEY_LEVELSPY_DOWN = GetPrivateProfileInt("Hotkeys", "LevelSpyDown", VK_HOTKEY_LEVELSPY_DOWN, "./zezenia290levelspy.ini");

    /* options */

    // hotkeys
    if (GetPrivateProfileInt("Options", "Hotkeys", 1, "./zezenia290levelspy.ini") == 0)
        bHotkeys = 0;
    else
        bHotkeys = 1;
}

/* windows messages */

void onCreate(HWND hwnd)
{
    // initialize level spy
    doLevelSpyInitialize();

    // load ini
    doLoadIni();

    // create tray icon
    doTrayIconCreate(hwnd);

    // create tray menu
    doTrayMenuCreate(hwnd);

    // register hotkeys
    doHotkeysRegister(hwnd);

    // check if num lock is enabled
    if (GetKeyState(VK_NUMLOCK) == 0)
    {
        // enable num lock for hotkeys
        keybd_event(VK_NUMLOCK, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(VK_NUMLOCK, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }
}

void onDestroy(HWND hwnd)
{
    // delete tray icon
    Shell_NotifyIcon(NIM_DELETE, &trayIcon);

    // destroy tray menu
    DestroyMenu(trayMenu);

    // unregister hotkeys
    doHotkeysUnregister(hwnd);

    // exit
    PostQuitMessage(0);
}

void onTray(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // tray icon
    if (wparam == ID_TRAY_ICON)
    {
        // left click or right click
        if (lparam == WM_LBUTTONDOWN || lparam == WM_RBUTTONDOWN)
        {
            // show tray menu
            doTrayMenuShow(hwnd);
        }
    }
}

void onHotkey(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // check if hotkeys are enabled
    if (bHotkeys == 0)
        return;

    // hotkeys
    switch (wparam)
    {
        // level spy up
        case ID_HOTKEY_LEVELSPY_UP:
            doLevelSpyUp();
            break;

        // level spy down
        case ID_HOTKEY_LEVELSPY_DOWN:
            doLevelSpyDown();
            break;
    }
}

void onCommand(HWND hwnd, int command, int notifycode, HWND hwnditem)
{
    switch (command)
    {
        // level spy up
        case ID_TRAY_MENU_LEVELSPY_UP:
            doLevelSpyUp();
            break;

        // level spy down
        case ID_TRAY_MENU_LEVELSPY_DOWN:
            doLevelSpyDown();
            break;

        // hotkeys
        case ID_TRAY_MENU_HOTKEYS:
            doHotkeysToggle();
            break;

        // about
        case ID_TRAY_MENU_ABOUT:
            doShowAbout();
            break;

        // homepage
        case ID_TRAY_MENU_HOMEPAGE:
            doOpenHomepage();
            break;

        // exit
        case ID_TRAY_MENU_EXIT:
            DestroyWindow(hwnd); // exit
            break;
    }
}

/* window messages handler */

LRESULT CALLBACK windowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        case WM_CREATE:
            onCreate(hwnd);
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd); // exit
            break;

        case WM_DESTROY:
            onDestroy(hwnd);
            break;

        case WM_SHELLNOTIFY:
            onTray(hwnd, wparam, lparam);
            break;

        case WM_HOTKEY:
            onHotkey(hwnd, wparam, lparam);
            break;

        case WM_COMMAND:
            onCommand(hwnd, LOWORD(wparam), HIWORD(wparam), (HWND)lparam);
            break;

        default:
            return DefWindowProc(hwnd, umsg, wparam, lparam);
    }
    return 0;
}

/* main function */

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR cmdline, int cmdshow)
{
    // only allow one instance of application
    HWND applicationWindow = FindWindow(0, "Zezenia 2.90 Level Spy");
    if (applicationWindow)
        return 0;

    // register window class
    WNDCLASSEX wc;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = windowProc; // window messages handler
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hinstance;
    wc.hIcon         = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_APPLICATION), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CXICON), LR_SHARED); //LoadIcon(0, IDI_WINLOGO);
    wc.hIconSm       = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON_APPLICATION), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON), LR_SHARED); //LoadIcon(0, IDI_WINLOGO);
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = 0; //(HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); // window background color
    wc.lpszMenuName  = 0;
    wc.lpszClassName = "zezenia290levelspy";

    if(!RegisterClassEx(&wc))
    {
        MessageBox(0,
            "Register Window Class failed!",
            "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // create window
    HWND hwnd;
    hwnd = CreateWindowEx(
        WS_EX_DLGMODALFRAME, // extended window style
        "zezenia290levelspy", "Zezenia 2.90 Level Spy", // window class and title
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window style
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_HEIGHT, WINDOW_WIDTH, // window dimensions
        HWND_DESKTOP, 0, hinstance, 0);

    if(hwnd == 0)
    {
        MessageBox(0,
            "Create Window failed!",
            "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // center window
    //HWND desktopWindow = GetDesktopWindow();
    //RECT desktopRect;
    //GetWindowRect(desktopWindow, &desktopRect);

    //RECT hwndRect;
    //GetWindowRect(hwnd, &hwndRect);

    //hwndRect.left = desktopRect.left + ((desktopRect.right  - desktopRect.left) - (hwndRect.right  - hwndRect.left)) / 2;
    //hwndRect.top  = desktopRect.top  + ((desktopRect.bottom - desktopRect.top)  - (hwndRect.bottom - hwndRect.top))  / 2;

    //SetWindowPos(hwnd, 0, hwndRect.left, hwndRect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

    // show window
    //ShowWindow(hwnd, cmdshow);
    //UpdateWindow(hwnd);

    // message loop
    MSG msg;
    while(GetMessage(&msg, 0, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
