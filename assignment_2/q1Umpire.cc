#include "q1Umpire.h"
#include <cstdlib>
#include <iostream>

using namespace std;

Umpire::Umpire(Player::PlayerList &players) : mPlayers(players) {}

/// Start a new round
void Umpire::set() {
  // I take back the potato
  resume();
}

void Umpire::main() {
  // Introduce and set up the game
  cout<<mPlayers.size()<<" players in the match"<<endl;
  Potato potato;
  int setNum = 1;

  for (;;) {
    // pick a player give the potato to initially
    cout<<"Set "<<setNum<<":\tU";
    mPlayers[rand() % mPlayers.size()]->toss(potato);
    // I've been given back the potato, is the game over?
    if (mPlayers.size() == 1) {
      break;
    }
    // I've got the potato back, reset it
    potato.reset();
    setNum++;
  }

  cout<<mPlayers[0]->getId()<<" wins the Match!"<<endl;
}
