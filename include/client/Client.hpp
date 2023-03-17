#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <memory>

#include "net/Socket.hpp"


class Client {
 private:
  std::unique_ptr<Socket> outSocket = {};

  void send(const Packet &packet) const;


 public:
  Client(unsigned short outPort, unsigned short remotePort, const std::string &remoteHost = "127.0.0.1");

  /**
   * @brief Start the client command prompt.
   */
  void start() const;
};


#endif
