#include <cstdio>
#include <iostream>
#include <string>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

void part1(const std::vector<char> input);
void part2(const std::vector<char> input);

int main() {
  const std::vector<char> input = Util::getSingleLineInput("input/day1.dat");
  part1(input);
  part2(input);
  return 0;
}

void part1(const std::vector<char> input) {
  I32 floor = 0;
  for (U32 i = 0; i < input.size(); ++i) {
    input[i] == '(' ? ++floor : --floor;
  }
  std::cout << "Current Floor: " << floor << std::endl;
}

void part2(const std::vector<char> input) {
  I32 floor = 0;
  for (U32 i = 0; i < input.size(); ++i) {
    input[i] == '(' ? ++floor : --floor;
    if (floor < 0) {
      std::cout << "Character at position " << i + 1 << " caused Santa to enter the basement." << std::endl;
      break;
    }
  }
}
