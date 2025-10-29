CC = clang

CFLAGS_COMMON =-I include -I . -Wno-incompatible-pointer-types -Wno-int-conversion -O3
CFLAGS = $(CFLAGS_COMMON)

C_SRC = $(wildcard *.c) # Includes only C source files
OBJ = $(patsubst %.c, %.o, $(C_SRC))
BUILD = ../build

libjas.a: $(OBJ) 
	ar rcs $(BUILD)/$@ $^ 

libjas_debug.a: CFLAGS = $(CFLAGS_COMMON) -g -std=c99 -O0 -gembed-source
libjas_debug.a: $(OBJ)
	ar rcs $(BUILD)/$@ $^

instructions.inc: instructions.yml
	node ./scripts/compile.js $^ 
	