/*
 * Shards of Dalaya 2 Box
 * by Evremonde
 * Last updated on 12/21/2008
 */

#include <stdio.h>
#include <string.h>

#include <windows.h>
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
#define ID_TRAY_ICON 101

// hotkey
#define ID_HOTKEY_NEXTBOX 100001

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

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
    strcpy(trayIcon.szTip, "Shards of Dalaya 2 Box\nHotkey: ~");

    // add tray icon
    Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* windows messages */

void onCreate(HWND hwnd)
{
    // create tray icon
    doTrayIconCreate(hwnd);

    // register hotkey
    RegisterHotKey(hwnd, ID_HOTKEY_NEXTBOX, 0, VK_OEM_3); // ~
}

void onDestroy(HWND hwnd)
{
    // remove tray icon
    Shell_NotifyIcon(NIM_DELETE, &trayIcon);

    // unregister hotkey
    UnregisterHotKey(hwnd, ID_HOTKEY_NEXTBOX);

    // exit
    PostQuitMessage(0);
}

void onTray(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // tray icon
    if (wparam == ID_TRAY_ICON)
    {
        // right click
        if (lparam == WM_RBUTTONUP)
        {
            // exit
            DestroyWindow(hwnd); // exit
        }
    }
}

void onHotkey(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // hotkey
    if (wparam == ID_HOTKEY_NEXTBOX)
    {
        // get game windows
        HWND gameWindow = FindWindow(0, "EverQuest");
        HWND nextGameWindow = FindWindowEx(0, gameWindow, 0, "EverQuest");

        // set focus to next game window
        SetForegroundWindow(nextGameWindow);
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

        default:
            return DefWindowProc(hwnd, umsg, wparam, lparam);
    }
    return 0;
}

/* main function */

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR cmdline, int cmdshow)
{
    // only allow one instance of application
    HWND applicationWindow = FindWindow(0, "Shards of Dalaya 2 Box");
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
    wc.lpszClassName = "sod2box";

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
        "sod2box", "Shards of Dalaya 2 Box", // window class and title
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
