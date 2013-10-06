
template<typename T> _Task Mergesort {
public:
  Mergesort(
      T values[],
      unsigned int low,
      unsigned int high,
      unsigned int depth);

private:
  unsigned int mLow, mHigh;
  unsigned int mDepth;
  T* mValues;

  void main();
  void sequentialSort(unsigned int low, unsigned int high);
  void mergeLists(unsigned int low, unsigned int mid, unsigned int high);
};

