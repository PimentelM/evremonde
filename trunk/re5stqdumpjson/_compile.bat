g++ --version
g++ re5stqdumpjson.cpp -Id:\code\jsoncpp\include -Ld:\code\jsoncpp\libs\mingw -ljson_mingw_libmt -Wl,--enable-auto-import -Wl,-subsystem,console -Wall -O0 -s -o release/re5stqdumpjson.exe
pause
