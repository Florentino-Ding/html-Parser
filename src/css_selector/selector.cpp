//
// Created by 丁麓然 on 11/20/23.
//

#include "../../inc/css_selector/selector.h"

void CssSelector::_parse(const string &selector_str) {
  // check if the selector_str is empty
  if (selector_str.find_first_not_of(' ') == string::npos) {
    throw std::invalid_argument("empty selector");
  }
  // divide the selector_str by ',' and push each parts into the queue
  queue<basic_selector *> selector_queue;
  int start_pos = selector_str.find_first_not_of(',');
  while (start_pos != string::npos) {

    // push the selector into the list
    _selector_list.push_back(selector_queue);
    // clear the queue
    while (not selector_queue.empty()) {
      selector_queue.pop();
    }
  }
}
