//
// Created by 丁麓然 on 10/25/23.
//

#include "../../inc/xpath/xpath.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

namespace custom {
void element_xpath_constraint::_parse(const string &node_str) {
  // check if the node_str is empty
  if (node_str.empty() or node_str.find_first_not_of(' ') == string::npos) {
    throw std::invalid_argument("empty xpath");
  }

  // create a string to store the current node
  int start_pos = node_str.find_first_not_of(' ');
  int end_pos = node_str.find_first_of(" [", start_pos);
  _name = node_str.substr(start_pos, end_pos - start_pos);
  // TODO: add full predicates support
  // check if the element has a predicates
  if (node_str.find_first_of('[') != string::npos) {
    // check if the element ends with ']'
    if (node_str[node_str.size() - 1] != ']') {
      throw std::invalid_argument("invalid xpath");
    }
  }
}

void attribute_xpath_constraint::_parse(const string &node_str) {
  // check if the node_str is empty
  if (node_str.empty() or node_str.find_first_not_of(' ') == string::npos) {
    throw std::invalid_argument("empty xpath");
  }
  // check if the node_str is an attribute node
  if (node_str.find('@') == string::npos) {
    throw std::invalid_argument("invalid xpath");
  }

  // create a string to store the current node (both attribute name and value)
  int start_pos = node_str.find_first_not_of(' ');
  int end_pos = node_str.find_first_of('[', start_pos);
  // if slash not found, then the end position is the end of the xpath_str
  end_pos = (end_pos == string::npos) ? node_str.size() : end_pos;
  string attr_val_str = node_str.substr(start_pos, end_pos - start_pos);
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
}

void xpath::xpath_node::_parse(const string &node_str) {
  int start_pos = node_str.find_first_not_of(' ');
  // check if the node_str is empty
  if (node_str.empty() or start_pos == string::npos) {
    throw std::invalid_argument("empty xpath");
  }

  int end_pos = node_str.find_first_of('|', start_pos);
  string content = node_str.substr(start_pos);
  // check whether node_str's prefix is "//", "/" or nothing
  // if first char is '/'
  if (content[0] == '/') {
    // check if the node_str is only '/'
    if (content.size() == 1 or content.find_first_not_of(' ') == string::npos) {
      throw std::invalid_argument("invalid xpath");
    }

    // check if the node_str is start with '//'
    if (content[1] == '/') {
      _axis = AXIS_DESCENDANT;
      // skip the second slash and find the next slash
      start_pos = content.find_first_not_of(' ', 2);
    }
    // else if the node_str is single slash
    else {
      // set the axis to child
      _axis = AXIS_CHILD;
      // reset the start_pos
      start_pos = content.find_first_not_of(' ', 1);
    }
  }
  // first char isn't '/'
  else {
    // set the axis to descendant
    _axis = AXIS_DESCENDANT;
    // reset the start_pos
    start_pos = content.find_first_not_of(' ');
  }
  // using loop to parse the node_str, divided by '|', each time using the
  // start_pos and end_pos to get the current node
  while (start_pos != string::npos) {
    // if next '|' not found, then the end position is the end of the
    // node_str
    if (end_pos == string::npos) {
      end_pos = content.size();
    }
    // get the current node
    string node = content.substr(start_pos, end_pos - start_pos);
    // check if the node_str is empty
    if (node.empty() or node.find_first_not_of(' ') == string::npos) {
      throw std::invalid_argument("invalid xpath");
    }

    // check if the node is an element node or attribute node
    if (node.find('@') == string::npos) {
      // check if the node is ".." or "."
      if (node == ".." or node == ".") {
        // check if the node is the only node
        if (constraints.size() != 0) {
          throw std::invalid_argument("invalid xpath");
        }
        _axis = (node == "..") ? AXIS_PARENT : AXIS_SELF;
      }
      constraints.push_back(std::make_shared<element_xpath_constraint>(node));
    } else {
      constraints.push_back(std::make_shared<attribute_xpath_constraint>(node));
    };

    // check if the end_pos is the end of the node_str
    if (end_pos == content.size()) {
      break;
    }
    // reset the start_pos and end_pos
    start_pos = content.find_first_of('|', end_pos + 1);
    // if next '|' not found, then the end position is the end of the
    if (start_pos != string::npos) {
      break;
    }
    end_pos = content.find_first_of('|', start_pos + 1);
  }
  _logic = LOGIC_OR;
}

std::function<bool(html::_element)> xpath::xpath_node::constraint() const {
  using std::function;

  function<bool(html::_element)> constraint;
  // if logic is and
  if (_logic == LOGIC_AND) {
    constraint = [this](html::_element node) -> bool {
      for (auto &c : constraints) {
        if (not c->check(node)) {
          return false;
        }
      }
      return true; // Placeholder return statement
    };
  }
  // else if logic is or
  else {
    constraint = [this](html::_element node) -> bool {
      for (auto &c : constraints) {
        if (c->check(node)) {
          return true;
        }
      }
      return false; // Placeholder return statement
    };
  }
  return constraint;
}

void xpath::_parse(const string &xpath_str) {
  // TODO: add multiple xpath support
  // check if the xpath_str is empty
  if (xpath_str.empty() or xpath_str.find_first_not_of(' ') == string::npos) {
    throw std::invalid_argument("empty xpath");
  }

  queue<xpath_node> parse(const string &xpath_str);

  queue<xpath_node> result;
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
queue<xpath::xpath_node> parse(const string &xpath_str) {
  // check if the xpath_str is empty
  if (xpath_str.empty() or xpath_str.find_first_not_of(' ') == string::npos) {
    return queue<xpath::xpath_node>();
  }

  // create a queue to store the result
  queue<xpath::xpath_node> result;
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
  // if end_pos is not found, then the end position is the end of the
  // xpath_str
  end_pos = (end_pos == string::npos) ? xpath_str.size() : end_pos;
  string node_str = xpath_str.substr(start_pos, end_pos - start_pos);
  // parse the current node
  result.push(xpath::xpath_node(node_str));
  // parse the rest of the xpath_str recursively
  queue<xpath::xpath_node> rest = parse(xpath_str.substr(end_pos));
  // add the rest of the xpath_str to the result
  while (not rest.empty()) {
    result.push(rest.front());
    rest.pop();
  }

  return result;
}
}; // namespace custom
