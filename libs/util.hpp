#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <optional>
#include <source_location>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#define RUNTIME_ASSERT_IMPL(condition, msg, location)                   \
  do {                                                                  \
    if (!(condition)) {                                                 \
      std::cerr << "\n================= Assertion failed !!!"           \
                << "\n Message:\t'" << msg << "'"                       \
                << "\n Condition:\t(" << #condition << ")"              \
                << "\n File:\t\t[" << location.file_name() << "]"       \
                << "\n Line:\t\t[" << location.line() << "]"            \
                << "\n Function:\t[" << location.function_name() << "]" \
                << "\n=================\n"                              \
                << std::endl;                                           \
      std::abort(); /* Generate core dump */                            \
    }                                                                   \
  } while (false)
#define RUNTIME_ASSERT_MSG(condition, msg) RUNTIME_ASSERT_IMPL(condition, msg, std::source_location::current())
#define RUNTIME_ASSERT(condition) RUNTIME_ASSERT_MSG(condition, "")

// Special case for unsigned 8-bit values
using UCHAR = unsigned char;

using U8 = std::uint8_t;
using I8 = std::int8_t;

using U16 = std::uint16_t;
using I16 = std::int16_t;

using U32 = std::uint32_t;
using I32 = std::int32_t;

using U64 = std::uint64_t;
using I64 = std::int64_t;

using F32 = float;
using F64 = double;

template <void (*DefaultLogger)(const std::string_view) = nullptr>
struct Logger {
  void log(const std::string_view msg) {
    if constexpr (nullptr == DefaultLogger) {
      std::cout << "[DefaultLogger]: " << msg << '\n';
    } else {
      (*DefaultLogger)(msg);
    }
  }
};

static inline void reloadStdinStream(const std::string_view filename) {
  std::cin.clear();
  std::rewind(stdin);
  //std::fseek(stdin, 0, SEEK_SET);
  RUNTIME_ASSERT(nullptr != freopen(filename.data(), "r", stdin));
}

static inline void closeStdinStream() {
  RUNTIME_ASSERT(0 == fclose(stdin));
}

namespace aoc {
  // String processing
  bool containsChar(const char ch, const std::vector<char> &chars) {
    const auto isEqual = [ch](const char ch2) -> bool { return ch == ch2; };
    return std::any_of(chars.cbegin(), chars.cend(), isEqual);
  }

  template <typename T>
  requires std::is_same_v<T, char> || std::is_same_v<T, std::string>
  class ReadFileStream {
  private:
    std::ifstream ifs;

  public:
    explicit ReadFileStream(const std::string_view path) : ifs(path.data()) {}

    std::optional<T> get() {
      if (isEmpty()) {
        return std::nullopt;
      }

      if constexpr (std::is_same_v<T, char>) {
        //char ch = ifs.get();
        //while (!isEmpty() && aoc::containsChar(ch, {' ', '\n', '\r', '\t'})) { ch = ifs.get(); }
        return std::optional<T>(ifs.get());
      } else { // std::is_same_v<T, std::string>
        std::string line;
        if (std::getline(ifs, line, '\n')) {
          return std::optional<T>(line);
        }
        return std::nullopt;
      }
    }

    bool isEmpty() const {
      return ifs.eof();
    }
  };

  template <typename T>
  requires std::is_same_v<T, char> || std::is_same_v<T, std::string>
  class WriteFileStream {
    private:
      std::ofstream ofs;

    public:
      explicit WriteFileStream(const std::string_view path) : ofs(path.data()) {}

      bool write(const T writeable) {
        if constexpr (std::is_same_v<T, char>) {

        } else { // std::is_same_v<T, std::string>

        }
      }
  };

/*
  template <typename T>
  requires std::is_same_v<T, char> || std::is_same_v<T, std::string>
  std::istringstream getInputStream(const std::string_view filename) {
    
  }
  */

  // STDIN processing
  template <typename T>
  requires std::is_same_v<T, char> || std::is_same_v<T, std::string>
  std::vector<T> getLineInput(const std::string_view filename) {
    reloadStdinStream(filename);
    std::vector<T> input;
    if constexpr (std::is_same_v<T, char>) {
      char ch;
      while (std::cin.get(ch)) {
        if (!aoc::containsChar(ch, {' ', '\n', '\r', '\t'})) {
          input.push_back(ch);
        }
      }
    } else {
      std::string line;
      while (std::getline(std::cin, line)) {
        if (!line.empty()) {
          input.push_back(line);
        }
      }
    }
    closeStdinStream();
    return input;
  }

  template <typename T>
  concept Streamable = requires(std::ostream &os, T t) {
    { os << t } -> std::same_as<std::ostream &>;
  };

  // STDOUT processing
  template <Streamable T>
  std::string quote(const T input) {
    std::stringstream ss;
    ss << '"' << input << '"';
    return ss.str();
  }

  template <int N = 10>
  std::optional<U64> to_u64(const std::string_view str) {
    try {
      return std::optional<U64>{ std::stoul(str.data(), nullptr, N) };
    } catch (const std::invalid_argument &e) {
      std::cerr << "Failed to convert " << quote(str) << " due to invalid_argument error: " << e.what() << std::endl;
    } catch (const std::out_of_range &e) {
      std::cerr << "Failed to convert " << quote(str) << " due to out_of_range error: " << e.what() << std::endl;
    }
    return std::nullopt;
  }

  std::vector<char> getSingleLineInput(const std::string_view filename) {
   reloadStdinStream(filename);
   std::vector<char> input;
   char ch;
   while (std::cin.get(ch)) {
     if (!containsChar(ch, {' ', '\n', '\r', '\t'})) {
       input.push_back(ch);
     }
   }
   closeStdinStream();
   return input;
 }

  std::vector<std::string> getMultiLineInput(const std::string_view filename) {
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

}

#endif /* _UTIL_HPP */
