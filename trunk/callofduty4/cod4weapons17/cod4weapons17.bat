REM http://www.mingw.org/
"c:\mingw\bin\windres.exe" "c:\code\cod4weapons17\cod4weapons17.rc" "c:\code\cod4weapons17\resource.o"
"c:\mingw\bin\gcc.exe" "c:\code\cod4weapons17\cod4weapons17.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\cod4weapons17\release\cod4weapons17.exe" "c:\code\cod4weapons17\resource.o"
DEL "c:\code\cod4weapons17\resource.o"
PAUSE
