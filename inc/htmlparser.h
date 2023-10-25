//
// Created by 丁麓然 on 10/15/23.
//

#ifndef HTML_PARSER_HTMLPARSER_H
#define HTML_PARSER_HTMLPARSER_H

#include "datastructure/customstack.h"
#include "datastructure/customtree.h"
#include "xpath_parser/xpath_parser.h"
#include <fstream>
#include <iterator>
#include <list>
#include <string>
#include <utility>

using custom::stack, custom::tree, custom::xpath;
using std::fstream, std::string, std::pair, std::list;

class html {
private:
  struct _tag;
  struct _element;
  friend class xpath;

  string _raw_content;
  string _clear_content;
  tree<_element> _html;

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
    string begin_tag() const;
    string end_tag() const;
  };
  struct _element {
    union data_union {
      _tag *tag;
      std::pair<int, int> content_idx;

      data_union() : tag(nullptr) {}
      data_union(const _tag &t) { tag = new _tag(t); }
      data_union(const int start, const int end)
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
    _element(const pair<int, int> &p) : is_tag(false) { data.content_idx = p; }
    _element(const _tag &t) : is_tag(true) { data.tag = new _tag(t); }
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
    _html = _parse();
  }
  string raw_content() const { return _raw_content; }
  string show(const xpath &path) const;
};

#endif // HTML_PARSER_HTMLPARSER_H
