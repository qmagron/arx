#ifndef __HASH_HPP__
#define __HASH_HPP__


#include <bitset>


/**
 * @brief Hash a bitset of size k.
 * @param[out] out The output bitset.
 * @param[in] in The input bitset.
 */
template<size_t k>
void hash(std::bitset<k>& out, const std::bitset<k>& in);


#endif
