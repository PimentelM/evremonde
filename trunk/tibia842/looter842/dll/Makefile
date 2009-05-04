CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: looter842dll

looter842dll: looter842dll.c
	$(CC) looter842dll.c $(CFLAGS) $(LDFLAGS) $(LIBS) -shared -o release/looter842.dll