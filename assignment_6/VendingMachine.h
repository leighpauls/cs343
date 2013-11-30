#pragma once

#include "Printer.h"
#include "NameServer.h"
#include "WATCard.h"
#include "Flavours.h"

_Task VendingMachine {
public:
  // flavours of soda (YOU DEFINE)
  enum Flavours { ... };
  // purchase status: successful buy, out of stock, insufficient funds
  enum Status { BUY, STOCK, FUNDS };

  VendingMachine(
      Printer &prt,
      NameServer &nameServer,
      unsigned int id,
      unsigned int sodaCost,
      unsigned int maxStockPerFlavour);

  Status buy( Flavours flavour, WATCard &card );
  unsigned int *inventory();
  void restocked();
  _Nomutex unsigned int cost();
  _Nomutex unsigned int getId();
private:
  void main();
};
