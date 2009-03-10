CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: levelspy840

levelspy840: levelspy840.c resource.o
	$(CC) levelspy840.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o release/levelspy840.exe resource.o

resource.o: levelspy840.rc
	windres levelspy840.rc resource.o

clean:
	rm -f resource.o
