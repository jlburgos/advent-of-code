#include <algorithm>
#include <array>
#include <chrono>
#include <bitset>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <variant>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

enum class Cmd {
  ON, OFF, TOGGLE
};

struct LightInstruction {
  Cmd cmd;
  std::array<std::array<U16, 2>, 2> coordinates;
  LightInstruction(const Cmd _cmd, const std::array<U16, 2> &coord1, const std::array<U16, 2> &coord2) {
    cmd = _cmd;
    coordinates[0] = coord1;
    coordinates[1] = coord2;
  }
  std::string_view toStringCmd() const;
};

// OLD Ops struct -- Uses simple std::function to store lambdas
template <class ROW>
struct Ops {
  using LAMBDA = std::function<void(ROW &row, const U16 column)>;
  const LAMBDA on_op;
  const LAMBDA off_op;
  const LAMBDA toggle_op;

  // Takes instructions and applies operations on 'lights' vector elements
  void apply(const std::vector<LightInstruction> &instructions, std::vector<ROW> &lights) const;
};

template <class LAMBDA, class Arg1, class Arg2>
concept Void2ArgsFunction = requires(LAMBDA lambda, Arg1 arg1, Arg2 arg2) {
  { lambda(arg1, arg2) } -> std::convertible_to<void>;
};

template <class LAMBDA, class ROW>
concept OpsConsumerRowRef = Void2ArgsFunction<LAMBDA, class ROW::value_type&, const U16>;

template <class LAMBDA, class ROW>
concept OpsConsumerRowPtr = Void2ArgsFunction<LAMBDA, class ROW::value_type*, const U16>;

// Updated Ops struct -- Using templates/concepts to take advantage of compiler optimizations
template <class ROW, class ON_OP, class OFF_OP, class TOGGLE_OP>
struct OpsV2 {
  const ON_OP on_op;
  const OFF_OP off_op;
  const TOGGLE_OP toggle_op;

  // Takes instructions and applies operations on 'lights' vector elements
  void applyLambda(const std::vector<LightInstruction> &instructions, std::vector<ROW> &lights) const
    requires OpsConsumerRowRef<ON_OP, std::vector<ROW>> && OpsConsumerRowRef<OFF_OP, std::vector<ROW>> && OpsConsumerRowRef<TOGGLE_OP, std::vector<ROW>>;
  void applyVisitor(const std::vector<LightInstruction> &instructions, std::vector<ROW> &lights) const
    requires OpsConsumerRowPtr<ON_OP, std::vector<ROW>> && OpsConsumerRowPtr<OFF_OP, std::vector<ROW>> && OpsConsumerRowPtr<TOGGLE_OP, std::vector<ROW>>;
};

template <class ROW, class ON_OP, class OFF_OP, class TOGGLE_OP>
requires OpsConsumerRowPtr<ON_OP, std::vector<ROW>> && OpsConsumerRowPtr<OFF_OP, std::vector<ROW>> && OpsConsumerRowPtr<TOGGLE_OP, std::vector<ROW>>
struct OpsV2Visitor {
  ROW *row;
  U16 column;
  void operator()(const ON_OP *op) { (*op)(row,column); }
  void operator()(const OFF_OP *op) { (*op)(row,column); }
  void operator()(const TOGGLE_OP *op) { (*op)(row,column); }
};

// These use the new std::function constructs
void part1(const std::vector<std::string> &input);
void part2(const std::vector<std::string> &input);
// These use the new template/concept stuff (w/ lambda)
void part1_V2(const std::vector<std::string> &input);
void part2_V2(const std::vector<std::string> &input);
// These use the new template/concept stuff (w/ visitor)
void part1_V3(const std::vector<std::string> &input);
void part2_V3(const std::vector<std::string> &input);

void printInstruction(const LightInstruction instruction);
std::array<U16, 2> parseCoordinates(const std::string_view str);
std::vector<LightInstruction> parseInput(const std::vector<std::string> &input);

int main() {
  const std::vector<std::string> input = Util::getMultiLineInput("input/day6.dat");
  // Running day6 solutions using std::function
  const auto start1 = std::chrono::high_resolution_clock::now();
  part1(input);
  part2(input);
  const auto end1 = std::chrono::high_resolution_clock::now();

  // Running day6 solutions using templates and concepts (w/ lambda)
  const auto start2 = std::chrono::high_resolution_clock::now();
  part1_V2(input);
  part2_V2(input);
  const auto end2 = std::chrono::high_resolution_clock::now();

  // Running day6 solutions using templates and concepts (w/ visitor)
  const auto start3 = std::chrono::high_resolution_clock::now();
  part1_V3(input);
  part2_V3(input);
  const auto end3 = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<F64, std::milli> elapsed1 = end1 - start1;
  const std::chrono::duration<F64, std::milli> elapsed2 = end2 - start2;
  const std::chrono::duration<F64, std::milli> elapsed3 = end3 - start3;

  std::cout << "Elapsed time (using std::function): " << elapsed1.count() << " ms" << std::endl;
  std::cout << "Elapsed time (using template/concepts w/ lambda): " << elapsed2.count() << " ms" << std::endl;
  std::cout << "Elapsed time (using template/concepts w/ visitor): " << elapsed3.count() << " ms" << std::endl;

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
  const std::size_t comma_loc = str.find(',');
  const std::string_view n1 = str.substr(0, comma_loc);
  const std::string_view n2 = str.substr(comma_loc + 1);
  return {
    static_cast<U16>(std::strtoul(n1.data(), nullptr, 10)),
    static_cast<U16>(std::strtoul(n2.data(), nullptr, 10))
  };
}

std::vector<LightInstruction> parseInput(const std::vector<std::string> &input) {
  constexpr std::array<std::string_view, 4> keywords = {"toggle", "turn on", "turn off", "through"};
  constexpr std::string_view digits = "0123456789";

  Cmd cmd;
  std::size_t first_pair, through, second_pair;
  std::string_view coord1, coord2;

  std::vector<LightInstruction> instructions;
  instructions.reserve(input.size());

  for (const std::string_view str : input) {
    if (std::string::npos != str.find(keywords[0])) {
      cmd = Cmd::TOGGLE;
    } else if (std::string::npos != str.find(keywords[1])) {
      cmd = Cmd::ON;
    } else if (std::string::npos != str.find(keywords[2])) {
      cmd = Cmd::OFF;
    } else {
      std::cerr << "Got some weird input: " << str << std::endl;
      std::exit(1);
    }

    first_pair = str.find_first_of(digits);
    through = str.find(keywords[3]);
    second_pair = str.find_first_of(digits, through);

    coord1 = str.substr(first_pair, through - first_pair);
    coord2 = str.substr(second_pair);

    instructions.emplace_back(cmd, parseCoordinates(coord1), parseCoordinates(coord2));

    // Some debugging to make sure I'm parsing inputs properly
    //printInstruction(instructions.back());
  }

  return instructions;
}

std::string_view LightInstruction::toStringCmd() const {
  switch(cmd) {
    case Cmd::ON: return "ON";
    case Cmd::OFF: return "OFF";
    case Cmd::TOGGLE: return "TOGGLE";
    default: {
      std::cout << "Unsupported cmd encountered!" << std::endl;
      std::exit(1);
    }
  }
}

template <class ROW>
void Ops<ROW>::apply(const std::vector<LightInstruction> &instructions, std::vector<ROW> &lights) const {
  const LAMBDA *op;

  for (const LightInstruction instruction : instructions) {
    // Set lambda operation depending on the parsed command
    switch(instruction.cmd) {
      case Cmd::OFF: {
        op = &off_op;
        break;
      }
      case Cmd::ON: {
        op = &on_op;
        break;
      }
      case Cmd::TOGGLE: {
        op = &toggle_op;
        break;
      }
      default: {
        std::cerr << "Need to implement support for cmd: " << instruction.toStringCmd() << std::endl;
        std::exit(1);
      }
    };

    // Since we're iterating through a vector, coordinates system is actually (y,x)
    // where "y" points to a row in the vector and "x" points to a column in the bitset.
    for (U16 y = instruction.coordinates[0][1]; y <= instruction.coordinates[1][1]; ++y) {
      for (U16 x = instruction.coordinates[0][0]; x <= instruction.coordinates[1][0]; ++x) {
        (*op)(lights[y], x);
      }
    }
  }
}

template <class ROW, class ON_OP, class OFF_OP, class TOGGLE_OP>
void OpsV2<ROW, ON_OP, OFF_OP, TOGGLE_OP>::applyLambda(const std::vector<LightInstruction> &instructions, std::vector<ROW> &lights) const
requires OpsConsumerRowRef<ON_OP, std::vector<ROW>> && OpsConsumerRowRef<OFF_OP, std::vector<ROW>> && OpsConsumerRowRef<TOGGLE_OP, std::vector<ROW>> {
  std::variant<const ON_OP*, const OFF_OP*, const TOGGLE_OP*> op;

  for (const LightInstruction instruction : instructions) {
    // Set lambda operation depending on the parsed command
    switch(instruction.cmd) {
      case Cmd::OFF: {
        op = &off_op;
        break;
      }
      case Cmd::ON: {
        op = &on_op;
        break;
      }
      case Cmd::TOGGLE: {
        op = &toggle_op;
        break;
      }
      default: {
        std::cerr << "Need to implement support for cmd: " << instruction.toStringCmd() << std::endl;
        std::exit(1);
      }
    };

    // Since we're iterating through a vector, coordinates system is actually (y,x)
    // where "y" points to a row in the vector and "x" points to a column in the bitset.
    for (U16 y = instruction.coordinates[0][1]; y <= instruction.coordinates[1][1]; ++y) {
      for (U16 x = instruction.coordinates[0][0]; x <= instruction.coordinates[1][0]; ++x) {
        std::visit([&](auto &&func) { (*func)(lights[y], x); }, op);
      }
    }
  }
}

template <class ROW, class ON_OP, class OFF_OP, class TOGGLE_OP>
void OpsV2<ROW, ON_OP, OFF_OP, TOGGLE_OP>::applyVisitor(const std::vector<LightInstruction> &instructions, std::vector<ROW> &lights) const
requires OpsConsumerRowPtr<ON_OP, std::vector<ROW>> && OpsConsumerRowPtr<OFF_OP, std::vector<ROW>> && OpsConsumerRowPtr<TOGGLE_OP, std::vector<ROW>> {
  OpsV2Visitor<ROW, ON_OP, OFF_OP, TOGGLE_OP> visitor;
  std::variant<const ON_OP*, const OFF_OP*, const TOGGLE_OP*> op;

  for (const LightInstruction instruction : instructions) {
    // Set lambda operation depending on the parsed command
    switch(instruction.cmd) {
      case Cmd::OFF: {
        op = &off_op;
        break;
      }
      case Cmd::ON: {
        op = &on_op;
        break;
      }
      case Cmd::TOGGLE: {
        op = &toggle_op;
        break;
      }
      default: {
        std::cerr << "Need to implement support for cmd: " << instruction.toStringCmd() << std::endl;
        std::exit(1);
      }
    };

    // Since we're iterating through a vector, coordinates system is actually (y,x)
    // where "y" points to a row in the vector and "x" points to a column in the bitset.
    for (U16 y = instruction.coordinates[0][1]; y <= instruction.coordinates[1][1]; ++y) {
      visitor.row = &lights[y];
      for (U16 x = instruction.coordinates[0][0]; x <= instruction.coordinates[1][0]; ++x) {
        visitor.column = x;
        std::visit(visitor, op);
      }
    }
  }
}

void part1(const std::vector<std::string> &input) {
  // Settings some configurations
  constexpr U16 num_columns = 1000;
  using ROW = std::bitset<num_columns>;

  const std::vector<LightInstruction> instructions = parseInput(input);
  std::vector<ROW> lights;
  lights.resize(num_columns); // initializes 1000 rows of columns with bit value '0'

  // Create lambdas that store the operations that need to be applied
  const Ops<ROW> ops = {
    [](ROW &row, const U16 column) { row.set(column); },
    [](ROW &row, const U16 column) { row.reset(column); },
    [](ROW &row, const U16 column) { row.flip(column); }
  };
  ops.apply(instructions, lights);

  const auto sumRow = [](std::size_t total, const ROW &row) -> std::size_t { return total + row.count(); };
  const std::size_t count = std::accumulate(lights.begin(), lights.end(), 0, sumRow);
  std::cout << "(Part 1) There are " << count << " lights that are lit." << std::endl;
}

void part2(const std::vector<std::string> &input) {
  // Settings some configurations
  constexpr U16 num_columns = 1000;
  using ROW = std::array<U16, num_columns>;

  const std::vector<LightInstruction> instructions = parseInput(input);
  std::vector<ROW> lights;
  lights.resize(num_columns); // initializes 1000 rows of columns with integral value '0'

  // Create lambdas that store the operations that need to be applied
  const Ops<ROW> ops = {
    [](ROW &row, const U16 column) { row[column] += 1; },
    [](ROW &row, const U16 column) { row[column] = (row[column] == 0 ? 0 : row[column] - 1); },
    [](ROW &row, const U16 column) { row[column] += 2; }
  };
  ops.apply(instructions, lights);

  const auto sumRow = [](std::size_t total, const ROW &row) -> std::size_t { return total + std::accumulate(row.begin(), row.end(), 0); };
  const std::size_t brightness = std::accumulate(lights.begin(), lights.end(), 0, sumRow);
  std::cout << "(Part 2) Total brightness of lit lights is " << brightness << std::endl;
}

void part1_V2(const std::vector<std::string> &input) {
  // Settings some configurations
  constexpr U16 num_columns = 1000;
  using ROW = std::bitset<num_columns>;

  const std::vector<LightInstruction> instructions = parseInput(input);
  std::vector<ROW> lights;
  lights.resize(num_columns); // initializes 1000 rows of columns with bit value '0'

  // Create lambdas that store the operations that need to be applied
  const auto on_op = [](ROW &row, const U16 column) { row.set(column); };
  const auto off_op = [](ROW &row, const U16 column) { row.reset(column); };
  const auto toggle_op = [](ROW &row, const U16 column) { row.flip(column); };
  const OpsV2<ROW, decltype(on_op), decltype(off_op), decltype(toggle_op)> ops = {on_op, off_op, toggle_op};
  ops.applyLambda(instructions, lights);

  const auto sumRow = [](std::size_t total, const ROW &row) -> std::size_t { return total + row.count(); };
  const std::size_t count = std::accumulate(lights.begin(), lights.end(), 0, sumRow);
  std::cout << "(Part 1 V2) There are " << count << " lights that are lit." << std::endl;
}

void part2_V2(const std::vector<std::string> &input) {
  // Settings some configurations
  constexpr U16 num_columns = 1000;
  using ROW = std::array<U16, num_columns>;

  const std::vector<LightInstruction> instructions = parseInput(input);
  std::vector<ROW> lights;
  lights.resize(num_columns); // initializes 1000 rows of columns with integral value '0'

  // Create lambdas that store the operations that need to be applied
  const auto on_op = [](ROW &row, const U16 column) { row[column] += 1; };
  const auto off_op = [](ROW &row, const U16 column) { row[column] = (row[column] == 0 ? 0 : row[column] - 1); };
  const auto toggle_op = [](ROW &row, const U16 column) { row[column] += 2; };
  const OpsV2<ROW, decltype(on_op), decltype(off_op), decltype(toggle_op)> ops = {on_op, off_op, toggle_op};
  ops.applyLambda(instructions, lights);

  const auto sumRow = [](std::size_t total, const ROW &row) -> std::size_t { return total + std::accumulate(row.begin(), row.end(), 0); };
  const std::size_t brightness = std::accumulate(lights.begin(), lights.end(), 0, sumRow);
  std::cout << "(Part 2 V2) Total brightness of lit lights is " << brightness << std::endl;
}

void part1_V3(const std::vector<std::string> &input) {
  // Settings some configurations
  constexpr U16 num_columns = 1000;
  using ROW = std::bitset<num_columns>;

  const std::vector<LightInstruction> instructions = parseInput(input);
  std::vector<ROW> lights;
  lights.resize(num_columns); // initializes 1000 rows of columns with bit value '0'

  // Create lambdas that store the operations that need to be applied
  const auto on_op = [](ROW *row, const U16 column) { row->set(column); };
  const auto off_op = [](ROW *row, const U16 column) { row->reset(column); };
  const auto toggle_op = [](ROW *row, const U16 column) { row->flip(column); };
  const OpsV2<ROW, decltype(on_op), decltype(off_op), decltype(toggle_op)> ops = {on_op, off_op, toggle_op};
  ops.applyVisitor(instructions, lights);

  const auto sumRow = [](std::size_t total, const ROW &row) -> std::size_t { return total + row.count(); };
  const std::size_t count = std::accumulate(lights.begin(), lights.end(), 0, sumRow);
  std::cout << "(Part 1 V2) There are " << count << " lights that are lit." << std::endl;
}

void part2_V3(const std::vector<std::string> &input) {
  // Settings some configurations
  constexpr U16 num_columns = 1000;
  using ROW = std::array<U16, num_columns>;

  const std::vector<LightInstruction> instructions = parseInput(input);
  std::vector<ROW> lights;
  lights.resize(num_columns); // initializes 1000 rows of columns with integral value '0'

  // Create lambdas that store the operations that need to be applied
  const auto on_op = [](ROW *row, const U16 column) { (*row)[column] += 1; };
  const auto off_op = [](ROW *row, const U16 column) { (*row)[column] = ((*row)[column] == 0 ? 0 : (*row)[column] - 1); };
  const auto toggle_op = [](ROW *row, const U16 column) { (*row)[column] += 2; };
  const OpsV2<ROW, decltype(on_op), decltype(off_op), decltype(toggle_op)> ops = {on_op, off_op, toggle_op};
  ops.applyVisitor(instructions, lights);

  const auto sumRow = [](std::size_t total, const ROW &row) -> std::size_t { return total + std::accumulate(row.begin(), row.end(), 0); };
  const std::size_t brightness = std::accumulate(lights.begin(), lights.end(), 0, sumRow);
  std::cout << "(Part 2 V2) Total brightness of lit lights is " << brightness << std::endl;
}