/*
 * IP Changer for Tibia
 * by Evremonde
 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

// version
#define TIBIA_VERSION "8.42"

// tibia version information
#define TIBIA_FILE_PRODUCT_NAME    "Tibia Player"
#define TIBIA_FILE_PRODUCT_VERSION "8.4.2.0"

// file names
#define FILENAME "ipchanger.exe"

#define FILENAME_TIBIA_CLIENT "Tibia.exe"

/* memory addresses */

// rsa key
#define TIBIA_RSA_KEY 0x005AF610

// login servers
#define TIBIA_LOGIN_SERVERS_BEGIN 0x00785D30
#define TIBIA_LOGIN_SERVERS_END   0x00786190 // = TIBIA_LOGIN_SERVERS_BEGIN + (TIBIA_STEP_LOGIN_SERVER * TIBIA_MAX_LOGIN_SERVERS)

/* constants */

// rsa keys
//static unsigned char TIBIA_RSA_KEY_DEFAULT_SERVER[]  = "124710459426827943004376449897985582167801707960697037164044904862948569380850421396904597686953877022394604239428185498284169068581802277612081027966724336319448537811441719076484340922854929273517308661370727105382899118999403808045846444647284499123164879035103627004668521005328367415259939915284902061793";
static unsigned char TIBIA_RSA_KEY_OPEN_TIBIA_SERVER[] = "109120132967399429278860960508995541528237502902798129123468757937266291492576446330739696001110603907230888610072655818825358503429057592827629436413108566029093628212635953836686562675849720620786279431090218017681061521755056710823876476444260558147179707119674283982419152118103759076030616683978566631413";

// rsa key size
const int TIBIA_RSA_KEY_SIZE = 309;

// distances to step between addresses in memory
const int TIBIA_STEP_LOGIN_SERVER = 112;

// max
const int TIBIA_MAX_LOGIN_SERVERS = 10;

// offsets to variables from login server step
const int TIBIA_OFFSET_LOGIN_SERVER_PORT = 100;

/* functions */

/* main function */

int main(int argc, char *argv[])
{
    printf("IP Changer for Tibia " TIBIA_VERSION "\nby Evremonde\nCompile Date: " __DATE__ "\n\n");

    // check command line arguments
    if (argc != 3)
    {
        printf("Usage:   " FILENAME " ip port\nExample: " FILENAME " 127.0.0.1 7171\n");
        goto exit;
    }

    // check if tibia client file exists
    FILE *file = fopen(FILENAME_TIBIA_CLIENT, "r");
    if (file == NULL)
    {
        printf(FILENAME_TIBIA_CLIENT " not found!\n" FILENAME " file must be in Tibia folder!\n");
        fclose(file);
        goto exit;
    }
    fclose(file);

    // get tibia client version
    DWORD dwHandle;
    DWORD dwVersionInfoSize = GetFileVersionInfoSize(FILENAME_TIBIA_CLIENT, &dwHandle);
    if (dwVersionInfoSize > 0)
    {
        void *pVersionInfo = GlobalAlloc(GMEM_FIXED, dwVersionInfoSize);
        if (pVersionInfo)
        {
            if (GetFileVersionInfo(FILENAME_TIBIA_CLIENT, 0, dwVersionInfoSize, pVersionInfo))
            {
                VS_FIXEDFILEINFO *pFileInfo;

                void *pBuffer;
                UINT uBufferSize;

                if (VerQueryValue(pVersionInfo, "\\", &pBuffer, &uBufferSize))
                {
                    pFileInfo = (VS_FIXEDFILEINFO *)pBuffer;

                    char productVersion[128] = {0};
                    sprintf(productVersion, "%d.%d.%d.%d",
                        HIWORD(pFileInfo->dwProductVersionMS),
                        LOWORD(pFileInfo->dwProductVersionMS),
                        HIWORD(pFileInfo->dwProductVersionLS),
                        LOWORD(pFileInfo->dwProductVersionLS));

                    // check if tibia client product version corresponds with application
                    if (strcmp(TIBIA_FILE_PRODUCT_VERSION, productVersion) != 0)
                    {
                        printf("Sorry, Tibia client version \"%s\" is not valid!\nYou need Tibia client version \"" TIBIA_FILE_PRODUCT_VERSION "\" to use this software!\n", productVersion);
                        GlobalFree(pVersionInfo);
                        goto exit;
                    }

                    GlobalFree(pVersionInfo);
                }
            }
        }
    }

    // get tibia client window
    HWND hwndClientWindow = FindWindow("tibiaclient", NULL);
    if (hwndClientWindow == NULL)
    {
        printf("Tibia window not found!\nPlease open the Tibia client first!\n");
        goto exit;
    }

    // get process id
    DWORD dwProcessId;
    GetWindowThreadProcessId(hwndClientWindow, &dwProcessId);

    // get process handle
    HANDLE hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

    // set memory to read write
    DWORD dwOldProtection;
    VirtualProtectEx(hProcessHandle, (LPVOID)TIBIA_RSA_KEY, TIBIA_RSA_KEY_SIZE, PAGE_READWRITE, &dwOldProtection);

    // write rsa key
    WriteProcessMemory(hProcessHandle, (LPVOID)TIBIA_RSA_KEY, TIBIA_RSA_KEY_OPEN_TIBIA_SERVER, TIBIA_RSA_KEY_SIZE, NULL);

    // restore memory protection
    DWORD dwNewProtection;
    VirtualProtectEx(hProcessHandle, (LPVOID)TIBIA_RSA_KEY, TIBIA_RSA_KEY_SIZE, dwOldProtection, &dwNewProtection);

    // write to all login servers
    int i = 0;
    for(i = TIBIA_LOGIN_SERVERS_BEGIN; i < TIBIA_LOGIN_SERVERS_END; i += TIBIA_STEP_LOGIN_SERVER)
    {
        // read old ip address
        static char oldIpAddress[256];
        ReadProcessMemory(hProcessHandle, (LPVOID)i, &oldIpAddress, sizeof(oldIpAddress), NULL);

        // read old port
        int oldPort = 0;
        ReadProcessMemory(hProcessHandle, (LPVOID)i + TIBIA_OFFSET_LOGIN_SERVER_PORT, &oldPort, 4, NULL);

        // print old ip address and old port
        printf("Changing %s:%i to ", oldIpAddress, oldPort);

        // write ip address
        char* ipAddress = argv[1];
        WriteProcessMemory(hProcessHandle, (LPVOID)i, ipAddress, strlen(ipAddress) + 1, NULL);

        // write port
        int port = atoi(argv[2]);
        WriteProcessMemory(hProcessHandle, (LPVOID)i + TIBIA_OFFSET_LOGIN_SERVER_PORT, &port, 4, NULL);

        // read new ip address
        static char newIpAddress[256];
        ReadProcessMemory(hProcessHandle, (LPVOID)i, &newIpAddress, sizeof(newIpAddress), NULL);

        // read new port
        int newPort = 0;
        ReadProcessMemory(hProcessHandle, (LPVOID)i + TIBIA_OFFSET_LOGIN_SERVER_PORT, &newPort, 4, NULL);

        // print new ip address and new port
        printf("%s:%i\n", newIpAddress, newPort);
    }

    // close process handle
    CloseHandle(hProcessHandle);

// exit main
exit:
    printf("\nPress enter to exit...");

    // wait for key press
    getchar();

    return 0;
}
