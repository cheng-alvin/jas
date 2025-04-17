BUILD = build
CC = clang

CFLAGS_COMMON = -I libjas/include
CFLAGS = $(CFLAGS_COMMON)

.PHONY: all tests format clean debug pre_build

all: pre_build $(BUILD)/lib/libjas.a 
# all: pre_build $(BUILD)/lib/libjas.a $(BUILD)/jas debug
debug: clean $(BUILD)/lib/libjas_debug.a $(BUILD)/jas_debug 

format:
	@find ./ -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format --verbose -i {} +

clean:
	@find . -name "*.o" -type f -delete

pre_build:
	@find . -name "*.o" -type f -delete
	@find . -name "*.a" -type f -delete
	@rm -r -f $(BUILD)
	@mkdir -p $(BUILD)/include
	@mkdir -p $(BUILD)/lib
	@mkdir -p $(BUILD)
	@$(MAKE) -C tests clean
	@rm -rf libjas/tabs.c
	@cp libjas/include/*.h $(BUILD)/include
	@cp README.md $(BUILD)
	@cp LICENSE $(BUILD)
	@cp THANKS.txt $(BUILD)

tests: 
	$(MAKE) -C tests

$(BUILD)/lib/libjas.a:
	$(MAKE) -C libjas tabs.c
	$(MAKE) -C libjas libjas.a
	mv $(BUILD)/libjas.a $(BUILD)/lib/libjas.a

$(BUILD)/lib/libjas_debug.a:
	$(MAKE) -C libjas tabs.c
	$(MAKE) -C libjas libjas_debug.a
	mv $(BUILD)/libjas_debug.a $(BUILD)/lib/libjas_debug.a

$(BUILD)/jas: CFLAGS = $(CFLAGS_COMMON) -O3
$(BUILD)/jas: main.o $(BUILD)/lib/libjas.a
	$(CC) -o $@ $^ $(CFLAGS)

$(BUILD)/jas_debug: CFLAGS = $(CFLAGS_COMMON) -g -O0
$(BUILD)/jas_debug: main_debug.o $(BUILD)/lib/libjas_debug.a
	$(CC) -o $@ $^ $(CFLAGS)

main_debug.o: CFLAGS = $(CFLAGS_COMMON) -g -O0
main_debug.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@
