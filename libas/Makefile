CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../util/include -I include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = parse.o

all: libas.o

libas.o: CFLAGS = $(CFLAGS_COMMON)
libas.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@ 