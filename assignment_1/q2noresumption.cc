#include <iostream>
#include <stdexcept>
#include <cstdlib>
using namespace std;

class FixupFunctor {
 public:
  virtual void doFixup(int &i) = 0;
};

void f(int &i, FixupFunctor &parentFixup);
void g(int &i, FixupFunctor &parentFixup);

class FFixup : public FixupFunctor {
 protected:
  FixupFunctor &mParentFixup;
 public:
  FFixup(FixupFunctor &parentFixup) : mParentFixup(parentFixup) {}

  /**
   * This code is taken from the _CatchResume block originally in f
   */
  virtual void doFixup(int &i) {
    cout << "f handler, i:" << i << endl;
    i -= 7;
    if (rand() % 7 == 0) g(i, mParentFixup);
  }
};

void f(int &i, FixupFunctor &parentFixup) {
  FFixup localFixup(parentFixup);
  cout << "f " << i << endl;

  // Things that can be fixed by me use the local fixup
  if (rand() % 5 == 0) localFixup.doFixup(i);
  if (rand() % 7 == 0) g(i, localFixup);

  // Things that I can't fix use the calling function's fixup
  if (0 < i) f(i, parentFixup);
}

class GFixup : public FixupFunctor {
 protected:
  FixupFunctor &mParentFixup;
 public:
  GFixup(FixupFunctor &parentFixup) : mParentFixup(parentFixup) {}

  /**
   * This code is taken from the _CatchResume block originally in g
   */
  virtual void doFixup(int &i) {
    cout << "g handler, i:" << i << endl;
    i -= 5;
    if (rand() % 5 == 0) f(i, mParentFixup);
  }
};

void g(int &i, FixupFunctor &parentFixup) {
  GFixup localFixup(parentFixup);
  cout << "g " << i << endl;

  // Things that can be fixed by me use the local fixup
  if (rand() % 7 == 0) localFixup.doFixup(i);
  if (rand() % 5 == 0) f(i, localFixup);

  // Things that I can't fix use the calling function's fixup
  if (0 < i) g(i, parentFixup);
}


class MainFixup : public FixupFunctor {
 public:
  virtual void doFixup(int &i) {
    throw new runtime_error("Invalid request for main to do a fixup");
  }
};

int main(int argc, char **argv) {
  int times = 25, seed = getpid();
  // control recursion depth
  if (argc >= 2) times = atoi(argv[1]);
  // allow repeatable experiment
  if (argc == 3) seed  = atoi(argv[2]);
  srand(seed);
  MainFixup fixup = MainFixup();
  f(times, (FixupFunctor&)fixup);
  return 0;
}
