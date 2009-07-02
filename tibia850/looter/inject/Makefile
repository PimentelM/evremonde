CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32 -lversion

all: looterinject

looterinject: looterinject.c
	$(CC) looterinject.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o release/looter.exe
