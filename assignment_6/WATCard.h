#pragma once

class WATCard {
 public:
  WATCard();
  // future watcard pointer
  typedef Future_ISM<WATCard *> FWATCard;

  void deposit( unsigned int amount );
  void withdraw( unsigned int amount );
  unsigned int getBalance();

 private:
  unsigned int mBalance;

  // prevent copying
  WATCard( const WATCard & );
  WATCard &operator=( const WATCard & );
};
