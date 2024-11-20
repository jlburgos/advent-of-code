#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

#include "util.hpp"

void part1(std::vector<std::string> input);
void part2(std::vector<std::string> input);

int main() {
  freopen("day2.dat", "r", stdin);
  const std::vector<std::string> input = Util::getMultiLineInput();
  part1(input);
  return 0;
}

void part1(const std::vector<std::string> input) {
  const auto find_x = [](std::string_view s, std::size_t old_pos = 0) -> std::size_t {
    return s.find('x', old_pos);
  };

  long square_feet = 0L;

  for (const std::string_view line : input) {
    std::array<unsigned int, 3> s3 = {0, 0, 0};
    const std::size_t pos1 = find_x(line);
    const std::size_t pos2 = find_x(line, pos1 + 1);
    const std::size_t pos3 = find_x(line, pos2 + 1);
    const unsigned int v1 = strtol(line.substr(0, pos1).data(), nullptr, 10);
    const unsigned int v2 = strtol(line.substr(pos1+1, pos2).data(), nullptr, 10);
    const unsigned int v3 = strtol(line.substr(pos2+1, pos3).data(), nullptr, 10);
    s3[0] = v1*v2;
    s3[1] = v2*v3;
    s3[2] = v3*v1;
    const unsigned int surface_area = 2*std::accumulate(s3.begin(), s3.end(), 0);
    const unsigned int* extra_side = std::min_element(s3.begin(), s3.end());
    square_feet += surface_area + *extra_side;
  }

  std::cout << "Elves will need '" << square_feet << "' square feet of paper." << std::endl;
}

void part2(std::vector<std::string> input) {
  // TODO
}