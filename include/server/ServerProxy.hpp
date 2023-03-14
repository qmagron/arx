#ifndef __SERVER_PROXY_HPP__
#define __SERVER_PROXY_HPP__

#include "Proxy.hpp"


class ServerProxy: public Proxy {
 protected:
  Packet forward(const Packet &data) override;

 public:
  ServerProxy(unsigned int inPort, unsigned int outPort);
};


#endif
