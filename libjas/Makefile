CC = clang

CFLAGS_COMMON = -I include  -I ./ -Wall -Wextra -Wno-int-conversion -Wno-unused-parameter
CFLAGS = $(CFLAGS_COMMON)

OBJ = buffer.o codegen.o endian.o error.o i.o instruction.o mi.o mode.o mr.o oi.o operand.o operand_cpp.o register.o rm.o
BUILD = ../build

all: libjas.a libjas.dylb

libjas.a: $(OBJ) 
	ar rcs $(BUILD)/$@ $^ 

# Please set `DYLD_LIBRARY_PATH` for dynamic library to work as well!

libjas.dylib: CFLAGS = $(CFLAGS_COMMON) -Ofast -std=c99 -dynamiclib  
libjas.dylib: $(OBJ) 
	$(CC) $^ -o $(BUILD)/$@ 

libjas_debug.a: CFLAGS = $(CFLAGS_COMMON) -g -std=c99
libjas_debug.a: $(OBJ)
	ar rcs $(BUILD)/$@ $^

# Individual object files rules:
operand_cpp.o: CFLAGS = $(CFLAGS_COMMON) -std=c++11
operand_cpp.o: operand.cpp
	$(CC) $(CFLAGS) -c $< -o $@	
