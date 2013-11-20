#pragma once

#include "q2table.h"

_Cormonitor Printer;

_Task Philosopher {
public:
  enum States {
    Thinking = 'T',
    Hungry = 'H',
    Eating ='E',
    Waiting = 'W',
    Finished = 'F'
  };
  Philosopher(
      unsigned int id,
      unsigned int noodles,
      Table& table,
      Printer& prt);
private:
  void main();
  void doYield(unsigned int times);

  unsigned int mId;
  unsigned int mNoodlesRemaining;
  Table& mTable;
  Printer& mPrinter;
};

void seedRandom(unsigned int seed);
