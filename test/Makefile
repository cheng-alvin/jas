CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../libjas/include -I ../libjas -I .. -Wall -Wextra -g
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

EXE = aaa.o aad.o aam.o aas.o adc.o add.o and.o arpl.o

all: libjas.o libjas

libjas.o:
	$(MAKE) -C ../libjas debug

libjas: CFLAGS = $(CFLAGS_COMMON)
libjas: $(EXE)
	@clear
	@for exe in $(EXE); do \
		$(CC) $$exe $(BUILD)/debug -o $(BUILD)/test/libjas/$${exe%.o}; \
		cd $(BUILD)/test/libjas/; \
		echo "\033[1mTesting \`$${exe%.o}\` ...\033[0m \c"; \
		./$${exe%.o}; \
		if [ $$? -eq 0  ]; then \
			echo "\033[1m\033[32mPASS\033[0m"; \
		else \
			echo "---"; \
			exit 1; \
		fi; \
		cd ../../../test; \
		done
			

