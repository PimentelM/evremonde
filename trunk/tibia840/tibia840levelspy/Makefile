CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32

all: tibia840levelspy

tibia840levelspy: tibia840levelspy.c resource.o
	$(CC) tibia840levelspy.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o release/tibia840levelspy.exe resource.o

resource.o: tibia840levelspy.rc
	windres tibia840levelspy.rc resource.o

clean:
	rm -f resource.o
