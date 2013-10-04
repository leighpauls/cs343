#pragma once

class Potato {
 public:
  Potato( unsigned int maxTicks = 10 );
  void reset( unsigned int maxTicks = 10 );
  bool countdown();
 private:
  unsigned int mTicksRemaining;
};

