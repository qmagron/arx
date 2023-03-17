#include "client-proxy/ClientProxy.hpp"


ClientProxy::ClientProxy(unsigned short inPort, unsigned short outPort): Proxy(inPort, outPort) {}


Packet ClientProxy::forward(const Packet &inPacket) {
  return inPacket;
}
