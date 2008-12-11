REM http://www.mingw.org/
"c:\mingw\bin\windres.exe" "c:\code\levelspy840\levelspy840.rc" "c:\code\levelspy840\resource.o"
"c:\mingw\bin\gcc.exe" "c:\code\levelspy840\levelspy840.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\levelspy840\release\levelspy840.exe" "c:\code\levelspy840\resource.o"
DEL "c:\code\levelspy840\resource.o"
PAUSE
