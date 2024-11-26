#include <algorithm>
#include <array>
#include <bitset>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

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
  std::string_view toStringCmd() const {
    switch(cmd) {
      case Cmd::ON: return "ON";
      case Cmd::OFF: return "OFF";
      case Cmd::TOGGLE: return "TOGGLE";
      default: {
        std::cout << "Unsupported cmd encountered!" << std::endl;
        exit(1);
      }
    }
  }
};


void part1(const std::vector<std::string>& input);
void part2(const std::vector<std::string>& input);
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
  std::cout
  << instruction.toStringCmd()
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

    // Only valid commands are permitted
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
  // Settings some configurations
  const U16 num_columns = 1000;
  using ROW = std::bitset<num_columns>;
  using LAMBDA = std::function<void(ROW &row, const U16 column)>;

  const std::vector<LightInstruction> instructions = parseInput(input);
  std::vector<ROW> lights;
  lights.resize(num_columns); // initializes 1000 columns with bit value 'false'

  // Create lambdas that store the operations that need to be done
  const LAMBDA *op;
  const LAMBDA ON_OP     = [](ROW &row, const U16 column) { row.set(column); };
  const LAMBDA OFF_OP    = [](ROW &row, const U16 column) { row.reset(column); };
  const LAMBDA TOGGLE_OP = [](ROW &row, const U16 column) { row.flip(column); };

  // Collect coordinates in convenient arrays
  std::array<U16, 2> xrange;
  std::array<U16, 2> yrange;

  for (const LightInstruction instruction : instructions) {
    // Set lambda operation depending on the parsed command
    switch(instruction.cmd) {
      case Cmd::OFF: {
        op = &OFF_OP;
        break;
      }
      case Cmd::ON: {
        op = &ON_OP;
        break;
      }
      case Cmd::TOGGLE: {
        op = &TOGGLE_OP;
        break;
      }
      default: {
        std::cerr << "Need to implement support for cmd: " << instruction.toStringCmd() << std::endl;
        exit(1);
      }
    };

    // Since we're iterating through a vector of bitsets, coordinates system is actually (y,x)
    // where "y" points to a row in the vector and "x" points to a column in the bitset.
    xrange[0] = instruction.coordinates[0][0];
    xrange[1] = instruction.coordinates[1][0];
    yrange[0] = instruction.coordinates[0][1];
    yrange[1] = instruction.coordinates[1][1];
    for (U16 y = yrange[0]; y <= yrange[1]; ++y) {
      for (U16 x = xrange[0]; x <= xrange[1]; ++x) {
        (*op)(lights[y], x);
      }
    }
  }

  const std::size_t count = std::accumulate(lights.begin(), lights.end(), 0, [](std::size_t total, const ROW& row) -> std::size_t {
    return total + row.count();
  });

  std::cout << "(Part 1) There are " << count << " lights that are lit." << std::endl;
}

void part2(const std::vector<std::string>& input) {
  // TODO
}