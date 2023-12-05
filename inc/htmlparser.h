//
// Created by 丁麓然 on 10/15/23.
//

#ifndef HTML_PARSER_HTMLPARSER_H
#define HTML_PARSER_HTMLPARSER_H

#include "datastructure/customtree.h"
#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

#define INDENT_SIZE 4

namespace custom {
class xpath;
}

using custom::tree, custom::xpath;
using std::fstream, std::string, std::pair, std::list, std::stack,
    std::unordered_set, std::unordered_map;

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
    const inline static unordered_set<string> _no_newline_tags = {
        "title", "li", "dt", "dd", "p",    "h1", "h2", "h3",
        "h4",    "h5", "h6", "a",  "span", "b",  "i",  "strong"};

    string _name;
    unordered_map<string, string> _attributes;
    bool _self_closing;
    bool _new_line_tag;

    void _parse(const string &);
    void _new_line_tag_init();

  public:
    _tag()
        : _name(""), _attributes(unordered_map<string, string>()),
          _self_closing(false) {}
    _tag(const string &s) {
      _parse(s);
      _new_line_tag_init();
    }
    _tag(const _tag &t)
        : _name(t._name), _attributes(t._attributes),
          _self_closing(t._self_closing), _new_line_tag(t._new_line_tag) {}

    bool operator==(const _tag &s) const { return _name == s._name; }
    bool operator==(const string &s) const;
    bool operator!=(const _tag &s) const { return !(*this == s); }
    bool operator!=(const string &s) const;

    bool is_self_closing_tag() const;
    bool has_attribute(const string &attr_name) const;
    bool attribute_value(const string &attr_name, const string &value) const;
    bool new_line_tag() const;
    string begin_tag() const;
    string end_tag() const;
  };
  struct _element {
    union data_union {
      _tag *tag;
      std::pair<string::const_iterator, string::const_iterator> content_idx;

      data_union() : tag(nullptr) {}
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

    bool has_attribute(const string &attr_name) const {
      if (is_tag) {
        return data.tag->has_attribute(attr_name);
      }
      return false;
    }
    bool attribute_value(const string &attr_name, const string &value) const {
      if (is_tag) {
        return data.tag->attribute_value(attr_name, value);
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
  string show(xpath &path, const bool text_only = false) const;
};

#endif // HTML_PARSER_HTMLPARSER_H
