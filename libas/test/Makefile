CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../util/include -I include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../../build
HOME = ../../

OBJ = parse.o main.o

all: test

libas: CFLAGS = $(CFLAGS_COMMON)
libas: $(OBJ)
	$(CC) $^ -o $(BUILD)/test/$@ 