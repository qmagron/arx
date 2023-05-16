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
 * @brief A branching garbled circuit with a hardcoded value.
 * @param n The number of input wires
 * @param k The security parameter
 * @param T The transition table
 * @param Xv The hardcoded garbled input
 * @note See https://ia.cr/2016/591
 */
template<size_t n, size_t k>
struct LightBGCC: public LightGarbledCircuit<n,1,k> {
  TransitionTable<n/2,k> T;
  std::array<CipherText<k>, n/2> Xv;
};


/**
 * @brief Generate a branching garbled circuit chain.
 * @param[in] n The number of input wires
 * @param[in] k The security parameter
 * @param[in] C The circuit
 * @param[in] nid The unique id of the garbled circuit
 * @param[in] nonce A unique nonce used for hashing
 * @param[in] nidL The unique id of the left child
 * @param[in] nonceL The nonce of the left child
 * @param[in] nidR The unique id of the right child
 * @param[in] nonceR The nonce of the right child
 * @return The branching garbled circuit chain
 * @note See https://ia.cr/2016/591
 */
template<size_t n, size_t k>
BGCC<n,k> generateBGCC(const Circuit<n,1>& C, size_t nid, size_t nonce, size_t nidL = 0, size_t nonceL = 0, size_t nidR = 0, size_t nonceR = 0) {
  constexpr std::bitset<n> x0, x1(-1);

  std::vector<CipherPair<k>> W;
  BGCC<n,k> bgcc{garble<n,1,k>(C, nid, nonce, W)};

  // If the node has children
  if (nidL != -1 || nidR != -1) {
    // Recompute R0 and R1
    const CipherText<k> R0 = toR<k>(nidL, nonceL);
    const CipherText<k> R1 = toR<k>(nidR, nonceR);

    // Recompute e0 and e1
    std::array<CipherText<k>, n> e0;
    std::array<CipherText<k>, n> e1;
    for (const Wire& w: C.in) {
      e0[w] = toE<k>(nidL, nonceL, w);
      e1[w] = toE<k>(nidR, nonceR, w);
    }

    auto& K = W[bgcc.out[0]];  // bgcc.out[0] is the unique output wire

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
template<size_t n, size_t k, size_t start=0, size_t end=n/2>
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
 * @brief Lighten a branching garbled circuit.
 * @param[in] bgcc The branching garbled circuit to lighten
 * @param[in] v The hardcoded input
 * @return A pointer to the light branching garbled circuit
 */
template<size_t n, size_t k>
LightBGCC<n,k>* lightenBGCC(const BGCC<n,k>& bgcc, size_t v) {
  LightBGCC<n,k>* lbgcc = new LightBGCC<n,k>;

  // Copy the garbled table and the decode information
  lbgcc->G = bgcc.G;
  lbgcc->d = bgcc.d;

  // Encode the hardcoded input
  lbgcc->Xv = encode<n,k,n/2,n>(CipherText<n/2>(v), bgcc.e, bgcc.R);

  // Copy the half transition table
  for (size_t i = 0; i < n/2; ++i) {
    lbgcc->T[L][0][i] = bgcc.T[L][0][i];
    lbgcc->T[R][0][i] = bgcc.T[R][0][i];
    lbgcc->T[L][1][i] = bgcc.T[L][1][i];
    lbgcc->T[R][1][i] = bgcc.T[R][1][i];
  }

  return lbgcc;
}


#endif
