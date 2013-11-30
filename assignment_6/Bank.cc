#include "Bank.h"

Bank::Bank(unsigned int numStudents) : mBalances(numStudents, 0) {}

void deposit(unsigned int id, unsigned int amount) {
  mBalances[id] += amount;
}

void withdraw(unsigned int id, unsigned int amount) {
  if (mBalances[id] < amount) {
    // wait for enough funds to withdraw
    _When(mBalances[id] >= amount) _Accept(deposit);
  }
  mBalances[id] -= amount;
}
