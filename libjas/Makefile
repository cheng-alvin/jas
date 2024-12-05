CC = clang

CFLAGS_COMMON = -Wall -Wextra -Wno-unused-parameter -I include -I ../libjas/ -Wno-incompatible-pointer-types
CFLAGS = $(CFLAGS_COMMON)

C_SRC = $(wildcard *.c) # Includes only C source files
OBJ = $(patsubst %.c, %.o, $(C_SRC)) operand_cpp.o 
BUILD = ../build

libjas.a: $(OBJ) 
	ar rcs $(BUILD)/$@ $^ 

libjas_debug.a: CFLAGS = $(CFLAGS_COMMON) -g -std=c99
libjas_debug.a: $(OBJ)
	ar rcs $(BUILD)/$@ $^

# Individual object files rules:
operand_cpp.o: CFLAGS = $(CFLAGS_COMMON) -std=c++11
operand_cpp.o: operand.cpp
	$(CC) $(CFLAGS) -c $< -o $@	
