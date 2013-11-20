
#include "q2Philosopher.h"
#include "q2table.h"
#include "q2Printer.h"

#include <iostream>
#include <cstdlib>

using namespace std;

void usageQuit(const char* cmd) {
  cout<<"Usage: "<<cmd
      <<" [ philosophers (> 1) [ noodles (> 0) [ Seed (> 0) ] ] ]"
      <<endl;
  exit(0);
}

int doConvert(const char* str, int minValue, const char* cmd) {
  char* end;
  int res = strtol(str, &end, 10);
  if ((const char*)end == str || res < minValue) {
    usageQuit(cmd);
  }
  return res;
}


void uMain::main() {
  unsigned int numPhil = 5;
  unsigned int numNoodles = 30;
  unsigned int seed = getpid();

  switch (argc) {
    case 4:
      seed = doConvert(argv[3], 1, argv[0]);
    case 3:
      numNoodles = doConvert(argv[2], 1, argv[0]);
    case 2:
      numPhil = doConvert(argv[1], 2, argv[0]);
    case 1:
      break;
    default:
      usageQuit(argv[0]);
  }

  seedRandom(seed);

  Printer printer(numPhil);
  Table table(numPhil, printer);

  Philosopher* phils[numPhil];
  for (unsigned int id = 0; id < numPhil; ++id) {
    phils[id] = new Philosopher(id, numNoodles, table, printer);
  }

  // wait for all the philosophers to finish
  for (unsigned int id = 0; id < numPhil; ++id) {
    delete phils[id];
  }

  cout<<"***********************"<<endl;
  cout<<"Philosophers terminated"<<endl;
}
