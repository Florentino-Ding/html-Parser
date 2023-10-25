//
// Created by 丁麓然 on 10/25/23.
//

#ifndef HTML_PARSER_INTERFACE_H
#define HTML_PARSER_INTERFACE_H

#include "../htmlparser.h"
#include "../xpath_parser/xpath_parser.h"
#include <string>
namespace interface {

using custom::xpath;
using std::string;
class CLTinterface {
private:
  static string _help_info;
  html _html;
  xpath _xpath;

  char _get_user_input();
  void _load_html(const string &html_path);
  void _show_html();

public:
  CLTinterface() {}
  CLTinterface(const string &html_path) { _load_html(html_path); }
  ~CLTinterface() {}

  void run();
};
}; // namespace interface

#endif // HTML_PARSER_INTERFACE_H
