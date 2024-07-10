CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I include  -I ./ -Wall -Wextra -Wno-int-conversion -Wno-unused-parameter
CFLAGS = $(CFLAGS_COMMON)

OBJ = buffer.o codegen.o endian.o error.o i.o instruction.o mi.o mode.o mr.o oi.o operand.o operand_cpp.o register.o rex.o rm.o
BUILD = ../build
HOME = ..

all: libjas.o

libjas.o: CFLAGS = $(CFLAGS_COMMON) -Ofast -std=c99
libjas.o: $(OBJ) 
	$(LD) -r $^ -o $(BUILD)/$@ 

# Please set `DYLD_LIBRARY_PATH` for dynamic library to work as well!

libjas.dylib: CFLAGS = $(CFLAGS_COMMON) -Ofast -std=c99
libjas.dylib: $(OBJ) 
	$(CC) $^ -o $(BUILD)/$@ -dynamiclib  

libjas_debug.o: CFLAGS = $(CFLAGS_COMMON) -g -std=c99
libjas_debug.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/libjas_debug.o

# Individual object files rules:
operand_cpp.o: CFLAGS = $(CFLAGS_COMMON) -std=c++11
operand_cpp.o: operand.cpp
	$(CC) $(CFLAGS) -c $< -o $@	
