## Simple makefile used to compile my targets.
FLAGS=-Wall -Werror -pedantic -std=c++20 -g -O0

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
	g++ -DUNIT_TEST=true $(FLAGS) util.cpp -o util.exe && ./util.exe

## Generic rule to handle cpp file targets.
## Example: `make day1`
%: %.cpp test
	@echo "--------------------------------------------"
	@echo "Compiling '$(@).exe' ..."
	@echo "--------------------------------------------"
	g++ $(FLAGS) $(<) util.cpp -o $(@).exe

## Since I'm adding the ".exe" extension, cleaning up is simple.
clean:
	@rm -f *.exe
