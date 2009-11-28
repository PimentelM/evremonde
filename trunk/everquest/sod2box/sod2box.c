/**
 *  Shards of Dalaya 2 Box
 *  by Evremonde
 */

#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <shellapi.h>

#include "resource.h"

/* definitions */

// window properties
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH  240
#define WINDOW_MARGIN 10

// tray notification message
#define WM_SHELLNOTIFY WM_USER+5

// application name
#define APPLICATION_NAME "Shards of Dalaya 2 Box"

// application class name
#define APPLICATION_CLASS_NAME "sod2box"

// application homepage
#define APPLICATION_HOMEPAGE "http://code.google.com/p/evremonde/"

// game window title
#define GAME_WINDOW_TITLE "EverQuest"

/* ids */

// tray icon
#define ID_TRAY_ICON 101

// hotkeys
#define ID_HOTKEY_SHOW_NEXT_WINDOW      1001
#define ID_HOTKEY_SHOW_AND_HIDE_WINDOWS 1002
#define ID_HOTKEY_WINDOWED_NO_BORDER    1003

/* globals */

// tray icon
NOTIFYICONDATA trayIcon;

/* functions */

// center window
void centerWindow(HWND hwnd)
{
    // get desktop window
    HWND desktopWindow = GetDesktopWindow();

    // get desktop window dimensions
    RECT desktopRect;
    GetWindowRect(desktopWindow, &desktopRect);

    // get window dimensions
    RECT hwndRect;
    GetWindowRect(hwnd, &hwndRect);

    // calculate center of desktop window
    hwndRect.left = desktopRect.left + ((desktopRect.right  - desktopRect.left) - (hwndRect.right  - hwndRect.left)) / 2;
    hwndRect.top  = desktopRect.top  + ((desktopRect.bottom - desktopRect.top)  - (hwndRect.bottom - hwndRect.top))  / 2;

    // get taskbar window
    HWND taskbarWindow = FindWindow("Shell_TrayWnd", NULL);

    // get taskbar window dimensions
    RECT taskbarRect;
    GetWindowRect(taskbarWindow, &taskbarRect);

    // substract taskbar window height
    hwndRect.top = hwndRect.top - (taskbarRect.bottom - taskbarRect.top) / 2;

    // set window position
    SetWindowPos(hwnd, HWND_TOP, hwndRect.left, hwndRect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

    // update window
    UpdateWindow(hwnd);
}

// windowed no border window
void windowedNoBorderWindow(HWND hwnd)
{
    // get window style
    DWORD windowStyle = GetWindowLong(hwnd, GWL_STYLE);

    // set window style
    SetWindowLong(hwnd, GWL_STYLE, windowStyle &~ WS_BORDER &~ WS_CAPTION &~ WS_MAXIMIZEBOX &~ WS_MINIMIZEBOX &~ WS_SYSMENU &~ WS_THICKFRAME);

    // get window extended style
    DWORD windowExtendedStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

    // set window extended style
    SetWindowLong(hwnd, GWL_EXSTYLE, windowExtendedStyle &~ WS_EX_CLIENTEDGE &~ WS_EX_WINDOWEDGE);

    // get desktop window
    HWND desktopWindow = GetDesktopWindow();

    // get desktop window dimensions
    RECT desktopRect;
    GetWindowRect(desktopWindow, &desktopRect);

    // set window position
    SetWindowPos(hwnd, HWND_TOP, 0, 0, desktopRect.right - desktopRect.left, desktopRect.bottom - desktopRect.top, SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // update window
    UpdateWindow(hwnd);
}

// create tray icon
void createTrayIcon(HWND hwnd)
{
    // initialize tray icon
    trayIcon.cbSize           = sizeof(NOTIFYICONDATA);
    trayIcon.hWnd             = hwnd;
    trayIcon.uID              = ID_TRAY_ICON;
    trayIcon.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    trayIcon.uCallbackMessage = WM_SHELLNOTIFY;
    //trayIcon.hIcon          = LoadIcon(NULL, IDI_WINLOGO);
    trayIcon.hIcon            = (HICON)LoadImage(GetModuleHandle(NULL),
                                                 MAKEINTRESOURCE(ID_APPLICATION_ICON),
                                                 IMAGE_ICON,
                                                 GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
                                                 LR_SHARED);

    // set tray icon tooltip text
    strcpy(trayIcon.szTip, APPLICATION_NAME);

    // add tray icon
    Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

/* window messages */

// WM_CREATE
void onCreate(HWND hwnd)
{
    // create tray icon
    createTrayIcon(hwnd);

    // register hotkeys
    RegisterHotKey(hwnd, ID_HOTKEY_SHOW_NEXT_WINDOW,      0,           VK_OEM_3); //        ~
    RegisterHotKey(hwnd, ID_HOTKEY_SHOW_AND_HIDE_WINDOWS, MOD_ALT,     VK_OEM_3); // ALT  + ~
    RegisterHotKey(hwnd, ID_HOTKEY_WINDOWED_NO_BORDER,    MOD_CONTROL, VK_OEM_3); // CTRL + ~
}

// WM_DESTROY
void onDestroy(HWND hwnd)
{
    // delete tray icon
    Shell_NotifyIcon(NIM_DELETE, &trayIcon);

    // unregister hotkeys
    UnregisterHotKey(hwnd, ID_HOTKEY_SHOW_NEXT_WINDOW);
    UnregisterHotKey(hwnd, ID_HOTKEY_SHOW_AND_HIDE_WINDOWS);
    UnregisterHotKey(hwnd, ID_HOTKEY_WINDOWED_NO_BORDER);

    // exit
    PostQuitMessage(0);
}

// WM_HOTKEY
void onHotkey(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // hotkeys
    switch (wparam)
    {
        // show next window
        case ID_HOTKEY_SHOW_NEXT_WINDOW:
        {
            // get game windows
            HWND gameWindow = FindWindow(NULL, GAME_WINDOW_TITLE);
            HWND nextGameWindow = FindWindowEx(NULL, gameWindow, NULL, GAME_WINDOW_TITLE);

            // check if next game window exists
            if (nextGameWindow == NULL)
            {
                // check if game window is visible
                if (GetWindowLong(gameWindow, GWL_STYLE) & WS_VISIBLE)
                    SetForegroundWindow(gameWindow); // set focus to game window
            }
            else
            {
                // check if next game window is visible
                if (GetWindowLong(nextGameWindow, GWL_STYLE) & WS_VISIBLE)
                    SetForegroundWindow(nextGameWindow); // set focus to next game window
            }
            break;
        }

        // show and hide windows
        case ID_HOTKEY_SHOW_AND_HIDE_WINDOWS:
        {
            // get game windows
            HWND gameWindow = FindWindow(NULL, GAME_WINDOW_TITLE);
            HWND nextGameWindow = FindWindowEx(NULL, gameWindow, NULL, GAME_WINDOW_TITLE);

            // check if game window is visible
            if (GetWindowLong(gameWindow, GWL_STYLE) & WS_VISIBLE)
            {
                // hide game windows
                ShowWindow(gameWindow,     SW_HIDE);
                ShowWindow(nextGameWindow, SW_HIDE);

                // update game windows
                UpdateWindow(gameWindow);
                UpdateWindow(nextGameWindow);

                // set focus to application window
                SetForegroundWindow(hwnd);
            }
            else
            {
                // show game windows
                ShowWindow(gameWindow,     SW_SHOW);
                ShowWindow(nextGameWindow, SW_SHOW);

                // update game windows
                UpdateWindow(gameWindow);
                UpdateWindow(nextGameWindow);

                // set focus to game window
                SetForegroundWindow(gameWindow);
            }
            break;
        }

        // windowed no border
        case ID_HOTKEY_WINDOWED_NO_BORDER:
        {
            // get game windows
            HWND gameWindow = FindWindow(NULL, GAME_WINDOW_TITLE);
            HWND nextGameWindow = FindWindowEx(NULL, gameWindow, NULL, GAME_WINDOW_TITLE);

            // windowed no border game window
            windowedNoBorderWindow(gameWindow);

            // windowed no border next game window
            windowedNoBorderWindow(nextGameWindow);

            break;
        }
    }
}

// WM_PAINT
void onPaint(HWND hwnd)
{
        // set drawing area
        RECT rect;
        rect.top    = WINDOW_MARGIN;
        rect.left   = WINDOW_MARGIN;
        rect.bottom = WINDOW_WIDTH  - (WINDOW_MARGIN * 2);
        rect.right  = WINDOW_HEIGHT - (WINDOW_MARGIN * 2);

        // begin paint
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);

        // draw text with windows variable font
        HFONT hfont = GetStockObject(ANSI_VAR_FONT);
        SelectObject(hdc, hfont);

        // draw text on transparent background
        SetBkMode(hdc, TRANSPARENT);

        // draw the text
        DRAWTEXTPARAMS dtp;
        dtp.cbSize       = sizeof(DRAWTEXTPARAMS);
        dtp.iLeftMargin  = 0;
        dtp.iRightMargin = 0;
        dtp.iTabLength   = 4;
        DrawTextEx(hdc,
                   "Hotkeys:\n~\t\t\tShow next window\nALT + ~\t\tShow/Hide windows\nCTRL + ~\tWindowed no border\n\nCompile Date:\n" __DATE__ "\n\nHomepage:\n" APPLICATION_HOMEPAGE,
                   -1, &rect, DT_TABSTOP | DT_EXPANDTABS, &dtp);

        // end paint
        EndPaint(hwnd, &ps);
}

// WM_SHELLNOTIFY
void onTray(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // tray icon clicked
    if (wparam == ID_TRAY_ICON)
    {
        // left click
        if (lparam == WM_LBUTTONUP)
        {
            // show and restore application window
            ShowWindow(hwnd, SW_SHOW);
            ShowWindow(hwnd, SW_RESTORE);
            UpdateWindow(hwnd);

            // set focus to application window
            SetForegroundWindow(hwnd);
        }

        // right click
        if (lparam == WM_RBUTTONUP)
            DestroyWindow(hwnd); // exit
    }
}

// WM_SIZE
void onSize(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    // application window is minimized
    if (wparam == SIZE_MINIMIZED)
    {
        // hide application window
        ShowWindow(hwnd, SW_HIDE);
        UpdateWindow(hwnd);
    }
}

/* window messages handler */

LRESULT CALLBACK windowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd); // exit
            break;

        case WM_CREATE:
            onCreate(hwnd);
            break;

        case WM_DESTROY:
            onDestroy(hwnd);
            break;

        case WM_HOTKEY:
            onHotkey(hwnd, wparam, lparam);
            break;

        case WM_PAINT:
            onPaint(hwnd);
            break;

        case WM_SHELLNOTIFY:
            onTray(hwnd, wparam, lparam);
            break;

        case WM_SIZE:
            onSize(hwnd, wparam, lparam);
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
    HWND applicationWindow = FindWindow(NULL, APPLICATION_NAME);
    if (applicationWindow != NULL)
        return 0;

    // register window class
    WNDCLASSEX wcx;
    wcx.cbSize        = sizeof(WNDCLASSEX);
    wcx.style         = 0;
    wcx.lpfnWndProc   = windowProc; // window messages handler
    wcx.cbClsExtra    = 0;
    wcx.cbWndExtra    = 0;
    wcx.hInstance     = hinstance;
    //wcx.hIcon       = LoadIcon(NULL, IDI_WINLOGO);
    //wcx.hIconSm     = LoadIcon(NULL, IDI_WINLOGO);
    wcx.hIcon         = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_APPLICATION_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXICON),   GetSystemMetrics(SM_CYICON),   LR_SHARED);
    wcx.hIconSm       = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_APPLICATION_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_SHARED);
    wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); // window background color
    wcx.lpszMenuName  = NULL;
    wcx.lpszClassName = APPLICATION_CLASS_NAME;

    RegisterClassEx(&wcx);

    // create window
    HWND hwnd;
    hwnd = CreateWindowEx(WS_EX_DLGMODALFRAME, // extended window styles
                          APPLICATION_CLASS_NAME, APPLICATION_NAME, // window class and title
                          WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window styles
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          WINDOW_HEIGHT, WINDOW_WIDTH, // window dimensions
                          HWND_DESKTOP, NULL, hinstance, NULL);

    // center window
    centerWindow(hwnd);

    // show window
    ShowWindow(hwnd, cmdshow);
    UpdateWindow(hwnd);

    // message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
