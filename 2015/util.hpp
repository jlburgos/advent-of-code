#include <iostream>
#include <vector>

namespace Util {
  const std::vector<char> getInput() {
    std::vector<char> input;
    char ch;
    while (std::cin.get(ch)) {
      if (ch != '\n') {
        input.push_back(ch);
      }
    }
    return input;
  }
}
