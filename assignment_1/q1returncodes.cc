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
  for (int j = 0; j < times; j += 1) {
    if (rand() % 100000000 == 42) {
      return j;
    }
  }
  return -1;
}

/**
 * @return positive on completion, 0 or negative on early exit
 */
int rtn2(int i) {
  for (int j = 0; -j < times; j -= 1) {
    if (rand() % 100000000 == 42) {
      return j;
    }
  }
  return 1;
}

/**
 * @return NO_ERROR on completion, another int on early exit
 */
int g(int i) {
  for (int j = 0; j < times; j += 1) {
    if (rand() % 2 == 0) {
      int v = rtn1(i);
      if (v >= 0) {
        return v;
      }
    } else {
      int v = rtn2(i);
      if (v <= 0) {
        return v;
      }
    }
  }
  if (i % 2){
    int v = rtn2(i);
    if (v <= 0) {
      return v;
    }
  }
  int v = rtn1(i);
  if (v >= 0) {
    return v;
  }
  return NO_ERROR;
}

/**
 * @return NO_ERROR on completion, another int on early exit
 */
int f(int i) {
  for (int j = 0; j < times; j += 1) {
    int v = g(i);
    if (v != NO_ERROR) {
      return v;
    }
  }
  if (i % 2) {
    int v = g(i);
    if (v != NO_ERROR) {
      return v;
    }
  }
  int v = g(i);
  if (v != NO_ERROR) {
    return v;
  }
  return NO_ERROR;
}

int main(int argc, char *argv[]) {
  int seed = getpid();
  if (argc >= 2) seed = atoi(argv[1]);
  srand(seed);
  if (argc == 3) times = atoi(argv[2]);
  int rc = f(3);
  if (rc == NO_ERROR) {
    cout << "seed:" << seed << " times:" << times << " complete" << endl;
  } else {
    cout << "seed:" << seed << " times:" << times << " rc:" << rc << endl;
  }
}
