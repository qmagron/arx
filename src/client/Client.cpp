#include <iostream>

#include "client/Client.hpp"


Client::Client(unsigned short outPort, unsigned short remotePort, const std::string &remoteHost){//}: outSocket(new Socket(outPort)) {
  this->outSocket->connect(remoteHost, remotePort);
}


void Client::send(const Packet &packet) const {
  this->outSocket->send(packet);
}


void Client::start() const {
  for (;;) {
    std::cout << "> ";

    std::string str;
    getline(std::cin, str);

    this->send(Packet(str));
  }
}
