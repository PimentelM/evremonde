gcc --version
gcc cJSON.c tf2items.c -I"../curl/include" -L"../curl/lib" -I../jansson/include -L../jansson/lib -lcurl -lcurldll -ljansson -Wl,-subsystem,console -Wall -O0 -s -o release/tf2items.exe
pause