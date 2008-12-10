REM http://www.mingw.org/
"c:\mingw\bin\windres.exe" "c:\code\levelspy831\levelspy831.rc" "c:\code\levelspy831\resource.o"
"c:\mingw\bin\gcc.exe" "c:\code\levelspy831\levelspy831.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\levelspy831\release\levelspy831.exe" "c:\code\levelspy831\resource.o"
DEL "c:\code\levelspy831\resource.o"
PAUSE
