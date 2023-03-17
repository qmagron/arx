#include "Proxy.hpp"


Proxy::Proxy(unsigned short inPort): inSocket(new Socket(inPort)) {}
Proxy::Proxy(unsigned short inPort, unsigned short outPort): inSocket(new Socket(inPort)), outSocket(new Socket(outPort)) {}


void Proxy::connect(const std::string &remoteHost, unsigned short remotePort) {
  this->outSocket->connect(remoteHost, remotePort);
}

void Proxy::send(const Packet &packet) {
  this->outSocket->send(packet);
}

void Proxy::start(unsigned short remotePort, const std::string &remoteHost) {
  this->inSocket->listen();
  this->connect(remoteHost, remotePort);

  for (;;) {
    Packet inPacket = this->inSocket->receive();
    Packet outPacket = this->forward(inPacket);
    this->send(outPacket);
  }
}
