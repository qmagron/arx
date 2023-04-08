#ifndef __HASH_HPP__
#define __HASH_HPP__

#include <bitset>


/**
 * @brief Hash a bitset.
 * @param[in] n The size of the input bitset
 * @param[in] m The size of the output bitset
 * @param[out] out The output bitset
 * @param[in] in The input bitset
 */
template<size_t n, size_t m>
void hash(std::bitset<m>& out, const std::bitset<n>& in);


#endif
