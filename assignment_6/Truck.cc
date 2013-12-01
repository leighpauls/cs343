#include "Truck.h"
#include "VendingMachine.h"
#include "MPRNG.h"

Truck::Truck(
    Printer &prt,
    NameServer &nameServer,
    BottlingPlant &plant,
    unsigned int numVendingMachines,
    unsigned int maxStockPerFlavour)
    : mPrinter(prt),
      mNameServer(nameServer),
      mPlant(plant),
      mNumVendingMachines(numVendingMachines),
      mMaxStockPerFlavour(maxStockPerFlavour) {}

void Truck::main() {
  mPrinter.print(Printer::Truck, Printer::Starting);

  VendingMachine** machines = mNameServer.getMachineList();
  unsigned int* cargo = new unsigned int[VendingMachine::NUM_FLAVOURS];
  for (;;) {
    // wait at timmies
    for (int i = mprng(1, 10); i > 0; --i) { yield(); }
    // get the shipment
    if (mPlant.getShipment(cargo)) {
      // Plant told me to terminate
      break;
    }
    // count the shipment
    unsigned int shipmentTotal = 0;
    for (int i = 0; i < VendingMachine::NUM_FLAVOURS; ++i) {
      shipmentTotal += cargo[i];
    }
    mPrinter.print(Printer::Truck, (char)ShipmentPickedUp, shipmentTotal);

    // do the deliveries
    for (int machineIdx = 0;
         machineIdx < mNumVendingMachines && shipmentTotal > 0;
         ++machineIdx) {
      // get the inventory
      unsigned int* inventory = machines[machineIdx]->inventory();
      mPrinter.print(
          Printer::Truck,
          (char)BeginDelivery,
          machines[machineIdx]->getId(),
          shipmentTotal);

      // fill the machine
      unsigned int unfilled = 0;
      for (int flavour = 0; flavour < VendingMachine::NUM_FLAVOURS; ++flavour) {
        unsigned int amount = min(
            mMaxStockPerFlavour-inventory[flavour],
            cargo[flavour]);
        inventory[flavour] += amount;
        cargo[flavour] -= amount;
        shipmentTotal -= amount;
        unfilled += mMaxStockPerFlavour - inventory[flavour];
      }
      // release the machine
      machines[machineIdx]->restocked();

      // was successful?
      if (unfilled == 0) {
        mPrinter.print(
            Printer::Truck,
            (char)SuccessfulDelivery,
            machines[machineIdx]->getId(),
            shipmentTotal);
      } else {
        mPrinter.print(
            Printer::Truck,
            (char)UnsuccessfulDelivery,
            machines[machineIdx]->getId(),
            unfilled);
      }
    }
  }

  delete[] cargo;
  mPrinter.print(Printer::Truck, Printer::Finished);
}
