#ifndef __CRYPTO_UTILS_HPP__
#define __CRYPTO_UTILS_HPP__

#include <bitset>
#include <cstddef>
#include <cryptopp/cryptlib.h>


#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif


/**
 * @brief Generate a random bitset of size k.
 * @param[in] k The size of the bitset.
 */
template<size_t k>
std::bitset<k> random_bitset();

/**
 * @brief Truncate a byte array of size len to a bitset of size k.
 * @param[out] out The output bitset.
 * @param[in] in The input byte array.
 * @param[in] len The size of the input byte array.
 * @return The output bitset.
 */
template<size_t k>
std::bitset<k> trunc(std::bitset<k>& out, const byte* in, size_t len);

/**
 * @brief Pad a bitset of size k to a byte array of size len.
 * @param[out] out The output byte array.
 * @param[in] in The input bitset.
 * @param[in] len The size of the output byte array.
 * @return The output byte array.
 */
template<size_t k>
byte* pad(byte *out, const std::bitset<k>& in, size_t len);

/**
 * @brief Multiplication of a bitset by a boolean.
 * @note This is equivalent to a logical AND.
 */
template<size_t k>
std::bitset<k> operator*(bool a, const std::bitset<k>& b);

/**
 * @brief Concatenation of two bitsets.
 */
template<size_t n1, size_t n2>
std::bitset<n1+n2> operator+(const std::bitset<n1>& a, const std::bitset<n2>& b);


#endif
