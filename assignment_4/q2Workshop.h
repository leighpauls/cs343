#pragma once

#include "q2Printer.h"

_Monitor Workshop {
  // private members go here
public:
  enum Status { Consulting, Delivery, Done };
  /// printer, bound, elves, reindeer delivery
  Workshop(Printer &prt, unsigned int N, unsigned int E, unsigned int D);

  /// santa calls to nap; when Santa wakes status of next action
  Status sleep();
  /// reindeer call to deliver toys
  void deliver(unsigned int id);
  /**
   * elves call to consult Santa,
   * @returns true => consultation successful, false => consultation failed
   */
  bool consult(unsigned int id);
  /// block Santa/elves until meeting over
  void doneConsulting(unsigned int id);
  /// block Santa/reindeer until all toys are delivered
  void doneDelivering(unsigned int id);
  /// elves call to indicate termination
  void termination(unsigned int id);
};

