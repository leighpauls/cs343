#include "q1Catching.h"

#include <stdexcept>
#include <iostream>

using namespace std;

void Catching::main() {
  try {
    doMain();
  } catch (runtime_error e) {
    cout<<endl<<"Runtime Error: "<<e.what()<<endl;
    throw;
  }
}
