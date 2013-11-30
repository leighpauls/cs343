#pragma once

#include "Printer.h"
#include "VendingMachine.h"

_Task NameServer {
public:
  NameServer(
      Printer &prt,
      unsigned int numVendingMachines,
      unsigned int numStudents);
  void VMregister(VendingMachine *vendingmachine);
  VendingMachine *getMachine(unsigned int id);
  VendingMachine **getMachineList();
private:
  void main();
};
