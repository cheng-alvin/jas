CC = clang
LD = clang
DB = gdb

CFLAGS = -I ../include -g

OBJ = main.o parse.o endian.o write.o
BUILD = ../build

all: clean jas

jas: $(OBJ)
	$(LD) $^ -o $(BUILD)/$@ 

debug: clean jas $(OBJ)
	sudo $(DB) $(BUILD)/$@

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)