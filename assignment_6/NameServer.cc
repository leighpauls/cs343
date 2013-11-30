#include "NameServer.h"

NameServer::NameServer(
    Printer &prt,
    unsigned int numVendingMachines,
    unsigned int numStudents)
    : mNumStudents(numStudents),
      mNumVendingMachines(numVendingMachine),
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
      RegisterMachine,
      vendingMachine->getId());
  mMachines[mNumMachinesRegistered] = vendingMachine;
  mNumMachinesRegistered++;
}

VendingMachine* NameServer::getMachine(unsigned int studentId) {
  unsigned int machineId = mStudentMachineMapping[studentId];
  mPrinter.print(Printer::NameServer, NewMachine, studentId, machineId);

  // prepare the mapping for next time
  mStudentMachineMapping[studentId] =
      (mStudentMachineMapping[studentId] + 1) % mNumVendingMachines;

  return mMachines[machineId];
}

VendingMachine **getMachineList() {
  return mMachines;
}

void main() {
  mPrinter.print(Printer::NameServer, Printer::Starting);

  // wait for all of the machines to register
  while (mNumMachinesRegistered < mNumVendingMachines) {
    _Accept(VMRegister);
  }

  for (;;) {
    _Accept(getMachine, getMachineList) {
      // get the selection ready for the next call
    } or _Accept(getMachineList) {
    } or _Accept(~NameServer) {
      break;
    }
  }

  mPrinter.printer(Printer::NameServer, Printer::Finished);
}
