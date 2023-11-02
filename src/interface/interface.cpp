//
// Created by 丁麓然 on 10/25/23.
//

#include "../../inc/interface/interface.h"
#include "../../inc/xpath_parser/xpath_parser.h"
#include <cstdio>
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
  string input, instruction, arg1;
  int ins_starts_pos, ins_end_pos, arg1_pos;
  getline(cin, input);
  // preprocess the input
  ins_starts_pos = input.find_first_not_of(' ');
  ins_end_pos = input.find_first_of(' ', ins_starts_pos);
  instruction = input.substr(ins_starts_pos, ins_end_pos - ins_starts_pos);
  if (input.find_first_not_of(' ', ins_end_pos) != string::npos) {
    arg1 = input.substr(input.find_first_not_of(' ', ins_end_pos));
  } else {
    arg1 = "";
  }
  // check if the instruction is valid
  if (instruction.size() == 1) {
    if (instruction == "l") {
      if (arg1.empty()) {
        _xpath = xpath();
      } else {
        _xpath = xpath(arg1);
      }
      return 'l';
    } else if (instruction == "o") {
      if (not(arg1.empty())) {
        _html_path = arg1;
        return 'o';
      }
    } else {
      return instruction[0];
    }
  } else {
    if (instruction == "help" or instruction == "quit") {
      return instruction[0];
    } else if (instruction == "list") {
      if (arg1.empty()) {
        _xpath = xpath();
      } else {
        _xpath = xpath(arg1);
      }
      return 'l';
    } else if (instruction == "open") {
      if (not(arg1.empty())) {
        _html_path = arg1;
        return 'o';
      }
    }
  }
  return -1;
}

void CLTinterface::_load_html() {
  if (_html_path.substr(0, 7) == "http://" ||
      _html_path.substr(0, 8) == "https://") {
    cout << "Reading from URL: " << _html_path << endl;
    string command = "wget -O html/cache/downloaded.html " + _html_path;
    int result = system(command.c_str());
    if (result != 0) {
      throw std::runtime_error("Failed to download the HTML file.");
    } else {
      _html_path = "html/cache/downloaded.html";
      cout << "Downloaded to local path: cache/" << _html_path << endl;
    }
  }

  string html_content;
  fstream html_file(_html_path);
  cout << "Reading from local path: " << _html_path << endl;
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
    cout << _html.show(_xpath) << endl;
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
    case 'o':
      _load_html();
      break;
    default:
    case -1:
      std::cout << "Invalid input. Please try again." << std::endl;
    }
  }
}

} // namespace interface
