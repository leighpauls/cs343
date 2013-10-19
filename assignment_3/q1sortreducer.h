#pragma once

#include "q1reducer.h"
#include "q1mapper.h"

#include <uSemaphore.h>
#include <vector>
#include <map>

using namespace std;

_Task SortReducer : public Reducer {
public:
  SortReducer(
      int id,
      int numReducers,
      uSemaphore* signal,
      const vector<Mapper*>& mappers);
  virtual ~SortReducer();

protected:
  virtual void main();

private:
  struct QueueState {
    Mapper* mapper;
    string lastWord;
    QueueState(Mapper* mapper, string lastWord)
        : mapper(mapper), lastWord(lastWord) {}
  };

  bool tryReduce(QueueState& queue, map<string, int>& wordCounts);
  void tryConsumeEvent(
      vector<QueueState>& queues,
      map<string, int>& wordCounts);
  void tryFlushWords(
      vector<QueueState>& queues,
      map<string, int>& wordCounts);
};
