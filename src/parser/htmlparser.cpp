#include "../../inc/htmlparser.h"
#include <string>

using std::string;

void html::_clear_comment(string &content) {
  int start = 0;
  while ((start = _raw_content.find("<!--", start)) != string::npos) {
    int end = _raw_content.find("-->", start);
    if (end == string::npos)
      throw std::runtime_error("Comment not closed");
    _raw_content.erase(start, end - start + 3);
  }
}

void html::_clear_script(string &content) {
  int start = 0;
  while ((start = _raw_content.find("<script", start)) != string::npos) {
    int end = _raw_content.find("</script>", start);
    if (end == string::npos)
      throw std::runtime_error("Script not closed");
    _raw_content.erase(start, end - start + 9);
  }
}

string html::_clear(string content) {
  _clear_comment(content);
  _clear_script(content);
  return content;
}

int html::_find_tag_end(const int &start) const {
  if (_clear_content[start] != '<') {
    throw std::runtime_error("Not a tag");
  }
  int end = start;
  while (_clear_content[end] != '>') {
    ++end;
  }

  return end;
}

int html::_find_next_tag(const int &start) const {}

bool html::_is_beginning_tag(const string &tag) const {}

html::_tag html::_parse_tag(const string &tag) const {}

tree<html::_tag> *html::_parse() const {
  if (_clear_content[0] != '<') {
    throw std::runtime_error("Invalid HTML file");
  }
  _tag tag = _parse_tag(_clear_content.substr(0, _find_tag_end(0) + 1));
  int close_tag_pos = _find_close_tag(0);
}

tree<html::_tag> *html::_parse(const string &content) const {
  if (content[0] != '<') {
    throw std::runtime_error("Not the beginning of a tag");
  }
}

bool html::valid() const {}
