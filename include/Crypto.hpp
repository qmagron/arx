#ifndef __personal_crypto__
#define __personal_crypto__

#include <cryptopp/aes.h>
#include <cstddef>
#include <iostream>
#include <string>

using CryptoPP::AES;

#ifdef __CRYPTOPP_BYTE__
using CryptoPP::byte;
#endif

namespace Crypto {
// 128 bit array key of 59E22E9D3351B9B46627F49DA8BF56D4

inline byte arxKey[AES::DEFAULT_KEYLENGTH] = {
    0x59, 0xE2, 0x2E, 0x9D, 0x33, 0x51, 0xB9, 0xB4,
    0x66, 0x27, 0xF4, 0x9D, 0xA8, 0xBF, 0x56, 0xD4};
inline byte iv[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
std::string encryptBASE(std::string, byte[4]);
std::string decryptBASE(std::string, byte[4]);

} // namespace Crypto

#endif
