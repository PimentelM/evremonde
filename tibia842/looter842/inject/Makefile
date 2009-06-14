CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: looter842

looter842: looter842.c
	$(CC) looter842.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o release/looter842.exe
