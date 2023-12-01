CC = clang
LD = ld
DB = lldb

# TODO Update makefile

CFLAGS_COMMON = -I include -Wall -Wextra 
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = endian.o error.o init.o parse.o

all: libjas.o

libjas.o: CFLAGS = $(CFLAGS_COMMON)
libjas.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@ 

# Please set `DYLD_LIBRARY_PATH` for dynamic library to work as well!

libjas.dylib: CFLAGS = $(CFLAGS_COMMON)
libjas.dylib: $(OBJ)
	$(CC) $^ -o $(BUILD)/$@ -dynamiclib