#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>

#include "util.hpp"

void part1(const std::string_view input);
void part2(const std::string_view input);

int main() {
  freopen("day1.dat", "r", stdin);
  const std::vector<char> input = Util::getSingleLineInput();
  const std::string args(input.begin(), input.end());
  part1(args);
  part2(args);
  return 0;
}

void part1(const std::string_view input) {
  int32_t floor = 0;
  for (uint32_t i = 0; i < input.size(); ++i) {
    input[i] == '(' ? ++floor : --floor;
  }
  std::cout << "Current Floor: " << floor << std::endl;
}

void part2(const std::string_view input) {
  int32_t floor = 0;
  for (uint32_t i = 0; i < input.size(); ++i) {
    input[i] == '(' ? ++floor : --floor;
    if (floor < 0) {
      std::cout << "Character at position " << i + 1 << " caused Santa to enter the basement." << std::endl;
      break;
    }
  }
}
