gcc tibia.c -Wl,-subsystem,windows -Wall -lkernel32 -luser32 -lgdi32 -lshell32 -shared -O0 -s -o release/bin/libtibia850.dll -Wl,--output-def,release/lib/libtibia850.def -Wl,--out-implib,release/lib/libtibia850.a
PAUSE
