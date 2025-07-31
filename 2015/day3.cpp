#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <sstream>

#include <unordered_map>
#include <unordered_set>

#include <libs/util.hpp>

void part1(const std::vector<char>& input);
void part2(const std::vector<char>& input);

// Alternative routes to the same answer
void part2_map(const std::vector<char>& input);
void part2_set(const std::vector<char>& input);

int main() {
  const std::vector<char> input = aoc::getSingleLineInput("input/day3.dat");
  part1(input);
  part2(input);
  part2_map(input);
  part2_set(input);
  return 0;
}

void part1(const std::vector<char>& input) {
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
      default: {
        std::cerr << "Ignoring unknown input '" << input[i] << "'" << std::endl;
        continue;
        }
    }
    pos[0] += move[0];
    pos[1] += move[1];
    houses.push_back(pos);
  }

  // sort vector, move duplicates to end of vector, remove duplicates
  std::sort(houses.begin(), houses.end());
  const auto beginning_of_dupes = std::unique(houses.begin(), houses.end());
  houses.erase(beginning_of_dupes, houses.end());

  std::cout << "Number of visited houses: " << houses.size() << std::endl;
}

void part2(const std::vector<char>& input) {
  std::vector<std::array<I32, 2>> houses;
  houses.reserve(input.size());

  std::array<I32, 2> santa = {0,0};
  std::array<I32, 2> robot = {0,0};
  houses.push_back(santa); // initial house

  bool move_santa = true;
  for (U32 i = 0; i < input.size(); ++i) {
    std::array<I32, 2> move;
    switch(input[i]) {
      case '^': {move = {0,1}; break;}
      case '>': {move = {1,0}; break;}
      case 'v': {move = {0,-1}; break;}
      case '<': {move = {-1,0}; break;}
      default: {
        std::cerr << "Ignoring unknown input '" << input[i] << "'" << std::endl;
        continue;
        }
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
  const auto beginning_of_dupes = std::unique(houses.begin(), houses.end());
  houses.erase(beginning_of_dupes, houses.end());

  std::cout << "Number of visited houses next year: " << houses.size() << std::endl;
}

void part2_map(const std::vector<char>& input) {
  std::unordered_map<std::string, bool> houses;
  houses.reserve(input.size());

  const auto to_str = [](std::array<I32, 2> pos) {
    std::stringstream ss;
    ss << pos[0] << "_" << pos[1];
    return ss.str();
  };

  std::array<I32, 2> santa = {0,0};
  std::array<I32, 2> robot = {0,0};
  houses.insert({to_str(santa), true}); // initial house

  bool move_santa = true;
  for (U32 i = 0; i < input.size(); ++i) {
    std::array<I32, 2> move;
    switch(input[i]) {
      case '^': {move = {0,1}; break;}
      case '>': {move = {1,0}; break;}
      case 'v': {move = {0,-1}; break;}
      case '<': {move = {-1,0}; break;}
      default: {
        std::cerr << "Ignoring unknown input '" << input[i] << "'" << std::endl;
        continue;
        }
    }

    if (move_santa) {
      santa[0] += move[0];
      santa[1] += move[1];
      houses.insert({to_str(santa), true});
    } else {
      robot[0] += move[0];
      robot[1] += move[1];
      houses.insert({to_str(robot), true});
    }
    move_santa = !move_santa;
  }

  std::cout << "Number of visited houses next year: " << houses.size() << std::endl;
}

void part2_set(const std::vector<char>& input) {
  std::unordered_set<std::string> houses;
  houses.reserve(input.size());

  const auto to_str = [](std::array<I32, 2> pos) {
    std::stringstream ss;
    ss << pos[0] << "_" << pos[1];
    return ss.str();
  };

  std::array<I32, 2> santa = {0,0};
  std::array<I32, 2> robot = {0,0};
  houses.insert(to_str(santa)); // initial house

  bool move_santa = true;
  for (U32 i = 0; i < input.size(); ++i) {
    std::array<I32, 2> move;
    switch(input[i]) {
      case '^': {move = {0,1}; break;}
      case '>': {move = {1,0}; break;}
      case 'v': {move = {0,-1}; break;}
      case '<': {move = {-1,0}; break;}
      default: {
        std::cerr << "Ignoring unknown input '" << input[i] << "'" << std::endl;
        continue;
        }
    }

    if (move_santa) {
      santa[0] += move[0];
      santa[1] += move[1];
      houses.insert(to_str(santa));
    } else {
      robot[0] += move[0];
      robot[1] += move[1];
      houses.insert(to_str(robot));
    }
    move_santa = !move_santa;
  }

  std::cout << "Number of visited houses next year: " << houses.size() << std::endl;
}
