#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <string>

#include "Exception.hpp"


struct Packet {
  size_t size;
  void* data;

  Packet() = default;
  ~Packet() {
    free(this->data);
  }
  Packet(const Packet& other) noexcept {
    this->size = other.size;
    this->data = malloc(other.size);
    memcpy(this->data, other.data, other.size);
  }
  Packet(Packet&& other) noexcept {
    this->size = other.size;
    this->data = other.data;
    other.data = nullptr;
  }
};


class Socket {
 private:
  constexpr static unsigned int DEFAULT_BACKLOG = 1;
  constexpr static unsigned int DEFAULT_MAX_PACKET_SIZE = 0xff;

  enum Status {
    IDLE,
    LISTENING,
    CONNECTED
  };

  int fd = -1;
  const unsigned short port;
  Status status = IDLE;

  int accept() const;
  int connect(const std::string& host, unsigned short port) const;


 public:
  /**
   * @brief Construct a new Socket object.
   *
   * @param port Port to which bind the socket
   * @throw Socket::InitException
   */
  Socket(unsigned short port);
  ~Socket();


  /* ---------- IN socket ---------- */

  /**
   * @brief Start listening for incoming connections.
   * @note The socket will continue to listen until the socket is destroyed.
   *
   * @param backlog Maximum length of the queue of pending connections
   * @throw Socket::ReceiveException
   */
  void listen(unsigned int backlog = DEFAULT_BACKLOG);

  /**
   * @brief Read data from a peer.
   *
   * @param maxPacketSize Maximum packet size
   * @return Read data
   * @throw Socket::ReceiveException
   */
  Packet receive(int maxPacketSize = DEFAULT_MAX_PACKET_SIZE) const;


  /* ---------- OUT socket ---------- */

  /**
   * @brief Connect to a remote host.
   *
   * @param host Remote hostname or IP address
   * @param port Remote port
   * @throw Socket::SendException
   */
  void connect(const std::string& host, unsigned short port);

  /**
   * @brief Send data to a peer.
   *
   * @throw Socket::SendException
   */
  void send(const Packet &packet) const;


  /* ---------- Exceptions ---------- */

  class SocketException: public CException {
   public:
    SocketException(int nErr) noexcept;
    ~SocketException() noexcept = 0;
  };

  class InitException: public SocketException {
   public:
    InitException(int nErr) noexcept;
    ~InitException() noexcept = default;
  };

  class ReceiveException: public SocketException {
   public:
    ReceiveException(int nErr) noexcept;
    ~ReceiveException() noexcept = default;
  };

  class SendException: public SocketException {
   public:
    SendException(int nErr) noexcept;
    ~SendException() noexcept = default;
  };
};



inline Socket::SocketException::SocketException(int nErr) noexcept: CException(nErr) {}
inline Socket::SocketException::~SocketException() noexcept {}

inline Socket::InitException::InitException(int nErr) noexcept: SocketException(nErr) {}
inline Socket::ReceiveException::ReceiveException(int nErr) noexcept: SocketException(nErr) {}
inline Socket::SendException::SendException(int nErr) noexcept: SocketException(nErr) {}


#endif
