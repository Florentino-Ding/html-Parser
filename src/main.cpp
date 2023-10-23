#include "../inc/htmlparser.h"
#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char *argv[]) {
  using namespace std;

  string path;
  if (argc < 2) {
    cout << "Please provide a local path or URL as an argument." << endl;
    return 0;
  }

  path = argv[1];
  if (path.substr(0, 7) == "http://" || path.substr(0, 8) == "https://") {
    cout << "Reading from URL: " << path << endl;
    string command = "wget -O cache/downloaded.html " + path;
    int result = system(command.c_str());
    if (result != 0) {
      throw std::runtime_error("Failed to download the HTML file.");
    } else {
      path = "cache/downloaded.html";
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

  cout << html_content << endl;
  html page(html_content);

  return 0;
}
