#ifndef __CLIENT_PROXY_HPP__
#define __CLIENT_PROXY_HPP__

#include "Proxy.hpp"


class ClientProxy: public Proxy {
 protected:
  Packet forward(const Packet &data) override;

 public:
  ClientProxy(unsigned int inPort, unsigned int outPort);
};


#endif
