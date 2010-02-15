gcc --version
gcc cJSON.c cjson_helper.c tf2items.c -I"d:\code\curl\include" -L"d:\code\curl\lib" -lcurl -lcurldll -Wl,-subsystem,console -Wall -O0 -s -o release/tf2items.exe
gcc cJSON.c cjson_helper.c tf2items_found.c -I"d:\code\curl\include" -L"d:\code\curl\lib" -lcurl -lcurldll -Wl,-subsystem,console -Wall -O0 -s -o release/tf2items_found.exe
gcc cJSON.c cjson_helper.c items_game.c -Wl,-subsystem,console -Wall -O0 -s -o release/items_game.exe
pause