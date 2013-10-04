#pragma once

#include "q1Player.h"

_Coroutine Umpire {
public:
  Umpire(Player::PlayerList &players);
  void set();
private:
  Player::PlayerList &mPlayers;

  void main();
};

