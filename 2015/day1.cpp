#include <cstdio>
#include <iostream>
#include <string>

#include "util.hpp"

void part1(const std::string_view input);
void part2(const std::string_view input);

int main() {
  const std::vector<char> input = Util::getSingleLineInput("day1.dat");
  const std::string args(input.begin(), input.end());
  part1(args);
  part2(args);
  return 0;
}

void part1(const std::string_view input) {
  int floor = 0;
  for (unsigned int i = 0; i < input.size(); ++i) {
    input[i] == '(' ? ++floor : --floor;
  }
  std::cout << "Current Floor: " << floor << std::endl;
}

void part2(const std::string_view input) {
  int floor = 0;
  for (unsigned int i = 0; i < input.size(); ++i) {
    input[i] == '(' ? ++floor : --floor;
    if (floor < 0) {
      std::cout << "Character at position " << i + 1 << " caused Santa to enter the basement." << std::endl;
      break;
    }
  }
}
