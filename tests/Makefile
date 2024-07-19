CC = arch -x86_64 clang

CFLAGS_COMMON = -I ../libjas/include -std=c99 -lcriterion
CFLAGS = $(CFLAGS_COMMON)

TESTS = sample

all: $(TESTS)
	@for test in $(TESTS); do \
		./$$test; \
	done
	@rm -r $(TESTS)