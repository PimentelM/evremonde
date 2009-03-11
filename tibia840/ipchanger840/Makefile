CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: ipchanger840

ipchanger840: ipchanger840.c
	$(CC) ipchanger840.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o ipchanger840.exe
