//
// Created by 丁麓然 on 10/15/23.
//

#ifndef HTML_PARSER_CUSTOMTREE_H
#define HTML_PARSER_CUSTOMTREE_H

#include <cstddef>
#include <list>
#include <new>
#include <stdexcept>
#include <string>

namespace custom {
using std::pair, std::string, std::list;

template <typename T> class tree {
private:
  struct _TreeNode {
    T data;
    list<_TreeNode *> children;
    _TreeNode *parent;

    _TreeNode() : parent(nullptr), data() {}

    _TreeNode(const T d, _TreeNode *p = nullptr) : data(d), parent(p) {}

    _TreeNode(const _TreeNode *another_node, _TreeNode *p = nullptr)
        : data(another_node->data), parent(p) {
      for (auto &child : another_node->children) {
        try {
          children.push_back(new _TreeNode(*child));
        } catch (std::bad_alloc &e) {
          throw std::runtime_error("Memory allocation failed");
        }
      }
    }
  };
  _TreeNode *_tree;
  _TreeNode *_back;
  size_t _size;

public:
  tree() : _size(0), _tree(nullptr), _back(nullptr) {}

  tree(const T &data) : _size(0) {
    try {
      _tree = new _TreeNode(data);
    } catch (std::bad_alloc &e) {
      throw std::runtime_error("Memory allocation failed");
    }
    _back = _tree;
    _size++;
  }

  tree(const tree &another_tree) : _size(another_tree._size) {
    try {
      _tree = new _TreeNode(another_tree._tree);
    } catch (std::bad_alloc &e) {
      throw std::runtime_error("Memory allocation failed");
    }
    _back = _tree;
    while (!_back->children.empty()) {
      _back = _back->children.back();
    }
  }

  void add_child(const T &data) {
    if (this->_tree == nullptr) {
      throw std::runtime_error("Empty tree");
    }
    _TreeNode *new_node;
    try {
      new_node = new _TreeNode(data, _back);
    } catch (std::bad_alloc &e) {
      throw std::runtime_error("Memory allocation failed");
    }
    _back->children.push_back(new_node);
  }

  void add_child(const tree<T> &another_tree) {
    if (this->_tree == nullptr) {
      throw std::runtime_error("Empty tree");
    }
    _TreeNode *new_node;
    try {
      new_node = new _TreeNode(another_tree._tree, _back);
    } catch (std::bad_alloc &e) {
      throw std::runtime_error("Memory allocation failed");
    }
    _back->children.push_back(new_node);
  }

  void add_sibling(const T &data) {
    _TreeNode *new_node;
    if (_back == _tree or _back->parent == nullptr) {
      throw std::runtime_error("Tree has only one layer");
    }
    try {
      new_node = new _TreeNode(data, _back->parent);
    } catch (std::bad_alloc &e) {
      throw std::runtime_error("Memory allocation failed");
    }
    _back->parent->children.push_back(new_node);
    _back = new_node;
  }

  void add_sibling(const tree<T> &another_tree) {
    _TreeNode *new_node;
    if (_back == _tree or _back->parent == nullptr) {
      throw std::runtime_error("Tree has only one layer");
    }
    try {
      new_node = new _TreeNode(another_tree._tree, _back->parent);
    } catch (std::bad_alloc &e) {
      throw std::runtime_error("Memory allocation failed");
    }
    _back->parent->children.push_back(new_node);
    _back = new_node;
  }
};
} // namespace custom

#endif // HTML_PARSER_CUSTOMTREE_H