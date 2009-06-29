CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
I_XML =  -I../../iconv/include -I../../zlib/include -I../../libxml2/include
L_XML =  -L../../iconv/lib -L../../zlib/lib -L../../libxml2/lib
LIBS_XML = -liconv -lzlib -lxml2
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: looter842dll

looter842dll: looter842dll.c
	$(CC) looter842dll.c $(CFLAGS) $(LDFLAGS) $(I_XML) $(L_XML) $(LIBS_XML) $(LIBS) -shared -o release/looter842.dll