CC = gcc
CFLAGS = -Wl,-subsystem,console -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32 -lversion

all: ipchanger

ipchanger: ipchanger.c
	$(CC) ipchanger.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o release/ipchanger.exe
