#include "q1Player.h"
#include "q1Umpire.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace std;

Player::Player(Umpire &umpire, unsigned int id, PlayerList &players)
    : mUmpire(&
umpire), mId(id), mPlayers(players) {}

unsigned int Player::getId() {
  return mId;
}

void Player::toss(Potato &potato) {
  mPotato = &potato;
  resume();
}

void Player::removeSelf() {
  for (unsigned int i = 0; i < mPlayers.size(); i++) {
    if (mPlayers[i]->getId() == mId) {
      mPlayers.erase(mPlayers.begin() + i);
    }
  }
}

void Player::main() {
  for (;;) {
    cout<<" -> "<<mId;
    if (mPotato->countdown()) {
      cout<<" is eliminated"<<endl;
      removeSelf();
      mUmpire->set();
    } else {
      // pick a player to throw it to
      int numPlayersLeft = mPlayers.size();
      Player* next;
      do {
        next = mPlayers[rand() % numPlayersLeft];
      } while (next->mId == mId);

      next->toss(*mPotato);
    }
  }
}


