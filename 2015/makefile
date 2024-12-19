## Simple makefile used to compile my targets.

## Use aggressive optimizations by default.
## Example for debugging do: `DEBUG=true make dayX`
FLAGS=-Wall -Werror -pedantic -std=c++20 -O3
ifeq ($(DEBUG), true)
	FLAGS=-Wall -Werror -pedantic -std=c++20 -g -O0 -fconcepts-diagnostics-depth=2
endif

## Additional compilation flags for includes and libs
INCS=-I/usr/include -I../
LIBS=-L/usr/lib

OS:=$(shell uname)
ifeq ($(OS), Darwin)  # macOS
	OPENSSL_PREFIX = $(shell brew --prefix openssl 2>/dev/null)
	ifneq ($(OPENSSL_PREFIX),)
		INCS+=-I$(OPENSSL_PREFIX)/include
		LIBS+=-L$(OPENSSL_PREFIX)/lib
	endif
endif

## Linker stuff
LIB_FLAGS=
## If cryptography libs are needed.
## Example: `CRYPTO=true make dayX`
ifeq ($(CRYPTO), true)
	LIB_FLAGS+=-lssl -lcrypto
endif

## Show help.
help:
	@echo "Usage:"
	@echo "\tCompile Challenge:\tmake <source_file_without_cpp>"
	@echo "\tRun Tests:\t\tmake test"

## Since I'm adding the ".exe" extension, cleaning up is simple.
clean:
	@rm -f *.exe

## Generic rule to handle cpp file targets.
## Example: `make dayX`
%: %.cpp
	@echo "----------------------------------------------"
	@echo "Compiling and attempting run of '$(@).exe' ..."
	@echo "----------------------------------------------"
	g++ $(FLAGS) $(<) ../libs/util.cpp $(INCS) $(LIBS) -o $(@).exe $(LIB_FLAGS) && ./$(@).exe
