REM http://www.mingw.org/
c:\mingw\bin\gcc.exe "c:\code\chameleon822\chameleon822.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\chameleon822\release\chameleon822.exe"
PAUSE