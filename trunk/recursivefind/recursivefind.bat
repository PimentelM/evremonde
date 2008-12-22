REM http://www.mingw.org/
"c:\mingw\bin\g++.exe" "c:\code\recursivefind\recursivefind.cpp" -lkernel32 -luser32 -lgdi32 -lshell32 -lshlwapi -Wl,-subsystem,console -Wall -O2 -s -o "c:\code\recursivefind\release\recursivefind.exe"
PAUSE
