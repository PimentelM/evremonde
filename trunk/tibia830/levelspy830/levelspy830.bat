REM http://www.mingw.org/
"c:\mingw\bin\windres.exe" "c:\code\levelspy830\levelspy830.rc" "c:\code\levelspy830\resource.o"
"c:\mingw\bin\gcc.exe" "c:\code\levelspy830\levelspy830.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\levelspy830\release\levelspy830.exe" "c:\code\levelspy830\resource.o"
DEL "c:\code\levelspy830\resource.o"
PAUSE
