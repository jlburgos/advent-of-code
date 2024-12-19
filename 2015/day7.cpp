#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <type_traits>
#include <numeric>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

/////////////////////////////////////////////////////////////
// Set up a blueprint containing requirements and constraints
/////////////////////////////////////////////////////////////

template <typename WIRE>
concept WireConcept = requires(WIRE &wire) {
  { wire.set } -> std::same_as<bool&>;
  { wire.signal } -> std::same_as<U16&>;
  { wire.label  } -> std::same_as<const std::string&>;
};

template <typename T>
concept IterableConcept = requires(T &t) {
  typename T::value_type;
  { t.begin() } -> std::same_as<decltype(std::begin(std::declval<T&>()))>;
  { t.end()   } -> std::same_as<decltype(std::end(std::declval<T&>()))>;
  { t.size()  } -> std::same_as<std::size_t>;
};

template <typename WIRES>
concept WiresConcept = requires(WIRES &wires){
  typename WIRES::value_type;
  requires WireConcept<std::remove_pointer_t<typename WIRES::value_type>>;
  requires IterableConcept<WIRES>;
};

template <typename FUNCTION, typename INPUT>
concept SignalFunction = requires(const FUNCTION &func, const INPUT &input) {
  requires WireConcept<INPUT> || WiresConcept<INPUT>;
  { func(input) } -> std::same_as<U16>;
};


template <typename FUNCTION, typename INPUT>
concept ShiftFunction = requires(const FUNCTION &func, const INPUT &input, const U16 shift) {
  requires WireConcept<INPUT>;
  { func(input, shift) } -> std::same_as<U16>;
};

////////////////////////////////////////////////////////////////
// Build circuit data structures according to the blueprints ...
////////////////////////////////////////////////////////////////

struct Wire {
  bool set = false;
  U16 signal;
  const std::string label;
};
static_assert(WireConcept<Wire>, "Wire type must satisfy WireConcept");

//using Wires = std::vector<Wire*>; // TODO :: DOES NOT WORK!? // TODO :: ALSO, WIRESCONCEPTS CHECK PASSES SOMEHOW W/O *
using Wires = std::array<Wire*, 2>;
static_assert(WiresConcept<Wires>, "Wires type must satisfy WiresConcept");

enum class GateTypes
{
  AND,
  OR,
  NOT,
  LSHIFT,
  RSHIFT,
  PASSTHROUGH,
  GATE_COUNT
};

struct GateFunctions {
  static std::string_view toString(const GateTypes type) {
    switch(type) {
      case GateTypes::AND: return "AND";
      case GateTypes::OR: return "OR";
      case GateTypes::NOT: return "NOT";
      case GateTypes::LSHIFT: return "LSHIFT";
      case GateTypes::RSHIFT: return "RSHIFT";
      case GateTypes::PASSTHROUGH: return "PASSTHROUGH";
      default: return "UNKNOWN_GATE_TYPE"; // GATE_COUNT isn't a valid type!
    }
  }

  static constexpr auto AND = [](const WiresConcept auto &input) -> U16 {
    return std::accumulate(
        input.begin(),
        input.end(),
        static_cast<U16>(~0), // Note: All bits set to '1' for cumulative AND operation
        [](const U16 result, const Wire *wire) -> U16 { return result & wire->signal; });
  };

  static constexpr auto OR = [](const WiresConcept auto &input) -> U16 {
    return std::accumulate(
        input.begin(),
        input.end(),
        static_cast<U16>(0), // Note: All bits set to '0' for cumulative OR operation
        [](const U16 result, const Wire *wire) -> U16 {
          std::cerr << "Is wire null? " << (wire == nullptr ? "YES" : "NO") << std::endl;
           return result | wire->signal;
           });
  };

  static constexpr auto LSHIFT = [](const WireConcept auto &input, const U16 shift) -> U16 {
    U16 signal = input.signal;
    return signal << shift;
  };

  static constexpr auto RSHIFT = [](const WireConcept auto &input, const U16 shift) -> U16 {
    U16 signal = input.signal;
    return signal >> shift;
  };

  static constexpr auto NOT = [](const WireConcept auto &input)  -> U16 {
    return ~input.signal;
  };

  // Fake Gate. Useful in cases where input wire signal is also output wire signal.
  static constexpr auto PASS_THROUGH = [](const WireConcept auto &input) -> U16 {
    return input.signal;
  };
};

template <typename FUNCTION, typename INPUT>
requires SignalFunction<FUNCTION, INPUT> || ShiftFunction<FUNCTION, INPUT>
struct Gate {
  const FUNCTION &function;
  Wire &output;
  INPUT &input;

  Gate(const FUNCTION &f, Wire &out, INPUT &in) : function(f), output(out), input(in) {}

  virtual std::string_view type() const noexcept = 0;
  virtual U16 activate() const noexcept = 0;

  void check_input() {
    std::cerr << "Checking input ... " << std::endl;
    check_input(type(), input);
  }

  bool try_activate() noexcept {
    std::cerr << "try_activate() ..." << std::endl;
    if (output.set) {
      std::cerr << "Gate " << type() << " already processed." << std::endl;
      return true; 
    }
    else if (is_all_input_set(input)) {
      std::cerr << "All inputs set for " << type() << " gate." << std::endl;
      output.signal = activate();
      return (output.set = true);
    }
    else {
      std::cerr << "Gate " << type() << " is waiting for all inputs to be defined." << std::endl;
      return false;
    }
  }

private:
  static void check_input(const std::string_view type, const WireConcept auto &input) {
    std::cerr << "Gate: " << type << " w/ wire: " << input.label << std::endl;
  }
  static void check_input(const std::string_view type, const WiresConcept auto &input) {
    U64 i = 0;
    std::cerr << "About to loop through wires ... " << std::endl;
    for (const WireConcept auto *wire : input) {
      if (wire == nullptr) {
        std::cerr << "Gate: " << type << " w/ wires[" << i++ << "]: NULL" << std::endl;
      }
      else {
        std::cerr << "Gate: " << type << " w/ wires[" << i++ << "]: " << wire->label << std::endl;
      }
    }
  }

  static bool is_all_input_set(const WireConcept auto &input) noexcept {
    std::cerr << "is_all_input_set(WireConcept)" << std::endl;
    return input.set;
  }

  static bool is_all_input_set(const WiresConcept auto &input) noexcept {
    std::cerr << "is_all_input_set(WiresConcept)" << std::endl;
    return std::all_of(
        input.begin(),
        input.end(),
        [](const WireConcept auto *wire) -> bool { 
          if (wire == nullptr) { std::cerr << "Wire is NULL" << std::endl; return false; }
          return wire->set;
          });
  }
};

using ANDGateSpecialization = Gate<decltype(GateFunctions::AND), Wires>;
using ORGateSpecialization  = Gate<decltype(GateFunctions::OR),  Wires>;
using NOTGateSpecialization = Gate<decltype(GateFunctions::NOT), Wire>;
using LSHIFTGateSpecialization  = Gate<decltype(GateFunctions::LSHIFT),  Wire>;
using RSHIFTGateSpecialization  = Gate<decltype(GateFunctions::RSHIFT),  Wire>;
using PASSTHROUGHGateSpecialization = Gate<decltype(GateFunctions::PASS_THROUGH), Wire>;

struct ANDGate : public ANDGateSpecialization {
  ANDGate(Wire &out, Wires &in) : ANDGateSpecialization(GateFunctions::AND, out, in) {}
  virtual std::string_view type() const noexcept override {
    return GateFunctions::toString(GateTypes::AND);
  }
  virtual U16 activate() const noexcept override {
    return function(input);
  }
};

struct ORGate : public ORGateSpecialization {
  ORGate(Wire &out, Wires &in) : ORGateSpecialization(GateFunctions::OR, out, in) {}
  virtual std::string_view type() const noexcept override {
    return GateFunctions::toString(GateTypes::OR);
  }
  virtual U16 activate() const noexcept override {
    return function(input);
  }
};

struct NOTGate : public NOTGateSpecialization {
  NOTGate(Wire &out, Wire &in) : NOTGateSpecialization(GateFunctions::NOT, out, in) {}
  virtual std::string_view type() const noexcept override {
    return GateFunctions::toString(GateTypes::NOT);
  }
  virtual U16 activate() const noexcept override {
    return function(input);
  }
};

struct LSHIFTGate : public LSHIFTGateSpecialization {
  const U16 shift;
  LSHIFTGate(const U16 svalue, Wire &out, Wire &in) : LSHIFTGateSpecialization(GateFunctions::LSHIFT, out, in), shift(svalue) {}
  virtual std::string_view type() const noexcept override {
    return GateFunctions::toString(GateTypes::LSHIFT);
  }
  virtual U16 activate() const noexcept override {
    return function(input, shift);
  }
};

struct RSHIFTGate : public RSHIFTGateSpecialization {
  const U16 shift;
  RSHIFTGate(const U16 svalue, Wire &out, Wire &in) : RSHIFTGateSpecialization(GateFunctions::RSHIFT, out, in), shift(svalue) {}
  virtual std::string_view type() const noexcept override {
    return GateFunctions::toString(GateTypes::RSHIFT);
  }
  virtual U16 activate() const noexcept override {
    return function(input, shift);
  }
};

struct PASSTHROUGHGate : public PASSTHROUGHGateSpecialization {
  PASSTHROUGHGate(Wire &out, Wire &in) : PASSTHROUGHGateSpecialization(GateFunctions::PASS_THROUGH, out, in) {}
  virtual std::string_view type() const noexcept override {
    return GateFunctions::toString(GateTypes::PASSTHROUGH);
  }
  virtual U16 activate() const noexcept override {
    return function(input);
  }
};

/////////////////////////////////////////////////////////////
// Implement solution ...
/////////////////////////////////////////////////////////////

void parseCircuit(const std::vector<std::string> &input);

void part1(const std::vector<std::string> &input);
void part2(const std::vector<std::string> &input);

int main() {
  const std::vector<std::string> input = Util::getMultiLineInput("input/day7.dat");
  part1(input);
  //part2(input);
  return 0;
}

void part1(const std::vector<std::string> &input) {
  std::vector<std::variant<ANDGate, ORGate, LSHIFTGate, RSHIFTGate, NOTGate>> gates;
  std::unordered_map<std::string, Wire> wires;
  std::vector<Wires> wire_groupings; // Used as a cache

  // Reserve space to avoid re-allocation of underlying memory,
  // which invalidates memory addresses we need later on!
  gates.reserve(input.size());
  wires.reserve(input.size());
  wire_groupings.reserve(input.size());

  // Collect all the wire definitions
  for (const std::string_view line : input) {
    std::istringstream iss(line.data());
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
      tokens.push_back(token);
    }
    // Len = 3 // RVALUE (direct input signal) OR ANOTHER WIRE
    // Len = 4 // NOT GATE
    // Len = 5 // AND,OR,LSHIFT,RSHIFT GATE
    if (tokens.size() == 3) {
      // TODO :: NEED TO ADD SUPPORT FOR WIRE -> WIRE CONNECTION!!!
      // IDEA: Create a "WIREGate" that sets output to the input!
      const U16 signal = static_cast<U16>(std::strtol(tokens[0].data(), nullptr, 10));
      wires.erase(tokens[2]); // Force overwrite since these wires already have a signal!
      wires.emplace(tokens[2], Wire{true, signal, tokens[2]});
    }
    else if (tokens.size() == 4) {
      wires.emplace(tokens[1], Wire{false, 0b0, tokens[1]});
      wires.emplace(tokens[3], Wire{false, 0b0, tokens[3]});
    }
    else { // tokens.size() == 5
      wires.emplace(tokens[0], Wire{false, 0b0, tokens[0]});
      if (tokens[1] != "LSHIFT" && tokens[1] != "RSHIFT") { // No second wire!
        wires.emplace(tokens[2], Wire{false, 0b0, tokens[2]});
      }
      wires.emplace(tokens[4], Wire{false, 0b0, tokens[4]});
    }
  }

  std::cerr << "Size of wires: " << wires.size() << std::endl;
  for (const auto &[name, wire] : wires) {
    std::cerr << "Found wire \"" << name << "\" with signal: " << wire.signal << std::endl;
  }

  // Collect all the gates and assign the wires
  U64 directSignalCount = 0;
  for (const std::string_view line : input) {
    std::istringstream iss(line.data());
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
      tokens.push_back(token);
    }
    // Len = 4 // NOT GATE
    // Len = 5 // AND,OR,LSHIFT,RSHIFT GATE
    // 
    std::cout << "\nProcessing line: " << line << std::endl;
    if (tokens.size() == 4) {
      Wire &in = wires.at(tokens[1]);
      Wire &out = wires.at(tokens[3]);
      gates.emplace_back(NOTGate{out, in});
      std::cerr << "Creating NOT gate with input wire " << tokens[1] << std::endl;
    }
    else if (tokens.size() == 5) {
      Wire &out = wires.at(tokens[4]);
      const std::string_view type = tokens[1];
      if (type == "AND") {
        wire_groupings.emplace_back(Wires{&wires.at(tokens[0]), &wires.at(tokens[2])});
        gates.emplace_back(ANDGate(out, wire_groupings.back()));
        std::cerr << "Creating AND gate with input wires " << tokens[0] << " and " << tokens[2] << std::endl;
      }
      else if (type == "OR") {
        wire_groupings.emplace_back(Wires{&wires.at(tokens[0]), &wires.at(tokens[2])});
        gates.emplace_back(ORGate(out, wire_groupings.back()));
        std::cerr << "Creating OR gate with input wires " << tokens[0] << " and " << tokens[2] << std::endl;
      }
      else if (type == "LSHIFT") {
        Wire &in = wires.at(tokens[0]);
        const U16 shift = static_cast<U16>(std::strtol(tokens[2].data(), nullptr, 10));
        gates.emplace_back(LSHIFTGate(shift, out, in));
        std::cerr << "Creating LSHIFT gate with input wire " << tokens[0] << " and shift " << shift << std::endl;
      }
      else if (type == "RSHIFT") {
        Wire &in = wires.at(tokens[0]);
        const U16 shift = static_cast<U16>(std::strtol(tokens[2].data(), nullptr, 10));
        gates.emplace_back(RSHIFTGate(shift, out, in));
        std::cerr << "Creating LSHIFT gate with input wire " << tokens[0] << " and shift " << shift << std::endl;
      }
      else {
        std::cerr << "THIS CANNOT HAPPEN" << std::endl;
        std::exit(1);
      }
    }
    else {
      // TODO :: Need to clean up later!
      std::cerr << "DIRECT SIGNAL LINE: " << line << std::endl;
      ++directSignalCount;
      bool is_pass_through = std::any_of(tokens[0].begin(), tokens[0].end(), [](const char ch) -> bool { return !std::isalpha(ch); });
      if (is_pass_through) {
        Wire &in = wires.at(tokens[0]);
        Wire &out = wires.at(tokens[2]);
        gates.emplace_back(PASSTHROUGHGate(out, in));
      }
      else {
        // Skipping direct signal feed line!
      }
    }
  }

  std::cerr << "There are " << directSignalCount << " direct signal lines." << std::endl;
  std::cerr << "There are " << wires.size() << " wires." << std::endl;
  std::cerr << "There are " << gates.size() << " gates." << std::endl;

  std::vector<bool> status(gates.size());
  U64 old_count = 0;
  do {
    std::cerr << "Visiting ..." << std::endl;
    U64 i = 0;
    for (auto &gate : gates) {
      std::visit([&](auto &g) {
        std::cerr << "\n\nRunning gate: " << g.type() << std::endl;
        g.check_input();
        std::cerr << "----------------------" << std::endl;
        status[i++] = g.try_activate();
      }, gate);
    }

    const U64 count = std::count_if(status.begin(), status.end(), [](const bool flag) -> bool { return flag;});
    std::cerr << "Number of gates passing the check: " << count << std::endl;
    if (old_count == count) {
      std::cerr << "Invalid circuit detected!" << std::endl;
      std::exit(1);
    }
    old_count = count;
    std::cerr << "\n\n";
  } while (!std::all_of(status.begin(), status.end(), [](const bool flag) { return flag;}));

  std::vector<std::string_view> keys;
  keys.reserve(wires.size());
  for (const auto &[key,wire] : wires) {
    keys.push_back(key);
  }
  std::sort(keys.begin(), keys.end());
  for (const std::string_view key : keys) {
    std::cout << key << ": " << wires[std::string(key)].signal << std::endl;
  }

/*
  Wire in {true, 0b0000000000000111, "a"};
  Wire out{false, 0b0, "b"};
  Wire out2{false, 0b0, "c"};
  NOTGate gate1(in, out);
  NOTGate gate2(out, out2);

  std::cerr << "Out wire output1: " << std::bitset<16>(gate1.output.signal) << std::endl;
  std::cerr << "Out wire output2: " << std::bitset<16>(gate2.output.signal) << std::endl;
  gate1.try_activate();
  std::cerr << "Out wire output1: " << std::bitset<16>(gate1.output.signal) << std::endl;
  std::cerr << "Out wire output2: " << std::bitset<16>(gate2.output.signal) << std::endl;
  gate2.try_activate();
  std::cerr << "Out wire output1: " << std::bitset<16>(gate1.output.signal) << std::endl;
  std::cerr << "Out wire output2: " << std::bitset<16>(gate2.output.signal) << std::endl;

  Wires in1_2 {{{true, 0b0000000000000111, "a"}, {true, 0b0000000000000101, "b"}}};
  Wire out1_2 {false, 0b0, "c"};
  ANDGate gate3{in1_2, out1_2};
  std::cerr << "Out wire output3: " << std::bitset<16>(gate3.output.signal) << std::endl;
  gate3.try_activate();
  std::cerr << "Out wire output3: " << std::bitset<16>(gate3.output.signal) << std::endl;
*/
}

void part2(const std::vector<std::string> &input) {
}

 void parseCircuit(const std::vector<std::string> &input) {
  std::vector<Wire> wires;

  //using GateOpVariant = std::variant<Gate<decltype(Gate::AND)>, Gate<decltype(Gate::OR)>, Gate<decltype(NOT)>, Gate<decltype(LSHIFT), Gate<decltype(RSHIFT)>>;
  //std::vector<GateOpVariant>;

  //using GateVariant = std::variant<Gate<U16 (*)(const std::vector<Wire> &), std::vector<Wire>, Wire>, Gate<U16 (*)(const Wire &), Wire, Wire>>;

  //Wire w1{'a', 0b0011},w2{'b', 0b0000};
  //std::vector<GateVariant> gate_map;
  //gate_map.emplace_back(GateFunctions::AND2, &w1, &w2);
  //GateVariant var = gate_map[0];
  //Gate<U16(*)(const Wire&), Wire, Wire> g1 = std::get<Gate<U16(*)(const Wire&), Wire, Wire>>(var);
  //g1.try_activate();
  //std::cerr << "Gate output: " << g1.out->signal << std::endl;
}
