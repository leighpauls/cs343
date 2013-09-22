#include <iostream>
#include <cstdlib>          // access: rand, srand
#include <unistd.h>         // access: getpid
#include <limits.h>         // access: INT_MIN
using namespace std;

#define NO_ERROR INT_MIN

int times = 1000;

/**
 * @return negative on completion, 0 or positive on early exit
 */
int rtn1(int i) {
  int returnValue = -1;
  for (int j = 0; j < times && returnValue < 0; j += 1) {
    if (rand() % 100000000 == 42) {
      returnValue = j;
    }
  }
  return returnValue;
}

/**
 * @return positive on completion, 0 or negative on early exit
 */
int rtn2(int i) {
  int returnValue = 1;
  for (int j = 0; -j < times && returnValue > 0; j -= 1) {
    if (rand() % 100000000 == 42) {
      returnValue = j;
    }
  }
  return returnValue;
}

/**
 * @return NO_ERROR on completion, another int on early exit
 */
int g(int i) {
  bool broken = false;
  int returnValue;
  for (int j = 0; j < times && !broken; j += 1) {
    if (rand() % 2 == 0) {
      returnValue = rtn1(i);
      if (returnValue >= 0) {
        broken = true;
      }
    } else {
      returnValue = rtn2(i);
      if (returnValue <= 0) {
        broken = true;
      }
    }
  }
  if ((!broken) && (i % 2)){
    returnValue = rtn2(i);
    if (returnValue <= 0) {
      broken = true;
    }
  }
  if (!broken) {
    returnValue = rtn1(i);
    if (returnValue >= 0) {
      broken = true;
    }
  }
  return broken ? returnValue : NO_ERROR;
}

/**
 * @return NO_ERROR on completion, another int on early exit
 */
int f(int i) {
  bool broken = false;
  int returnValue;
  for (int j = 0; j < times && !broken; j += 1) {
    returnValue = g(i);
    if (returnValue != NO_ERROR) {
      broken = true;
    }
  }
  if ((!broken) && (i % 2)) {
    returnValue = g(i);
    if (returnValue != NO_ERROR) {
      broken = true;
    }
  }
  if (!broken) {
    returnValue = g(i);
    if (returnValue != NO_ERROR) {
      broken = true;
    }
  }
  return broken ? returnValue : NO_ERROR;
}

int main(int argc, char *argv[]) {
  int seed = getpid();
  if (argc >= 2) seed = atoi(argv[1]);
  srand(seed);
  if (argc == 3) times = atoi(argv[2]);
  int returnValue = f(3);
  if (returnValue == NO_ERROR) {
    cout << "seed:" << seed << " times:" << times << " complete" << endl;
  } else {
    cout << "seed:" << seed
         << " times:" << times
         << " rc:" << returnValue
         << endl;
  }
}
