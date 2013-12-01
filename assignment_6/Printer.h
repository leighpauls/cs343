#pragma once

_Cormonitor Printer {
public:
  enum Kind {
    Parent,
    WATCardOffice,
    NameServer,
    Truck,
    BottlingPlant,
    Student,
    Vending,
    Courier,
  };
  enum CommonStates {
    Starting = 'S',
    Finished = 'F',
  }
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
  unsigned int kindToIdx(Kind kind);
  unsigned int kindToIdx(Kind kind, unsigned int id);
  vector<string> buffer;
};
