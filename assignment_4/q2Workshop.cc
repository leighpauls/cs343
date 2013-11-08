#include "q2Workshop.h"

Workshop::Workshop(Printer &prt, unsigned int N, unsigned int E, unsigned int D)
    : mPrinter(prt),
      mReindeerBound(N),
      mNumElvesWithConsultations(E),
      mNumDeliveries(D),
      mReindeerWaiting(0),
      mElvesWaiting(0),
      mTimesReindeerServed(0),
      mConsultersRemaining(4), // elves + santa
      mDeliverersRemaining(6), // reindeer + santa
      mConsultationsOver(false)
{}

/// santa calls to nap; when Santa wakes status of next action
Status Workshop::sleep() {
  while (mReindeerWaiting < 5 && mElvesWaiting < 3) {
    // there's nothing to do yet, sleep...
    mPrinter.print(SANTA_ID, Printer::Blocked);
    mWakeSanta.wait();
    mPrinter.print(SANTA_ID, Printer::Unblocked);
    if (mNumElvesWithConsultations < 3) {
      // fewer than 3 elves will ever want consultation
      mConsultationsOver = true;
      while (!mWakeElves.empty()) {
        mWakeElves.signalBlock();
      }
      return Done;
    }
  }

  // Santa must have something to do at this point
  if (mReindeerWaiting == 5 &&
      (mElvesWaiting < 3 || mTimesReindeerServed < mReindeerBound)) {
    // Serving the reindeer
    mTimesReindeerServed++;
    mReindeerWaiting = 0;
    for (int i = 0; i < 5; i++) {
      mWakeReindeer.signal();
    }
    return Delivery;
  }

  // Serving the elves
  mTimesReindeerServed = 0;
  mConsultationSuccessful = true;
  for (int i = 0; i < 3; ++i) {
    // use signalBlock, to make sure that the elf reads the current value
    // of mConsultationsOver
    mWakeElves.signalBlock();
  }
  mConsultationSuccessful = false;
  return Consulting;
}

/// reindeer call to deliver toys
void deliver(unsigned int id) {
  mReindeerWaiting++;
  mWakeSanta.signal();
  // Wait for santa to start me
  mPrinter.print(id, Printer::Blocked, mReindeerWaiting);
  mWakeReindeer.wait();
  mPrinter.print(id, Printer::Unblocked, mReindeerWaiting);
}

/**
 * elves call to consult Santa,
 * @returns true => consultation successful, false => consultation failed
 */
bool consult(unsigned int id) {
  if (mConsultationsOver) {
    return false;
  }
  mElvesWaiting++;
  mWakeSanta.signal();
  // wait for santa to start me
  mPrinter.print(id, Printer::Blocked, mElvesWaiting);
  mWakeElves.wait();
  mPrinter.print(id, Printer::Unblocked, mElvesWaiting);
  return !mConsultationsOver;
}

/// block Santa/elves until meeting over
void doneConsulting(unsigned int id) {
  mConsultersRemaining--;
  if (mConsultersRemaining == 0) {
    // I'm the last to complete the meeting, wake everyone else up
    while (!mConsultationComplete.empty()) {
      mConsultationComplete.signalBlock();
      mConsiltersRemaining++;
    }
  } else {
    // I need to wait for the rest of the consultation to be done
    mPrinter.print(id, Printer::Blocked, 4 - mConsultersRemaining);
    mConsultationComplete.wait();
    mPrinter.print(id, Printer::Unblocked, 4 - mConsultersRemaining);
  }
}

/// block Santa/reindeer until all toys are delivered
void doneDelivering(unsigned int id) {
  mDeliverersRemaining--;
  if (mDeliverersRemaining == 0) {
    // I'm the last to complete the delivery, wake everyone else up
    while (!mDeliveryComplete.empty()) {
      mDeliveryComplete.signalBlock();
      mDelivererRemaining++;
    }
  } else {
    // I need to wait for the rest of the delivery to complete
    mPrinter.print(id, Printer::Blocked, 6 - mDeliverersRemaining);
    mDeliveryComplete.wait();
    mPrinter.print(id, Printer::Unblocked, 6 - mDeliverersRemaining);
  }
}

/// elves call to indicate termination
void termination(unsigned int id) {
  mNumElvesWithConsultations--;
  mWakeSanta.signal();
}
