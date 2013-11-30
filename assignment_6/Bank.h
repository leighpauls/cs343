#pragma once

_Monitor Bank {
public:
  Bank(unsigned int numStudents);
  void deposit(unsigned int id, unsigned int amount);
  void withdraw(unsigned int id, unsigned int amount);
private:
  vector<unsigned int> mBalances;
};

