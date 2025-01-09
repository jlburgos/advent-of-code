#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <vector>
#include <string>
#include <sstream>

namespace Util {
  // Test lib
  int testUtil();

  // STDIN processing
  std::vector<char> getSingleLineInput(const std::string_view filename);
  std::vector<std::string> getMultiLineInput(const std::string_view filename);

  // STDOUT processing
  template <typename T>
  std::string quoted(const T input) {
    std::stringstream ss;
    ss << '"' << input << '"';
    return ss.str();
  }
}

#endif /* _UTIL_HPP */
