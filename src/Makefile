CC = clang
LD = clang
CFLAGS = -g

OBJ = main.o parse.o
BUILD = ../build

all: jas

jas: $(OBJ)
	$(LD) $^ -o $(BUILD)/$@ 

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)