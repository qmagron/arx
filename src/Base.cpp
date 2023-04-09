#include "Base.hpp"

#include <cryptopp/ccm.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>

#include "queries/GenericQuery.hpp"

using CryptoPP::AutoSeededRandomPool;

using CryptoPP::CTR_Mode;

using CryptoPP::StreamTransformationFilter;
using CryptoPP::StringSink;
using CryptoPP::StringSource;

#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif

std::string Base::encryptBASE(std::string message, byte counter[4]) {
  std::string cipher;

  byte fullIV[16];

  // merge counter and iv into fullIV
  for (int i = 0; i < 12; i++) {
    fullIV[i] = iv[i];
  }
  for (int i = 12; i < 16; i++) {
    fullIV[i] = counter[i - 12];
  }

  try {

    CTR_Mode<AES>::Encryption e;
    e.SetKeyWithIV(Base::arxKey, sizeof(Base::arxKey), fullIV);

    // The StreamTransformationFilter adds padding
    //  as required. ECB and CBC Mode must be padded
    //  to the block size of the cipher.
    StringSource(message, true,
                 new StreamTransformationFilter(
                     e,
                     new StringSink(cipher)) // StreamTransformationFilter
    );                                       // StringSource
  } catch (const CryptoPP::Exception &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  return cipher;
}

std::string Base::decryptBASE(std::string cipher, byte counter[4]) {
  std::string recovered;

  byte fullIV[16];

  // merge counter and iv into fullIV
  for (int i = 0; i < 12; i++) {
    fullIV[i] = iv[i];
  }
  for (int i = 12; i < 16; i++) {
    fullIV[i] = counter[i - 12];
  }

  try {
    CTR_Mode<AES>::Decryption d;
    d.SetKeyWithIV(Base::arxKey, sizeof(Base::arxKey), fullIV);

    // The StreamTransformationFilter removes
    //  padding as required.
    StringSource s(cipher, true,
                   new StreamTransformationFilter(
                       d,
                       new StringSink(recovered)) // StreamTransformationFilter
    );                                            // StringSource

  } catch (const CryptoPP::Exception &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  return recovered;
}