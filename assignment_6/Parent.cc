#include "Parent.h"
#include "MPRNG.h"

Parent::Parent(
    Printer &prt,
    Bank &bank,
    unsigned int numStudents,
    unsigned int parentalDelay)
    : mPrinter(prt),
      mBank(bank),
      mNumStudents(numStudents),
      mParentalDelay(parentalDelay) {}

void Parent::main() {
  mPrinter.print(Printer::Parent, Printer::Starting);

  for (;;) {
    for (int i = mParentalDelay; i > 0; --i) { yield(); }
    _Accept(~Parent) {
      break;
    } _Else {
      unsigned int student = mprng(0, mNumStudents-1);
      unsigned int amount = mprng(1, 3);
      mPrinter.print(Printer::Parent, (char)Depositing, student, amount);
      mBank.deposit(student, amount);
    }
  }

  mPrinter.print(Printer::Parent, Printer::Finished);
}
