# It should be noted that the final build directory in which all
# resulting libraries are produced can be customized by setting the 
# `BUILD` property, which defaults to `../build/lib`

CC              = clang
CFLAGS_COMMON 	= -I include -I encoders -Wall -Werror -std=c11

SRC := $(wildcard *.c)
OBJ := $(patsubst %.c,$(BUILD)/%.o,$(SRC))

# `BUILD` should be provided by the command's caller:
BUILD ?= ../build/lib # And is defaulted where not provided.

$(BUILD)/libjas.a: CFLAGS = $(CFLAGS_COMMON) -O3
$(BUILD)/libjas.a: $(OBJ)
	ar rcs $@ $^

DEBUG_FLAGS := $(CFLAGS_COMMON) -O0 -gembed-source
$(BUILD)/libjas_debug.a: $(DEBUG_FLAGS)
$(BUILD)/libjas_debug.a: $(patsubst %.o, %.debug.o, $(OBJ))
	ar rcs $@ $^

instructions.inc: encoders/instructions.yml
	node ./encoders/compile.js $^ 
