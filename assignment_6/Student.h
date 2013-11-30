#pragma once

#include "Printer.h"
#include "NameServer.h"
#include "WATCardOffice.h"

_Task Student {
public:
  Student(
      Printer &prt,
      NameServer &nameServer,
      WATCardOffice &cardOffice,
      unsigned int id,
      unsigned int maxPurchases);
  enum State {
    SelectMachine = 'V',
    BoughtSoda = 'B',
    WATCardLost = 'L',
  };
private:
  void main();

  Printer& mPrinter;
  NameServer& mNameServer;
  WATCardOffice mCardOffice;
  unsigned int mId;
  unsigned int mMaxPurchases;
};
