CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I $(HOME)/util/include -I ../include -Wall -Wextra -g
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../../build
HOME = ../..

OBJ = main.o

all: $(OBJ)
	$(MAKE) -C ../../ libs
	$(CC) $^  $(wildcard $(BUILD)/*.o) -o $(BUILD)/test/util $(CFLAGS) 
	@clear
	@$(BUILD)/test/util
