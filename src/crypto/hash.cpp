#include "crypto/hash.hpp"

#include <cryptopp/keccak.h>

#include "crypto/utils.hpp"


#ifdef __CRYPTOPP_BYTE__
using Keccak_256 = CryptoPP::Keccak_256;
using byte = CryptoPP::byte;
#endif


template<size_t k>
constexpr size_t nB() {
  return k/8+(k%8!=0);
}


template<size_t k>
void hash(std::bitset<k>& out, const std::bitset<k>& in) {
  byte inB[nB<k>()];
  byte outB[nB<k>()];

  // Convert bitset to byte array
  pad<k>(inB, in, nB<k>());

  // Perform hash
  Keccak_256 hash;
  hash.Update(inB, nB<k>());
  hash.TruncatedFinal(outB, nB<k>());

  // Convert byte array to bitset
  trunc<k>(out, outB, nB<k>());
}


/* ---------- Template instanciations ---------- */

template void hash(std::bitset<64> &out, const std::bitset<64> &in);
