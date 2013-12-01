#pragma once

#include "Printer.h"
#include "NameServer.h"
#include "WATCard.h"

_Task VendingMachine {
public:
  // flavours of soda (YOU DEFINE)
  enum Flavours {
    BlackCherry = 0,
    CreamSoda,
    RootBeer,
    Lime,
    NUM_FLAVOURS,
  };
  // purchase status: successful buy, out of stock, insufficient funds
  enum Status { BUY, STOCK, FUNDS };

  VendingMachine(
      Printer &prt,
      NameServer &nameServer,
      unsigned int id,
      unsigned int sodaCost,
      unsigned int maxStockPerFlavour);

  Status buy(Flavours flavour, WATCard &card);
  unsigned int *inventory();
  void restocked();
  _Nomutex unsigned int cost();
  _Nomutex unsigned int getId();

  virtual ~VendingMachine();

private:
  void main();

  enum States {
    StartReloading = 'r',
    DoneReloading = 'R',
    BoughtSoda = 'B',
  };

  Printer& mPrinter;
  NameServer& mNameServer;
  unsigned int mId;
  unsigned int mSodaCost;
  unsigned int mMaxStockPerFlavour;
  unsigned int *mStockLevels;
};
