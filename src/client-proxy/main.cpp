#include <iostream>
#include <string>

#include "Base.hpp"
#include "client-proxy/ClientProxy.hpp"
#include "queries/DeleteQuery.hpp"
#include "queries/GenericQuery.hpp"

constexpr unsigned int IN_PORT = 1235;
constexpr unsigned int OUT_PORT = 1236;
constexpr unsigned int REMOTE_PORT = 1237;

int main() {

  ClientProxy client(IN_PORT, OUT_PORT);
  client.start(REMOTE_PORT);

  /*
  byte counter[4] = { 0x00, 0x00, 0x00, 0x00 };
  byte key[16] = {0x59, 0xE2, 0x2E, 0x9D, 0x33, 0x51, 0xB9, 0xB4,
    0x66, 0x27, 0xF4, 0x9D, 0xA8, 0xBF, 0x56, 0xD4};
  std::string cyper = Base::encryptBASE("caca",counter,key);
  std::cout << cyper << std::endl;
  std:: cout << Base::decryptBASE(cyper,counter,key) << std::endl;
  */
  return 0;
}
