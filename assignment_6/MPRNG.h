#pragma once

_Monitor MPRNG {
public:
  /// set seed
  MPRNG(unsigned int seed = 1009) {
    srand( seed );
  }
  /// set seed
  void seed(unsigned int seed) {
    srand( seed );
  }
  /// [0,UINT_MAX]
  unsigned int operator()() {
    return rand();
  }
  /// [0,u]
  unsigned int operator()( unsigned int u ) {
    return operator()() % (u + 1);
  }
  /// [l,u]
  unsigned int operator()( unsigned int l, unsigned int u ) {
    return operator()( u - l ) + l;
  }
};

extern MPRNG mprng;
