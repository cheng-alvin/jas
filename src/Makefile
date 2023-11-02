CC = clang
LD = clang
DB = lldb

CFLAGS = -I ../include -g

OBJ = main.o parse.o endian.o write.o
BUILD = ../build

all: clean jas

jas: $(OBJ)
	$(LD) $^ -o $(BUILD)/$@ 

# TODO: fix this ugly hack!
debug: clean jas $(OBJ)
	 $(DB) $(BUILD)/jas

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)