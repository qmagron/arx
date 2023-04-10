#include "server-proxy/indexes/ArxRange.hpp"

#include "crypto/circuits.hpp"


const auto C = COMP<GCN/2>();


ArxRange::Node::~Node() {
  delete children[0];
  delete children[1];
}


bool ArxRange::traverse(Node*& node, const std::array<CipherText<GCK>, GCN/2>& Xa, bool i, std::vector<Node*>& path) {
  std::array<CipherText<GCK>, GCN> X;
  X >>= Xa;

  bool y;
  while (node) {
    auto& gC = node->gC[i];
    X <<= gC->Xv;

    y = evaluateBGCC<GCN,GCK,0,GCN/2>(X, C, gC->G, gC->d, gC->T);

    path.push_back(node);
    node = node->children[y];
  }

  return y;
}


void ArxRange::searchDoc(std::set<Cipher<16>>& out, size_t nidL, size_t nidH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh, std::vector<Node*>& N) {
  Node* nodeL = this->nodes[nidL];
  Node* nodeH = this->nodes[nidH];

  this->traverse(nodeL, Xl, 0, N);
  this->traverse(nodeH, Xh, 1, N);

  std::vector<Node*> S;
  // TODO In-order traversal of index from nodeL to nodeH, and collect the intermediate nodes in S

  for (Node* node: S) {
    out.insert(node->pk);
  }
}


void ArxRange::repairNode(size_t nid, const LightBGCC<GCN,GCK>* gC[2]) {
  Node* node = this->nodes[nid];

  delete node->gC[0];
  delete node->gC[1];

  node->gC[0] = gC[0];
  node->gC[1] = gC[1];
}


void ArxRange::insertDoc(size_t docID, const Cipher<16>& eID, const Cipher<16>& eNID, Node* newNode, size_t rootNID, const std::array<CipherText<GCK>, GCN/2>& X, std::vector<Node*>& N) {
  size_t nid = newNode->nid;
  this->docToNode.insert_or_assign(docID, eNID);
  this->nodes.insert_or_assign(nid, newNode);

  Node* node = this->nodes[rootNID];
  bool dir = this->traverse(node, X, 0, N);
  node->children[dir] = newNode;

  // TODO rebalance the tree
}


void ArxRange::deleteDoc(size_t nidL, size_t nidH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh, std::vector<Node*>& N) {
  Node* nodeL = this->nodes[nidL];
  Node* nodeH = this->nodes[nidH];

  this->traverse(nodeL, Xl, 0, N);
  this->traverse(nodeH, Xh, 1, N);

  std::vector<Node*> S;
  // TODO In-order traversal of index from nodeL to nodeH, and collect the intermediate nodes in S

  std::set<Cipher<16>> pkSet;
  for (Node* node: S) {
    pkSet.insert(node->pk);
    this->nodes.erase(node->nid);
    delete node;
    N.erase(std::find(N.begin(), N.end(), node));
  }

  // TODO send pkSet to the client for decryption
  // TODO remove from docToNode
  // TODO adjust node pointers
  // TODO rebalance the tree
  // TODO update N
}


void ArxRange::deleteID(size_t docID, std::vector<Node*>& N) {
  // TODO send this->docToNode[docID] to the client for decryption
  // TODO remove from docToNode
  // TODO remove from index
  // TODO Adjust node pointers
  // TODO rebalance the tree
  // TODO update N
}
