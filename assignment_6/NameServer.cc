#include "NameServer.h"
#include "VendingMachine.h"

NameServer::NameServer(
    Printer &prt,
    unsigned int numVendingMachines,
    unsigned int numStudents)
    : mPrinter(prt),
      mNumStudents(numStudents),
      mNumVendingMachines(numVendingMachines),
      mNumMachinesRegistered(0) {
  mMachines = new VendingMachine*[mNumVendingMachines];
  mStudentMachineMapping = new unsigned int[mNumStudents];
  for (int curStudent = 0; curStudent < mNumStudents; curStudent++) {
    mStudentMachineMapping[curStudent] = curStudent % mNumVendingMachines;
  }
}

void NameServer::VMRegister(VendingMachine *vendingMachine) {
  mPrinter.print(
      Printer::NameServer,
      (char)RegisterMachine,
      vendingMachine->getId());
  mMachines[mNumMachinesRegistered] = vendingMachine;
  mNumMachinesRegistered++;
}

VendingMachine* NameServer::getMachine(unsigned int studentId) {
  unsigned int machineId = mStudentMachineMapping[studentId];
  mPrinter.print(Printer::NameServer, (char)NewMachine, studentId, machineId);

  // prepare the mapping for next time
  mStudentMachineMapping[studentId] =
      (mStudentMachineMapping[studentId] + 1) % mNumVendingMachines;

  return mMachines[machineId];
}

VendingMachine** NameServer::getMachineList() {
  return mMachines;
}

void NameServer::main() {
  mPrinter.print(Printer::NameServer, Printer::Starting);

  // wait for all of the machines to register
  while (mNumMachinesRegistered < mNumVendingMachines) {
    _Accept(VMRegister);
  }

  for (;;) {
    _Accept(getMachine, getMachineList) {
    } or _Accept(~NameServer) {
      break;
    }
  }

  mPrinter.print(Printer::NameServer, Printer::Finished);
}
