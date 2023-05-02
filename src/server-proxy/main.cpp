#include "server-proxy/ServerProxy.hpp"

#include "server-proxy/indexes/ArxRange.hpp"
#include "crypto/bgcc.hpp"
#include "crypto/circuits.hpp"
#include "crypto/utils.hpp"
#include <iostream>


constexpr unsigned int IN_PORT = 1237;
constexpr unsigned int REMOTE_PORT = 5432;

const Circuit C = COMP<GCN/2>();


int main() {
  // ServerProxy server(IN_PORT);
  // server.start(REMOTE_PORT);




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





  ArxRange index;




  return 0;
}
