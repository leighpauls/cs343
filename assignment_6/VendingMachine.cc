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
  for (unsigned int i = 0; i < NUM_FLAVOURS; ++i) {
    mStockLevels[i] = 0;
  }
}

VendingMachine::~VendingMachine() {
  delete [] mStockLevels;
}

VendingMachine::Status VendingMachine::buy(Flavours flavour, WATCard &card) {
  // I'm allowed to try buying now
  if (mStockLevels[flavour] == 0) {
    return STOCK;
  }
  if (card.getBalance() < mSodaCost) {
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
  mPrinter.print(Printer::VendingMachine, mId, (char)StartReloading);
  return mStockLevels;
}
void VendingMachine::restocked() {
  mPrinter.print(Printer::VendingMachine, mId, (char)DoneReloading);
}

_Nomutex unsigned int VendingMachine::cost() {
  return mSodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
  return mId;
}

void VendingMachine::main() {
  mPrinter.print(
      Printer::VendingMachine,
      mId,
      (char)Printer::Starting,
      mSodaCost);

  mNameServer.VMRegister(this);
  for (;;) {
    _Accept(inventory) {
      // wait for restocking to finish
      _Accept(restocked);
    } or _Accept(buy) {
      // do nothing
    } or _Accept(~VendingMachine) {
      break;
    }
  }

  mPrinter.print(Printer::VendingMachine, mId, (char)Printer::Finished);
}
