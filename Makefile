BUILD = build

all: 
	$(MAKE) -C libjas libjas.a
	cp libjas/include/*.h $(BUILD)/include
	mv $(BUILD)/libjas.a $(BUILD)/lib/libjas.a
	cp README.md $(BUILD)
	cp LICENSE $(BUILD)`

format:
	@find ./ -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format --verbose -i {} +

clean:
	@find . -name "*.o" -type f -delete
	@find . -name "*.a" -type f -delete
	@rm -r -f $(BUILD)
	@mkdir -p $(BUILD)/include
	@mkdir -p $(BUILD)/lib
	@mkdir -p $(BUILD)

tests: 
	$(MAKE) -C tests

libjas.deb: all
	mkdir -p $(BUILD)/usr/include
	mkdir -p $(BUILD)/usr/lib
	mv $(BUILD)/* $(BUILD)/usr 	
	chmod 644 $(BUILD/)DEBIAN/control
	dpkg-deb --build $(BUILD) $(BUILD)/libjas.deb

.PHONY: all tests format clean
