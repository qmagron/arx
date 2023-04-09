#ifndef __GARBLED_HPP__
#define __GARBLED_HPP__

#include <array>
#include <cstddef>
#include <vector>
#include <bitset>

#include "crypto/hash.hpp"
#include "crypto/utils.hpp"


constexpr bool DOWN = false;
constexpr bool UP = true;


/**
 * @note To add support for other gates, see https://ia.cr/2014/756
 */
enum GateType {
  XOR,
  NXOR,
  AND,
};

using Wire = size_t;

struct Gate2 {
  GateType type;
  Wire in[2];
  Wire out;
};

/**
 * @brief A binary circuit.
 * @param n The number of input wires
 * @param m The number of output wires
 * @param nWires The number of wires
 * @param in The input wires
 * @param out The output wires
 * @param gates The gates
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t m>
struct Circuit {
  size_t nWires;
  std::array<Wire, n> in;
  std::array<Wire, m> out;
  std::vector<Gate2> gates;
};


template<size_t k> using CipherText = std::bitset<k>;
template<size_t k> using CipherPair = std::array<CipherText<k>, 2>;

template<size_t k>
using GarbledTable = std::vector<CipherPair<k>>;


/**
 * @brief A complete garbled circuit.
 * @param n The number of input wires
 * @param m The number of output wires
 * @param k The security parameter
 * @param G The garbled table
 * @param e The encode information
 * @param d The decode information
 * @param R The free-XOR value
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t m, size_t k>
struct GarbledCircuit: public Circuit<n,m> {
  GarbledTable<k> G = {};
  std::array<CipherText<k>, n> e = {};
  CipherText<m> d = {};
  CipherText<k> R = {};
};


/**
 * @brief Garble a circuit.
 * @param[in] n The number of input wires
 * @param[in] m The number of output wires
 * @param[in] k The security parameter
 * @param[in] C The circuit to garble
 * @param[out] W An table containing all wire labels
 * @return The garbled Circuit
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t m, size_t k>
GarbledCircuit<n,m,k> garble(const Circuit<n,m>& C, std::vector<CipherPair<k>>& W) {
  GarbledCircuit<n,m,k> gC {C};

  auto& G = gC.G;
  auto& e = gC.e;
  auto& d = gC.d;
  auto& R = gC.R;
  W.resize(C.nWires);

  R = random_bitset<k>();
  R[0] = 1;

  // Generate input labels
  for (const Wire& i: C.in) {
    W[i][DOWN] = random_bitset<k>();
    W[i][UP] = W[i][DOWN] ^ R;
    e[i] = W[i][DOWN];
  }

  // Generate other labels
  for (const Gate2& g: C.gates) {
    switch (g.type) {
     case XOR:
      eFreeXOR(W, g);
      break;
     case NXOR:
      eFreeNXOR(W, g);
      break;
     case AND:
      eHalfAND(W, g, R, G);
      break;
    }

   W[g.out][UP] = W[g.out][DOWN] ^ R;
  }

  // Generate decode information
  for (size_t o = 0; o < m; ++o) {
    d[m-o-1] = W[C.out[o]][DOWN][0];
  }

  return gC;
}

/**
 * @brief Garble a circuit.
 * @param[in] n The number of input wires
 * @param[in] m The number of output wires
 * @param[in] k The security parameter
 * @param[in] C The circuit to garble
 * @return The garbled Circuit
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t m, size_t k>
inline GarbledCircuit<n,m,k> garble(const Circuit<n,m>& C) {
  std::vector<CipherPair<k>> W;
  return garble(C, W);
}

/**
 * @brief Encode an input.
 * @param[in] n The number of input wires
 * @param[in] k The security parameter
 * @param[in] x The input to encode
 * @param[in] e The encode information
 * @param[in] R The free-XOR value
 * @return The garbled input (X)
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t k>
std::array<CipherText<k>, n> encode(const std::bitset<n>& x, const std::array<CipherText<k>, n>& e, const CipherText<k>& R) {
  std::array<CipherText<k>, n> X;

  for (size_t i = 0; i < n; ++i) {
    X[i] = e[i] ^ x[n-i-1]*R;
  }

  return X;
}

/**
 * @brief Evaluate a garbled circuit.
 * @param[in] n The number of input wires
 * @param[in] m The number of output wires
 * @param[in] k The security parameter
 * @param[in] X The garbled input
 * @param[in] C The garbled circuit
 * @param[in] G The garbled table
 * @param[out] W An table containing all wire labels
 * @return The garbled output (Y)
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t m, size_t k>
std::array<CipherText<k>, m> evaluate(const std::array<CipherText<k>, n>& X, const Circuit<n,m>& C, const GarbledTable<k>& G, std::vector<CipherText<k>>& W) {
  W.resize(C.nWires);
  size_t t = 0;

  // Recover input labels
  for (const Wire& i: C.in) {
    W[i] = X[i];
  }

  // Recover other labels
  for (const Gate2& g: C.gates) {
    switch (g.type) {
     case XOR:
     case NXOR:
      dFreeXOR(W, g);
      break;
     case AND:
      dHalfAND(W, g, G, t++);
      break;
    }
  }

  // Construct the garbled output
  std::array<CipherText<k>, m> Y;
  for (size_t o = 0; o < m; ++o) {
    Y[o] = W[C.out[o]];
  }

  return Y;
}

/**
 * @brief Evaluate a garbled circuit.
 * @param[in] n The number of input wires
 * @param[in] m The number of output wires
 * @param[in] k The security parameter
 * @param[in] X The garbled input
 * @param[in] C The garbled circuit
 * @param[in] G The garbled table
 * @return The garbled output (Y)
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t m, size_t k>
inline std::array<CipherText<k>, m> evaluate(const std::array<CipherText<k>, n>& X, const Circuit<n,m>& C, const GarbledTable<k>& G) {
  std::vector<CipherText<k>> W;
  return evaluate(X, C, G, W);
}

/**
 * @brief Decode a garbled output.
 * @param[in] m The number of output wires
 * @param[in] k The security parameter
 * @param[in] Y The garbled output
 * @param[in] d The decode information
 * @return The decoded output (y)
 * @note See https://ia.cr/2014/756
 */
template<size_t m, size_t k>
std::bitset<m> decode(const std::array<CipherText<k>, m>& Y, const CipherText<m>& d) {
  std::bitset<m> y;

  for (size_t o = 0; o < m; ++o) {
    y[m-o-1] = d[m-o-1] ^ Y[o][0];
  }

  return y;
}


template<size_t k>
inline void eFreeXOR(std::vector<CipherPair<k>>& W, const Gate2& g) {
  W[g.out][DOWN] = W[g.in[0]][DOWN] ^ W[g.in[1]][DOWN];
}

template<size_t k>
inline void dFreeXOR(std::vector<CipherText<k>>& W, const Gate2& g) {
  W[g.out] = W[g.in[0]] ^ W[g.in[1]];
}


template<size_t k>
inline void eFreeNXOR(std::vector<CipherPair<k>>& W, const Gate2& g) {
  W[g.out][DOWN] = W[g.in[0]][UP] ^ W[g.in[1]][DOWN];
}


template<size_t k>
inline void eHalfAND(std::vector<CipherPair<k>>& W, const Gate2& g, const CipherText<k>& R, GarbledTable<k>& G) {
  bool a = W[g.in[0]][DOWN].test(0);
  bool b = W[g.in[1]][DOWN].test(0);

  // Hash input labels
  CipherText<k> H[2][2];
  for (size_t i = 0; i < 2; ++i) {
    hash(H[i][DOWN], W[g.in[i]][DOWN]);
    hash(H[i][UP], W[g.in[i]][UP]);
  }

  size_t t = G.size();
  G.push_back({});

  // First half-gate
  G[t][0] = H[0][DOWN] ^ H[0][UP] ^ b*R;
  CipherText<k> Wg = H[0][DOWN] ^ a*G[t][0];

  // Second half-gate
  G[t][1] = H[1][DOWN] ^ H[1][UP] ^ W[g.in[0]][DOWN];
  CipherText<k> We = H[1][DOWN] ^ b*(G[t][1] ^ W[g.in[0]][DOWN]);

  // Combine half-gates
  W[g.out][DOWN] = Wg ^ We;
}

template<size_t k>
inline void dHalfAND(std::vector<CipherText<k>>& W, const Gate2& g, const GarbledTable<k>& G, size_t t) {
  bool a = W[g.in[0]].test(0);
  bool b = W[g.in[1]].test(0);

  // Hash input labels
  CipherText<k> H[2];
  for (size_t i = 0; i < 2; ++i) {
    hash(H[i], W[g.in[i]]);
  }

  CipherText<k> Wg = H[0] ^ a*G[t][0];
  CipherText<k> We = H[1] ^ b*(G[t][1] ^ W[g.in[0]]);
  W[g.out] = Wg ^ We;
}


#endif
