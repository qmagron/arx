#include <array>
#include <iostream>

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
std::array<size_t, 12> database = random_array<size_t, 12>();


/* The root node is stored on the client side to encode the query */
size_t rootNode = 0;
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
void repairNodes(ArxRange& index, const std::set<ArxRange::Node*>& nodes) {
  for (auto& node: nodes) {
    // Retrieve hardcoded value
    size_t nid = node->nid;
    size_t pk = Base::decryptInt(node->pk, 0, rangeKey);
    size_t v = Base::decryptInt(node->v, pk, rangeKey);

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
    if (nid == rootNode) {
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
    size_t v = database[pk];
    size_t nid = static_cast<size_t>(rand());

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
    std::array<CipherText<GCK>, GCN/2> Xa = encode(CipherText<GCN/2>(v), rootE, rootR);
    index.insertDoc(pk, Base::encryptInt(node->nid, 0, rangeKey), node, rootNode, Xa, consumedNodes);

    // Repair consumed nodes and those whose children have changed
    repairNodes(index, consumedNodes);

    // Update the root garbled circuit information on the first insertion
    // These informations are stored on the client side
    if (rootNode == 0) {
      rootNode = node->nid;
      rootE >>= gC1.e;
      rootR = gC1.R;
    }
  }

  return index;
}

int main() {
  ArxRange rangeIndex = buildRangeIndex();


  // std::array<CipherText<GCK>, GCN> _e0_;
  // CipherText<GCK> _R0_ = random_bitset<GCK>();

  // //        a
  // //    b       c
  // //  d   e   f   g
  // // h i j k l m n o

  // for (int _i_ = 0; _i_ < 1000; ++_i_) {
  //   struct Node {
  //     BGCC<GCN,GCK> gC;
  //     Node* children[2];
  //     LightBGCC<GCN,GCK> lG;
  //     CipherText<GCN/2> v;
  //   };

  //   Node h = { .gC = generateBGCC(C, _e0_, _R0_, _e0_, _R0_), .children = { nullptr, nullptr } };
  //   Node i = { .gC = generateBGCC(C, _e0_, _R0_, _e0_, _R0_), .children = { nullptr, nullptr } };
  //   Node j = { .gC = generateBGCC(C, _e0_, _R0_, _e0_, _R0_), .children = { nullptr, nullptr } };
  //   Node k = { .gC = generateBGCC(C, _e0_, _R0_, _e0_, _R0_), .children = { nullptr, nullptr } };
  //   Node l = { .gC = generateBGCC(C, _e0_, _R0_, _e0_, _R0_), .children = { nullptr, nullptr } };
  //   Node m = { .gC = generateBGCC(C, _e0_, _R0_, _e0_, _R0_), .children = { nullptr, nullptr } };
  //   Node n = { .gC = generateBGCC(C, _e0_, _R0_, _e0_, _R0_), .children = { nullptr, nullptr } };
  //   Node o = { .gC = generateBGCC(C, _e0_, _R0_, _e0_, _R0_), .children = { nullptr, nullptr } };
  //   Node d = { .gC = generateBGCC(C, h.gC.e, h.gC.R, i.gC.e, i.gC.R), .children = { &h, &i } };
  //   Node e = { .gC = generateBGCC(C, j.gC.e, j.gC.R, k.gC.e, k.gC.R), .children = { &j, &k } };
  //   Node f = { .gC = generateBGCC(C, l.gC.e, l.gC.R, m.gC.e, m.gC.R), .children = { &l, &m } };
  //   Node g = { .gC = generateBGCC(C, n.gC.e, n.gC.R, o.gC.e, o.gC.R), .children = { &n, &o } };
  //   Node b = { .gC = generateBGCC(C, d.gC.e, d.gC.R, e.gC.e, e.gC.R), .children = { &d, &e } };
  //   Node c = { .gC = generateBGCC(C, f.gC.e, f.gC.R, g.gC.e, g.gC.R), .children = { &f, &g } };
  //   Node a = { .gC = generateBGCC(C, b.gC.e, b.gC.R, c.gC.e, c.gC.R), .children = { &b, &c } };

  //   for (Node* n: { &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o }) {
  //     n->v = random_bitset<GCN/2>();
  //     n->lG.d = n->gC.d;
  //     n->lG.G = n->gC.G;

  //     std::array<std::bitset<GCK>, GCN/2> halfArray;
  //     halfArray <<= n->gC.e;
  //     n->lG.Xv = encode(n->v, halfArray, n->gC.R);

  //     for (int i = 0; i < 2; ++i) {
  //       for (int j = 0; j < 2; ++j) {
  //         halfArray >>= n->gC.T[i][j];
  //         n->lG.T[i][j] = halfArray;
  //       }
  //     }
  //   }


  //   std::bitset<GCN/2> A = random_bitset<GCN/2>();

  //   std::array<std::bitset<GCK>, GCN/2> e0;
  //   e0 >>= a.gC.e;

  //   auto Xa = encode(A,e0,a.gC.R);
  //   auto X = Xa+a.lG.Xv;

  //   auto* _ = &a;
  //   size_t depth = 0;

  //   while (_) {
  //     X <<= _->lG.Xv;
  //     auto y = evaluateBGCC(X,C,_->lG.G,_->lG.d,_->lG.T);

  //     if (y != (A.to_ulong() < _->v.to_ulong())) {
  //       std::cout << "[" << depth << "]: " << A << " < " << _->v << " != " << y << std::endl;
  //     }

  //     _ = _->children[y];
  //     ++depth;
  //   }
  // }



  return 0;
}
