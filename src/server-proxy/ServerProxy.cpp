#include "server-proxy/ServerProxy.hpp"


ServerProxy::ServerProxy(unsigned short inPort): Proxy(inPort) {}


void ServerProxy::connect(const std::string &remoteHost, unsigned short remotePort) {
  // TODO
}

void ServerProxy::send(const Packet &packet) {
  // TODO
}


Packet ServerProxy::forward(const Packet &inPacket) {
  return inPacket;
}
