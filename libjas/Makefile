CC = gcc
LD = ld
DB = lldb

CFLAGS_COMMON = -I include  -I ./ -Wall -Wextra -std=c99 
CFLAGS = $(CFLAGS_COMMON)

OBJ = buffer.o codegen.o error.o instruction.o mode.o mr.o operand.o register.o rm.o operand_cpp.o
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

# Individual object files rules:

operand_cpp.o: operand.cpp
	$(CC) $(CFLAGS) -c $< -o $(BUILD)/$@	
