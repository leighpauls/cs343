#include "q1sortreducer.h"
#include "q1kvqueue.h"

#include <uC++.h>
#include <iostream>

using namespace std;

SortReducer::SortReducer(
    int id,
    int numReducers,
    uSemaphore* signal,
    const vector<Mapper*>& mappers) :
    Reducer(id, numReducers, signal, mappers) {}

SortReducer::~SortReducer() {}

void SortReducer::main() {
  map<string, int> wordCounts;
  vector<QueueState> queues;
  for (unsigned int i = 0; i < getMappers().size(); ++i) {
    // initialize queues to have a lexographic 0 previous word
    queues.push_back(QueueState(getMappers()[i], string("")));
  }

  while (!queues.empty()) {
    tryConsumeEvent(queues, wordCounts);
  }
  tryFlushWords(queues, wordCounts);
}

void SortReducer::tryConsumeEvent(
    vector<QueueState>& queues,
    map<string, int>& wordCounts) {
  // wait for something to do
  getSignal()->P();

  for (vector<QueueState>::iterator it = queues.begin();
       it != queues.end();
       ++it) {
    try {
      if (tryReduce(*it, wordCounts)) {
        tryFlushWords(queues, wordCounts);
        return;
      }
    } catch (KVQueue::EmptyAndClosed& e) {
      // remove the mapper
      queues.erase(it);
      // If i'm not the last to remove this mapper, let someone else remove it
      if (e.numThrown < getNumReducers()) {
        getSignal()->V();
      }
      return;
    }
  }
  // if I've made it here, then I didn't consume anything
  getSignal()->V();
}

/**
 * Try to remove an element from the mapper and reduce it
 * @param queue the mapper to try and reduce from
 * @param wordCounts the word count to amend
 * @returns true iff it reduced something
 * @throws KVQueue::EmptyAndClosed if the mapper is done
 */
bool SortReducer::tryReduce(QueueState& queue, map<string, int>& wordCounts) {
  Mapper::KeyValue pair;
  if (queue.mapper->mQueue->peekFront(&pair) ||
      (hash(pair.key) % getNumReducers()) != (unsigned int) getID()) {
    // I can't handle an element from this queue
    return false;
  }
  // this value belongs to me, take it away from the others
  queue.mapper->mQueue->popFront();
  queue.lastWord = pair.key;
  wordCounts[pair.key] += pair.value;
  return true;
}

void SortReducer::tryFlushWords(
    vector<QueueState>& queues,
    map<string, int>& wordCounts) {
  map<string, int>::iterator it = wordCounts.begin();
  while (it != wordCounts.end()) {
    for (unsigned int i = 0; i < queues.size(); ++i) {
      if (queues[i].lastWord < it->first) {
        // this word, and all after it can't be printed this time around
        goto finishedWords;
      }
    }
    // this word is <= all words to come, so print it
    osacquire(cout)<<it->first<<": "<<it->second<<endl;
    ++it;
  }
finishedWords:
  // remove all of the words that were printed
  wordCounts.erase(wordCounts.begin(), it);
}
