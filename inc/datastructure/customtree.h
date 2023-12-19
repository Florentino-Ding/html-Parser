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

    _TreeNode() = default;

    _TreeNode(const T &d, _TreeNode *p = nullptr) : data(d), parent(p) {}

    // _TreeNode(const T &d, shared_ptr<_TreeNode> p = nullptr)
    //     : data(d), parent(p) {}

    _TreeNode(const _TreeNode &another_node) : data(another_node.data) {
      // copy constructor
      if (another_node.parent != nullptr) {
        parent = another_node.parent;
      }
      for (auto &child : another_node.children) {
        children.push_back(child);
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
    // copy constructor
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
    return tree<T>(_tree->parent);
  }

  list<tree<T>> find(const T &data, int first_of = INT_MAX) const {
    list<tree<T>> result;
    // check if the tree is empty
    if (_tree == nullptr) {
      return result;
    }
    for (auto &child : _tree->children) {
      // check if the data of the current node is the same as the data
      if (child->data == data) {
        result.push_back(tree<T>(child));
        // if the number of the result is enough, return the result
        if (not --first_of) {
          return result;
        }
      }
    }
    return result;
  }

  list<tree<T>> find(const list<T> &data_list, int first_of = INT_MAX) const {
    list<tree<T>> result;
    // check if the tree is empty
    if (_tree == nullptr) {
      return result;
    }
    for (auto &child : _tree->children) {
      // compare the data of the current node with each data in the list
      for (auto &data : data_list) {
        // check if the data of the current node is the same as the data
        if (child->data == data) {
          result.push_back(tree<T>(child));
          // if the number of the result is enough, return the result
          if (not --first_of) {
            return result;
          }
          // if the data of the current node is the same as the data, break
          // the loop to avoid duplicate result
          break;
        }
      }
    }
    return result;
  }

  list<tree<T>> find(const std::function<bool(const T &)> &cmp,
                     int first_of = INT_MAX) const {
    list<tree<T>> result;
    // check if the tree is empty
    if (_tree == nullptr) {
      return result;
    }
    for (auto &child : _tree->children) {
      // check if the data of the current node is the same as the data
      if (cmp(child->data)) {
        result.push_back(tree<T>(child));
        // if the number of the result is enough, return the result
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
    // check if the tree is empty
    if (_tree == nullptr) {
      return result;
    }
    for (auto &child : _tree->children) {
      if (cmp(child->data, data)) {
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
  find(const list<AnotherT> &data_list,
       const std::function<bool(const T &, const AnotherT &)> &cmp,
       int first_of = INT_MAX) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    for (auto &child : _tree->children) {
      // compare the data of the current node with each data in the list
      for (auto &data : data_list) {
        // check if the data of the current node is the same as the data
        if (cmp(child->data, data)) {
          result.push_back(tree<T>(child));
          // if the number of the result is enough, return the result
          if (not --first_of) {
            return result;
          }
          // if the data of the current node is the same as the data, break the
          // loop to avoid duplicate result
          break;
        }
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

  list<tree<T>> find_all(const list<T> &data_list) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    // check if the data of the current node is the same as the data
    for (auto &data : data_list) {
      // check if the data of the current node is the same as the data
      if (_tree->data == data) {
        result.push_back(tree<T>(_tree));
        // if the data of the current node is the same as the data, break the
        // loop to avoid duplicate result
        break;
      }
    }
    // recursively find the descendant
    for (auto &child : _tree->children) {
      list<tree<T>> child_result = tree<T>(child).find_all(data_list);
      result.insert(result.end(), child_result.begin(), child_result.end());
    }

    return result;
  }

  list<tree<T>> find_all(const std::function<bool(const T &)> &cmp) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    // check if the data of the current node conforms to the condition
    if (cmp(_tree->data)) {
      result.push_back(tree<T>(_tree));
    }
    // recursively find the descendant
    for (auto &child : _tree->children) {
      list<tree<T>> child_result = tree<T>(child).find_all(cmp);
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

  template <typename AnotherT>
  list<tree<T>>
  find_all(const list<AnotherT> &data_list,
           const std::function<bool(const T &, const AnotherT &)> &cmp) const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    // check if the data of the current node conforms to the condition
    for (auto &data : data_list) {
      if (cmp(_tree->data, data)) {
        result.push_back(tree<T>(_tree));
        // if the data of the current node is the same as the data, break the
        // loop to avoid duplicate result
        break;
      }
    }
    // recursively find the descendant
    for (auto &child : _tree->children) {
      list<tree<T>> child_result = tree<T>(child).find_all(data_list, cmp);
      result.insert(result.end(), child_result.begin(), child_result.end());
    }

    return result;
  }

  list<tree<T>> find_sibling(const std::function<bool(const T &)> &cmp) const {
    // check if the tree has a parent
    if (_tree == nullptr or _tree->parent == nullptr) {
      return list<tree<T>>();
    }
    // get the parent of the current node
    shared_ptr<_TreeNode> parent = _tree->parent;
    list<tree<T>> result;
    // get the position of the current node in the parent's children list
    auto pos = std::find_if(parent->children.begin(), parent->children.end(),
                            [this](const shared_ptr<_TreeNode> &node) {
                              return node->data == _tree->data;
                            });
    // find the sibling after the current node
    for (auto it = ++pos; it != parent->children.end(); ++it) {
      if (cmp((*it)->data)) {
        result.push_back(tree<T>(*it));
      }
    }

    return result;
  }

  list<tree<T>> all() const {
    list<tree<T>> result;
    if (_tree == nullptr) {
      return result;
    }
    if (_tree->parent != nullptr)
      result.push_back(tree<T>(_tree));
    for (auto &child : _tree->children) {
      list<tree<T>> child_result = tree<T>(child).all();
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
