#include "q1kvqueue.h"

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
  return popFrontWithP();
}

int KVQueue::peekFront(Mapper::KeyValue* val) throw(EmptyAndClosed) {
  if (!mFilledPositions.TryP()) {
    if (mClosed) {
      mTimesPeekedEmptyAndClosed++;
      throw EmptyAndClosed(mTimesPeekedEmptyAndClosed);
    }
    return -1;
  }
  *val = popFrontWithP();
  return 0;
}

Mapper::KeyValue KVQueue::popFrontWithP() {
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

void KVQueue::close() {
  mClosed = true;
}
