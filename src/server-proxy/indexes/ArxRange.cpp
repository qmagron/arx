#include "server-proxy/indexes/ArxRange.hpp"


template<class T>
void ArxRange<T>::getArxRange(T &low, T &high, std::vector<std::string> &range, Node *node) {
  if (node->left != nullptr && low <= node->value) {
    getArxRange(low, high, range, node->left);
  }

  if (low <= node->value && node->value < high) {
    range.push_back(node->encryptedPrimaryKey);
  }

  if (node->right != nullptr && high >= node->value) {
    getArxRange(low, high, range, node->right);
  }
}


template<class T>
ArxRange<T>::Node *&ArxRange<T>::search(T &value, Node *&node) {
  if (value < node->value) {
    if (node->left == nullptr) {
      return node;
    } else {
      return search(value, node->left);
    }
  } else if (value > node->value) {
    if (node->right == nullptr) {
      return node;
    } else {
      return search(value, node->right);
    }
  } else {
    return node;
  }
}


template<class T>
void ArxRange<T>::insert(T &value, std::string encryptedPrimaryKey) {
  // TODO multitreap

  Node *&node = search(value, roots[0]);

  if (value < node->value) {
    node->left = new Node(value, encryptedPrimaryKey);
  } else if (value > node->value) {
    node->right = new Node(value, encryptedPrimaryKey);
  } else {
    Node *newNode = new Node(value, encryptedPrimaryKey);
    newNode->left = node->left;
    newNode->right = node->right;
    node->left = nullptr;
    node->right = nullptr;
    delete node;
    node = newNode;
  }
}
