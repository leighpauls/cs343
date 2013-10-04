#include "q1Umpire.h"
#include <cstdlib>
#include <iostream>

using namespace std;

Umpire::Umpire(Player::PlayerList &players) : mPlayers(players) {}

void Umpire::set() {
  // I take back the potato
  resume();
}

void Umpire::main() {
  cout<<mPlayers.size()<<" players in the match"<<endl;
  Potato potato;
  int setNum = 1;
  while (mPlayers.size() > 1) {
    // pick a player give the potato to initially
    cout<<"Set "<<setNum<<":\tU";
    mPlayers[rand() % mPlayers.size()]->toss(potato);
    // I've got the potato back, reset it
    potato.reset();
  }
  cout<<mPlayers[0]->getId()<<" wins the Match!"<<endl;
}
