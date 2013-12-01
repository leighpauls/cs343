#pragma once

#include "Printer.h"
#include "NameServer.h"
#include "BottlingPlant.h"

_Task Truck {
public:
  Truck(
      Printer &prt,
      NameServer &nameServer,
      BottlingPlant &plant,
      unsigned int numVendingMachines,
      unsigned int maxStockPerFlavour);
private:
  void main();

  enum Status {
    ShipmentPickedUp = 'P',
    BeginDelivery = 'd',
    UnsuccessfulDelivery = 'U',
    SuccessfulDelivery = 'D',
  };

  Printer& mPrinter;
  NameServer& mNameServer;
  BottlingPlant& mPlant;
  unsigned int mNumVendingMachines;
  unsigned int mMaxStockPerFlavour;
};
