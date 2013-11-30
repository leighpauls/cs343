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
      unsigned int maxPurchases );
private:
  void main();
};
