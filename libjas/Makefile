CC = clang

CFLAGS_COMMON = -Wall -Wextra -Wno-int-conversion -Wno-unused-parameter
CFLAGS = $(CFLAGS_COMMON)

SRC = $(wildcard *.c) # Includes only C source files
OBJ = $(patsubst %.c, %.o, $(SRC)) operand_cpp.o 
BUILD = ../build

all: libjas.a

libjas.a: $(OBJ) 
	ar rcs $(BUILD)/$@ $^ 

libjas_debug.a: CFLAGS = $(CFLAGS_COMMON) -g -std=c99
libjas_debug.a: $(OBJ)
	ar rcs $(BUILD)/$@ $^

# Individual object files rules:
operand_cpp.o: CFLAGS = $(CFLAGS_COMMON) -std=c++11
operand_cpp.o: operand.cpp
	$(CC) $(CFLAGS) -c $< -o $@	

.PHONY: all