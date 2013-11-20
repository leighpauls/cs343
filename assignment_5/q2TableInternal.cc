#include "q2Table.h"
#include "q2Printer.h"

#if defined( TABLETYPE_INT )

Table::Table(const unsigned int numOfPhil, Printer &prt) :
    mForkStates(numOfPhil, false),
    mPrinter(prt) {
  for (unsigned int i = 0; i < numOfPhil; ++i) {
    mWaitingForFork.push_back(new uCondition());
  }
}

Table::~Table() {
  for (unsigned int i = 0; i < mWaitingForFork.size(); ++i) {
    delete mWaitingForFork[i];
  }
}

void Table::pickup(unsigned int id) {
  // block if I can't pick up immediately
  unsigned int left = id;
  unsigned int right = (id + 1) % mForkStates.size();
  if (mForkStates[left] || mForkStates[right]) {
    mPrinter.print(id, Philosopher::Waiting, left, right);
    mWaitingForFork[id]->wait();
  }
  mForkStates[left] = true;
  mForkStates[right] = true;
}

void Table::putdown(unsigned int id) {
  // put down the forks
  unsigned int leftFork = id;
  unsigned int rightFork = (id + 1) % mForkStates.size();
  mForkStates[leftFork] = false;
  mForkStates[rightFork] = false;

  // cooperate with my neighbors
  unsigned int leftPhilosopher = id == 0 ? mWaitingForFork.size() - 1 : id - 1;
  if (philosopherCanPickUp(leftPhilosopher)) {
    mWaitingForFork[leftPhilosopher]->signal();
  }

  unsigned int rightPhilosopher = rightFork;
  if (philosopherCanPickUp(rightPhilosopher)) {
    mWaitingForFork[rightPhilosopher]->signal();
  }
}

#endif
