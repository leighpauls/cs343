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
};
