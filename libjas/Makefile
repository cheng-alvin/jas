CC = clang
LD = ld
DB = lldb

# TODO Update makefile

CFLAGS_COMMON = -I include -Wall -Wextra 
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = aaa.o aad.o aam.o aas.o codegen.o endian.o error.o init.o instruction.o parse.o size.o write.o

all: libjas.o

libjas.o: CFLAGS = $(CFLAGS_COMMON)
libjas.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@ 

# Please set `DYLD_LIBRARY_PATH` for dynamic library to work as well!

libjas.dylib: CFLAGS = $(CFLAGS_COMMON)
libjas.dylib: $(OBJ)
	$(CC) $^ -o $(BUILD)/$@ -dynamiclib