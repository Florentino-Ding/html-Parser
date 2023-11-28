//
// Created by 丁麓然 on 10/25/23.
//

#ifndef HTML_PARSER_XPATH_PARSER_H
#define HTML_PARSER_XPATH_PARSER_H

#include "../htmlparser.h"
#include <istream>
#include <queue>
#include <string>

#define CHILD 1
#define DESCENDANT 2
#define PARENT 3
#define ATTRIBUTE 4

namespace custom {

using std::string, std::queue;

class xpath {
private:
  struct _xpath_node {
    string obj;
    char type;
    bool all;
  };

  bool _abs_path;
  queue<_xpath_node> _xpath_operation_queue;

  void _parse(const string &);

public:
  xpath() = default;
  xpath(const string &s) { _parse(s); }
  xpath(const xpath &x)
      : _xpath_operation_queue(x._xpath_operation_queue),
        _abs_path(x._abs_path) {}

  friend std::istream &operator>>(std::istream &is, xpath &x) {
    string s;
    is >> s;
    x._parse(s);
    return is;
  }

  bool abs_path() const { return _abs_path; }
  bool empty() const { return _xpath_operation_queue.empty(); }
  string top() const { return _xpath_operation_queue.front().obj; }
  char type() const { return _xpath_operation_queue.front().type; }
  bool all() const { return _xpath_operation_queue.front().all; }
  void next() { _xpath_operation_queue.pop(); }
};
}; // namespace custom

#endif // HTML_PARSER_XPATH_PARSER_H
