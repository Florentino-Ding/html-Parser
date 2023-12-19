//
// Created by 丁麓然 on 10/25/23.
//

#ifndef HTML_PARSER_XPATH_PARSER_H
#define HTML_PARSER_XPATH_PARSER_H

#include "../htmlparser.h"
#include <functional>
#include <initializer_list>
#include <istream>
#include <memory>
#include <queue>
#include <string>

#define AXIS_CHILD 0
#define AXIS_DESCENDANT 1
#define AXIS_SELF 2
#define AXIS_PARENT 3
#define AXIS_NEARBY_SIBLING 4
#define AXIS_SIBLING 5
#define AXIS_ALL 6

#define LOGIC_OR 0
#define LOGIC_AND 1

namespace custom {

using std::string, std::queue, std::shared_ptr, std::function;

class xpath;

class basic_xpath_constraint {
protected:
  string _name;

  virtual void _parse(const string &) = 0;

public:
  basic_xpath_constraint() = default;
  basic_xpath_constraint(const string &name) : _name(name) {}
  virtual ~basic_xpath_constraint() = default;

  virtual bool check(const html::_element &) const = 0;
};

class element_xpath_constraint : public basic_xpath_constraint {
private:
  void _parse(const string &) override;

public:
  element_xpath_constraint() = default;
  element_xpath_constraint(const string &s) : basic_xpath_constraint() {
    _parse(s);
  }
  ~element_xpath_constraint() override {}

  string element_name() const { return _name; }
  bool check(const html::_element &e) const override {
    if (not e.is_tag) {
      return false;
    }
    return e.data.tag->name() == _name;
  }
};

class attribute_xpath_constraint : public basic_xpath_constraint {
private:
  string _value;

  void _parse(const string &) override;

public:
  attribute_xpath_constraint() = default;
  attribute_xpath_constraint(const string &s) : basic_xpath_constraint(s) {
    _parse(s);
  }
  attribute_xpath_constraint(const string &attr, const string &value)
      : basic_xpath_constraint(attr), _value(value) {}
  ~attribute_xpath_constraint() override {}

  string attribute_name() const { return _name; }
  string attribute_value() const { return _value; }
  bool has_value() const { return !_value.empty(); }
  bool check(const html::_element &e) const override {
    if (not e.is_tag) {
      return false;
    }
    return e.data.tag->has_attribute(_name) and
           (not has_value() or e.data.tag->attribute(_name) == _value);
  }
};

class xpath {
public:
  struct xpath_node {
    char _axis;
    char _logic;
    list<shared_ptr<basic_xpath_constraint>> constraints;

    void _parse(const string &node_str);

    xpath_node() : _axis(AXIS_SELF), _logic(LOGIC_AND) {}
    xpath_node(const xpath_node &x)
        : constraints(x.constraints), _axis(x._axis), _logic(x._logic) {}
    xpath_node(char axis) {
      if (axis != AXIS_PARENT and axis != AXIS_SELF and axis != AXIS_ALL) {
        throw std::invalid_argument("invalid axis");
      }
      _axis = axis;
      _logic = LOGIC_AND;
    }
    xpath_node(const string &node_str) { _parse(node_str); }

    char axis() const { return _axis; }
    function<bool(html::_element)> constraint() const;
    void add_constraint(const shared_ptr<basic_xpath_constraint> &c) {
      constraints.push_back(c);
    }
  };

protected:
  queue<xpath_node> _xpath_queue;

private:
  void _parse(const string &);

public:
  xpath() = default;
  xpath(const string &s) { _parse(s); }
  xpath(const xpath &x) : _xpath_queue(x._xpath_queue) {}

  friend std::istream &operator>>(std::istream &is, xpath &x) {
    string s;
    is >> s;
    x._parse(s);
    return is;
  }

  bool empty() const { return _xpath_queue.empty(); }
  char axis() const { return _xpath_queue.front().axis(); }
  function<bool(html::_element)> constraint() const {
    return _xpath_queue.front().constraint();
  }
  void next() { _xpath_queue.pop(); }
};
}; // namespace custom

#endif // HTML_PARSER_XPATH_PARSER_H
