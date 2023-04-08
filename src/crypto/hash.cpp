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


template<size_t n, size_t m>
void hash(std::bitset<m>& out, const std::bitset<n>& in) {
  byte inB[nB<n>()];
  byte outB[nB<m>()];

  // Convert bitset to byte array
  pad(inB, in, nB<n>());

  // Perform hash
  Keccak_256 hash;
  hash.Update(inB, nB<n>());
  hash.TruncatedFinal(outB, nB<m>());

  // Convert byte array to bitset
  trunc(out, outB, nB<m>());
}


/* ---------- Template instanciations ---------- */

template void hash(std::bitset<GCK>& out, const std::bitset<GCK>& in);
template void hash(std::bitset<GCK>& out, const std::bitset<2*GCK>& in);
