#pragma once
#include "q1Potato.h"
#include "q1Catching.h"

#include <vector>

using namespace std;

_Coroutine Umpire;

_Coroutine Player : public Catching {
public:
  typedef vector<Player*> PlayerList;
  Player(Umpire& umpire, unsigned int id, PlayerList& players);
  unsigned int getId();
  void toss(Potato &potato);

private:
  // private fields
  Umpire *mUmpire;
  unsigned int mId;
  PlayerList &mPlayers;

  // Co-routine communication
  Potato *mPotato;

  void removeSelf();

protected:
  void doMain();
};

