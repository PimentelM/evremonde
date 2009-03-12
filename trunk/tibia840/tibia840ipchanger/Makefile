CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: tibia840ipchanger

tibia840ipchanger: tibia840ipchanger.c
	$(CC) tibia840ipchanger.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o tibia840ipchanger.exe
