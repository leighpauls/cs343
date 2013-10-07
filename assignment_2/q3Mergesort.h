
template<typename T> _Task Mergesort {
public:
  Mergesort(
      T values[],
      unsigned int low,
      unsigned int high,
      unsigned int depth);

  ~Mergesort();
private:
  T* mValues;
  unsigned int mLow, mHigh;
  unsigned int mDepth;
  T* mBuffer;
  bool mOwner;

  Mergesort(
      T values[],
      unsigned int low,
      unsigned int high,
      unsigned int depth,
      T buffer[]);

  void main();
  void sequentialSort(unsigned int low, unsigned int high);
  void mergeLists(unsigned int low, unsigned int mid, unsigned int high);
};

