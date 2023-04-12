#ifndef __CLIENT_PROXY_HPP__
#define __CLIENT_PROXY_HPP__

#include "Proxy.hpp"
#include <cryptopp/aes.h>
#include <map>
#include <vector>

#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif

class ClientProxy : public Proxy {
private:
  byte fieldEncryptionKey[16] = {0x59, 0xE2, 0x2E, 0x9D, 0x33, 0x51, 0xB9, 0xB4,
                          0x66, 0x27, 0xF4, 0x9D, 0xA8, 0xBF, 0x56, 0xD4};
  std::map<std::string, std::map<std::string, byte[4]>> *value_counters;
  std::map<std::string, std::map<std::string, byte[16]>> field_keys;

protected:
  Packet forward(const Packet &inPacket) override;

public:
  ClientProxy(unsigned short inPort, unsigned short outPort);
  ~ClientProxy() = default;
};

#endif
