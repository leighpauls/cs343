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
  // lock removing from the buffer
  mPopLock.P();
  // reserve an item to take
  mFilledPositions.P();
  // get the value
  Mapper::KeyValue res = mBuffer[mFront];
  mFront += 1;
  if (mFront >= mSize) {
    mFront = 0;
  }
  // make the new space available
  mOpenPositions.V();
  // unlock the buffer
  mPopLock.V();
  return res;
}

int KVQueue::peekFront(Mapper::KeyValue* val) throw(EmptyAndClosed) {
  mPopLock.P();
  if (!mFilledPositions.TryP()) {
    if (mClosed) {
      mTimesPeekedEmptyAndClosed++;
      mPopLock.V();
      throw EmptyAndClosed(mTimesPeekedEmptyAndClosed);
    }
    mPopLock.V();
    return -1;
  }
  *val = mBuffer[mFront];
  // don't actually consume the value
  mFilledPositions.V();
  mPopLock.V();
  return 0;
}

void KVQueue::close() {
  mClosed = true;
}
