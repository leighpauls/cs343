#pragma once

#include <uSemaphore.h>
#include "q1mapper.h"

class KVQueue {
 public:
  struct EmptyAndClosed {
    int num_thrown;
    EmptyAndClosed(int num_thrown) : num_thrown(num_thrown) {}
  };
  KVQueue(int size);
  virtual ~KVQueue();
  void pushBack(const Mapper::KeyValue& item);
  Mapper::KeyValue popFront();

  /** Copy the front value to "val" without removing it from the queue. Return 0
   * on success and -1 if the queue is empty. Throw an EmptyAndClosed exception
   *  if the queue is closed and empty.
   */
  int peekFront(Mapper::KeyValue* val) throw(EmptyAndClosed);
  void close();

 private:
  /// Pops an item that has already been reserved from mFilledPositions
  Mapper::KeyValue popFrontWithP();

  /// places available in the queue
  uSemaphore mOpenPositions;
  /// places filled in the queue
  uSemaphore mFilledPositions;
  /// binary lock for removing form the queue
  uSemaphore mPopLock;

  bool mClosed;
  Mapper::KeyValue* mBuffer;
  int mFront, mBack, mSize;
  int mTimesPeekedEmptyAndClosed;
};
