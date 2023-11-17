//
// Created by 丁麓然 on 10/15/23.
//

#ifndef HTML_PARSER_HTMLPARSER_H
#define HTML_PARSER_HTMLPARSER_H

#include "datastructure/customstack.h"
#include "datastructure/customtree.h"
#include <fstream>
#include <list>
#include <string>

namespace custom {
class xpath;
}

using custom::stack, custom::tree, custom::xpath;
using std::fstream, std::string, std::pair, std::list;

class html {
private:
  class _tag;
  struct _element;

  string _raw_content;
  string _clear_content;
  tree<_element> _html;

  friend class custom::tree<_element>;

  class _tag {
  private:
    string _name;
    list<pair<string, string>> _attributes;
    bool _self_closing;

  public:
    _tag()
        : _name(""), _attributes(list<pair<string, string>>()),
          _self_closing(false) {}
    _tag(const string &);
    _tag(const _tag &t)
        : _name(t._name), _attributes(t._attributes),
          _self_closing(t._self_closing) {}

    bool operator==(const _tag &s) const { return _name == s._name; }
    bool operator==(const string &s) const;
    bool operator!=(const _tag &s) const { return !(*this == s); }
    bool operator!=(const string &s) const;

    bool is_self_closing_tag() const;
    bool has_attr(const string &attr_name) const;
    string begin_tag() const;
    string end_tag() const;
  };
  struct _element {
    union data_union {
      _tag *tag;
      std::pair<string::const_iterator, string::const_iterator> content_idx;

      data_union() : tag(nullptr) {}
      data_union(const _tag &t) { tag = new _tag(t); }
      data_union(const string::iterator &start, const string::iterator &end)
          : content_idx(std::make_pair(start, end)) {}
    };

    bool is_tag;
    data_union data;

    _element() : is_tag(false) {}
    _element(const _element &e) : is_tag(e.is_tag) {
      if (is_tag) {
        data.tag = e.data.tag;
      } else {
        data.content_idx = e.data.content_idx;
      }
    }
    _element(const pair<string::const_iterator, string::const_iterator> &p)
        : is_tag(false) {
      data.content_idx = p;
    }
    _element(const _tag &t) : is_tag(true) { data.tag = new _tag(t); }

    bool operator==(const _element &e) const {
      if (is_tag != e.is_tag) {
        return false;
      }
      if (is_tag) {
        return *data.tag == *e.data.tag;
      } else {
        return data.content_idx == e.data.content_idx;
      }
    }
    bool operator==(const _tag &t) const {
      if (is_tag) {
        return *data.tag == t;
      } else {
        return false;
      }
    }

    bool has_attr(const string &attr_name) const {
      if (is_tag) {
        return data.tag->has_attr(attr_name);
      }
      return false;
    }
  };

  void _clear_comment();
  void _clear_script();
  void _clear();
  int _find_tag_end(const int &) const;
  string _get_tag_content(const int &) const;
  int _find_next_tag(const int &) const;
  int _find_close_tag(const int &) const;
  bool _is_beginning_tag(const int &) const;
  tree<_element> _parse() const;
  tree<_element> _parse(const pair<int, int> &scope) const;

public:
  html() = default;
  html(const string &content) : _raw_content(content), _clear_content(content) {
    _clear();
    _html = tree<_element>(_element());
    _html.add_child(_parse());
  }
  string raw_content() const { return _raw_content; }
  string show(xpath &path) const;
};

namespace custom {
template <> inline tree<html::_element>::operator string() const {
  string result;
  if (_tree == nullptr) {
    return result;
  }
  if (_tree->data.is_tag) {
    result += _tree->data.data.tag->begin_tag();
    for (auto &child : _tree->children) {
      result += string(tree<html::_element>(child));
    }
    result += _tree->data.data.tag->end_tag();
  } else {
    if (not(_tree->children.empty())) {
      throw std::runtime_error("invalid html tree");
    }
    result = string(_tree->data.data.content_idx.first,
                    _tree->data.data.content_idx.second);
  }
  return result;
}
} // namespace custom

#endif // HTML_PARSER_HTMLPARSER_H
