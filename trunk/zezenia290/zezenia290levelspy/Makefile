CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O0
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32 -lpsapi

all: zezenia290levelspy

zezenia290levelspy: zezenia290levelspy.c resource.o
	$(CC) zezenia290levelspy.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o release/zezenia290levelspy.exe resource.o

resource.o: zezenia290levelspy.rc
	windres zezenia290levelspy.rc resource.o

clean:
	rm -f resource.o
