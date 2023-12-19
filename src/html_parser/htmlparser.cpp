#include "../../inc/xpath/xpath.h"
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using custom::xpath, custom::basic_xpath_constraint,
    custom::element_xpath_constraint, custom::attribute_xpath_constraint;
using std::string, std::shared_ptr, std::dynamic_pointer_cast, std::function,
    std::list, std::stack;

bool _text_only;

namespace custom {
template <> inline tree<html::_element>::operator string() const {
  static int indent = 0;
  static bool new_line_tag = false;

  string result;
  bool parent_new_line_tag = new_line_tag;
  if (_tree->parent != nullptr)
    new_line_tag = _tree->data.data.tag->new_line_tag();
  if (_tree == nullptr) {
    return result;
  }
  // if _element is a tag, add tag content
  if (_tree->data.is_tag) {
    // add indent if the parent is not a new line tag
    if (parent_new_line_tag) {
      result += '\n';
      for (int i = 0; i < indent * INDENT_SIZE; i++) {
        result += ' ';
      }
    }
    if (not _text_only) {
      result += _tree->data.data.tag->begin_tag();
    }
    // increase indent and add children
    indent++;
    for (auto &child : _tree->children) {
      result += string(tree<html::_element>(child));
    }
    // reduce indent and add end tag
    indent--;
    // add a new line and ident for the end tag
    if (_tree->data.data.tag->new_line_tag()) {
      if (not _text_only) {
        result += '\n';
      }
      for (int i = 0; i < indent * INDENT_SIZE; i++) {
        result += ' ';
      }
    }
    if (not _text_only) {
      result += _tree->data.data.tag->end_tag();
    }
  }
  // if _element is not a tag, insert raw content into result
  else {
    // add indent if the parent is not a new line tag
    if (parent_new_line_tag) {
      for (int i = 0; i < indent * INDENT_SIZE; i++) {
        result += ' ';
      }
    }
    if (not(_tree->children.empty())) {
      return "";
    }
    result = string(_tree->data.data.content_idx.first,
                    _tree->data.data.content_idx.second);
  }
  // restore new_line_tag
  new_line_tag = parent_new_line_tag;

  return result;
}
} // namespace custom

void html::_tag::_parse(const string &s) {
  if (s[0] != '<') {
    throw std::runtime_error("Not a tag");
  }
  if (s[1] == '/') {
    throw std::runtime_error("Not a beginning tag");
  }

  // get the name of the tag, which is the first word
  int pos = 1;
  while (s[pos] != ' ' and s[pos] != '>') {
    ++pos;
  }
  _name = s.substr(1, pos - 1);

  // get the attributes of the tag
  while (s[pos] != '>') {
    if (s.substr(pos, s.find_first_of('>') + 1).find('=') == string::npos) {
      break;
    }
    // skip the space
    while (s[pos] == ' ') {
      ++pos;
    }
    // get the attribute name
    int start = pos;
    while (s[pos] != ' ' and s[pos] != '=') {
      ++pos;
    }
    string attribute_name = s.substr(start, pos - start);
    // skip the space
    while (s[pos] == ' ') {
      ++pos;
    }
    // skip the '='
    ++pos;
    // skip the space, and the '"' if there is
    while (s[pos] == ' ') {
      ++pos;
    }
    if (s[pos] == '"') {
      ++pos;
    }
    // get the attribute value, which is the content between the start '"' and
    // the end '"'
    start = pos;
    while (s[pos] != '"') {
      ++pos;
    }
    string attribute_value = s.substr(start, pos - start);
    _attributes.insert(std::make_pair(attribute_name, attribute_value));
    // skip the '"'
    pos++;
  }

  // check if the tag is self-closing
  if (_name == "br" or _name == "hr" or _name == "img" or _name == "input" or
      _name == "link" or _name == "meta" or _name == "param" or
      _name == "area" or _name == "base" or _name == "!DOCTYPE") {
    _self_closing = true;
  } else {
    _self_closing = false;
  }
}

void html::_tag::_new_line_tag_init() {
  _new_line_tag = _no_newline_tags.find(_name) == _no_newline_tags.end();
}

bool html::_tag::operator==(const string &s) const {
  // TODO: make the code more robust
  if (s[0] != '<') {
    throw std::runtime_error("Not a tag");
  }
  // get the name of the tag, no matter it is a beginning tag or a close tag
  std::cout << "s: " << s << std::endl;
  int pos = s[1] == '/' ? 2 : 1;
  while (s[pos] != ' ' and s[pos] != '>') {
    ++pos;
  }
  string tag_name = s.substr(1, pos - 1);
  return _name == tag_name;
}

bool html::_tag::operator!=(const string &s) const {
  // TODO: make the code more robust
  if (s[0] != '<') {
    throw std::runtime_error("Not a tag");
  }
  // get the name of the tag, no matter it is a beginning tag or a close tag
  int pos =
      s[1] == '/' ? s.find_first_not_of(' ', 2) : s.find_first_not_of(' ', 1);
  int start = pos;
  while (s[pos] != ' ' and s[pos] != '>') {
    ++pos;
  }
  string tag_name = s.substr(start, pos - start);
  string name = _name;
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  std::transform(tag_name.begin(), tag_name.end(), tag_name.begin(), ::tolower);
  return name != tag_name;
}

bool html::_tag::is_self_closing_tag() const { return _self_closing; }

bool html::_tag::has_attribute(const string &attr_name) const {
  return _attributes.find(attr_name) != _attributes.end();
}

string html::_tag::attribute(const string &attr_name) const {
  if (not has_attribute(attr_name)) {
    throw std::runtime_error("Attribute not found");
  }
  return _attributes.at(attr_name);
}

bool html::_tag::new_line_tag() const {
  return _new_line_tag and not(_self_closing);
}

string html::_tag::begin_tag() const {
  string tag_str;
  tag_str += '<';
  tag_str += _name;
  for (auto &attribute : _attributes) {
    tag_str += ' ';
    tag_str += attribute.first;
    tag_str += '=';
    tag_str += '"';
    tag_str += attribute.second;
    tag_str += '"';
  }
  tag_str += '>';
  return tag_str;
}

string html::_tag::end_tag() const {
  if (_self_closing) {
    return "";
  }
  return "</" + _name + ">";
}

void html::_clear_comment() {
  int start = 0;
  while ((start = _clear_content.find("<!DOCTYPE", start)) != string::npos) {
    int end = _clear_content.find(">", start);
    if (end == string::npos)
      throw std::runtime_error("DOCTYPE not closed");
    _clear_content.erase(start, end - start + 1);
  }
  start = 0;
  while ((start = _clear_content.find("<!doctype", start)) != string::npos) {
    int end = _clear_content.find(">", start);
    if (end == string::npos)
      throw std::runtime_error("DOCTYPE not closed");
    _clear_content.erase(start, end - start + 1);
  }
  start = 0;
  while ((start = _clear_content.find("<!--", start)) != string::npos) {
    int end = _clear_content.find("-->", start);
    if (end == string::npos)
      throw std::runtime_error("Comment not closed");
    _clear_content.erase(start, end - start + 3);
  }
}

void html::_clear_script() {
  int start = 0;
  while ((start = _clear_content.find("<script", start)) != string::npos) {
    int end = _clear_content.find("</script>", start);
    if (end == string::npos)
      throw std::runtime_error("Script not closed");
    _clear_content.erase(start, end - start + 9);
  }
}

void html::_clear() {
  _clear_comment();
  _clear_script();
  _clear_content.erase(std::remove_if(_clear_content.begin(),
                                      _clear_content.end(),
                                      [](const char &c) { return c == '\n'; }),
                       _clear_content.end());
  _clear_content.erase(std::remove_if(_clear_content.begin(),
                                      _clear_content.end(),
                                      [](const char &c) { return c == '\t'; }),
                       _clear_content.end());
  _clear_content.erase(std::remove_if(_clear_content.begin(),
                                      _clear_content.end(),
                                      [](const char &c) { return c == '\r'; }),
                       _clear_content.end());
}

int html::_find_tag_end(const int &start) const {
  // TODO: make the code more robust
  /*
   * Find the end of the tag in the clear content
   * @param start: the position to start searching(inclusive)
   * @return: the exceeded position of the end of the tag(exclusive)
   */
  if (_clear_content[start] != '<') {
    throw std::runtime_error("Not a tag");
  }
  int end = start;
  while (_clear_content[end] != '>') {
    ++end;
    if (end >= _clear_content.size()) {
      return -1;
    }
  }

  return end + 1;
}

int html::_find_next_tag(const int &start) const {
  // TODO: make the code more robust
  /*
   * Find the next tag in the clear content
   * @param start: the position to start searching(inclusive)
   * @return: the position of the next tag
   */
  int end = start;
  while (_clear_content[end] != '<') {
    ++end;
  }
  return end;
}

string html::_get_tag_content(const int &start) const {
  /*
   * Get the content of the tag
   * @param start: the position of the tag
   * @return: the content of the tag
   */
  if (_clear_content[start] != '<') {
    throw std::runtime_error("Not a tag");
  }
  int end = _find_tag_end(start);
  return _clear_content.substr(start, end - start);
}

int html::_find_close_tag(const int &start) const {
  /*
   * Find the close tag of the tag at the start position
   * @param start: the position of the tag
   * @return: the exceeded end position of the close tag
   */
  if (_clear_content[start] != '<') {
    throw std::runtime_error("Not a tag");
  }
  if (_clear_content[start + 1] == '/') {
    throw std::runtime_error("Not a beginning tag");
  }
  int next_tag_pos = start;
  stack<_tag> tag_stack;
  do {
    // when loop start, next_tag_pos must be the position a tag
    next_tag_pos = _find_next_tag(next_tag_pos);
    // if there is an begin tag in the stack while the next tag is not found
    if (next_tag_pos == -1) {
      throw std::runtime_error("Invalid HTML file");
    }
    string tag_str = _get_tag_content(next_tag_pos);
    if (_is_beginning_tag(next_tag_pos)) {
      if (not _tag(tag_str).is_self_closing_tag()) {
        tag_stack.push(_tag(tag_str));
      }
    } else {
      if (tag_stack.top() != tag_str) {
        throw std::runtime_error("Invalid HTML file");
      }
      tag_stack.pop();
    }
    // when loop end, next_tag_pos must be the end position of a tag so that the
    // search process can continue
    next_tag_pos = _find_tag_end(next_tag_pos);
  } while (!tag_stack.empty());

  return next_tag_pos;
}

bool html::_is_beginning_tag(const int &start) const {
  /*
   * Check if the tag is a beginning tag
   * @param start: the position of the tag
   * @return: true if the tag is a beginning tag
   */
  if (_clear_content[start] != '<') {
    throw std::runtime_error("Not a tag");
  }
  return _clear_content[start + 1] != '/';
}

tree<html::_element> html::_parse() const {
  if (_clear_content[0] != '<') {
    throw std::runtime_error("Invalid HTML file");
  }
  tree<_element> root;
  int end;
  try {
    end = _find_close_tag(0);
  } catch (std::runtime_error &e) {
    throw e;
  }
  root = _parse(std::make_pair(0, end));

  return root;
}

tree<html::_element> html::_parse(const pair<int, int> &scope) const {
  const int start = scope.first, end = scope.second;
  if (_clear_content[start] != '<') {
    throw std::runtime_error("Not the start of a tag");
  }
  if (_clear_content[start + 1] == '/') {
    throw std::runtime_error("Not a beginning tag");
  }
  // tag_end_pos is the exceeded end position of the tag, next_tag_pos is the
  // first position of the next tag
  int tag_end_pos = _find_tag_end(start),
      next_tag_pos = _find_next_tag(tag_end_pos);
  _tag tag(_get_tag_content(start));
  tree<_element> root = tree<_element>(_element(tag));
  // if the tag is a self-closing tag, return the root
  if (tag.is_self_closing_tag()) {
    return root;
  }
  // using while loop to add all the children recursively using the same
  // function
  do {
    if (next_tag_pos == -1) {
      throw std::runtime_error("Invalid HTML file");
    }
    if (next_tag_pos > end) {
      throw std::runtime_error("Invalid HTML file");
    }
    // get the content between the tags and check if it is space, if not, add it
    // as a content in this tag
    string content =
        _clear_content.substr(tag_end_pos, next_tag_pos - tag_end_pos);
    if (content.find_first_not_of(' ') != string::npos) {
      root.add_child(
          _element(std::make_pair(_clear_content.begin() + tag_end_pos,
                                  _clear_content.begin() + next_tag_pos)));
    }
    // if the next tag is a beginning tag, parse the tag and add it as a child
    if (_is_beginning_tag(next_tag_pos)) {
      try {
        tag_end_pos = _find_close_tag(next_tag_pos);
      } catch (std::runtime_error &e) {
        throw e;
      }
      root.add_child(_parse(std::make_pair(next_tag_pos, tag_end_pos)));
    } else {
      // if the next tag is a close tag, check if it is the close tag of this
      // tag
      if (tag != _get_tag_content(next_tag_pos)) {
        throw std::runtime_error("Invalid HTML file");
      }
      break;
    }
    next_tag_pos = _find_next_tag(tag_end_pos);
  } while (next_tag_pos != -1);

  return root;
}

string html::show(xpath &path, const bool text_only) const {
  _text_only = text_only;
  string result;
  list<tree<html::_element>> elements;
  // check if the xpath is absolute
  // if the xpath is absolute, then the root is the html tag
  if (path.axis() == AXIS_CHILD) {
    elements.push_back(_html);
  } else if (path.axis() == AXIS_ALL) {
    elements = _html.find_all(path.constraint());
    path.next();
  }
  // if the xpath is relative, then the root should be found by the xpath
  else {
    elements = _html.find_all(path.constraint());
    path.next();
  }
  // find all the elements that match the xpath
  while (not elements.empty() and not path.empty()) {
    list<tree<html::_element>> new_elements;
    char axis = path.axis();
    for (auto &e : elements) {
      list<tree<html::_element>> t;
      switch (axis) {
      case AXIS_SELF:
        t = {e};
        break;
      case AXIS_DESCENDANT:
        t = e.find_all(path.constraint());
        break;
      case AXIS_CHILD:
        t = e.find(path.constraint());
        break;
      case AXIS_PARENT:
        t = {e.parent()};
      case AXIS_SIBLING:
        t = e.find_sibling(path.constraint());
        break;
      case AXIS_ALL:
        t = e.all();
        break;
      }
      if (not t.empty()) {
        new_elements.insert(new_elements.end(), t.begin(), t.end());
      }
    }
    elements = new_elements;
    path.next();
  }
  // convert each element to string and add them to result
  for (auto &e : elements) {
    result += string(e);
    result += "\n\n";
  }
  // remove the last two '\n'
  if (not result.empty()) {
    result.erase(result.end() - 1, result.end());
  }
  // output the number of elements found
  result += "\nFound " + std::to_string(elements.size()) + " elements";

  return result;
}
