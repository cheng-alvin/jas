CC = clang

CFLAGS_COMMON =-I include -I . -I encoders -Wall -Werror -std=c11

C_SRC = $(wildcard *.c) # Includes only C source files
OBJ = $(patsubst %.c, %.o, $(C_SRC))
BUILD = ../build

libjas.a: CFLAGS = $(CFLAGS_COMMON) -O3 
libjas.a: $(OBJ) 
	ar rcs $(BUILD)/$@ $^ 

libjas_debug.a: CFLAGS = $(CFLAGS_COMMON) -g -O0 -gembed-source
libjas_debug.a: $(OBJ)
	ar rcs $(BUILD)/$@ $^

instructions.inc: encoders/instructions.yml
	node ./encoders/compile.js $^ 
	
