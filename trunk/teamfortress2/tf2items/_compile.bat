gcc --version
gcc cJSON.c cjson_helper.c tf2items.c -I"d:\code\curl\include" -L"d:\code\curl\lib" -lcurl -lcurldll -Wl,-subsystem,console -Wall -O0 -s -o release/tf2items.exe
pause