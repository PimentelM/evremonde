gcc --version
gcc sc2betaresources.c -Wl,-subsystem,console -Wall -O0 -s -lkernel32 -luser32 -o release/sc2betaresources.exe
PAUSE
