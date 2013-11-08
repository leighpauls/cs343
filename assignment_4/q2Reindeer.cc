#include "q2Reindeer.h"
#include "q2Yielder.h"

Reindeer::Reindeer(
    unsigned int id,
    Workshop &wrk,
    Printer &prt,
    unsigned int numDeliveries)
    : mId(id), mWorkshop(wrk), mPrinter(prt), mNumDeliveries(numDeliveries) {}

void Reindeer::main() {
  randomYield(10);
  mPrinter.print(mId, Printer::Starting);
  for (unsigned int i = 0; i < mNumDeliveries; ++i) {
    randomYield(3);
    mPrinter.print(mId, Printer::OnVacation);
    randomYield(5);
    mPrinter.print(mId, Printer::CheckingIn);
    mWorkshop.deliver(mId);
    mPrinter.print(mId, Printer::DeliveringToys);
    randomYield(5);
    mWorkshop.doneDelivering(mId);
    mPrinter.print(mId, Printer::DoneDelivering);
  }
  mPrinter.print(mId, Printer::Finished);
}
