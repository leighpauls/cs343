#pragma once

#include "q2Workshop.h"
#include "q2Printer.h"

_Task Reindeer {
public:
  enum {
    MAX_NUM_REINDEER = 5, // number of reindeer in system for delivering toys
  };
  Reindeer(
      unsigned int id,
      Workshop& wrk,
      Printer& prt,
      unsigned int numDeliveries);
private:
  void main();

  unsigned int mId;
  Workshop& mWorkshop;
  Printer& mPrinter;
  unsigned int mNumDeliveries;

};
