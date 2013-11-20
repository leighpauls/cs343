#include "q2table.h"
#include "q2Printer.h"

#if defined( TABLETYPE_AUTO )

Table::Table(const unsigned int numOfPhil, Printer &prt)
    : mForkStates(numOfPhil, false), mPrinter(prt) {}

void Table::pickup(unsigned int id) {
  unsigned int left = id;
  unsigned int right = (id + 1) % mForkStates.size();
  WAITUNTIL(
      !(mForkStates[left] || mForkStates[right]),
      mPrinter.print(id, Philosopher::Waiting, left, right), );
  mForkStates[left] = true;
  mForkStates[right] = true;
  RETURN();
}
void Table::putdown(unsigned int id) {
  unsigned int left = id;
  unsigned int right = (id + 1) % mForkStates.size();
  mForkStates[left] = false;
  mForkStates[right] = false;
  RETURN();
}

Table::~Table() {}

#endif
