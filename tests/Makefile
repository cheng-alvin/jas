CC = clang

CFLAGS_COMMON = -I ../libjas/include -lstdc++ -g
CFLAGS = $(CFLAGS_COMMON)

TESTS = buffer codegen endian error operand

tests: CFLAGS = $(CFLAGS_COMMON)
tests: $(TESTS)


$(TESTS): libjas_debug.a
$(TESTS): %: %.o 
	$(CC) -o $@ $< ../build/libjas_debug.a $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -rf $(TESTS)

.PHONY: all clean
