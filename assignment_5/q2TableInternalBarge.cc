#include "q2Table.h"

#if defined (TABLETYPE_INTB)

void Table::wait() {
    mWaiting.wait();                   // wait until signalled
    while ( rand() % 5 == 0 ) {        // multiple bargers allowed
        _Accept( pickup, putdown ) {   // accept callers
        } _Else {                      // do not wait if no callers
        } // _Accept
    } // if
} // Table::wait


Table::Table(const unsigned int numOfPhil, Printer &prt) :
    mForkStates(numOfPhil, false), mWaitingPhils(numOfPhil), mPrinter(prt) {}
Table::~Table() {}

void Table::pickup(unsigned int id) {
  unsigned int left = id;
  unsigned int right = (id + 1) % mForkStates.size();
  while (mForkStates[left] || mForkStates[right]) {
    mWaitingPhils[id] = true;
    wait();
    mWaitingPhils[id] = false;
  }
  mForkStates[left] = true;
  mForkStates[right] = true;
}

void Table::putdown(unsigned int id) {
  unsigned int leftFork = id;
  unsigned int rightFork = (id + 1) % mForkStates.size();
  mForkStates[leftFork] = false;
  mForkStates[rightFork] = false;

  // cooperate with my neighbors
  unsigned int leftPhil = id == 0 ? mWaitingForFork.size() - 1 : id - 1;
  unsigned int rightPhil = rightFork;

  // ping each waiting phil until my left and right eat, if they're able to
  while ((philosopherCanPickUp(leftPhil) && mWaitingPhils[leftPhil])
         || (philosopherCanPickUp(rightPhil) && mWaitingPhils[rightPhil])) {
    mWaiting.signalBlock();
  }
  fucking broken
}

#endif
