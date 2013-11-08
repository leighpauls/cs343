#include "q2Yielder.h"
#include "MPRNG.h"
#include <uC++.h>

static MPRNG mprng;

void seedRandom(unsigned int seed) {
  mprng.seed(seed);
}

void RandomYielder::randomYield(unsigned int maxTimes) {
  for (int i = mprng(maxTimes); i > 0; --i) {
    yield();
  }
}
