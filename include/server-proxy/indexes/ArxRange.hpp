#ifndef __ARX_RANGE_HPP__
#define __ARX_RANGE_HPP__

#include <string>
#include <vector>


template<class T>
class ArxRange {
 private:
  static constexpr unsigned int N_ROOTS = 1;  // TODO forest

 protected:
  struct Node {
    T value;
    std::string encryptedPrimaryKey;
    Node *left = nullptr;
    Node *right = nullptr;

    constexpr Node(T value, std::string encryptedPrimaryKey): value(value), encryptedPrimaryKey(encryptedPrimaryKey) {}
    constexpr ~Node() {
      delete left;
      delete right;
    }
  };

  Node *roots[N_ROOTS] = {nullptr};

  void getArxRange(T &low, T &high, std::vector<std::string> &range, Node *node);
  Node *&search(T &value, Node *&node);
  void remove(Node *&node);
  void rotateLeft(Node *&node);
  void rotateRight(Node *&node);

 public:
  constexpr ArxRange() = default;
  constexpr ~ArxRange();
  inline void getArxRange(T &low, T &high, std::vector<std::string> &range);
  void insert(T &value, std::string encryptedPrimaryKey);
  inline void remove(T &value);
};


template<class T>
constexpr ArxRange<T>::~ArxRange() {
  for (unsigned int r = 0; r < ArxRange::N_ROOTS; ++r) {
    delete roots[r];
  }
}

template<class T>
inline void ArxRange<T>::getArxRange(T &low, T &high, std::vector<std::string> &range) {
  getArxRange(low, high, range, roots[0]);
}


template<class T>
inline void ArxRange<T>::remove(T &value) {
  Node *&node = search(value, roots[0]);
  if (value == node->value) {
    remove(node);
  }
}


template class ArxRange<uint64_t>;


#endif
