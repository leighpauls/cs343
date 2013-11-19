

_Task Philosopher {
  public:
    enum States { Thinking = 'T', Hungry = 'H', Eating ='E', Waiting = 'W', Finished = 'F' };
    Philosopher( unsigned int id, unsigned int noodles, Table &table, Printer &prt );
};
