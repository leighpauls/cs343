#include "Printer.h"

#include <iostream>
#include <sstream>

using namespace std;

Printer::Printer(
    unsigned int numStudents,
    unsigned int numVendingMachines,
    unsigned int numCouriers)
    : mBuffer(5 + numStudents + numVendingMachines + numCouriers, string("")),
      mNumStudents(numStudents),
      mNumMachines(numVendingMachines),
      mNumCouriers(numCouriers),
      mNumPending(0) {
  // print the banner
  resume();
}

void Printer::main() {
  // print the banner
  stringstream ss;
  mBuffer[kindToIdx(Parent)] = string("Parent");
  mBuffer[kindToIdx(WATCardOffice)] = string("WATOff");
  mBuffer[kindToIdx(NameServer)] = string("Names");
  mBuffer[kindToIdx(Truck)] = string("Truck");
  mBuffer[kindToIdx(BottlingPlant)] = string("Plant");
  for (unsigned int i = 0; i < mNumStudents; ++i) {
    ss<<"Stud"<<i;
    mBuffer[kindToIdx(Student, i)] = ss.str();
    ss.str("");
  }
  for (unsigned int i = 0; i < mNumMachines; ++i) {
    ss<<"Mach"<<i;
    mBuffer[kindToIdx(VendingMachine, i)] = ss.str();
    ss.str("");
  }
  for (unsigned int i = 0; i < mNumCouriers; ++i) {
    ss<<"Cour"<<i;
    mBuffer[kindToIdx(Courier, i)] = ss.str();
    ss.str("");
  }
  flushBuffer();
  // print the asterisks
  for (unsigned int i = 0; i < mBuffer.size(); ++i) {
    mBuffer[i] = string("*******");
  }
  flushBuffer();
  for (;;) {
    // get a new state
    suspend();
    if (mNextState == Finished) {
      printNextStateFinished();
    } else {
      printNextState();
    }
  }
}

void Printer::printNextStateFinished() {
  // print any pending states
  if (mNumPending > 0) {
    flushBuffer();
  }
  for (unsigned int i = 0; i < mBuffer.size(); ++i) {
    if (i == mNextIdx) {
      stringstream ss;
      ss<<(char)Finished;
      mBuffer[i] = ss.str();
    } else {
      mBuffer[i] = string("...");
    }
  }
  flushBuffer();
}

void Printer::printNextState() {
  if (mBuffer[mNextIdx].length() != 0) {
    // don't overwrite values
    flushBuffer();
  }
  stringstream ss;
  ss<<mNextState;
  if (mNextParamA >= 0) {
    ss<<mNextParamA;
    if (mNextParamB >= 0) {
      ss<<','<<mNextParamB;
    }
  }
  mBuffer[mNextIdx] = ss.str();
  mNumPending++;
}

void Printer::flushBuffer() {
  stringstream ss;
  for (unsigned int i = 0; i < mBuffer.size()-1; ++i) {
    ss<<mBuffer[i]<<'\t';
    mBuffer[i] = string("");
  }
  ss<<mBuffer[mBuffer.size()-1]<<endl;
  cout<<ss.str();
  mNumPending = 0;
}

void Printer::print(Kind kind, char state) {
  print(kind, 0, state, -1, -1);
}
void Printer::print(Kind kind, char state, int value1) {
  print(kind, 0, state, value1, -1);
}
void Printer::print(Kind kind, char state, int value1, int value2) {
  print(kind, 0, state, value1, value2);
}
void Printer::print(Kind kind, unsigned int lid, char state) {
  print(kind, lid, state, -1, -1);
}
void Printer::print(Kind kind, unsigned int lid, char state, int value1) {
  print(kind, lid, state, value1, -1);
}

void Printer::print(
    Kind kind,
    unsigned int lid,
    char state,
    int value1,
    int value2) {
  mNextIdx = kindToIdx(kind, lid);
  mNextState = state;
  mNextParamA = value1;
  mNextParamB = value2;
  resume();
}

unsigned int Printer::kindToIdx(Kind kind) {
  switch (kind) {
    case Parent: return 0;
    case WATCardOffice: return 1;
    case NameServer: return 2;
    case Truck: return 3;
    case BottlingPlant: return 4;
    default:
      break;
  }
  return (unsigned int)-1; // Fail
}

unsigned int Printer::kindToIdx(Kind kind, unsigned int id) {
  unsigned int res = id;
  switch (kind) {
    case Courier:
      res += mNumMachines;
    case VendingMachine:
      res += mNumStudents;
    case Student:
      res += 5;
      return res;
    default:
      break;
  }
  return (unsigned int)-1; // fail
}
