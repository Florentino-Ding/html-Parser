//
// Created by 丁麓然 on 10/15/23.
//

#ifndef HTML_PARSER_HTMLPARSER_H
#define HTML_PARSER_HTMLPARSER_H

#include "datastructure/customstack.h"
#include "datastructure/customtree.h"
#include <fstream>
#include <string>
#include <utility>

using custom::stack, custom::tree, custom::list;
using std::fstream, std::string, std::pair;

class html {
private:
  struct _tag {
    string name;
    list<pair<string, string>> attributes;
    bool self_closing;
  };

  string _raw_content;
  string _clear_content;
  tree<_tag> *_html;

  void _clear_comment(string &);
  void _clear_script(string &);
  string _clear(string);
  int _find_tag_end(const int &) const;
  int _find_next_tag(const int &) const;
  bool _is_beginning_tag(const string &) const;
  int _find_close_tag(const int &) const;
  _tag _parse_tag(const string &) const;
  tree<_tag> *_parse() const;
  tree<_tag> *_parse(const string &) const;

public:
  html() : _html(nullptr) {}
  html(const string &content)
      : _raw_content(content), _clear_content(_clear(_raw_content)),
        _html(_parse()) {}

  bool valid() const;
};

#endif // HTML_PARSER_HTMLPARSER_H
