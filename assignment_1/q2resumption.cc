#include <iostream>
using namespace std;

_Event H {                                                // uC++ exception type
  public:
    int &i;                                               // pointer to fixup variable at raise
    H( int &i ) : i( i ) {}
};

void f( int &i );                                         // mutually recursive routines
void g( int &i );

void f( int &i ) {
    cout << "f " << i << endl;
    try {
        if ( rand() % 5 == 0 ) _Resume H( i );            // require correction ?
        if ( rand() % 7 == 0 ) g( i );                    // mutual recursion
    } _CatchResume( H &h ) {                              // fixup action
        cout << "f handler, i:" << h.i << endl;
        h.i -= 7;                                         // fix variable at raise
        if ( rand() % 7 == 0 ) g( h.i );                  // mutual recursion
    } // try
    if ( 0 < i ) f( i );                                  // recursion
}
void g( int &i ) {
    cout << "g " << i << endl;
    try {
        if ( rand() % 7 == 0 ) _Resume H( i );            // require correction ?
        if ( rand() % 5 == 0 ) f( i );                    // mutual recursion
    } _CatchResume( H &h ) {                              // fixup action
        cout << "g handler, i:" << h.i << endl;
        h.i -= 5;                                         // fix variable at raise
        if ( rand() % 5 == 0 ) f( h.i );                  // mutual recursion
    } // try
    if ( 0 < i ) g( i );                                  // recursion
}
void uMain::main() {
    int times = 25, seed = getpid();
    if ( argc >= 2 ) times = atoi( argv[1] );             // control recursion depth
    if ( argc == 3 ) seed  = atoi( argv[2] );             // allow repeatable experiment
    srand( seed );                                        // fixed or random seed
    f( times );
}
