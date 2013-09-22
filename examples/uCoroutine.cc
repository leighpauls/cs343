#include <uC++.h>
#include <iostream>
using namespace std;

_Event ev {};

_Coroutine X {
    X *partner;
    static int id;
    int myid;
    void main();
  public:
    X( X *partner ) : partner( partner ), myid( id ) { id += 1; }
    void next() { resume(); }
};

int X::id = 0;


void X::main() {
    cout << "i am " << myid << endl;
    try {
	_Enable {
	    if ( partner != NULL ) partner->next();
	    else suspend();
	    if ( myid == 1 ) {
		cout << "i am " << myid << " throwing exception " << endl;
		_Resume ev() _At *partner;
	    } // if
	} // _Enable
    } catch( ev ) {
	cout << "i am2 " << myid << endl;
	suspend();
    } // catch
} // X::main
	
void uMain::main() {
    X x1( NULL );
    X x2( &x1 );
    X x3( &x2 );
    x3.next();
    x1.next();
} // uMain::main
