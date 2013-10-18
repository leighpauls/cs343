#include "q1kvqueue.h"

#include <iostream>
#include <uC++.h>

using namespace std;

KVQueue::KVQueue(int size)
    : mOpenPositions(size),
      mFilledPositions(0),
      mPopLock(1),
      mClosed(false),
      mBuffer(new Mapper::KeyValue[size]),
      mFront(0),
      mBack(0),
      mSize(size),
      mTimesPeekedEmptyAndClosed(0) {}

KVQueue::~KVQueue() {
  delete [] mBuffer;
}

void KVQueue::pushBack(const Mapper::KeyValue& item) {
  // reserve a spot in the queue
  mOpenPositions.P();
  // place the item, assuming that there's only one producer
  mBuffer[mBack] = item;
  mBack++;
  if (mBack >= mSize) {
    mBack = 0;
  }
  // make the new item available to consume
  mFilledPositions.V();
}

Mapper::KeyValue KVQueue::popFront() {
  // reserve an item to take
  mFilledPositions.P();
  // lock removing from the buffer
  mPopLock.P();
  // get the value
  Mapper::KeyValue res = mBuffer[mFront];
  mFront += 1;
  if (mFront >= mSize) {
    mFront = 0;
  }
  // unlock the buffer
  mPopLock.V();
  // make the new space available
  mOpenPositions.V();
  return res;
}

int KVQueue::peekFront(Mapper::KeyValue* val) throw(EmptyAndClosed) {
  if (!mFilledPositions.TryP()) {
    if (mClosed) {
      mTimesPeekedEmptyAndClosed++;
      throw EmptyAndClosed(mTimesPeekedEmptyAndClosed);
    }
    return -1;
  }
  mPopLock.P();
  *val = mBuffer[mFront];
  mPopLock.V();
  // don't actually consume the value
  mFilledPositions.V();
  return 0;
}

void KVQueue::close() {
  mClosed = true;
}
