CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../util/include -I include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../../build
HOME = ../../

OBJ = parse.o main.o

all: test

as: CFLAGS = $(CFLAGS_COMMON)
as: $(OBJ)
	$(CC) $^ -o $(BUILD)/test/$@ 