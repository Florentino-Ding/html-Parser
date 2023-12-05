//
// Created by 丁麓然 on 11/20/23.
//

#ifndef HTMLPARSER_CSSSELECTOR_H
#define HTMLPARSER_CSSSELECTOR_H

#include "../xpath/xpath.h"
#include <list>
#include <queue>
#include <string>

using custom::xpath;
using std::string, std::queue, std::list;

class basic_selector {
private:
  string _selector_name;

public:
  basic_selector() {}
  basic_selector(const basic_selector &) {}
  basic_selector(const string &s) {}
  virtual ~basic_selector() {}
};

class uni_selector : basic_selector {};

class element_selector : basic_selector {
public:
  element_selector() : basic_selector() {}
  element_selector(const element_selector &) : basic_selector() {}
  element_selector(const string &s) : basic_selector(s) {}
};

class class_selector : basic_selector {
public:
  class_selector() : basic_selector() {}
  class_selector(const class_selector &) : basic_selector() {}
  class_selector(const string &s) : basic_selector(s) {}
};

class id_selector : basic_selector {
public:
  id_selector() : basic_selector() {}
  id_selector(const id_selector &) : basic_selector() {}
  id_selector(const string &s) : basic_selector(s) {}
};

class attr_selector : basic_selector {
public:
  attr_selector() : basic_selector() {}
  attr_selector(const attr_selector &) : basic_selector() {}
  attr_selector(const string &s) : basic_selector(s) {}
};

class CssSelector {
private:
  list<queue<basic_selector *>> _selector_list;
  void _parse(const string &);

public:
  CssSelector() {}
  CssSelector(const CssSelector &) {}
  CssSelector(const string &s) { _parse(s); }
  ~CssSelector() {}

  explicit operator xpath() const;
};

#endif // HTMLPARSER_CSSSELECTOR_H
