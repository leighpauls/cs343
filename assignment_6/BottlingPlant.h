#pragma once

#include "Printer.h"
#include "NameServer.h"

_Task BottlingPlant {
public:
  BottlingPlant(
      Printer &prt,
      NameServer &nameServer,
      unsigned int numVendingMachines,
      unsigned int maxShippedPerFlavour,
      unsigned int maxStockPerFlavour,
      unsigned int timeBetweenShipments);
  bool getShipment(unsigned int cargo[]);

private:
  void main();
};
