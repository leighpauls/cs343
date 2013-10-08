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

/// Convenience function for printing an array of values
void printArray(ostream* output, TYPE* values, unsigned int len) {
  if (output == NULL) {
    return;
  }
  for (unsigned int i = 0; i < len; ++i) {
    (*output)<<values[i]<<" ";
  }
  (*output)<<endl;
}

void doSorting(
    ValueGenerator& valueGen,
    ostream* output,
    unsigned int processors) {
  // create extra processors for depth
  uProcessor p[processors - 1] __attribute__(( unused ));
  for (;;) {
    // find out if there are more values, and how long the next list is
    unsigned int numValues;
    try {
      numValues = valueGen.listLen();
    } catch (ValueGenerator::EndOfFileException) {
      break;
    }

    // read the actual values
    TYPE* values = valueGen.getValues();
    printArray(output, values, numValues);

    // sort the data
    unsigned int depth = log2(processors);
    {
      Mergesort<TYPE> sorter(values, 0, numValues, depth);
    }

    // output the results
    printArray(output, values, numValues);
    if (output != NULL) {
      (*output)<<endl;
    }

    // free memory
    delete [] values;
  }
}

void uMain::main() {
  if (argc == 2) {
    // 1st mode, printing to the terminal
    ValuesFromFile values(argv[1]);
    doSorting(values, &cout, 1);
  } else if (argc == 3) {
    if (argv[2][0] == '-') {
      // 2nd mode
      // treat 2nd arg as a negative number of cpu's
      int depth = -atoi(argv[2]);
      DefaultValues values(argv[1]);
      doSorting(values, NULL, depth);
    } else {
      // 1st mode, printing to a file
      // treat 2nd arg as an output filename
      ValuesFromFile values(argv[1]);
      ofstream output(argv[2]);
      doSorting(values, &output, 1);
    }
  } else {
    // this must be an error
    cout<<"Usage: "<<argv[0]<<" unsorted-file [ sorted-file | -depth (> 0) ]";
  }
}
