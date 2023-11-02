CC = clang
LD = clang
DB = gdb

CFLAGS = -I ../include

OBJ = main.o parse.o
BUILD = ../build

all: clean jas

jas: $(OBJ)
	$(LD) $^ -o $(BUILD)/$@ 


debug: $(OBJ)
	$(LD) $^ -o $(BUILD)/$@ $(CFLAGS) -g
	sudo $(DB) $(BUILD)/$@

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)