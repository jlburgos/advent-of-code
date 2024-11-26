#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <string>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

void part1(const std::vector<std::string>& input);
void part2(const std::vector<std::string>& input);

bool hasAtLeastNumVowels(const std::string_view str, const U8 count);
bool hasPairs(const std::string_view str);
bool containsNoBadPairs(const std::string_view str);

bool containsPairsNotOverlapping(const std::string_view str);
bool containsPairWithInBetween(const std::string_view str);

int main() {
  const std::vector<std::string> input = Util::getMultiLineInput("input/day5.dat");
  part1(input);
  part2(input);
  return 0;
}

bool hasAtLeastNumVowels(const std::string_view str, const U8 count) {
  static const std::array<char, 5> vowels = {'a','e','i','o','u'};
  U8 num_vowels = 0;
  for (const char ch : str) {
    num_vowels += std::count_if(vowels.begin(), vowels.end(), [ch](const char ch2) -> bool {
      return ch == ch2;
    });
  }
  return num_vowels >= count;
}

bool hasPairs(const std::string_view str) {
  for (U32 i = 0; i < str.size() - 1; ++i) {
    const std::string_view substr = str.substr(i, 2);
    if (substr[0] == substr[1]) {
      return true;
    }
  }
  return false;
}

bool containsNoBadPairs(const std::string_view str) {
  static const std::array<std::string, 4> bad_pairs = {"ab","cd", "pq", "xy"};
  return std::all_of(bad_pairs.begin(), bad_pairs.end(), [str](const std::string_view bad_pair) {
    return str.find(bad_pair) == std::string::npos;
  });
}

bool containsPairsNotOverlapping(const std::string_view str) {
  for (U8 i = 0; i < str.size() - 1; ++i) {
    std::string_view p1 = str.substr(i,2);
    for (U8 j = i+2; j < str.size() - 1; ++j) {
      std::string_view p2 = str.substr(j,2);
      if (p1 == p2) {
        return true;
      }
    }
  }
  return false;
}

bool containsPairWithInBetween(const std::string_view str) {
  for (U8 i = 0; i < str.size() - 2; ++i) {
    if (str[i] == str[i+2]) {
      return true;
    }
  }
  return false;
}

void part1(const std::vector<std::string>& input) {
  U32 nice = 0;
  for (const std::string_view str : input) {
    if (hasAtLeastNumVowels(str, 3) && hasPairs(str) && containsNoBadPairs(str)) {
      ++nice;
    }
  }
  std::cout << "(Part 1) There are '" << nice << "' nice strings" << std::endl;
}

void part2(const std::vector<std::string>& input) {
  U32 nice = 0;
  for (const std::string_view str : input) {
    if (containsPairsNotOverlapping(str) && containsPairWithInBetween(str)) {
      ++nice;
    }
  }
  std::cout << "(Part 2) There are '" << nice << "' nice strings" << std::endl;
}