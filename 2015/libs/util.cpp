#include "util.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <fstream>

static bool isEmptyInput(const std::string_view input) {
  const std::array<char, 4> illegalChars = {' ', '\n', '\r', '\t'};
  return input.empty() || std::any_of(input.begin(), input.end(), [illegalChars](const char inputChar) {
      return std::any_of(illegalChars.begin(), illegalChars.end(), [inputChar](const char illegalCharsChar) { 
          return inputChar == illegalCharsChar;
          });
      });
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
    if (!isEmptyInput(std::string_view(&ch, 1))) {
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
    if (!isEmptyInput(line)) {
      input.push_back(line);
    }
  }
  closeStdinStream();
  return input;
}

#if UTIL_UNIT_TEST == true
int main() {
  std::cerr << "Running unit-tests ..." << std::endl;

  std::vector<char> line = Util::getSingleLineInput("libs/input/util.dat");
  std::string lineStr(line.begin(), line.end());
  std::cerr << "lineStr = " << lineStr << std::endl;
  assert(lineStr == "HelloWorld!!!Thereisatabhere");

  std::vector<std::string> lines = Util::getMultiLineInput("libs/input/util.dat");
  std::cerr << "Lines size: " << lines.size() << std::endl;
  for (unsigned int i = 0; i < lines.size(); ++i) {
    std::cerr << "Lines[" << i << "] = " << lines[i] << std::endl;
  }
  assert(lines.size() == 3);
  assert(lines[0] == "Hello");
  assert(lines[1] == "World");
  assert(lines[2] == "!!!");

  std::cout << "Successfully completed unit-test!" << std::endl;
  return 0;
}
#endif
