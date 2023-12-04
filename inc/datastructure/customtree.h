//
// Created by 丁麓然 on 10/15/23.
//

#ifndef HTML_PARSER_CUSTOMTREE_H
#define HTML_PARSER_CUSTOMTREE_H

#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>

namespace custom {
using std::pair, std::string, std::list, std::shared_ptr, std::weak_ptr;

template <typename T> class tree {
private:
  struct _TreeNode {
    T data;
    list<shared_ptr<_TreeNode>> children;
    shared_ptr<_TreeNode> parent;

    _TreeNode() : parent(nullptr), data() {}

    _TreeNode(const T &d, _TreeNode *p = nullptr) : data(d), parent(p) {}

    _TreeNode(const T &d, shared_ptr<_TreeNode> p) : data(d), parent(p) {}

    _TreeNode(const _TreeNode &another_node) : data(another_node.data) {
      for (auto &child : another_node.children) {
        children.push_back(std::make_shared<_TreeNode>(*child));
      }
    }
  };

  shared_ptr<_TreeNode> _tree;
  shared_ptr<_TreeNode> _back;

public:
  tree() : _tree(nullptr), _back(nullptr) {}

  tree(const T &data) {
    _tree = std::make_shared<_TreeNode>(data);
    _back = _tree;
  }

  tree(const shared_ptr<_TreeNode> &node) : _tree(node), _back(node) {
    while (_back != nullptr and _back->children.size() > 0) {
      _back = shared_ptr<_TreeNode>(_back->children.back());
    }
  }

  tree(const tree &another_tree) {
    _tree = std::make_shared<_TreeNode>(*another_tree._tree);
    _back = _tree;
    while (_back != nullptr and _back->children.size() > 0) {
      _back = _back->children.back();
    }
  }

  void add_child(const T &data) {
    if (this->_tree == nullptr) {
      throw std::runtime_error("Empty tree");
    }
    _back->children.push_back(std::make_shared<_TreeNode>(data));
  }

  void add_child(const tree<T> &another_tree) {
    if (this->_tree == nullptr) {
      throw std::runtime_error("Empty tree");
    }
    another_tree._tree->parent = _back;
    _back->children.push_back(another_tree._tree);
  }

  bool empty() const { return _tree == nullptr; }

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
    return tree<T>(_tree->parent);
  }

  list<tree<T>> find(const T &data, int first_of = INT_MAX) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    for (auto &child : _tree->children) {
      if (child->data == data) {
        result.push_back(tree<T>(child));
        if (not --first_of) {
          return result;
        }
      }
    }
    return result;
  }

  template <typename AnotherT>
  list<tree<T>>
  find(const AnotherT &data,
       const std::function<bool(const T &, const AnotherT &)> &cmp,
       int first_of = INT_MAX) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    for (auto &child : _tree->children) {
      if (cmp(child->data, data)) {
        result.push_back(tree<T>(child));
      }
      if (not --first_of) {
        return result;
      }
    }
    return result;
  }

  list<tree<T>> find_all(const T &data) const {
    list<tree<T>> result;
    // check if the tree is empty
    if (_tree == nullptr) {
      return result;
    }
    // check if the data of the current node is the same as the data
    if (_tree->data == data) {
      result.push_back(tree<T>(_tree));
    }
    // recursively find the descendant
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
    // check if the data of the current node conforms to the condition
    if (cmp(_tree->data, data)) {
      result.push_back(tree<T>(_tree));
    }
    // recursively find the descendant
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
