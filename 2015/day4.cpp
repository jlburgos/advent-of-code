#include <algorithm>
#include <array>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

// evp.h - high-level cryptographic functions
#include <openssl/evp.h>

#include "libs/numeric-types.hpp"
#include "libs/util.hpp"

void part1(const std::string_view key);
void part2(const std::string_view key);

std::string toHex(const UCHAR ch);
std::string toMd5(const std::string_view key);
void compute_md5_suffix(const std::string_view key, const U8 prefix_zeroes);

int main() {
  const std::vector<char> input = Util::getSingleLineInput("input/day4.dat");
  std::string key(input.begin(), input.end());
  part1(key);
  part2(key);
  return 0;
}

void part1(const std::string_view key) {
  std::cout << "Solving part 1 ... " << std::flush;
  compute_md5_suffix(key, 5);
}

void part2(const std::string_view key) {
  std::cout << "Solving part 2 ... " << std::flush;
  compute_md5_suffix(key, 6);
}

void compute_md5_suffix(const std::string_view key, const U8 prefix_zeroes) {
  const std::string prefix(prefix_zeroes, '0');
  std::string md5_str;
  std::ostringstream oss;
  U64 i = -1; // underflowing so first iteration uses suffix i==0
  while (md5_str.substr(0, prefix_zeroes) != prefix) {
    oss.str("");
    oss << key << ++i;
    md5_str = toMd5(oss.str());
  }
  std::cout << "Hash challenge solved with additional number '" << i << "' giving MD5(" << oss.str() << ") = " << md5_str << std::endl;
}

std::string toHex(const UCHAR ch) {
  static const UCHAR index[] = "0123456789ABCDEF";
  std::string hex;
  hex.reserve(2);
  hex += index[ch >> 4]; // upper nibble
  hex += index[ch & 0x0F]; // lower nibble
  return hex;
}

std::string toMd5(const std::string_view key) {
  static std::array<UCHAR, EVP_MAX_MD_SIZE> digest;
  U32 length;

  // Documentation: https://docs.openssl.org/3.0/man3
  EVP_MD_CTX *context = EVP_MD_CTX_new();
  EVP_DigestInit(context, EVP_md5());
  EVP_DigestUpdate(context, key.data(), key.size());
  EVP_DigestFinal(context, digest.data(), &length);
  EVP_MD_CTX_free(context);

  std::string md5_str;
  md5_str.reserve(2 * length); // each char converted to 2-byte hexadecimal representation
  for (U8 i = 0; i < length; ++i) {
    md5_str += toHex(digest[i]);
  }
  return md5_str;
}