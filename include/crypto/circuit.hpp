#ifndef __CIRCUIT_HPP__
#define __CIRCUIT_HPP__

#include <cstddef>


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


#endif
