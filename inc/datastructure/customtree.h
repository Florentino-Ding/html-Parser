//
// Created by 丁麓然 on 10/15/23.
//

#ifndef HTML_PARSER_CUSTOMTREE_H
#define HTML_PARSER_CUSTOMTREE_H

#include <cstddef>
#include <functional>
#include <iterator>
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

    list<T> find_all(const T &data) const {
      list<T> result;
      if (this->data == data) {
        result.push_back(this->data);
      }
      for (auto &child : children) {
        list<T> child_result = child->find_all(data);
        result.insert(result.end(), child_result.begin(), child_result.end());
      }

      return result;
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

  tree(const _TreeNode &node) : _size(0) {
    try {
      _tree = new _TreeNode(&node);
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

  bool empty() const { return _size == 0; }
  int size() const { return _size; }
  tree<T> root() const {
    if (_tree == nullptr) {
      throw std::runtime_error("Empty tree");
    }
    return tree<T>(_tree);
  }
  tree<T> parent() const {
    if (_tree == nullptr) {
      throw std::runtime_error("Empty tree");
    }
    if (_tree->parent == nullptr) {
      throw std::runtime_error("Tree has no parent");
    }
    if (_back == _tree) {
      throw std::runtime_error("Root has no parent");
    }
    return tree<T>(_back->parent);
  }
  list<tree<T>> find(const T &data) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    for (auto &child : _tree->children) {
      if (child->data == data) {
        result.push_back(tree<T>(child));
      }
    }
    return result;
  }
  template <typename AnotherT>
  list<tree<T>>
  find(const AnotherT &data,
       const std::function<bool(const T &, const AnotherT &)> &cmp) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    for (auto &child : _tree->children) {
      if (cmp(child->data, data)) {
        result.push_back(tree<T>(child));
      }
    }
    return result;
  }
  list<tree<T>> find_all(const T &data) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    if (_tree->data == data) {
      result.push_back(tree<T>(_tree));
    }
    for (auto &child : _tree->children) {
      list<tree<T>> child_result = tree<T>(child).find_all(data);
      result.insert(result.end(), child_result.begin(), child_result.end());
    }

    return result;
  }
  template <typename AnotherT>
  list<tree<T>>
  find_all(const AnotherT &data,
           const std::function<bool(const T &, const AnotherT &)> &cmp) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    if (cmp(_tree->data, data)) {
      result.push_back(tree<T>(_tree));
    }
    for (auto &child : _tree->children) {
      list<tree<T>> child_result = tree<T>(child).find_all(data, cmp);
      result.insert(result.end(), child_result.begin(), child_result.end());
    }

    return result;
  }
  operator string() const {
    string result;
    if (_tree == nullptr) {
      return result;
    }
    result += string(_tree->data);
    for (auto &child : _tree->children) {
      result += string(tree<T>(child));
    }
    return result;
  }
};
}; // namespace custom

#endif // HTML_PARSER_CUSTOMTREE_H