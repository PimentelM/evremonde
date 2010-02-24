gcc --version
gcc recursivefind.c -Wl,-subsystem,console -Wall -O0 -s -lkernel32 -luser32 -lgdi32 -lshell32 -lshlwapi -o release/recursivefind.exe
pause