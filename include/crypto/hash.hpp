#ifndef __HASH_HPP__
#define __HASH_HPP__

#include <bitset>
#include <cryptopp/keccak.h>

#include "crypto/utils.hpp"


#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif


template<size_t k>
constexpr size_t nB() {
  return k/8+(k%8!=0);
}


/**
 * @brief Hash a bitset.
 * @param[in] n The size of the input bitset
 * @param[in] m The size of the output bitset
 * @param[out] out The output bitset
 * @param[in] in The input bitset
 */
template<size_t n, size_t m>
void hash(std::bitset<m>& out, const std::bitset<n>& in) {
  byte inB[nB<n>()];
  byte outB[nB<m>()];

  // Convert bitset to byte array
  pad(inB, in, nB<n>());

  // Perform hash
  CryptoPP::Keccak_256 hash;
  hash.Update(inB, nB<n>());
  hash.TruncatedFinal(outB, nB<m>());

  // Convert byte array to bitset
  trunc(out, outB, nB<m>());
}


#endif
