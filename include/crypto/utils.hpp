#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <bitset>
#include <cstddef>
#include <cryptopp/cryptlib.h>
#include <random>


#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif


/**
 * @brief Generate a random bitset of size k.
 * @param[in] k The size of the bitset.
 */
template<size_t k>
constexpr std::bitset<k> random_bitset() {
  std::bitset<k> b;

  std::random_device device;
  std::mt19937 generator(device());
  std::bernoulli_distribution d(0.5);

  for (size_t i = 0; i < k; ++i) {
    b[i] = d(generator);
  }

  return b;
}

/**
 * @brief Truncate a byte array of size len to a bitset of size k.
 * @param[out] out The output bitset.
 * @param[in] in The input byte array.
 * @param[in] len The size of the input byte array.
 * @return The output bitset.
 */
template<size_t k>
std::bitset<k> trunc(std::bitset<k>& out, const byte* in, size_t len) {
  for (size_t b = 0; b < k; ++b) {
    out[b] = (in[(len-1) - (b/8)] >> (b%8)) & 0x01;
  }

  return out;
}

/**
 * @brief Pad a bitset of size k to a byte array of size len.
 * @param[out] out The output byte array.
 * @param[in] in The input bitset.
 * @param[in] len The size of the output byte array.
 * @return The output byte array.
 */
template<size_t k>
byte* pad(byte* out, const std::bitset<k>& in, size_t len) {
  for (size_t b = 0; b < k; ++b) {
    if (in[b]) {
      out[len-1-b/8] |= (0x01 << (b%8));
    } else {
      out[len-1-b/8] &= ~(0x01 << (b%8));
    }
  }

  return out;
}

/**
 * @brief Multiplication of a bitset by a boolean.
 * @note This is equivalent to a logical AND.
 */
template<size_t k>
constexpr std::bitset<k> operator*(bool a, const std::bitset<k>& b) {
  if (a) {
    return b;
  } else {
    return std::bitset<k>();
  }
}

/**
 * @brief Concatenation of two bitsets.
 */
template<size_t n1, size_t n2>
constexpr std::bitset<n1+n2> operator+(const std::bitset<n1>& a, const std::bitset<n2>& b) {
  std::bitset<n1+n2> c;

  for (size_t i = 0; i < n1; ++i) {
    c[i] = b[i];
    c[i+n2] = a[i];
  }

  return c;
}


#endif
