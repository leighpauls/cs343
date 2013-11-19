
_Comonitor Printer {
  public:
    Printer( unsigned int NoOfPhil );
    void print( unsigned int id, Philosopher::States state );
    void print( unsigned int id, Philosopher::States state, unsigned int bite, unsigned int noodles );
};

