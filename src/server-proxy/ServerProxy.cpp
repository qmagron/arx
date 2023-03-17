#include "server-proxy/ServerProxy.hpp"


ServerProxy::ServerProxy(unsigned int inPort, unsigned int outPort): Proxy(inPort, outPort) {}


Packet ServerProxy::forward(const Packet &data) {
  return data;
}
