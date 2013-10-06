#include "q3Mergesort.h"

using namespace std;

template <typename T>
Mergesort<T>::Mergesort(
    T values[],
    unsigned int low,
    unsigned int high,
    unsigned int depth)
    : mValues(values), mLow(low), mHigh(high), mDepth(depth) {
}

template <typename T>
void Mergesort<T>::main() {
  if (mHigh - mLow <= 1) {
    // empty or single-item lists are already sorted
    return;
  }
  if (mDepth == 0) {
    // out of CPUs, sort sequentually from here
    sequentialSort(mLow, mHigh);
    return;
  }
  // branch and merge this sub-list
  unsigned int midPoint = (mLow + mHigh) / 2;
  {
    Mergesort<T> a(mValues, mLow, midPoint, mDepth-1);
    Mergesort<T> b(mValues, midPoint, mHigh, mDepth-1);
  }
  mergeLists(mLow, midPoint, mHigh);
}

/**
 * Merge 2 sorted lists
 */
template <typename T>
void Mergesort<T>::mergeLists(
    unsigned int low,
    unsigned int mid,
    unsigned int high) {
  // make a temp array to copy from
  T* source = new T[high - low];
  memcpy(source, &mValues[low], (high-low) * sizeof(T));

  // merge
  unsigned int lowIdx = low;
  unsigned int highIdx = mid;
  unsigned int destIdx = low;
  while (lowIdx != mid && highIdx != high) {
    if (source[lowIdx-low] < source[highIdx-low]) {
      mValues[destIdx] = source[lowIdx-low];
      lowIdx++;
    } else {
      mValues[destIdx] = source[highIdx-low];
      highIdx++;
    }
    destIdx++;
  }

  // remaing from low and high lists, one of these will copy 0 bytes
  memcpy(&mValues[destIdx], &source[lowIdx-low], (mid - lowIdx) * sizeof(T));
  memcpy(&mValues[destIdx], &source[highIdx-low], (high - highIdx) * sizeof(T));

  delete [] source;
}

template <typename T>
void Mergesort<T>::sequentialSort(
    unsigned int low,
    unsigned int high) {
  if (high - low <= 1) {
    // empty or single-item lists are already sorted
    return;
  }
  // TODO: check stack size
  unsigned int midPoint = (low + high) / 2;
  sequentialSort(low, midPoint);
  sequentialSort(midPoint, high);
  mergeLists(low, midPoint, high);
}

template _Task Mergesort<int>;
