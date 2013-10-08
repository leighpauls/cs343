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

/// Receive the potato, and resume this player's coroutine
void Player::toss(Potato &potato) {
  mPotato = &potato;
  resume();
}

/// Removes this player from the game
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
      // I've been eliminated from the game
      cout<<" is eliminated"<<endl;
      removeSelf();
      mUmpire->set();
    } else {
      // I'm still in the game, pick a player to throw it to
      int numPlayersLeft = mPlayers.size();
      Player* next;
      do {
        next = mPlayers[rand() % numPlayersLeft];
      } while (next->mId == mId);

      next->toss(*mPotato);
    }
  }
}


