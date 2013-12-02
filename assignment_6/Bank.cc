#include "Bank.h"

Bank::Bank(unsigned int numStudents) : mBalances(numStudents, 0) {}

/// Increase a student's balance
void Bank::deposit(unsigned int id, unsigned int amount) {
  mBalances[id] += amount;
}

// decrease a students balance, blocking until enough funds are available
void Bank::withdraw(unsigned int id, unsigned int amount) {
  if (mBalances[id] < amount) {
    // wait for enough funds to withdraw
    _When(mBalances[id] >= amount) _Accept(deposit);
  }
  mBalances[id] -= amount;
}
