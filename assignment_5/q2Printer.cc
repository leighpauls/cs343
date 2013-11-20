#include "q2Printer.h"
#include <iostream>
#include <sstream>

Printer::Printer(unsigned int numPhil) : mNumPhil(numPhil) {
  // print the banner
  resume();
}

void Printer::print(unsigned int id, Philosopher::States state) {
  mNextId = id;
  mNextState = state;
  resume();
}

void Printer::print(
    unsigned int id,
    Philosopher::States state,
    unsigned int bite,
    unsigned int noodles) {
  mNextId = id;
  mNextState = state;
  mNextLeftNum = bite;
  mNextRightNum = noodles;
  resume();
}

void Printer::main() {
  // called from constructor, print the banner
  stringstream ss;
  for (unsigned int curId = 0; curId < mNumPhil; curId++) {
    ss<<"Phil"<<curId;
    mBuffer[curId] = ss.str();
    ss.str("");
  }
  flushBuffer();
  for (;;) {
    // get a new state
    suspend();
    if (mNextState == Philosopher::Finished) {
      printFinished();
    } else {
      printState();
    }
  }
}

void Printer::printFinished() {
  if (!mBuffer.empty()) {
    flushBuffer();
  }
  for (unsigned int curId = 0; curId < mNumPhil; curId++) {
    if (curId != mNextId) {
      mBuffer[curId] = string("...");
    } else {
      stringstream ss;
      ss<<(char)Philosopher::Finished;
      mBuffer[curId] = ss.str();
    }
  }
  flushBuffer();
}

void Printer::printState() {
  if (mBuffer.count(mNextId)) {
    // something is currently in this position
    flushBuffer();
  }
  if (mNextState == Philosopher::Waiting || mNextState == Philosopher::Eating) {
    stringstream ss;
    ss<<(char)mNextState<<mNextLeftNum<<","<<mNextRightNum;
    mBuffer[mNextId] = ss.str();
    return;
  }

  mBuffer[mNextId] = mNextState;
}

void Printer::flushBuffer() {
  stringstream ss;
  for (unsigned int curId = 0; curId < mNumPhil; curId++) {
    if (mBuffer.count(curId)) {
      ss<<mBuffer[curId];
    }
    ss<<'\t';
  }
  mBuffer.clear();
  string s = ss.str();
  cout<<s<<endl;
}
