#include "q1reducer.h"
#include "q1kvqueue.h"

#include <iostream>
#include <uC++.h>

using namespace std;

Reducer::Reducer(
    int id,
    int numReducers,
    uSemaphore* signal,
    const vector<Mapper*>& mappers)
    : mId(id),
      mNumReducers(numReducers),
      mSignal(signal),
      mMappers(mappers) { }

Reducer::~Reducer() { }

/// DJB2 hash.
unsigned long Reducer::hash(const string& str) {
  unsigned long hash = 5381;
  for (unsigned int i = 0; i < str.size(); ++i) {
    hash = ((hash << 5) + hash) + str[i];
  }
  return hash;
}

int Reducer::getID() {
  return mId;
}
int Reducer::getNumReducers() {
  return mNumReducers;
}
uSemaphore* Reducer::getSignal() {
  return mSignal;
}
vector<Mapper*>& Reducer::getMappers() {
  return mMappers;
}

void Reducer::main() {
  for (;;) {
    // Wait for something to do
    mSignal->P();
    // ask all of the mappers for a value I can handle
    unsigned int numDone = 0;
    for (unsigned int i = 0; i < mMappers.size(); ++i) {
      try {
        Mapper::KeyValue curPair;
        if ((!mMappers[i]->mQueue->peekFront(&curPair)) &&
            (hash(curPair.key) % mNumReducers == (unsigned int) mId)) {
          // this values belongs to me, take it away from the others
          mMappers[i]->mQueue->popFront();
          reduce(curPair);
          break;
        }
      } catch (KVQueue::EmptyAndClosed& e) {
        numDone++;
      }
    }
    if (numDone == mMappers.size()) {
      // this was a termination cycle
      printCounts();
      return;
    }
    // Let another reducer try
    mSignal->V();
  }
}

void Reducer::reduce(Mapper::KeyValue& pair) {
  mWordCounts[pair.key] += pair.value;
}

void Reducer::printCounts() {
  for (map<string, int>::iterator it = mWordCounts.begin();
       it != mWordCounts.end();
       it++) {
    osacquire(cout)<<it->first<<": "<<it->second<<endl;
  }
}
