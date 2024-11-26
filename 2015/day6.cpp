#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

void part1(const std::vector<std::string>& input);
void part2(const std::vector<std::string>& input);

enum class Cmd {
  ON, OFF, TOGGLE
};

struct LightInstruction {
  Cmd cmd;
  std::array<std::array<U16, 2>, 2> coordinates;
  LightInstruction(const Cmd _cmd,
                   const std::array<U16, 2> coord1,
                   const std::array<U16, 2> coord2)
                   {
                    cmd = _cmd;
                    coordinates[0] = coord1;
                    coordinates[1] = coord2;
                   }
};

void printInstruction(const LightInstruction instruction);
std::array<U16, 2> parseCoordinates(const std::string_view str);
std::vector<LightInstruction> parseInput(const std::vector<std::string>& input);

int main() {
  const std::vector<std::string> input = Util::getMultiLineInput("input/day6.dat");
  part1(input);
  part2(input);
  return 0;
}

void printInstruction(const LightInstruction instruction) {
  std::string cmd;
  switch(instruction.cmd) {
    case Cmd::OFF:    { cmd = "OFF"; break; }
    case Cmd::ON:     { cmd = "ON"; break; }
    case Cmd::TOGGLE: { cmd = "TOGGLE"; break; }
  }
  std::cout
  << cmd
  << ") <"
  << instruction.coordinates[0][0]
  << ","
  << instruction.coordinates[0][1]
  << "> :: <"
  << instruction.coordinates[1][0]
  << ","
  << instruction.coordinates[1][1]
  << ">"
  << std::endl;
}

std::array<U16, 2> parseCoordinates(const std::string_view str) {
  std::array<U16, 2> coordinates;
  const std::size_t comma_loc = str.find(',');
  std::string_view n1 = str.substr(0, comma_loc);
  std::string_view n2 = str.substr(comma_loc + 1);
  coordinates[0] = static_cast<U16>(strtoul(n1.data(), nullptr, 10));
  coordinates[1] = static_cast<U16>(strtoul(n2.data(), nullptr, 10));
  return coordinates;
}

std::vector<LightInstruction> parseInput(const std::vector<std::string>& input) {
  std::vector<LightInstruction> instructions;
  instructions.reserve(input.size());

  const std::string cmds[3] = {"toggle", "turn on", "turn off"};
  const std::string digits = "0123456789";

  for (const std::string_view str : input) {
    const std::size_t toggle_cmd_loc = str.find(cmds[0]);
    const std::size_t turn_on_cmd_loc = str.find(cmds[1]);
    const std::size_t turn_off_cmd_loc = str.find(cmds[2]);

    Cmd cmd;
    if (toggle_cmd_loc != std::string::npos) {
      cmd = Cmd::TOGGLE;
    } else if (turn_on_cmd_loc != std::string::npos) {
      cmd = Cmd::ON;
    } else if (turn_off_cmd_loc != std::string::npos) {
      cmd = Cmd::OFF;
    } else {
      std::cerr << "Got some weird input: " << str << std::endl;
      exit(1);
    }

    const std::size_t first_pair = str.find_first_of(digits);
    const std::size_t through = str.find("through");
    const std::size_t second_pair = str.find_first_of(digits, through);

    const std::string_view coord1 = str.substr(first_pair, through - first_pair);
    const std::string_view coord2 = str.substr(second_pair);

    instructions.emplace_back(cmd, parseCoordinates(coord1), parseCoordinates(coord2));

    // Some debugging to make sure I'm parsing inputs properly
    //printInstruction(instructions.back());
  }

  return instructions;
}

void part1(const std::vector<std::string>& input) {
  const std::vector<LightInstruction> instructions = parseInput(input);
  std::vector<std::bitset<1000>> lights;
  lights.resize(1000); // initializes 1000 rows with bit value 'false'

  for (const LightInstruction instruction : instructions) {
    const U16 xrange[2] = {instruction.coordinates[0][0], instruction.coordinates[1][0]};
    const U16 yrange[2] = {instruction.coordinates[0][1], instruction.coordinates[1][1]};
    for (U16 x = xrange[0]; x <= xrange[1]; ++x) {
      for (U16 y = yrange[0]; y <= yrange[1]; ++y) {
        switch(instruction.cmd) {
          case Cmd::OFF: {
            lights[y].reset(x);
            break;
          }
          case Cmd::ON: {
            lights[y].set(x);
            break;
          }
          case Cmd::TOGGLE: {
            lights[y].flip(x);
            break;
          }
        }
      }
    }
  }

  const std::size_t lit = std::accumulate(lights.begin(), lights.end(), 0, [](std::size_t total, const std::bitset<1000>& row) -> std::size_t {
    return total + row.count();
  });

  std::cout << "(Part 1) There are " << lit << " lights that are lit." << std::endl;
}

void part2(const std::vector<std::string>& input) {
  // TODO
}