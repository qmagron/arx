#include "client/ClientProxy.hpp"


ClientProxy::ClientProxy(unsigned int inPort, unsigned int outPort): Proxy(inPort, outPort) {}


Packet ClientProxy::forward(const Packet &data) {
  return data;
}
