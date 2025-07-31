## Simple makefile used to compile my targets.

## Common Vars
CXX=clang++
STD=-std=c++23
CUSTOM_LIBS=../libs
INCS=-I../
LINKER_FLAGS=-L$(CUSTOM_LIBS) -laocutil

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

libaocutil.so:
	@echo "----------------------------------------------"
	@echo "Compiling shared library '$(@)' ..."
	@echo "----------------------------------------------"
ifeq ($(OS), Darwin) # MacOS
	$(CXX) $(FLAGS) *.cpp $(INCS) $(LIBS) -Wl,-install_name,../libs/$(@) -shared -fPIC -o $(@) $(LINKER_FLAGS)
else
	$(CXX) $(FLAGS) *.cpp $(INCS) $(LIBS) -shared -fPIC -o $(@) $(LINKER_FLAGS)
endif

## Since I'm adding the ".exe" extension, cleaning up is simple.
clean:
	@rm -f *.exe *.so

## Generic rule to handle cpp file targets.
## Example: `make dayX`
PHONY += %.cpp
%: %.cpp libaocutil.so
	@echo "----------------------------------------------"
	@echo "Compiling and attempting run of '$(@).exe' ..."
	@echo "----------------------------------------------"
	$(CXX) $(FLAGS) $(<) $(INCS) $(LIBS) -o $(@).exe $(LINKER_FLAGS) && LD_LIBRARY_PATH=$(CUSTOM_LIBS) DEBUG=$(DEBUG) ./$(@).exe && rm $(@).exe
