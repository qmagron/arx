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

template<size_t k> using Label      = std::bitset<k>;
template<size_t k> using LabelsPair = std::array<Label<k>, 2>;


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

template<size_t k>
using GarbledTable = std::vector<LabelsPair<k>>;

/**
 * @brief A complete garbled circuit.
 * @param n The number of input wires
 * @param m The number of output wires
 * @param k The security parameter
 * @param C The circuit
 * @param T The garbled table
 * @param e The encode information
 * @param d The decode information
 * @param R The free-XOR value
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t m, size_t k>
struct GarbledCircuit {
  Circuit<n,m> C;
  GarbledTable<k> T = {};
  std::array<std::bitset<k>, n> e = {};
  std::bitset<m> d = {};
  std::bitset<k> R = {};
};


/**
 * @brief Garble a circuit.
 * @param[in] n The number of input wires
 * @param[in] m The number of output wires
 * @param[in] k The security parameter
 * @param[in,out] C The circuit to garble
 * @return The garbled Circuit
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t m, size_t k>
GarbledCircuit<n,m,k> garble(const Circuit<n,m>& C);

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
std::array<std::bitset<k>, n> encode(const std::bitset<n>& x, const std::array<std::bitset<k>, n>& e, const std::bitset<k>& R);

/**
 * @brief Evaluate a garbled circuit.
 * @param[in] n The number of input wires
 * @param[in] m The number of output wires
 * @param[in] k The security parameter
 * @param[in] X The garbled input
 * @param[in] C The garbled circuit
 * @param[in] T The garbled table
 * @return The garbled output (Y)
 * @note See https://ia.cr/2014/756
 */
template<size_t n, size_t m, size_t k>
std::array<std::bitset<k>, m> evaluate(const std::array<std::bitset<k>, n>& X, const Circuit<n,m>& C, const GarbledTable<k>& T);

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
std::bitset<m> decode(const std::array<std::bitset<k>, m>& Y, const std::bitset<m>& d);


#endif
