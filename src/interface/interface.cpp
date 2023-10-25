//
// Created by 丁麓然 on 10/25/23.
//

#include "../../inc/interface/interface.h"
#include "../../inc/xpath_parser/xpath_parser.h"
#include <iostream>
#include <string>

namespace interface {

using namespace std;
using custom::xpath;

string CLTinterface::_help_info =
    "Usage: \n"
    "  help: show this help info\n"
    "  quit: quit the program\n"
    "  list <xpath>: list all the nodes that match the "
    "given xpath\n";

char CLTinterface::_get_user_input() {
  string input;
  cin >> input;
  if (input.size() == 1) {
    if (input == "I") {
      if (input.find_first_not_of(' ', 1) == string::npos) {
        _xpath = xpath();
      } else {
        _xpath = xpath(input.substr(1));
      }
      return 'l';
    }
    return input[0];
  } else {
    if (input == "help" or input == "quit") {
      return input[0];
    } else if (input.substr(0, 4) == "list") {
      if (input.find_first_not_of(' ', 4) == string::npos) {
        _xpath = xpath();
      } else {
        _xpath = xpath(input.substr(input.find_first_not_of(' ', 4)));
      }
      return 'l';
    }
  }
  return -1;
}

void CLTinterface::_load_html(const string &p) {
  string path = p;
  if (path.substr(0, 7) == "http://" || path.substr(0, 8) == "https://") {
    cout << "Reading from URL: " << path << endl;
    string command = "wget -O html/cache/downloaded.html " + path;
    int result = system(command.c_str());
    if (result != 0) {
      throw std::runtime_error("Failed to download the HTML file.");
    } else {
      path = "html/cache/downloaded.html";
      cout << "Downloaded to local path: cache/" << path << endl;
    }
  }

  string html_content;
  fstream html_file(path);
  cout << "Reading from local path: " << path << endl;
  if (html_file.is_open()) {
    string line;
    while (getline(html_file, line)) {
      html_content += line;
    }
    html_file.close();
  } else {
    throw std::runtime_error("Failed to open the HTML file.");
  }

  _html = html(html_content);
}

void CLTinterface::_show_html() {
  if (_xpath.empty()) {
    cout << _html.raw_content() << endl;
  } else {
  }
}

void CLTinterface::run() {
  char user_input;
  xpath path;
  while (true) {
    cout << "(html-parser) ";
    user_input = _get_user_input();
    switch (user_input) {
    case 'h':
      std::cout << _help_info << std::endl;
      break;
    case 'q':
      return;
    case 'l':
      _show_html();
      break;
    default:
    case -1:
      std::cout << "Invalid input. Please try again." << std::endl;
    }
  }
}

} // namespace interface
