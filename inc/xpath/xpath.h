//
// Created by 丁麓然 on 10/25/23.
//

#ifndef HTML_PARSER_XPATH_PARSER_H
#define HTML_PARSER_XPATH_PARSER_H

#include "../htmlparser.h"
#include <istream>
#include <memory>
#include <queue>
#include <string>

#define AXIS_CHILD 0
#define AXIS_DESCENDANT 1
#define AXIS_SELF 2
#define AXIS_PARENT 3
#define AXIS_ANCESTOR 4
#define AXIS_SIBLING 5

namespace custom {

using std::string, std::queue, std::shared_ptr;

class basic_xpath_node {
private:
  struct predicates {
    list<int> positions;
    string attribute;
    string value;

    predicates() = default;
    predicates(const predicates &p)
        : positions(p.positions), attribute(p.attribute), value(p.value) {}
    predicates(const string &s);
    ~predicates() = default;
  };

  predicates _predicates;

protected:
  string _name;
  char _axis;

  virtual void _parse(const string &) = 0;
  void set_predicates(const string &s) { _predicates = predicates(s); }

public:
  basic_xpath_node() = default;
  basic_xpath_node(const string &name) : _name(name) {}
  basic_xpath_node(const string &name, const char axis,
                   const string &predicates = "")
      : _name(name), _axis(axis), _predicates(predicates) {}
  virtual ~basic_xpath_node() = default;

  char axis() const { return _axis; }
};

class element_xpath_node : public basic_xpath_node {
private:
  void _parse(const string &) override;

public:
  element_xpath_node() = default;
  element_xpath_node(const string &s) : basic_xpath_node() { _parse(s); }
  element_xpath_node(const string &name, const char axis,
                     const string &predicates = "")
      : basic_xpath_node(name, axis, predicates) {}
  ~element_xpath_node() override {}

  string element_name() const { return _name; }
};

class attribute_xpath_node : public basic_xpath_node {
private:
  string _value;

  void _parse(const string &) override;

public:
  attribute_xpath_node() = default;
  attribute_xpath_node(const string &s) : basic_xpath_node(s) { _parse(s); }
  attribute_xpath_node(const string &attr, const string &value, const char axis,
                       const string &predicates = "")
      : basic_xpath_node(attr, axis, predicates), _value(value) {}
  ~attribute_xpath_node() override {}

  string value() const { return _value; }
  string attribute_name() const { return _name; }
  string attribute_value() const { return _value; }
  bool has_value() const { return !_value.empty(); }
};

class xpath {
private:
  queue<shared_ptr<basic_xpath_node>> _xpath_queue;

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
  char axis() const { return _xpath_queue.front()->axis(); }
  shared_ptr<basic_xpath_node> front() const { return _xpath_queue.front(); }
  void next() { _xpath_queue.pop(); }
};
}; // namespace custom

#endif // HTML_PARSER_XPATH_PARSER_H
