#include "q2Workshop.h"
#include "q2Elf.h"
#include "q2Santa.h"
#include "q2Reindeer.h"

void usageQuit(const char* cmd) {
  cout<<"Usage: "<<cmd
      <<" [ N (> 0) [ E (> 0) [ Seed (> 0) [ C (>= 0) [ D (>= 0) ] ] ] ] ];"
      <<endl;
  exit(0);
}

int doConvert(const char* str, int minValue) {
  char* end;
  int res = strtol(str, &end, 10);
  if ((cosnt char*)end == str || res < minValue) {
    usageQuit();
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
      numDeliveries = doConvert(argv[5], 0);
    case 5:
      numConsultations = doConvert(argv[4], 0);
    case 4:
      seed = doConvert(argv[3], 1);
    case 3:
      numElves = doConvert(argv[2], 1);
    case 2:
      reindeerBound = doConvert(argv[1], 1);
      break;
    default:
      usageMessage(argv[0]);
  }

  Printer printer;
  Workshop workshop(printer, reindeerBound, numElves, numDeliveries);
  {
    Santa santa(workshop, printer);
    vector<Elf> elves;
    unsigned int curId = 1;
    for (int i = 0; i < numElves; ++i) {
      elves.push_back(Elf(curId, workshop, printer, numConsultations));
      curId++;
    }
    vector<Reindeer> reindeer;
    for (int i = 0; i < 5; ++i) {
      reindeer.push_back(Reindeer(curId, workshop, printer, numDeliveries));
      curId++;
    }
  }
}
