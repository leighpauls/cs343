#include <iostream>
#include <cstdlib>          // access: rand, srand
#include <unistd.h>         // access: getpid
using namespace std;

int times = 1000;

void rtn1( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 100000000 == 42 ) throw j;
    }
}
void rtn2( int i ) {
    for ( int j = 0; -j < times; j -= 1 ) {
        if ( rand() % 100000000 == 42 ) throw j;
    }
}
void g( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 2 == 0 ) rtn1( i );
        else rtn2( i );
    }
    if ( i % 2 ) rtn2( i );
    rtn1( i );
}
void f( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        g( i );
    }
    if ( i % 2 ) g( i );
    g( i );
}
int main( int argc, char *argv[] ) {
    int seed = getpid();
    if ( argc >= 2 ) seed = atoi( argv[1] );
    srand( seed );
    if ( argc == 3 ) times = atoi( argv[2] );
    try {
        f( 3 );
        cout << "seed:" << seed << " times:" << times << " complete" << endl;
    } catch( int rc ) {
        cout << "seed:" << seed << " times:" << times << " rc:" << rc << endl;
    }
}
