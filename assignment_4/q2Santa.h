#pragma once

#include "q2Workshop.h"
#include "q2Printer.h"

_Task Santa {
public:
  Santa(Workshop& wrk, Printer& prt);
private:
  void main();

  Workshop& mWorkshop;
  Printer& mPrinter;
};
