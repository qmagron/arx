#ifndef __SERVER_PROXY_HPP__
#define __SERVER_PROXY_HPP__

#include "Proxy.hpp"


class ServerProxy: public Proxy {
 protected:
  Packet forward(const Packet &inPacket) override;

  void connect(const std::string &remoteHost, unsigned short remotePort) override;
  void send(const Packet &packet) override;

 public:
  ServerProxy(unsigned short inPort);
};


#endif
