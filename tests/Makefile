CC = arch -x86_64 clang

CFLAGS_COMMON = -I ../libjas/include -lcriterion -lstdc++
CFLAGS = $(CFLAGS_COMMON)

TESTS = buffer endian error operand

all: CFLAGS = $(CFLAGS_COMMON)
all: $(TESTS)
	@for test in $(TESTS); do \
		./$$test; \
	done
	@rm -r $(TESTS)

$(TESTS): libjas.o
$(TESTS): %: %.o 
	$(CC) -o $@ $< ../build/libjas.o $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

libjas.o: 
	$(MAKE) -C ../libjas libjas.o