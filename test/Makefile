CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I . -I ../util/include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = test.o info.o

all: labrador.o

labrador.o: CFLAGS = $(CFLAGS_COMMON)
labrador.o: ${OBJ}
	$(LD) -r $^ -o $(BUILD)/$@ 