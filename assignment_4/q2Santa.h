#pragma once

#include "q2Workshop.h"
#include "q2Printer.h"
#include "q2Yielder.h"

_Task Santa : public RandomYielder {
public:
  Santa(Workshop& wrk, Printer& prt);
private:
  void main();

  Workshop& mWorkshop;
  Printer& mPrinter;
};
