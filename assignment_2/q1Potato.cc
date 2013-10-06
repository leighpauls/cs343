#include "q1Potato.h"
#include <cstdlib>
#include <stdexcept>
#include <iostream>

using namespace std;

Potato::Potato(unsigned int maxTicks) {
  reset(maxTicks);
}

void Potato::reset(unsigned int maxTicks) {
  mTicksRemaining = (rand() % maxTicks) + 1;
  cout<<"  POTATO will go off after "<<mTicksRemaining<<" toss";
  if (mTicksRemaining != 1) {
    cout<<"es";
  }
  cout<<endl;
}

bool Potato::countdown() {
  if (mTicksRemaining == 0) {
    throw runtime_error("Tried to countdown on a finished potato");
  }

  return (--mTicksRemaining) == 0;
}
