#include "server-proxy/indexes/ArxRange.hpp"

#include <iostream>

#include "crypto/circuits.hpp"


const auto C = COMP<GCN/2>();


bool ArxRange::traverse(Node*& node, const std::array<CipherText<GCK>, GCN/2>& Xa, bool i, ConsumedNodes& path) {
  std::array<CipherText<GCK>, GCN> X;
  X >>= Xa;

  Node* nextNode = node;

  bool y;
  do {
    node = nextNode;

    auto& gC = node->gC[i];
    X <<= gC->Xv;

    y = evaluateBGCC(X, C, gC->G, gC->d, gC->T);

    path.push(node);
    nextNode = node->children[!y];
  } while (nextNode);

  return y;
}


ArxRange::Node* ArxRange::next(Node* node) const {
  if (node->children[1]) {
    node = node->children[1];
    while (node->children[0]) {
      node = node->children[0];
    }
  } else {
    while (node->parent && node->parent->children[1] == node) {
      node = node->parent;
    }
    node = node->parent;
  }

  return node;
}


ArxRange::Node* ArxRange::rotateRight(Node* node, ConsumedNodes& N) {
  // Update pointers
  Node* left = node->children[0];
  node->children[0] = left->children[1];
  left->children[1] = node;

  // Update heights
  node->height = std::max(node->children[0] ? node->children[0]->height : 0, node->children[1] ? node->children[1]->height : 0) + 1;
  left->height = std::max(left->children[0] ? left->children[0]->height : 0, left->children[1] ? left->children[1]->height : 0) + 1;

  // Mark nodes whose children have changed as consumed
  N.push(node);
  N.push(left);

  return left;
}

ArxRange::Node* ArxRange::rotateLeft(Node* node, ConsumedNodes& N) {
  // Update pointers
  Node* right = node->children[1];
  node->children[1] = right->children[0];
  right->children[0] = node;

  // Update heights
  node->height = std::max(node->children[0] ? node->children[0]->height : 0, node->children[1] ? node->children[1]->height : 0) + 1;
  right->height = std::max(right->children[0] ? right->children[0]->height : 0, right->children[1] ? right->children[1]->height : 0) + 1;

  // Mark nodes whose children have changed as consumed
  N.push(node);
  N.push(right);

  return right;
}


void ArxRange::rebalance(Node* node, ConsumedNodes& N) {
  size_t leftHeight = node->children[0] ? node->children[0]->height : 0;
  size_t rightHeight = node->children[1] ? node->children[1]->height : 0;

  // If unbalanced to right
  if (rightHeight > leftHeight && rightHeight - leftHeight > 1) {
    size_t leftHeight = node->children[1]->children[0] ? node->children[1]->children[0]->height : 0;
    size_t rightHeight = node->children[1]->children[1] ? node->children[1]->children[1]->height : 0;

    // If right child is unbalanced to left
    if (rightHeight - leftHeight < 0) {
      node->children[1] = this->rotateRight(node->children[1], N);
    }

    this->rotateLeft(node, N);
  }

  // If unbalanced to left
  else if (leftHeight > rightHeight && leftHeight - rightHeight > 1) {
    size_t leftHeight = node->children[0]->children[0] ? node->children[0]->children[0]->height : 0;
    size_t rightHeight = node->children[0]->children[1] ? node->children[0]->children[1]->height : 0;

    // If left child is unbalanced to right
    if (rightHeight - leftHeight < 0) {
      node->children[0] = this->rotateLeft(node->children[0], N);
    }

    this->rotateRight(node, N);
  }
}


ArxRange::Node* ArxRange::remove(Node* node, ConsumedNodes& N) {
  Node* newRoot = node->children[0];

  while (node->children[0]) {
    this->rotateRight(node, N);
  }

  return newRoot;
}


void ArxRange::searchDoc(std::vector<Node*>& out, size_t treeL, size_t treeH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh, ConsumedNodes& N) {
  Node* nodeL = this->root[treeL];
  Node* nodeH = this->root[treeH];

  this->traverse(nodeL, Xl, 0, N);
  this->traverse(nodeH, Xh, 1, N);

  // Get the nodes in (nodeL, nodeH]
  while (nodeL && nodeL != nodeH) {
    out.push_back(nodeL);
    nodeL = this->next(nodeL);
  }
}


void ArxRange::repairNode(size_t nid, const LightBGCC<GCN,GCK>* gC[2]) {
  Node* node = this->nodes[nid];

  delete node->gC[0];
  delete node->gC[1];

  node->gC[0] = gC[0];
  node->gC[1] = gC[1];
}


void ArxRange::insertDoc(size_t docID, const Cipher<32>& eNID, Node* newNode, size_t tree, const std::array<CipherText<GCK>, GCN/2>& X, ConsumedNodes& N) {
  this->nodes.insert_or_assign(newNode->nid, newNode);
  this->nodeToDoc.insert_or_assign(newNode->nid, newNode->pk);
  this->docToNode.insert_or_assign(docID, eNID);

  if (this->root[tree] == nullptr) {
    this->root[tree] = newNode;
  } else {
    Node* node = this->root[tree];
    bool dir = this->traverse(node, X, 0, N);

    // Update pointers
    node->children[!dir] = newNode;
    newNode->parent = node;

    // Update parent height if necessary
    if (node->height == 1) {
      node->height = 2;
    }

    while (node) {
      this->rebalance(node, N);
      node = node->parent;
    }
  }
}


void ArxRange::deleteDoc(std::set<Cipher<32>>& out, size_t treeL, size_t treeH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh, ConsumedNodes& N) {
  Node* nodeL = this->root[treeL];
  Node* nodeH = this->root[treeH];

  this->traverse(nodeL, Xl, 0, N);
  this->traverse(nodeH, Xh, 1, N);

  // Get the nodes in (nodeL, nodeH]
  std::vector<Node*> S;
  while (nodeL != nodeH) {
    nodeL = this->next(nodeL);
    S.push_back(nodeL);
  }

  for (Node* node: S) {
    out.insert(node->pk);
    this->nodes.erase(node->nid);
    this->nodeToDoc.erase(node->nid);
    this->remove(node, N);
    N.remove(node);
    delete node;
  }
}


Cipher<32> ArxRange::deleteID(EDoc docID, ConsumedNodes& N) {
  auto it = this->docToNode.find(docID);
  Cipher<32> eNID = it->second;
  this->docToNode.erase(it);
  return eNID;
}


void ArxRange::deleteNode(size_t nid, ConsumedNodes& N) {
  Node* node = this->nodes[nid];
  this->nodes.erase(nid);
  this->nodeToDoc.erase(nid);
  this->remove(node, N);
  delete node;
}


#ifdef DEBUG
void ArxRange::print() const {
  for (auto& [nid, node]: this->nodes) {
    std::cout << nid << ": ";

    if (node->children[0]) {
      std::cout << node->children[0]->nid << " (L) ";
    }

    if (node->children[1]) {
      std::cout << node->children[1]->nid << " (R)";
    }

    std::cout << std::endl;
  }
}
#endif
