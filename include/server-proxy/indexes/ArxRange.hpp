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

constexpr size_t N_ROOTS = 1;


class ArxRange {
 public:
  struct Node {
    const size_t nid;
    const LightBGCC<GCN,GCK>* gC[2];
    const Cipher<16> v;
    const Cipher<16> pk;
    Node* children[2] = { nullptr, nullptr };

    inline ~Node();
  };

 private:
  std::map<size_t, Node*> nodes;
  // std::map<size_t, const EDoc> nodeToDoc;
  std::map<size_t, Cipher<16>> docToNode;

  /**
   * @brief Traverse a node of the index.
   * @param[in,out] node The node to traverse
   * @param[in] Xa The half garbled input
   * @param[in] i The garbled circuit index
   * @param[in,out] path The path to the node (will be updated)
   * @return The direction
   */
  bool traverse(Node*& node, const std::array<CipherText<GCK>, GCN/2>& Xa, bool i, std::vector<Node*>& path);

 public:
  inline ArxRange() = default;
  inline ~ArxRange() = default;

  /**
   * @brief Search for documents in the index.
   * @param[out] out The encrypted primary keys of the found documents
   * @param[in] nidL The root node on which to search for the lower bound
   * @param[in] nidH The root node on which to search for the higher bound
   * @param[in] Xl The garbled input for the lower bound
   * @param[in] Xh The garbled input for the higher bound
   * @param[in] N Nodes to repair
   */
  void searchDoc(std::set<Cipher<16>>& out, size_t nidL, size_t nidH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh, std::vector<Node*>& N);

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
   * @param[out] N Nodes to repair
   */
  void insertDoc(size_t docID, const Cipher<16>& eID, const Cipher<16>& eNID, Node* newNode, size_t rootNID, const std::array<CipherText<GCK>, GCN/2>& X, std::vector<Node*>& N);

  /**
   * @brief Delete a document from the index.
   * @param[in] nidL The root node on which to search for the lower bound
   * @param[in] nidH The root node on which to search for the higher bound
   * @param[in] Xl The garbled input for the lower bound
   * @param[in] Xh The garbled input for the higher bound
   * @param[out] N Nodes to repair
   */
  void deleteDoc(size_t nidL, size_t nidH, const std::array<CipherText<GCK>, GCN/2>& Xl, const std::array<CipherText<GCK>, GCN/2>& Xh, std::vector<Node*>& N);

  /**
   * @brief Delete a document from the index.
   * @param[in] docID The document ID
   * @param[out] N Nodes to repair
   */
  void deleteID(size_t docID, std::vector<Node*>& N);
};


#endif
