#ifndef __ARX_RANGE_HPP__
#define __ARX_RANGE_HPP__

#include <string>
#include <vector>


template<class T>
class ArxRange {
 private:
  static constexpr unsigned int N_ROOTS = 1;

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

 public:
  constexpr ArxRange();
  constexpr ~ArxRange();
  inline void getArxRange(T &low, T &high, std::vector<std::string> &range);
  void insert(T &value, std::string encryptedPrimaryKey);
};


template<class T>
constexpr ArxRange<T>::ArxRange() {
  for (unsigned int r = 0; r < ArxRange::N_ROOTS; ++r) {
    roots[r] = new ArxRange::Node(0, "");
  }
}

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


template class ArxRange<uint64_t>;


#endif
