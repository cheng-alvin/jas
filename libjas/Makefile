CC = gcc
LD = ld
DB = lldb

CFLAGS_COMMON = -I include  -I ./ -Wall -Wextra 
CFLAGS = $(CFLAGS_COMMON)

OBJ = buffer.o error.o instruction.o mode.o mr.o operand.o register.o operand_cpp.o
BUILD = ../build
HOME = ..

all: libjas.o

libjas.o: CFLAGS = $(CFLAGS_COMMON) 
libjas.o: $(OBJ) 
	$(LD) -r $^ -o $(BUILD)/$@ 

# Please set `DYLD_LIBRARY_PATH` for dynamic library to work as well!

libjas.dylib: CFLAGS = $(CFLAGS_COMMON) -Ofast
libjas.dylib: $(OBJ) 
	$(CC) $^ -o $(BUILD)/$@ -dynamiclib  

libjas_debug.o: CFLAGS = $(CFLAGS_COMMON) -g
libjas_debug.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/libjas_debug.o

# Individual object files rules:
operand_cpp.o: operand.cpp
	$(CC) $(CFLAGS) -c $< -o $(BUILD)/$@	
