REM http://www.mingw.org/
"c:\mingw\bin\gcc.exe" "c:\code\ipchanger831\ipchanger831.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\ipchanger831\release\ipchanger831.exe"
PAUSE
