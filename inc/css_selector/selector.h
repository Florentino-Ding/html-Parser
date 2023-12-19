//
// Created by 丁麓然 on 11/20/23.
//

#ifndef HTMLPARSER_CSSSELECTOR_H
#define HTMLPARSER_CSSSELECTOR_H

#include "../xpath/xpath.h"
#include <list>
#include <memory>
#include <queue>
#include <string>

using custom::xpath;
using std::string, std::queue, std::list, std::shared_ptr;

class CssSelector : public xpath {
private:
  void _parse(const string &);

public:
  CssSelector() {}
  CssSelector(const CssSelector &) {}
  CssSelector(const string &s) { _parse(s); }
  ~CssSelector() {}
};

#endif // HTMLPARSER_CSSSELECTOR_H
