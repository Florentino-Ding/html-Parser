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
using std::list, std::pair, std::string;

template <typename T> class tree {
private:
  struct _TreeNode {
    T data;
    list<_TreeNode *> children;
    _TreeNode *parent;
    _TreeNode(T d, _TreeNode *parent) : data(d), parent(parent) {}
    void add_child(_TreeNode *child) { children.push_back(child); }
  };
  _TreeNode *_root;

public:
  tree() : _root(nullptr) {}
};
} // namespace custom

#endif // HTML_PARSER_CUSTOMTREE_H