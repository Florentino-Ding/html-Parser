//
// Created by 丁麓然 on 10/25/23.
//

#include "../../inc/xpath_parser/xpath_parser.h"
#include <queue>
#include <string>

namespace custom {

using std::string, std::queue;

void xpath::_parse(const string &xpath_str) {
  // check if the xpath_str is empty
  if (xpath_str.find_first_not_of(' ') == string::npos) {
    throw std::invalid_argument("empty xpath");
  }

  queue<_xpath_node> result;
  int start_pos, next_pos;
  start_pos = next_pos = xpath_str.find_first_not_of(' ');
  // check if the xpath_str is an absolute path
  if (xpath_str[start_pos] == '/' and xpath_str[start_pos + 1] != '/') {
    _abs_path = true;
    start_pos++;
  } else {
    _abs_path = false;
  }
  // divide the xpath_str by '/' and push each parts into the queue
  while (true) {
    // check if double slash
    if (xpath_str[start_pos] == '/') {
      next_pos = xpath_str.find_first_of('/', start_pos + 1);
    } else {
      next_pos = xpath_str.find_first_of('/', start_pos);
    }
    string part;
    if (next_pos == string::npos) {
      part = xpath_str.substr(start_pos);
    } else {
      part = xpath_str.substr(start_pos, next_pos - start_pos);
    }
    if (part.empty()) {
      throw std::invalid_argument("invalid xpath");
    }
    switch (part[0]) {
    case '/': {
      switch (part[1]) {
      case '/':
        throw std::invalid_argument("invalid xpath");
      case '@':
        result.push({part.substr(2), ATTRIBUTE, true});
        break;
      default:
        result.push({part.substr(1), CHILD, true});
      }
      break;
    }
    case '@':
      result.push({part.substr(1), ATTRIBUTE, false});
      break;
    case '.': {
      if (part[1] == '.') {
        result.push({part.substr(1), PARENT, false});
      } else {
        break;
      }
    } break;
    default:
      result.push({part, CHILD, false});
    }

    if (next_pos == string::npos) {
      break;
    }
    start_pos = next_pos + 1;
  }
  _xpath_operation_queue = result;
}

}; // namespace custom
