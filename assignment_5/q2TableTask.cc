#include "q2Table.h"
#include "q2Printer.h"

#if defined( TABLETYPE_TASK )

Table::Table(const unsigned int numOfPhil, Printer &prt) :
    mForkStates(numOfPhil, false), mPrinter(prt) {
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
  mActionId = id;
  mWaitingForFork[id]->wait();
  mActionId = -1;
}
void Table::putdown(unsigned int id) {
  mActionId = id;
}

void Table::main() {
  for (;;) {
    // wait for something to happen
    _Accept(pickup) {
      if (mActionId >= 0 && philosopherCanPickUp(mActionId)) {
        giveForks(mActionId);
      }
    } or _Accept(putdown) {
      unsigned int leftFork = mActionId;
      unsigned int rightFork = (mActionId + 1) % mForkStates.size();
      mForkStates[leftFork] = false;
      mForkStates[rightFork] = false;

      // see if my neighbors want to pick up
      unsigned int leftPhil = (mActionId == 0)
          ? mWaitingForFork.size() - 1
          : mActionId - 1;
      unsigned int rightPhil = rightFork;
      if (philosopherCanPickUp(leftPhil)
          && !mWaitingForFork[leftPhil]->empty()) {
        giveForks(leftPhil);
      }
      if (philosopherCanPickUp(rightPhil)
          && !mWaitingForFork[rightPhil]->empty()) {
        giveForks(rightPhil);
      }
    } or _Accept(~Table) {
      return;
    };
  }
}

void Table::giveForks(unsigned int philId) {
  unsigned int left = philId;
  unsigned int right = (philId + 1) % mForkStates.size();
  // give them the forks
  mForkStates[left] = true;
  mForkStates[right] = true;
  // wait for the philosipher to receive the forks
  mWaitingForFork[philId]->signalBlock();
}

#endif
