#include "crypto/bgcc.hpp"

#include <iostream>

#include "crypto/utils.hpp"
#include "crypto/hash.hpp"


constexpr bool DOWN = false;
constexpr bool UP = true;


template<size_t n, size_t k>
BGCC<n,k> generateBGCC(const Circuit<n,1>& C, const std::array<CipherText<k>, n>& e0, const std::array<CipherText<k>, n>& e1) {
  constexpr std::bitset<n> x0, x1(-1);

  std::vector<CipherPair<k>> W;
  BGCC<n,k> bgcc {garble<n,1,k>(C, &W)};

  auto& K = W[bgcc.out[0]];  // bgcc.out[0] is the only output wire

  // Input labels
  std::array<CipherText<k>, n> I[2] {
    encode(x0,bgcc.e,bgcc.R),  // DOWN
    encode(x1,bgcc.e,bgcc.R)   // UP
  };

  // Input labels of children
  std::array<CipherText<k>, n> O[2][2] {
    { encode(x0,e0,bgcc.R), encode(x1,e0,bgcc.R) },  // L(eft)
    { encode(x0,e1,bgcc.R), encode(x1,e1,bgcc.R) }   // R(ight)
  };

  // Compute transition table
  for (size_t i = 0; i < n; ++i) {
    bool s = I[DOWN][i][0];  // Select bit

    hash(bgcc.T[L][0][i], K[DOWN]+I[s][i]);
    hash(bgcc.T[R][0][i], K[UP]+I[s][i]);
    hash(bgcc.T[L][1][i], K[DOWN]+I[!s][i]);
    hash(bgcc.T[R][1][i], K[UP]+I[!s][i]);

    bgcc.T[L][0][i] ^= O[L][s][i];
    bgcc.T[R][0][i] ^= O[R][s][i];
    bgcc.T[L][1][i] ^= O[L][!s][i];
    bgcc.T[R][1][i] ^= O[R][!s][i];
  }

  return bgcc;
}


template<size_t n, size_t k>
bool evaluateBGCC(std::array<CipherText<k>, n>& X, const Circuit<n,1>& C, const GarbledTable<k>& G, const CipherText<1>& d, const TransitionTable<n,k>& T) {
  std::vector<CipherText<k>> W;
  bool y = decode(evaluate(X, C, G, &W), d)[0];

  auto& K = W[C.out[0]];

  for (size_t i = 0; i < n; ++i) {
    bool s = X[i][0];  // Select bit
    hash(X[i], K+X[i]);
    X[i] ^= T[y][s][i];
  }

  return y;
}


/* ---------- Template instanciations ---------- */

// NOTE Templates instanciations are required

template BGCC<GCN,GCK> generateBGCC(const Circuit<GCN,1>&, const std::array<CipherText<GCK>, GCN>&, const std::array<CipherText<GCK>, GCN>&);
template bool evaluateBGCC(std::array<CipherText<GCK>, GCN>&, const Circuit<GCN,1>&, const GarbledTable<GCK>&, const CipherText<1>&, const TransitionTable<GCN,GCK>&);
