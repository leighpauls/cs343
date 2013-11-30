#pragma once

#include "Printer.h"
#include "Bank.h"

_Task Parent {
public:
  Parent(
      Printer &prt,
      Bank &bank,
      unsigned int numStudents,
      unsigned int parentalDelay);
private:
  void main();

  Printer& mPrinter;
  Bank& mBank;
  unsigned int mNumStudents;
  unsigned int mParentalDelay;

  enum Status {
    Depositing = 'D',
  };
};
