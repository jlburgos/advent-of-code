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

## Additional compilation flags for includes and libs
INCS=-I/usr/include -I../
LIBS=-L/usr/lib

## Since I'm adding the ".exe" extension, cleaning up is simple.
clean:
	@rm -f *.exe *.so

libaocutil.so:
	@echo "----------------------------------------------"
	@echo "Compiling shared library '$(@)' ..."
	@echo "----------------------------------------------"
ifeq ($(OS), Darwin) # MacOS
	$(CXX) $(FLAGS) util.cpp $(INCS) $(LIBS) -Wl,-install_name,../libs/$(@) -shared -fPIC -o $(@) $(LIB_FLAGS)
else
	@echo "OS: $(OS)"
	$(CXX) $(FLAGS) util.cpp $(INCS) $(LIBS) -shared -fPIC -o $(@) $(LIB_FLAGS)
endif

## Generic rule to handle cpp file targets.
## Example: `make dayX`
%: %.cpp
	@echo "----------------------------------------------"
	@echo "Compiling and attempting run of '$(@).exe' ..."
	@echo "----------------------------------------------"
	$(CXX) -DUNIT_TEST=true $(FLAGS) $(<) $(INCS) $(LIBS) -o $(@).exe $(LIB_FLAGS) && ./$(@).exe && rm $(@).exe

