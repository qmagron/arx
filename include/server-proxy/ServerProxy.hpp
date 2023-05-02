#ifndef __SERVER_PROXY_HPP__
#define __SERVER_PROXY_HPP__

#include <array>
#include <vector>

#include "Proxy.hpp"
#include "indexes/ArxRange.hpp"


class ServerProxy: public Proxy {
 private:
  std::vector<ArxRange> rangeIndexes;

  /**
   * @brief Delete a range of documents from an index.
   * @param[in] idx The index identifier
   * @param[in] rootL The root node from which to search for the lower bound
   * @param[in] rootH The root node from which to search for the higher bound
   * @param[in] Xl The half garbled input for the lower bound
   * @param[in] Xh The half garbled input for the upper bound
   */
  void rangeDelete(size_t idx, size_t rootL, size_t rootH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh);

 protected:
  Packet forward(const Packet& inPacket) override;

  void connect(const std::string& remoteHost, unsigned short remotePort) override;
  void send(const Packet& packet) override;

 public:
  ServerProxy(unsigned short inPort);
};


#endif
