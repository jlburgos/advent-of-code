## Simple makefile used to compile my targets.

## Common Vars
CXX=clang++
STD=-std=c++23
CUSTOM_LIBS=../libs
AOC_LIB=libaocutil.so
INCS=-I../
LINKER_FLAGS=-L$(CUSTOM_LIBS) -laocutil

## Detect the OS if not windows
ifneq ($(OS),Windows_NT)
	OS:=$(shell uname -s)
endif

## Use aggressive optimizations by default.
## Example for debugging do: `DEBUG=true make dayX`
OPT_FLAGS=-Wall -Werror -pedantic $(STD) -O3
ifeq ($(DEBUG), true)
	OPT_FLAGS=-Wall -Werror -pedantic $(STD) -g -O0
	ifeq ($(OS), Darwin) # MacOS
	  OPT_FLAGS+=-fconcepts-diagnostics-depth=2
	endif
endif


## If cryptography libs are needed.
## Example: `CRYPTO=true make dayX`
ifeq ($(CRYPTO),true)
	LINKER_FLAGS+=-lssl -lcrypto
	OS:=$(shell uname)
	ifeq ($(OS), Darwin) # MacOS
		OPENSSL_PREFIX=$(shell brew --prefix openssl 2>/dev/null)
		ifneq ($(OPENSSL_PREFIX),)
			INCS+=-I$(OPENSSL_PREFIX)/include
			LINKER_FLAGS+=-L$(OPENSSL_PREFIX)/lib
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
	$(MAKE) -C $(CUSTOM_LIBS) clean

$(CUSTOM_LIBS)/$(AOC_LIB):
	$(MAKE) -C $(CUSTOM_LIBS) $(AOC_LIB)

## Generic rule to handle cpp file targets.
## Example: `make dayX`
%: %.cpp $(CUSTOM_LIBS)/libaocutil.so
	@echo "----------------------------------------------"
	@echo "Compiling and attempting run of '$(@).exe' ..."
	@echo "----------------------------------------------"
	$(CXX) $(OPT_FLAGS) $(INCS) -o $(@).exe $(<) $(LINKER_FLAGS) && LD_LIBRARY_PATH=$(CUSTOM_LIBS) DEBUG=$(DEBUG) ./$(@).exe && rm $(@).exe
