REM http://www.mingw.org/
"c:\mingw\bin\windres.exe" "c:\code\fpslimiter831\fpslimiter831.rc" "c:\code\fpslimiter831\resource.o"
"c:\mingw\bin\gcc.exe" "c:\code\fpslimiter831\fpslimiter831.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\fpslimiter831\release\fpslimiter831.exe" "c:\code\fpslimiter831\resource.o"
DEL "c:\code\fpslimiter831\resource.o"
PAUSE
