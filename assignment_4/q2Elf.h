#pragma once

#include "q2Workshop.h"
#include "q2Printer.h"

_Task Elf {
  // private members go here
public:
  enum {
    CONSULTING_GROUP_SIZE = 3, // number of elves for a consultation with Santa
  };
  Elf(
      unsigned int id,
      Workshop& wrk,
      Printer& prt,
      unsigned int numConsultations);
};

