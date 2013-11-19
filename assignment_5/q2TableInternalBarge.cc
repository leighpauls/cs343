#include "q2Table.h"

void Table::wait() {
    waiting.wait();                    // wait until signalled
    while ( rand() % 5 == 0 ) {        // multiple bargers allowed
        _Accept( pickup, putdown ) {   // accept callers
        } _Else {                      // do not wait if no callers
        } // _Accept
    } // if
} // Table::wait

