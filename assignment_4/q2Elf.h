#pragma once

#include "q2Workshop.h"
#include "q2Printer.h"
#include "q2Yielder.h"

_Task Elf : public RandomYielder {
public:
  enum {
    CONSULTING_GROUP_SIZE = 3, // number of elves for a consultation with Santa
  };
  Elf(
      unsigned int id,
      Workshop& wrk,
      Printer& prt,
      unsigned int numConsultations);
private:
  void main();

  unsigned int mId;
  Workshop& mWorkshop;
  Printer& mPrinter;
  unsigned int mNumConsultations;
};

