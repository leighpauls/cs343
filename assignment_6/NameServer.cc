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
  mMachines[vendingMachine->getId()] = vendingMachine;
  mNumMachinesRegistered++;
}

void NameServer::waitForAllMachines() {
  // don't respond until registration is done
  if (mNumMachinesRegistered != mNumVendingMachines) {
    _When(mNumMachinesRegistered == mNumVendingMachines) _Accept(VMRegister);
  }
}

VendingMachine* NameServer::getMachine(unsigned int studentId) {
  waitForAllMachines();
  unsigned int machineId = mStudentMachineMapping[studentId];
  mPrinter.print(Printer::NameServer, NewMachine, studentId, machineId);

  // Tell the task which student to update
  mCalledStudent = studentId;

  return mMachines[machineId];
}
VendingMachine **getMachineList() {
  waitForAllMachines();
  return mMachines;
}

void main() {
  mPrinter.print(Printer::NameServer, Printer::Starting);

  for (;;) {
    _Accept(getMachine) {
      // get the selection ready for the next call
      mStudentMachineMapping[mCalledStudent] =
          (mStudentMachineMapping[mCalledStudent] + 1) % mNumVendingMachines;
    } or _Accept(~NameServer) {
      break;
    }
  }

  mPrinter.printer(Printer::NameServer, Printer::Finished);
}
