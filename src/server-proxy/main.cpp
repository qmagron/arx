#include <array>
#include <iostream>
#include <set>
#include <vector>

#include "server-proxy/indexes/ArxRange.hpp"
#include "Base.hpp"
#include "crypto/circuits.hpp"
#include "crypto/utils.hpp"


const auto C = COMP<GCN/2>();

/* 128 bit array key of 59E22E9D3351B9B46627F49DA8BF56D4 */
inline byte rangeKey[AES::DEFAULT_KEYLENGTH] = {
  0x59, 0xE2, 0x2E, 0x9D, 0x33, 0x51, 0xB9, 0xB4,
  0x66, 0x27, 0xF4, 0x9D, 0xA8, 0xBF, 0x56, 0xD4
};


/* The database contains documents with random values v that will be indexed. */
std::array<unsigned, 12> database = random_array<unsigned, 12>();


/* The root node is stored on the client side to encode the query */
bool hasRoot = false;
std::array<CipherText<GCK>, GCN/2> rootE;
CipherText<GCK> rootR;


/* Counters are used as nonce. */
std::map<size_t, size_t> garbledCounter;


/**
 * @brief Repair a set of nodes in an ArxRange index.
 * @param[in] index The range index
 * @param[in] nodes The set of nodes to repair
 * @note This function only uses data from the given node.
 */
void repairNodes(ArxRange& index, std::set<ArxRange::Node*>& nodes) {
  std::vector<ArxRange::Node*> vNodes(nodes.begin(), nodes.end());

  // Sort nodes by increasing height
  std::sort(vNodes.begin(), vNodes.end(), [](ArxRange::Node* a, ArxRange::Node* b) {
    return a->height < b->height;
  });

  for (ArxRange::Node* node: vNodes) {
    // Skip leaf nodes
    if (node->height == 1) {
      continue;
    }

    // Retrieve hardcoded value
    size_t nid = node->nid;
    size_t pk = Base::decryptInt(node->pk, 0, rangeKey);
    unsigned v = Base::decryptInt(node->v, pk, rangeKey);

    // Compute nid and nonce for children
    size_t nidL = node->children[0] ? node->children[0]->nid : 0;
    size_t nidR = node->children[1] ? node->children[1]->nid : 0;
    size_t nonceL1 = nidL ? garbledCounter[nidL]-2 : 0;
    size_t nonceR1 = nidR ? garbledCounter[nidR]-2 : 0;
    size_t nonceL2 = nidL ? garbledCounter[nidL]-1 : 0;
    size_t nonceR2 = nidR ? garbledCounter[nidR]-1 : 0;

    // Build new garbled circuits
    BGCC<GCN,GCK> gC1 = generateBGCC<GCN,GCK>(C, nid, garbledCounter[nid]++, nidL, nonceL1, nidR, nonceR1);
    BGCC<GCN,GCK> gC2 = generateBGCC<GCN,GCK>(C, nid, garbledCounter[nid]++, nidL, nonceL2, nidR, nonceR2);

    // Finally repair nodes
    const LightBGCC<GCN,GCK>* lgC[2] = { lightenBGCC(gC1,v), lightenBGCC(gC2,v) };
    index.repairNode(nid, lgC);

    // Update the root garbled circuit information if necessary
    if (node->parent == nullptr) {
      rootE >>= gC1.e;
      rootR = gC1.R;
    }
  }
}


/**
 * @brief Build a range index for testing purposes.
 */
ArxRange buildRangeIndex() {
  ArxRange index;

  // Index documents on the field v
  for (size_t pk = 0; pk != database.size(); ++pk) {
    size_t nid = random_array<size_t,1>()[0];
    size_t v = database[pk];

#if DEBUG >=2
    std::cout << nid << "\t->\t" << v << std::endl;
#endif

    // Build initial garbled circuits
    BGCC<GCN,GCK> gC1 = generateBGCC<GCN,GCK>(C, nid, garbledCounter[nid]++);
    BGCC<GCN,GCK> gC2 = generateBGCC<GCN,GCK>(C, nid, garbledCounter[nid]++);

    // Note that counters must be different for each index and each node.
    // Since there is only one range index, counters are only different for each node.
    // Moreover, since there is only one node per pk, counters are only different for each pk.
    ArxRange::Node* node = new ArxRange::Node {
      .nid = nid,
      .gC = { lightenBGCC(gC1,v), lightenBGCC(gC2,v) },
      .pk = Base::encryptInt(pk, 0, rangeKey),
      .v = Base::encryptInt(v, pk, rangeKey),
    };

    // For the demonstration, the document is inserted by traversing the index.
    // In a real use case, the initial index is built without traversing to prevent
    // garbled circuits from being consumed so there is no need to repair them.
    std::set<ArxRange::Node*> consumedNodes;

    // Encore the hardcoded value as a half garbled input and insert the document
    std::array<CipherText<GCK>, GCN/2> Xa = encode(v, rootE, rootR);

    // Insert the document into the index
    // Note that in the example, there is only one tree so the root is always 0
    index.insertDoc(pk, Base::encryptInt(node->nid, 0, rangeKey), node, 0, Xa, consumedNodes);

    // Repair consumed nodes and those whose children have changed
    repairNodes(index, consumedNodes);

    // Update the root garbled circuit information on the client side
    if (!hasRoot) {
      hasRoot = true;
      rootE >>= gC1.e;
      rootR = gC1.R;
    }
  }

#if DEBUG >= 2
  std::cout << std::endl;
#endif

  return index;
}


/**
 * @brief Select documents in a given range.
 * @param[in] index The range index
 * @param[in] low The lower bound
 * @param[in] high The higher bound
 * @return The list of primary keys of documents in the given range
 */
std::vector<size_t> select(ArxRange& index, size_t low, size_t high) {
  // Encode the query
  std::array<CipherText<GCK>, GCN/2> Xl = encode(low, rootE, rootR);
  std::array<CipherText<GCK>, GCN/2> Xh = encode(high, rootE, rootR);

  // Search for documents between the lower and higher bounds
  std::vector<ArxRange::Node*> out;
  std::set<ArxRange::Node*> consumedNodes;
  index.searchDoc(out, 0, 0, Xl, Xh, consumedNodes);

  // Decrypt the primary keys
  std::vector<size_t> docs;
  for (auto& node: out) {
    size_t pk = Base::decryptInt(node->pk, 0, rangeKey);
    docs.push_back(pk);
  }

  // Repair consumed nodes
  repairNodes(index, consumedNodes);

  return docs;
}


void testArxRange() {
  std::cout << "====================[ ArxRange ]====================" << std::endl;

  ArxRange rangeIndex = buildRangeIndex();

#if DEBUG >= 1
    rangeIndex.print();
    std::cout << std::endl;
#endif

  {
    std::cout << "--------------------[ SELECT * ]--------------------" << std::endl;

    std::vector<size_t> docs = select(rangeIndex, 0, -1);

    // All documents should be fetched
    if (docs.size() == database.size()) {
      std::cout << "Size: OK" << std::endl;
    } else {
      std::cerr << "Size: FAIL" << std::endl;
      std::cerr << "  Expected: " << database.size() << std::endl;
      std::cerr << "  Got: " << docs.size() << std::endl;
      std::cerr << "  Got: "; for (auto& doc: docs) std::cerr << doc << " "; std::cerr << std::endl;
    }
  }
}


void printPreamble() {
  std::cout << "====================[ Database ]====================" << std::endl;
  for (size_t i = 0; i < database.size() / 2; ++i) {
    std::cout << "database[" << i << "] = " << (size_t) database[i];
    std::cout << "  \t\t";
    std::cout << "database[" << database.size()/2 + i << "] = " << (size_t) database[database.size()/2 + i];
    std::cout << std::endl;
  }
  std::cout << std::endl;
}


int main() {
  printPreamble();
  testArxRange();



  // //        a
  // //    b       c
  // //  d   e   f   g
  // // h i j k l m n o

  // for (int _i_ = 0; _i_ < 1000; ++_i_) {
  //   struct Node {
  //     BGCC<GCN,GCK> gC;
  //     Node* children[2];
  //     LightBGCC<GCN,GCK>* lG;
  //     unsigned v;
  //   };

  //   Node h = { .gC = generateBGCC<GCN,GCK>(C,8,8), .children = { nullptr, nullptr } };
  //   Node i = { .gC = generateBGCC<GCN,GCK>(C,9,9), .children = { nullptr, nullptr } };
  //   Node j = { .gC = generateBGCC<GCN,GCK>(C,10,10), .children = { nullptr, nullptr } };
  //   Node k = { .gC = generateBGCC<GCN,GCK>(C,11,11), .children = { nullptr, nullptr } };
  //   Node l = { .gC = generateBGCC<GCN,GCK>(C,12,12), .children = { nullptr, nullptr } };
  //   Node m = { .gC = generateBGCC<GCN,GCK>(C,13,13), .children = { nullptr, nullptr } };
  //   Node n = { .gC = generateBGCC<GCN,GCK>(C,14,14), .children = { nullptr, nullptr } };
  //   Node o = { .gC = generateBGCC<GCN,GCK>(C,15,15), .children = { nullptr, nullptr } };
  //   Node d = { .gC = generateBGCC<GCN,GCK>(C,4,4,8,8,9,9), .children = { &h, &i } };
  //   Node e = { .gC = generateBGCC<GCN,GCK>(C,5,5,10,10,11,11), .children = { &j, &k } };
  //   Node f = { .gC = generateBGCC<GCN,GCK>(C,6,6,12,12,13,13), .children = { &l, &m } };
  //   Node g = { .gC = generateBGCC<GCN,GCK>(C,7,7,14,14,15,15), .children = { &n, &o } };
  //   Node b = { .gC = generateBGCC<GCN,GCK>(C,2,2,4,4,5,5), .children = { &d, &e } };
  //   Node c = { .gC = generateBGCC<GCN,GCK>(C,3,3,6,6,7,7), .children = { &f, &g } };
  //   Node a = { .gC = generateBGCC<GCN,GCK>(C,1,1,2,2,3,3), .children = { &b, &c } };

  //   for (Node* n: { &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o }) {
  //     n->v = random_array<unsigned, 1>()[0];
  //     n->lG = lightenBGCC(n->gC, n->v);
  //   }


  //   std::bitset<GCN/2> A = random_bitset<GCN/2>();

  //   std::array<std::bitset<GCK>, GCN/2> e0;
  //   e0 >>= a.gC.e;

  //   auto Xa = encode(A,e0,a.gC.R);

  //   std::array<CipherText<GCK>, GCN> X;
  //   X >>= Xa;

  //   auto* _ = &a;
  //   size_t depth = 0;

  //   while (_) {
  //     X <<= _->lG->Xv;
  //     auto y = evaluateBGCC(X,C,_->lG->G,_->lG->d,_->lG->T);

  //     if (!y != (A.to_ulong() < _->v)) {
  //       std::cout << "[" << depth << "]: " << A.to_ulong() << " < " << _->v << " != " << !y << std::endl;
  //     }

  //     _ = _->children[y];
  //     ++depth;
  //   }


  //   for (Node* n: { &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o }) {
  //     delete n->lG;
  //   }
  // }



  return 0;
}
