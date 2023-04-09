#ifndef __GARBLED_HPP__
#define __GARBLED_HPP__

#include <array>
#include <cstddef>
#include <vector>
#include <bitset>


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
GarbledCircuit<n,m,k> garble(const Circuit<n,m>& C, std::vector<CipherPair<k>>& W);

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
std::array<CipherText<k>, n> encode(const std::bitset<n>& x, const std::array<CipherText<k>, n>& e, const CipherText<k>& R);

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
std::array<CipherText<k>, m> evaluate(const std::array<CipherText<k>, n>& X, const Circuit<n,m>& C, const GarbledTable<k>& G, std::vector<CipherText<k>>& W);

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
std::bitset<m> decode(const std::array<CipherText<k>, m>& Y, const CipherText<m>& d);


#endif
