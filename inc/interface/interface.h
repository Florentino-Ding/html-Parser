//
// Created by 丁麓然 on 10/25/23.
//

#ifndef HTML_PARSER_INTERFACE_H
#define HTML_PARSER_INTERFACE_H

#include "../css_selector/selector.h"
#include "../xpath/xpath.h"
#include <string>

namespace interface {

using custom::xpath;
using std::string;
class CLTinterface {
private:
  static string _help_info;
  html _html;
  xpath _xpath;
  string _html_path;

  string _get_input_instruction(const string &);
  string _get_input_arg(const string &);
  char _get_user_input();
  void _load_html();
  void _show_html(const bool _text_only = false);

public:
  CLTinterface() {}
  CLTinterface(const string &html_path) : _html_path(html_path) {
    _load_html();
  }
  ~CLTinterface() {}

  void run();
};
}; // namespace interface

#endif // HTML_PARSER_INTERFACE_H
