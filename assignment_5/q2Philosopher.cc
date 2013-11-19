#include "q2Philosopher.h"
#include "MPRNG.h"

Philosopher::Philosopher(
    unsigned int id,
    unsigned int noodles,
    Table& table,
    Printer& prt) :
    mId(id), mNoodlesRemaining(noodles), mTable(table), mPrinter(prt) {}

static MPRNG sRandom;
void seedRandom(unsigned int seed) {
  sRandom.seed(seed);
}

void Pilosopher::main() {
  for (;;) {
    mPrinter.print(mId, Hungry);
    doYield(sRandom(4));
    mTable.pickup(mId);

    unsigned int numEaten = min(sRandom(1, 5), mNoodlesRemaining);
    mNoodlesRemaining -= numEaten;
    mPrinter.print(mId, Eating, numEaten, mNoodelsRemaining);

    doYield(sRandom(4));
    mTable.putDown(mId);

    if (mNoodlesRemaining == 0) {
      break;
    }

    mPrinter.print(mId, Thinking);
    doYield(sRandom(19));
  }
  mPrinter.print(mId, Finished);
}

void Pilosopher::doYield(unsigned int times) {
  for (unsigned int i = 0; i < times; i++) {
    yield();
  }
}
