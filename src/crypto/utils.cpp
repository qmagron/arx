#include "crypto/utils.hpp"

#include <random>


template<size_t k>
std::bitset<k> random_bitset() {
  std::bitset<k> b;

  std::random_device device;
  std::mt19937 generator(device());
  std::bernoulli_distribution d(0.5);

  for (size_t i = 0; i < k; ++i) {
    b[i] = d(generator);
  }

  return b;
}


template<size_t k>
std::bitset<k> trunc(std::bitset<k>& out, const byte* in, size_t len) {
  for (size_t b = 0; b < k; ++b) {
    out[b] = (in[(len-1) - (b/8)] >> (b%8)) & 0x01;
  }

  return out;
}

template<size_t k>
byte* pad(byte* out, const std::bitset<k>& in, size_t len) {
  for (size_t b = 0; b < k; ++b) {
    if (in[b]) {
      out[len-1-b/8] |= (0x01 << (b%8));
    } else {
      out[len-1-b/8] &= ~(0x01 << (b%8));
    }
  }

  return out;
}

template<size_t k>
std::bitset<k> operator*(bool a, const std::bitset<k>& b) {
  if (a) {
    return b;
  } else {
    return std::bitset<k>();
  }
}

template<size_t n1, size_t n2>
std::bitset<n1+n2> operator+(const std::bitset<n1>& a, const std::bitset<n2>& b) {
  std::bitset<n1+n2> c;

  for (size_t i = 0; i < n1; ++i) {
    c[i] = b[i];
    c[i+n2] = a[i];
  }

  return c;
}


/* ---------- Template instanciations ---------- */

template std::bitset<GCK> random_bitset();

template std::bitset<GCK> trunc(std::bitset<GCK>&, const byte*, size_t len);

template byte* pad(byte*, const std::bitset<GCK>&, size_t);
template byte* pad(byte*, const std::bitset<2*GCK>&, size_t);

template std::bitset<GCK> operator*(bool, const std::bitset<GCK>&);

template std::bitset<2*GCK> operator+(const std::bitset<GCK>&, const std::bitset<GCK>&);
