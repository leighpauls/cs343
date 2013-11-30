#pragma once

#include "Printer.h"
#include "VendingMachine.h"

_Task NameServer {
public:
  NameServer(
      Printer &prt,
      unsigned int numVendingMachines,
      unsigned int numStudents);
  void VMRegister(VendingMachine *vendingmachine);
  VendingMachine *getMachine(unsigned int studentId);
  VendingMachine **getMachineList();

private:
  void main();

  unsigned int mNumStudents;
  unsigned int mNumVendingMachines;
  unsigned int mNumMachinesRegistered;
  VendingMachine** mMachines;
  unsigned int* mStudentMachineMapping;

  enum States {
    RegisterMachine = 'R',
    NewMachine = 'N',
  };
};
