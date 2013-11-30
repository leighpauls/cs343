#include "BottlingPlant.h"
#include "MPRNG.h"

BottlingPlant::BottlingPlant(
    Printer &prt,
    NameServer &nameServer,
    unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour,
    unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments)
    : mPrinter(prt),
      mNameServer(nameServer),
      mNumVendingMachines(numVendingMachines),
      mMaxStockPerFlavour(maxStockPerFlavour),
      mTimeBetweenShipments(timeBetweenShipments),
      mStock(VendingMachine::NUM_FLAVOURS, 0) {}

bool BottlingPlant::getShipment(unsigned int cargo[]) {
  if (mTerminating) {
    // it's time to quit
    return true;
  }
  // copy the cargo over
  for (int i = 0; i < mStock.size(); i++) {
    cargo[i] = mStock[i];
    mStock[i] = 0;
  }
  return false;
}

void BottlingPlant::main() {
  mPrinter.print(Printer::BottlingPlant, Printer::Starting);
  {
    Truck truck(
        mPrinter,
        mNameServer,
        *this,
        mNumVendingMachines,
        mMaxStockPerFlavour);

    for (;;) {
      // make some soda
      for (int i = mTimeBetweenShipments; i > 0; --i) { yield(); }
      for (int i = 0; i < mStock.size(); ++i) {
        mStock[i] = mprng(maxShippedPerFlavour);
      }
      // wait for the truck to take some, or to be told to quit
      _Accept(~BottlingPlant) {
        break;
      } or _Accept(getShipment) {
        mPrinter.print(Printer::BottlingPlant, ShipmentPickedUp);
      };
    }
    // let the truck terminate
    mTerminating = true;
    _Accept(getShipment);
  }
  mPrinter.print(Printer::BottlingPlant, Printer::Finished);
}
