#include "net/Socket.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>


Socket::Socket(unsigned short port): port(port) {
  this->fd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->fd == -1) {
    throw InitException(errno);
  }

  int optval = 1;
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)) == -1) {
    throw InitException(errno);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(this->port);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(this->fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    throw InitException(errno);
  }
}

Socket::~Socket() {
  if (this->status == LISTENING) {
    shutdown(this->fd, SHUT_RDWR);
  }

  close(this->fd);
}


/* ---------- IN socket ---------- */

void Socket::listen(unsigned int backlog) {
  if (::listen(this->fd, backlog) == -1) {
    throw ReceiveException(errno);
  } else {
    this->status = LISTENING;
  }
}

int Socket::accept() const {
  struct pollfd pfd = {this->fd, POLLIN, 0};

  // Wait for incoming connection
  if (poll(&pfd, 1, -1) == -1) {
    // If peer closed the connection, then close the socket
    if (errno != POLLHUP || shutdown(this->fd, SHUT_RDWR) == -1 || close(this->fd) == -1) {
      throw ReceiveException(errno);
    }
  }

  // Accept the incoming connection
  int peer = accept4(this->fd, NULL, NULL, SOCK_NONBLOCK);
  if (peer == -1) {
    throw ReceiveException(errno);
  }

  return peer;
}

Packet Socket::receive(int maxPacketSize) const {
  int peer = this->accept();

  Packet packet;

  // First, receive the packet size
  int n = recv(peer, &packet.size, sizeof(packet.size), 0);
  if (n == -1) {
    throw ReceiveException(errno);
  }

  if (packet.size > maxPacketSize) {
    packet.size = maxPacketSize;
  }

  packet.data = malloc(packet.size);

  // Then, receive packet data
  n = recv(peer, packet.data, packet.size, 0);
  if (n == -1) {
    throw ReceiveException(errno);
  }

  return packet;
}


/* ---------- OUT socket ---------- */

void Socket::connect(const std::string& host, unsigned short port) {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(host.c_str());

  if (::connect(this->fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    throw SendException(errno);
  }
}

void Socket::send(const void *data, size_t dataSize) const {
  // First, send the packet size
  int n = ::send(this->fd, &dataSize, sizeof(dataSize), 0);
  if (n == -1) {
    throw SendException(errno);
  }

  // Then, send packet data
  n = ::send(this->fd, data, dataSize, 0);
  if (n == -1) {
    throw SendException(errno);
  }
}
