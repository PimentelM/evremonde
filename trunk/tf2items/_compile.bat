gcc --version
gcc cJSON.c tf2items.c -I"d:\code\curl\include" -L"d:\code\curl\lib" -lcurl -lcurldll -Wl,-subsystem,console -Wall -O0 -s -o release/tf2items.exe
gcc cJSON.c items_game.c -Wl,-subsystem,console -Wall -O0 -s -o release/items_game.exe
PAUSE
