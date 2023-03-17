#ifndef __PROXY_HPP__
#define __PROXY_HPP__

#include <memory>

#include "net/Socket.hpp"


/**
 * @brief An abstract serial proxy class.
 */
class Proxy {
 private:
  std::unique_ptr<Socket> inSocket = {};
  std::unique_ptr<Socket> outSocket = {};

 protected:
  Proxy(unsigned short inPort);
  Proxy(unsigned short inPort, unsigned short outPort);

  /**
   * @brief Connect to the output socket.
   * @note This method allows to override the default connection method.
   *
   * @param remoteHost Host to which connect the output socket
   * @param remotePort Port to which connect the output socket
   * @throw Socket::SendException
   */
  virtual void connect(const std::string &remoteHost, unsigned short remotePort);

  /**
   * @brief Send a data to the remote peer.
   * @note This method allows to override the default send method.
   *
   * @throw Socket::SendException
   */
  virtual void send(const Packet &packet);

  /**
   * @brief Take an input packet and return an output packet.
   *
   * @param inPacket The input packet
   * @return The output packet
   */
  virtual Packet forward(const Packet &inPacket) = 0;


 public:
  /**
   * @brief Start listening for incoming requests.
   * @note This method will never end.
   *
   * @param remotePort Port to which connect the output socket
   * @param remoteHost Host to which connect the output socket
   * @throw Socket::ReceiveException, Socket::SendException
   */
  void start(unsigned short remotePort, const std::string& remoteHost = "127.0.0.1");
};


#endif
