#pragma once

#include "q2Philosopher.h"

#include <map>
using namespace std;

_Cormonitor Printer {
public:
  Printer(unsigned int numPhil);
  void print(unsigned int id, Philosopher::States state);
  void print(
      unsigned int id,
      Philosopher::States state,
      unsigned int bite,
      unsigned int noodles);

private:
  void main();
  void flushBuffer();
  void printState();
  void printFinished();
  map<unsigned int, string> mBuffer;
  unsigned int mNumPhil;

  // communication variables
  unsigned int mNextId;
  Philosopher::States mNextState;
  unsigned int mNextLeftNum;
  unsigned int mNextRightNum;
};

