#pragma once

#include "Printer.h"
#include "NameServer.h"
#include <vector>

using namespace std;

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

  Printer& mPrinter;
  NameServer& mNameServer;
  unsigned int mNumVendingMachines;
  unsigned int mMaxShippedPerFlavour;
  unsigned int mMaxStockPerFlavour;
  unsigned int mTimeBetweenShipments;
  vector<unsigned int> mStock;
  bool mTerminating;

  enum States {
    ShipmentPickedUp = 'P'
  };
};
