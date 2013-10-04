template<typename T> _Task Mergesort {
  public:
    Mergesort( T values[], unsigned int low, unsigned int high, unsigned int depth );
};

unsigned int uDefaultStackSize() {
    return 512 * 1000;        // set task stack-size to 512K
}

uProcessor p[ processors - 1 ] __attribute__(( unused )); // use "processors" kernel threads
