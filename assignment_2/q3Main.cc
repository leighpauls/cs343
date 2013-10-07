#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include "q3Mergesort.h"
#include "q3ValueGenerator.h"

using namespace std;

unsigned int uDefaultStackSize() {
    return 512 * 1000;        // set task stack-size to 512K
}

void printArray(ostream* output, TYPE* values, unsigned int len) {
  if (output == NULL) {
    return;
  }
  for (unsigned int i = 0; i < len; ++i) {
    (*output)<<values[i]<<" ";
  }
  (*output)<<endl;
}

void doSorting(ValueGenerator& valueGen, ostream* output, unsigned int processors) {
  // create extra processors for depth
  uProcessor p[processors - 1] __attribute__(( unused ));
  for (;;) {
    unsigned int numValues;
    try {
      numValues = valueGen.listLen();
    } catch (ValueGenerator::EndOfFileException) {
      break;
    }

    TYPE* values = valueGen.getValues();
    printArray(output, values, numValues);
    // sort the data
    unsigned int depth = log2(processors);
    {
      Mergesort<int> sorter(values, 0, numValues, depth);
    }
    printArray(output, values, numValues);
    if (output != NULL) {
      (*output)<<endl;
    }

    delete [] values;
  }
}

void uMain::main() {
  try {
    if (argc == 2) {
      ValuesFromFile values(argv[1]);
      doSorting(values, &cout, 1);
    } else if (argc == 3) {
      if (argv[2][0] == '-') {
        // treat 2nd arg as a negative number of cpu's
        int depth = -atoi(argv[2]);
        DefaultValues values(argv[1]);
        doSorting(values, NULL, depth);
      } else {
        // treat 2nd arg as an output filename
        ValuesFromFile values(argv[1]);
        ofstream output(argv[2]);
        if (!output.is_open()) {
          throw runtime_error("unable to open output file");
        }
        doSorting(values, &output, 1);
      }
    } else {
      stringstream ss;
      ss<<"Usage: "<<argv[0]<<" unsorted-file [ sorted-file | -depth (> 0) ]";
      throw runtime_error(ss.str());
    }
  } catch (runtime_error e) {
    // TODO: remove
    cout<<"LEIGHPAULS ERROR:"<<e.what()<<endl;
  }
}
