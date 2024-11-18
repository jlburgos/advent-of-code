#include <iostream>
#include <cstdint>
#include <cstring>

void part1(const char * const input);
void part2(const char * const input);

int main(const int argc, const char * const * const argv) {
  if (argc != 2) {
    std::cerr << "Expected an argument!" << std::endl;
    return 1;
  }

  part1(argv[1]);
  part2(argv[1]);

  return 0;
}

void part1(const char * const input) {
  int32_t floor = 0;
  const uint32_t len = strlen(input);
  for(uint32_t i = 0; i < len; ++i) {
    input[i] == '(' ? ++floor : --floor;
  }

  std::cout << "Current Floor: " << floor << std::endl;
}

void part2(const char * const input) {
  int32_t floor = 0;
  const uint32_t len = strlen(input);
  for(uint32_t i = 0; i < len; ++i) {
    input[i] == '(' ? ++floor : --floor;
    if (floor < 0) {
      std::cout << "Character at position " << i + 1 << " caused Santa to enter the basement." << std::endl;
      break;
    }
  }
}
