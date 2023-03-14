#include "Proxy.hpp"


Proxy::Proxy(unsigned int inPort, unsigned int outPort): inSocket(inPort), outSocket(outPort) {}


void Proxy::start(const std::string& remoteHost, unsigned short remotePort) {
  this->inSocket.listen();
  this->outSocket.connect(remoteHost, remotePort);


  for (;;) {
    Packet inPacket = this->inSocket.receive();
    Packet outPacket = this->forward(inPacket);
    this->outSocket.send(outPacket);
  }
}
