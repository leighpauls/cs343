#include <iostream>
#include <stdexcept>
#include <cstdlib>
using namespace std;

class H {                                                // uC++ exception type
 public:
  int &i;                                               // pointer to fixup variable at raise
  H( int &i ) : i( i ) {}
};

class FixupFunctor {
 public:
  virtual void do_fixup( int &i) = 0;
};

void f( int &i, FixupFunctor &parent_fixup );
void g( int &i, FixupFunctor &parent_fixup );

class FFixup : public FixupFunctor {
 protected:
  FixupFunctor &m_parent_fixup;
 public:
  FFixup(FixupFunctor &parent_fixup) : m_parent_fixup(parent_fixup) {}
  
  virtual void do_fixup( int &i ) {
    cout << "f handler, i:" << i << endl;
    i -= 7;                                         // fix variable at raise
    if ( rand() % 7 == 0 ) g( i, m_parent_fixup);   // mutual recursion
  }
};

void f( int &i, FixupFunctor &parent_fixup) {
  FFixup local_fixup(parent_fixup);
  cout << "f " << i << endl;

  // do things that can be fixed by me
  if ( rand() % 5 == 0 ) local_fixup.do_fixup(i);
  if ( rand() % 7 == 0 ) g( i, local_fixup );

  // do things that I can't fix on my own
  if ( 0 < i ) f( i, parent_fixup );
}

class GFixup : public FixupFunctor {
 protected:
  FixupFunctor &m_parent_fixup;
 public:
  GFixup(FixupFunctor &parent_fixup) : m_parent_fixup(parent_fixup) {}

  virtual void do_fixup( int &i ) {
    cout << "g handler, i:" << i << endl;
    i -= 5;
    if ( rand() % 5 == 0 ) f( i, m_parent_fixup );
  }
};

void g( int &i, FixupFunctor &parent_fixup ) {
  GFixup local_fixup(parent_fixup);
  cout << "g " << i << endl;

  // do things that can be fixed by me
  if ( rand() % 7 == 0 ) local_fixup.do_fixup( i );
  if ( rand() % 5 == 0 ) f( i, local_fixup );

  // do things that I can't fix on my own
  if ( 0 < i ) g( i, parent_fixup );
}


class MainFixup : public FixupFunctor {
 public:
  virtual void do_fixup( int &i ) {
    throw new runtime_error("Invalid request for main to do a fixup");
  }
};

int main(int argc, char **argv) {
  int times = 25, seed = getpid();
  if ( argc >= 2 ) times = atoi( argv[1] );             // control recursion depth
  if ( argc == 3 ) seed  = atoi( argv[2] );             // allow repeatable experiment
  srand( seed );    
  MainFixup fixup = MainFixup();
  f( times, (FixupFunctor&)fixup );
  return 0;
}
