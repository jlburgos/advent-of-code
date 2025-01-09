#include "util.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <fstream>

static bool isEmptyChar(const char ch) {
  const std::array<char, 4> illegalChars = {' ', '\n', '\r', '\t'};
  const auto contains_input = [ch](const char ch2) { return ch == ch2; };
  return std::any_of(illegalChars.cbegin(), illegalChars.cend(), contains_input);
}

static void reloadStdinStream(const std::string_view filename) {
  std::cin.clear();
  std::fseek(stdin, 0, SEEK_SET);
  assert(freopen(filename.data(), "r", stdin) != nullptr);
}

static void closeStdinStream() {
  assert(fclose(stdin) == 0);
}

std::vector<char> Util::getSingleLineInput(const std::string_view filename) {
  reloadStdinStream(filename);
  std::vector<char> input;
  char ch;
  while (std::cin.get(ch)) {
    if (!isEmptyChar(ch)) {
      input.push_back(ch);
    }
  }
  closeStdinStream();
  return input;
}

std::vector<std::string> Util::getMultiLineInput(const std::string_view filename) {
  reloadStdinStream(filename);
  std::vector<std::string> input;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (!line.empty()) {
      input.push_back(line);
    }
  }
  closeStdinStream();
  return input;
}

int Util::testUtil() { return 42; }

#if UNIT_TEST == true
#include "numeric-types.hpp"
int main() {
  std::cerr << "Running unit-tests ..." << std::endl;

  std::vector<char> line = Util::getSingleLineInput("input/util.dat");
  std::string lineStr(line.cbegin(), line.cend());
  std::cerr << "lineStr = " << lineStr << std::endl;
  assert(lineStr == "HelloWorld!!!Thereisatabhere");

  std::vector<std::string> lines = Util::getMultiLineInput("input/util.dat");
  std::cerr << "Lines size: " << lines.size() << std::endl;
  for (U8 i = 0; i < lines.size(); ++i) {
    std::cerr << "Lines[" << i << "] = " << lines[i] << std::endl;
  }
  assert(lines.size() == 5);
  assert(lines[0] == "Hello");
  assert(lines[1] == "World");
  assert(lines[2] == "!!!");
  assert(lines[3] == "	There is a tab here");
  assert(lines[4] == "        ");

  std::cout << Util::quoted(Util::testUtil()) << std::endl;

  std::cout << "Successfully completed unit-test!" << std::endl;
  return 0;
}
#endif
