#pragma once

#define SANTA_ID 0

#include <map>

using namespace std;

_Monitor Printer {
public:
  enum States {
    Starting = 'S', Blocked = 'B', Unblocked = 'U', Finished = 'F', // general
    Napping = 'N', Awake = 'A',             // Santa
    Working = 'W', NeedHelp = 'H',          // elf
    OnVacation = 'V', CheckingIn = 'I',     // reindeer
    DeliveringToys = 'D', DoneDelivering = 'd', // Santa, reindeer
    Consulting = 'C', DoneConsulting = 'c', // Santa, elves
    ConsultingFailed = 'X',                 // elf
  };
  Printer(const unsigned int MAX_NUM_ELVES);
  void print(unsigned int id, States state);
  void print(unsigned int id, States state, unsigned int numBlocked);

private:
  void addToBuffer(unsigned int id, string& val);
  void flushBuffer();
  void writeHeader();

  const unsigned int NUM_ELVES;
  map<unsigned int, string> mPendingOutput;
};
