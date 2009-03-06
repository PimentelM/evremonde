CC = gcc
CFLAGS = -Wl,-subsystem,windows -Wall -O2
LDFLAGS = -s
LIBS = -lkernel32 -luser32 -lgdi32 -lshell32
SRCS = levelspy840.c
RSRCS = levelspy840.rc
OBJS = resource.o
EXE = levelspy840.exe

all: levelspy840

levelspy840: $(OBJS)
	$(CC) $(SRCS) $(CFLAGS) $(LDFLAGS) $(LIBS) -o release/$(EXE) $(OBJS)

resource.o: $(RSRCS)
	windres $(RSRCS) $(OBJS)

clean:
	rm -f $(OBJS)
