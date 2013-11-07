#include "q2Santa.h"
#include "q2Yielder.h"

Santa::Santa(Workshop& wrk, Printer& prt) : mWorkshop(wrk), mPrinter(prt) {}

void Santa::main() {
  randomYield(10);
  mPrinter.print(SANTA_ID, Printer::Starting);
  for (;;) {
    randomYield(3);
    mPrinter.print(SANTA_ID, Printer::Napping);
    Workshop::Status status = mWorkshop.sleep();
    mPrinter.print(SANTA_ID, Printer::Awake);
    if (status == Workshop::Done) {
      break;
    } else if (status == Workshop::Delivery) {
      mPrinter.print(SANTA_ID, Printer::DeliveringToys);
      randomYield(5);
      mWorkshop.doneDelivering(SANTA_ID);
      mPrinter.print(SANTA_ID, Printer::DoneDelivering);
    } else {
      mPrinter.print(SANTA_ID, Printer::Consulting);
      randomYield(3);
      mWorkshop.doneConsulting(SANTA_ID);
      mPrinter.print(SANTA_ID, Printer::DoneConsulting);
    }
  }
  mPrinter.print(SANTA_ID, Printer::Finished);
}
