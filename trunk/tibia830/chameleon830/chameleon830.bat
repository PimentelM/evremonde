REM http://www.mingw.org/
"c:\mingw\bin\windres.exe" "c:\code\chameleon830\chameleon830.rc" "c:\code\chameleon830\resource.o"
"c:\mingw\bin\gcc.exe" "c:\code\chameleon830\chameleon830.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\chameleon830\release\chameleon830.exe" "c:\code\chameleon830\resource.o"
DEL "c:\code\chameleon830\resource.o"
PAUSE
