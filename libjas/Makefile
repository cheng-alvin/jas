CC = clang

CFLAGS_COMMON = -I include  -I ./ -Wall -Wextra -Wno-int-conversion -Wno-unused-parameter
CFLAGS = $(CFLAGS_COMMON)

SRC = $(wildcard *.c) # Includes only C source files
OBJ = $(patsubst %.c, %.o, $(SRC)) operand_cpp.o # Individual C++ rule, add if required

BUILD = ../build

all: libjas.a

libjas.a: $(OBJ) 
	echo $(OBJ)
	ar rcs $(BUILD)/$@ $^ 

libjas_debug.a: CFLAGS = $(CFLAGS_COMMON) -g -std=c99
libjas_debug.a: $(OBJ)
	ar rcs $(BUILD)/$@ $^

# Individual object files rules:
operand_cpp.o: CFLAGS = $(CFLAGS_COMMON) -std=c++11 -g # Remove -g later
operand_cpp.o: operand.cpp
	$(CC) $(CFLAGS) -c $< -o $@	

.PHONY: all
