#include "BottlingPlant.h"
#include "MPRNG.h"
#include "VendingMachine.h"
#include "Truck.h"

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
      mMaxShippedPerFlavour(maxShippedPerFlavour),
      mMaxStockPerFlavour(maxStockPerFlavour),
      mTimeBetweenShipments(timeBetweenShipments),
      mStock(VendingMachine::NUM_FLAVOURS, 0),
      mTerminating(false) {}

/**
 * Gets the next shipment of soda. This will be blocked on the task monitor
 * until the shipment is ready.
 * @return true iff production has stopped
 */
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
    // create a new truck
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
        mStock[i] = mprng(mMaxShippedPerFlavour);
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
    // wait for the truck to terminate
  }
  mPrinter.print(Printer::BottlingPlant, Printer::Finished);
}
