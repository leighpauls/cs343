#include "q1Player.h"
#include "q1Umpire.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

void usageAndExit(char* execName) {
  cout<<"Usage: "<<execName
        <<" number-of-players (between 2 and 20) [ random-seed ]"
        <<endl;
  exit(1);
}

void uMain::main() {
  if (argc != 2 && argc != 3) {
    usageAndExit(argv[0]);
  }

  // get the number of players and a possible seed
  unsigned int numPlayers = atoi(argv[1]);
  if (numPlayers < 2 || numPlayers > 20) {
    usageAndExit(argv[0]);
  }
  srand((argc == 3) ? atoi(argv[2]) : getpid());

  // 2 lists: 1 of all the players, and one of the players remaining in the game
  Player::PlayerList gamePlayers;
  Player::PlayerList allPlayers;
  Umpire umpire(gamePlayers);
  for (unsigned int i = 0; i < numPlayers; ++i) {
    allPlayers.push_back(new Player(umpire, i, gamePlayers));
    gamePlayers.push_back(allPlayers[i]);
  }

  // Start the match
  umpire.set();

  // free memory
  for (unsigned int i = 0; i < numPlayers; ++i) {
    delete allPlayers[i];
  }
}
