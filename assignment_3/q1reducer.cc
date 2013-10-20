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
  // wait until all mappers are exausteed
  while (tryConsumeEvent()) { }
  // print the counts
  for (map<string, int>::iterator it = mWordCounts.begin();
       it != mWordCounts.end();
       it++) {
    osacquire(cout)<<it->first<<" : "<<it->second<<endl;
  }
}

bool Reducer::tryConsumeEvent() {
  // Wait for something to do
  mSignal->P();

  for (vector<Mapper*>::iterator it = mMappers.begin();
       it != mMappers.end();
       ++it) {
    // ask each mapper for a value I can handle
    try {
      if (tryReduce(*it)) {
        return true;
      }
    } catch (KVQueue::EmptyAndClosed& e) {
      // remove the mapper
      mMappers.erase(it);
      // If I'm not the last to remove this mapper, let someone else remove it
      if (e.numThrown < mNumReducers) {
        mSignal->V();
      }
      // quit if the mappers are exausted
      return !mMappers.empty();
    }
  }
  // if I've made it here I haven't consumed anything, let another reducer try
  mSignal->V();
  return true;
}

/**
 * Try to remove an element from the mapper and reduce it
 * @param mapper the mapper to try and reduce from
 * @returns true iff it reduced something
 * @throws KVQueue::EmptyAndClosed if the mapper is done
 */
bool Reducer::tryReduce(Mapper* mapper) {
  Mapper::KeyValue pair;
  if (mapper->mQueue->peekFront(&pair) ||
      (hash(pair.key) % mNumReducers) != (unsigned int) mId) {
    // I can't handle an element from this queue
    return false;
  }
  // this value belongs to me, take it away from the others
  mapper->mQueue->popFront();
  mWordCounts[pair.key] += pair.value;
  return true;
}

