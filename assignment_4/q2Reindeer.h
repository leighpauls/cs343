#pragma once

#include "q1Workshop.h"
#include "q1Printer.h"

_Task Reindeer {
  // private members go here
public:
  enum {
    MAX_NUM_REINDEER = 5, // number of reindeer in system for delivering toys
  };
  Reindeer(
      unsigned int id,
      Workshop &wrk,
      Printer &prt,
      unsigned int numDeliveries );
};
