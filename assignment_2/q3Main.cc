#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "q3Mergesort.h"

using namespace std;

unsigned int uDefaultStackSize() {
    return 512 * 1000;        // set task stack-size to 512K
}

void uMain::main() {
  // use "processors" kernel threads
  unsigned int depth = 2;
  uProcessor p[ depth - 1 ] __attribute__(( unused ));
  if (argc < 2) {
    throw runtime_error("no input file");
  }

  ifstream inputFile(argv[1]);

  if (!inputFile.is_open()) {
    throw runtime_error("couldn't open input file");
  }

  // while I've still got lists to sort
  for (;;) {
    // get the values of the list
    unsigned int numValues;
    inputFile>>numValues;
    if (inputFile.eof()) {
      return;
    }
    int* values = new int[numValues];
    cout<<"source:";
    for (unsigned int i = 0; i < numValues; ++i) {
      inputFile>>values[i];
      cout<<" "<<values[i];
    }
    cout<<endl;

    // Sort the list
    {
      Mergesort<int> sorter(values, 0, numValues, depth);
    }

    cout<<"result:";
    for (unsigned int i = 0; i < numValues; ++i) {
      cout<<" "<<values[i];
    }
    cout<<endl;
    delete [] values;
  }
}
