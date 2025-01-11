#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

void part1(const std::vector<std::string> &input);
void part2(const std::vector<std::string> &input);

int main() {
  const std::vector<std::string> input = Util::getMultiLineInput("input/day2.dat");
  part1(input);
  part2(input);
  return 0;
}

void part1(const std::vector<std::string> &input) {
  U64 square_feet = 0;
  for (const std::string_view line : input) {
    const std::size_t pos1 = line.find('x', 0);
    const std::size_t pos2 = line.find('x', pos1+1);
    const std::size_t pos3 = line.find('x', pos2+1);

    const U64 v1 = std::stoul(line.substr(0, pos1).data(), nullptr, 10);
    const U64 v2 = std::stoul(line.substr(pos1+1, pos2).data(), nullptr, 10);
    const U64 v3 = std::stoul(line.substr(pos2+1, pos3).data(), nullptr, 10);

    const std::array<U64, 3> s3 = {v1*v2, v2*v3, v3*v1};
    const U64 surface_area = 2 * std::accumulate(s3.cbegin(), s3.cend(), 0);
    const U64 extra_side = *std::min_element(s3.cbegin(), s3.cend());

    square_feet += surface_area + extra_side;
  }
  std::cout << "Elves will need '" << square_feet << "' square feet of paper." << std::endl;
}

void part2(const std::vector<std::string> &input) {
  U64 feet = 0;
  for (const std::string_view line : input) {
    const std::size_t pos1 = line.find('x', 0);
    const std::size_t pos2 = line.find('x', pos1+1);
    const std::size_t pos3 = line.find('x', pos2+1);

    std::array<U64, 3> s3 = {
      std::stoul(line.substr(0, pos1).data(), nullptr, 10),
      std::stoul(line.substr(pos1 + 1, pos2).data(), nullptr, 10),
      std::stoul(line.substr(pos2 + 1, pos3).data(), nullptr, 10)
    };
    feet += s3[0]*s3[1]*s3[2]; // volume

    const U64* max_element = std::max_element(s3.cbegin(), s3.cend()); // find max element
    U64* min_element_ptr = std::min_element(s3.begin(), s3.end()); // find min element
    feet += 2 * (*min_element_ptr);

    *min_element_ptr += *max_element; // make original min element larger
    feet += 2 * (*std::min_element(s3.cbegin(), s3.cend())); // find new min element (originally second min element)
  }
  std::cout << "Elves will need '" << feet << "' feet of ribbon." << std::endl;
}
