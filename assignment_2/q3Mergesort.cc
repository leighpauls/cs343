#include "q3Mergesort.h"
#include <stdexcept>

using namespace std;

template <typename T>
Mergesort<T>::Mergesort(
    T values[],
    unsigned int low,
    unsigned int high,
    unsigned int depth)
    : mValues(values), mLow(low), mHigh(high), mDepth(depth), mOwner(true) {
  if (low != 0) {
    throw runtime_error("using the wrong constructor for a division!");
  }
  mBuffer = new T[high];
}

// private construtor to save mBuffer, and precent memory thrashing
template <typename T>
Mergesort<T>::Mergesort(
    T values[],
    unsigned int low,
    unsigned int high,
    unsigned int depth,
    T buffer[])
    : mValues(values),
      mLow(low),
      mHigh(high),
      mDepth(depth),
      mBuffer(buffer),
      mOwner(false) {
}

template <typename T>
Mergesort<T>::~Mergesort() {
  if (mOwner) {
    delete [] mBuffer;
  }
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
    Mergesort<T> a(mValues, mLow, midPoint, mDepth-1, mBuffer);
    Mergesort<T> b(mValues, midPoint, mHigh, mDepth-1, mBuffer);
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
  // move the current values into the buffer
  memcpy(&mBuffer[low], &mValues[low], (high-low) * sizeof(T));

  // merge
  unsigned int lowIdx = low;
  unsigned int highIdx = mid;
  unsigned int destIdx = low;
  while (lowIdx != mid && highIdx != high) {
    if (mBuffer[lowIdx] < mBuffer[highIdx]) {
      mValues[destIdx] = mBuffer[lowIdx];
      lowIdx++;
    } else {
      mValues[destIdx] = mBuffer[highIdx];
      highIdx++;
    }
    destIdx++;
  }

  // remaing from low and high lists, one of these will copy 0 bytes
  memcpy(&mValues[destIdx], &mBuffer[lowIdx], (mid - lowIdx) * sizeof(T));
  memcpy(&mValues[destIdx], &mBuffer[highIdx], (high - highIdx) * sizeof(T));
}

template <typename T>
void Mergesort<T>::sequentialSort(
    unsigned int low,
    unsigned int high) {
  // verify();
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
