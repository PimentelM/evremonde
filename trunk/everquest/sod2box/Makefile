COMPILER           = gcc
OS_TYPE            = -Wl,-subsystem,windows
WARNING_LEVEL      = -Wall
OPTIMIZATION_FLAGS = -O0
STRIP_SYMBOLS      = -s
WINDOWS_LIBRARIES  = -lcomctl32 -lcomdlg32 -lgdi32 -lkernel32 -lole32 -lshell32 -luser32

all: sod2box

sod2box: sod2box.c resource.o
	$(COMPILER) sod2box.c $(OS_TYPE) $(WARNING_LEVEL) $(OPTIMIZATION_FLAGS) $(STRIP_SYMBOLS) $(WINDOWS_LIBRARIES) \
	-o release/sod2box.exe resource.o

resource.o: sod2box.rc
	windres sod2box.rc resource.o

clean:
	rm -f resource.o
