//
// Created by 丁麓然 on 10/25/23.
//

#include "../../inc/xpath/xpath.h"
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>

namespace custom {

using std::string, std::queue, std::shared_ptr;

basic_xpath_node::predicates::predicates(const string &s) {
  // if the predicates is empty, then return
  if (s.empty()) {
    return;
  }
  // if not start with '[' or end with ']', then throw an exception
  if (s[0] != '[' or s[s.size() - 1] != ']') {
    throw std::invalid_argument("invalid predicates");
  }
  // get the predicates string
  string predicates_str = s.substr(1, s.size() - 2);
  // check if the predicates_str is empty
  if (predicates_str.empty()) {
    throw std::invalid_argument("invalid predicates");
  }
  // if contains '@', then it is an attribute predicates
  if (predicates_str.find('@') != string::npos) {
    // skip the '@'
    predicates_str = predicates_str.substr(predicates_str.find('@') + 1);
    // check if there is a value, i.e. there is a '='
    if (predicates_str.find('=') == string::npos) {
      attribute = predicates_str;
    }
    // if there is a '=', then get the attribute name and value
    else {
      // get the attribute name
      attribute = predicates_str.substr(1, predicates_str.find('=') - 1);
      // get the attribute value
      value = predicates_str.substr(predicates_str.find('=') + 2,
                                    predicates_str.size() -
                                        predicates_str.find('=') - 3);
    }
  } else {
    // if not contains '@', then it is a position predicates
    // TODO: implement the position predicates
    throw std::runtime_error("position predicates is not implemented");
  }
}

void element_xpath_node::_parse(const string &node_str) {
  // check if the node_str is empty
  if (node_str.empty() or node_str.find_first_not_of(' ') == string::npos) {
    throw std::invalid_argument("empty xpath");
  }

  // create a string to store the current node
  int start_pos = 0;
  int end_pos;
  // if start_pos is '/', then check if double slash
  if (node_str[start_pos] == '/') {
    // check if the xpath_str is only '/'
    if (node_str.size() == 1) {
      throw std::invalid_argument("invalid xpath");
    }
    // if double slash
    if (node_str[start_pos + 1] == '/') {
      _axis = AXIS_DESCENDANT;
      // skip the second slash and find the next slash
      end_pos = node_str.find_first_of("/[", start_pos + 2);
      _name = node_str.substr(start_pos + 2, end_pos - start_pos - 2);
    }
    // single slash
    else {
      // set the axis to child
      _axis = AXIS_CHILD;
      // find the next slash
      end_pos = node_str.find_first_of("/[", start_pos + 1);
      _name = node_str.substr(start_pos + 1, end_pos - start_pos - 1);
    }
  }
  // if start_pos is not '/'
  else {
    // set the axis to descendant
    _axis = AXIS_DESCENDANT;
    // find the next slash
    end_pos = node_str.find_first_of("/[", start_pos);
    _name = node_str.substr(start_pos, end_pos - start_pos);
  }
  // if slash not found, then the end position is the end of the xpath_str
  end_pos = (end_pos == string::npos) ? node_str.size() : end_pos;
  // check if the name is "."
  if (_name == ".") {
    _name = "";
    _axis = AXIS_SELF;
  }
  // check if the name is ".."
  else if (_name == "..") {
    _name = "";
    _axis = AXIS_PARENT;
  }
  // TODO: add full predicates support
  // check if the element has a predicates
  if (node_str.find_first_of('[') != string::npos) {
    // check if the element ends with ']'
    if (node_str[node_str.size() - 1] != ']') {
      throw std::invalid_argument("invalid xpath");
    }
    set_predicates(node_str.substr(node_str.find_first_of('[')));
  }
}

void attribute_xpath_node::_parse(const string &node_str) {
  // check if the node_str is empty
  if (node_str.empty() or node_str.find_first_not_of(' ') == string::npos) {
    throw std::invalid_argument("empty xpath");
  }
  // check if the node_str is an attribute node
  if (node_str.find('@') == string::npos) {
    throw std::invalid_argument("invalid xpath");
  }

  // create a string to store the current node (both attribute name and value)
  int start_pos = 0;
  int end_pos;
  string attr_val_str;
  // if start_pos is '/', then check if double slash
  if (node_str[start_pos] == '/') {
    // check if the xpath_str is only '/'
    if (node_str.size() == 1) {
      throw std::invalid_argument("invalid xpath");
    }
    // if double slash
    if (node_str[start_pos + 1] == '/') {
      _axis = AXIS_DESCENDANT;
      // skip the second slash and find the next slash
      end_pos = node_str.find_first_of("/[", start_pos + 2);
      attr_val_str = node_str.substr(start_pos + 2, end_pos - start_pos - 2);
    }
    // single slash
    else {
      // set the axis to child
      _axis = AXIS_CHILD;
      // find the next slash
      end_pos = node_str.find_first_of("/[", start_pos + 1);
      attr_val_str = node_str.substr(start_pos + 1, end_pos - start_pos - 1);
    }
  }
  // if start_pos is not '/'
  else {
    // set the axis to descendant
    _axis = AXIS_DESCENDANT;
    // find the next slash
    end_pos = node_str.find_first_of("/[", start_pos);
    attr_val_str = node_str.substr(start_pos, end_pos - start_pos);
  }
  // if slash not found, then the end position is the end of the xpath_str
  end_pos = (end_pos == string::npos) ? node_str.size() : end_pos;
  // check if the attribute has a value
  // if has a value, then the attribute name and value are separated by '='
  if (attr_val_str.find('=') != string::npos) {
    // skip the '@'
    start_pos = attr_val_str.find('@') + 1;
    if (start_pos == attr_val_str.size() or attr_val_str[start_pos] == ' ') {
      throw std::invalid_argument("invalid xpath");
    }
    // get the attribute name
    _name = attr_val_str.substr(start_pos, attr_val_str.find('=') - start_pos);
    // reset the start_pos and end_pos
    start_pos =
        attr_val_str.find_first_not_of(" '\"", attr_val_str.find('=') + 1);
    end_pos = attr_val_str.find_first_of(" '\"", start_pos);
    // get the attribute value
    _value = attr_val_str.substr(start_pos, end_pos - start_pos);
  }
  // if attr has no value, then the attribute name is the whole string
  else {
    // skip the '@'
    start_pos = attr_val_str.find('@') + 1;
    // get the attribute name
    _name = attr_val_str.substr(attr_val_str.find_first_not_of(' ', start_pos));
    // set value to empty string
    _value = "";
  }
  // TODO: add basic support for predicates
  // check if the attribute has a predicates
  if (node_str.find_first_of('[') != string::npos) {
    set_predicates(node_str.substr(node_str.find_first_of('[')));
  }
}

void xpath::_parse(const string &xpath_str) {
  // TODO: add multiple xpath support
  // check if the xpath_str is empty
  if (xpath_str.empty() or xpath_str.find_first_not_of(' ') == string::npos) {
    throw std::invalid_argument("empty xpath");
  }

  queue<shared_ptr<basic_xpath_node>> parse(const string &xpath_str);

  queue<shared_ptr<basic_xpath_node>> result;
  int start_pos = 0;
  // basic error checking
  if (xpath_str[0] == '/') {
    // if the xpath_str is only '/', then throw an exception
    if (xpath_str.size() == 1) {
      throw std::invalid_argument("invalid xpath");
    }
    // if the xpath_str is start with '//', then the base axis is descendant
    else if (xpath_str[1] == '/') {
      if (xpath_str.size() == 2) {
        throw std::invalid_argument("invalid xpath");
      }
    }
  }
  result = parse(xpath_str);

  _xpath_queue = result;
}

// this function recursively parse the xpath_str and return a queue of xpath
// nodes
queue<shared_ptr<basic_xpath_node>> parse(const string &xpath_str) {
  // check if the xpath_str is empty
  if (xpath_str.empty() or xpath_str.find_first_not_of(' ') == string::npos) {
    return queue<shared_ptr<basic_xpath_node>>();
  }

  // create a queue to store the result
  queue<shared_ptr<basic_xpath_node>> result;
  // create a string to store the current node
  int start_pos = xpath_str.find_first_not_of(' ');
  int end_pos;
  // if start_pos is '/', then check if double slash
  if (xpath_str[start_pos] == '/') {
    // check if the xpath_str is only '/'
    if (xpath_str.size() == 1) {
      throw std::invalid_argument("invalid xpath");
    }
    // if double slash, then skip the second slash and find the next slash
    if (xpath_str[start_pos + 1] == '/') {
      end_pos = xpath_str.find_first_of('/', start_pos + 2);
    }
    // else find the next slash
    else {
      end_pos = xpath_str.find_first_of('/', start_pos + 1);
    }
  }
  // if start_pos is not '/', then find the next slash
  else {
    end_pos = xpath_str.find_first_of('/', start_pos);
    // if slash not found, then the end position is the end of the xpath_str
  }
  // if end_pos is not found, then the end position is the end of the xpath_str
  end_pos = (end_pos == string::npos) ? xpath_str.size() : end_pos;
  string node_str = xpath_str.substr(start_pos, end_pos - start_pos);
  // parse the current node
  // if right after slash is '@', then it is an attribute node
  if (node_str.find('@') != string::npos) {
    result.push(
        std::make_shared<attribute_xpath_node>(attribute_xpath_node(node_str)));
  }
  // if not start with '@', then it is an element node
  else {
    result.push(
        std::make_shared<element_xpath_node>(element_xpath_node(node_str)));
  }
  // parse the rest of the xpath_str recursively
  queue<shared_ptr<basic_xpath_node>> rest = parse(xpath_str.substr(end_pos));
  // add the rest of the xpath_str to the result
  while (not rest.empty()) {
    result.push(rest.front());
    rest.pop();
  }

  return result;
}

}; // namespace custom
