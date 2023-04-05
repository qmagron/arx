#include "server-proxy/indexes/ArxRange.hpp"


template<class T>
void ArxRange<T>::getArxRange(T &low, T &high, std::vector<std::string> &range, Node *node) {
  if (node->left != nullptr && low <= node->value) {
    getArxRange(low, high, range, node->left);
  }

  if (low <= node->value && node->value < high && node != roots[0]) {
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
  // TODO multitreap & random priorities
  // TODO balancing

  // If the tree is empty, create a new root
  if (roots[0] == nullptr) {
    roots[0] = new Node(value, encryptedPrimaryKey);
    return;
  }

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


template<class T>
void ArxRange<T>::remove(Node *&node) {
  // TODO multitreap
  // TODO balancing

  if (node->left == nullptr && node->right == nullptr) {
    delete node;
    node = nullptr;
  } else if (node->left == nullptr) {
    Node *tmp = node;
    node = node->right;
    tmp->right = nullptr;
    delete tmp;
  } else if (node->right == nullptr) {
    Node *tmp = node;
    node = node->left;
    tmp->left = nullptr;
    delete tmp;
  } else {
    Node *tmp = node->right;
    while (tmp->left != nullptr) {
      tmp = tmp->left;
    }
    node->value = tmp->value;
    node->encryptedPrimaryKey = tmp->encryptedPrimaryKey;
    remove(tmp);
  }
}
