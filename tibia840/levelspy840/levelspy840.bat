REM http://www.mingw.org/
"c:\mingw\bin\windres.exe" "c:\code\evremonde\tibia840\levelspy840\levelspy840.rc" "c:\code\evremonde\tibia840\levelspy840\resource.o"
"c:\mingw\bin\gcc.exe" "c:\code\evremonde\tibia840\levelspy840\levelspy840.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\evremonde\tibia840\levelspy840\release\levelspy840.exe" "c:\code\evremonde\tibia840\levelspy840\resource.o"
DEL "c:\code\evremonde\tibia840\levelspy840\resource.o"
PAUSE
