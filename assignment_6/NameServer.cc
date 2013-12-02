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

NameServer::~NameServer() {
  delete[] mMachines;
  delete[] mStudentMachineMapping;
}

/**
 * Register the existance of vendingMachine. Each vending machine instance
 * must call this function exatly once.
 * @param vendingMachine The machine to register
 */
void NameServer::VMRegister(VendingMachine *vendingMachine) {
  mPrinter.print(
      Printer::NameServer,
      (char)RegisterMachine,
      vendingMachine->getId());
  mMachines[mNumMachinesRegistered] = vendingMachine;
  mNumMachinesRegistered++;
}

/**
 * For a student to call when it requires a new vending machine. The prepared
 * machine mapping for that student is returned, and then updated.
 * @param studentId The student requesting the new machine
 */
VendingMachine* NameServer::getMachine(unsigned int studentId) {
  unsigned int machineId = mStudentMachineMapping[studentId];
  mPrinter.print(Printer::NameServer, (char)NewMachine, studentId, machineId);

  // prepare the mapping for next time
  mStudentMachineMapping[studentId] =
      (mStudentMachineMapping[studentId] + 1) % mNumVendingMachines;

  return mMachines[machineId];
}

/**
 * @eturn a list of all machines registered
 */
VendingMachine** NameServer::getMachineList() {
  return mMachines;
}

void NameServer::main() {
  mPrinter.print(Printer::NameServer, Printer::Starting);

  // first wait for all of the machines to register
  while (mNumMachinesRegistered < mNumVendingMachines) {
    _Accept(VMRegister);
  }

  for (;;) {
    // Then allow machines to be found
    _Accept(getMachine, getMachineList) {
    } or _Accept(~NameServer) {
      break;
    }
  }

  mPrinter.print(Printer::NameServer, Printer::Finished);
}
