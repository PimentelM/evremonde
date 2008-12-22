REM http://www.mingw.org/
"c:\mingw\bin\windres.exe" "c:\code\evremonde\everquest\sod2box\sod2box.rc" "c:\code\evremonde\everquest\sod2box\resource.o"
"c:\mingw\bin\gcc.exe" "c:\code\evremonde\everquest\sod2box\sod2box.c" -lkernel32 -luser32 -lgdi32 -lshell32 -Wl,-subsystem,windows -Wall -O2 -s -o "c:\code\evremonde\everquest\sod2box\release\sod2box.exe" "c:\code\evremonde\everquest\sod2box\resource.o"
DEL "c:\code\evremonde\everquest\sod2box\resource.o"
PAUSE
