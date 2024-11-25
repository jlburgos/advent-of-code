## Simple makefile used to compile my targets.

## Use aggressive optimizations by default.
## Example for debugging do: `DEBUG=true make dayX`
FLAGS=-Wall -Werror -pedantic -std=c++20 -O3
ifeq ($(DEBUG), true)
	FLAGS=-Wall -Werror -pedantic -std=c++20 -g -O0
endif

## If cryptography libs are needed.
## Example: `CRYPTO=true make dayX`
ifeq ($(CRYPTO), true)
	CRYPTO_FLAGS=-lssl -lcrypto
endif


## Show help.
help:
	@echo "Usage:"
	@echo "\tCompile Challenge:\tmake <source_file_without_cpp>"
	@echo "\tRun Tests:\t\tmake test"

## Run unit tests as needed!
test:
	@echo "--------------------------------------------"
	@echo "Compiling and running test files ..."
	@echo "--------------------------------------------"
	g++ -DUTIL_UNIT_TEST=true $(FLAGS) libs/util.cpp -o util.exe && ./util.exe

## Since I'm adding the ".exe" extension, cleaning up is simple.
clean:
	@rm -f *.exe

## Generic rule to handle cpp file targets.
## Example: `make dayX`
%: %.cpp
	@echo "----------------------------------------------"
	@echo "Compiling and attempting run of '$(@).exe' ..."
	@echo "----------------------------------------------"
	g++ $(FLAGS) $(<) libs/util.cpp -o $(@).exe $(CRYPTO_FLAGS) && ./$(@).exe
