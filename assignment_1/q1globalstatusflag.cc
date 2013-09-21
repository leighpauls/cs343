#include <iostream>
#include <cstdlib>          // access: rand, srand
#include <unistd.h>         // access: getpid
#include <limits.h>         // access: INT_MIN
using namespace std;

#define NO_ERROR INT_MIN

int times = 1000;
int error_code = NO_ERROR;

void rtn1( int i ) {
  for (int j = 0; j < times; j += 1) {
    if ( rand() % 100000000 == 42 ) {
      error_code = j;
      return;
    }
  }
}

void rtn2( int i ) {
  for ( int j = 0; -j < times; j -= 1 ) {
    if ( rand() % 100000000 == 42 ) {
      error_code = j;
      return;
    }
  }
}

void g( int i ) {
  for ( int j = 0; j < times; j += 1 ) {
    if ( rand() % 2 == 0 ) {
      rtn1( i );
      if (error_code != NO_ERROR) {
        return;
      }
    } else {
      rtn2( i );
      if (error_code != NO_ERROR) {
        return;
      }
    }
  }
  if (i % 2){
    rtn2( i );
    if (error_code != NO_ERROR) {
      return;
    }
  }
  rtn1( i );
  if (error_code != NO_ERROR) {
    return;
  }
}

void f( int i ) {
  for ( int j = 0; j < times; j += 1 ) {
    g( i );
    if (error_code != NO_ERROR) {
      return;
    }
  }
  if (i % 2) {
    g( i );
    if (error_code != NO_ERROR) {
      return;
    }
  }
  g( i );
  if (error_code != NO_ERROR) {
    return;
  }
}

int main( int argc, char *argv[] ) {
  int seed = getpid();
  if ( argc >= 2 ) seed = atoi( argv[1] );
  srand( seed );
  if ( argc == 3 ) times = atoi( argv[2] );
  f( 3 );
  if (error_code == NO_ERROR) {
    cout << "seed:" << seed << " times:" << times << " complete" << endl;
  } else {
    cout << "seed:" << seed << " times:" << times << " rc:" << error_code << endl;
  }
}
