gcc --version
gcc flvrenamer.c -Wl,-subsystem,console -Wall -O0 -s -lkernel32 -luser32 -lshlwapi -o release/flvrenamer.exe
PAUSE
