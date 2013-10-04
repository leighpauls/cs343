#include "q1Player.h"
#include "q1Umpire.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

void uMain::main() {
  unsigned int numPlayers;
  srand(getpid());
  stringstream ss;

  switch (argc) {
    case 3:
      // TODO: verify a valid int
      ss<<argv[2];
      int seed;
      ss>>seed;
      srand(seed);
    case 2:
      // TODO: verify a valid int greater than 1
      ss<<argv[1];
      ss>>numPlayers;
      break;
    default:
      cout<<"Invalid args, expected `hotpotato players [seed]'"<<endl;
      exit(1);
  }

  Player::PlayerList gamePlayers;
  Player::PlayerList allPlayers;
  Umpire umpire(gamePlayers);

  for (unsigned int i = 0; i < numPlayers; ++i) {
    allPlayers.push_back(new Player(umpire, i, gamePlayers));
    gamePlayers.push_back(allPlayers[i]);
  }

  umpire.set();

  // free memory
  for (unsigned int i = 0; i < numPlayers; ++i) {
    delete allPlayers[i];
  }
}
