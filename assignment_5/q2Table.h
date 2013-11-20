#pragma once

_Cormonitor Printer;

/// internal scheduling monitor solution
#if defined( TABLETYPE_INT )
#include <vector>
using namespace std;
_Monitor Table {
  vector<bool> mForkStates;
  vector<uCondition*> mWaitingForFork;


/// internal scheduling monitor solution with barging
#elif defined( TABLETYPE_INTB )
_Monitor Table {
  /// only one condition variable
  uCondition mWaiting;
  /// barging version of wait
  void wait();

  vector<bool> mForkStates;
  vector<bool> mWaitingPhils;

/// automatic-signal monitor solution
#elif defined( TABLETYPE_AUTO )
_Monitor Table {

/// internal/external scheduling task solution
#elif defined( TABLETYPE_TASK )
_Task Table {

#else
#error unsupported table
#endif

  // common declarations
  bool philosopherCanPickUp(unsigned int philId);
  Printer &mPrinter;

public:
  Table(const unsigned int numOfPhil, Printer &prt);
  void pickup(unsigned int id);
  void putdown(unsigned int id);

  virtual ~Table();
};

