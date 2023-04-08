#ifndef __BGCC_HPP__
#define __BGCC_HPP__

#include "crypto/garbled.hpp"


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
 * @brief Generate a branching garbled circuit chain.
 * @param[in] n The number of input wires
 * @param[in] k The security parameter
 * @param[in] C The circuit
 * @param[in] e0 The first encode information
 * @param[in] e1 The second encode information
 * @return The branching garbled circuit chain
 * @note See https://ia.cr/2016/591
 */
template<size_t n, size_t k>
BGCC<n,k> generateBGCC(const Circuit<n,1>& C, const std::array<CipherText<k>, n>& e0, const std::array<CipherText<k>, n>& e1);

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
bool evaluateBGCC(std::array<CipherText<k>, n>& X, const Circuit<n,1>& C, const GarbledTable<k>& G, const CipherText<1>& d, const TransitionTable<n,k>& T);


#endif
