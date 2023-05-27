#ifndef __personal_crypto__
#define __personal_crypto__

#include "queries/DeleteQuery.hpp"
#include "queries/InsertQuery.hpp"
#include "queries/SelectQuery.hpp"
#include "queries/UpdateQuery.hpp"
#include <cryptopp/aes.h>
#include <cstddef>
#include <iostream>
#include <string>


using CryptoPP::AES;

#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif

template<size_t n>
using Cipher = std::array<byte, n>;


namespace Base {
    inline byte iv[16] = {0x01, 0xAB, 0x5F, 0xC2, 0x33, 0x70, 0xFF, 0x88, 0x44, 0x10, 0xE5, 0x7D, 0x20, 0x9A, 0xD1, 0xB3};

    Cipher<32> encryptBASE(std::string, byte[4], byte[16]);
    std::string decryptBASE(Cipher<32>, byte[4], byte[16]);

    Cipher<32> encryptInt(unsigned, size_t, byte[16]);
    unsigned decryptInt(Cipher<32>, size_t, byte[16]);
}

#endif
