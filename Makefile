.PHONY: all build run clean
all: build
build:
	cmake -P build.cmake
run: build
	./build/dewox
clean:
	rm -rf build/

