#ifndef __CLIENT_PROXY_HPP__
#define __CLIENT_PROXY_HPP__

#include "Proxy.hpp"
#include <cryptopp/aes.h>
#include <map>


#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif


class ClientProxy : public Proxy {
private:
  std::map<std::string, std::map<std::string, int>> *value_counters;
  std::map<std::string, std::array<byte, 16>> field_keys;
  std::map<std::string, std::string> field_encryptedname;

protected:
  Packet forward(const Packet &inPacket) override;

public:
  ClientProxy(unsigned short inPort, unsigned short outPort);
};

#endif
