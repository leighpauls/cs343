#pragma once

#include <vector>

using namespace std;

_Cormonitor Printer {
public:
  enum Kind {
    Parent,
    WATCardOffice,
    NameServer,
    Truck,
    BottlingPlant,
    Student,
    VendingMachine,
    Courier,
  };
  enum CommonStates {
    Starting = 'S',
    Finished = 'F',
  };
  Printer(
      unsigned int numStudents,
      unsigned int numVendingMachines,
      unsigned int numCouriers);
  void print(Kind kind, char state);
  void print(Kind kind, char state, int value1);
  void print(Kind kind, char state, int value1, int value2);
  void print(Kind kind, unsigned int lid, char state);
  void print(Kind kind, unsigned int lid, char state, int value1);
  void print(Kind kind, unsigned int lid, char state, int value1, int value2);
private:
  void main();
  // helper functions
  unsigned int kindToIdx(Kind kind);
  unsigned int kindToIdx(Kind kind, unsigned int id);
  void flushBuffer();
  void printNextStateFinished();
  void printNextState();

  // internal variables
  vector<string> mBuffer;
  unsigned int mNumStudents;
  unsigned int mNumMachines;
  unsigned int mNumCouriers;
  unsigned int mNumPending;

  // coroutine communication
  unsigned int mNextIdx;
  char mNextState;
  int mNextParamA;
  int mNextParamB;
};
