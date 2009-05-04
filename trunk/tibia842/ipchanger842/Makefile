CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: ipchanger842

ipchanger842: ipchanger842.c
	$(CC) ipchanger842.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o release/ipchanger842.exe
