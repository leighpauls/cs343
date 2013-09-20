#include <iostream>
#include <cstdlib>          // access: rand, srand
#include <unistd.h>         // access: getpid
#include <limits.h>         // access: INT_MIN
using namespace std;

int times = 1000;

/**
 * @return negative on completion, 0 or positive on early exit
 */
int rtn1( int i ) {
  int return_value = -1;
  for (int j = 0; j < times && return_value < 0; j += 1) {
    if ( rand() % 100000000 == 42 ) {
      return_value = j;
    }
  }
  return return_value;
}

/**
 * @return positive on completion, 0 or negative on early exit
 */
int rtn2( int i ) {
  int return_value = 1;
  for ( int j = 0; -j < times && return_value > 0; j -= 1 ) {
    if ( rand() % 100000000 == 42 ) {
      return_value = j;
    }
  }
  return return_value;
}

/**
 * @return INT_MIN on completion, another int on early exit
 */
int g( int i ) {
  bool broken = false;
  int return_value;
  for ( int j = 0; j < times && !broken; j += 1 ) {
    if ( rand() % 2 == 0 ) {
      return_value = rtn1( i );
      if (return_value >= 0) {
        broken = true;
      }
    } else {
      return_value = rtn2( i );
      if (return_value <= 0) {
        broken = true;
      }
    }
  }
  if ((!broken) && (i % 2)){
    return_value = rtn2( i );
    if (return_value <= 0) {
        broken = true;
    }
  }
  if (!broken) {
    return_value = rtn1( i );
    if (return_value >= 0) {
      broken = true;
    }
  }
  return broken ? return_value : INT_MIN;
}

/**
 * @return INT_MIN on completion, another int on early exit
 */
int f( int i ) {
  bool broken = false;
  int return_value;
  for ( int j = 0; j < times && !broken; j += 1 ) {
    return_value = g( i );
    if (return_value != INT_MIN) {
      broken = true;
    }
  }
  if ((!broken) && (i % 2)) {
    return_value = g( i );
    if (return_value != INT_MIN) {
      broken = true;
    }
  }
  if (!broken) {
    return_value = g( i );
    if (return_value != INT_MIN) {
      broken = true;
    }
  }
  return broken ? return_value : INT_MIN;
}
int main( int argc, char *argv[] ) {
  int seed = getpid();
  if ( argc >= 2 ) seed = atoi( argv[1] );
  srand( seed );
  if ( argc == 3 ) times = atoi( argv[2] );
  int return_value = f( 3 );
  if (return_value == INT_MIN) {
    cout << "seed:" << seed << " times:" << times << " complete" << endl;
  } else {
    cout << "seed:" << seed << " times:" << times << " rc:" << return_value << endl;
  }
}
