CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: sod2box

sod2box: sod2box.c resource.o
	$(CC) sod2box.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o sod2box.exe resource.o

resource.o: sod2box.rc
	windres sod2box.rc resource.o

clean:
	rm -f resource.o
