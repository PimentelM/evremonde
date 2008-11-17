/*
 * IP Changer for Tibia 8.31
 * by Evremonde
 * Last updated on 11/16/2008
 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include "resource.h"

/* memory addresses */

// rsa key
#define RSA_KEY 0x005AA610

// login server
#define LOGIN_SERVER_BEGIN	0x0077AD88
#define LOGIN_SERVER_END	0x0077B1E8 // = LOGIN_SERVER_BEGIN + (STEP_LOGIN_SERVER * MAX_LOGIN_SERVERS)

/* constants */

// rsa keys
static unsigned char RSA_KEY_TIBIA[]		= "124710459426827943004376449897985582167801707960697037164044904862948569380850421396904597686953877022394604239428185498284169068581802277612081027966724336319448537811441719076484340922854929273517308661370727105382899118999403808045846444647284499123164879035103627004668521005328367415259939915284902061793";
static unsigned char RSA_KEY_OPEN_TIBIA[]	= "109120132967399429278860960508995541528237502902798129123468757937266291492576446330739696001110603907230888610072655818825358503429057592827629436413108566029093628212635953836686562675849720620786279431090218017681061521755056710823876476444260558147179707119674283982419152118103759076030616683978566631413";

// rsa key size
const int RSA_KEY_SIZE = 309;

// distances to step between addresses in memory
const int STEP_LOGIN_SERVER = 112;

// max
const int MAX_LOGIN_SERVERS = 10;

// offsets to variables from login server step
const int OFFSET_LOGIN_SERVER_PORT = 100;

/* main function */

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		MessageBox(0,
			"Usage: ipchanger831.exe ip port\nExample: ipchanger831.exe 127.0.0.1 7171",
			"Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	// get game window
	HWND gameWindow = FindWindow("tibiaclient", 0);

	// get process id
	DWORD processId;
	GetWindowThreadProcessId(gameWindow, &processId);

	// get process handle
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

	// set memory to read write
	DWORD oldProtection;
	VirtualProtectEx(processHandle, (LPVOID)RSA_KEY, RSA_KEY_SIZE, PAGE_READWRITE, &oldProtection);

	// write rsa key
	WriteProcessMemory(processHandle, (LPVOID)RSA_KEY, RSA_KEY_OPEN_TIBIA, RSA_KEY_SIZE, 0);

	// restore memory protection
	DWORD newProtection;
	VirtualProtectEx(processHandle, (LPVOID)RSA_KEY, RSA_KEY_SIZE, oldProtection, &newProtection);

	// write to all login servers
	int i = 0;
	for(i = LOGIN_SERVER_BEGIN; i < LOGIN_SERVER_END; i += STEP_LOGIN_SERVER)
	{
		// write ip address
		char* ipAddress = argv[1];
		WriteProcessMemory(processHandle, (LPVOID)i, ipAddress, strlen(ipAddress) + 1, 0);

		// write port
		int port = atoi(argv[2]);
		WriteProcessMemory(processHandle, (LPVOID)i + OFFSET_LOGIN_SERVER_PORT, &port, 4, 0);
	}

	// close process handle
	CloseHandle(processHandle);

	return 0;
}
