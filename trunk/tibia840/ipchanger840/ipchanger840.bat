REM http://www.mingw.org/
"c:\mingw\bin\gcc.exe" "c:\code\ipchanger840\ipchanger840.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\ipchanger840\release\ipchanger840.exe"
PAUSE
