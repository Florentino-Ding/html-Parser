//
// Created by 丁麓然 on 11/20/23.
//

#include "../../inc/css_selector/selector.h"
#include <string>

using custom::attribute_xpath_constraint, custom::element_xpath_constraint,
    custom::xpath, std::queue, std::string;

queue<custom::xpath::xpath_node> parse(const string &selector_str, char axis);

void CssSelector::_parse(const string &selector_str) {
  // check if the selector_str is empty
  if (selector_str.find_first_not_of(' ') == string::npos) {
    throw std::invalid_argument("empty selector");
  }

  int start_pos = selector_str.find_first_not_of(' ');
  string clean_selector_str = selector_str.substr(start_pos);
  // clean the selector_str
  // TODO: remove the spaces around the '>' or '+' or '~' or ','

  // check if the selector_str is a empty selector
  if (start_pos == string::npos) {
    throw std::invalid_argument("empty selector");
  }

  // find the end position of the next node
  int end_pos = clean_selector_str.find_first_of(" >+~", start_pos);
  // if the end position is not found, then the end position is the end of the
  // selector
  if (end_pos == string::npos) {
    _xpath_queue = parse(clean_selector_str, AXIS_DESCENDANT);
  } else {
    switch (clean_selector_str[end_pos]) {
    case ' ':
      _xpath_queue = parse(clean_selector_str, AXIS_DESCENDANT);
      break;
    case '>':
      _xpath_queue = parse(clean_selector_str, AXIS_DESCENDANT);
      break;
    case '+':
      _xpath_queue = parse(clean_selector_str, AXIS_DESCENDANT);
      break;
    case '~':
      _xpath_queue = parse(clean_selector_str, AXIS_DESCENDANT);
      break;
    }
  }
}

queue<custom::xpath::xpath_node> parse(const string &selector, char axis) {
  // if a empty string is passed in, then return a empty queue
  if (selector.empty()) {
    return queue<custom::xpath::xpath_node>();
  }

  // create a queue to store the xpath nodes
  queue<custom::xpath::xpath_node> xpath_queue;
  // find the start position of the next node
  int start_pos = selector.find_first_not_of(' ');
  // find the end position of the next node
  int end_pos = selector.find_first_of(" >+~", start_pos);
  // check if the end position is found
  if (end_pos == string::npos) {
    // if not found, then the end position is the end of the selector
    end_pos = selector.size();
  }
  // get the current node
  string node_str = selector.substr(start_pos, end_pos - start_pos);
  // parse the current selector node
  // if a universal selector is passed in, then push a universal xpath node
  if (node_str[0] == '*') {
    // check if the current node is only a '*'
    if (not(node_str.size() == 1)) {
      throw std::invalid_argument("invalid selector");
    }
    xpath_queue.push(custom::xpath::xpath_node(AXIS_ALL));
  }
  // if a class selector is passed in, then push a class xpath node
  else if (node_str[0] == '.') {
    // check if the current node is only a '.'
    if (node_str.size() == 1) {
      throw std::invalid_argument("invalid selector");
    }

    // check if the current node contains another class
    int div_pos = node_str.find_first_of('.', 1);
    xpath::xpath_node current_node;
    // WARNING: here I only support max two classes
    if (div_pos != string::npos) {
      // get the second class name
      string second_class_name = node_str.substr(div_pos + 1);
      // add the second class name to the xpath node
      current_node.add_constraint(std::make_shared<attribute_xpath_constraint>(
          "class", second_class_name));
    } else {
      div_pos = node_str.size();
    }
    // get the first or the only class name
    string class_name = node_str.substr(1, div_pos - 1);
    // add the class name to the xpath node
    current_node.add_constraint(
        std::make_shared<attribute_xpath_constraint>("class", class_name));
    // set the axis of the xpath node to "descendant"
    current_node._axis = axis;

    // push the xpath node to the queue
    xpath_queue.push(current_node);
  }
  // if a id selector is passed in, then push a id xpath node
  else if (node_str[0] == '#') {
    // check if the current node is only a '#'
    if (node_str.size() == 1) {
      throw std::invalid_argument("invalid selector");
    }
    // get the id name
    string id_name = node_str.substr(1);
    // add the id name to the xpath node
    xpath::xpath_node current_node;
    // add the id name to the xpath node
    current_node.add_constraint(
        std::make_shared<attribute_xpath_constraint>("id", id_name));
    // set the axis of the xpath node to "descendant"
    current_node._axis = axis;

    // push the xpath node to the queue
    xpath_queue.push(current_node);
  }
  // if a element selector is passed in, then push a element xpath node
  else {
    // check if the current node has a class name
    // WARNING: here I only support element with only class
    int start_pos = node_str.find_first_not_of(' ');
    // check if the current node has a class name
    int dot_pos = node_str.find_first_of('.', start_pos);
    // if the current node has a class name
    if (dot_pos != string::npos) {
      // get the class name
      string class_name = node_str.substr(dot_pos + 1);
      // add the class name to the xpath node
      xpath::xpath_node current_node;
      // add the element name to the xpath node
      current_node.add_constraint(std::make_shared<element_xpath_constraint>(
          node_str.substr(0, dot_pos)));
      // add the class name to the xpath node
      current_node.add_constraint(
          std::make_shared<attribute_xpath_constraint>("class", class_name));
      // set the axis of the xpath node to "descendant"
      current_node._axis = axis;

      // push the xpath node to the queue
      xpath_queue.push(current_node);
    }
    // else the current node doesn't have a class name
    else {
      int start_pos = node_str.find_first_not_of(' ');
      int comma_pos = node_str.find_first_of(',', start_pos);
      int size = node_str.size();
      xpath::xpath_node current_node;
      // using loop constantly scan for ',' to get the element name
      for (; start_pos != size;
           comma_pos = node_str.find_first_of(',', start_pos)) {
        // if ',' not found, then the end position is the end of the node_str
        if (comma_pos == string::npos) {
          comma_pos = size;
        }
        // add the element name to the xpath node
        current_node.add_constraint(std::make_shared<element_xpath_constraint>(
            node_str.substr(start_pos, comma_pos - start_pos)));
        // set the start position to the end position of the last element name
        start_pos = comma_pos == size ? size : comma_pos + 1;
      }
      // set the axis of the xpath node to "descendant"
      current_node._axis = axis;
      // set the logic of the xpath node to "or"
      current_node._logic = LOGIC_OR;

      // push the xpath node to the queue
      xpath_queue.push(current_node);
    }
  }
  // if end position is not the end of the selector, then parse the rest of
  // the selector based on the character at the end position
  if (end_pos != selector.size()) {
    // get the rest of the selector
    string rest = selector.substr(end_pos + 1);
    // parse the rest of the selector
    queue<custom::xpath::xpath_node> rest_xpath_queue;
    switch (selector[end_pos]) {
    case ' ':
      rest_xpath_queue = parse(rest, AXIS_DESCENDANT);
      break;
    case '>':
      rest_xpath_queue = parse(rest, AXIS_CHILD);
      break;
    case '+':
      rest_xpath_queue = parse(rest, AXIS_NEARBY_SIBLING);
      break;
    case '~':
      rest_xpath_queue = parse(rest, AXIS_SIBLING);
      break;
    }
    // push the rest of the xpath nodes to the queue
    while (not rest_xpath_queue.empty()) {
      xpath_queue.push(rest_xpath_queue.front());
      rest_xpath_queue.pop();
    }
  }

  return xpath_queue;
}
