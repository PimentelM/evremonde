#include <sstream>

#include <windows.h>

#include "levelspy.h"

#include "tibia.h"

/*
Level Spy
by Evremonde
for Tibia 8.0
*/

// tibia class
CTibia Tibia;

/* toggle */

// toggle level spying
void toggleLevelSpy()
{
	// check if already enabled
	if (!Tibia.isLevelSpyEnabled())
	{
		// enable level spying
		Tibia.setLevelSpy(true);

		// set statusbar
		Tibia.setStatusbar("Level Spy: On");
	}
	else
	{
		// disable level spying
		Tibia.setLevelSpy(false);

		// set statusbar
		Tibia.setStatusbar("Level Spy: Off");
	}
}

// toggle name spying
void toggleNameSpy()
{
	// check if already enabled
	if (!Tibia.isNameSpyEnabled())
	{
		// enable name spying
		Tibia.setNameSpy(true);

		// set statusbar
		Tibia.setStatusbar("Name Spy: On");
	}
	else
	{
		// disable name spying
		Tibia.setNameSpy(false);

		// set statusbar
		Tibia.setStatusbar("Name Spy: Off");
	}
}

/* do */

void doLevelSpyUp()
{
    // level spy must be enabled
	if (Tibia.isLevelSpyEnabled())
	{
        // increase spy level
		Tibia.doLevelSpyUp();

		// set statusbar
		Tibia.setStatusbar("Level Spy: Up");
	}
	else
	{
		// note
		Tibia.setStatusbar("Please enable Level Spy first!");
	}
}

void doLevelSpyDown()
{
    // level spy must be enabled
	if (Tibia.isLevelSpyEnabled())
	{
        // decrease spy level
		Tibia.doLevelSpyDown();

		// set statusbar
		Tibia.setStatusbar("Level Spy: Down");
	}
	else
	{
		// note
		Tibia.setStatusbar("Please enable Level Spy first!");
	}
}

/* timer */

// hotkeys
void timerHotkeys(HWND hwnd)
{
	// restrict hotkeys to tibia and level spy by window focus
	HWND currentWindow = ::GetForegroundWindow();
	HWND tibiaWindow = Tibia.getTibiaWindow();
	if (currentWindow != tibiaWindow && currentWindow != hwnd)
		return;

	// increase spy level
	if (::GetAsyncKeyState(VK_PRIOR)) // page up
	{
		doLevelSpyUp();
		Sleep(100); // wait
	}

	// decrease spy level
	if (::GetAsyncKeyState(VK_NEXT)) // page down
	{
		doLevelSpyDown();
		Sleep(100); // wait
	}

	// toggle level spying
	if (::GetAsyncKeyState(VK_END)) // end
	{
		toggleLevelSpy();
		Sleep(100); // wait
	}

	// toggle name spying
	if (::GetAsyncKeyState(VK_HOME)) // home
	{
		toggleNameSpy();
		Sleep(100); // wait
	}
}

/* windows messages */

void onCreate(HWND hwnd)
{
	// initialize timers
	::SetTimer(hwnd, TMR_HOTKEYS, 10, 0); // speed is 10 milliseconds

	// loaded
	Tibia.setStatusbar("Level Spy for Tibia 8.0 loaded!");
}

void onDestroy(HWND hwnd)
{
	// disable level spying
	Tibia.setLevelSpy(false);

	// disable name spying
	Tibia.setNameSpy(false);

	// exit
	PostQuitMessage(0);
}

void onPaint(HWND hwnd)
{
	// set drawing area
	RECT rect;
	rect.top	= 10; // small margin
	rect.left	= 10; // small margin
	rect.bottom	= WINDOW_HEIGHT;
	rect.right	= WINDOW_WIDTH;

	// begin paint
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = ::BeginPaint(hwnd, &ps);

	// set text to windows font
	HFONT hfont = static_cast<HFONT>(::GetStockObject(ANSI_VAR_FONT));
	::SelectObject(hdc, hfont);

	// draw text on transparent background
	::SetBkMode(hdc, TRANSPARENT);
	
	// level spy text
	std::stringstream buffer;
	buffer << "Level for Tibia 8.0" << std::endl
		<< "by Evremonde" << std::endl
		<< "Last updated on November 19th, 2007" << std::endl << std::endl
		<< "Homepage:" << std::endl
		<< "http://code.google.com/p/evremonde/" << std::endl << std::endl
		<< "Hotkeys:" << std::endl
		<< "Page Up\t\tLevel spy up" << std::endl
		<< "Page Down\t\tLevel spy down" << std::endl
		<< "End\t\t\t\tToggle level spy on/off" << std::endl
		<< "Home\t\t\tToggle name spy on/off";

	// draw the text
	DRAWTEXTPARAMS dtp;
	dtp.cbSize			= sizeof(DRAWTEXTPARAMS);
	dtp.iLeftMargin		= 0;
	dtp.iRightMargin	= 0;
	dtp.iTabLength		= 4;
	::DrawTextEx(
		hdc,
		(char*)buffer.str().c_str(),
		-1, &rect, DT_TABSTOP | DT_EXPANDTABS, &dtp);

	//end paint
	::EndPaint(hwnd, &ps);
}

void onTimer(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		// hotkeys
		case TMR_HOTKEYS:
			timerHotkeys(hwnd);
			break;
	}
}

/* window messages handler */

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			onCreate(hwnd);
			break;

		case WM_CLOSE:
			::DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			onDestroy(hwnd);
			break;

		case WM_PAINT:
			onPaint(hwnd);

		case WM_TIMER:
			onTimer(hwnd, wParam, lParam);
			break;

		default:
			return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

/* main function */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // find tibia window
	if (!Tibia.findTibia())
	{
		::MessageBox(0,
            "Tibia window not found!",
            "Error", MB_ICONERROR | MB_OK);
        return 0; // exit
	}

	// register window class
	WNDCLASSEX wc;
	wc.cbSize           = sizeof(WNDCLASSEX);
	wc.style            = 0;
	wc.lpfnWndProc      = windowProc; // window messages handler
	wc.cbClsExtra       = 0;
	wc.cbWndExtra       = 0;
	wc.hInstance        = hInstance;
	wc.hIcon            = 0;
	wc.hCursor          = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground    = static_cast<HBRUSH>(::CreateSolidBrush(RGB(236, 233, 216))); // window background color
	wc.lpszMenuName     = 0;
	wc.lpszClassName    = "levelspy";
	wc.hIconSm          = 0;

	if(!::RegisterClassEx(&wc))
    {
        ::MessageBox(0,
            "Register Window Class failed!",
            "Error", MB_ICONERROR | MB_OK);
        return 0; // exit
    }

	// create window
	HWND hwnd;
	hwnd = ::CreateWindowEx(
		WS_EX_DLGMODALFRAME, //WS_EX_WINDOWEDGE, // icon
		"levelspy", "Level Spy - Tibia 8.0", // title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window options
		CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_WIDTH, WINDOW_HEIGHT, // window dimensions
		0, 0, hInstance, 0);

	if(hwnd == 0)
	{
		::MessageBox(0,
			"Create Window failed!",
			"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	// center window
	HWND desktopWindow = ::GetDesktopWindow();
	RECT desktopRect;
	::GetWindowRect(desktopWindow, &desktopRect);

	RECT hwndRect;
	::GetWindowRect(hwnd, &hwndRect);

	hwndRect.left	= desktopRect.left + ((desktopRect.right - desktopRect.left) - (hwndRect.right - hwndRect.left)) / 2;
	hwndRect.top	= desktopRect.top + ((desktopRect.bottom - desktopRect.top) - (hwndRect.bottom - hwndRect.top)) / 2;

	::SetWindowPos(hwnd, 0, hwndRect.left, hwndRect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

	// show window
	::ShowWindow(hwnd, nCmdShow);
	::UpdateWindow(hwnd);

	// message loop
	MSG msg;
	while(::GetMessage(&msg, 0, 0, 0) > 0)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}
