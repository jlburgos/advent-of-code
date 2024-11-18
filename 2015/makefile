## Simple makefile used to compile my targets.

FLAGS=-Wall -Werror -pedantic -std=c++20 -g -O0

## Show help.
help:
	@echo "Usage: make <source_file>.cpp"

## Generic rule to handle cpp file targets.
## Example: `make day1`
%: %.cpp
	g++ $(FLAGS) $(<) -o $(@).exe

## Since I'm adding the ".exe" extension, cleaning up is simple.
clean:
	@rm -f *.exe
