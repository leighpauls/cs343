#include "q2Yielder.h"

static MPRNG mprng;

void randomYield(int maxTimes) {
  for (int i = mprng(maxTimes); i > 0; --i) {
    yield();
  }
}

void seedRandom(unsigned int seed) {
  mprng.seed(seed);
}
