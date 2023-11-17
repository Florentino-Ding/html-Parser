#include "../inc/interface/interface.h"
#include <iostream>

auto main(const int argc, char* argv[]) -> int {
  using namespace std;
  using interface::CLTinterface;

  if (argc < 2) {
    cout << "Please provide a local path or URL as an argument." << endl;
    return 0;
  }

  CLTinterface interface(argv[1]);
  interface.run();

  return 0;
}
