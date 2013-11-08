#include "q2Printer.h"
#include <iostream>
#include <sstream>

using namespace std;

Printer::Printer(const unsigned int MAX_NUM_ELVES)
    : NUM_ELVES(MAX_NUM_ELVES) {
  writeHeader();
}

void Printer::print(unsigned int id, States state) {
  stringstream ss;
  ss<<(char)state;
  if (state == Finished) {
    flushBuffer();
    for (unsigned int i = 0; i < NUM_ELVES + 6; ++i) {
      mPendingOutput[i] = "...";
    }
    mPendingOutput[id] = ss.str();
    flushBuffer();
    return;
  }
  string s(ss.str());
  addToBuffer(id, s);
}

void Printer::print(unsigned int id, States state, unsigned int numBlocked) {
  stringstream ss;
  ss<<(char)state<<" "<<numBlocked;
  string s(ss.str());
  addToBuffer(id, s);
}

void Printer::addToBuffer(unsigned int id, string& val) {
  if (mPendingOutput.count(id)) {
    flushBuffer();
  }
  mPendingOutput[id] = val;
}

void Printer::flushBuffer() {
  // find the highest id
  unsigned int highest = 0;
  for (unsigned int id = 0; id < NUM_ELVES + 6; ++id) {
    if (mPendingOutput.count(id)) {
      highest = id;
    }
  }
  for (unsigned int id = 0; id <= highest; ++id) {
    if (mPendingOutput.count(id)) {
      cout<<mPendingOutput[id];
    }
    if (id < highest) {
      cout<<'\t';
    }
  }
  cout<<endl;
  mPendingOutput.clear();
}

void Printer::writeHeader() {
  cout<<"Sa";
  unsigned int curId = 1;
  for (unsigned int i = 0; i < NUM_ELVES; ++i) {
    cout<<"\tE"<<curId;
    ++curId;
  }
  for (unsigned int i = 0; i < 5; ++i) {
    cout<<"\tD"<<curId;
    curId++;
  }
  cout<<endl;
  cout<<"--";
  for (unsigned int i = 1; i < NUM_ELVES + 6; ++i) {
    cout<<"\t--";
  }
  cout<<endl;
}
