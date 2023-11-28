#include "../inc/interface/interface.h"
#include <exception>
#include <iostream>

int main(const int argc, char *argv[]) {
  using namespace std;
  using interface::CLTinterface;

  if (argc < 2) {
    cout << "Please provide a local path or URL as an argument." << endl;
    exit(1);
  }

  try {
    CLTinterface interface(argv[1]);
    interface.run();
  } catch (exception &e) {
    cout << "An error occurred when program running: " << e.what() << endl;
    exit(1);
  }

  exit(0);
}
