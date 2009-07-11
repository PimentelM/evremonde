#ifndef TIBIA_H
#define TIBIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#include "addresses.h"
#include "constants.h"
#include "types.h"

#define DLL_EXPORT __declspec(dllexport)

DLL_EXPORT void tibiaShowVersion();

DLL_EXPORT HWND tibiaGetClientWindow();
DLL_EXPORT DWORD tibiaGetProcessId();
DLL_EXPORT HANDLE tibiaGetProcessHandle();

DLL_EXPORT void tibiaWriteBytes(DWORD dwAddress, int value, int bytes);
DLL_EXPORT int tibiaReadBytes(DWORD dwAddress, int bytes);

DLL_EXPORT void tibiaWriteString(DWORD dwAddress, char* text);
DLL_EXPORT char* tibiaReadString(DWORD dwAddress);

DLL_EXPORT void tibiaWriteNops(DWORD dwAddress, int nops);

DLL_EXPORT void tibiaSetStatusbarTimer(int duration);
DLL_EXPORT int tibiaGetStatusbarTimer();

DLL_EXPORT void tibiaSetStatusbarText(char* text);
DLL_EXPORT char* tibiaGetStatusbarText();

#endif // TIBIA_H

