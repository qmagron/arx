#ifndef __BGCC_HPP__
#define __BGCC_HPP__

#include "crypto/garbled.hpp"
#include "crypto/hash.hpp"
#include "crypto/utils.hpp"


constexpr bool L = false;
constexpr bool R = true;


template<size_t n, size_t k>
using TransitionTable = std::array<std::array<std::array<CipherText<k>, n>, 2>, 2>;


/**
 * @brief A branching garbled circuit chain.
 * @param n The number of input wires
 * @param k The security parameter
 * @param T The transition table
 * @note See https://ia.cr/2016/591
 */
template<size_t n, size_t k>
struct BGCC: public GarbledCircuit<n,1,k> {
  TransitionTable<n,k> T;
};

/**
 * @brief A topology-less branching garbled circuit chain.
 * @param n The number of input wires
 * @param k The security parameter
 * @param T The transition table
 * @note See https://ia.cr/2016/591
 */
template<size_t n, size_t k>
struct LightBGCC: public LightGarbledCircuit<n,1,k> {
  TransitionTable<n/2,k> T;
};


/**
 * @brief Generate a branching garbled circuit chain.
 * @param[in] n The number of input wires
 * @param[in] k The security parameter
 * @param[in] C The circuit
 * @param[in] e0 The first encode information
 * @param[in] R0 The first free-XOR value
 * @param[in] e1 The second encode information
 * @param[in] R1 The second free-XOR value
 * @return The branching garbled circuit chain
 * @note See https://ia.cr/2016/591
 */
template<size_t n, size_t k>
BGCC<n,k> generateBGCC(const Circuit<n,1>& C, const std::array<CipherText<k>, n>& e0, const CipherText<k>& R0, const std::array<CipherText<k>, n>& e1, const CipherText<k>& R1) {
  constexpr std::bitset<n> x0, x1(-1);

  std::vector<CipherPair<k>> W;
  BGCC<n,k> bgcc {garble<n,1,k>(C, W)};

  auto& K = W[bgcc.out[0]];  // bgcc.out[0] is the only output wire

  // Input labels
  std::array<CipherText<k>, n> I[2] {
    encode(x0,bgcc.e,bgcc.R),  // DOWN
    encode(x1,bgcc.e,bgcc.R)   // UP
  };

  // Input labels of children
  std::array<CipherText<k>, n> O[2][2] {
    { encode(x0,e0,R0), encode(x1,e0,R0) },  // L(eft)
    { encode(x0,e1,R1), encode(x1,e1,R1) }   // R(ight)
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

/**
 * @brief Evaluate a branching garbled circuit chain.
 * @param[in] n The number of input wires
 * @param[in] k The security parameter
 * @param[in] start The start index of the output encoding
 * @param[in] end The end index of the output encoding
 * @param[in,out] X The garbled input that will be overwritten for the next evaluation
 * @param[in] C The circuit
 * @param[in] G The garbled table
 * @param[in] d The decode information
 * @param[in] T The transition table
 * @return The output of the circuit (L/R)
 */
template<size_t n, size_t k, size_t start=0, size_t end=GCN/2>
bool evaluateBGCC(std::array<CipherText<k>, n>& X, const Circuit<n,1>& C, const GarbledTable<k>& G, const CipherText<1>& d, const TransitionTable<end-start,k>& T) {
  static_assert(start < end, "start must be smaller than end");
  static_assert(end <= n, "end must be smaller than n");

  std::vector<CipherText<k>> W;
  bool y = decode(evaluate(X, C, G, W), d)[0];

  auto& K = W[C.out[0]];

  for (size_t i = 0; i < end-start; ++i) {
    bool s = X[start+i][0];  // Select bit
    hash(X[start+i], K+X[start+i]);
    X[start+i] ^= T[y][s][i];
  }

  return y;
}

/**
 * @brief Evaluate a branching garbled circuit chain.
 * @param[in] n The number of input wires
 * @param[in] k The security parameter
 * @param[in,out] X The garbled input that will be overwritten for the next evaluation
 * @param[in] C The circuit
 * @param[in] G The garbled table
 * @param[in] d The decode information
 * @param[in] T The transition table
 * @return The output of the circuit (L/R)
 */
template<size_t n, size_t k>
bool evaluateBGCC(std::array<CipherText<k>, n>& X, const Circuit<n,1>& C, const GarbledTable<k>& G, const CipherText<1>& d, const TransitionTable<n,k>& T) {
  return evaluateBGCC<n,k,0,n>(X, C, G, d, T);
}


#endif
