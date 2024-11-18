## Simple makefile used to compile my targets.

## Show help.
help:
	@echo "Usage: make <source_file>.cpp"

## Generic rule to handle cpp file targets.
## Example: `make day1`
%: %.cpp
	g++ -Wall -Werror -pedantic -g -O0 $< -o $@.exe

## Since I'm adding the ".exe" extension, cleaning up is simple.
clean:
	@rm -f *.exe
