#include "q2Table.h"

Table::Table(const unsigned int numOfPhil, Printer &prt) :
    mForkStates(numOfPhil, false),
    mWaitingForFork(numOfPhil, uCondition()),
    mPrinter(prt) {}

/// helper function to check both forks
bool Table::philosopherCanPickUp(unsigned int philId) {
  unsigned int left = id;
  unsigned int right = id + 1;
  if (right >= mForkStates.size()) {
    right = 0;
  }
  return !(mForkStates[left] || mForkStates[right]);
}

void Table::pickup(unsigned int id) {
  // block if I can't pick up immediately
  if (!philosopherCanPickUp(id)) {
    mPrinter.print(id, Philosopher::Waiting, left, right);
    mWaitingForFork[id].wait();
  }
  mForkStates[left] = true;
  mForkStates[right] = true;
}

void Table::putdown(unsigned int id) {
  // put down the forks
  unsigned int leftFork = id;
  unsigned int rightFork = id + 1;
  if (rightFork >= mForkStates.size()) {
    right = 0;
  }
  mForkStates[leftFork] = false;
  mForkStates[rightFork] = false;

  // cooperate with my neighbors
  unsigned int leftPhilosopher = id - 1;
  if (leftPhilosopher < 0) {
    leftPhilosopher = mWaitingForFork.size() - 1;
  }
  if (philosopherCanPickUp(leftPhilosopher)) {
    mWaitingForFork[leftPhilosopher].signal();
  }

  unsigned int rightPhilosopher = rightFork;
  if (philosopherCanPickUp(rightPhilosopher)) {
    mWaitingForFork[rightPhilosopher].signal();
  }
}
