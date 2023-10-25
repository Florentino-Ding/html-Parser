//
// Created by 丁麓然 on 10/25/23.
//

#ifndef HTML_PARSER_XPATH_PARSER_H
#define HTML_PARSER_XPATH_PARSER_H

#include "../datastructure/customstack.h"
#include <istream>
#include <string>

namespace custom {

using custom::stack;
using std::string;

typedef stack<string> xpath_stack;

class xpath {
private:
  xpath_stack _xpath_stack;

  xpath_stack _parse(const string &);

public:
  xpath() : _xpath_stack(xpath_stack()) {}
  xpath(const string &s) : _xpath_stack(_parse(s)) {}
  xpath(const xpath &x) : _xpath_stack(x._xpath_stack) {}

  friend std::istream &operator>>(std::istream &is, xpath &x) {
    string s;
    is >> s;
    x._xpath_stack = x._parse(s);
    return is;
  }

  bool empty() const { return _xpath_stack.empty(); }
};
}; // namespace custom

#endif // HTML_PARSER_XPATH_PARSER_H
