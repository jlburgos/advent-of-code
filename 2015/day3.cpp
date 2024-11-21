#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <string>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

void part1(const std::vector<char> input);
void part2(const std::vector<char> input);

int main() {
  const std::vector<char> input = Util::getSingleLineInput("input/day3.dat");
  part1(input);
  part2(input);
  return 0;
}

void part1(const std::vector<char> input) {
  std::vector<std::array<I32, 2>> houses;
  houses.reserve(input.size());

  std::array<I32, 2> pos = {0,0};
  houses.push_back(pos); // initial house

  for (U32 i = 0; i < input.size(); ++i) {
    std::array<I32, 2> move;
    switch(input[i]) {
      case '^': {move = {0,1}; break;}
      case '>': {move = {1,0}; break;}
      case 'v': {move = {0,-1}; break;}
      case '<': {move = {-1,0}; break;}
    }
    pos[0] += move[0];
    pos[1] += move[1];
    houses.push_back(pos);
  }

  // sort vector, move duplicates to end of vector, remove duplicates
  std::sort(houses.begin(), houses.end());
  auto beginning_of_dupes = std::unique(houses.begin(), houses.end());
  houses.erase(beginning_of_dupes, houses.end());

  std::cout << "Number of visited houses: " << houses.size() << std::endl;
}

void part2(const std::vector<char> input) {
  std::vector<std::array<I32, 2>> houses;
  houses.reserve(input.size());

  std::array<I32, 2> santa = {0,0};
  std::array<I32, 2> robot = {0,0};
  houses.push_back(santa); // initial house
  houses.push_back(robot); // initial house

  bool move_santa = true;
  for (U32 i = 0; i < input.size(); ++i) {
    std::array<I32, 2> move;
    switch(input[i]) {
      case '^': {move = {0,1}; break;}
      case '>': {move = {1,0}; break;}
      case 'v': {move = {0,-1}; break;}
      case '<': {move = {-1,0}; break;}
    }

    if (move_santa) {
      santa[0] += move[0];
      santa[1] += move[1];
      houses.push_back(santa);
    } else {
      robot[0] += move[0];
      robot[1] += move[1];
      houses.push_back(robot);
    }
    move_santa = !move_santa;
  }

  // sort vector, move duplicates to end of vector, remove duplicates
  std::sort(houses.begin(), houses.end());
  auto beginning_of_dupes = std::unique(houses.begin(), houses.end());
  houses.erase(beginning_of_dupes, houses.end());

  std::cout << "Number of visited houses next year: " << houses.size() << std::endl;
}