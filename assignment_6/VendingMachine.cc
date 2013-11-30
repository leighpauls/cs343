#include "VendingMachine.h"

VendingMachine::VendingMachine(
    Printer &prt,
    NameServer &nameServer,
    unsigned int id,
    unsigned int sodaCost,
    unsigned int maxStockPerFlavour)
    : mPrinter(prt),
      mNameServer(nameServer),
      mId(id),
      mSodaCost(sodaCost),
      mMaxStockPerFlavour(maxStockPerFlavour) {
  mStockLevels = new unsigned int[NUM_FLAVOURS];
}

VendingMachine::~VendingMachine() {
  delete [] mStockLevels;
}

Status VendingMachine::buy(Flavours flavour, WATCard &card) {
  // make sure that it's not being re-stocked
  if (mBeingRestocked) {
    _Accept(restocked);
  }
  // I'm allowed to try buying now
  if (mStockLevels[flavour] == 0) {
    return STOCK;
  }
  if (card.balance() < mSodaCost) {
    return FUNDS;
  }
  // successful sale
  mStockLevels[flavour] -= 1;
  card.withdraw(mSodaCost);
  mPrinter.print(
      Printer::VendingMachine,
      mId,
      BoughtSoda,
      flavour,
      mStockLevels[flavour]);
  return BUY;
}

unsigned int* VendingMachine::inventory() {
  mPrinter.print(Printer::VendingMachine, mId, StartReloading);
  mBeingRestocked = true;
  return mStockLevels;
}
void VendingMachine::restocked() {
  mPrinter.print(Printer::VendingMachine, mId, DoneReloading);
  mBeingRestocked = false;
}

_Nomutex unsigned int VendingMachine::cost() {
  return mSodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
  return mId;
}

void main() {
  mPrinter.print(Printer::VendingMachine, mId, Printer::Starting, mSodaCost);

  mNameServer.VMRegister(this);
  for (;;) {
    _Accept(
  }

  _Accept(~VendingMachine);

  mPrinter.print(Printer::VendingMachine, mId, Printer::Finished);
}
