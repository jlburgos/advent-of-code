## Simple makefile used to compile my targets.

## Use aggressive optimizations by default.
## Example for debugging do: `DEBUG=true make dayX`
FLAGS=-Wall -Werror -pedantic -std=c++20 -O3
ifeq ($(DEBUG), true)
	FLAGS=-Wall -Werror -pedantic -std=c++20 -g -O0
endif

## Additional compilation flags for includes and libs
INCS=-I/usr/include -I../
LIBS=-L/usr/lib

## Since I'm adding the ".exe" extension, cleaning up is simple.
clean:
	@rm -f *.exe

## Generic rule to handle cpp file targets.
## Example: `make dayX`
%: %.cpp
	@echo "----------------------------------------------"
	@echo "Compiling and attempting run of '$(@).exe' ..."
	@echo "----------------------------------------------"
	g++ -DUNIT_TEST=true $(FLAGS) $(<) $(INCS) $(LIBS) -o $(@).exe $(LIB_FLAGS) && ./$(@).exe
