CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: dll

dll: cJSON.c dll.c
	$(CC) cJSON.c dll.c $(CFLAGS) $(LDFLAGS) $(LIBS) -shared -o release/looter.dll