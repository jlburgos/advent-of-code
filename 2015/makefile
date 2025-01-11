## Simple makefile used to compile my targets.

## Compiler settings
CXX=clang++
STD=-std=c++23

## Detect the OS if not windows
ifneq ($(OS),Windows_NT)
	OS:=$(shell uname -s)
endif

## Use aggressive optimizations by default.
## Example for debugging do: `DEBUG=true make dayX`
FLAGS=-Wall -Werror -pedantic $(STD) -O3
ifeq ($(DEBUG), true)
	FLAGS=-Wall -Werror -pedantic $(STD) -g -O0
	ifeq ($(OS), Darwin) # MacOS
	  FLAGS+=-fconcepts-diagnostics-depth=2
	endif
endif

## Additional compilation/linker flags
INCS=-I/usr/include -I../
LDFLAGS=-L/usr/lib -L../libs -laocutil
LD_LIBRARY_PATH:=../libs:$(LD_LIBRARY_PATH)

## If cryptography libs are needed.
## Example: `CRYPTO=true make dayX`
ifeq ($(CRYPTO),true)
	LDFLAGS+=-lssl -lcrypto
	OS:=$(shell uname)
	ifeq ($(OS), Darwin) # MacOS
		OPENSSL_PREFIX=$(shell brew --prefix openssl 2>/dev/null)
		ifneq ($(OPENSSL_PREFIX),)
			INCS+=-I$(OPENSSL_PREFIX)/include
			LDFLAGS+=-L$(OPENSSL_PREFIX)/lib
		endif
	endif
endif

## Show help.
help:
	@echo "Usage:"
	@echo "\tCompile Challenge:\tmake <source_file_without_cpp>"
	@echo "\tRun Tests:\t\tmake test"

## Since I'm adding the ".exe" extension, cleaning up is simple.
clean:
	@rm -f *.exe

../libs/libaocutil.so:
	$(MAKE) -C ../libs libaocutil.so

## Generic rule to handle cpp file targets.
## Example: `make dayX`
%: %.cpp ../libs/libaocutil.so
	@echo "----------------------------------------------"
	@echo "Compiling and attempting run of '$(@).exe' ..."
	@echo "----------------------------------------------"
	$(CXX) $(FLAGS) $(INCS) -o $(@).exe $(<) $(LDFLAGS) && LD_LIBRARY_PATH=../libs ./$(@).exe
