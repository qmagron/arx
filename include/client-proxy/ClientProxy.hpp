#ifndef __CLIENT_PROXY_HPP__
#define __CLIENT_PROXY_HPP__

#include "Proxy.hpp"


class ClientProxy: public Proxy {
 protected:
  Packet forward(const Packet &inPacket) override;

 public:
  ClientProxy(unsigned short inPort, unsigned short outPort);
};


#endif
