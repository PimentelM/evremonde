#ifndef TIBIA_H
#define TIBIA_H

#include <windows.h>

#include "addresses.h"
#include "constants.h"

#define DLL_EXPORT __declspec(dllexport)

DLL_EXPORT void tibiaMessageBox();

#endif // TIBIA_H

