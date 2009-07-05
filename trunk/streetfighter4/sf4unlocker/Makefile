CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: sf4unlocker

sf4unlocker: sf4unlocker.c
	$(CC) sf4unlocker.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o release/sf4unlocker.exe
