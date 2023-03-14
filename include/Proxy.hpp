#ifndef __PROXY_HPP__
#define __PROXY_HPP__

#include "net/Socket.hpp"


/**
 * @brief An abstract serial proxy class.
 */
class Proxy {
 private:
  Socket inSocket;
  Socket outSocket;

 protected:
  Proxy(unsigned int inPort, unsigned int outPort);

  /**
   * @brief Take an input packet and return an output packet.
   */
  virtual Packet forward(const Packet &data) = 0;

 public:
  /**
   * @brief Start listening for incoming requests.
   * @note This method will never end.
   *
   * @param remoteHost Host to which connect the output socket
   * @param remotePort Port to which connect the output socket
   * @throw Socket::ReceiveException
   */
  void start(const std::string& remoteHost, unsigned short remotePort);
};


#endif
