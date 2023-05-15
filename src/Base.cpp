#include "Base.hpp"

#include <cryptopp/ccm.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

#include "queries/GenericQuery.hpp"

#include <boost/algorithm/string.hpp>

using CryptoPP::CBC_Mode;

using CryptoPP::StreamTransformationFilter;
using CryptoPP::StringSink;
using CryptoPP::StringSource;


#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif

/*
  * @brief Encrypt a string using AES
  * @param[in] plain The data to encrypt read as a string
  * @param[in] counter The counter to use for the encryption
  * @return The encrypted string
  * @note The string is padded to 64 bytes
  * @note The counter is 4 bytes long allowing for 4,294,967,295 different counter values
*/
Cipher<16> Base::encryptBASE(std::string plain, byte counter[4], byte key[16]) {
  // TODO support longer size
  Cipher<16> cipher = {0};
  byte fullIV[16];

  // merge counter and iv into fullIV
  for (int i = 0; i < 12; i++) {
    fullIV[i] = iv[i];
  }
  for (int i = 12; i < 16; i++) {
    fullIV[i] = counter[i - 12];
  }

  try {

    CBC_Mode<AES>::Encryption enc;
    enc.SetKeyWithIV(key, 16, fullIV, 16);

    CryptoPP::ArraySink cs(cipher.data(), cipher.size());
    CryptoPP::StringSource(plain, true, new CryptoPP::StreamTransformationFilter(enc, new CryptoPP::Redirector(cs)));

  } catch (const CryptoPP::Exception& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  return cipher;
}

std::string Base::decryptBASE(Cipher<16> cipher, byte counter[4], byte key[16]) {
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

    CBC_Mode<AES>::Decryption dec;
    dec.SetKeyWithIV(key, 16, fullIV, 16);

    StringSink cs(recovered);
    CryptoPP::ArraySource(cipher.data(), cipher.size(), true, new StreamTransformationFilter(dec, new CryptoPP::Redirector(cs)));

  } catch (const CryptoPP::Exception &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  return boost::algorithm::trim_copy(recovered);
}
