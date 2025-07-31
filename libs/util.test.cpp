#include "util.hpp"

static void test_func(const std::string_view str) {
  std::cout << "[FUNCTION] " << str << std::endl;
}

int main() {
  std::cerr << "Running unit-tests ..." << std::endl;

  std::vector<char> line = aoc::getLineInput<char>("input/util.dat");
  std::string lineStr(line.cbegin(), line.cend());
  std::cerr << "lineStr = " << lineStr << std::endl;
  assert(lineStr == "HelloWorld!!!Thereisatabhere");

  std::vector<std::string> lines = aoc::getLineInput<std::string>("input/util.dat");
  std::cerr << "Lines size: " << aoc::quote(lines.size()) << std::endl;
  for (U8 i = 0; i < lines.size(); ++i) {
    std::cerr << "Lines[" << i << "] = " << lines[i] << std::endl;
  }

  RUNTIME_ASSERT(lines.size() == 5);
  RUNTIME_ASSERT(lines[0] == "Hello");
  RUNTIME_ASSERT(lines[1] == "World");
  RUNTIME_ASSERT(lines[2] == "!!!");
  RUNTIME_ASSERT(lines[3] == "	There is a tab here");
  RUNTIME_ASSERT(lines[4] == "        ");

  RUNTIME_ASSERT_MSG(42uL == aoc::to_u64("42"), "42 == 42");

  const auto func = [](const std::string_view str) { std::cout << "[LAMBDA] " << str << std::endl; };
  Logger logger1;
  Logger<func> logger2;
  Logger<&test_func> logger3;
  logger1.log("Hello World");
  logger2.log("Hello World");
  logger3.log("Hello World");

/*
  aoc::ReadFileStream<char> fs1("input/util.dat");
  while (!fs1.isEmpty()) {
    std::cout << fs1.get().value() << ' ';
  }
  */
  aoc::ReadFileStream<std::string> fs2("input/util.dat");
  while (!fs2.isEmpty()) {
    std::optional<std::string> line = fs2.get();
    if (line.has_value()) {
      std::cout << fs2.get().value();
    } else {
      std::cout << "NO CONTENT" << std::endl;
    }
  }

  std::cout << "Successfully completed unit-test!" << std::endl;
  return 0;
}
