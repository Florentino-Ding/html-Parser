//
// Created by 丁麓然 on 10/25/23.
//

#include "../../inc/xpath_parser/xpath_parser.h"

namespace custom {

xpath_stack xpath::_parse(const string &s) {
  xpath_stack result;
  string::const_iterator it = s.begin();
  string::const_iterator end = s.end();
  string tag;

  while (it != end) {
    if (*it == '/') {
      if (!tag.empty()) {
        result.push(tag);
        tag.clear();
      }
    } else {
      tag += *it;
    }
    ++it;
  }

  if (!tag.empty()) {
    result.push(tag);
  }

  return result;
}

}; // namespace custom
