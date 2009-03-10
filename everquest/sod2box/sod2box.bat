REM http://www.mingw.org/
windres sod2box.rc resource.o
gcc sod2box.c -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O0 -s -o sod2box.exe resource.o
DEL resource.o
PAUSE
