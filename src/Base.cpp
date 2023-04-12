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
  * @param[in] message The data to encrypt read as a string
  * @param[in] counter The counter to use for the encryption
  * @return The encrypted string
  * @note The string is padded to 64 bytes
  * @note The counter is 4 bytes long allowing for 4,294,967,295 different counter values
*/

std::string Base::encryptBASE(std::string message, byte counter[4]) {


  message.resize(64);
  std::string cipher;
  std::string encoded;

  byte fullIV[16];

  // merge counter and iv into fullIV
  for (int i = 0; i < 12; i++) {
    fullIV[i] = iv[i];
  }
  for (int i = 12; i < 16; i++) {
    fullIV[i] = counter[i - 12];
  }

  try {

    CBC_Mode<AES>::Encryption e;
    e.SetKeyWithIV(Base::arxKey, sizeof(Base::arxKey), fullIV);

    
    StringSource(message, true,
                 new StreamTransformationFilter(
                     e,
                     new StringSink(cipher)) 
    );                             

    // encode the encrypted string into a readable hex string        
    encoded.clear();
    StringSource(cipher, true,
		new CryptoPP::HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	);
  
  } catch (const CryptoPP::Exception &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  return encoded;
}

std::string Base::decryptBASE(std::string cipher, byte counter[4]) {
  std::string recovered;
  std::string encoded;


  byte fullIV[16];

  // merge counter and iv into fullIV
  for (int i = 0; i < 12; i++) {
    fullIV[i] = iv[i];
  }
  for (int i = 12; i < 16; i++) {
    fullIV[i] = counter[i - 12];
  }

  try {
    CBC_Mode<AES>::Decryption d;
    d.SetKeyWithIV(Base::arxKey, sizeof(Base::arxKey), fullIV);

    
    // decode the ciphertext into usable data
    StringSource(cipher, true,new CryptoPP::HexDecoder(new StringSink(encoded)));
    // decrypt the data
    recovered.clear();
    StringSource s(encoded, true,
                   new StreamTransformationFilter(
                       d,
                       new StringSink(recovered))
    );                                           

  } catch (const CryptoPP::Exception &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  return boost::algorithm::trim_copy(recovered);
}