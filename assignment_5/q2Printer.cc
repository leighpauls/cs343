#include "q2Printer.h"
#include <iostream>

Printer::Printer(unsigned int NoOfPhil)

void Printer::print(unsigned int id, Philosopher::States state);
void Printer::print(
    unsigned int id,
    Philosopher::States state,
    unsigned int bite,
    unsigned int noodles);

void Printer::main() {
  // called from constructor, print the banner
  stringstream ss;
  for (unsigned int curId = 0; curId < mNumPhil; curId++) {
    ss<<"Phil"<<curId;
    mBuffer[curId] = ss.str();
    ss.str(""):
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
    if (curId != mNextState) {
      mBuffer[curId] = string("...");
    } else {
      mBuffer[curId] = string(Philosopher::Finished);
    }
  }
  flushBuffer();
}

void Printer::printState() {
  if (mBuffer.count(mNextId)) {
    // something is currently in this position
    flushBuffer();
  }
  if (mState == Philosopher::Waiting || mState == Philosopher::Eating) {
    stringstream ss;
    ss<<mState<<mNextLeftNum<<","<<mNextRightNum;
    mBuffer[mNextId] = ss.str();
    return;
  }

  mBuffer[mNextId] = string(mState);
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
