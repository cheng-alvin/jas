CC = clang
LD = clang

OBJ = main.o
BUILD = ../build

all: jas

jas: $(OBJ)
	$(LD) $^ -o $(BUILD)/$@

clean:
	rm -r -f **/*.o $(BUILD)
	mkdir -p $(BUILD)