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

namespace Base {
    inline byte iv[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    std::string encryptBASE(std::string, byte[4],byte[16]);
    std::string decryptBASE(std::string, byte[4],byte[16]);
} // namespace Base

#endif
