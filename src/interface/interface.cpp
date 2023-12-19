//
// Created by 丁麓然 on 10/25/23.
//

#include "../../inc/interface/interface.h"
#include "../../inc/xpath/xpath.h"
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

string CLTinterface::_get_input_instruction(const string &input) {
  // preprocess the input
  int ins_starts_pos = input.find_first_not_of(' ');
  int ins_end_pos = input.find_first_of(' ', ins_starts_pos);

  return input.substr(ins_starts_pos, ins_end_pos - ins_starts_pos);
}

string CLTinterface::_get_input_arg(const string &input) {
  int ins_starts_pos = input.find_first_not_of(' ');
  int ins_end_pos = input.find_first_of(' ', ins_starts_pos);

  if (input.find_first_not_of(' ', ins_end_pos) != string::npos) {
    return input.substr(input.find_first_not_of(' ', ins_end_pos));
  } else {
    return "";
  }
}

char CLTinterface::_get_user_input() {
  string input, instruction, arg1;
  // get the user input
  getline(cin, input);
  instruction = _get_input_instruction(input);
  arg1 = _get_input_arg(input);
  // check if the instruction is valid
  if (instruction.size() == 1) {
    switch (instruction[0]) {
    case 'l':
    case 't':
      if (arg1.empty()) {
        _xpath = xpath();
      } else {
        _xpath = xpath(arg1);
      }
      break;
    case 'o':
      if (!arg1.empty()) {
        _html_path = arg1;
        return 'o';
      }
      break;
    case 's':
      if (arg1.empty()) {
        _xpath = xpath();
      } else {
        _xpath = CssSelector(arg1);
      }
      break;
    }
  } else {
    if (instruction == "help" or instruction == "quit") {
      return instruction[0];
    } else if (instruction == "list" or instruction == "text") {
      if (arg1.empty()) {
        _xpath = xpath();
      } else {
        _xpath = xpath(arg1);
      }
      return instruction[0];
    } else if (instruction == "select") {
      if (arg1.empty()) {
        _xpath = xpath();
      } else {
        _xpath = CssSelector(arg1);
      }
    } else if (instruction == "open") {
      if (not(arg1.empty())) {
        _html_path = arg1;
        return 'o';
      }
    }
  }
  return instruction[0];
}

void CLTinterface::_load_html() {
  if (_html_path.substr(0, 7) == "http://" ||
      _html_path.substr(0, 8) == "https://") {
    cout << "Reading from URL: " << _html_path << endl;
    string command =
        "wget -O html_parser/cache/downloaded.html_parser " + _html_path;
    int result = system(command.c_str());
    if (result != 0) {
      throw std::runtime_error("Failed to download the HTML file.");
    } else {
      _html_path = "html_parser/cache/downloaded.html_parser";
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

void CLTinterface::_show_html(const bool text_only) {
  if (_xpath.empty()) {
    cout << _html.raw_content() << endl;
  } else {
    cout << _html.show(_xpath, text_only) << endl;
  }
}

void CLTinterface::run() {
  char user_input;
  while (true) {
    cout << "(html_parser-parser) ";
    user_input = _get_user_input();
    switch (user_input) {
    case 'h':
      std::cout << _help_info << std::endl;
      break;
    case 'q':
      return;
    // receive a string of xpath and list all the nodes that match the xpath
    case 'l':
      _show_html();
      break;
    // receive a string of xpath and list all the nodes that match the
    // xpath(text only)
    case 't':
      _show_html(true);
      break;
    // receive a string of CssSelector and list all the nodes that match the
    // selector
    case 's':
      _show_html();
      break;
    case 'o':
      _load_html();
      break;
    default:
      std::cout << "Invalid input. Please try again." << std::endl;
    }
  }
}

} // namespace interface
