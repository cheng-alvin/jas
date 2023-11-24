CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = endian.o error.o init.o parse.o

all: libjas.o

libjas.o: CFLAGS = $(CFLAGS_COMMON)
libjas.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@ 