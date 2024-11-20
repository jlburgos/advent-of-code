#include <vector>
#include <string>

namespace Util {
  std::vector<char> getSingleLineInput(const std::string_view filename);
  std::vector<std::string> getMultiLineInput(const std::string_view filename);
}
