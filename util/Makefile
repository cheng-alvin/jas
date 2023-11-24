CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = endian.o init.o error.o write.o 

all: util.o 

util.o: CFLAGS = $(CFLAGS_COMMON)
util.o: $(OBJ) 
	$(LD) -r $^ -o $(BUILD)/$@ 
