CC = clang

CFLAGS_COMMON = -I include  -I ./ -Wall -Wextra -Wno-int-conversion -Wno-unused-parameter
CFLAGS = $(CFLAGS_COMMON)

OBJ = buffer.o codegen.o endian.o error.o i.o instruction.o mi.o mode.o mr.o oi.o operand.o operand_cpp.o register.o rm.o
BUILD = ../build

all: libjas.a

libjas.a: $(OBJ) 
	ar rcs $(BUILD)/$@ $^ 

libjas_debug.a: CFLAGS = $(CFLAGS_COMMON) -g -std=c99
libjas_debug.a: $(OBJ)
	ar rcs $(BUILD)/$@ $^

# Individual object files rules:
operand_cpp.o: CFLAGS = $(CFLAGS_COMMON) -std=c++11 -g # Remove -g later
operand_cpp.o: operand.cpp
	$(CC) $(CFLAGS) -c $< -o $@	

.PHONY: all