#include "q1Potato.h"
#include <cstdlib>
#include <stdexcept>
#include <iostream>

using namespace std;

Potato::Potato(unsigned int maxTicks) {
  reset(maxTicks);
}

/**
 * Set the potato and print the intro for the round
 * @param maxTicks set the timer to to a value between 1 and maxticks inclusive
 */
void Potato::reset(unsigned int maxTicks) {
  mTicksRemaining = (rand() % maxTicks) + 1;
  cout<<"  POTATO will go off after "<<mTicksRemaining<<" toss";
  if (mTicksRemaining != 1) {
    cout<<"es";
  }
  cout<<endl;
}

/**
 * decrement the time remaining
 * @return true iff the potato has gone off
 */
bool Potato::countdown() {
  return (--mTicksRemaining) == 0;
}
