#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

void part1(const std::vector<std::string> input);
void part2(const std::vector<std::string> input);

int main() {
  const std::vector<std::string> input = Util::getMultiLineInput("input/day2.dat");
  part1(input);
  part2(input);
  return 0;
}

void part1(const std::vector<std::string> input) {
  U64 square_feet = 0;
  for (const std::string_view line : input) {
    std::array<U32, 3> s3 = {0, 0, 0};

    const std::size_t pos1 = line.find('x', 0);
    const std::size_t pos2 = line.find('x', pos1+1);
    const std::size_t pos3 = line.find('x', pos2+1);

    const U32 v1 = strtol(line.substr(0, pos1).data(), nullptr, 10);
    const U32 v2 = strtol(line.substr(pos1+1, pos2).data(), nullptr, 10);
    const U32 v3 = strtol(line.substr(pos2+1, pos3).data(), nullptr, 10);

    s3[0] = v1*v2;
    s3[1] = v2*v3;
    s3[2] = v3*v1;

    const U64 surface_area = 2 * std::accumulate(s3.begin(), s3.end(), 0);
    const U32* extra_side = std::min_element(s3.begin(), s3.end());

    square_feet += surface_area + *extra_side;
  }
  std::cout << "Elves will need '" << square_feet << "' square feet of paper." << std::endl;
}

void part2(std::vector<std::string> input) {
  U64 feet = 0;
  for (const std::string_view line : input) {
    std::array<U32, 3> s3 = {0, 0, 0};

    const std::size_t pos1 = line.find('x', 0);
    const std::size_t pos2 = line.find('x', pos1+1);
    const std::size_t pos3 = line.find('x', pos2+1);

    s3[0] = strtol(line.substr(0, pos1).data(), nullptr, 10);
    s3[1] = strtol(line.substr(pos1+1, pos2).data(), nullptr, 10);
    s3[2] = strtol(line.substr(pos2+1, pos3).data(), nullptr, 10);
    feet += s3[0]*s3[1]*s3[2]; // volume

    const U32* max_element = std::max_element(s3.begin(), s3.end()); // find max element
    U32* min_element_ptr = std::min_element(s3.begin(), s3.end()); // find min element
    feet += 2 * (*min_element_ptr);

    *min_element_ptr += *max_element; // make original min element large
    feet += 2 * (*std::min_element(s3.begin(), s3.end())); // find new min element (originally second min element)
  }
  std::cout << "Elves will need '" << feet << "' feet of ribbon." << std::endl;
}
