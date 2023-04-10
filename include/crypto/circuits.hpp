#ifndef __CIRCUITS_HPP__
#define __CIRCUITS_HPP__

#include <array>
#include <vector>

#include "crypto/circuit.hpp"


template<size_t n>
constexpr std::array<size_t, n> in() {
  std::array<size_t, n> in;

  for (size_t i = 0; i < n; ++i) {
    in[i] = i;
  }

  return in;
}


template<size_t n>
constexpr std::vector<Gate2> gatesCOMP() {
  #define X(i) (n-i-1)
  #define Y(i) (2*n-i-1)
  #define BEG(i) (2*n+1+4*(i-1))
  #define C(i) (BEG(i))
  #define A(i) (BEG(i)+1)
  #define B(i) (BEG(i)+2)
  #define D(i) (BEG(i)+3)
  #define Z(i) (BEG(i)+4)

  std::vector<Gate2> gates;

  // Skip c for the first bit
  gates.push_back(Gate2{ XOR, X(0), Y(0),  2*n });
  gates.push_back(Gate2{ AND,  2*n, Y(0), Z(0) });

  for (size_t i = 1; i < n; ++i) {
    gates.push_back(Gate2{ NXOR, X(i), C(i), A(i) });
    gates.push_back(Gate2{  XOR, Y(i), C(i), B(i) });
    gates.push_back(Gate2{  AND, A(i), B(i), D(i) });
    gates.push_back(Gate2{  XOR, D(i), C(i), Z(i) });
  }

  return gates;
}


/**
 * @brief Constructs a circuit for the comparison of two numbers.
 * @param[in] n The number of bits
 * @return The circuit
 */
template<size_t n>
constexpr Circuit<2*n,1> COMP() {
  return Circuit<2*n,1> {
    .nWires = 4+6*(n-1),
    .in = in<2*n>(),
    .out = {4+6*(n-1)-1},
    .gates = gatesCOMP<n>()
  };
}


#endif
