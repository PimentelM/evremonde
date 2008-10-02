REM http://www.mingw.org/
"c:\mingw\bin\windres.exe" "c:\code\chameleon831\chameleon831.rc" "c:\code\chameleon831\resource.o"
"c:\mingw\bin\gcc.exe" "c:\code\chameleon831\chameleon831.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\chameleon831\release\chameleon831.exe" "c:\code\chameleon831\resource.o"
DEL "c:\code\chameleon831\resource.o"
PAUSE
