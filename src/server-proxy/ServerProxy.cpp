#include "server-proxy/ServerProxy.hpp"

#include <set>


ServerProxy::ServerProxy(unsigned short inPort): Proxy(inPort) {}


void ServerProxy::connect(const std::string& remoteHost, unsigned short remotePort) {
  // TODO
}

void ServerProxy::send(const Packet& packet) {
  // TODO
}


Packet ServerProxy::forward(const Packet& inPacket) {
  return inPacket;
}


void ServerProxy::rangeDelete(size_t idx, size_t rootL, size_t rootH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh) {
  ArxRange& index = this->rangeIndexes[idx];

  std::set<ArxRange::Node*> consumedNodes;
  std::set<Cipher<16>> eDocs;
  index.deleteDoc(eDocs, rootL, rootH, Xl, Xh, consumedNodes);

  // TODO send encrypted pk to the client-proxy
  // TODO receive pk from the client-proxy
  // TODO delete pk from the database

  std::set<EDoc> docs;
  for (auto& pk: docs) {
    index.deleteID(pk, consumedNodes);
  }

  this->repairNodes(index, consumedNodes);
}

void ServerProxy::rangeDeleteID(size_t idx, const EDoc& pk) {
  ArxRange& index = this->rangeIndexes[idx];

  std::set<ArxRange::Node*> consumedNodes;
  Cipher<16> eNid = index.deleteID(pk, consumedNodes);

  // TODO send encrypted nid to the client-proxy
  // TODO receive nid from the client-proxy

  size_t nid;
  index.deleteNode(nid, consumedNodes);

  this->repairNodes(index, consumedNodes);
}

void ServerProxy::repairNodes(ArxRange& index, const std::set<ArxRange::Node*>& N) {
  Packet headerPacket(N.size());
  this->send(headerPacket);

  for (ArxRange::Node* node: N) {
    // TODO send node to the client-proxy
    // TODO receive repaired node from the client-proxy
    // TODO repair node
  }
}
