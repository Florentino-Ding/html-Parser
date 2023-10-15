//
// Created by 丁麓然 on 10/15/23.
//

#ifndef HTML_PARSER_HTMLPARSER_H
#define HTML_PARSER_HTMLPARSER_H

#include "datastructure/customstack.h"
#include "datastructure/customtree.h"
#include <cstddef>
#include <fstream>
#include <new>
#include <stdexcept>
#include <string>
#include <utility>

using custom::stack, custom::tree;
using std::fstream, std::string, std::list, std::pair;

class html {
private:
  struct _tag {
    string name;
    list<pair<string, string>> attributes;
    string content;
  };
  string _raw_content;
  string _url;
  tree<_tag> *_html;
  int _find_close_tag(const int &start);
  int _find_next_tag(const int &start);
  int _find_end_tag(const int &start);

public:
  html() : _html(nullptr) {}
  html(const string &url) : _url(url), _html(nullptr) {}
  html(const string &content, const string &url);
  html(const fstream &html_file, const string &url);
};

#endif // HTML_PARSER_HTMLPARSER_H
