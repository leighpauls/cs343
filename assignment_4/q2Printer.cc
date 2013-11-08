#include "q2Printer.h"
#include <iostream>
#include <sstream>

using namespace std;

Printer::Printer(const unsigned int MAX_NUM_ELVES)
    : NUM_ELVES(MAX_NUM_ELVES),
      bufferFlushable(false) {
  writeHeader();
}

void Printer::print(unsigned int id, States state) {
  stringstream ss;
  ss<<(char)state;
  if (state == Finished) {
    flushBuffer();
    for (int i = 0; i < mPendingOutput.size(); ++i) {
      mPendingOutput[i] = "...";
    }
    mPendingOutput[id] = ss.str();
    flushBuffer();
    return;
  }
  addToBuffer(id, ss.str());
}

void Printer::print(unsigned int id, States state, unsigned int numBlocked) {
  stringstream ss;
  ss<<(char)state<<" "<<numBlocked;
  addToBuffer(id, ss.str());
}

void Printer::addToBuffer(unsigned int id, string& val) {
  if (mPendingOutput.count(id)) {
    flushBuffer();
  }
  mPendingOutput[id] = val;
}

void Printer::flushBuffer() {
  for (int id = 0; id < NUM_ELVES + 6l ++id) {
    if (mPendingOutput.count(id)) {
      cout<<mPendingOutput;
    }
    cout<<'\t';
  }
  cout<<endl;
  mPendingOutput.clear();
}

void Printer::writeHeader() {
  cout<<"Sa"<<'\t';
  unsigned int curId = 1;
  for (unsigned int i = 0; i < NUM_ELVES; ++i) {
    cout<<'E'<<curId<<'\t';
    ++curId;
  }
  for (unsigned int i = 0; i < 5; ++i) {
    cout<<'D'<<curId<<'\t';
  }
  cout<<endl;
  for (unsigned int i = 0; i < NUM_ELVES + 6; ++i) {
    cout<<"--"<<'\t';
  }
  cout<<endl;
}
