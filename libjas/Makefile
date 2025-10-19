CC = clang

CFLAGS_COMMON =-I include -Wno-incompatible-pointer-types -Wno-int-conversion -O3
CFLAGS = $(CFLAGS_COMMON)

C_SRC = $(wildcard *.c) # Includes only C source files
OBJ = $(patsubst %.c, %.o, $(C_SRC)) operand_cpp.o 
BUILD = ../build

libjas.a: $(OBJ) 
	ar rcs $(BUILD)/$@ $^ 

libjas_debug.a: CFLAGS = $(CFLAGS_COMMON) -g -std=c99 -O0 -gembed-source
libjas_debug.a: $(OBJ)
	ar rcs $(BUILD)/$@ $^

tabs.c: instructions.yml
	node ./scripts/compile.js $^ 

# Individual object files rules:
operand_cpp.o: CFLAGS = $(CFLAGS_COMMON) -std=c++11 -O0
operand_cpp.o: operand.cpp
	$(CC) $(CFLAGS) -c $< -o $@	
