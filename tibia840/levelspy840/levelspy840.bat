REM http://www.mingw.org/
windres levelspy840.rc resource.o
gcc levelspy840.c -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o release/levelspy840.exe resource.o
DEL resource.o
PAUSE
