#include "../inc/interface/interface.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
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
