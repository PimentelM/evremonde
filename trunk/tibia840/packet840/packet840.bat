REM http://www.masm32.com/
"c:\masm32\bin\ml.exe" /c /coff /Cp /nologo /I"c:\masm32\include" "c:\code\evremonde\tibia840\packet840\packet840.asm"
"c:\masm32\bin\link.exe" /SUBSYSTEM:WINDOWS /RELEASE /VERSION:4.0 "/LIBPATH:c:\masm32\lib" /DLL /ENTRY:DllEntry /EXPORT:SendPacket "c:\code\evremonde\tibia840\packet840\packet840.obj" "/OUT:c:\code\evremonde\tibia840\packet840\release\packet840.dll"
DEL "c:\code\evremonde\tibia840\packet840\packet840.obj"
PAUSE
