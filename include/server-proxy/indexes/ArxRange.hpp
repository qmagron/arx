#ifndef __ARX_RANGE_HPP__
#define __ARX_RANGE_HPP__

#include <cryptopp/cryptlib.h>
#include <array>
#include <map>
#include <set>

#include "crypto/bgcc.hpp"


#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif

template<size_t n>
using Cipher = std::array<byte, n>;
using EDoc = size_t;  // TODO

constexpr size_t N_ROOTS = 1;


class ArxRange {
 public:
  struct Node {
    const size_t nid;
    const LightBGCC<GCN,GCK>* gC[2];
    const Cipher<16> v;
    const Cipher<16> pk;

    Node* children[2] = { nullptr, nullptr };
    Node* parent = nullptr;
    size_t height = 1;

    inline ~Node();
  };

 private:
  std::map<size_t, Node*> nodes;
  std::map<size_t, const EDoc> nodeToDoc;
  std::map<size_t, Cipher<16>> docToNode;

  /**
   * @brief Traverse a node of the index.
   * @param[in,out] node The node to traverse
   * @param[in] Xa The half garbled input
   * @param[in] i The garbled circuit index
   * @param[in,out] path The path to the node (will be updated)
   * @return The direction
   */
  bool traverse(Node*& node, const std::array<CipherText<GCK>, GCN/2>& Xa, bool i, std::set<Node*>& path);

  /**
   * @brief Get the next node of the index.
   * @param[in] node The initial node
   * @return The next node
   */
  Node* next(Node* node) const;

  /**
   * @brief Rotate a node to the right.
   * @param[in] node The node to rotate
   * @param[in,out] N Nodes to repair
   * @return The new root node
   */
  Node* rotateRight(Node* node, std::set<Node*>& N);

  /**
   * @brief Rotate a node to the left.
   * @param[in] node The node to rotate
   * @param[in,out] N Nodes to repair
   * @return The new root node
   */
  Node* rotateLeft(Node* node, std::set<Node*>& N);

  /**
   * @brief Remove a node from the index.
   * @param[in] node The node to remove
   * @param[in,out] N Nodes to repair
   * @return The new root node
   */
  Node* remove(Node* node, std::set<Node*>& N);

  /**
   * @brief Rebalance a node of the index.
   * @param[in] node The node to rebalance
   * @param[in,out] N Nodes to repair
   * @return The new root node
   */
  Node* rebalance(Node* node, std::set<Node*>& N);

 public:
  inline ArxRange() = default;
  inline ~ArxRange() = default;

  /**
   * @brief Search for documents in the index.
   * @param[out] out The matching documents
   * @param[in] rootL The root node from which to search for the lower bound
   * @param[in] rootH The root node from which to search for the higher bound
   * @param[in] Xl The garbled input for the lower bound
   * @param[in] Xh The garbled input for the higher bound
   * @param[in,out] N Nodes to repair
   */
  void searchDoc(std::vector<Node*>& out, size_t rootL, size_t rootH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh, std::set<Node*>& N);

  /**
   * @brief Repair a node of the index.
   * @param[in] nid The node to repair
   * @param[in] gC The new garbled circuitS
   */
  void repairNode(size_t nid, const LightBGCC<GCN,GCK>* gC[2]);

  /**
   * @brief Insert a document in the index.
   * @param[in] docID The document ID
   * @param[in] eID The encrypted document ID
   * @param[in] eNID The encrypted node ID
   * @param[in] newNode The new node to insert
   * @param[in] rootNID The root node ID
   * @param[in] X The garbled input of the hardcoded value
   * @param[in,out] N Nodes to repair
   */
  void insertDoc(size_t docID, const Cipher<16>& eID, const Cipher<16>& eNID, Node* newNode, size_t rootNID, const std::array<CipherText<GCK>, GCN/2>& X, std::set<Node*>& N);

  /**
   * @brief Delete a document from the index.
   * @param[out] eDocs The encrypted document IDs of the documents to delete
   * @param[in] rootL The root node from which to search for the lower bound
   * @param[in] rootH The root node from which to search for the higher bound
   * @param[in] Xl The garbled input for the lower bound
   * @param[in] Xh The garbled input for the higher bound
   * @param[in,out] N Nodes to repair
   */
  void deleteDoc(std::set<Cipher<16>>& eDocs, size_t rootL, size_t rootH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh, std::set<Node*>& N);

  /**
   * @brief Delete a document from the index.
   * @param[in] docID The document ID
   * @param[in,out] N Nodes to repair
   * @return The encrypted node ID
   */
  Cipher<16> deleteID(EDoc docID, std::set<Node*>& N);

  /**
   * @brief Delete a node from the index.
   * @param[in] nid The node ID
   * @param[in,out] N Nodes to repair
   */
  void deleteNode(size_t nid, std::set<Node*>& N);
};


inline ArxRange::Node::~Node() {
  delete children[0];
  delete children[1];
}


#endif
