#include "q2Elf.h"
#include "MPRNG.h"

Elf(
    unsigned int id,
    Workshop& wrk,
    Printer& prt,
    unsigned int numConsultations)
    : mId(id),
      mWorkshop(wrk),
      mPrinter(prt),
      mNumConsultations(numConsultations) {}

void Elf::main() {
  randomYield(10);
  mPrinter.print(mId, Printer::Starting);
  for (int i = 0; i < mNumconsultations; ++i) {
    randomYield(3);
    mPrinter.print(mId, Printer::Working);
    randomYield(5);
    mPrinter.print(mId, Printer::NeedHelp);
    if (!mWorkshop.consult(mId)) {
      mPrinter.print(mId, Printer::ConsultingFailed);
      break;
    }
    mPrinter.print(mId, Printer::Consulting);
    randomYield(3);
    mWorkshop.doneConsulting(mId);
    mPrinter.print(mId, Printer::DoneConsulting);
  }
  mWorkshop.termination(mId);
  mPrinter.print(mId, Printer::Finished);
}
