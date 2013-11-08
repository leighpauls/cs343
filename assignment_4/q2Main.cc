#include "q2Workshop.h"
#include "q2Elf.h"
#include "q2Santa.h"
#include "q2Reindeer.h"
#include "q2Yielder.h"

#include <iostream>
#include <vector>

using namespace std;

void usageQuit(const char* cmd) {
  cout<<"Usage: "<<cmd
      <<" [ N (> 0) [ E (> 0) [ Seed (> 0) [ C (>= 0) [ D (>= 0) ] ] ] ] ];"
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
  int reindeerBound = 3;
  int numElves = 3;
  int seed = getpid();
  int numConsultations = 3;
  int numDeliveries = 3;
  switch (argc) {
    case 6:
      numDeliveries = doConvert(argv[5], 0, argv[0]);
    case 5:
      numConsultations = doConvert(argv[4], 0, argv[0]);
    case 4:
      seed = doConvert(argv[3], 1, argv[0]);
    case 3:
      numElves = doConvert(argv[2], 1, argv[0]);
    case 2:
      reindeerBound = doConvert(argv[1], 1, argv[0]);
    case 1:
      break;
    default:
      usageQuit(argv[0]);
  }

  seedRandom(seed);

  Printer printer(numElves);
  Workshop workshop(printer, reindeerBound, numElves, numDeliveries);

  {
    Santa santa(workshop, printer);

    unsigned int curId = 1;
    vector<Elf*> elves;
    for (int i = 0; i < numElves; ++i) {
      elves.push_back(new Elf(curId, workshop, printer, numConsultations));
      curId++;
    }
    vector<Reindeer*> reindeer;
    for (int i = 0; i < 5; ++i) {
      reindeer.push_back(new Reindeer(curId, workshop, printer, numDeliveries));
      curId++;
    }
    // make it run to completion...
    for (unsigned int i = 0; i < elves.size(); ++i) {
      delete elves[i];
    }
    for (unsigned int i = 0; i < reindeer.size(); ++i) {
      delete reindeer[i];
    }
  }
  cout<<"Workshop closed"<<endl;
}
